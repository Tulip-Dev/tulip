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
#include "tulip/ColorButton.h"

#include <QColorDialog>
#include <QMainWindow>
#include <QStylePainter>

#include <tulip/TlpQtTools.h>
#include <tulip/Perspective.h>

using namespace tlp;

ChooseColorButton::ChooseColorButton(QWidget *parent): QPushButton(parent), _color(Qt::black), _dialogParent(parent), _dialogTitle((char *) NULL) {
  connect(this,SIGNAL(clicked()),this,SLOT(chooseColor()));
  setFocusPolicy(Qt::WheelFocus);
}

QColor ChooseColorButton::color() const {
  return _color;
}

void ChooseColorButton::setDialogParent(QWidget *w) {
  _dialogParent = w;
}

void ChooseColorButton::setDialogTitle(const QString& title) {
  _dialogTitle = title;
}

tlp::Color ChooseColorButton::tulipColor() const {
  return QColorToColor(_color);
}

void ChooseColorButton::setColor(const QColor& c) {
  _color = c;
  repaint();
  emit colorChanged(_color);
  emit tulipColorChanged(QColorToColor(_color));
}

void ChooseColorButton::setTulipColor(const tlp::Color& c) {
  setColor(colorToQColor(c));
}

void ChooseColorButton::chooseColor() {
  QColor c = QColorDialog::getColor(_color, _dialogParent,
                                    _dialogTitle.isNull()
                                    ? QString("Choose a color")
                                    : _dialogTitle,
                                    QColorDialog::ShowAlphaChannel);

  if (c.isValid())
    setColor(c);
}

void ColorButton::paintEvent(QPaintEvent *event) {
  QPushButton::paintEvent(event);
  QStylePainter p(this);
  float tickW = width() / 4.;
  float tickH = height() / 4;
  p.setPen(Qt::black);
  _color.setAlpha(255);
  p.setBrush(_color);
  p.drawRect(tickW, tickH, tickW * 2, tickH * 2);
}

