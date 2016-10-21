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

#ifndef GLSPHERE_H
#define GLSPHERE_H

#include <tulip/tulipconf.h>

#include <tulip/Coord.h>
#include <tulip/Color.h>

#include <tulip/GlGlyphEntity.h>

namespace tlp {

class TLP_GLES_SCOPE GlSphere : public GlGlyphEntity {

public:
  GlSphere(const Coord &position, float radius, const Color &color = Color(0, 0, 0, 255), float rotX = 0, float rotY = 0, float rotZ = 0);

  GlSphere(const Coord &position, float radius, const std::string &textureFile, int alpha = 255, float rotX = 0, float rotY = 0, float rotZ = 0);
};
}

#endif // GLSPHERE_H
