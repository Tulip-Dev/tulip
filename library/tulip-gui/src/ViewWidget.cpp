#include "tulip/ViewWidget.h"

#include <QtGui/QApplication>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsProxyWidget>
#include <tulip/GlMainWidget.h>
#include <tulip/GlMainWidgetGraphicsItem.h>
#include <tulip/Interactor.h>

using namespace tlp;

static QGLFormat GlInit() {
  QGLFormat tmpFormat;
  tmpFormat.setDirectRendering(true);
  tmpFormat.setDoubleBuffer(true);
  tmpFormat.setAccum(false);
  tmpFormat.setStencil(true);
  tmpFormat.setOverlay(false);
  tmpFormat.setDepth(true);
  tmpFormat.setRgba(true);
  tmpFormat.setAlpha(true);
  tmpFormat.setOverlay(false);
  tmpFormat.setStereo(false);
  tmpFormat.setSampleBuffers(true);
  return tmpFormat;
}

ViewWidget::ViewWidget()
  : View(), _graphicsView(NULL), _centralWidget(NULL), _centralWidgetItem(NULL) {
}

ViewWidget::~ViewWidget() {
}

QGraphicsView* ViewWidget::graphicsView() const {
  return _graphicsView;
}

void ViewWidget::setupUi() {
  _graphicsView = new QGraphicsView(new QGraphicsScene());
  _graphicsView->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
  _graphicsView->setViewport(new QGLWidget(GlInit(), 0, GlMainWidget::getFirstQGLWidget()));
  _graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  _graphicsView->setFrameStyle(QFrame::NoFrame);
  _graphicsView->scene()->setBackgroundBrush(Qt::white);
  _graphicsView->installEventFilter(this);
  setupWidget();
  assert(_centralWidget);
}

void ViewWidget::currentInteractorChanged(tlp::Interactor *i) {
  i->install(_centralWidget);
}

void ViewWidget::setCentralWidget(QWidget* w) {
  assert(w);
  QGraphicsItem *oldCentralItem = _centralWidgetItem;
  QWidget *oldCentralWidget = _centralWidget;

  if (currentInteractor())
    currentInteractor()->uninstall();

  _centralWidget = w;

  if (currentInteractor())
    currentInteractor()->install(w);

  if (_centralWidgetItem)
    _graphicsView->scene()->removeItem(_centralWidgetItem);

  GlMainWidget *glMainWidget = dynamic_cast<GlMainWidget *>(w);

  if (glMainWidget) {
    _centralWidgetItem = new GlMainWidgetGraphicsItem(glMainWidget, QSize(_graphicsView->width(), _graphicsView->height()));
    _graphicsView->scene()->addItem(_centralWidgetItem);
  }
  else {
    _graphicsView->setViewport(new QGLWidget(GlInit(), 0, 0));
    _graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    _centralWidgetItem = _graphicsView->scene()->addWidget(w);
    _centralWidget->resize(_graphicsView->width(),_graphicsView->height());
  }

  _centralWidgetItem->setPos(0,0);
  _centralWidgetItem->setZValue(0);

  refreshItemsParenthood();

  delete oldCentralItem;
  delete oldCentralWidget;
}

bool ViewWidget::eventFilter(QObject *obj, QEvent *e) {
  QResizeEvent *event = dynamic_cast<QResizeEvent *>(e);

  if (event && obj == _graphicsView) {
    if (_graphicsView->scene())
      _graphicsView->scene()->setSceneRect(QRect(QPoint(0, 0), _graphicsView->size()));

    GlMainWidgetGraphicsItem *glMainWidgetItem = dynamic_cast<GlMainWidgetGraphicsItem *>(_centralWidgetItem);

    if (glMainWidgetItem)
      glMainWidgetItem->resize(QSize(_graphicsView->width(), _graphicsView->height()));

    else if (_centralWidget)
      _centralWidget->resize(_graphicsView->size());

    if (_graphicsView->scene())
      _graphicsView->scene()->update();

    // Hack : send a mouse event to force redraw of the scene (otherwise artifacts was displayed when maximizing or minimizing the graphics view)
    QMouseEvent *eventModif = new QMouseEvent(QEvent::MouseMove,QPoint(_graphicsView->size().width()/2, _graphicsView->size().height()/2), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
    QApplication::sendEvent(_graphicsView, eventModif);
    return true;
  }

  return false;
}

void ViewWidget::addToScene(QGraphicsItem *item) {
  if (_items.contains(item)) {
#ifndef NDEBUG
    qWarning("Trying to double-add an item to an AbstractGraphicsView");
#endif
    return;
  }

  _items.insert(item);
  item->setParentItem(_centralWidgetItem);
}

void ViewWidget::removeFromScene(QGraphicsItem *item) {
  if (!_items.contains(item))
    return;

  _items.remove(item);

  if (_graphicsView->scene())
    _graphicsView->scene()->removeItem(item);
}

void ViewWidget::refreshItemsParenthood() {
  for (QSet<QGraphicsItem *>::iterator it = _items.begin(); it != _items.end(); ++it) {
    QGraphicsItem *item = *it;
    item->setParentItem(_centralWidgetItem);
  }
}
