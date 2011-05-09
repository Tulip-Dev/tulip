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

%ModuleHeaderCode
#include <tulip/Vector.h>
#include <tulip/GlLayer.h>
typedef std::pair<std::string, tlp::GlLayer*> std_pair_0100std_string_0101tlp_GlLayer;
namespace tlp {
typedef tlp::Vector<int, 4> Vec4i;
}
%End

typedef std::pair<std::string, tlp::GlLayer*> std_pair_0100std_string_0101tlp_GlLayer;

namespace tlp {

class GlScene  : tlp::Observable {
%TypeHeaderCode
#include <tulip/GlScene.h>
%End
public:

	GlScene();
	
	void initGlParameters();

	void prerenderMetaNodes();

	void draw();

	void centerScene();

	void ajustSceneToSize(int width, int height);

	void zoomXY(int step, const int x, const int y);

	void zoom(float factor,const tlp::Coord& dest);

	void zoom(int step);

	void translateCamera(const int x, const int y, const int z);

	void rotateScene(const int x, const int y, const int z);

	unsigned char * getImage();

	void setViewport(tlp::Vec4i &newViewport);
	
	void setViewport(int x, int y, int width, int height);

	tlp::Vec4i getViewport();

	void setBackgroundColor(const tlp::Color& color);

	tlp::Color getBackgroundColor();

	void setViewOrtho(bool viewOrtho);

	bool isViewOrtho();

	void setViewLabel(bool viewLabel);

	void addLayer(tlp::GlLayer *layer /Transfer/);
	
	tlp::GlLayer* getLayer(const std::string& name);
	
	std::vector<std_pair_0100std_string_0101tlp_GlLayer>* getLayersList();

	void clearLayersList();

	void getXML(std::string &out /Out/);

	void setWithXML(std::string &in, tlp::Graph *graph);

	/*
	TODO - GlGraphComposite
	void addGlGraphCompositeInfo(tlp::GlLayer* layer, tlp::GlGraphComposite *glGraphComposite);
	tlp::GlGraphComposite* getGlGraphComposite();
	*/
	
	tlp::GlLayer* getGraphLayer();
	
	tlp::Camera& getCamera();
	
	void setCamera(const tlp::Camera& camera);
};
};