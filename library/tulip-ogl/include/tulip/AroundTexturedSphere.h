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
///@cond DOXYGEN_HIDDEN

#include <tulip/Glyph.h>

namespace tlp {

struct BoundingBox;

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
  static void drawGlyph(const Color &glyphColor, const Size &glyphSize, const std::string &texture,
                        const std::string &texturePath, const std::string &aroundTextureFile,
                        unsigned char alpha = 255);
};
} // end of namespace tlp

///@endcond
