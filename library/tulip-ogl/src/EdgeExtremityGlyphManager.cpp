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
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include <tulip/tuliphash.h>

#ifdef _WIN32
#ifdef DLL_EXPORT
tlp::EdgeExtremityGlyphManager* tlp::EdgeExtremityGlyphManager::eeinst=0;
#endif
#else
tlp::EdgeExtremityGlyphManager* tlp::EdgeExtremityGlyphManager::eeinst = 0;
#endif

using namespace std;

namespace tlp {
static TLP_HASH_MAP<int, std::string> eeglyphIdToName;
static TLP_HASH_MAP<std::string, int> nameToEeGlyphId;

EdgeExtremityGlyphManager::EdgeExtremityGlyphManager() {
	EdgeExtremityGlyphFactory::initFactory();
}
//====================================================
string EdgeExtremityGlyphManager::glyphName(int id) {
	if (id == 0)
		return string("NONE");
	if (eeglyphIdToName.find(id) != eeglyphIdToName.end())
		return eeglyphIdToName[id];
	else {
		cerr << __PRETTY_FUNCTION__ << endl;
		cerr << "Invalid glyph id" << endl;
		return string("invalid");
	}
}
//====================================================
int EdgeExtremityGlyphManager::glyphId(string name) {
	if (name.compare("NONE") == 0)
		return 0;
	if (nameToEeGlyphId.find(name) != nameToEeGlyphId.end())
		return nameToEeGlyphId[name];
	else {
		cerr << __PRETTY_FUNCTION__ << endl;
		cerr << "Invalid glyph name" << endl;
		return 0;
	}
}
//====================================================
void EdgeExtremityGlyphManager::loadGlyphPlugins() {
		Iterator<string> *itS =
			EdgeExtremityGlyphFactory::factory->availablePlugins();
	while (itS->hasNext()) {
		string pluginName = itS->next();
		int pluginId =
				EdgeExtremityGlyphFactory::factory->objMap[pluginName]->getId();
		eeglyphIdToName[pluginId] = pluginName;
		nameToEeGlyphId[pluginName] = pluginId;
	}
	delete itS;

}
//====================================================
void EdgeExtremityGlyphManager::loadPlugins(PluginLoader *plug) {
		EdgeExtremityGlyphFactory::initFactory();
	string::const_iterator begin = tlp::TulipPluginsPath.begin();
	string::const_iterator end = begin;
	eeglyphIdToName.clear();
	nameToEeGlyphId.clear();
	while (end != tlp::TulipPluginsPath.end())
		if ((*end) == tlp::PATH_DELIMITER) {
			if (begin != end)
				tlp::loadPluginsFromDir(string(begin, end) + "/glyphs",
						"Glyph", plug);
			++end;
			begin = end;
		} else
			++end;
	if (begin != end) {
		tlp::loadPluginsFromDir(string(begin, end) + "/glyphs", "Glyph", plug);
	}
	loadGlyphPlugins();
}
//
void EdgeExtremityGlyphManager::initGlyphList(Graph **graph,
		GlGraphInputData* glGraphInputData, MutableContainer<
				EdgeExtremityGlyph *>& glyphs) {
		EdgeExtremityGlyphContext gc = EdgeExtremityGlyphContext(graph,
				glGraphInputData);
		glyphs.setAll(0);
		Iterator<string> *itS =
				EdgeExtremityGlyphFactory::factory->availablePlugins();
		while (itS->hasNext()) {
			string glyphName = itS->next();
			EdgeExtremityGlyph *newGlyph =
					EdgeExtremityGlyphFactory::factory->getPluginObject(glyphName,
							&gc);
			glyphs.set(
					EdgeExtremityGlyphFactory::factory->objMap[glyphName]->getId(),
					newGlyph);
		}
		delete itS;
}

void EdgeExtremityGlyphManager::clearGlyphList(Graph **,
		GlGraphInputData*, MutableContainer<
				EdgeExtremityGlyph *>& glyphs) {
		Iterator<string> *itS =
			EdgeExtremityGlyphFactory::factory->availablePlugins();
	while (itS->hasNext()) {
		string glyphName = itS->next();
		delete glyphs.get(
				EdgeExtremityGlyphFactory::factory->objMap[glyphName]->getId());
	}
	delete itS;
}
}
