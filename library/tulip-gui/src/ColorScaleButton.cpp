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
#include "tulip/ColorScaleButton.h"

#include <QPainter>
#include <QPaintEvent>
#include <QApplication>


#include <tulip/TlpQtTools.h>
#include <tulip/ColorScaleConfigDialog.h>

using namespace tlp;

void ColorScaleButton::paintScale(QPainter *painter, const QRect &baseRect, const ColorScale &colorScale) {
  QRect rect = baseRect;
  rect.setX(rect.x()+2);
  rect.setY(rect.y()+2);
  rect.setWidth(rect.width()-4);
  rect.setHeight(rect.height()-4);

  QLinearGradient grad(QPointF(rect.x(),rect.y()),QPointF(rect.x()+rect.width(),rect.y()));
  std::map<float,Color> stops = colorScale.getColorMap();

  for (std::map<float,Color>::iterator it = stops.begin(); it != stops.end(); ++it)
    grad.setColorAt(it->first,colorToQColor(it->second));

  painter->setBrush(QBrush(grad));

  painter->drawRect(rect);
}

ColorScaleButton::ColorScaleButton(ColorScale colorScale, QWidget* parent):
  QPushButton(parent), _colorScale(colorScale) {
  connect(this,SIGNAL(clicked()),this,SLOT(editColorScale()));
}

const ColorScale& ColorScaleButton::colorScale() const {
  return _colorScale;
}

void ColorScaleButton::paintEvent(QPaintEvent *event) {
  QPushButton::paintEvent(event);
  QPainter painter(this);
  paintScale(&painter, event->rect(), colorScale());
}

void ColorScaleButton::editColorScale(const ColorScale& cs) {
  ColorScaleConfigDialog dlg(cs, this);

  if (dlg.exec() == QDialog::Accepted)
    _colorScale = dlg.getColorScale();
  else if (&cs != &_colorScale)
    _colorScale = cs;
}

void ColorScaleButton::editColorScale() {
  editColorScale(_colorScale);
}
