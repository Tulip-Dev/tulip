#include <QtOpenGL/QGLFramebufferObject>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QApplication>

#include <tulip/GlTextureManager.h>
#include <tulip/GlQuad.h>
#include <tulip/GlTools.h>

#include <sstream>

#include "tulip/GlMainWidgetItem.h"

using namespace std;

namespace tlp {

GlMainWidgetItem::GlMainWidgetItem(GlMainWidgetGraphicsView *parent,GlMainWidget *glMainWidget, int width, int height,QGraphicsItem *parentItem,bool decorate) :
	QGraphicsItem(parentItem),parent(parent),glMainWidget(glMainWidget),redrawNeed(true),decorate(decorate),fbo1(NULL),fbo2(NULL),width(width),height(height) {
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setAcceptHoverEvents(true);

	QObject::connect(glMainWidget,SIGNAL(viewDrawn(GlMainWidget *,bool)),this,SLOT(glMainWidgetDraw(GlMainWidget *,bool)));
	QObject::connect(glMainWidget,SIGNAL(viewRedrawn(GlMainWidget *)),this,SLOT(glMainWidgetRedraw(GlMainWidget *)));
}

GlMainWidgetItem::~GlMainWidgetItem() {
  delete fbo1;
  delete fbo2;
}

const float offset = 2;

QRectF GlMainWidgetItem::boundingRect() const {
  return QRectF(-width / 2.0f, -height / 2.0f, width, height);
}

void GlMainWidgetItem::glMainWidgetDraw(GlMainWidget *,bool){
  redrawNeed=true;
  parent->scene()->update();
}

void GlMainWidgetItem::glMainWidgetRedraw(GlMainWidget *){
  parent->scene()->update();
}


void GlMainWidgetItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  QRectF rect = boundingRect().translated(pos());

  float left = 2.0f * float(rect.left()) / width - 1.0f;
  float top = 1.0f - 2.0f * float(rect.top()) / height;

  ostringstream oss1;
  oss1 << "fbo1#" << (unsigned long) this;
  ostringstream oss2;
  oss2 << "fbo2#" << (unsigned long) this;

  if (fbo1 && (fbo1->width() != width || fbo1->height() != height)){
    delete fbo1;
    fbo1=NULL;
  }
  if(!fbo1)
    fbo1 = new QGLFramebufferObject(width, height, QGLFramebufferObject::CombinedDepthStencil);

  if (fbo2 && (fbo2->width() != width || fbo2->height() != height)){
    delete fbo2;
    fbo2=NULL;
  }
  if(!fbo2)
    fbo2 = new QGLFramebufferObject(width, height, QGLFramebufferObject::CombinedDepthStencil);

#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0))
        painter->beginNativePainting();
#endif

  glMainWidget->getScene()->setViewport(0,0,width, height);
  glMainWidget->resize(width,height);

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  if(redrawNeed){

    fbo1->bind();
    glMainWidget->getScene()->initGlParameters();
    glMainWidget->computeInteractors();
    glMainWidget->getScene()->draw();
    fbo1->release();

    GlTextureManager::getInst().registerExternalTexture(oss1.str(), fbo1->texture());

    redrawNeed=false;
  }

  fbo2->bind();
  glMainWidget->getScene()->initGlParameters();
  Camera newCamera(glMainWidget->getScene(),false);
  newCamera.initGl();
  glClearColor(255, 255, 255, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  glDisable(GL_BLEND);
  setMaterial(Color(255,255,255,255));

  GlTextureManager::getInst().activateTexture(oss1.str());
  glBegin(GL_QUADS);
  glNormal3f(0.0f, 0.0f, 1.0f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(rect.left(), rect.top(), 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(rect.right(), rect.top(), 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(rect.right(), rect.bottom(), 0);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(rect.left(), rect.bottom(), 0);
  glEnd();
  GlTextureManager::getInst().desactivateTexture();
  glEnable(GL_BLEND);

  glMainWidget->drawForegroundEntities();
  glMainWidget->drawInteractors();
  fbo2->release();

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glPopAttrib();

  GlTextureManager::getInst().registerExternalTexture(oss2.str(), fbo2->texture());

#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0))
        painter->endNativePainting();
#endif

  rect = QRectF(left - width / 2.0f, top - height / 2.0f, width, height);

  if(decorate){
    QPainterPath path;
    path.addRect(rect.x()-offset,rect.y()-offset,rect.width()+offset*2,rect.height()+offset*2);

    painter->setBrush(Qt::black);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->drawPath(path);
    painter->setRenderHint(QPainter::Antialiasing, false);
  }

#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0))
        painter->beginNativePainting();
#endif

  glDisable(GL_BLEND);
  setMaterial(Color(255,255,255,255));
  GlTextureManager::getInst().activateTexture(oss2.str());
  glBegin(GL_QUADS);
  glNormal3f(0.0f, 0.0f, 1.0f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(rect.left(), rect.top(), 0);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(rect.right(), rect.top(), 0);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(rect.right(), rect.bottom(), 0);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(rect.left(), rect.bottom(), 0);
  glEnd();
  glEnable(GL_BLEND);
  GlTextureManager::getInst().desactivateTexture();

#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0))
        painter->endNativePainting();
#endif
}

void GlMainWidgetItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  QMouseEvent *eventModif=new QMouseEvent(QEvent::MouseMove,QPoint(event->pos().x()+((float)width)/2.,event->pos().y()+((float)height)/2.), Qt::NoButton, event->buttons(), event->modifiers());
  QApplication::sendEvent(glMainWidget,eventModif);
}

void GlMainWidgetItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QMouseEvent *eventModif=new QMouseEvent(QEvent::MouseButtonPress,QPoint(event->pos().x()+((float)width)/2.,event->pos().y()+((float)height)/2.), event->button(), event->buttons(), event->modifiers());
  QApplication::sendEvent(glMainWidget,eventModif);
}

void GlMainWidgetItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
	QMouseEvent *eventModif=new QMouseEvent(QEvent::MouseButtonDblClick,QPoint(event->pos().x()+((float)width)/2.,event->pos().y()+((float)height)/2.), event->button(), event->buttons(), event->modifiers());
	QApplication::sendEvent(glMainWidget,eventModif);
}

void GlMainWidgetItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  QMouseEvent *eventModif=new QMouseEvent(QEvent::MouseButtonRelease,QPoint(event->pos().x()+((float)width)/2.,event->pos().y()+((float)height)/2.), event->button(), event->buttons(), event->modifiers());
  QApplication::sendEvent(glMainWidget,eventModif);
}

void GlMainWidgetItem::wheelEvent(QGraphicsSceneWheelEvent *event) {
  QWheelEvent *eventModif=new QWheelEvent(QPoint(event->pos().x()+width/2,event->pos().y()+height/2), event->delta(),event->buttons(), event->modifiers(),event->orientation());
  QApplication::sendEvent(glMainWidget,eventModif);
}

void GlMainWidgetItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event){
  QMouseEvent *eventModif=new QMouseEvent(QEvent::MouseMove,QPoint(event->pos().x()+((float)width)/2.,event->pos().y()+((float)height)/2.), Qt::NoButton, Qt::NoButton, event->modifiers());
  QApplication::sendEvent(glMainWidget,eventModif);
}

}
