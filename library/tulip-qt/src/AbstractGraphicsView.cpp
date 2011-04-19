#include "tulip/AbstractGraphicsView.h"

#include <tulip/Interactor.h>
#include <tulip/GlMainWidgetGraphicsItem.h>
#include <tulip/GlMainWidget.h>

#include <QtGui/QGraphicsView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QResizeEvent>
#include <QtGui/QApplication>
#include <QtGui/QGraphicsProxyWidget>
#include <QtOpenGL/QGLWidget>

#include "tulip/InteractorToolbarItem.h"

// FIXME: remove me
#include <QtGui/QGraphicsWidget>
#include <QtGui/QGraphicsLinearLayout>
#include <QtGui/QPushButton>
#include "tulip/PushButtonItem.h"
#include "tulip/TulipGraphicsLayout.h"
#include "tulip/ToolbarItem.h"
#include <QtGui/QAction>

using namespace tlp;
using namespace std;

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

namespace tlp {
AbstractGraphicsView::AbstractGraphicsView():
  _interactors(list<Interactor *>()), _activeInteractor(0),
  _centralView(new QGraphicsView()), _mainLayout(0), _centralWidget(0), _mainWidget(0), _centralWidgetItem(0) {
  _centralView->setScene(new QGraphicsScene());
}
// ===================================
AbstractGraphicsView::~AbstractGraphicsView() {
  delete _centralWidgetItem;
  delete _centralWidget;
}
// ===================================
QWidget *AbstractGraphicsView::construct(QWidget *parent) {
  _mainWidget = new QWidget(parent);
  _mainLayout = new QVBoxLayout;
  _mainLayout->setMargin(0);
  _mainLayout->setSpacing(0);
  _mainWidget->setLayout(_mainLayout);
  _mainLayout->addWidget(_centralView);
  _centralView->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
  _centralView->setViewport(new QGLWidget(GlInit(), 0, GlMainWidget::getFirstQGLWidget()));
  _centralView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  _centralView->setFrameStyle(QFrame::NoFrame);
  _centralView->scene()->setBackgroundBrush(Qt::white);
  _mainWidget->installEventFilter(this);
  return _mainWidget;
}
// ===================================
QWidget *AbstractGraphicsView::getWidget() {
  return _mainWidget;
}
// ===================================
bool AbstractGraphicsView::createPicture(const string &pictureName, int width, int height, bool center, int zoom, int xOffset, int yOffset) {
  return false;
}
// ===================================
void AbstractGraphicsView::setInteractors(const list<Interactor *> &interactors) {
  _interactors = interactors;
  for (list<Interactor *>::iterator it = _interactors.begin(); it != _interactors.end(); ++it)
    (*it)->setView(this);
  buildInteractorsToolbar();
}
// ===================================
list<Interactor *> AbstractGraphicsView::getInteractors() {
  return _interactors;
}
// ===================================
void AbstractGraphicsView::setActiveInteractor(Interactor *interactor) {
  if (_activeInteractor)
    _activeInteractor->remove();
  _activeInteractor = interactor;
  interactor->install(_centralWidget);
}
// ===================================
Interactor *AbstractGraphicsView::getActiveInteractor() {
  return _activeInteractor;
}
// ===================================
QWidget *AbstractGraphicsView::getCentralWidget() {
  return _centralWidget;
}
// ===================================
void AbstractGraphicsView::setCentralWidget(QWidget *w) {
  assert(w);
  QGraphicsItem *oldCentralItem = _centralWidgetItem;
  QWidget *oldCentralWidget = _centralWidget;

  if (_activeInteractor)
    _activeInteractor->remove();

  _centralWidget = w;

  if (_activeInteractor)
    _activeInteractor->install(w);

  if (!_centralView || !_centralView->scene())
    return;

  if (_centralWidgetItem)
    _centralView->scene()->removeItem(_centralWidgetItem);

  GlMainWidget *glMainWidget = dynamic_cast<GlMainWidget *>(w);
  if (glMainWidget) {
    _centralWidgetItem = new GlMainWidgetGraphicsItem(glMainWidget, _centralView->width(), _centralView->height());
    _centralView->scene()->addItem(_centralWidgetItem);
  }
  else {
    _centralView->setViewport(new QGLWidget(GlInit(), 0, 0));
    _centralView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    _centralWidgetItem = _centralView->scene()->addWidget(w);
    _centralWidget->resize(_centralView->width(),_centralView->height());
  }
  _centralWidgetItem->setPos(0,0);
  _centralWidgetItem->setZValue(0);

  refreshItemsParenthood();

  delete oldCentralItem;
  delete oldCentralWidget;
}
// ===================================
void AbstractGraphicsView::buildInteractorsToolbar() {
//  InteractorToolbarItem *toolbar = new InteractorToolbarItem(_interactors);
  ToolbarItem *tb = new ToolbarItem();
  for (list<Interactor *>::iterator it = _interactors.begin(); it != _interactors.end(); ++it)
    tb->addAction((*it)->getAction());
  addToScene(tb);
}
// ===================================
bool AbstractGraphicsView::eventFilter(QObject *obj, QEvent *e) {
  QResizeEvent *event = dynamic_cast<QResizeEvent *>(e);

  if (event && obj == _mainWidget) {
    _centralView->resize(_mainWidget->size());
    if (_centralView->scene())
      _centralView->scene()->setSceneRect(QRect(QPoint(0, 0), _centralView->size()));

    GlMainWidgetGraphicsItem *glMainWidgetItem = dynamic_cast<GlMainWidgetGraphicsItem *>(_centralWidgetItem);
    if (glMainWidgetItem)
      glMainWidgetItem->resize(_centralView->width(), _centralView->height());

    else if (_centralWidget)
      _centralWidget->resize(_centralView->size());

    if (_centralView->scene())
      _centralView->scene()->update();

    // Hack : send a mouse event to force redraw of the scene (otherwise artifacts was displayed when maximizing or minimizing the graphics view)
    QMouseEvent *eventModif = new QMouseEvent(QEvent::MouseMove,QPoint(_centralView->size().width()/2, _centralView->size().height()/2), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
    QApplication::sendEvent(_centralView, eventModif);
  }
}
// ===================================
void AbstractGraphicsView::addToScene(QGraphicsItem *item) {
  if (_items.contains(item)) {
#ifndef NDEBUG
    qWarning("Trying to double-add an iterm to an AbstractGraphicsView");
#endif
    return;
  }
  _items.insert(item);
  item->setParentItem(_centralWidgetItem);
}
// ===================================
void AbstractGraphicsView::removeFromScene(QGraphicsItem *item) {
  if (!_items.contains(item))
    return;
  _items.remove(item);
  if (_centralView->scene())
    _centralView->scene()->removeItem(item);
}
// ===================================
void AbstractGraphicsView::refreshItemsParenthood() {
  for (QSet<QGraphicsItem *>::iterator it = _items.begin(); it != _items.end(); ++it) {
    QGraphicsItem *item = *it;
    item->setParentItem(_centralWidgetItem);
  }
}
}
