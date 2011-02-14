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
// | Original Bindings by the Booggie project developpment team              |
// | (http://booggie.org/)                                                   |
// | Improved and maintained by Antoine Lambert 			                 |
// | Contact:  antoine.lambert@labri.fr 								     |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | Tulip Python Bindings is free software; you can redistribute it         |
// | and/or modify  it under the terms of the GNU General Public License     |
// | as published by the Free Software Foundation; either version 2 of the   |
// | License, or (at your option) any later version.                         |
// |                                                                         |
// | Tulip Python Bindings is distributed in the hope that it will be        |
// | useful, but WITHOUT ANY WARRANTY; without even the implied warranty of  |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with this program.  If not, see <http://www.gnu.org/licenses/>.   |
// |                                                                         |
// +-------------------------------------------------------------------------+

namespace tlp {
// TODO - inherits GlObservableScene
class GlScene  {
%TypeHeaderCode
#include <tulip/GlScene.h>
%End
public:
	/*
	 * TODO - GlLODCalculator
	 * GlScene(GlLODCalculator *calculator=NULL);
	 */
	

	void initGlParameters();

	void prerenderMetaNodes();

	void draw();

	void centerScene();

	void computeAjustSceneToSize(int width, int height, tlp::Coord *center, tlp::Coord *eye, float *sceneRadius, float *xWhiteFactor, float *yWhiteFactor);

	void ajustSceneToSize(int width, int height);

	void zoomXY(int step, const int x, const int y);

	void zoom(float factor,const tlp::Coord& dest);

	void zoom(int step);

	void translateCamera(const int x, const int y, const int z);

	void rotateScene(const int x, const int y, const int z);

	/*
	TODO - SelectionFlag, GlLayer
	bool selectEntities(SelectionFlag type, int x, int y, int h, int w,GlLayer *layer, std::vector<unsigned long>& selectedEntities);
	*/
	
	void outputSVG(unsigned int size,const std::string& filename);

	void outputEPS(unsigned int size,const std::string& filename);

	unsigned char * getImage();

	/*
	TODO - Vector
	void setViewport(Vector<int, 4> newViewport) {viewport=newViewport;}
	*/
	
	void setViewport(int x, int y, int width, int height);

	/*
	TODO - Vector
	Vector<int, 4> getViewport() {return viewport;}
	*/
	
	void setBackgroundColor(const tlp::Color& color);

	tlp::Color getBackgroundColor();

	void setViewOrtho(bool viewOrtho);

	bool isViewOrtho();

	void setViewLabel(bool viewLabel);

	/*
	TODO - GlLayer
	void addLayer(GlLayer *layer);
	
	GlLayer* getLayer(const std::string& name);
	
	std::vector<std::pair<std::string, GlLayer*> >* getLayersList() {return &layersList;}
	*/
	
	void clearLayersList();

	void getXML(std::string &out);

	void setWithXML(std::string &in, tlp::Graph *graph);

	/*
	TODO - GlLayer, GlGraphComposite
	void addGlGraphCompositeInfo(GlLayer* layer,GlGraphComposite *glGraphComposite) {this->graphLayer=layer;this->glGraphComposite=glGraphComposite;}
	GlGraphComposite* getGlGraphComposite() {return glGraphComposite;}
	GlLayer* getGraphLayer() {return graphLayer;}

	GlLayer* getSelectionLayer() {return selectionLayer;}
	*/
	
	tlp::Camera& getCamera();
	
	void setCamera(const tlp::Camera& camera);
};
};