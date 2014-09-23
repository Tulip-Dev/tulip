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

#ifndef GLLABELLEDCOLORSCALE_H_
#define GLLABELLEDCOLORSCALE_H_

#include <tulip/GlComposite.h>
#include <tulip/GlLabel.h>
#include <tulip/GlColorScale.h>
namespace tlp {
class GlLayer;

class GlLabelledColorScale: public tlp::GlComposite {
public:
  GlLabelledColorScale(tlp::Coord position, tlp::Size size, tlp::ColorScale* colorScale,
                       double minValue = 0, double maxValue = 0, bool ColorScaleAtTop = true);

  virtual ~GlLabelledColorScale();

  void setMinValue(double value);
  double getMinValue() const {
    return minValue;
  }
  void setMaxValue(double value);
  double getMaxValue() const {
    return maxValue;
  }

  tlp::Coord getPosition() {
    return position;
  }
  tlp::Size getSize() {
    return size;
  }

  tlp::BoundingBox getColorScaleBoundingBox();

  tlp::GlColorScale *getGlColorScale() {
    return glColorScale;
  }

  void setColorScale(tlp::ColorScale* cs);

  void setPosition(tlp::Coord nPosition);

  void setSize(tlp::Size nSize);

  void addLayerParent(tlp::GlLayer *layer);

protected:
  void buildComposite(tlp::ColorScale* colorScale);
  tlp::Coord position;
  tlp::Size size;
  tlp::GlColorScale *glColorScale;
  tlp::GlLabel *minLabel;
  tlp::GlLabel *maxLabel;
  double minValue;
  double maxValue;
  bool colorScaleAtTop;
};
}
#endif /* GLLABELLEDCOLORSCALE_H_ */
