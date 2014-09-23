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

/** \ brief Singleton class use to store Glyphs plugins
 * This class is a singleton use to sore Glyphs plugins
 */
class TLP_GL_SCOPE GlyphManager {

public:

  /**
   * Return the singleton (if the singleton doesn't exist this function create it)
   */
  static GlyphManager &getInst() {
    if(!inst)
      inst=new GlyphManager();

    return *inst;
  }

  /**
   * Return the name of glyph with given id
   */
  std::string glyphName(int id);
  /**
   * Return the id if glyph with given name
   */
  int glyphId(const std::string& name);
  /**
   * Load glyphs plugins
   */
  void loadGlyphPlugins();

  /**
   * Create the glyph list and store it in glyphs parameter
   */
  void initGlyphList(Graph **graph,GlGraphInputData* glGraphInputData,MutableContainer<Glyph *>& glyphs);
  /**
   * Clear the glyph list
   */
  void clearGlyphList(Graph **graph,GlGraphInputData* glGraphInputData,MutableContainer<Glyph *>& glyphs);

private:

  GlyphManager();

  static GlyphManager* inst;

};

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLYPHMANAGER_H
///@endcond
