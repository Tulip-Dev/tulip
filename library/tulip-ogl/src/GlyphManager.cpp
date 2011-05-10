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

namespace tlp
{
  static TLP_HASH_MAP<int,std::string>   glyphIdToName;
  static TLP_HASH_MAP<std::string, int>  nameToGlyphId;

  GlyphManager::GlyphManager() {
  }
  //====================================================
  string GlyphManager::glyphName(int id) {
    if (glyphIdToName.find(id)!=glyphIdToName.end())
      return glyphIdToName[id];
    else {
      cerr << __PRETTY_FUNCTION__ << endl;
      cerr << "Invalid glyph id" << endl;
      return string("invalid");
    }
  }
  //====================================================
  int GlyphManager::glyphId(string name) {
    if (nameToGlyphId.find(name)!=nameToGlyphId.end())
      return nameToGlyphId[name];
    else {
      cerr << __PRETTY_FUNCTION__ << endl;
      cerr << "Invalid glyph name" << endl;
      return 0;
    }
  }
  //====================================================
  void GlyphManager::loadGlyphPlugins() {
    Iterator<string> *itS=GlyphPluginManager::availablePlugins();
    while (itS->hasNext()) {
      string pluginName=itS->next();
      int pluginId=GlyphPluginManager::objMap().at(pluginName).factory->getId();
      glyphIdToName[pluginId]=pluginName;
      nameToGlyphId[pluginName]=pluginId;
    } delete itS;
  }
  //
  void GlyphManager::initGlyphList(Graph **graph,GlGraphInputData* glGraphInputData,MutableContainer<Glyph *>& glyphs) {
    GlyphContext gc = GlyphContext(graph,glGraphInputData);
    glyphs.setAll(GlyphPluginManager::getPluginObject("3D - Cube OutLined", &gc));
    Iterator<string> *itS = GlyphPluginManager::availablePlugins();
    while (itS->hasNext()) {
      string glyphName=itS->next();
      //cerr << "loading : " << glyphName << endl;
      Glyph *newGlyph = GlyphPluginManager::getPluginObject(glyphName, &gc);
      //    cerr << newGlyph << endl;
      //if (GlyphPluginManager::objMap[glyphName]->getId() == 0)
      //  glyphs.setAll(newGlyph);
      //    cerr << " id:" << GlyphPluginManager::objMap[glyphName]->getId() << endl;
      glyphs.set(GlyphPluginManager::objMap().at(glyphName).factory->getId(), newGlyph);
    //    cerr << glyphs.get(GlyphPluginManager::objMap[glyphName]->getId()) << endl;
    } delete itS;
  }

  void GlyphManager::clearGlyphList(Graph**,GlGraphInputData*,MutableContainer<Glyph *>& glyphs) {
    Iterator<string> *itS = GlyphPluginManager::availablePlugins();
    while (itS->hasNext()) {
      string glyphName=itS->next();
      delete glyphs.get(GlyphPluginManager::objMap().at(glyphName).factory->getId());
    } delete itS;
  }
} 
