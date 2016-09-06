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

#ifndef GLYPHSMANAGER_H
#define GLYPHSMANAGER_H

#include <tulip/Glyph.h>

#include <map>
#include <string>

namespace tlp {

class TLP_GLES_SCOPE GlyphsManager {

public:

  ~GlyphsManager();

  static GlyphsManager* instance();

  void loadGlyphPlugins();

  const std::map<int, Glyph*> &getGlyphs() {
    return _glyphs;
  }

  Glyph *getGlyph(int glyphId);

private:

  GlyphsManager();

  static GlyphsManager *_instance;

  std::map<int, Glyph*> _glyphs;
  std::map<int, Glyph*> _iconGlyphs;

};

}

#endif // GLYPHSMANAGER_H
