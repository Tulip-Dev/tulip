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
  
  class TLP_GL_SCOPE GlyphManager {
  
  public:

    static GlyphManager &getInst() {
      if(!inst)
	inst=new GlyphManager();
      return *inst;
    }

    std::string glyphName(int id);
    int glyphId(std::string name);
    void loadGlyphPlugins();
    void loadPlugins(PluginLoader *plug=0);

    void initGlyphList(Graph **graph,GlGraphInputData* glGraphInputData,MutableContainer<Glyph *>& glyphs);

  private:

    GlyphManager();

    static GlyphManager* inst;

  };
 
}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLYPHMANAGER_H
