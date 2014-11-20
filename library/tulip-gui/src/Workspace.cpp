/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include <QPainter>
#include <QPaintEvent>
#include <QGraphicsView>
#include <QGraphicsEffect>
#include <QGraphicsSceneDragDropEvent>
#include <QDomDocument>
#include <QDebug>

#include <tulip/TulipMetaTypes.h>
#include <tulip/View.h>
#include <tulip/WorkspacePanel.h>
#include <tulip/TulipProject.h>
#include <tulip/TulipMimes.h>
#include <tulip/GraphHierarchiesModel.h>

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

#include "ui_Workspace.h"

using namespace tlp;

/*
  Helper storage class to ensure synchronization between panels list and model passed down to opened panels
  */
Workspace::Workspace(QWidget *parent)
  : QWidget(parent), _ui(new Ui::Workspace), _currentPanelIndex(0), _oldWorkspaceMode(NULL), _focusedPanel(NULL),
    _focusedPanelHighlighting(false), _model(NULL), _pageCountLabel(NULL), _autoCenterViews(false) {
  _ui->setupUi(this);
  _ui->startupMainFrame->hide();
  _pageCountLabel = _ui->pagesLabel;
  _ui->workspaceContents->setCurrentWidget(_ui->startupPage);
  connect(_ui->startupButton,SIGNAL(clicked()),this,SIGNAL(addPanelRequest()));
  connect(_ui->importButton,SIGNAL(clicked()),this,SIGNAL(importGraphRequest()));
  connect(_ui->exposeMode,SIGNAL(exposeFinished()),this,SLOT(hideExposeMode()));

  // This map allows us to know how much slots we have for each mode and which widget corresponds to those slots
  _modeToSlots[_ui->startupPage] = QVector<PlaceHolderWidget*>();
  _modeToSlots[_ui->singlePage] = QVector<PlaceHolderWidget*>() << _ui->singlePage;
  _modeToSlots[_ui->splitPage] = QVector<PlaceHolderWidget*>() << _ui->splitPagePanel1 << _ui->splitPagePanel2;
  _modeToSlots[_ui->splitPageHorizontal] = QVector<PlaceHolderWidget*>() << _ui->splitPageHorizontalPanel1 << _ui->splitPageHorizontalPanel2;
  _modeToSlots[_ui->split3Page] = QVector<PlaceHolderWidget*>() << _ui->split3PagePanel1 << _ui->split3PagePanel2 << _ui->split3PagePanel3;
  _modeToSlots[_ui->split32Page] = QVector<PlaceHolderWidget*>() << _ui->split32Panel1 << _ui->split32Panel2 << _ui->split32Panel3;
  _modeToSlots[_ui->split33Page] = QVector<PlaceHolderWidget*>() << _ui->split33Panel1 << _ui->split33Panel2 << _ui->split33Panel3;
  _modeToSlots[_ui->gridPage] = QVector<PlaceHolderWidget*>() << _ui->gridPagePanel1 << _ui->gridPagePanel2 << _ui->gridPagePanel3 << _ui->gridPagePanel4;
  _modeToSlots[_ui->sixPage] = QVector<PlaceHolderWidget*>() << _ui->sixMode1 << _ui->sixMode2 << _ui->sixMode3 << _ui->sixMode4 << _ui->sixMode5 << _ui->sixMode6;

  // This map allows us to know which widget can toggle a mode
  _modeSwitches[_ui->singlePage] = _ui->singleModeButton;
  _modeSwitches[_ui->splitPage] = _ui->splitModeButton;
  _modeSwitches[_ui->splitPageHorizontal] = _ui->splitHorizontalModeButton;
  _modeSwitches[_ui->split3Page] = _ui->split3ModeButton;
  _modeSwitches[_ui->split32Page] = _ui->split32ModeButton;
  _modeSwitches[_ui->split33Page] = _ui->split33ModeButton;
  _modeSwitches[_ui->gridPage] = _ui->gridModeButton;
  _modeSwitches[_ui->sixPage] = _ui->sixModeButton;
  updateAvailableModes();
}

Workspace::~Workspace() {
  foreach(WorkspacePanel* p, _panels) {
    disconnect(p,SIGNAL(destroyed(QObject*)),this,SLOT(panelDestroyed(QObject*)));
    delete p;
  }

  delete _ui;
}

void Workspace::setModel(tlp::GraphHierarchiesModel* model) {
  if (_model != NULL) {
    disconnect(_model,SIGNAL(currentGraphChanged(tlp::Graph*)),this,SLOT(updateStartupMode()));
  }

  _model = model;

  if (_model != NULL) {
    foreach(WorkspacePanel* panel,_panels)
    panel->setGraphsModel(_model);
    connect(_model,SIGNAL(currentGraphChanged(tlp::Graph*)),this,SLOT(updateStartupMode()));
  }
}

tlp::GraphHierarchiesModel* Workspace::graphModel() const {
  return _model;
}

void Workspace::closeAll() {
  foreach(WorkspacePanel* p, _panels) {
    delete p; //beware: the destroyed signal is connected to panelDestroyed
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

  panel->setWindowTitle(panelTitle(panel));
  connect(panel,SIGNAL(drawNeeded()),this,SLOT(viewNeedsDraw()));
  connect(panel,SIGNAL(swapWithPanels(WorkspacePanel*)),this,SLOT(swapPanelsRequested(WorkspacePanel*)));
  connect(panel,SIGNAL(destroyed(QObject*)),this,SLOT(panelDestroyed(QObject*)));
  view->graphicsView()->installEventFilter(this);

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

  // Force the first panel's graph combo box update when underlaying model has been updated.
  panel->viewGraphSet(view->graph());
  setFocusedPanel(panel);
  return _panels.size()-1;
}

void Workspace::delView(tlp::View* view) {
  foreach(WorkspacePanel* it, _panels) {
    if (it->view() == view) {
      delete it;
      return;
    }
  }
}

void Workspace::panelDestroyed(QObject* obj) {
  WorkspacePanel* panel = static_cast<WorkspacePanel*>(obj);

  if (panel == _focusedPanel)
    _focusedPanel = NULL;

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

  updatePanels();
}

void Workspace::viewNeedsDraw() {
  WorkspacePanel* panel = static_cast<WorkspacePanel*>(sender());

  if (_autoCenterViews) {
    // we assume graph has changed
    panel->view()->centerView(true);
  }
  else
    panel->view()->draw();
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

void Workspace::switchToSplitHorizontalMode() {
  switchWorkspaceMode(_ui->splitPageHorizontal);
}

void Workspace::setSplitHorizontalModeSwitch(QWidget *w) {
  _modeSwitches[_ui->splitPageHorizontal] = w;
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
void Workspace::switchToSplit33Mode() {
  switchWorkspaceMode(_ui->split33Page);
}

void Workspace::setSplit33ModeSwitch(QWidget *w) {
  _modeSwitches[_ui->split33Page] = w;
}
void Workspace::switchToGridMode() {
  switchWorkspaceMode(_ui->gridPage);
}

void Workspace::setGridModeSwitch(QWidget*w) {
  _modeSwitches[_ui->gridPage] = w;
}

void Workspace::switchToSixMode() {
  switchWorkspaceMode(_ui->sixPage);
}

void Workspace::setSixModeSwitch(QWidget* w) {
  _modeSwitches[_ui->sixPage] = w;
}

void Workspace::switchWorkspaceMode(QWidget *page) {
  if (currentModeWidget() == page)
    return;

  _ui->workspaceContents->setCurrentWidget(page);
  _ui->bottomFrame->setEnabled(page != _ui->startupPage);
  updateStartupMode();
  updatePanels();
}

void Workspace::updatePageCountLabel() {
  int current = _currentPanelIndex+1;
  int total = _panels.size()-currentSlotsCount()+1;

  _pageCountLabel->setText(QString::number(current) + " / " + QString::number(total));
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
    _modeSwitches[page]->setEnabled(_panels.size() >= _modeToSlots[page].size());
  }

  bool enableNavigation = !_panels.empty();
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

  if ((uint)_currentPanelIndex > _panels.size()-currentSlotsCount())
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

  if (currentModeWidget() == _ui->startupPage) {
    updatePageCountLabel();
    return;
  }

  if (!_modeSwitches[currentModeWidget()]->isEnabled()) {
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

  updatePageCountLabel();
}

void Workspace::nextPage() {
  if(_ui->nextPageButton->isEnabled()) {
    _currentPanelIndex++;
    updatePanels();
  }
}

void Workspace::previousPage() {
  if(_ui->previousPageButton->isEnabled()) {
    _currentPanelIndex--;
    updatePanels();
  }
}

void Workspace::setExposeModeSwitch(QPushButton* b) {
  _ui->exposeButton = b;
}

void Workspace::setActivePanel(tlp::View* view) {
  int newIndex = panels().indexOf(view);
  _currentPanelIndex = newIndex;
  updatePanels();
}

void Workspace::setGraphForFocusedPanel(tlp::Graph* g) {
  if (_focusedPanel && _focusedPanel->isGraphSynchronized() &&
      _focusedPanel->view()->graph() != g)
    _focusedPanel->view()->setGraph(g);
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
  if (ev->type() == QEvent::ChildRemoved) {
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
      setFocusedPanel(panel);
    }
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
  if(dynamic_cast<const GraphMimeType*>(mimedata) != NULL ) {
    e->accept();
    return true;
  }

  return false;
}

bool Workspace::handleDropEvent(const QMimeData* mimedata) {
  const GraphMimeType* graphMime = dynamic_cast<const GraphMimeType*>(mimedata);

  if (graphMime == NULL)
    return false;

  if (graphMime != NULL && graphMime->graph()) {
    emit(addPanelRequest(graphMime->graph()));
    return true;
  }

  return false;
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
  if (currentModeWidget() != _ui->exposePage)
    return;

  _ui->exposeButton->setChecked(false);
  QVector<WorkspacePanel*> newPanels = _ui->exposeMode->panels();
  _panels.clear();
  foreach(WorkspacePanel* p, newPanels)
  _panels.push_back(p);
  _currentPanelIndex = _ui->exposeMode->currentPanelIndex();

  if (!_ui->exposeMode->isSwitchToSingleMode()) {
    switchWorkspaceMode(suitableMode(_oldWorkspaceMode));
  }
  else {
    updateAvailableModes();
    switchToSingleMode();
  }

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
    viewDescFile->write(doc.toString().toLatin1());
    viewDescFile->close();
    delete viewDescFile;
    i++;
  }
  QDomDocument doc;
  QDomElement root = doc.createElement("workspace");
  root.setAttribute("current",_currentPanelIndex);
  root.setAttribute("mode",currentSlotsCount());

  if (currentModeWidget() == _ui->splitPage) {
    root.setAttribute("modeWidget","splitPage");
  }
  else if (currentModeWidget() == _ui->splitPageHorizontal) {
    root.setAttribute("modeWidget","splitPageHorizontal");
  }
  else if (currentModeWidget() == _ui->split3Page) {
    root.setAttribute("modeWidget","split3Page");
  }
  else if (currentModeWidget() == _ui->split32Page) {
    root.setAttribute("modeWidget","split32Page");
  }
  else if (currentModeWidget() == _ui->split33Page) {
    root.setAttribute("modeWidget","split33Page");
  }

  doc.appendChild(root);
  project->removeFile("/workspace.xml");
  QIODevice* workspaceXml = project->fileStream("/workspace.xml");
  workspaceXml->write(doc.toString().toLatin1());
  workspaceXml->close();
  delete workspaceXml;
}

void Workspace::readProject(TulipProject* project, QMap<QString, Graph *> rootIds, PluginProgress* progress) {
  QStringList entries = project->entryList("views",QDir::Dirs | QDir::NoDot | QDir::NoDotDot, QDir::Name);
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
    Graph* g = rootGraph->getDescendantGraph(id.toInt());

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
  workspaceXml->close();
  delete workspaceXml;
  QDomElement root = doc.documentElement();
  int current = root.attribute("current","0").toInt();
  int mode = root.attribute("mode","-1").toInt();
  foreach(QWidget* modeWidget, _modeToSlots.keys()) {
    if (_modeToSlots[modeWidget].size() == mode) {
      if (current > 0 && current < _panels.size())
        setActivePanel(_panels[current]->view());

      QString modeWidgetName = root.attribute("modeWidget");

      if (!modeWidgetName.isEmpty() && (mode == 2 || mode ==3)) {
        if (modeWidgetName == "splitPage") {
          switchToSplitMode();
        }
        else if (modeWidgetName == "splitPageHorizontal") {
          switchToSplitHorizontalMode();
        }
        else if (modeWidgetName == "split3Page") {
          switchToSplit3Mode();
        }
        else if (modeWidgetName == "split32Page") {
          switchToSplit32Mode();
        }
        else {
          switchToSplit33Mode();
        }
      }
      else {
        switchWorkspaceMode(modeWidget);
      }
    }
  }
}

void Workspace::setBottomFrameVisible(bool f) {
  _ui->bottomFrame->setVisible(f);
}

void Workspace::setPageCountLabel(QLabel *l) {
  _pageCountLabel = l;
}

void Workspace::redrawPanels(bool center) {
  foreach(WorkspacePanel* panel, _panels) {
    if (center)
      panel->view()->centerView();
    else
      panel->view()->draw();
  }
}

void Workspace::setAutoCenterPanelsOnDraw(bool f) {
  _autoCenterViews = f;
}

bool Workspace::isBottomFrameVisible() const {
  return _ui->bottomFrame->isVisible();
}

void Workspace::swapPanelsRequested(WorkspacePanel* panel) {
  WorkspacePanel* sourcePanel = dynamic_cast<WorkspacePanel*>(sender());

  if(sourcePanel) {
    _panels.swap(_panels.indexOf(sourcePanel), _panels.indexOf(panel));
    updatePanels();
  }
}

void Workspace::updateStartupMode() {
  if (currentModeWidget() == _ui->startupPage && _model != NULL) {
    _ui->startupImportFrame->setVisible(_model->empty());
    _ui->startupMainFrame->setVisible(!_model->empty());
  }
}

// enable/disable highlight of focused panel
void Workspace::setFocusedPanelHighlighting(bool h) {
  _focusedPanelHighlighting = h;

  if (_focusedPanel)
    _focusedPanel->setHighlightMode(h);
}

// update focused panel
void Workspace::setFocusedPanel(WorkspacePanel* panel) {
  if (_focusedPanel) {
    if (_focusedPanelHighlighting)
      _focusedPanel->setHighlightMode(false);

    disconnect(_focusedPanel, SIGNAL(changeGraphSynchronization(bool)),
               this, SLOT(changeFocusedPanelSynchronization(bool)));
  }

  _focusedPanel = panel;
  connect(_focusedPanel, SIGNAL(changeGraphSynchronization(bool)),
          this, SLOT(changeFocusedPanelSynchronization(bool)));

  if (_focusedPanelHighlighting)
    _focusedPanel->setHighlightMode(true);

  emit panelFocused(panel->view());

  if (_focusedPanel->isGraphSynchronized())
    emit focusedPanelSynchronized();
}

void Workspace::changeFocusedPanelSynchronization(bool s) {
  if (s)
    emit focusedPanelSynchronized();
}



