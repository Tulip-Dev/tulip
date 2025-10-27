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
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/OpenGlIncludes.h>
#include <tulip/AroundTexturedSphere.h>

using namespace std;
using namespace tlp;

namespace tlp {

/** \addtogroup glyph */
/*@{*/
/// A 3D glyph.
/**
 * This glyph draws a sphere with a glow halo colored with the "viewColor"
 * node property value.
 */
class GlowSphere : public AroundTexturedSphere {
public:
  GLYPHINFORMATION("3D - Glow Sphere", "Patrick Mary", "24/01/2012", "Glow Sphere", "1.0",
                   NodeShape::GlowSphere)
  GlowSphere(const tlp::PluginContext *context = nullptr)
      : AroundTexturedSphere(context, "radialGradientTexture.png", 128) {}
  ~GlowSphere() override {}
};

PLUGIN(GlowSphere)

class EEGlowSphere : public EdgeExtremityGlyph {
public:
  GLYPHINFORMATION("3D - Glow Sphere extremity", "Patrick Mary", "24/01/2012",
                   "Glow Sphere for edge extremities", "1.0", EdgeExtremityShape::GlowSphere)
  EEGlowSphere(const tlp::PluginContext *context = nullptr) : EdgeExtremityGlyph(context) {}
  ~EEGlowSphere() override {}
  void draw(edge e, node n, const Color &glyphColor, const Color & /* borderColor */,
            float /* lod */) override {
    glDisable(GL_LIGHTING);
    AroundTexturedSphere::drawGlyph(
        glyphColor, edgeExtGlGraphInputData->getElementSize()->getNodeValue(n),
        edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e),
        edgeExtGlGraphInputData->parameters->getTexturePath(), "radialGradientTexture.png", 128);
  }
};

PLUGIN(EEGlowSphere)

} // end of namespace tlp
