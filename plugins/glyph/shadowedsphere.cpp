/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <tulip/TulipViewSettings.h>
#include <tulip/Glyph.h>
#include <tulip/AroundTexturedSphere.h>

using namespace std;
using namespace tlp;

namespace tlp {

/** \addtogroup glyph */
/*@{*/
/// A 3D glyph.
/**
 * This glyph draws a sphere with a bottom shadow
 */
class BottomShadowSphere : public AroundTexturedSphere {
public:
  GLYPHINFORMATION("3D - Sphere with bottom shadow", "Patrick Mary", "09/03/2018",
                   "Sphere with its drop shadow on bottom", "1.0", NodeShape::BottomShadowedSphere)
  BottomShadowSphere(const tlp::PluginContext *context = nullptr)
      : AroundTexturedSphere(context, "bottomShadowTexture.png") {}
};

PLUGIN(BottomShadowSphere)

/** \addtogroup glyph */
/*@{*/
/// A 3D glyph.
/**
 * This glyph draws a sphere with a left bottom shadow
 */
class LeftBottomShadowSphere : public AroundTexturedSphere {
public:
  GLYPHINFORMATION("3D - Sphere with left bottom shadow", "Patrick Mary", "09/03/2018",
                   "Sphere with its drop shadow on bottom left", "1.0",
                   NodeShape::LeftBottomShadowedSphere)
  LeftBottomShadowSphere(const tlp::PluginContext *context = nullptr)
      : AroundTexturedSphere(context, "leftBottomShadowTexture.png") {}
};

PLUGIN(LeftBottomShadowSphere)

/** \addtogroup glyph */
/*@{*/
/// A 3D glyph.
/**
 * This glyph draws a sphere with a right bottom shadow
 */
class RightBottomShadowSphere : public AroundTexturedSphere {
public:
  GLYPHINFORMATION("3D - Sphere with right bottom shadow", "Patrick Mary", "09/03/2018",
                   "Sphere with its drop shadow on right bottom", "1.0",
                   NodeShape::RightBottomShadowedSphere)
  RightBottomShadowSphere(const tlp::PluginContext *context = nullptr)
      : AroundTexturedSphere(context, "rightBottomShadowTexture.png") {}
};

PLUGIN(RightBottomShadowSphere)

} // end of namespace tlp
