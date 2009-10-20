#include <QtOpenGL/QGLFramebufferObject>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QApplication>

#include <tulip/GlTextureManager.h>
#include <tulip/GlRectTextured.h>
#include <tulip/GlQuad.h>
#include <tulip/GlTools.h>

#include <sstream>

#include "tulip/GlMainWidgetItem.h"

using namespace std;

namespace tlp {

static GLuint copyFboTexture(QGLFramebufferObject *glFrameBuf) {

	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);


	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	unsigned char* buff = new unsigned char[glFrameBuf->width() * glFrameBuf->height() * 4];
	glBindTexture(GL_TEXTURE_2D, glFrameBuf->texture());
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buff);

	glBindTexture(GL_TEXTURE_2D, textureId);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, glFrameBuf->width(), glFrameBuf->height(), GL_RGBA, GL_UNSIGNED_BYTE, buff);

	delete [] buff;

	return textureId;
}

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
	if (fbo1)
		delete fbo1;

	if(fbo2)
	  delete fbo2;
}

const float offset = 2;

QRectF GlMainWidgetItem::boundingRect() const {
  return QRectF(-width / 2, -height / 2, width, height);
}

void GlMainWidgetItem::glMainWidgetDraw(GlMainWidget *,bool){
  redrawNeed=true;
  parent->scene()->update();
}

void GlMainWidgetItem::glMainWidgetRedraw(GlMainWidget *){
  parent->scene()->update();
}


void GlMainWidgetItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  QRectF rect = boundingRect().translated(pos());

  vpWidth = painter->device()->width();
  vpHeight = painter->device()->height();

  float left = 2.0f * float(rect.left()) / vpWidth - 1.0f;
  float top = 1.0f - 2.0f * float(rect.top()) / vpHeight;

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

    GlTextureManager::getInst().deleteTexture(oss1.str());
    GlTextureManager::getInst().registerExternalTexture(oss1.str(), copyFboTexture(fbo1));

    redrawNeed=false;
  }

  fbo2->bind();
  glMainWidget->getScene()->initGlParameters();
  Camera newCamera(glMainWidget->getScene(),false);
  newCamera.initGl();
  glClearColor(255, 255, 255, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

  GlTextureManager::getInst().deleteTexture(oss2.str());
  GlTextureManager::getInst().registerExternalTexture(oss2.str(), copyFboTexture(fbo2));

  rect = QRectF(left - width / 2, top - height / 2, width, height);

  if(decorate){
    QPainterPath path;
    path.addRect(rect.x()-offset,rect.y()-offset,rect.width()+offset*2,rect.height()+offset*2);

    painter->setBrush(Qt::black);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->drawPath(path);
    painter->setRenderHint(QPainter::Antialiasing, false);
  }


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
}

void GlMainWidgetItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  QMouseEvent *eventModif=new QMouseEvent(QEvent::MouseMove,QPoint(event->pos().x()+((float)width)/2.,event->pos().y()+((float)height)/2.), Qt::NoButton, event->buttons(), event->modifiers());
  QApplication::sendEvent(glMainWidget,eventModif);
}

void GlMainWidgetItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QMouseEvent *eventModif=new QMouseEvent(QEvent::MouseButtonPress,QPoint(event->pos().x()+((float)width)/2.,event->pos().y()+((float)height)/2.), event->button(), event->buttons(), event->modifiers());
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
