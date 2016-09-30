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

/**
 *
 * tulip-ogles library : rewrite of old tulip-ogl library using OpenGL ES API
 * Copyright (c) 2016 Antoine Lambert, Thales Services SAS
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#ifndef GLCIRCLE_H
#define GLCIRCLE_H

#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/GlGlyphEntity.h>

namespace tlp {

class TLP_GLES_SCOPE GlCircle : public GlGlyphEntity {

public:
  GlCircle(const Coord &center = Coord(0, 0, 0), float radius = 1., const Color &fillColor = Color(0, 0, 255, 255),
           const Color &outlineColor = Color(255, 0, 0, 255), bool filled = true, bool outlined = true);

  void setRadius(float radius);
};
}

#endif
