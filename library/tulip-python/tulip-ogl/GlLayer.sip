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

class GlLayer {
%TypeHeaderCode
#include <tulip/GlLayer.h>
%End
public:

	GlLayer(const std::string& name);

	void setScene(tlp::GlScene *scene);

	tlp::GlScene *getScene();

	std::string getName();

	void setCamera(const tlp::Camera& camera);

	void set2DMode();

	tlp::Camera &getCamera();

	void setVisible(bool visible);
	
	bool isVisible();

	void addGlEntity(tlp::GlSimpleEntity *entity /Transfer/, const std::string& name);

	void deleteGlEntity(const std::string &key);
	
	void deleteGlEntity(tlp::GlSimpleEntity *entity);

	tlp::GlSimpleEntity* findGlEntity(const std::string &key);

	std::map<std::string, tlp::GlSimpleEntity*> *getDisplays();

	void acceptVisitor(tlp::GlSceneVisitor *visitor);

	tlp::GlComposite *getComposite();

	void clear();
};
};