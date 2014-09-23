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

#include "tulip/ClearableLineEdit.h"

#include <QPaintEvent>
#include <QPainter>

QPixmap* ClearableLineEdit::CLEAR_PIXMAP = NULL;

void ClearableLineEdit::initPixmap() {
  if (CLEAR_PIXMAP == NULL) {
    CLEAR_PIXMAP = new QPixmap(":/tulip/gui/ui/clearbutton.png");
  }
}

ClearableLineEdit::ClearableLineEdit(QWidget *parent): QLineEdit(parent), _clearButtonHovered(false) {
  setMouseTracking(true);
}

void ClearableLineEdit::paintEvent(QPaintEvent* ev) {
  QLineEdit::paintEvent(ev);
  QPainter p(this);
  p.setOpacity(_clearButtonHovered ? 1 : 0.7);
  initPixmap();
  p.drawPixmap(pixmapRect(),*CLEAR_PIXMAP);
}

QRect ClearableLineEdit::pixmapRect() {
  initPixmap();
  QRect pixmapRect(width() - CLEAR_PIXMAP->width() - 5, height()/2-CLEAR_PIXMAP->height()/2,CLEAR_PIXMAP->width(),CLEAR_PIXMAP->height());
  return pixmapRect;
}

void ClearableLineEdit::mouseMoveEvent(QMouseEvent* ev) {
  QLineEdit::mouseMoveEvent(ev);
  bool oldValue = _clearButtonHovered;
  _clearButtonHovered = pixmapRect().contains(ev->pos());

  if (oldValue != _clearButtonHovered)
    repaint();
}

void ClearableLineEdit::mousePressEvent(QMouseEvent* ev) {
  QLineEdit::mousePressEvent(ev);

  if (pixmapRect().contains(ev->pos())) {
    clear();
    emit textEdited("");
    emit editingFinished();
  }
}


