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
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include <tulip/tuliphash.h>
#include <tulip/ForEach.h>
#include <tulip/Glyph.h>
#include <tulip/TulipViewSettings.h>

tlp::EdgeExtremityGlyphManager* tlp::EdgeExtremityGlyphManager::eeinst = NULL;

using namespace std;

namespace tlp {
static TLP_HASH_MAP<int, std::string> eeglyphIdToName;
static TLP_HASH_MAP<std::string, int> nameToEeGlyphId;

EdgeExtremityGlyphManager::EdgeExtremityGlyphManager() {
}
//====================================================
string EdgeExtremityGlyphManager::glyphName(int id) {
  if (id == EdgeExtremityShape::None) {
    return string("NONE");
  }

  if (eeglyphIdToName.find(id) != eeglyphIdToName.end()) {
    return eeglyphIdToName[id];
  }
  else {
    tlp::warning() << __PRETTY_FUNCTION__ << endl;
    tlp::warning() << "Invalid glyph id" << endl;
    return string("invalid");
  }
}
//====================================================
int EdgeExtremityGlyphManager::glyphId(const string& name) {
  if (name.compare("NONE") == 0) {
    return EdgeExtremityShape::None;
  }

  if (nameToEeGlyphId.find(name) != nameToEeGlyphId.end()) {
    return nameToEeGlyphId[name];
  }
  else {
    tlp::warning() << __PRETTY_FUNCTION__ << endl;
    tlp::warning() << "Invalid glyph name" << endl;
    return 0;
  }
}
//====================================================
void EdgeExtremityGlyphManager::loadGlyphPlugins() {
  static std::list<std::string> plugins = PluginLister::instance()->availablePlugins<EdgeExtremityGlyph>();

  for(std::list<std::string>::const_iterator it = plugins.begin(); it != plugins.end(); ++it) {
    string pluginName = *it;
    int pluginId = PluginLister::pluginInformation(pluginName).id();
    eeglyphIdToName[pluginId] = pluginName;
    nameToEeGlyphId[pluginName] = pluginId;
  }
}
//====================================================
void EdgeExtremityGlyphManager::initGlyphList(Graph **graph,
    GlGraphInputData* glGraphInputData, MutableContainer<
    EdgeExtremityGlyph *>& glyphs) {
  GlyphContext gc = GlyphContext(graph,
                                 glGraphInputData);
  glyphs.setAll(0);


  static std::list<std::string> plugins = PluginLister::instance()->availablePlugins<EdgeExtremityGlyph>();

  for(std::list<std::string>::const_iterator it = plugins.begin(); it != plugins.end(); ++it) {
    string glyphName = *it;
    EdgeExtremityGlyph *newGlyph = PluginLister::instance()->getPluginObject<EdgeExtremityGlyph>(glyphName, &gc);
    glyphs.set(PluginLister::pluginInformation(glyphName).id(), newGlyph);
  }
}

void EdgeExtremityGlyphManager::clearGlyphList(Graph **, GlGraphInputData*, MutableContainer<EdgeExtremityGlyph *>& glyphs) {
  static std::list<std::string> plugins = PluginLister::instance()->availablePlugins<EdgeExtremityGlyph>();

  for(std::list<std::string>::const_iterator it = plugins.begin(); it != plugins.end(); ++it) {
    string glyphName = *it;
    delete glyphs.get(PluginLister::pluginInformation(glyphName).id());
  }
}

}
