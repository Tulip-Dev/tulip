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


#ifndef EDGEEXTREMITYGLYPHMANAGER_H_
#define EDGEEXTREMITYGLYPHMANAGER_H_

#include <string>

#include <tulip/tulipconf.h>
#include <tulip/MutableContainer.h>

namespace tlp {
class GlGraphInputData;
class EdgeExtremityGlyph;
class Graph;

class TLP_GL_SCOPE EdgeExtremityGlyphManager {

public:
  virtual ~EdgeExtremityGlyphManager() {
  }

  /**
   * Return the singleton (if the singleton doesn't exist this function create it)
   */
  static EdgeExtremityGlyphManager &getInst() {
    if (!eeinst)
      eeinst = new EdgeExtremityGlyphManager();

    return *eeinst;
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
  void initGlyphList(Graph **graph, GlGraphInputData* glGraphInputData,
                     MutableContainer<EdgeExtremityGlyph *>& glyphs);
  /**
   * Clear the glyph list
   */
  void clearGlyphList(Graph **graph, GlGraphInputData* glGraphInputData,
                      MutableContainer<EdgeExtremityGlyph *>& glyphs);
private:

  EdgeExtremityGlyphManager();

  static EdgeExtremityGlyphManager* eeinst;


};
}
#endif /* EDGEEXTREMITYGLYPHMANAGER_H_ */
///@endcond
