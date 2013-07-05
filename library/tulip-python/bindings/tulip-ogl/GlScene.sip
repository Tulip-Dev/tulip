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
#include <tulip/GlLODCalculator.h>
typedef std::pair<std::string, tlp::GlLayer*> pairStringGlLayer;
namespace tlp {
typedef tlp::Vector<int, 4> Vec4i;
}
%End

typedef std::pair<std::string, tlp::GlLayer*> pairStringGlLayer;

namespace tlp {

enum RenderingEntitiesFlag {
  RenderingSimpleEntities =1,
  RenderingNodes = 2,
  RenderingEdges = 4,
  RenderingAll = 7,
  RenderingWithoutRemove = 8
};

struct SelectedEntity {
%TypeHeaderCode
#include <tulip/GlScene.h>
%End

  enum SelectedEntityType {
    UNKNOW_SELECTED = 0,
    NODE_SELECTED = 1,
    EDGE_SELECTED = 2,
    SIMPLE_ENTITY_SELECTED = 3
  };

  SelectedEntity();

  SelectedEntity(tlp::GlSimpleEntity *entity);

  SelectedEntity(tlp::Graph *graph,unsigned int id, SelectedEntityType type);

  tlp::GlSimpleEntity *getSimpleEntity() const;

  unsigned int getComplexEntityId() const;

  tlp::Graph *getComplexEntityGraph() const;

  SelectedEntityType getEntityType() const;

  tlp::node getNode() const;

  tlp::edge getEdge() const;

};

class GlScene  : tlp::Observable {
%TypeHeaderCode
#include <tulip/GlScene.h>
%End

%ConvertToSubClassCode
    if (dynamic_cast<tlp::GlScene*>(sipCpp)) {
      sipType = sipFindType("tlp::GlScene");
    } else {
      sipType = NULL;
    }
%End

public:

	GlScene();
	
	void initGlParameters();

	void draw();

	void centerScene();

	void ajustSceneToSize(int width, int height);

	void zoomXY(int step, const int x, const int y);

	void zoom(float factor,const tlp::Coord& dest);

	void zoom(int step);

	void translateCamera(const int x, const int y, const int z);

	void rotateScene(const int x, const int y, const int z);

    bool selectEntities(tlp::RenderingEntitiesFlag type, int x, int y, int h, int w, tlp::GlLayer *layer, std::vector<tlp::SelectedEntity>& selectedEntities /Out/);

	unsigned char * getImage();

	void setViewport(tlp::Vec4i &newViewport);
	
	void setViewport(int x, int y, int width, int height);

	tlp::Vec4i getViewport();

	void setBackgroundColor(const tlp::Color& color);

	tlp::Color getBackgroundColor();

	void setViewOrtho(bool viewOrtho);

	bool isViewOrtho();

    tlp::GlLayer *createLayer(const std::string &name);

    tlp::GlLayer *createLayerBefore(const std::string &layerName,const std::string &beforeLayerWithName);

    tlp::GlLayer *createLayerAfter(const std::string &layerName,const std::string &afterLayerWithName);

    void addExistingLayer(tlp::GlLayer *layer /Transfer/);

    bool addExistingLayerBefore(tlp::GlLayer *layer /Transfer/, const std::string &beforeLayerWithName);
	
    bool addExistingLayerAfter(tlp::GlLayer *layer, const std::string &afterLayerWithName);

	tlp::GlLayer* getLayer(const std::string& name);
	
    void removeLayer(const std::string& name,bool deleteLayer=true);

    void removeLayer(tlp::GlLayer *layer,bool deleteLayer=true);

    const std::vector<pairStringGlLayer> &getLayersList();

	void clearLayersList();

    tlp::BoundingBox getBoundingBox();

    tlp::GlGraphComposite* getGlGraphComposite();

	tlp::GlLayer* getGraphLayer();
	
    tlp::Camera getGraphCamera();

    void setGraphCamera(tlp::Camera* camera);

    void setClearBufferAtDraw(bool clear);

    bool getClearBufferAtDraw() const;

};
};
