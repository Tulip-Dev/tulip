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

#include "tulip/GlMainWidgetGraphicsItem.h"

#include <QGLFramebufferObject>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QGraphicsProxyWidget>
#include <QContextMenuEvent>
#include <QGraphicsScene>

#include <tulip/GlTextureManager.h>
#include <tulip/GlQuad.h>
#include <tulip/GlTools.h>
#include <tulip/GlMainWidget.h>

using namespace std;
using namespace tlp;

GlMainWidgetGraphicsItem::GlMainWidgetGraphicsItem(GlMainWidget *glMainWidget, int width,
                                                   int height)
    : QGraphicsObject(), glMainWidget(glMainWidget), _redrawNeeded(true), _graphChanged(true) {

  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setFlag(QGraphicsItem::ItemIsFocusable, true);
  setAcceptHoverEvents(true);
  setHandlesChildEvents(false);
  setAcceptDrops(true);

  connect(glMainWidget, SIGNAL(viewDrawn(GlMainWidget *, bool)), this,
          SLOT(glMainWidgetDraw(GlMainWidget *, bool)));
  connect(glMainWidget, SIGNAL(viewRedrawn(GlMainWidget *)), this,
          SLOT(glMainWidgetRedraw(GlMainWidget *)));

  resize(width, height);
  glMainWidget->installEventFilter(this);
  setHandlesChildEvents(false);
}

void GlMainWidgetGraphicsItem::setGlMainWidget(GlMainWidget *glmw) {
  if (glmw) {
    disconnect(glMainWidget, SIGNAL(viewDrawn(GlMainWidget *, bool)), this,
               SLOT(glMainWidgetDraw(GlMainWidget *, bool)));
    disconnect(glMainWidget, SIGNAL(viewRedrawn(GlMainWidget *)), this,
               SLOT(glMainWidgetRedraw(GlMainWidget *)));
    glMainWidget->removeEventFilter(this);
    glMainWidget = glmw;
    connect(glMainWidget, SIGNAL(viewDrawn(GlMainWidget *, bool)), this,
            SLOT(glMainWidgetDraw(GlMainWidget *, bool)));
    connect(glMainWidget, SIGNAL(viewRedrawn(GlMainWidget *)), this,
            SLOT(glMainWidgetRedraw(GlMainWidget *)));
    glMainWidget->installEventFilter(this);
  }
}

GlMainWidgetGraphicsItem::~GlMainWidgetGraphicsItem() {
  delete glMainWidget;
}

QRectF GlMainWidgetGraphicsItem::boundingRect() const {
  return QRectF(0, 0, width, height);
}

void GlMainWidgetGraphicsItem::resize(int width, int height) {

  this->width = width;
  this->height = height;
  glMainWidget->resize(width, height);
  glMainWidget->resizeGL(width, height);
  _redrawNeeded = true;
  _graphChanged = true;
  prepareGeometryChange();
}

void GlMainWidgetGraphicsItem::glMainWidgetDraw(GlMainWidget *, bool graphChanged) {
  _redrawNeeded = true;
  _graphChanged = graphChanged;
  update();
}

void GlMainWidgetGraphicsItem::glMainWidgetRedraw(GlMainWidget *) {
  update();
}

void GlMainWidgetGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                                     QWidget *) {

  if (_redrawNeeded) {
    emit widgetPainted(_graphChanged);
  }

  painter->beginNativePainting();

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  if (_redrawNeeded) {
    glMainWidget->render(GlMainWidget::RenderingOptions(GlMainWidget::RenderScene), false);
    _redrawNeeded = false;
  } else {
    glMainWidget->render(GlMainWidget::RenderingOptions(), false);
  }

  glFlush();

  glPopAttrib();

  painter->endNativePainting();
}

void GlMainWidgetGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  QMouseEvent eventModif(QEvent::MouseMove, QPoint(event->pos().x(), event->pos().y()),
                         Qt::NoButton, event->buttons(), event->modifiers());
  QApplication::sendEvent(glMainWidget, &eventModif);
  event->setAccepted(eventModif.isAccepted());
}

void GlMainWidgetGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QMouseEvent eventModif(QEvent::MouseButtonPress, QPoint(event->pos().x(), event->pos().y()),
                         event->button(), event->buttons(), event->modifiers());
  QApplication::sendEvent(glMainWidget, &eventModif);
  event->setAccepted(eventModif.isAccepted());
}

void GlMainWidgetGraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
  QMouseEvent eventModif(QEvent::MouseButtonDblClick, QPoint(event->pos().x(), event->pos().y()),
                         event->button(), event->buttons(), event->modifiers());
  QApplication::sendEvent(glMainWidget, &eventModif);
  event->setAccepted(eventModif.isAccepted());
}

void GlMainWidgetGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  QMouseEvent eventModif(QEvent::MouseButtonRelease, QPoint(event->pos().x(), event->pos().y()),
                         event->button(), event->buttons(), event->modifiers());
  QApplication::sendEvent(glMainWidget, &eventModif);
  event->setAccepted(eventModif.isAccepted());
}

void GlMainWidgetGraphicsItem::wheelEvent(QGraphicsSceneWheelEvent *event) {
  QWheelEvent eventModif(QPoint(event->pos().x(), event->pos().y()), event->delta(),
                         event->buttons(), event->modifiers(), event->orientation());
  QApplication::sendEvent(glMainWidget, &eventModif);
  event->setAccepted(eventModif.isAccepted());
}

void GlMainWidgetGraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
  QMouseEvent eventModif(QEvent::MouseMove, QPoint(event->pos().x(), event->pos().y()),
                         Qt::NoButton, Qt::NoButton, event->modifiers());
  QApplication::sendEvent(glMainWidget, &eventModif);
  event->setAccepted(eventModif.isAccepted());
}

void GlMainWidgetGraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
  QContextMenuEvent eventModif(static_cast<QContextMenuEvent::Reason>(event->reason()),
                               QPoint(event->pos().x(), event->pos().y()));
  QApplication::sendEvent(glMainWidget, &eventModif);
  event->setAccepted(eventModif.isAccepted());
}

void GlMainWidgetGraphicsItem::keyReleaseEvent(QKeyEvent *event) {
  QKeyEvent eventModif(event->type(), event->key(), event->modifiers(), event->text(),
                       event->isAutoRepeat(), event->count());
  QApplication::sendEvent(glMainWidget, &eventModif);
  event->setAccepted(eventModif.isAccepted());
}

void GlMainWidgetGraphicsItem::keyPressEvent(QKeyEvent *event) {
  QKeyEvent eventModif(event->type(), event->key(), event->modifiers(), event->text(),
                       event->isAutoRepeat(), event->count());
  QApplication::sendEvent(glMainWidget, &eventModif);
  event->setAccepted(eventModif.isAccepted());
}

void GlMainWidgetGraphicsItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
  if (glMainWidget->isEnabled() && glMainWidget->acceptDrops()) {
    QDragEnterEvent proxyDragEnter(event->pos().toPoint(), event->dropAction(), event->mimeData(),
                                   event->buttons(), event->modifiers());
    proxyDragEnter.setAccepted(event->isAccepted());
    QApplication::sendEvent(glMainWidget, &proxyDragEnter);
    event->setAccepted(proxyDragEnter.isAccepted());
    event->setDropAction(proxyDragEnter.dropAction());
  } else {
    event->ignore();
  }
}
void GlMainWidgetGraphicsItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *event) {
  QDragLeaveEvent proxyDragLeave;
  QApplication::sendEvent(glMainWidget, &proxyDragLeave);
  event->setAccepted(proxyDragLeave.isAccepted());
}

void GlMainWidgetGraphicsItem::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {
  if (glMainWidget->isEnabled() && glMainWidget->acceptDrops()) {
    QDragMoveEvent dragMove(event->pos().toPoint(), event->possibleActions(), event->mimeData(),
                            event->buttons(), event->modifiers());
    QApplication::sendEvent(glMainWidget, &dragMove);
    event->setAccepted(dragMove.isAccepted());
    event->setDropAction(dragMove.dropAction());
  } else {
    event->ignore();
  }
}

void GlMainWidgetGraphicsItem::dropEvent(QGraphicsSceneDragDropEvent *event) {
  QDropEvent dropEvent(event->pos().toPoint(), event->possibleActions(), event->mimeData(),
                       event->buttons(), event->modifiers());
  QApplication::sendEvent(glMainWidget, &dropEvent);
  event->setAccepted(dropEvent.isAccepted());
}

bool GlMainWidgetGraphicsItem::eventFilter(QObject *, QEvent *evt) {
  if (evt->type() == QEvent::CursorChange)
    setCursor(glMainWidget->cursor());

// There is a bug with Qt5 on windows that leads to an incorrect viewport size of 160x160
// when initializing a Tulip OpenGL view.
// Seems that a delayed resize event is sent to the GlMainWidget, don't really know why ...
// In the context of a GlMainWidgetGraphicsItem, the internally used GlMainWidget is hidden,
// so it should not receive that event but it does once after it has been created.
// As a workaround to fix that annoying behaviour, catch that resize event, discard it
// and restore the correct size that was previously set.
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)) && defined(WIN32)

  if (evt->type() == QEvent::Resize) {
    glMainWidget->resize(width, height);
    return true;
  }

#endif
  return false;
}
