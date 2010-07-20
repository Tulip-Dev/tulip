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
#include "tulip/GlyphManager.h"

#include <tulip/tuliphash.h>

#include "tulip/Glyph.h"

//====================================================
#ifdef _WIN32 
#ifdef DLL_EXPORT
tlp::GlyphManager* tlp::GlyphManager::inst=0;
#endif
#else
tlp::GlyphManager* tlp::GlyphManager::inst=0;
#endif

using namespace std;

namespace tlp
{
  static TLP_HASH_MAP<int,std::string>   glyphIdToName;
  static TLP_HASH_MAP<std::string, int>  nameToGlyphId;

  GlyphManager::GlyphManager() {
    GlyphFactory::initFactory();
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
    Iterator<string> *itS=GlyphFactory::factory->availablePlugins();
    while (itS->hasNext()) {
      string pluginName=itS->next();
      int pluginId=GlyphFactory::factory->objMap[pluginName]->getId();
      glyphIdToName[pluginId]=pluginName;
      nameToGlyphId[pluginName]=pluginId;
    } delete itS;
  }
  //====================================================
  void GlyphManager::loadPlugins(PluginLoader *plug) {
    GlyphFactory::initFactory();
    string::const_iterator begin=tlp::TulipPluginsPath.begin();
    string::const_iterator end=begin;
    glyphIdToName.clear();
    nameToGlyphId.clear();
    while (end!=tlp::TulipPluginsPath.end())
      if ((*end)==tlp::PATH_DELIMITER) {
	if (begin!=end)
	  tlp::loadPluginsFromDir(string(begin,end)+"/glyphs", "Glyph", plug);
	++end;
	begin=end;
      } else
	++end;
    if (begin!=end) {
      tlp::loadPluginsFromDir(string(begin,end)+"/glyphs", "Glyph", plug);
    }
    loadGlyphPlugins();
  }
  //
  void GlyphManager::initGlyphList(Graph **graph,GlGraphInputData* glGraphInputData,MutableContainer<Glyph *>& glyphs) {
    GlyphContext gc = GlyphContext(graph,glGraphInputData);
    glyphs.setAll(GlyphFactory::factory->getPluginObject("3D - Cube OutLined", &gc));
    Iterator<string> *itS = GlyphFactory::factory->availablePlugins();
    while (itS->hasNext()) {
      string glyphName=itS->next();
      //cerr << "loading : " << glyphName << endl;
      Glyph *newGlyph = GlyphFactory::factory->getPluginObject(glyphName, &gc);
      //    cerr << newGlyph << endl;
      //if (GlyphFactory::factory->objMap[glyphName]->getId() == 0) 
      //  glyphs.setAll(newGlyph);
      //    cerr << " id:" << GlyphFactory::factory->objMap[glyphName]->getId() << endl;
      glyphs.set(GlyphFactory::factory->objMap[glyphName]->getId(), newGlyph);
    //    cerr << glyphs.get(GlyphFactory::factory->objMap[glyphName]->getId()) << endl;
    } delete itS;
  }

  void GlyphManager::clearGlyphList(Graph**,GlGraphInputData* glGraphInputData,MutableContainer<Glyph *>& glyphs) {
    Iterator<string> *itS = GlyphFactory::factory->availablePlugins();
    while (itS->hasNext()) {
      string glyphName=itS->next();
      delete glyphs.get(GlyphFactory::factory->objMap[glyphName]->getId());
    } delete itS;
  }
} 
