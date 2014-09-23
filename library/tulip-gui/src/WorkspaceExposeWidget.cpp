/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#include "tulip/WorkspaceExposeWidget.h"

#include <QEvent>
#include <QDebug>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QGraphicsTextItem>

#include <tulip/View.h>
#include <tulip/WorkspacePanel.h>

#include <cmath>

using namespace tlp;

// Helper classes
QPixmap* PreviewItem::_closeButtonPixmap = NULL;
QRect PreviewItem::_closePixmapRect = QRect();

PreviewItem::PreviewItem(const QPixmap& pixmap, WorkspacePanel* panel, QGraphicsItem* parent)
  : QGraphicsObject(parent), _pixmap(pixmap), _panel(panel), _hovered(false), _closeButtonHovered(false) {
  if (_closeButtonPixmap == NULL) {
    _closeButtonPixmap = new QPixmap(":/tulip/gui/ui/darkclosebutton.png");
    _closePixmapRect = QRect(boundingRect().width()-_closeButtonPixmap->width() - 5,-0.5 * _closeButtonPixmap->height(),_closeButtonPixmap->width(),_closeButtonPixmap->height());
  }

  setFlag(ItemIsMovable);
  setFlag(ItemIsSelectable);
  setAcceptHoverEvents(true);
}
int PreviewItem::textHeight() const {
  QGraphicsTextItem text;
  QFont f;
  f.setBold(true);
  text.setFont(f);
  text.setPlainText(_panel->windowTitle());
  text.setTextWidth(WorkspaceExposeWidget::previewSize().width());
  return text.boundingRect().height();
}
QRectF PreviewItem::boundingRect() const {
  QRectF result = QRectF(0,0,WorkspaceExposeWidget::previewSize().width(),WorkspaceExposeWidget::previewSize().height()+textHeight());

  if (_hovered) {
    result.setTop(_closePixmapRect.top());
  }

  return result;
}
tlp::WorkspacePanel* PreviewItem::panel() const {
  return _panel;
}
void PreviewItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
  painter->drawPixmap(0,0,WorkspaceExposeWidget::previewSize().width(),WorkspaceExposeWidget::previewSize().height(),_pixmap);
  QFont f;
  f.setBold(true);
  painter->setFont(f);
  painter->drawText(0,WorkspaceExposeWidget::previewSize().height()+5,WorkspaceExposeWidget::previewSize().width(),textHeight(),Qt::AlignHCenter | Qt::TextWordWrap,_panel->windowTitle());

  if (_hovered) {
    painter->setOpacity(_closeButtonHovered ? 1 : 0.5);
    painter->drawPixmap(_closePixmapRect,*_closeButtonPixmap);
  }
}
void PreviewItem::hoverEnterEvent(QGraphicsSceneHoverEvent* /*event*/) {
  _hovered = true;
  prepareGeometryChange();
}
void PreviewItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* /*event*/) {
  _hovered = false;
  prepareGeometryChange();
}
void PreviewItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
  bool newCloseButtonHovered = _closePixmapRect.contains(event->pos().toPoint());

  if (newCloseButtonHovered != _closeButtonHovered) {
    _closeButtonHovered = newCloseButtonHovered;
    update();
  }
}
void PreviewItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* /*event*/) {
  emit opened();
}
bool PreviewItem::shouldClose(const QPointF& p) {
  return _closePixmapRect.contains(p.toPoint());
}

// *************************
QSize WorkspaceExposeWidget::previewSize() {
  return QSize(150,100);
}

const int WorkspaceExposeWidget::MARGIN = 50;

WorkspaceExposeWidget::WorkspaceExposeWidget(QWidget *parent): QGraphicsView(parent), _positionAnimation(NULL), _selectedItem(NULL), _placeholderItem(NULL), _switchToSingleMode(false) {
  setScene(new QGraphicsScene);
  setSceneRect(0,0,width(),height());
  setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

int WorkspaceExposeWidget::currentPanelIndex() const {
  return _currentPanelIndex;
}

QVector<WorkspacePanel*> WorkspaceExposeWidget::panels() const {
  QVector<WorkspacePanel*> result;
  foreach(PreviewItem* item, _items)
  result << item->panel();
  return result;
}

bool WorkspaceExposeWidget::isSwitchToSingleMode() const {
  return _switchToSingleMode;
}

void WorkspaceExposeWidget::setData(const QVector<WorkspacePanel *> &panels, int currentPanelIndex) {
  scene()->clear();
  _items.clear();
  foreach(WorkspacePanel* p, panels) {
    QPixmap pixmap = p->view()->snapshot(previewSize());
    PreviewItem* item = new PreviewItem(pixmap,p);
    scene()->addItem(item);
    _items << item;
    item->installEventFilter(this);
    connect(item,SIGNAL(opened()),this,SLOT(itemOpened()));
  }
  _currentPanelIndex = currentPanelIndex;
  updatePositions();
}

void WorkspaceExposeWidget::resizeEvent(QResizeEvent* /*event*/) {
  updatePositions();
}

qreal distance(const QPointF& a, const QPointF& b) {
  return sqrt( pow((b.x() - a.x()),2) +  pow((b.y() - a.y()),2) );
}

void WorkspaceExposeWidget::updatePositions(bool resetScenePos) {
//  delete _positionAnimation;
  // Use the reference distance as the distance for a preview to move to the next position. We will index animation speed on that.
  const int referenceDuration = 120;
  qreal referenceDistance = distance(QPointF(0,0),QPointF(previewSize().width()+MARGIN,0));

  QParallelAnimationGroup* group = new QParallelAnimationGroup(this);
  int x=MARGIN,y=MARGIN;
  foreach(PreviewItem* i, _items) {
    if (i != _selectedItem) {
      QPropertyAnimation* moveAnim = new QPropertyAnimation(i,"pos",group);
      QPointF startPoint = i->pos();
      QPointF endPoint = QPointF(x,y);
      qreal d = distance(startPoint,endPoint);
      int actualDuration = std::min<int>(d*referenceDuration/referenceDistance,referenceDuration*2);
      moveAnim->setDuration(actualDuration);
      moveAnim->setStartValue(startPoint);
      moveAnim->setEndValue(endPoint);
      group->addAnimation(moveAnim);
    }
    else if (_selectedItem != NULL) {
      if (_placeholderItem == NULL) {
        _placeholderItem = new QGraphicsRectItem(0,0,previewSize().width(),previewSize().height());
        _placeholderItem->setBrush(QColor(220, 220, 220));
        _placeholderItem->setPen(QColor(190, 190, 190));
        scene()->addItem(_placeholderItem);
      }

      _placeholderItem->setPos(x,y);
    }

    x+=WorkspaceExposeWidget::previewSize().width() + MARGIN;

    if (x>=width()-WorkspaceExposeWidget::previewSize().width()-MARGIN) {
      x=MARGIN;
      y+=WorkspaceExposeWidget::previewSize().height()+MARGIN;
    }
  }

  _positionAnimation = group;

  if (resetScenePos) {
    connect(group,SIGNAL(finished()),this,SLOT(resetSceneRect()));
  }

  connect(group,SIGNAL(finished()),this,SLOT(updatePositionsAnimationFinished()));
  group->start(QAbstractAnimation::DeleteWhenStopped);
}

void WorkspaceExposeWidget::updatePositionsAnimationFinished() {
  _positionAnimation = NULL;
}

void WorkspaceExposeWidget::resetSceneRect() {
  QRectF itemsRect = scene()->itemsBoundingRect();
  setSceneRect(0,0,std::max<qreal>(width(),itemsRect.width()),std::max<qreal>(height(),itemsRect.height()));
}

bool WorkspaceExposeWidget::eventFilter(QObject* obj, QEvent* ev) {
  PreviewItem* item = static_cast<PreviewItem*>(obj);

  if (ev->type() == QEvent::GraphicsSceneMousePress) {
    if (item->shouldClose(static_cast<QGraphicsSceneMouseEvent*>(ev)->pos())) {
      _items.removeAll(item);
      item->panel()->close();
      item->deleteLater();

      if (_items.empty())
        finish();
      else
        updatePositions();
    }
    else {
      _selectedItem = item;
      _selectedItem->setZValue(1);
    }

    return false;
  }

  else if (item == _selectedItem) {
    if (ev->type() == QEvent::GraphicsSceneMouseMove) {
      QGraphicsSceneMouseEvent* mouseEv = static_cast<QGraphicsSceneMouseEvent*>(ev);
      QPointF itemPos = mouseEv->scenePos();
      int itemPerLine = floor(width()/(previewSize().width() + MARGIN));
      int nbLines = _items.size()/itemPerLine;
      int line = itemPos.y() / (previewSize().height()+MARGIN);
      line = std::min<int>(nbLines, line);
      int col = itemPos.x() / (previewSize().width()+MARGIN);
      int index = line*itemPerLine + col;

      if (index != _items.indexOf(item)) {
        _items.removeOne(item);

        if (index < 0)
          index = 0;

        if (index > _items.size())
          index = _items.size();

        _items.insert(index,item);
        updatePositions(false);
      }
    }
    else if (ev->type() == QEvent::GraphicsSceneMouseRelease) {
      _selectedItem->setZValue(0);
      _selectedItem = NULL;
      updatePositions();
      delete _placeholderItem;
      _placeholderItem = NULL;
    }
  }

  return false;
}

bool WorkspaceExposeWidget::event(QEvent *event) {
  if (event->type() == QEvent::KeyPress && static_cast<QKeyEvent*>(event)->key() == Qt::Key_Escape) {
    finish();
  }

  return QGraphicsView::event(event);
}

void WorkspaceExposeWidget::itemOpened() {
  PreviewItem* item = static_cast<PreviewItem*>(sender());
  _currentPanelIndex = _items.indexOf(item);
  _switchToSingleMode = true;
  finish();
}

void WorkspaceExposeWidget::finish() {
  emit exposeFinished();
}
