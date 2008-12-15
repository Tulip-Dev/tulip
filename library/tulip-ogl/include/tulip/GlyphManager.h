//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 08/11/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/
#ifndef Tulip_GLYPHMANAGER_H
#define Tulip_GLYPHMANAGER_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <string>
#include <cassert>

#include <tulip/tulipconf.h>
#include <tulip/PluginLoader.h>
#include <tulip/Glyph.h>

namespace tlp {

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
    int glyphId(std::string name);
    /**
     * Load glyphs plugins
     */
    void loadGlyphPlugins();
    /**
     * Load glyphs plugins with given PluginLoader
     */
    void loadPlugins(PluginLoader *plug=0);

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
