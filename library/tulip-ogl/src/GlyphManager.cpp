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

#include <tulip/GlyphManager.h>
#include <tulip/Glyph.h>
#include <tulip/PluginLister.h>

//====================================================
using namespace std;

namespace tlp {
static std::list<std::string> glyphList;
static tlp_hash_map<int, std::string> glyphIdToName;
static tlp_hash_map<std::string, int> nameToGlyphId;

//====================================================
string GlyphManager::glyphName(int id) {
  auto it = glyphIdToName.find(id);
  if (it != glyphIdToName.end()) {
    return it->second;
  } else {
    assert(false);
    tlp::warning() << "Invalid glyph id: " << id << endl;
    return string("invalid");
  }
}
//====================================================
int GlyphManager::glyphId(const string &name, bool warnIfNotFound) {
  auto it = nameToGlyphId.find(name);
  if (it != nameToGlyphId.end()) {
    return it->second;
  } else {
    if (warnIfNotFound) {
      assert(false);
      tlp::warning() << "Invalid glyph name: \"" << name.c_str() << '"' << endl;
    }
    return 0;
  }
}
//====================================================
void GlyphManager::loadGlyphPlugins() {
  glyphList = PluginLister::availablePlugins<Glyph>();

  for (const std::string &pluginName : glyphList) {
    int pluginId = PluginLister::pluginInformation(pluginName).id();
    glyphIdToName.emplace(pluginId, pluginName);
    nameToGlyphId.emplace(pluginName, pluginId);
  }
}

void GlyphManager::initGlyphList(Graph **graph, GlGraphInputData *glGraphInputData,
                                 MutableContainer<Glyph *> &glyphs) {
  // first destroy current default glyph
  Glyph *defaultGlyph = glyphs.getDefault();

  if (defaultGlyph)
    delete defaultGlyph;

  // then set a new one
  GlyphContext gc = GlyphContext(graph, glGraphInputData);
  glyphs.setAll(PluginLister::getPluginObject<Glyph>("3D - Cube OutLined", &gc));

  for (const std::string &glyphName : glyphList) {
    Glyph *newGlyph = PluginLister::getPluginObject<Glyph>(glyphName, &gc);
    glyphs.set(PluginLister::pluginInformation(glyphName).id(), newGlyph);
  }
}

void GlyphManager::clearGlyphList(Graph **, GlGraphInputData *, MutableContainer<Glyph *> &glyphs) {

  for (const std::string &glyphName : glyphList) {
    delete glyphs.get(PluginLister::pluginInformation(glyphName).id());
  }

  delete glyphs.getDefault();
}
} // namespace tlp
