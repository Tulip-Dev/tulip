/*
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

#ifndef Tulip_GLYPHMANAGER_H
#define Tulip_GLYPHMANAGER_H

#ifndef DOXYGEN_NOTFOR_DEVEL

//#include <string>

#include <tulip/tulipconf.h>
#include <tulip/Glyph.h>
#include <tulip/MutableContainer.h>

namespace tlp {

class GlGraphInputData;
class Graph;

/**
 * Used to store Glyphs plugins
 */
class TLP_GL_SCOPE GlyphManager {

public:
  /**
   * Return the name of glyph with given id
   */
  static std::string glyphName(int id);
  /**
   * Return the id if glyph with given name
   */
  static int glyphId(const std::string &name, bool warnIfNotFound = true);
  /**
   * Load glyphs plugins
   */
  static void loadGlyphPlugins();

  /**
   * Create the glyph list and store it in glyphs parameter
   */
  static void initGlyphList(Graph **graph, GlGraphInputData *glGraphInputData,
                            MutableContainer<Glyph *> &glyphs);
  /**
   * Clear the glyph list
   */
  static void clearGlyphList(Graph **graph, GlGraphInputData *glGraphInputData,
                             MutableContainer<Glyph *> &glyphs);
};
} // namespace tlp

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLYPHMANAGER_H
///@endcond
