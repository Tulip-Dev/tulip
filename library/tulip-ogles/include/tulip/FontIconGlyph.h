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
 * Copyright (c) 2016 Antoine Lambert, Thales Services
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#ifndef FONTAWESOMEGLYPH_H
#define FONTAWESOMEGLYPH_H

#include <tulip/Glyph.h>
#include <tulip/TulipViewSettings.h>

namespace tlp {

class TLP_GLES_SCOPE FontIconGlyph : public Glyph {

public:

  FontIconGlyph(const std::string &fontFile, unsigned int iconCodePoint);

};

class TLP_GLES_SCOPE FontAwesomeGlyph : public FontIconGlyph {

public:

  GLYPHINFORMATION("2D - Font Awesome Icon", "2D - Font Awesome Icon extremity", "Antoine Lambert", "26/02/2015", "Font Awesome Icon", "1.0", NodeShape::FontAwesomeIcon)

  FontAwesomeGlyph(unsigned int iconCodePoint);

};

class TLP_GLES_SCOPE MaterialDesignIconGlyph : public FontIconGlyph {

public:

  GLYPHINFORMATION("2D - Material Design Icon", "2D - Material Design Icon extremity", "Antoine Lambert", "29/07/2016", "Material Design Icon", "1.0", NodeShape::MaterialDesignIcon)

  MaterialDesignIconGlyph(unsigned int iconCodePoint);

};

}

#endif // FONTAWESOMEGLYPH_H

