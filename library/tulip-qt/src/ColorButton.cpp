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
#include "ColorButton.h"

#include <QtGui/QPaintEvent>
#include <QtGui/QStylePainter>
#include <QtGui/QColorDialog>

using namespace tlp;

ColorButton::ColorButton(QWidget *parent): QPushButton(parent) {
  setColor(Qt::black);
  connect(this, SIGNAL(clicked()), this, SLOT(askColor()));
}

ColorButton::ColorButton(const QRgb &c, QWidget *parent, const char *, Qt::WFlags ):QPushButton(parent){
    connect(this, SIGNAL(clicked()), this, SLOT(askColor()));
    setColor(QColor::fromRgb(c));
}

ColorButton::~ColorButton() {
}

void ColorButton::setColor(const QColor &c) {
  _color = c;
  emit colorChanged(_color);
  repaint();
}

QColor ColorButton::color() const {
  return _color;
}

void ColorButton::paintEvent(QPaintEvent *event) {
  QPushButton::paintEvent(event);
  QStylePainter p(this);
  float tickW = width() / 4.;
  float tickH = height() / 4;

  p.setPen(Qt::black);
  p.setBrush(_color);
  p.drawRect(tickW, tickH, tickW * 2, tickH * 2);
}

void ColorButton::askColor() {
  QColor newColor(QColorDialog::getColor(_color, this,tr("Choose a color"), QColorDialog::ShowAlphaChannel));
  if (!newColor.isValid())
    return;
  setColor(newColor);
  repaint();
}
