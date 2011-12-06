/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include <tulip/GlyphManager.h>

#include <tulip/tuliphash.h>

#include <tulip/Glyph.h>

//====================================================
tlp::GlyphManager* tlp::GlyphManager::inst=0;

using namespace std;

namespace tlp {
static TLP_HASH_MAP<int,std::string>   glyphIdToName;
static TLP_HASH_MAP<std::string, int>  nameToGlyphId;

GlyphManager::GlyphManager() {
}
//====================================================
string GlyphManager::glyphName(int id) {
  if (glyphIdToName.find(id)!=glyphIdToName.end()) {
    std::cout << glyphIdToName[id] << ": " << id << std::endl;
    return glyphIdToName[id];
  }
  else {
    cerr << __PRETTY_FUNCTION__ << endl;
    cerr << "Invalid glyph id" << endl;
    return string("invalid");
  }
}
//====================================================
int GlyphManager::glyphId(string name) {
  if (nameToGlyphId.find(name)!=nameToGlyphId.end()) {
    std::cout << name << ": " << nameToGlyphId[name] << std::endl;
    return nameToGlyphId[name];
  }
  else {
    cerr << __PRETTY_FUNCTION__ << endl;
    cerr << "Invalid glyph name" << endl;
    return 0;
  }
}
//====================================================
void GlyphManager::loadGlyphPlugins() {
  string pluginName;
  forEach(pluginName, GlyphLister::availablePlugins()) {
    int pluginId=GlyphLister::pluginInformations(pluginName).getId();
    glyphIdToName[pluginId]=pluginName;
    nameToGlyphId[pluginName]=pluginId;
  }
}

void GlyphManager::initGlyphList(Graph **graph,GlGraphInputData* glGraphInputData,MutableContainer<Glyph *>& glyphs) {
  // first destroy current default glyph
  Glyph* defaultGlyph = glyphs.getDefault();

  if (defaultGlyph)
    delete defaultGlyph;

  // then set a new one
  GlyphContext gc = GlyphContext(graph,glGraphInputData);
  glyphs.setAll(GlyphLister::getPluginObject("3D - Cube OutLined", &gc));

  string glyphName;
  forEach(glyphName, GlyphLister::availablePlugins()) {
    Glyph *newGlyph = GlyphLister::getPluginObject(glyphName, &gc);
    glyphs.set(GlyphLister::pluginInformations(glyphName).getId(), newGlyph);
  }
}

void GlyphManager::clearGlyphList(Graph**,GlGraphInputData*,MutableContainer<Glyph *>& glyphs) {
  string glyphName;
  forEach(glyphName, GlyphLister::availablePlugins()) {
    delete glyphs.get(GlyphLister::pluginInformations(glyphName).getId());
  }
}
}
