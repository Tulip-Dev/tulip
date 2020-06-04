/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include "tulip/InteractorConfigWidget.h"
#include "ui_WorkspacePanel.h"

#include <QGraphicsView>
#include <QCloseEvent>
#include <QPushButton>
#include <QApplication>
#include <QGraphicsProxyWidget>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QTabWidget>
#include <QGraphicsSceneContextMenuEvent>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QPropertyAnimation>

#include <tulip/TulipMetaTypes.h>
#include <tulip/ProcessingAnimationItem.h>
#include <tulip/Interactor.h>
#include <tulip/View.h>
#include <tulip/Graph.h>
#include <tulip/GraphHierarchiesModel.h>
#include <tulip/PluginLister.h>
#include <tulip/TulipMimes.h>
#include <tulip/TlpQtTools.h>

using namespace tlp;

// helper class
class ProgressItem : public QGraphicsObject {
  ProcessingAnimationItem *_animation;

public:
  ProgressItem(QGraphicsScene *parentScene) : QGraphicsObject() {
    _animation = new ProcessingAnimationItem(QPixmap(":/tulip/gui/ui/process-working.png"),
                                             QSize(64, 64), this);
    _animation->setZValue(5);
    parentScene->addItem(_animation);
  }

  ~ProgressItem() override {
    delete _animation;
  }

  QRectF boundingRect() const override {
    return QRectF();
  }

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override {
    painter->setPen(QColor(255, 255, 255));
    painter->setBrush(QColor(255, 255, 255, 170));
    painter->drawRect(scene()->sceneRect());
    _animation->setOpacity(opacity());
    _animation->setPos(scene()->width() / 2 - 16, scene()->height() / 2 - 16);
  }
};

#ifdef WIN32

class CustomTabBar : public QTabBar {

public:
  CustomTabBar(QWidget *parent = 0) : QTabBar(parent) {
    setDrawBase(false);
  }

protected:
  QSize tabSizeHint(int index) const {
    int width = QTabBar::tabSizeHint(index).width();
#if (QT_VERSION < QT_VERSION_CHECK(5, 11, 0))
    return QSize(width, fontMetrics().width(tabText(index)) * 2 + iconSize().width());
#else
    return QSize(width, fontMetrics().horizontalAdvance(tabText(index)) * 2 + iconSize().width());
#endif
  }
};

class CustomTabWidget : public QTabWidget {

public:
  CustomTabWidget(QWidget *parent = 0) : QTabWidget(parent) {
    setTabBar(new CustomTabBar());
  }
};

#endif

// ========================

WorkspacePanel::WorkspacePanel(tlp::View *view, QWidget *parent)
    : QFrame(parent), _ui(new Ui::WorkspacePanel),
      _interactorConfigWidget(new InteractorConfigWidget(this)), _view(nullptr),
      _overlayRect(nullptr), _viewConfigurationWidgets(nullptr), _viewConfigurationExpanded(false) {
  _ui->setupUi(this);
  _ui->actionClose->setShortcutContext(Qt::WidgetWithChildrenShortcut);
  _ui->interactorsFrame->installEventFilter(this);
  _ui->dragHandle->setPanel(this);
  _ui->graphCombo->installEventFilter(this);
  connect(_ui->linkButton, SIGNAL(toggled(bool)), this, SLOT(toggleSynchronization(bool)));
  connect(_ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
  setView(view);
  setAttribute(Qt::WA_DeleteOnClose);
  setAutoFillBackground(true);
}

WorkspacePanel::~WorkspacePanel() {
  delete _ui;
  // because of possible mis-synchronization of Qt events
  // set it to nullptr
  // to avoid any invalid read in the eventFilter method
  _ui = nullptr;

  if (_view != nullptr) {
    disconnect(_view, SIGNAL(destroyed()), this, SLOT(viewDestroyed()));
    _interactorConfigWidget->clearWidgets();
    delete _view;
    // same as above
    _view = nullptr;
  }
}
void WorkspacePanel::viewDestroyed() {
  if (_view != nullptr) {
    disconnect(_view, SIGNAL(destroyed()), this, SLOT(viewDestroyed()));
    _interactorConfigWidget->clearWidgets();
    _view = nullptr;
  }

  deleteLater();
}

View *WorkspacePanel::view() const {
  return _view;
}

QString WorkspacePanel::viewName() const {
  return tlp::tlpStringToQString(_view->name());
}

void WorkspacePanel::setView(tlp::View *view) {
  assert(view != nullptr);
  _ui->currentInteractorButton->setChecked(false);

  if (_view != nullptr) {
    disconnect(_view, SIGNAL(destroyed()), this, SLOT(viewDestroyed()));
    disconnect(_view, SIGNAL(graphSet(tlp::Graph *)), this, SLOT(viewGraphSet(tlp::Graph *)));
    disconnect(_view, SIGNAL(drawNeeded()), this, SIGNAL(drawNeeded()));
    delete _view->graphicsView();
  }

  delete _view;

  _view = view;

  QList<Interactor *> compatibleInteractors;
  QList<std::string> interactorNames = InteractorLister::compatibleInteractors(view->name());

  for (const std::string &name : interactorNames) {
    compatibleInteractors << PluginLister::getPluginObject<Interactor>(name);
  }

  _view->setInteractors(compatibleInteractors);
  _ui->scrollArea->setVisible(!compatibleInteractors.empty());
  _view->graphicsView()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  _view->graphicsView()->addAction(_ui->actionClose);
  layout()->addWidget(_view->graphicsView());
  refreshInteractorsToolbar();

  if (!compatibleInteractors.empty())
    setCurrentInteractor(compatibleInteractors[0]);

  connect(_view, SIGNAL(destroyed()), this, SLOT(viewDestroyed()));
  connect(_view, SIGNAL(graphSet(tlp::Graph *)), this, SLOT(viewGraphSet(tlp::Graph *)));
  connect(_view, SIGNAL(drawNeeded()), this, SIGNAL(drawNeeded()));
  connect(_view, SIGNAL(interactorsChanged()), this, SLOT(refreshInteractorsToolbar()));
  _view->graphicsView()->scene()->installEventFilter(this);

  if (!_view->configurationWidgets().empty()) {
#ifdef WIN32
    QTabWidget *viewConfigurationTabs = new CustomTabWidget();
#else
    QTabWidget *viewConfigurationTabs = new QTabWidget();
#endif
    viewConfigurationTabs->setTabsClosable(true);
    connect(viewConfigurationTabs, SIGNAL(tabCloseRequested(int)), this,
            SLOT(hideConfigurationTab()));
    viewConfigurationTabs->setTabPosition(QTabWidget::West);
    viewConfigurationTabs->setStyleSheet(_view->configurationWidgetsStyleSheet());
    viewConfigurationTabs->findChild<QTabBar *>()->installEventFilter(this);

    for (auto w : _view->configurationWidgets()) {
      w->installEventFilter(this);
      w->resize(w->width(), w->sizeHint().height());
      viewConfigurationTabs->addTab(w, w->windowTitle());
    }

    _viewConfigurationWidgets = new QGraphicsProxyWidget(_view->centralItem());
    _viewConfigurationWidgets->installEventFilter(this);
    _viewConfigurationWidgets->setWidget(viewConfigurationTabs);
    _viewConfigurationWidgets->setZValue(DBL_MAX);
  }

  resetInteractorsScrollButtonsVisibility();
}

// Workaround to avoid a Qt5 bug :
// After the panels containing QGraphicsView objects were rearranged in the workspace,
// some events were no more sent to the QGraphicsWidget objects embedded in the asoociated
// QGraphicScene objects.
// Those events are necessary for important parts of the view GUI (context menu, keyboard focus) to
// work correctly.
// So add a hack that, each time a view is shown, creates a new QGraphicsScene object
// and refill it with QGraphicsItem objects contained in the previous one.
// Seems to be the only way to workaround that issue.
void WorkspacePanel::showEvent(QShowEvent *event) {
  QFrame::showEvent(event);

  if (_view && _view->graphicsView() && _view->graphicsView()->scene() &&
      _view->rebuildSceneOnShowEvent()) {
    // disable warnings output about QGraphicScene update
    enableWarningOutput(false);
    // first remove central item of the scene and its children
    _view->graphicsView()->scene()->removeItem(_view->centralItem());
    // get remaining items (if any) that were not descendant of the central item
    // and remove it from the scene
    QList<QGraphicsItem *> items = _view->graphicsView()->scene()->items();

    for (int i = 0; i < items.size(); ++i) {
      _view->graphicsView()->scene()->removeItem(items.at(i));
    }

    // get old scene pointer for further deletion
    QGraphicsScene *oldScene = _view->graphicsView()->scene();
    QGraphicsScene *newScene = new QGraphicsScene();
    newScene->setSceneRect(oldScene->sceneRect());
    // create a new QGraphicsScene and set it in the QGraphicsView
    _view->graphicsView()->setScene(newScene);
    // restore central item and its children in the new scene
    _view->graphicsView()->scene()->addItem(_view->centralItem());

    // restore remaining items in the new scene
    for (int i = 0; i < items.size(); ++i) {
      _view->graphicsView()->scene()->addItem(items.at(i));
    }

    // set event filter for the new scene
    _view->graphicsView()->scene()->installEventFilter(this);
    // restore any specific behavior of the QGraphicsScene
    _view->resetGraphicsScene();

    // delete old scene
    delete oldScene;
    // re-enable warnings output
    enableWarningOutput(true);
  }
}

void WorkspacePanel::closeEvent(QCloseEvent *event) {
  if (_view->checkOnClose())
    event->accept();
  else
    event->ignore();
}

bool WorkspacePanel::eventFilter(QObject *obj, QEvent *ev) {
  if (_view != nullptr) {
    if (ev->type() == QEvent::GraphicsSceneContextMenu) {
      _view->showContextMenu(QCursor::pos(),
                             static_cast<QGraphicsSceneContextMenuEvent *>(ev)->scenePos());
    } else if (_viewConfigurationWidgets != nullptr &&
               _view->configurationWidgets().contains(qobject_cast<QWidget *>(obj)))
      return true;

    else if (ev->type() == QEvent::MouseButtonPress && !_viewConfigurationExpanded &&
             qobject_cast<QTabBar *>(obj) != nullptr) {
      setConfigurationTabExpanded(true);
    } else if (ev->type() == QEvent::Wheel && qobject_cast<QTabBar *>(obj) != nullptr) {
      return true;
    }
  }

  // we must check _ui has not been deleted
  // because of possible mis-synchronization of Qt events
  if (_ui) {
    if (obj == _ui->interactorsFrame && ev->type() == QEvent::Wheel) {
      if (static_cast<QWheelEvent *>(ev)->angleDelta().y() > 0)
        scrollInteractorsLeft();
      else
        scrollInteractorsRight();
    }

    if (obj == _ui->graphCombo && ev->type() == QEvent::Wheel) {
      return true;
    }
  }

  return QWidget::eventFilter(obj, ev);
}

void WorkspacePanel::setCurrentInteractor(tlp::Interactor *i) {
  assert(i);
  view()->setCurrentInteractor(i);
  _ui->currentInteractorButton->setText(i->action()->text());
  _ui->currentInteractorButton->setIcon(i->action()->icon());
  _ui->currentInteractorButton->setToolTip(
      QString("Active tool:<br/><b>") + i->action()->text() +
      QString(_view->currentInteractor()->configurationWidget()
                  ? "</b><br/><i>click to show/hide its configuration panel.</i>"
                  : "</b>"));
  _view->graphicsView()->setFocus();
}

void WorkspacePanel::setCurrentInteractorConfigurationVisible(bool) {
  if (_view->currentInteractor() == nullptr)
    return;

  if (_interactorConfigWidget->isVisible())
    return;

  if (_interactorConfigWidget->setWidgets(_view->currentInteractor()))
    _interactorConfigWidget->show();
}

void WorkspacePanel::interactorActionTriggered() {
  QAction *action = static_cast<QAction *>(sender());
  Interactor *interactor = static_cast<Interactor *>(action->parent());

  if (interactor == view()->currentInteractor())
    return;

  setCurrentInteractor(interactor);
  if (_interactorConfigWidget->isVisible()) {
    _interactorConfigWidget->setWidgets(_view->currentInteractor());
  }
}

void WorkspacePanel::hideConfigurationTab() {
  setConfigurationTabExpanded(false);
}

void clearLayout(QLayout *layout, bool deleteWidgets = true) {
  while (QLayoutItem *item = layout->takeAt(0)) {
    if (deleteWidgets) {
      if (QWidget *widget = item->widget())
        delete widget;
    } else if (QLayout *childLayout = item->layout())
      clearLayout(childLayout, deleteWidgets);

    delete item;
  }
}

void WorkspacePanel::refreshInteractorsToolbar() {
  _actionTriggers.clear();
  auto compatibleInteractors = _view->interactors();

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
    QHBoxLayout *interactorsLayout = new QHBoxLayout;
    interactorsLayout->setContentsMargins(0, 0, 0, 0);
    interactorsLayout->setSpacing(4);

    for (auto i : compatibleInteractors) {
      QPushButton *button = new QPushButton();
      button->setMinimumSize(22, 22);
      button->setFlat(true);
      button->setMaximumSize(22, 22);
      button->setIcon(i->action()->icon());
      button->setToolTip(i->action()->text());
      interactorsLayout->addWidget(button);
      button->setEnabled(i->action()->isEnabled());
      connect(button, SIGNAL(clicked()), i->action(), SLOT(trigger()));
      connect(i->action(), SIGNAL(triggered()), this, SLOT(interactorActionTriggered()));
      connect(i->action(), SIGNAL(changed()), this, SLOT(actionChanged()));
      _actionTriggers[i->action()] = button;
    }

    _ui->interactorsFrame->setLayout(interactorsLayout);
    setCurrentInteractor(compatibleInteractors[0]);
  }
}

void WorkspacePanel::actionChanged() {
  QAction *action = static_cast<QAction *>(sender());

  if (!_actionTriggers.contains(action))
    return;

  _actionTriggers[action]->setEnabled(action->isEnabled());
}

void WorkspacePanel::scrollInteractorsRight() {
  QScrollBar *scrollBar = _ui->scrollArea->horizontalScrollBar();
  scrollBar->setSliderPosition(scrollBar->sliderPosition() + scrollBar->singleStep());
}

void WorkspacePanel::scrollInteractorsLeft() {
  QScrollBar *scrollBar = _ui->scrollArea->horizontalScrollBar();
  scrollBar->setSliderPosition(scrollBar->sliderPosition() - scrollBar->singleStep());
}

void WorkspacePanel::resetInteractorsScrollButtonsVisibility() {
  QScrollBar *scrollBar = _ui->scrollArea->horizontalScrollBar();
  bool interactorScrollBtnVisible = scrollBar->minimum() != scrollBar->maximum();
  _ui->interactorsLeft->setVisible(interactorScrollBtnVisible);
  _ui->interactorsRight->setVisible(interactorScrollBtnVisible);
}

void WorkspacePanel::setGraphsModel(tlp::GraphHierarchiesModel *model) {
  _ui->graphCombo->setModel(model);
  connect(_ui->graphCombo, SIGNAL(currentItemChanged()), this, SLOT(graphComboIndexChanged()));
}

void WorkspacePanel::viewGraphSet(tlp::Graph *g) {
  assert(dynamic_cast<tlp::GraphHierarchiesModel *>(_ui->graphCombo->model()));
#ifndef NDEBUG

  if (g) {
    qDebug() << "Setting graph " << tlp::tlpStringToQString(g->getName()) << " for panel "
             << windowTitle();
  }

#endif // NDEBUG

  tlp::GraphHierarchiesModel *model =
      static_cast<tlp::GraphHierarchiesModel *>(_ui->graphCombo->model());
  QModelIndex graphIndex = model->indexOf(g);

  if (graphIndex == _ui->graphCombo->selectedIndex())
    return;

  _ui->graphCombo->selectIndex(graphIndex);
}

void WorkspacePanel::graphComboIndexChanged() {
  tlp::Graph *g = _ui->graphCombo->model()
                      ->data(_ui->graphCombo->selectedIndex(), TulipModel::GraphRole)
                      .value<tlp::Graph *>();
#ifndef NDEBUG

  if (g != nullptr) {
    qDebug() << "selecting graph " << tlp::tlpStringToQString(g->getName()) << " in view";
  }

#endif /* NDEBUG */

  if (g != nullptr && _view != nullptr && g != _view->graph()) {
    _view->setGraph(g);
  }
}

void WorkspacePanel::resizeEvent(QResizeEvent *ev) {
  if (_viewConfigurationWidgets) {
    setConfigurationTabExpanded(_viewConfigurationExpanded, false);
  }

  resetInteractorsScrollButtonsVisibility();

  QWidget::resizeEvent(ev);
}

void WorkspacePanel::setConfigurationTabExpanded(bool expanded, bool animate) {

  if (_view != nullptr) {
    _viewConfigurationWidgets->setMinimumHeight(_view->graphicsView()->height());
    _viewConfigurationWidgets->setMaximumHeight(_view->graphicsView()->height());
    _viewConfigurationWidgets->setMaximumWidth(_view->graphicsView()->width());
  }

  QPointF newPos = configurationTabPosition(expanded);

  if (newPos == _viewConfigurationWidgets->pos())
    return;

  if (animate) {
    QPropertyAnimation *anim =
        new QPropertyAnimation(_viewConfigurationWidgets, "pos", _viewConfigurationWidgets);
    anim->setDuration(250);
    anim->setStartValue(_viewConfigurationWidgets->pos());
    anim->setEndValue(newPos);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
  } else
    _viewConfigurationWidgets->setPos(newPos);

  // there are artefacts in the fonts when the opacity is 1; ugly fix
  _viewConfigurationWidgets->setOpacity((expanded ? 0.99 : 0.6));

  if (!expanded && _viewConfigurationExpanded) {
    _view->applySettings();
  }

  _viewConfigurationExpanded = expanded;
}

QPointF WorkspacePanel::configurationTabPosition(bool expanded) const {
  if (expanded)
    return QPointF(width() - _viewConfigurationWidgets->size().width(), 10);
  else {
    QTabWidget *tabWidget = static_cast<QTabWidget *>(_viewConfigurationWidgets->widget());
    int tabWidth = (tabWidget != nullptr)
                       ? (_viewConfigurationWidgets->size().width() - tabWidget->widget(0)->width())
                       : 0;
    return QPointF(width() - tabWidth, 10);
  }
}

void WorkspacePanel::setOverlayMode(bool m) {
  if (m && _overlayRect == nullptr) {
    _overlayRect = new QGraphicsRectItem(_view->graphicsView()->sceneRect());
    _overlayRect->setBrush(QColor::fromHsv(0, 0, 0, 50));
    _overlayRect->setPen(QColor(67, 86, 108));
    _view->graphicsView()->scene()->addItem(_overlayRect);
    _overlayRect->setZValue(30);
  }

  if (!m && _overlayRect != nullptr) {
    delete _overlayRect;
    _overlayRect = nullptr;
  }
}

void WorkspacePanel::setHighlightMode(bool hm) {
  // when highlighted the background color
  // is set to a light green
  // which made appear a 2 pixel height line
  // on top of this panel
  // (only top margin has a non null value in WorkspacePanel.ui)
  if (hm)
    _ui->borderFrame->setStyleSheet("QFrame[border = \"true\"] {\n"
                                    "border-image:none;\n"
                                    "background-color: #CBDE5D;\n"
                                    "color: white;\n"
                                    "}");
  else
    // restore the style sheet as described in WorkspacePanel.ui
    _ui->borderFrame->setStyleSheet(
        "QFrame[border = \"true\"] {\n"
        "border-image:none;\n"
        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0.0, y2: 1.0,\n"
        "stop: 0 #838383,\n"
        "stop: 0.4 #707070,\n"
        "stop: 0.401 #636363,\n"
        "stop: 1 #4a4a4a);\n"
        "color: white;\n"
        "}");
}

void WorkspacePanel::dragEnterEvent(QDragEnterEvent *evt) {
  handleDragEnterEvent(evt, evt->mimeData());
}

void WorkspacePanel::dropEvent(QDropEvent *evt) {
  handleDropEvent(evt->mimeData());
}

void WorkspacePanel::dragLeaveEvent(QDragLeaveEvent *) {
  setOverlayMode(false);
}

bool WorkspacePanel::handleDragEnterEvent(QEvent *e, const QMimeData *mimedata) {
  if (dynamic_cast<const GraphMimeType *>(mimedata) != nullptr ||
      dynamic_cast<const PanelMimeType *>(mimedata) != nullptr ||
      dynamic_cast<const AlgorithmMimeType *>(mimedata) != nullptr) {
    setOverlayMode(true);
    e->accept();
    return true;
  }

  return false;
}

bool WorkspacePanel::handleDropEvent(const QMimeData *mimedata) {
  const GraphMimeType *graphMime = dynamic_cast<const GraphMimeType *>(mimedata);
  const PanelMimeType *panelMime = dynamic_cast<const PanelMimeType *>(mimedata);
  const AlgorithmMimeType *algorithmMime = dynamic_cast<const AlgorithmMimeType *>(mimedata);

  if (graphMime != nullptr && graphMime->graph()) {
    viewGraphSet(graphMime->graph());
  } else if (panelMime) {
    // Emit swap panels
    emit swapWithPanels(panelMime->panel());
  }

  else if (algorithmMime) {
    algorithmMime->run(view()->graph());
  }

  setOverlayMode(false);
  return graphMime != nullptr || panelMime != nullptr || algorithmMime != nullptr;
}

bool WorkspacePanel::isGraphSynchronized() const {
  return _ui->linkButton->isChecked();
}

void WorkspacePanel::toggleSynchronization(bool f) {
  if (f) {
    _ui->linkButton->setIcon(QIcon(":/tulip/gui/icons/16/link.png"));
    _ui->linkButton->setToolTip("Click here to disable the synchronization with the Graphs "
                                "panel.\nWhen synchronization is enabled, the current graph of the "
                                "Graphs panel,\nbecomes the current one in the workspace active "
                                "panel.");
  } else {
    _ui->linkButton->setIcon(QIcon(":/tulip/gui/icons/16/unlink.png"));
    _ui->linkButton->setToolTip("Click here to enable the synchronization with the Graphs "
                                "panel.\nWhen synchronization is enabled, the current graph of the "
                                "Graphs panel,\nbecomes the current one in the workspace active "
                                "panel.");
  }

  emit changeGraphSynchronization(f);
}
