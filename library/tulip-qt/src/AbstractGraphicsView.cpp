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

// FIXME: remove me
#include "tulip/PushButtonItem.h"
#include <QtGui/QGraphicsLayout>
#include <QtGui/QGraphicsWidget>
#include <QtGui/QGraphicsLinearLayout>
#include "tulip/TulipGraphicsLayout.h"

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
  _centralView(new QGraphicsView()), _mainLayout(0), _centralWidget(0), _mainWidget(0), _centralWidgetItem(0), _interactorsToolbar(0) {
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
  _interactorsToolbar = buildInteractorsToolbar();
  if (_interactorsToolbar) {
    _interactorsToolbar->setParentItem(0);
    _centralView->scene()->addItem(_interactorsToolbar);
  }
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

  if (dynamic_cast<GlMainWidget *>(_centralWidget)) {
    disconnect(_centralWidget, SIGNAL(viewDrawn(GlMainWidget *,bool)), this, SLOT(updateCentralView()));
    disconnect(_centralWidget, SIGNAL(viewRedrawn(GlMainWidget *)), this, SLOT(updateCentralView()));
  }

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
//    _centralView->scene()->addItem(_centralWidgetItem);
    connect(_centralWidget, SIGNAL(viewDrawn(GlMainWidget *,bool)), this, SLOT(updateCentralView()));
    connect(_centralWidget, SIGNAL(viewRedrawn(GlMainWidget *)), this, SLOT(updateCentralView()));
  }
  else {
    _centralView->setViewport(new QGLWidget(GlInit(), 0, 0));
    _centralView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    _centralWidgetItem = _centralView->scene()->addWidget(w);
    _centralWidget->resize(_centralView->width(),_centralView->height());
  }
  _centralWidgetItem->setPos(0,0);
  _centralWidgetItem->setZValue(0);

  if (_interactorsToolbar)
    _interactorsToolbar->setParentItem(_centralWidgetItem); // Refresh parenthood between central widget item and toolbar

  delete oldCentralItem;
  delete oldCentralWidget;
}
// ===================================
QGraphicsItem *AbstractGraphicsView::buildInteractorsToolbar() {
  QGraphicsWidget *w = new QGraphicsWidget;
  TulipGraphicsLayout *layout = new TulipGraphicsLayout("Circular");
  w->setLayout(layout);
  for (int i=0; i < 10; ++i) {
    PushButtonItem *item = new PushButtonItem("D:/dev/tulip-3.5-maint/testing/controllers/TulipLite/designer/dialog-apply.svg");
    _centralView->scene()->addItem(item);
    layout->addItem(item);
  }
  _centralView->scene()->addItem(w);
  return 0;
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
void AbstractGraphicsView::updateCentralView() {
  dynamic_cast<GlMainWidgetGraphicsItem *>(_centralWidgetItem)->setRedrawNeeded(true);
  _centralView->scene()->update();
}
} /* namespace tlp_new */
