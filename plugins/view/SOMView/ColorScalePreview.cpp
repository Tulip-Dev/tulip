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

#include "ColorScalePreview.h"

#include <tulip/ColorScaleConfigDialog.h>

using namespace tlp;

ColorScalePreview::ColorScalePreview(ColorScale* colorScale) :
  currentColorScale(colorScale) {
  setAutoFillBackground(true);
  fillLabel();
}

ColorScalePreview::~ColorScalePreview() {
  // TODO Auto-generated destructor stub
}

void ColorScalePreview::fillLabel() {

  if (currentColorScale != NULL && width() != 0 && height() != 0) {

    std::map<float, tlp::Color> colorMap = currentColorScale->getColorMap();
    QPalette p = this->palette();
    QLinearGradient grad(0, height() * 0.5, width(), height() * 0.5);

    for (std::map<float, tlp::Color>::iterator it = colorMap.begin(); it
         != colorMap.end(); ++it) {
      tlp::Color col = it->second;
      QColor c(col.getR(), col.getG(), col.getB(), col.getA());
      grad.setColorAt(it->first, c);
    }

    p.setBrush(QPalette::Window, QBrush(grad));
    this->setPalette(p);
  }
}

void ColorScalePreview::setColorScale(ColorScale* colorScale) {
  currentColorScale = colorScale;
  fillLabel();
}

void ColorScalePreview::resizeEvent(QResizeEvent * event) {
  QLabel::resizeEvent(event);
  fillLabel();
}

void ColorScalePreview::mousePressEvent(QMouseEvent *) {
  tlp::ColorScaleConfigDialog dialog(currentColorScale, this);
  dialog.exec();
  fillLabel();
}
