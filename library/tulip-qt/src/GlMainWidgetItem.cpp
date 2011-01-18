/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include <QtOpenGL/QGLFramebufferObject>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QApplication>
#include <QtGui/QGraphicsProxyWidget>
#include <QtGui/QContextMenuEvent>


#include <tulip/GlTextureManager.h>
#include <tulip/GlQuad.h>
#include <tulip/GlTools.h>

#include <sstream>

#include "tulip/GlMainWidgetItem.h"

using namespace std;

static void setRasterPosition(unsigned int x, unsigned int y) {
	//  std::cerr << __PRETTY_FUNCTION__ << std::endl;
	float val[4];
	unsigned char tmp[10];
	glGetFloatv(GL_CURRENT_RASTER_POSITION, (float*)&val);
	glBitmap(0,0,0,0,-val[0] + x, -val[1] + y, tmp);
	glGetFloatv(GL_CURRENT_RASTER_POSITION, (float*)&val);
	tlp::glTest(__PRETTY_FUNCTION__);
}

namespace tlp {

GlMainWidgetItem::GlMainWidgetItem(GlMainWidget *glMainWidget, int width, int height,bool decorate,const QColor &frameColor, float borderWidth) : QGraphicsItem(),
		glMainWidget(glMainWidget), frameColor(frameColor), redrawNeeded(true),decorate(decorate), borderWidth(borderWidth),renderingStore(NULL) {
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setAcceptHoverEvents(true);

	lockedCB = new QCheckBox("locked");
	lockedCB->setChecked(true);
	if (decorate) {
		lockedCB->resize(60, borderWidth - 2);
		lockedCB->move(1, 1);
		lockedCB->setPalette(frameColor);
		QGraphicsProxyWidget *cbProxy = new QGraphicsProxyWidget(this);
		cbProxy->setWidget(lockedCB);
	}

	QObject::connect(glMainWidget,SIGNAL(viewDrawn(GlMainWidget *,bool)),this,SLOT(glMainWidgetDraw(GlMainWidget *,bool)));
	QObject::connect(glMainWidget,SIGNAL(viewRedrawn(GlMainWidget *)),this,SLOT(glMainWidgetRedraw(GlMainWidget *)));

	resize(width, height);

	glMainWidget->installEventFilter(this);
}

GlMainWidgetItem::~GlMainWidgetItem() {
	delete lockedCB;
	delete [] renderingStore;
	glMainWidget->removeEventFilter(this);
}

QRectF GlMainWidgetItem::boundingRect() const {
	if (decorate) {
		return QRectF(0, 0, width + 2* borderWidth, height+ 2* borderWidth);
	} else {
		return QRectF(0, 0, width, height);
	}
}

void GlMainWidgetItem::resize(int width, int height) {
	this->width = width;
	this->height = height;
	glMainWidget->resize(width,height);
	glMainWidget->resizeGL(width,height);
	redrawNeeded = true;
	delete [] renderingStore;
	renderingStore = new unsigned char[width*height*4];
	prepareGeometryChange();
}

void GlMainWidgetItem::glMainWidgetDraw(GlMainWidget *,bool){
	redrawNeeded=true;
	if (scene())
		scene()->update();
}

void GlMainWidgetItem::glMainWidgetRedraw(GlMainWidget *){
	if (scene())
		scene()->update();
}


void GlMainWidgetItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
	QRectF rect = boundingRect();

	if (pos().x() < 0 || pos().x()+rect.width() > scene()->width() || pos().y() < 0 || pos().y()+rect.height() > scene()->height()) {
		redrawNeeded = true;
	}

	if(decorate){
		QPainterPath path;
		path.addRect(rect.x(),rect.y(),rect.width(),rect.height());
		painter->setBrush(frameColor);
		painter->setRenderHint(QPainter::Antialiasing, true);
		painter->drawPath(path);
		painter->setRenderHint(QPainter::Antialiasing, false);
	}

	QPainterPath path;
	if (decorate) {
		path.addRect(rect.x()+borderWidth,rect.y()+borderWidth,rect.width()-2*borderWidth,rect.height()-2*borderWidth);
	} else {
		path.addRect(rect.x(),rect.y(),rect.width(),rect.height());
	}
	Color backgroundColor = glMainWidget->getScene()->getBackgroundColor();

	painter->setBrush(QColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]));
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->drawPath(path);
	painter->setRenderHint(QPainter::Antialiasing, false);

#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0))
	painter->beginNativePainting();
#endif


	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	rect = rect.translated(pos());

	float vpX = rect.x();
	float vpY = scene()->height() - (rect.y() + rect.height());
	float vpW = rect.width();
	float vpH = rect.height();


	if (decorate) {
		vpX += borderWidth;
		vpY += borderWidth;
		vpW -= 2*borderWidth;
		vpH -= 2*borderWidth;
	}

	glMainWidget->getScene()->setViewport(vpX,vpY,vpW,vpH);
	glMainWidget->getScene()->setNoClearBackground(true);
	glMainWidget->getScene()->initGlParameters();

	if(redrawNeeded){

		glMainWidget->computeInteractors();
		glMainWidget->getScene()->draw();

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_LIGHTING);

		glReadBuffer(GL_BACK);
		glReadPixels(vpX,vpY,vpW,vpH,GL_RGBA,GL_UNSIGNED_BYTE,renderingStore);
		glFlush();

		redrawNeeded=false;
	} else {

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_LIGHTING);

		glDrawBuffer(GL_BACK);
		setRasterPosition(vpX,vpY);
		glDrawPixels(vpW,vpH,GL_RGBA,GL_UNSIGNED_BYTE,renderingStore);
		glFlush();
	}

	glMainWidget->drawForegroundEntities();
	glMainWidget->drawInteractors();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopAttrib();

#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0))
	painter->endNativePainting();
#endif

}

void GlMainWidgetItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	float offset = 0;
	if (decorate) {
		offset = -borderWidth;
	}
	if (lockedCB->isChecked()) {
		QPoint point(event->pos().x()+offset,event->pos().y()+offset);
		if (! (point.x() < 0 || point.x() > width || point.y() < 0 || point.y() > height)) {
			QMouseEvent *eventModif=new QMouseEvent(QEvent::MouseMove,QPoint(event->pos().x()+offset,event->pos().y()+offset), Qt::NoButton, event->buttons(), event->modifiers());
			QApplication::sendEvent(glMainWidget,eventModif);
		}
	} else {
		QGraphicsItem::mouseMoveEvent(event);
	}
}

void GlMainWidgetItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	float offset = 0;
	if (decorate) {
		offset = -borderWidth;
	}
	if (lockedCB->isChecked()) {
		QPoint point(event->pos().x()+offset,event->pos().y()+offset);
		if (! (point.x() < 0 || point.x() > width || point.y() < 0 || point.y() > height)) {
			QMouseEvent *eventModif=new QMouseEvent(QEvent::MouseButtonPress,QPoint(event->pos().x()+offset,event->pos().y()+offset), event->button(), event->buttons(), event->modifiers());
			QApplication::sendEvent(glMainWidget,eventModif);
		}
	} else {
		QGraphicsItem::mousePressEvent(event);
	}
}

void GlMainWidgetItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
	float offset = 0;
	if (decorate) {
		offset = -borderWidth;
	}

	if (lockedCB->isChecked()) {
		QPoint point(event->pos().x()+offset,event->pos().y()+offset);
		if (! (point.x() < 0 || point.x() > width || point.y() < 0 || point.y() > height)) {
			QMouseEvent *eventModif=new QMouseEvent(QEvent::MouseButtonDblClick,QPoint(event->pos().x()+offset,event->pos().y()+offset), event->button(), event->buttons(), event->modifiers());
			QApplication::sendEvent(glMainWidget,eventModif);
		}
	}
}

void GlMainWidgetItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	float offset = 0;
	if (decorate) {
		offset = -borderWidth;
	}
	if (lockedCB->isChecked()) {
		QPoint point(event->pos().x()+offset,event->pos().y()+offset);
		if (! (point.x() < 0 || point.x() > width || point.y() < 0 || point.y() > height)) {
			QMouseEvent *eventModif=new QMouseEvent(QEvent::MouseButtonRelease,QPoint(event->pos().x()+offset,event->pos().y()+offset), event->button(), event->buttons(), event->modifiers());
			QApplication::sendEvent(glMainWidget,eventModif);
		}
	} else {
		QGraphicsItem::mouseReleaseEvent(event);
	}
}

void GlMainWidgetItem::wheelEvent(QGraphicsSceneWheelEvent *event) {
	float offset = 0;
	if (decorate) {
		offset = -borderWidth;
	}
	QWheelEvent *eventModif=new QWheelEvent(QPoint(event->pos().x()+offset,event->pos().y()+offset), event->delta(),event->buttons(), event->modifiers(),event->orientation());
	QApplication::sendEvent(glMainWidget,eventModif);
}

void GlMainWidgetItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event){
	float offset = 0;
	if (decorate) {
		offset = -borderWidth;
	}
	QMouseEvent *eventModif=new QMouseEvent(QEvent::MouseMove,QPoint(event->pos().x()+offset,event->pos().y()+offset), Qt::NoButton, Qt::NoButton, event->modifiers());
	QApplication::sendEvent(glMainWidget,eventModif);
}

void GlMainWidgetItem::contextMenuEvent(QGraphicsSceneContextMenuEvent * event) {
	float offset = 0;
	if (decorate) {
		offset = -borderWidth;
	}
	QContextMenuEvent *eventModif = new QContextMenuEvent(static_cast<QContextMenuEvent::Reason>(event->reason()), QPoint(event->pos().x()+offset,event->pos().y()+offset));
	QApplication::sendEvent(glMainWidget,eventModif);
}

bool GlMainWidgetItem::eventFilter(QObject *, QEvent *evt) {
	if (evt->type() == QEvent::CursorChange) {
		setCursor(glMainWidget->cursor());
	}
	return false;
}


}
