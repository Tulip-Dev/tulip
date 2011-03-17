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
  _centralView(0), _mainLayout(0), _centralWidget(0), _mainWidget(0), _centralWidgetItem(0), _viewportWidget(0) {
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

  _centralView = new QGraphicsView();
  _mainLayout->addWidget(_centralView);
  QGraphicsScene *scene = new QGraphicsScene();
  _centralView->setScene(scene);
  _centralView->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
  _viewportWidget = new QGLWidget(GlInit(), 0, GlMainWidget::getFirstQGLWidget());
  _centralView->setViewport(_viewportWidget);
  _centralView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  _centralView->setFrameStyle(QFrame::NoFrame);
  scene->setBackgroundBrush(Qt::white);
  _mainWidget->installEventFilter(this);

  QGraphicsRectItem *rect = new QGraphicsRectItem(640,20,50,50,0, scene);
  rect->setPen(QPen(Qt::red));
  rect->setBrush(Qt::red);

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
  buildInteractorsToolBar();
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
    _centralView->scene()->addItem(_centralWidgetItem);
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

  delete oldCentralItem;
  delete oldCentralWidget;
}
// ===================================
void AbstractGraphicsView::buildInteractorsToolBar() {

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
