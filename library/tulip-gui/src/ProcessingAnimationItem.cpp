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
#include "tulip/ProcessingAnimationItem.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

ProcessingAnimationItem::ProcessingAnimationItem(const QPixmap &pixmap, const QSize &size, QGraphicsItem *parent)
  : QGraphicsPixmapItem(parent), _currentFrame(0), _brush(Qt::transparent) {
  for (int y=0; y<pixmap.height(); y+=size.height()) {
    for (int x=0; x<pixmap.width(); x+=size.width()) {
      _pixmaps.push_back(pixmap.copy(x,y,size.width(),size.height()));
    }
  }

  _animationTimer.setInterval(50);
  _animationTimer.setSingleShot(false);
  connect(&_animationTimer,SIGNAL(timeout()),this,SLOT(animationTimeout()));
  _animationTimer.start();
}

ProcessingAnimationItem::~ProcessingAnimationItem() {
}

void ProcessingAnimationItem::animationTimeout() {
  _currentFrame = (_currentFrame+1)%_pixmaps.size();
  setPixmap(_pixmaps[_currentFrame]);
  update();
}

void ProcessingAnimationItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  painter->setBrush(_brush);
  painter->setPen(Qt::transparent);
  painter->drawRect(boundingRect());
  QGraphicsPixmapItem::paint(painter,option,widget);
}
