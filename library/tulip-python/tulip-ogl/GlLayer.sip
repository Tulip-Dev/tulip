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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired for bindings by the Booggie project development team           |
// | (http://booggie.org/)                                                   |
// +-------------------------------------------------------------------------+

namespace tlp {
// TODO - inherits GlObservableScene
class GlLayer {
%TypeHeaderCode
#include <tulip/GlLayer.h>
%End
public:

	/**
	 * Layer constructor : construct a layer with his name
	 */
	GlLayer(const std::string& name);

	/**
	 * Set the scene where the layer is
	 */
	void setScene(tlp::GlScene *scene);

	/**
	 * Return the scene where the layer is
	 */
	tlp::GlScene *getScene();

	/**
	 * Return the layer's name
	 */
	std::string getName();


	/**
	 * Set the layer's camera
	 */
	void setCamera(const tlp::Camera& camera);

	/**
	 * Replace the layer's camera with a new 2D one
	 */
	void set2DMode();

	/**
	 * Return the layer's camera
	 */
	tlp::Camera &getCamera();

	/**
	 * Set if the layer is visible
	 */
	void setVisible(bool visible);

	/**
	 * Return if the layer is visible
	 */
	bool isVisible();

	
//	/**
//	 * Add an entity to GlComposite of the layer
//	 */
//	void addGlEntity(GlSimpleEntity *entity,const std::string& name);
//
//	/**
//	 * Delete entity with name : key
//	 */
//	void deleteGlEntity(const std::string &key);
//
//	/**
//	 * Delete entity
//	 */
//	void deleteGlEntity(GlSimpleEntity *entity);
//
//	/**
//	 * Return entity with name : key
//	 */
//	GlSimpleEntity* findGlEntity(const std::string &key);
//
//	/**
//	 * Return the map of layer's entities
//	 */
//	std::map<std::string, GlSimpleEntity*> *getDisplays();
//
//	/**
//	 * function used by visitors to visit this layer
//	 */
//	void acceptVisitor(GlSceneVisitor *visitor);
//
//	/**
//	 * Return the GlComposite used by the layer
//	 */
//	GlComposite *getComposite() {return &composite;}
//
//	/**
//	 * Remove all entities of the layer
//	 */
//	void clear() {composite.reset(false);}
//
//	/**
//	 * Return the layer's data in XML
//	 */
//	void getXML(xmlNodePtr rootNode);
//
//	/**
//	 * Set the layer's data with XML
//	 */
//	void setWithXML(xmlNodePtr rootNode);
};
};