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
#include <tulip/BoundingBox.h>
#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlRect.h>
#include <tulip/GlSphere.h>
#include <tulip/AroundTexturedSphere.h>

using namespace std;
using namespace tlp;

void AroundTexturedSphere::getIncludeBoundingBox(BoundingBox &boundingBox, node) {
  boundingBox[0] = Coord(-0.35f, -0.35f, -0.35f);
  boundingBox[1] = Coord(0.35f, 0.35f, 0.35f);
}

void AroundTexturedSphere::drawGlyph(const Color &glyphColor, const Size &glyphSize,
                                     const string &texture, const string &texturePath,
                                     const string &aroundTextureFile, unsigned char alpha) {
  // draw a sphere
  static GlSphere sphere(Coord(0, 0, 0), 0.5);
  sphere.setColor(glyphColor);
  sphere.setTexture(texturePath + texture);
  sphere.draw(0, nullptr);

  // draw a texture in the screen plane around the sphere
  static GlRect rect(Coord(0, 0, 0), 2., 2, Color(0, 0, 0, 255), Color(0, 0, 0, 255));
  rect.setOutlineMode(false);

  rect.setTextureName(TulipBitmapDir + aroundTextureFile);
  Color aroundColor(glyphColor);
  aroundColor.setA(alpha);
  rect.setFillColor(aroundColor);

  Glyph::drawRectInScreenPlane(rect, glyphSize, true);
}

void AroundTexturedSphere::draw(node n, float) {
  drawGlyph(glGraphInputData->getElementColor()->getNodeValue(n),
            glGraphInputData->getElementSize()->getNodeValue(n),
            glGraphInputData->getElementTexture()->getNodeValue(n),
            glGraphInputData->parameters->getTexturePath(), textureFile, alpha);
}
