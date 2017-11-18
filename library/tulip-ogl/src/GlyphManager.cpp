/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <tulip/GlyphManager.h>

#include <tulip/tuliphash.h>

#include <tulip/Glyph.h>

//====================================================
std::list<std::string> tlp::GlyphManager::glyphList;
tlp::GlyphManager tlp::GlyphManager::inst;

using namespace std;

namespace tlp {
static TLP_HASH_MAP<int, std::string> glyphIdToName;
static TLP_HASH_MAP<std::string, int> nameToGlyphId;

GlyphManager::GlyphManager() {}
//====================================================
string GlyphManager::glyphName(int id) {
  if (glyphIdToName.find(id) != glyphIdToName.end()) {
    return glyphIdToName[id];
  } else {
    assert(false);
    tlp::warning() << __PRETTY_FUNCTION__ << endl;
    tlp::warning() << "Invalid glyph id: " << id << endl;
    return string("invalid");
  }
}
//====================================================
int GlyphManager::glyphId(const string &name) {
  if (nameToGlyphId.find(name) != nameToGlyphId.end()) {
    return nameToGlyphId[name];
  } else {
    assert(false);
    tlp::warning() << __PRETTY_FUNCTION__ << endl;
    tlp::warning() << "Invalid glyph name: \"" << name.c_str() << '"' << endl;
    return 0;
  }
}
//====================================================
void GlyphManager::loadGlyphPlugins() {
  glyphList = PluginLister::instance()->availablePlugins<Glyph>();

  for (std::list<std::string>::const_iterator it = glyphList.begin(); it != glyphList.end(); ++it) {
    string pluginName(*it);
    int pluginId = PluginLister::pluginInformation(pluginName).id();
    glyphIdToName[pluginId] = pluginName;
    nameToGlyphId[pluginName] = pluginId;
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
  glyphs.setAll(PluginLister::instance()->getPluginObject<Glyph>("3D - Cube OutLined", &gc));

  for (std::list<std::string>::const_iterator it = glyphList.begin(); it != glyphList.end(); ++it) {
    string glyphName(*it);
    Glyph *newGlyph = PluginLister::instance()->getPluginObject<Glyph>(glyphName, &gc);
    glyphs.set(PluginLister::pluginInformation(glyphName).id(), newGlyph);
  }
}

void GlyphManager::clearGlyphList(Graph **, GlGraphInputData *, MutableContainer<Glyph *> &glyphs) {

  for (std::list<std::string>::const_iterator it = glyphList.begin(); it != glyphList.end(); ++it) {
    string glyphName(*it);
    delete glyphs.get(PluginLister::pluginInformation(glyphName).id());
  }

  delete glyphs.getDefault();
}
}
