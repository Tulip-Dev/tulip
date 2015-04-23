/*
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
///@cond DOXYGEN_HIDDEN


#ifndef GLCOLORSCALE_H_
#define GLCOLORSCALE_H_

#include <tulip/Observable.h>
#include <tulip/GlSimpleEntity.h>
#include <tulip/Color.h>

namespace tlp {

class ColorScale;
class GlPolyQuad;

class TLP_GL_SCOPE GlColorScale : public GlSimpleEntity, public Observable {

public:

  enum Orientation {
    Horizontal, Vertical
  };

  GlColorScale(ColorScale *colorScale, const Coord &baseCoord, const float length,
               const float thickness, Orientation orientation);

  ~GlColorScale();

  /**
    * @brief Compute the color corresponding to the position in the color scale.
    * The orientation of the scale define the coordinate used to compute the color (if the orientation is horizontal use only the X coordinate).
    * If the position is outside of the entity coordinates returns the nearest extremity value.
    **/
  Color getColorAtPos(Coord pos);

  void draw(float lod, Camera* camera);

  void translate(const Coord &move);

  Coord getBaseCoord() const {
    return baseCoord;
  }

  float getThickness() const {
    return thickness;
  }

  float getLength() const {
    return length;
  }

  GlPolyQuad *getColorScalePolyQuad() const {
    return colorScalePolyQuad;
  }

  void setColorScale(ColorScale * scale);

  ColorScale *getColorScale() {
    return colorScale;
  }

  void getXML(std::string &) {}

  void setWithXML(const std::string &,unsigned int &) {}

protected :

  void treatEvent(const Event &);

private:

  void updateDrawing();

  ColorScale *colorScale;
  Coord baseCoord;
  float length, thickness;
  GlPolyQuad *colorScalePolyQuad;
  Orientation orientation;
};

}

#endif /* GLCOLORSCALE_H_ */
///@endcond
