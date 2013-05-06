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
#include "tulip/ColorScaleButton.h"

#include <QPainter>
#include <QPaintEvent>

#include <tulip/TlpQtTools.h>

using namespace tlp;

void ColorScaleButton::paintScale(QPainter *painter, const QRect &baseRect, const ColorScale &colorScale) {
  QRect rect = baseRect;
  rect.setX(rect.x()+2);
  rect.setY(rect.y()+2);
  rect.setWidth(rect.width()-4);
  rect.setHeight(rect.height()-4);

  if (colorScale.isGradient()) {
    QLinearGradient grad(QPointF(rect.x(),rect.y()),QPointF(rect.x()+rect.width(),rect.y()));
    std::map<float,Color> stops = colorScale.getColorMap();

    for (std::map<float,Color>::iterator it = stops.begin(); it != stops.end(); ++it)
      grad.setColorAt(it->first,colorToQColor(it->second));

    painter->setBrush(QBrush(grad));
  }

  painter->drawRect(rect);
}

ColorScaleButton::ColorScaleButton(ColorScale colorScale, QWidget* parent): QPushButton(parent), _colorScale(colorScale), _dlg(NULL) {
  connect(this,SIGNAL(clicked()),this,SLOT(showDialog()));
}

ColorScaleButton::~ColorScaleButton() {
}

ColorScale ColorScaleButton::colorScale() const {
  return _colorScale;
}

void ColorScaleButton::setColorScale(const ColorScale& colorScale) {
  _colorScale = colorScale;

  if (_dlg)
    _dlg->setColorScale(colorScale);
}

void ColorScaleButton::paintEvent(QPaintEvent *event) {
  QPushButton::paintEvent(event);
  QPainter painter(this);
  QRect rect = event->rect();
  rect.setX(rect.x()+2);
  rect.setY(rect.y()+2);
  rect.setWidth(rect.width()-4);
  rect.setHeight(rect.height()-4);

  if (colorScale().isGradient()) {
    QLinearGradient grad(QPointF(rect.x(),rect.y()),QPointF(rect.x()+rect.width(),rect.y()));
    std::map<float,Color> stops = colorScale().getColorMap();

    for (std::map<float,Color>::iterator it = stops.begin(); it != stops.end(); ++it)
      grad.setColorAt(it->first,colorToQColor(it->second));

    painter.setBrush(QBrush(grad));
  }

  painter.drawRect(rect);
}

void ColorScaleButton::showDialog() {
  _dlg = new ColorScaleConfigDialog(_colorScale,this);
  _dlg->exec();
  _colorScale = _dlg->getColorScale();
  delete _dlg;
  _dlg = NULL;
}
