/*
 * EdgeExtremityGlyphManager.h
 *
 *  Created on: 8 avr. 2009
 *      Author: Jonathan Dubois
 *       Email: jonathan.dubois@labri.fr
 *
 *	This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 */

#ifndef EDGEEXTREMITYGLYPHMANAGER_H_
#define EDGEEXTREMITYGLYPHMANAGER_H_

#include <string>
#include <tulip/tulipconf.h>
//#include <tulip/PluginLoader.h>
//#include <tulip/GlGraphInputData.h>
#include <tulip/MutableContainer.h>

namespace tlp {
class PluginLoader;
class GlGraphInputData;
class EdgeExtremityGlyph;
class Graph;

class TLP_GL_SCOPE EdgeExtremityGlyphManager {

public:

	virtual ~EdgeExtremityGlyphManager() {
	}

	/**
	 * Return the singleton (if the singleton doesn't exist this function create it)
	 */
	static EdgeExtremityGlyphManager &getInst() {
		if (!eeinst)
			eeinst = new EdgeExtremityGlyphManager();
		return *eeinst;
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
	void loadPlugins(PluginLoader *plug = 0);

	/**
	 * Create the glyph list and store it in glyphs parameter
	 */
	void initGlyphList(Graph **graph, GlGraphInputData* glGraphInputData,
			MutableContainer<EdgeExtremityGlyph *>& glyphs);
	/**
	 * Clear the glyph list
	 */
	void clearGlyphList(Graph **graph, GlGraphInputData* glGraphInputData,
			MutableContainer<EdgeExtremityGlyph *>& glyphs);

private:

	EdgeExtremityGlyphManager();

	static EdgeExtremityGlyphManager* eeinst;
};
}
#endif /* EDGEEXTREMITYGLYPHMANAGER_H_ */
