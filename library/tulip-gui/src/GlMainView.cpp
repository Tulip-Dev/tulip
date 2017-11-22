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
#include <tulip/GlMainView.h>

#include <QApplication>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QAction>
#include <QMenu>
#include <QPushButton>
#include <QTimer>

#include <tulip/GlMainWidgetGraphicsItem.h>
#include <tulip/GlMainWidget.h>
#include <tulip/SceneConfigWidget.h>
#include <tulip/SceneLayersConfigWidget.h>
#include <tulip/GlOverviewGraphicsItem.h>
#include <tulip/QuickAccessBar.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/SnapshotDialog.h>
#include <tulip/Gl2DRect.h>
#include <tulip/OpenGlConfigManager.h>
#include <tulip/TlpQtTools.h>

using namespace tlp;

GlMainView::GlMainView()
    : _glMainWidget(nullptr), _overviewItem(nullptr), _showOvButton(nullptr),
      _showQabButton(nullptr), needQuickAccessBar(false), _quickAccessBarItem(nullptr),
      _quickAccessBar(nullptr), _sceneConfigurationWidget(nullptr),
      _sceneLayersConfigurationWidget(nullptr), _overviewPosition(OVERVIEW_BOTTOM_RIGHT),
      _updateOverview(true) {}

GlMainView::~GlMainView() {
  delete _sceneConfigurationWidget;
  delete _sceneLayersConfigurationWidget;
  delete _overviewItem;
}

void GlMainView::draw() {
  _glMainWidget->draw();
}

void GlMainView::redraw() {
  _glMainWidget->redraw();
}

void GlMainView::refresh() {
  _glMainWidget->draw(false);
}

void GlMainView::setOverviewPosition(const OverviewPosition &position) {
  _overviewPosition = position;
  drawOverview();
}

GlMainView::OverviewPosition GlMainView::overviewPosition() const {
  return _overviewPosition;
}

void GlMainView::setUpdateOverview(bool updateOverview) {
  _updateOverview = updateOverview;
}

bool GlMainView::updateOverview() const {
  return _updateOverview;
}

void GlMainView::setState(const tlp::DataSet &data) {
  bool overviewVisible = true;

  if (data.get<bool>("overviewVisible", overviewVisible))
    setOverviewVisible(overviewVisible);

  bool quickAccessBarVisible = true;

  if (data.get<bool>("quickAccessBarVisible", quickAccessBarVisible)) {
    needQuickAccessBar = true;
    setQuickAccessBarVisible(quickAccessBarVisible);
  }
}

tlp::DataSet GlMainView::state() const {
  DataSet data;
  data.set("overviewVisible", overviewVisible());

  if (needQuickAccessBar)
    data.set("quickAccessBarVisible", quickAccessBarVisible());

  return data;
}

void GlMainView::drawOverview(bool generatePixmap) {
  if (_overviewItem == nullptr) {
    _overviewItem = new GlOverviewGraphicsItem(this, *_glMainWidget->getScene());
    addToScene(_overviewItem);
    generatePixmap = true;
    // used to set the overview at the right place
    sceneRectChanged(QRectF(QPoint(0, 0), graphicsView()->size()));
  }

  if (_updateOverview) {
    _overviewItem->draw(generatePixmap);
  }
}

void GlMainView::assignNewGlMainWidget(GlMainWidget *glMainWidget, bool deleteOldGlMainWidget) {
  _glMainWidget = glMainWidget;

  if (_sceneLayersConfigurationWidget == nullptr) {
    _sceneLayersConfigurationWidget = new SceneLayersConfigWidget();
  }

  _sceneLayersConfigurationWidget->setGlMainWidget(_glMainWidget);
  connect(_sceneLayersConfigurationWidget, SIGNAL(drawNeeded()), this, SIGNAL(drawNeeded()));

  setCentralWidget(_glMainWidget, deleteOldGlMainWidget);
  GlMainWidgetGraphicsItem *glMainWidgetGraphicsItem =
      static_cast<GlMainWidgetGraphicsItem *>(centralItem());
  delete _sceneConfigurationWidget;
  _sceneConfigurationWidget = new SceneConfigWidget();
  _sceneConfigurationWidget->setGlMainWidget(_glMainWidget);

  connect(glMainWidgetGraphicsItem, SIGNAL(widgetPainted(bool)), this, SLOT(glMainViewDrawn(bool)));
}

GlOverviewGraphicsItem *GlMainView::overviewItem() const {
  return _overviewItem;
}

void GlMainView::setupWidget() {
  graphicsView()->viewport()->parentWidget()->installEventFilter(this);
  assignNewGlMainWidget(new GlMainWidget(nullptr, this), true);

  _forceRedrawAction = new QAction(trUtf8("Force redraw"), this);
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(_forceRedrawAction, "Redraw the current view", "Shift+R");
  connect(_forceRedrawAction, SIGNAL(triggered()), this, SLOT(redraw()));
  _forceRedrawAction->setShortcut(tr("Ctrl+Shift+R"));
  _forceRedrawAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);

  _centerViewAction = new QAction(trUtf8("Center view"), this);
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(
      _centerViewAction, "Make the view to fully display and center its contents", "Shif+C");
  connect(_centerViewAction, SIGNAL(triggered()), this, SLOT(centerView()));
  _centerViewAction->setShortcut(tr("Ctrl+Shift+C"));
  _centerViewAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);

  _snapshotAction = new QAction(trUtf8("Take a snapshot"), this);
  SET_TOOLTIP_WITH_CTRL_SHORTCUT(
      _snapshotAction, "Show a dialog to save a snapshot of the current view display", "Shift+P");
  connect(_snapshotAction, SIGNAL(triggered()), this, SLOT(openSnapshotDialog()));
  _snapshotAction->setShortcut(tr("Ctrl+Shift+P"));
  _snapshotAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);

  _advAntiAliasingAction = new QAction(trUtf8("Advanced anti-aliasing"), this);
  _advAntiAliasingAction->setToolTip(QString("Enable to use a better but more expensive technique "
                                             "of anti-aliasing (needs off screen rendering)"));
  _advAntiAliasingAction->setCheckable(true);
  _advAntiAliasingAction->setChecked(_glMainWidget->advancedAntiAliasingActivated());
  connect(_advAntiAliasingAction, SIGNAL(triggered(bool)), this,
          SLOT(setAdvancedAntiAliasing(bool)));

  graphicsView()->addAction(_centerViewAction);
  graphicsView()->addAction(_forceRedrawAction);
  graphicsView()->addAction(_advAntiAliasingAction);
  graphicsView()->addAction(_snapshotAction);
}

GlMainWidget *GlMainView::getGlMainWidget() const {
  return _glMainWidget;
}

void GlMainView::centerView(bool graphChanged) {
  if (QApplication::activeWindow() == nullptr) {
    // the view may not have the correct size
    // if the call occurs before the first window activation
    // so we ensure it happens when everything is ok
    QTimer::singleShot(100, this, SLOT(delayedCenterView()));
    return;
  }

  float gvWidth = graphicsView()->width();
  // we apply a zoom factor to preserve a 50 px margin width
  // to ensure the scene will not be drawn under the configuration tabs title
  getGlMainWidget()->centerScene(graphChanged, (gvWidth - 50) / gvWidth);

  if (_overviewItem && _overviewItem->isVisible())
    drawOverview(graphChanged);
}

void GlMainView::delayedCenterView() {
  centerView();
}

void GlMainView::glMainViewDrawn(bool graphChanged) {
  if (_overviewItem && _overviewItem->isVisible())
    drawOverview(graphChanged);
}

QList<QWidget *> GlMainView::configurationWidgets() const {
  return QList<QWidget *>() << _sceneConfigurationWidget << _sceneLayersConfigurationWidget;
}

void GlMainView::updateShowOverviewButton() {
  if (_showOvButton == nullptr) {
    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget();
    _showOvButton = new QPushButton();
    _showOvButton->setMaximumSize(10, 10);
    _showOvButton->setCheckable(true);
    _showOvButton->setStyleSheet("QPushButton {font-family: Arial; font-size: 10pt; border:none};");
    proxy->setWidget(_showOvButton);
    addToScene(proxy);
    proxy->setZValue(10);
    connect(_showOvButton, SIGNAL(toggled(bool)), this, SLOT(setOverviewVisible(bool)));
  }

  _showOvButton->setVisible(_overviewPosition == OVERVIEW_BOTTOM_RIGHT);

  if (_showOvButton->isVisible()) {
    QRectF rect(QPoint(0, 0), graphicsView()->size());

    _showOvButton->blockSignals(true);

    if (_overviewItem && _overviewItem->isVisible()) {
      _showOvButton->setText("x");
      _showOvButton->setChecked(true);
      _showOvButton->setToolTip("Hide overview display");
      _showOvButton->move(
          rect.width() - _overviewItem->getWidth() - 1,
          rect.height() - _overviewItem->getHeight() -
              ((_quickAccessBar != nullptr) ? _quickAccessBarItem->size().height() : 0));
    } else {
      _showOvButton->setText("<");
      _showOvButton->setChecked(false);
      _showOvButton->setToolTip("Show overview display");
      _showOvButton->move(
          rect.width() - _showOvButton->width(),
          rect.height() - _overviewItem->getHeight() -
              ((_quickAccessBar != nullptr) ? _quickAccessBarItem->size().height() : 0));
    }

    _showOvButton->blockSignals(false);
  }
}

void GlMainView::setOverviewVisible(bool display) {
  drawOverview(true);
  _overviewItem->setVisible(display);
  updateShowOverviewButton();
}

bool GlMainView::overviewVisible() const {
  return (_overviewItem && _overviewItem->isVisible());
}

void GlMainView::setViewOrtho(bool viewOrtho) {
  getGlMainWidget()->getScene()->setViewOrtho(viewOrtho);
  _glMainWidget->draw(false);
}

void GlMainView::updateShowQuickAccessBarButton() {
  if (needQuickAccessBar) {
    if (_showQabButton == nullptr) {
      QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget();
      _showQabButton = new QPushButton();
      _showQabButton->setMaximumSize(10, 10);
      _showQabButton->setCheckable(true);
      _showQabButton->setStyleSheet(
          "QPushButton {font-family: Arial; font-size: 10pt; border:none};");
      proxy->setWidget(_showQabButton);
      addToScene(proxy);
      proxy->setZValue(10);
      connect(_showQabButton, SIGNAL(toggled(bool)), this, SLOT(setQuickAccessBarVisible(bool)));
    }

    QRectF rect(QPoint(0, 0), graphicsView()->size());

    _showQabButton->blockSignals(true);

    if (quickAccessBarVisible()) {
      _showQabButton->setText("x");
      _showQabButton->setChecked(true);
      _showQabButton->setToolTip("Hide quick access bar");
      _showQabButton->move(0, rect.height() - _quickAccessBarItem->size().height() - 4);
    } else {
      _showQabButton->setText("^");
      _showQabButton->setChecked(false);
      _showQabButton->setToolTip("Show quick access bar");
      _showQabButton->move(0, rect.height() - _showQabButton->height());
    }

    _showQabButton->blockSignals(false);
  }
}

QuickAccessBar *GlMainView::getQuickAccessBarImpl() {
  return new QuickAccessBarImpl(_quickAccessBarItem);
}

void GlMainView::setQuickAccessBarVisible(bool visible) {
  if (!visible) {
    delete _quickAccessBar;
    _quickAccessBar = nullptr;
    _quickAccessBarItem = nullptr;
  }

  else if (!quickAccessBarVisible()) {
    needQuickAccessBar = true;
    _quickAccessBarItem = new QGraphicsProxyWidget();
    _quickAccessBar = getQuickAccessBarImpl();
// workaround to get rid of Qt5 warning messages : "QMacCGContext:: Unsupported painter devtype type
// 1"
// see https://bugreports.qt.io/browse/QTBUG-32639
#if defined(__APPLE__) && QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    _quickAccessBar->setWindowOpacity(0.99);
#endif
    connect(_quickAccessBar, SIGNAL(settingsChanged()), _sceneConfigurationWidget,
            SLOT(resetChanges()));
    connect(_sceneConfigurationWidget, SIGNAL(settingsApplied()), _quickAccessBar, SLOT(reset()));

    _quickAccessBar->setGlMainView(this);
    _quickAccessBarItem->setWidget(_quickAccessBar);
    addToScene(_quickAccessBarItem);
    _quickAccessBarItem->setZValue(10);
  }

  sceneRectChanged(QRectF(QPoint(0, 0), graphicsView()->size()));
}

bool GlMainView::quickAccessBarVisible() const {
  return _quickAccessBarItem != nullptr;
}

void GlMainView::sceneRectChanged(const QRectF &rect) {
  if (_quickAccessBar != nullptr) {
    _quickAccessBarItem->setPos(0, rect.height() - _quickAccessBarItem->size().height());
    _quickAccessBarItem->resize(rect.width(), _quickAccessBarItem->size().height());
  }

  updateShowQuickAccessBarButton();

  if (_overviewItem != nullptr) {
    // put overview in the bottom right corner
    if (_overviewPosition == OVERVIEW_BOTTOM_RIGHT)
      _overviewItem->setPos(
          rect.width() - _overviewItem->getWidth() - 1,
          rect.height() - _overviewItem->getHeight() -
              ((_quickAccessBarItem != nullptr) ? _quickAccessBarItem->size().height() : 0));
    else if (_overviewPosition == OVERVIEW_BOTTOM_LEFT)
      _overviewItem->setPos(
          0, rect.height() - _overviewItem->getHeight() -
                 ((_quickAccessBarItem != nullptr) ? _quickAccessBarItem->size().height() : 0));
    else if (_overviewPosition == OVERVIEW_TOP_LEFT)
      _overviewItem->setPos(0, 0);
    else if (_overviewPosition == OVERVIEW_TOP_RIGHT)
      _overviewItem->setPos(rect.width() - _overviewItem->getWidth() - 1, 0);

    updateShowOverviewButton();
  }

  GlLayer *fgLayer = getGlMainWidget()->getScene()->getLayer("Foreground");

  if (fgLayer) {
    Gl2DRect *labriLogo = static_cast<Gl2DRect *>(fgLayer->findGlEntity("labrilogo"));

    if (labriLogo) {
      labriLogo->setCoordinates((_quickAccessBar != nullptr) ? 35. : 0., 5., 50., 50.);
      draw();
    }
  }
}

QPixmap GlMainView::snapshot(const QSize &outputSize) const {
  if (_glMainWidget == nullptr)
    return QPixmap();

  QSize realSize = outputSize;

  if (!realSize.isValid())
    realSize = _glMainWidget->size();

  return QPixmap::fromImage(
      _glMainWidget->createPicture(realSize.width(), realSize.height(), false));
}

void GlMainView::openSnapshotDialog() {
  SnapshotDialog dlg(this, graphicsView()->window());
  dlg.exec();
}

void GlMainView::undoCallback() {
  float gvWidth = graphicsView()->width();
  // we apply a zoom factor to preserve a 50 px margin width
  // to ensure the scene will not be drawn under the configuration tabs title
  getGlMainWidget()->centerScene(true, (gvWidth - 50) / gvWidth);
  draw();
}

void GlMainView::fillContextMenu(QMenu *menu, const QPointF &) {
  menu->addAction(trUtf8("View"))->setEnabled(false);
  menu->addSeparator();
  menu->addAction(_forceRedrawAction);
  menu->addAction(_centerViewAction);

  QAction *viewOrtho = menu->addAction(trUtf8("Use orthogonal projection"));
  viewOrtho->setToolTip(QString("Enable to switch between true perspective and orthogonal"));
  viewOrtho->setCheckable(true);
  viewOrtho->setChecked(_glMainWidget->getScene()->isViewOrtho());
  connect(viewOrtho, SIGNAL(triggered(bool)), this, SLOT(setViewOrtho(bool)));

  QAction *antiAliasing = menu->addAction(trUtf8("Anti-aliasing"));
  antiAliasing->setToolTip(QString("Improve line rendering quality"));
  antiAliasing->setCheckable(true);
  antiAliasing->setChecked(OpenGlConfigManager::getInst().antiAliasing());
  connect(antiAliasing, SIGNAL(triggered(bool)), this, SLOT(setAntiAliasing(bool)));

  menu->addAction(_advAntiAliasingAction);
  menu->addAction(_snapshotAction);
  menu->addSeparator();
  menu->addAction(trUtf8("Augmented display"))->setEnabled(false);
  menu->addSeparator();

  QAction *a = menu->addAction(trUtf8("Show overview"), this, SLOT(setOverviewVisible(bool)));
  a->setToolTip(QString("Show/hide the overview in a corner of the view"));
  a->setCheckable(true);
  a->setChecked(overviewVisible());

  if (needQuickAccessBar) {
    QAction *quickbarAction = menu->addAction(trUtf8("Show quick access bar"), this,
                                              SLOT(setQuickAccessBarVisible(bool)));
    quickbarAction->setToolTip(QString("Show/hide the quick access bar"));
    quickbarAction->setCheckable(true);
    quickbarAction->setChecked(quickAccessBarVisible());
  }
}

void GlMainView::applySettings() {
  _sceneConfigurationWidget->applySettings();
}

void GlMainView::setAntiAliasing(bool aa) {
  OpenGlConfigManager::getInst().setAntiAliasing(aa);
  _advAntiAliasingAction->setVisible(aa);

  if (_advAntiAliasingAction->isChecked()) {
    _advAntiAliasingAction->setChecked(false);
  } else {
    draw();
  }
}

void GlMainView::setAdvancedAntiAliasing(bool aaa) {
  _glMainWidget->setAdvancedAntiAliasing(aaa);
  draw();
}

bool GlMainView::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::Resize) {
    // ensure automatic resize of the viewport
    QResizeEvent *resizeEvent = static_cast<QResizeEvent *>(event);
    graphicsView()->viewport()->setFixedSize(resizeEvent->size());
    // same for the configuration widgets
    QList<QWidget *> list = configurationWidgets();

    sceneRectChanged(QRectF(QPoint(0, 0), graphicsView()->size()));

    if (!list.isEmpty() &&
        list.first()->parentWidget()) { // test if the current view has a configuration widget
      QWidget *pqw = list.first()->parentWidget()->parentWidget();
      QSize sSize = pqw->size();
      sSize.setHeight(resizeEvent->size().height() - 50);
      pqw->resize(sSize);
      sSize.setHeight(resizeEvent->size().height() - 60);
      sSize = list.first()->size();

      foreach (QWidget *c, list) { // resize each configuration widget
        c->resize(sSize);
      }
    }

    return true;
  }

  // standard event processing
  return ViewWidget::eventFilter(obj, event);
}
