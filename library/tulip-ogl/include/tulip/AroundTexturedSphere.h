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
#include <tulip/TlpTools.h>
#include <tulip/Graph.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/ColorProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlTools.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/GlSphere.h>
#include <tulip/GlRect.h>

using namespace std;
using namespace tlp;

namespace tlp {

/** \addtogroup glyph */
/*@{*/
/// A 3D glyph.
/**
 * This glyph draws a sphere using the "viewColor" node property value.
 * and apply a texture around it
 */
class TLP_GL_SCOPE AroundTexturedSphere : public NoShaderGlyph {
  const std::string textureFile;
  const unsigned char alpha;

public:
  AroundTexturedSphere(const tlp::PluginContext *context = nullptr,
                       const std::string &aroundTextureFile = "", unsigned char alphaVal = 255)
      : NoShaderGlyph(context), textureFile(aroundTextureFile), alpha(alphaVal) {}
  void getIncludeBoundingBox(BoundingBox &boundingBox, node) override;
  void draw(node n, float) override;
  static void drawGlyph(const Color &glyphColor, const Size &glyphSize, const string &texture,
                        const string &texturePath, const string &aroundTextureFile,
                        unsigned char alpha = 255);
};
} // end of namespace tlp
