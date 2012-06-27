/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include "tulip/Workspace.h"

#include <math.h>
#include <iostream>
#include <sstream>
#include <string>

#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsEffect>
#include <QtGui/QGraphicsSceneDragDropEvent>
#include <QtXml/QDomDocument>

#include <tulip/TulipMetaTypes.h>
#include <tulip/View.h>
#include <tulip/WorkspacePanel.h>
#include <tulip/TulipProject.h>
#include <tulip/TulipMimes.h>

#include <QtCore/QDebug>

#ifndef NDEBUG
#include <modeltest.h>
#endif /* NDEBUG */

#include "ui_Workspace.h"

using namespace tlp;

/*
  Helper storage class to ensure synchronization between panels list and model passed down to opened panels
  */
Workspace::Workspace(QWidget *parent)
  : QWidget(parent), _ui(new Ui::Workspace), _currentPanelIndex(0), _model(NULL), _pageCountLabel(NULL) {
  _ui->setupUi(this);
  _pageCountLabel = _ui->pagesLabel;
  _ui->workspaceContents->setCurrentWidget(_ui->startupPage);
  connect(_ui->startupButton,SIGNAL(clicked()),this,SIGNAL(addPanelRequest()));
  connect(_ui->exposeMode,SIGNAL(exposeFinished()),this,SLOT(hideExposeMode()));

  // This map allows us to know how much slots we have for each mode and which widget corresponds to those slots
  _modeToSlots[_ui->startupPage] = QVector<PlaceHolderWidget*>();
  _modeToSlots[_ui->singlePage] = QVector<PlaceHolderWidget*>() << _ui->singlePage;
  _modeToSlots[_ui->splitPage] = QVector<PlaceHolderWidget*>() << _ui->splitPagePanel1 << _ui->splitPagePanel2;
  _modeToSlots[_ui->split3Page] = QVector<PlaceHolderWidget*>() << _ui->split3PagePanel1 << _ui->split3PagePanel2 << _ui->split3PagePanel3;
  _modeToSlots[_ui->split32Page] = QVector<PlaceHolderWidget*>() << _ui->split32Panel1 << _ui->split32Panel2 << _ui->split32Panel3;
  _modeToSlots[_ui->gridPage] = QVector<PlaceHolderWidget*>() << _ui->gridPagePanel1 << _ui->gridPagePanel2 << _ui->gridPagePanel3 << _ui->gridPagePanel4;

  // This map allows us to know which widget can toggle a mode
  _modeSwitches[_ui->singlePage] = _ui->singleModeButton;
  _modeSwitches[_ui->splitPage] = _ui->splitModeButton;
  _modeSwitches[_ui->split3Page] = _ui->split3ModeButton;
  _modeSwitches[_ui->split32Page] = _ui->split32ModeButton;
  _modeSwitches[_ui->gridPage] = _ui->gridModeButton;
  updateAvailableModes();
}

Workspace::~Workspace() {
  foreach(WorkspacePanel* p, _panels) {
      delete p;
    }

  delete _ui;
}

void Workspace::setModel(tlp::GraphHierarchiesModel* model) {
  _model = model;

  if (_model != NULL) {
    foreach(WorkspacePanel* panel,_panels)
    panel->setGraphsModel(_model);
  }
}

QList<tlp::View*> Workspace::panels() const {
  QList<tlp::View*> result;
  foreach(WorkspacePanel* panel, _panels) {
    result.push_back(panel->view());
  }
  return result;
}

QString Workspace::panelTitle(tlp::WorkspacePanel* panel) const {
  int digit = 0;

  QRegExp regExp("^.*(?:<([^>])*>){1}$");
  foreach(WorkspacePanel* other, _panels) {
    if (other == panel)
      continue;

    if (other->viewName() == panel->viewName()) {
      if (regExp.exactMatch(other->windowTitle()))
        digit = std::max<int>(digit,regExp.cap(1).toInt());
      else
        digit = std::max<int>(digit,1);
    }
  }

  if (digit == 0) {
    return panel->viewName();
  }

  return panel->viewName() + " <" + QString::number(digit+1) + ">";
}

int Workspace::addPanel(tlp::View* view) {
  WorkspacePanel* panel = new WorkspacePanel(view);

  if (_model != NULL)
    panel->setGraphsModel(_model);

  panel->installEventFilter(this);
  panel->setWindowTitle(panelTitle(panel));
  connect(panel,SIGNAL(drawNeeded()),this,SLOT(viewNeedsDraw()));
  connect(panel,SIGNAL(destroyed(QObject*)),this,SLOT(panelDestroyed(QObject*)));

  // Add it to the list
  _panels.push_back(panel);

  // activate available modes
  updateAvailableModes();

  // If on startup mode, switch to single
  if (currentModeWidget() == _ui->startupPage) {
    switchToSingleMode();
  }
  else {
    updatePanels();
  }

  // Force the first panel's graph combo box update when underleying model has been updated.
  panel->viewGraphSet(view->graph());
  emit panelFocused(view);
  return _panels.size()-1;
}


void Workspace::delView(tlp::View* view) {
  WorkspacePanel* panel = NULL;
  foreach(WorkspacePanel* it, _panels) {
    if (it->view() == view) {
      panel = it;
      break;
    }
  }
  delete panel;
}

void Workspace::panelDestroyed(QObject* obj) {
  WorkspacePanel* panel = static_cast<WorkspacePanel*>(obj);
  int removeCount = _panels.removeAll(panel);

  if (removeCount==0)
    return;


  // To prevent segfaults due to Qt's event queue handling when deleting views, we reset the placeholder widget that contained this panel
  foreach(QWidget* mode,_modeToSlots.keys()) {
    foreach(PlaceHolderWidget* p, _modeToSlots[mode]) {
      if (p->widget() == panel)
        p->resetWidget();
    }
  }

  if (currentModeWidget() == _ui->exposePage)
    return;

  updateAvailableModes();

  if (!_modeSwitches[currentModeWidget()]->isVisible()) {
    int maxSize = 0;
    QWidget* fallbackMode = _ui->startupPage;

    // Current mode is not available, fallback to the largest available mode
    foreach(QWidget* it, _modeToSlots.keys()) {
      if (_panels.size() >= _modeToSlots[it].size() && _modeToSlots[it].size() > maxSize) {
        maxSize = _modeToSlots[it].size();
        fallbackMode = it;
      }
    }
    switchWorkspaceMode(fallbackMode);
  }

  if (currentModeWidget() == _ui->startupPage) {
    return;
  }

  updatePanels();
}

void Workspace::viewNeedsDraw() {
  WorkspacePanel* panel = static_cast<WorkspacePanel*>(sender());
  panel->toggleProgressMode(true);
  panel->view()->draw(panel);
  panel->toggleProgressMode(false);
}

void Workspace::switchToStartupMode() {
  switchWorkspaceMode(_ui->startupPage);
}
void Workspace::switchToSingleMode() {
  switchWorkspaceMode(_ui->singlePage);
}

void Workspace::setSingleModeSwitch(QWidget* w) {
  _modeSwitches[_ui->singlePage] = w;
}
void Workspace::switchToSplitMode() {
  switchWorkspaceMode(_ui->splitPage);
}

void Workspace::setSplitModeSwitch(QWidget *w) {
  _modeSwitches[_ui->splitPage] = w;
}
void Workspace::switchToSplit3Mode() {
  switchWorkspaceMode(_ui->split3Page);
}

void Workspace::setSplit3ModeSwitch(QWidget *w) {
  _modeSwitches[_ui->split3Page] = w;
}
void Workspace::switchToSplit32Mode() {
  switchWorkspaceMode(_ui->split32Page);
}

void Workspace::setSplit32ModeSwitch(QWidget *w) {
  _modeSwitches[_ui->split32Page] = w;
}
void Workspace::switchToGridMode() {
  switchWorkspaceMode(_ui->gridPage);
}

void Workspace::setGridModeSwitch(QWidget*w) {
  _modeSwitches[_ui->gridPage] = w;
}

void Workspace::switchWorkspaceMode(QWidget *page) {
  _ui->workspaceContents->setCurrentWidget(page);
  _ui->bottomFrame->setEnabled(page != _ui->startupPage);
  updatePanels();
}

void Workspace::updatePageCountLabel() {
  int index=0, total=0;

  if (currentSlotsCount() != 0) {
    index = _currentPanelIndex+1;
    total = ceil(_panels.size() * 1./currentSlotsCount());

  }

  _pageCountLabel->setText(QString::number(index) + " / " + QString::number(total));
}

QWidget* Workspace::currentModeWidget() const {
  return _ui->workspaceContents->currentWidget();
}

QVector<PlaceHolderWidget*> Workspace::currentModeSlots() const {
  return _modeToSlots[currentModeWidget()];
}

unsigned int Workspace::currentSlotsCount() const {
  return currentModeSlots().size();
}

void Workspace::updateAvailableModes() {
  foreach(QWidget* page, _modeSwitches.keys()) {
    _modeSwitches[page]->setVisible(_panels.size() >= _modeToSlots[page].size());
  }

  bool enableNavigation = _panels.size()>0;
  _ui->nextPageButton->setEnabled(enableNavigation);
  _ui->previousPageButton->setEnabled(enableNavigation);
  _ui->exposeButton->setEnabled(enableNavigation);
}

void Workspace::updatePanels() {
  foreach(QWidget* mode,_modeToSlots.keys()) {
    if (mode == currentModeWidget())
      continue;

    QVector<PlaceHolderWidget*> panelSlots = _modeToSlots[mode];
    foreach(PlaceHolderWidget* panel, panelSlots) {
      panel->setWidget(NULL);
    }
  }

  if (_currentPanelIndex<0)
    _currentPanelIndex=0;

  if (_currentPanelIndex > _panels.size()-currentSlotsCount())
    _currentPanelIndex = _panels.size()-currentSlotsCount();

//   Fill up slots according to the current index until there is no panel to show
  int i = _currentPanelIndex;
  foreach (PlaceHolderWidget* slt, currentModeSlots()) {
    if (i >= _panels.size()) {
      slt->setWidget(NULL);
    }
    else if (slt->widget() != _panels[i]) {
      slt->setWidget(NULL);
    }

    i++;
  }

  i = _currentPanelIndex;
  foreach (PlaceHolderWidget* slt, currentModeSlots()) {
    if (i >= _panels.size())
      break;
    else if (slt->widget() != _panels[i]) {
      slt->setWidget(_panels[i]);
    }

    i++;
  }

  updatePageCountLabel();
}

void Workspace::nextPage() {
  _currentPanelIndex++;
  updatePanels();
}

void Workspace::previousPage() {
  _currentPanelIndex--;
  updatePanels();
}

void Workspace::setActivePanel(tlp::View* view) {
  int newIndex = panels().indexOf(view);
  _currentPanelIndex = newIndex;
  updatePanels();
}

WorkspacePanel* Workspace::panelForScene(QObject *obj) {
  WorkspacePanel* p = NULL;
  foreach(WorkspacePanel* panel, _panels) {
    if(panel->view()->graphicsView()->scene() == obj) {
      p = panel;
      break;
    }
  }
  return p;
}

bool Workspace::eventFilter(QObject* obj, QEvent* ev) {
  if (ev->type() == QEvent::ChildAdded || ev->type() == 70) {
    QObject* childObj = static_cast<QChildEvent*>(ev)->child();
    childObj->installEventFilter(this);
    QGraphicsView* graphicsView = dynamic_cast<QGraphicsView*>(childObj);

    if (graphicsView != NULL && graphicsView->scene() != NULL)  {
      graphicsView->scene()->installEventFilter(this);
    }
  }
  else if (ev->type() == QEvent::ChildRemoved) {
    QObject* childObj = static_cast<QChildEvent*>(ev)->child();
    childObj->removeEventFilter(this);
    QGraphicsView* graphicsView = dynamic_cast<QGraphicsView*>(childObj);

    if (graphicsView != NULL && graphicsView->scene())  {
      graphicsView->scene()->removeEventFilter(this);
    }
  }
  else if (ev->type() == QEvent::FocusIn) {
    if (dynamic_cast<QGraphicsView*>(obj) != NULL) {
      tlp::WorkspacePanel* panel = static_cast<tlp::WorkspacePanel*>(obj->parent());
      emit panelFocused(panel->view());
    }
  }
  else if(ev->type() == QEvent::DragEnter || ev->type() == QEvent::DragMove) {
    const QMimeData* mimedata = static_cast<QDragMoveEvent*>(ev)->mimeData();
    return handleDragEnterEvent(ev, mimedata);
  }
  else if(ev->type() == QEvent::Drop) {
    const QMimeData* mimedata = static_cast<QDropEvent*>(ev)->mimeData();

    QWidget* w = dynamic_cast<QWidget*>(obj);
    WorkspacePanel* p = NULL;

    while(p == NULL && w != NULL) {
      p = dynamic_cast<WorkspacePanel*>(w);
      w = w->parentWidget();
    }

    return handleDropEvent(mimedata, p);
  }
  else if (ev->type() == QEvent::GraphicsSceneDragEnter || ev->type() == QEvent::GraphicsSceneDragMove) {
    QGraphicsSceneDragDropEvent* dragDropEv = static_cast<QGraphicsSceneDragDropEvent*>(ev);
    const QMimeData* mimedata = dragDropEv->mimeData();

    if (panelForScene(obj) != dragDropEv->source() && dynamic_cast<const PanelMimeType*>(mimedata) != NULL) {
      panelForScene(obj)->setOverlayMode(true);
    }

    return handleDragEnterEvent(ev, mimedata);
  }
  else if (ev->type() == QEvent::GraphicsSceneDrop) {
    const QMimeData* mimedata = static_cast<QGraphicsSceneDragDropEvent*>(ev)->mimeData();
    panelForScene(obj)->setOverlayMode(false);
    return handleDropEvent(mimedata, panelForScene(obj));
  }
  else if (ev->type() == QEvent::GraphicsSceneDragLeave) {
    panelForScene(obj)->setOverlayMode(false);
  }

  return false;
}

void Workspace::dragEnterEvent(QDragEnterEvent* event) {
  handleDragEnterEvent(event, event->mimeData());
}

void Workspace::dropEvent(QDropEvent* event) {
  handleDropEvent(event->mimeData());
}

bool Workspace::handleDragEnterEvent(QEvent* e, const QMimeData* mimedata) {
  if(dynamic_cast<const GraphMimeType*>(mimedata) != NULL || dynamic_cast<const PanelMimeType*>(mimedata) != NULL || dynamic_cast<const AlgorithmMimeType*>(mimedata) !=  NULL) {
    e->accept();
    return true;
  }

  return false;
}

bool Workspace::handleDropEvent(const QMimeData* mimedata, WorkspacePanel* panel) {
  const GraphMimeType* graphMime = dynamic_cast<const GraphMimeType*>(mimedata);
  const PanelMimeType* panelMime = dynamic_cast<const PanelMimeType*>(mimedata);
  const AlgorithmMimeType* algorithmMime = dynamic_cast<const AlgorithmMimeType*>(mimedata);

  if (panel == NULL && graphMime == NULL)
    return false;

  if (graphMime != NULL && graphMime->graph()) {
    if (panel != NULL)
      panel->view()->setGraph(graphMime->graph());
    else
      emit(addPanelRequest(graphMime->graph()));
  }

  else if (panelMime) {
    if(panel) {
      _panels.swap(_panels.indexOf(panel), _panels.indexOf(panelMime->panel()));
      updatePanels();
    }
  }

  else if (algorithmMime) {
    algorithmMime->run(panel->view()->graph());
  }

  return graphMime != NULL || panelMime != NULL || algorithmMime != NULL;
}

void Workspace::expose(bool f) {
  if (f)
    showExposeMode();
  else
    hideExposeMode();
}

void Workspace::showExposeMode() {
  if (_ui->workspaceContents->currentWidget() == _ui->exposePage)
    return;

  _oldWorkspaceMode = currentModeWidget();
  foreach(QWidget* s, _modeSwitches.values()) {
    s->hide();
  }
  _ui->nextPageButton->setEnabled(false);
  _ui->previousPageButton->setEnabled(false);

  QVector<WorkspacePanel*> panels;
  foreach(WorkspacePanel* p, _panels) {
    panels << p;
  }
  _ui->exposeMode->setData(panels,_currentPanelIndex);
  _ui->workspaceContents->setCurrentWidget(_ui->exposePage);
}

void Workspace::uncheckExposeButton() {
  _ui->exposeButton->setChecked(false);
}

void Workspace::hideExposeMode() {
  _ui->exposeButton->setChecked(false);
  QVector<WorkspacePanel*> newPanels = _ui->exposeMode->panels();
  _panels.clear();
  foreach(WorkspacePanel* p, newPanels)
  _panels.push_back(p);
  _currentPanelIndex = _ui->exposeMode->currentPanelIndex();
  switchWorkspaceMode(suitableMode(_oldWorkspaceMode));
  updatePageCountLabel();
}

QWidget* Workspace::suitableMode(QWidget* oldMode) {
  updateAvailableModes();

  if (_modeSwitches.contains(oldMode) && _modeSwitches[oldMode]->isEnabled())
    return oldMode;

  int maxSlots = 0;
  QWidget* result = _ui->startupPage;
  foreach(QWidget* mode, _modeToSlots.keys()) {
    int slotCount = _modeToSlots[mode].size();

    if (slotCount <= _panels.size() && slotCount > maxSlots) {
      maxSlots = slotCount;
      result = mode;
    }
  }
  return result;
}

/*
  Project serialization
  */
void Workspace::writeProject(TulipProject* project, QMap<Graph *, QString> rootIds, tlp::PluginProgress* progress) {
  project->removeAllDir("views");
  int i=0;
  foreach(View* v, panels()) {
    progress->progress(i,panels().size());
    QString path = "views/" + QString::number(i);
    project->mkpath(path);
    Graph* g = v->graph();
    QIODevice* viewDescFile = project->fileStream(path + "/view.xml");
    QDomDocument doc;
    QDomElement root = doc.createElement("view");
    root.setAttribute("name",v->name().c_str());
    root.setAttribute("root",rootIds[g->getRoot()]);
    root.setAttribute("id",QString::number(g->getId()));
    QDomElement data = doc.createElement("data");
    std::stringstream dataStr;
    DataSet::write(dataStr,v->state());
    QDomText dataText = doc.createTextNode(dataStr.str().c_str());
    data.appendChild(dataText);
    root.appendChild(data);
    doc.appendChild(root);
    viewDescFile->write(doc.toString().toAscii());
    viewDescFile->close();
    delete viewDescFile;
    i++;
  }
  QDomDocument doc;
  QDomElement root = doc.createElement("workspace");
  root.setAttribute("current",_currentPanelIndex);
  root.setAttribute("mode",currentSlotsCount());
  doc.appendChild(root);
  project->removeFile("/workspace.xml");
  QIODevice* workspaceXml = project->fileStream("/workspace.xml");
  workspaceXml->write(doc.toString().toAscii());
  workspaceXml->close();
  delete workspaceXml;
}

void Workspace::readProject(TulipProject* project, QMap<QString, Graph *> rootIds, PluginProgress* progress) {
  QStringList entries = project->entryList("views",QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
  int step = 0,max_step = entries.size();
  foreach(QString entry, entries) {
    progress->progress(step++,max_step);
    QDomDocument doc;
    QIODevice* xmlFile = project->fileStream("views/" + entry + "/view.xml");
    doc.setContent(xmlFile);
    xmlFile->close();
    delete xmlFile;

    QDomElement root = doc.documentElement();
    QString viewName = root.attribute("name");
    QString rootId = root.attribute("root");
    QString id = root.attribute("id");
    QString data;
    QDomNodeList children = root.childNodes();

    for (int i=0; i<children.size(); ++i) {
      QDomNode n = children.at(i);
      QDomElement child = n.toElement();

      if (child.isNull())
        continue;

      if (child.tagName() == "data") {
        data = child.text();
        break;
      }
    }

    View* view = PluginLister::instance()->getPluginObject<View>(viewName.toStdString(),NULL);

    if (view == NULL)
      continue;

    view->setupUi();
    Graph* rootGraph = rootIds[rootId];
    assert(rootGraph);
    Graph* g = rootGraph->getSubGraph(id.toInt());

    if (g == NULL)
      g = rootGraph;

    view->setGraph(g);
    DataSet dataSet;
    std::istringstream iss(data.toStdString());
    DataSet::read(iss,dataSet);
    view->setState(dataSet);
    addPanel(view);
  }

  QIODevice* workspaceXml = project->fileStream("/workspace.xml");

  if (workspaceXml == NULL)
    return;

  QDomDocument doc;
  doc.setContent(workspaceXml);
  QDomElement root = doc.documentElement();
  int current = root.attribute("current","0").toInt();
  int mode = root.attribute("mode","-1").toInt();
  foreach(QWidget* modeWidget, _modeToSlots.keys()) {
    if (_modeToSlots[modeWidget].size() == mode) {
      if (current > 0 && current < _panels.size())
        setActivePanel(_panels[current]->view());

      switchWorkspaceMode(modeWidget);
    }
  }
}

void Workspace::setBottomFrameVisible(bool f) {
  _ui->bottomFrame->setVisible(f);
}

void Workspace::setPageCountLabel(QLabel *l) {
  _pageCountLabel = l;
}

bool Workspace::isBottomFrameVisible() const {
  return _ui->bottomFrame->isVisible();
}
