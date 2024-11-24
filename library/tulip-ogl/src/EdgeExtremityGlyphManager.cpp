/**
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
#include <list>
#include <tulip/tuliphash.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include <tulip/Glyph.h>
#include <tulip/PluginLister.h>
#include <tulip/TulipViewSettings.h>

using namespace std;

namespace tlp {
static std::list<std::string> plugins;
static tlp_hash_map<int, std::string> eeglyphIdToName;
static tlp_hash_map<std::string, int> nameToEeGlyphId;

//====================================================
string EdgeExtremityGlyphManager::glyphName(int id) {
  if (id == EdgeExtremityShape::None) {
    return string("NONE");
  }

  auto it = eeglyphIdToName.find(id);
  if (it != eeglyphIdToName.end()) {
    return it->second;
  } else {
    tlp::warning() << __PRETTY_FUNCTION__ << endl;
    tlp::warning() << "Invalid glyph id" << endl;
    return string("invalid");
  }
}
//====================================================
int EdgeExtremityGlyphManager::glyphId(const string &name) {
  if (name.compare("NONE") == 0) {
    return EdgeExtremityShape::None;
  }

  auto it = nameToEeGlyphId.find(name);
  if (it != nameToEeGlyphId.end()) {
    return it->second;
  } else {
    tlp::warning() << __PRETTY_FUNCTION__ << endl;
    tlp::warning() << "Invalid glyph name" << endl;
    return 0;
  }
}
//====================================================
void EdgeExtremityGlyphManager::loadGlyphPlugins() {
  plugins = PluginLister::availablePlugins<EdgeExtremityGlyph>();

  for (const std::string &pluginName : plugins) {
    int pluginId = PluginLister::pluginInformation(pluginName).id();
    eeglyphIdToName[pluginId] = pluginName;
    nameToEeGlyphId[pluginName] = pluginId;
  }
}
//====================================================
void EdgeExtremityGlyphManager::initGlyphList(Graph **graph, GlGraphInputData *glGraphInputData,
                                              MutableContainer<EdgeExtremityGlyph *> &glyphs) {
  GlyphContext gc = GlyphContext(graph, glGraphInputData);
  glyphs.setAll(nullptr);

  for (const string &glyphName : plugins) {
    EdgeExtremityGlyph *newGlyph =
        PluginLister::getPluginObject<EdgeExtremityGlyph>(glyphName, &gc);
    glyphs.set(PluginLister::pluginInformation(glyphName).id(), newGlyph);
  }
}

void EdgeExtremityGlyphManager::clearGlyphList(Graph **, GlGraphInputData *,
                                               MutableContainer<EdgeExtremityGlyph *> &glyphs) {

  for (const string &glyphName : plugins) {
    delete glyphs.get(PluginLister::pluginInformation(glyphName).id());
  }
}
} // namespace tlp
