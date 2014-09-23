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
#include <tulip/ColorScaleWidget.h>
#include <tulip/ColorScale.h>

#include <QPaintEvent>
#include <QPainter>

using namespace tlp;
using namespace std;

ColorScaleWidget::ColorScaleWidget(QWidget* parent) :
  QWidget(parent), colorScale(NULL), orientation(Qt::Horizontal) {
}

ColorScaleWidget::~ColorScaleWidget() {
}

void ColorScaleWidget::setColorScale(ColorScale *colorScale) {
  this->colorScale = colorScale;
  update();
}

void ColorScaleWidget::paintEvent(QPaintEvent * event) {
  QPainter painter(this);

  if (colorScale != NULL) {
    paintColorScale(painter, event->rect());
  }

  painter.setPen(QPen(QBrush(QColor("black")), 1.0));
  painter.drawRect(QRect(event->rect().x(), event->rect().y(), event->rect().width() - 1, event->rect().height() - 1));

}

void ColorScaleWidget::paintColorScale(QPainter& painter, const QRect& rect) {
  painter.setPen(Qt::NoPen);
  map<float, Color> colorMap = colorScale->getColorMap();

  if (colorScale->isGradient()) {
    QPoint start;
    QPoint stop;

    if (orientation == Qt::Horizontal) {
      start = QPoint(rect.left(), rect.center().y());
      stop = QPoint(rect.right(), rect.center().y());
    }
    else {
      start = QPoint(rect.center().x(), rect.bottom());
      stop = QPoint(rect.center().x(), rect.top());
    }

    QLinearGradient qLinearGradient(start, stop);

    for (map<float, Color>::iterator it = colorMap.begin(); it != colorMap.end(); ++it) {
      qLinearGradient.setColorAt(it->first, QColor(it->second[0], it->second[1], it->second[2], it->second[3]));
    }

    painter.fillRect(rect, qLinearGradient);
  }
  else {
    unsigned int nb = 0;
    float rectWidth = ((float) rect.width()) / colorMap.size();
    float rectHeight = ((float) rect.height()) / colorMap.size();

    for (map<float, Color>::iterator it = colorMap.begin(); it != colorMap.end(); ++it) {
      QRectF rectangle;

      if (orientation == Qt::Horizontal) {
        rectangle.setTopLeft(QPointF(rect.left() + nb * rectWidth, rect.top()));
        rectangle.setSize(QSizeF(rectWidth, rect.height()));
      }
      else {
        rectangle.setTopLeft(QPointF(rect.left(), rect.bottom() - (nb + 1) * rectHeight));
        rectangle.setSize(QSizeF(rect.width(), rectHeight));
      }

      painter.fillRect(rectangle, QBrush(QColor(it->second[0], it->second[1], it->second[2], it->second[3])));
      ++nb;
    }
  }
}

void ColorScaleWidget::setOrientation(Qt::Orientation orientation) {
  if (this->orientation != orientation) {
    this->orientation = orientation;
    update();
  }
}

QSize ColorScaleWidget::minimumSizeHint() const {
  if (orientation == Qt::Horizontal) {
    return QSize(100,25);
  }
  else {
    return QSize(25,100);
  }
}

QSize ColorScaleWidget::sizeHint () const {
  return minimumSizeHint();

}
