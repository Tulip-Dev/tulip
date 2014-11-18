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

#include "tulip/WorkspacePanel.h"
#include "ui_WorkspacePanel.h"

#include <QPropertyAnimation>
#include <QGraphicsView>
#include <QCloseEvent>
#include <QPushButton>
#include <QApplication>
#include <QGraphicsProxyWidget>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QTabWidget>
#include <QGraphicsSceneContextMenuEvent>
#include <QScrollBar>
#include <QHBoxLayout>

#include <tulip/TulipMetaTypes.h>
#include <tulip/ProcessingAnimationItem.h>
#include <tulip/Interactor.h>
#include <tulip/ForEach.h>
#include <tulip/View.h>
#include <tulip/Graph.h>
#include <tulip/GraphHierarchiesModel.h>
#include <tulip/TulipMimes.h>
#include <tulip/TlpQtTools.h>


using namespace tlp;

// helper class
class ProgressItem: public QGraphicsObject {
  ProcessingAnimationItem* _animation;
public:
  ProgressItem(QGraphicsScene* parentScene): QGraphicsObject() {
    _animation = new ProcessingAnimationItem(QPixmap(":/tulip/gui/ui/process-working.png"),QSize(64,64),this);
    _animation->setZValue(5);
    parentScene->addItem(_animation);
  }

  virtual ~ProgressItem() {
    delete _animation;
  }

  QRectF boundingRect() const {
    return QRectF();
  }

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setPen(QColor(255,255,255));
    painter->setBrush(QColor(255,255,255,170));
    painter->drawRect(scene()->sceneRect());
    _animation->setOpacity(opacity());
    _animation->setPos(scene()->width()/2-16,scene()->height()/2-16);
  }
};

#ifdef WIN32

class CustomTabBar : public QTabBar {

public:

  CustomTabBar(QWidget *parent = 0) : QTabBar(parent) {}

protected:

  QSize tabSizeHint(int index) const {
    int width = QTabBar::tabSizeHint(index).width();
    return QSize(width, fontMetrics().width(tabText(index))*2 + iconSize().width());
  }

};

class CustomTabWidget : public QTabWidget {

public:

  CustomTabWidget(QWidget *parent=0) : QTabWidget(parent) {
    setTabBar(new CustomTabBar());
  }

};

#endif

// ========================

WorkspacePanel::WorkspacePanel(tlp::View* view, QWidget *parent)
  : QFrame(parent),
    _ui(new Ui::WorkspacePanel),
    _view(NULL),
    _overlayRect(NULL),
    _viewConfigurationWidgets(NULL),
    _viewConfigurationExpanded(false),
    _currentInteractorConfigurationItem(NULL) {
  _ui->setupUi(this);
  _ui->actionClose->setShortcutContext(Qt::WidgetWithChildrenShortcut);
  _ui->interactorsFrame->installEventFilter(this);
  _ui->dragHandle->setPanel(this);
  _ui->graphCombo->installEventFilter(this);
  connect(_ui->linkButton, SIGNAL(toggled(bool)),
          this, SLOT(toggleSynchronization(bool)));
  connect(_ui->closeButton,SIGNAL(clicked()),this,SLOT(close()));
  setView(view);
  setAttribute(Qt::WA_DeleteOnClose);
}

WorkspacePanel::~WorkspacePanel() {
  //Ensure the current interactor configuration widget is not deleted by the scrool area.
  //It is up to the interactor developer to delete its configuration widget.
  if (_currentInteractorConfigurationItem != NULL) {
    static_cast<QScrollArea*>(_currentInteractorConfigurationItem->widget())->takeWidget();
  }

  delete _ui;

  if (_view != NULL) {
    disconnect(_view,SIGNAL(destroyed()),this,SLOT(viewDestroyed()));
    delete _view;
  }
}
void WorkspacePanel::viewDestroyed() {
  if (_view != NULL) {
    disconnect(_view,SIGNAL(destroyed()),this,SLOT(viewDestroyed()));
    _view = NULL;
  }

  deleteLater();
}

View* WorkspacePanel::view() const {
  return _view;
}

QString WorkspacePanel::viewName() const {
  return _viewName;
}

void WorkspacePanel::setView(tlp::View* view) {
  assert(view != NULL);
  _ui->currentInteractorButton->setChecked(false);

  if (_view != NULL) {
    disconnect(_view,SIGNAL(destroyed()),this,SLOT(viewDestroyed()));
    disconnect(_view,SIGNAL(graphSet(tlp::Graph*)),this,SLOT(viewGraphSet(tlp::Graph*)));
    disconnect(_view,SIGNAL(drawNeeded()),this,SIGNAL(drawNeeded()));
    delete _view->graphicsView();
  }

  delete _view;

  _view = view;
  _viewName = tlp::tlpStringToQString(view->name());

  QList<Interactor*> compatibleInteractors;
  QList<std::string> interactorNames = InteractorLister::compatibleInteractors(view->name());
  foreach(std::string name,interactorNames) {
    compatibleInteractors << PluginLister::instance()->getPluginObject<Interactor>(name,NULL);
  }
  _view->setInteractors(compatibleInteractors);
  _ui->scrollArea->setVisible(!compatibleInteractors.empty());
  _view->graphicsView()->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  _view->graphicsView()->addAction(_ui->actionClose);
  layout()->addWidget(_view->graphicsView());
  refreshInteractorsToolbar();


  if (!compatibleInteractors.empty())
    setCurrentInteractor(compatibleInteractors[0]);

  connect(_view,SIGNAL(destroyed()),this,SLOT(viewDestroyed()));
  connect(_view,SIGNAL(graphSet(tlp::Graph*)),this,SLOT(viewGraphSet(tlp::Graph*)));
  connect(_view,SIGNAL(drawNeeded()),this,SIGNAL(drawNeeded()));
  connect(_view,SIGNAL(interactorsChanged()), this, SLOT(refreshInteractorsToolbar()));

  if (_view->configurationWidgets().empty())
    return;

#ifdef WIN32
  QTabWidget* viewConfigurationTabs = new CustomTabWidget();
#else
  QTabWidget* viewConfigurationTabs = new QTabWidget();
#endif
  viewConfigurationTabs->setTabsClosable(true);
  connect(viewConfigurationTabs,SIGNAL(tabCloseRequested(int)),this,SLOT(hideConfigurationTab()));
  viewConfigurationTabs->setTabPosition(QTabWidget::West);
  viewConfigurationTabs->setStyleSheet(_view->configurationWidgetsStyleSheet());
  viewConfigurationTabs->findChild<QTabBar*>()->installEventFilter(this);


  foreach(QWidget* w, _view->configurationWidgets()) {
    w->installEventFilter(this);
    w->resize(w->width(),w->sizeHint().height());
    viewConfigurationTabs->addTab(w,w->windowTitle());
  }


  _viewConfigurationWidgets = new QGraphicsProxyWidget(_view->centralItem());
  _viewConfigurationWidgets->installEventFilter(this);
  _viewConfigurationWidgets->setWidget(viewConfigurationTabs);
  _viewConfigurationWidgets->setZValue(DBL_MAX);
  _view->graphicsView()->scene()->installEventFilter(this);
  resetInteractorsScrollButtonsVisibility();
}

bool WorkspacePanel::eventFilter(QObject* obj, QEvent* ev) {
  if (_viewConfigurationWidgets != NULL && _view != NULL) {
    if (ev->type() == QEvent::GraphicsSceneContextMenu) {
      _view->showContextMenu(QCursor::pos(),static_cast<QGraphicsSceneContextMenuEvent*>(ev)->scenePos());
    }

    else if (_view->configurationWidgets().contains(qobject_cast<QWidget*>(obj)))
      return true;

    else if (ev->type() == QEvent::MouseButtonPress && !_viewConfigurationExpanded && qobject_cast<QTabBar*>(obj) != NULL) {
      setConfigurationTabExpanded(true);
    }
    else if (ev->type() == QEvent::Wheel && qobject_cast<QTabBar*>(obj) != NULL) {
      return true;
    }
  }

  if (obj == _ui->interactorsFrame && ev->type() == QEvent::Wheel) {
    if (static_cast<QWheelEvent*>(ev)->delta()>0)
      scrollInteractorsLeft();
    else
      scrollInteractorsRight();
  }


  if (obj == _ui->graphCombo && ev->type() == QEvent::Wheel) {
    return true;
  }

  return QWidget::eventFilter(obj,ev);
}

void WorkspacePanel::setCurrentInteractor(tlp::Interactor *i) {
  assert(i);
  view()->setCurrentInteractor(i);
  _ui->currentInteractorButton->setText(i->action()->text());
  _ui->currentInteractorButton->setIcon(i->action()->icon());
  _ui->currentInteractorButton->setChecked(false);
  _ui->currentInteractorButton->setToolTip(QString("Active tool:<br/><b>") + i->action()->text() + QString("</b><br/><i>click to show/hide its configuration panel.</i>"));
}

void WorkspacePanel::setCurrentInteractorConfigurationVisible(bool toggle) {
  if (_currentInteractorConfigurationItem != NULL) {
    if (!toggle)
      _currentInteractorConfigurationItem->hide();
    else {
      QWidget* interactorWidget = _view->currentInteractor()->configurationWidget();

      if (!interactorWidget)
        return;

      QScrollArea* area = static_cast<QScrollArea*>(_currentInteractorConfigurationItem->widget());
      // avoid deletion of previous contents
      area->takeWidget();

      // set new contents
      if (interactorWidget->objectName() != "contents")
        interactorWidget->setObjectName("contents");

      area->setWidget(interactorWidget);

      // resize as much as possible
      QSize size(interactorWidget->sizeHint());
      QSize psize(_view->graphicsView()->size());

      if (size.width() > psize.width() - 30)
        size.setWidth(psize.width() - 30);

      if (size.height() > psize.height() - 30)
        size.setHeight(psize.height() - 30);

      area->resize(size);

      _currentInteractorConfigurationItem->show();
    }

    return;
  }

  if (!toggle || _view->currentInteractor() == NULL || _view->currentInteractor()->configurationWidget() == NULL)
    return;

  _currentInteractorConfigurationItem = new QGraphicsProxyWidget();
  _currentInteractorConfigurationItem->setParent(_view->graphicsView());
  _currentInteractorConfigurationItem->setObjectName("currentInteractorConfigurationItem");
  _currentInteractorConfigurationItem->setOpacity(0);
  _currentInteractorConfigurationItem->setPos(0,0);
  QScrollArea* area = new QScrollArea();
  area->setFrameShape(QScrollArea::NoFrame);
  QWidget* interactorWidget = _view->currentInteractor()->configurationWidget();
  interactorWidget->setObjectName("contents");
  area->setStyleSheet("#contents { background-color: white; border: 1px solid #C9C9C9; }");
  area->setWidget(interactorWidget);
  _currentInteractorConfigurationItem->setWidget(area);
  _currentInteractorConfigurationItem->setPos(0,0);
  _view->graphicsView()->scene()->addItem(_currentInteractorConfigurationItem);
  QPropertyAnimation* anim = new QPropertyAnimation(_currentInteractorConfigurationItem,"opacity",_currentInteractorConfigurationItem);
  anim->setStartValue(0);
  //there are artefacts in the fonts when the opacity is 1; ugly fix
  anim->setEndValue(0.99);
  anim->setDuration(300);
  anim->setEasingCurve(QEasingCurve::OutQuad);
  anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void WorkspacePanel::interactorActionTriggered() {
  QAction* action = static_cast<QAction*>(sender());
  Interactor* interactor = static_cast<Interactor*>(action->parent());

  if (interactor == view()->currentInteractor())
    return;

  setCurrentInteractor(interactor);
}

void WorkspacePanel::hideConfigurationTab() {
  setConfigurationTabExpanded(false);
}

void clearLayout(QLayout* layout, bool deleteWidgets = true) {
  while (QLayoutItem* item = layout->takeAt(0)) {
    if (deleteWidgets) {
      if (QWidget* widget = item->widget())
        delete widget;
    }
    else if (QLayout* childLayout = item->layout())
      clearLayout(childLayout, deleteWidgets);

    delete item;
  }
}

void WorkspacePanel::refreshInteractorsToolbar() {
  _actionTriggers.clear();
  QList<Interactor*> compatibleInteractors = _view->interactors();

  if (_ui->interactorsFrame->layout()) {
    clearLayout(_ui->interactorsFrame->layout());
  }

  delete _ui->interactorsFrame->layout();
  bool interactorsUiShown = !compatibleInteractors.isEmpty();
  _ui->currentInteractorButton->setVisible(interactorsUiShown);
  _ui->interactorsFrame->setVisible(interactorsUiShown);
  _ui->sep1->setVisible(interactorsUiShown);
  _ui->sep2->setVisible(interactorsUiShown);

  if (interactorsUiShown) {
    QHBoxLayout* interactorsLayout = new QHBoxLayout;
    interactorsLayout->setContentsMargins(0,0,0,0);
    interactorsLayout->setSpacing(4);
    foreach(Interactor* i, compatibleInteractors) {
      QPushButton* button = new QPushButton();
      button->setMinimumSize(22,22);
      button->setFlat(true);
      button->setMaximumSize(22,22);
      button->setIcon(i->action()->icon());
      button->setToolTip(i->action()->text());
      interactorsLayout->addWidget(button);
      button->setEnabled(i->action()->isEnabled());
      connect(button,SIGNAL(clicked()),i->action(),SLOT(trigger()));
      connect(i->action(),SIGNAL(triggered()),this,SLOT(interactorActionTriggered()));
      connect(i->action(),SIGNAL(changed()),this,SLOT(actionChanged()));
      _actionTriggers[i->action()] = button;
    }
    _ui->interactorsFrame->setLayout(interactorsLayout);
    setCurrentInteractor(compatibleInteractors[0]);
  }
}

void WorkspacePanel::actionChanged() {
  QAction* action = static_cast<QAction*>(sender());

  if (!_actionTriggers.contains(action))
    return;

  _actionTriggers[action]->setEnabled(action->isEnabled());
}

void WorkspacePanel::scrollInteractorsRight() {
  QScrollBar* scrollBar = _ui->scrollArea->horizontalScrollBar();
  scrollBar->setSliderPosition(scrollBar->sliderPosition()+scrollBar->singleStep());
}

void WorkspacePanel::scrollInteractorsLeft() {
  QScrollBar* scrollBar = _ui->scrollArea->horizontalScrollBar();
  scrollBar->setSliderPosition(scrollBar->sliderPosition()-scrollBar->singleStep());
}

void WorkspacePanel::resetInteractorsScrollButtonsVisibility() {
  QScrollBar* scrollBar = _ui->scrollArea->horizontalScrollBar();
  bool interactorScrollBtnVisible = scrollBar->minimum() != scrollBar->maximum();
  _ui->interactorsLeft->setVisible(interactorScrollBtnVisible);
  _ui->interactorsRight->setVisible(interactorScrollBtnVisible);
}

void WorkspacePanel::setGraphsModel(tlp::GraphHierarchiesModel* model) {
  _ui->graphCombo->setModel(model);
  connect(_ui->graphCombo,SIGNAL(currentItemChanged()),this,SLOT(graphComboIndexChanged()));

}

void WorkspacePanel::viewGraphSet(tlp::Graph* g) {
  assert(dynamic_cast<tlp::GraphHierarchiesModel*>(_ui->graphCombo->model()));
#ifndef NDEBUG

  if(g) {
    qDebug() << "Setting graph " << tlp::tlpStringToQString(g->getName()) << " for panel " << windowTitle();
  }

#endif // NDEBUG

  tlp::GraphHierarchiesModel* model = static_cast<tlp::GraphHierarchiesModel*>(_ui->graphCombo->model());
  QModelIndex graphIndex = model->indexOf(g);

  if (graphIndex == _ui->graphCombo->selectedIndex())
    return;

  _ui->graphCombo->selectIndex(graphIndex);
}

void WorkspacePanel::graphComboIndexChanged() {
  tlp::Graph* g = _ui->graphCombo->model()->data(_ui->graphCombo->selectedIndex(),TulipModel::GraphRole).value<tlp::Graph*>();
#ifndef NDEBUG

  if (g != NULL) {
    qDebug() << "selecting graph " << tlp::tlpStringToQString(g->getName()) << " in view";
  }

#endif /* NDEBUG */

  if (g != NULL && _view != NULL && g != _view->graph()) {
    _view->setGraph(g);
  }

}

void WorkspacePanel::resizeEvent(QResizeEvent* ev) {
  if (_viewConfigurationWidgets) {
    setConfigurationTabExpanded(_viewConfigurationExpanded,false);
  }

  resetInteractorsScrollButtonsVisibility();

  QWidget::resizeEvent(ev);
}

void WorkspacePanel::setConfigurationTabExpanded(bool expanded, bool animate) {
  QPointF newPos = configurationTabPosition(expanded);

  if (newPos == _viewConfigurationWidgets->pos())
    return;

  if (animate) {
    QPropertyAnimation* anim = new QPropertyAnimation(_viewConfigurationWidgets,"pos",_viewConfigurationWidgets);
    anim->setDuration(250);
    anim->setStartValue(_viewConfigurationWidgets->pos());
    anim->setEndValue(newPos);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
  }
  else
    _viewConfigurationWidgets->setPos(newPos);

  //there are artefacts in the fonts when the opacity is 1; ugly fix
  _viewConfigurationWidgets->setOpacity((expanded ? 0.99 : 0.6));

  if (!expanded && _viewConfigurationExpanded) {
    _view->applySettings();
  }

  _viewConfigurationExpanded = expanded;

  if (_view != NULL) {
    _viewConfigurationWidgets->setMaximumSize(_view->graphicsView()->size());
  }
}

QPointF WorkspacePanel::configurationTabPosition(bool expanded) const {
  if (expanded)
    return QPointF(width()-_viewConfigurationWidgets->size().width(),10);
  else {
    QTabWidget* tabWidget = static_cast<QTabWidget*>(_viewConfigurationWidgets->widget());
    int tabWidth = _viewConfigurationWidgets->size().width() - tabWidget->widget(0)->width();
    return QPointF(width() - tabWidth,10);
  }
}

void WorkspacePanel::setOverlayMode(bool m) {
  if (m && _overlayRect == NULL) {
    _overlayRect = new QGraphicsRectItem(_view->graphicsView()->sceneRect());
    _overlayRect->setBrush(QColor::fromHsv(0,0,0,50));
    _overlayRect->setPen(QColor(67, 86, 108));
    _view->graphicsView()->scene()->addItem(_overlayRect);
    _overlayRect->setZValue(30);
  }

  if (!m && _overlayRect != NULL) {
    delete _overlayRect;
    _overlayRect = NULL;
  }
}

void WorkspacePanel::setHighlightMode(bool hm) {
  // when highlighted the backgroud color
  // is set to a light green
  // which made appear a 2 pixel height line
  // on top of this panel
  // (only top margin has a non null value in WorkspacePanel.ui)
  if (hm)
    _ui->borderFrame->setStyleSheet(QString::fromUtf8("QFrame[border = \"true\"] {\n"
                                    "border-image:none;\n"
                                    "background-color: #CBDE5D;\n"
                                    "color: white;\n"
                                    "}"));
  else
    // restore the style sheet as described in WorkspacePanel.ui
    _ui->borderFrame->setStyleSheet(QString::fromUtf8("QFrame[border = \"true\"] {\n"
                                    "border-image:none;\n"
                                    "background-color: qlineargradient(x1: 0, y1: 0, x2: 0.0, y2: 1.0,\n"
                                    "stop: 0 #838383,\n"
                                    "stop: 0.4 #707070,\n"
                                    "stop: 0.401 #636363,\n"
                                    "stop: 1 #4a4a4a);\n"
                                    "color: white;\n"
                                    "}"));
}

void WorkspacePanel::dragEnterEvent(QDragEnterEvent * evt) {
  handleDragEnterEvent(evt,evt->mimeData());
}

void WorkspacePanel::dropEvent(QDropEvent *evt) {
  handleDropEvent(evt->mimeData());
}

void WorkspacePanel::dragLeaveEvent(QDragLeaveEvent *) {
  setOverlayMode(false);
}

bool WorkspacePanel::handleDragEnterEvent(QEvent* e, const QMimeData* mimedata) {
  if(dynamic_cast<const GraphMimeType*>(mimedata) != NULL || dynamic_cast<const PanelMimeType*>(mimedata) != NULL || dynamic_cast<const AlgorithmMimeType*>(mimedata) !=  NULL) {
    setOverlayMode(true);
    e->accept();
    return true;
  }

  return false;
}

bool WorkspacePanel::handleDropEvent(const QMimeData* mimedata) {
  const GraphMimeType* graphMime = dynamic_cast<const GraphMimeType*>(mimedata);
  const PanelMimeType* panelMime = dynamic_cast<const PanelMimeType*>(mimedata);
  const AlgorithmMimeType* algorithmMime = dynamic_cast<const AlgorithmMimeType*>(mimedata);

  if (graphMime != NULL && graphMime->graph()) {
    viewGraphSet(graphMime->graph());
  }
  else if (panelMime) {
    //Emit swap panels
    emit swapWithPanels(panelMime->panel());
  }

  else if (algorithmMime) {
    algorithmMime->run(view()->graph());
  }

  setOverlayMode(false);
  return graphMime != NULL || panelMime != NULL || algorithmMime != NULL;
}

bool WorkspacePanel::isGraphSynchronized() const {
  return _ui->linkButton->isChecked();
}

void WorkspacePanel::toggleSynchronization(bool f) {
  if (f) {
    _ui->linkButton->setIcon(QIcon(":/tulip/gui/icons/16/link.png"));
    _ui->linkButton->setToolTip("Click here to disable the synchronization with the Graphs panel.\nWhen synchronization is enabled, the current graph of the Graphs panel,\nbecomes the current one in the workspace active panel.");
  }
  else {
    _ui->linkButton->setIcon(QIcon(":/tulip/gui/icons/16/unlink.png"));
    _ui->linkButton->setToolTip("Click here to enable the synchronization with the Graphs panel.\nWhen synchronization is enabled, the current graph of the Graphs panel,\nbecomes the current one in the workspace active panel.");
  }

  emit changeGraphSynchronization(f);
}
