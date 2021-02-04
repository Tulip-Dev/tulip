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
#include "tulip/ColorButton.h"

#include <QApplication>
#include <QColorDialog>
#include <QStylePainter>

#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>
#include <tulip/Perspective.h>

using namespace tlp;

ChooseColorButton::ChooseColorButton(QWidget *parent) : QPushButton(parent), _color(Qt::black) {
  connect(this, SIGNAL(clicked()), this, SLOT(chooseColor()));
  setFocusPolicy(Qt::WheelFocus);
}

QColor ChooseColorButton::color() const {
  return _color;
}

QString ChooseColorButton::text() const {
  return _text;
}

void ChooseColorButton::setDialogTitle(const QString &title) {
  _dialogTitle = title;
}

tlp::Color ChooseColorButton::tulipColor() const {
  return QColorToColor(_color);
}

void ChooseColorButton::setText(const QString &text) {
  _text = text;
}

void ChooseColorButton::setColor(const QColor &c) {
  _color = c;
  update();
  emit colorChanged(_color);
  emit tulipColorChanged(QColorToColor(_color));
}

void ChooseColorButton::setTulipColor(const tlp::Color &c) {
  setColor(colorToQColor(c));
}

void ChooseColorButton::chooseColor() {
  QColor c =
      QColorDialog::getColor(_color, QApplication::activeWindow(),
                             _dialogTitle.isEmpty() ? QString("Choose a color") : _dialogTitle,
                             // do not use native dialog to ensure alpha channel can be set
                             // it may not be shown when using gnome
                             QColorDialog::ShowAlphaChannel | QColorDialog::DontUseNativeDialog);

  if (c.isValid())
    setColor(c);
}

void ColorButton::paintEvent(QPaintEvent *event) {
  QPushButton::paintEvent(event);
  QStylePainter p(this);
  float tickW = width() / 4.;
  float tickH = height() / 4.;
  p.setPen(Qt::black);
  p.setBrush(_color);
  QRectF r(tickW, tickH, tickW * 2, tickH * 2);
  p.drawRect(r);
  p.setPen(QColor(255 ^ _color.red(), 255 ^ _color.green(), 255 ^ _color.blue(), 255));

  if (!_text.isEmpty())
    p.drawText(r, Qt::AlignCenter, _text);
}
