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
#ifndef Tulip_QLQUADTREELODCALCULATOR_H
#define Tulip_QLQUADTREELODCALCULATOR_H

#include <map>
#include <vector>

#include <tulip/BoundingBox.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/Matrix.h>
#include <tulip/Vector.h>
#include <tulip/GlSceneObserver.h>
#include <tulip/GlCPULODCalculator.h>

namespace tlp {

  class Camera;
  template <class TYPE> class QuadTreeNode;
  class GlScene;

  /**
   * Class use to compute bounding boxs of a vector of GlEntity
   */
  class TLP_GL_SCOPE GlQuadTreeLODCalculator : public GlCPULODCalculator, public GraphObserver, public PropertyObserver, public Observable {

  public:

    GlQuadTreeLODCalculator();
    ~GlQuadTreeLODCalculator();

    void setScene(GlScene &scene);

    bool needEntities();
    void setNeedEntities(bool);

    void addSimpleEntityBoundingBox(GlSimpleEntity * entity,const BoundingBox& bb);
    void addNodeBoundingBox(unsigned int id,const BoundingBox& bb);
    void addEdgeBoundingBox(unsigned int id,const BoundingBox& bb);

    void compute(const Vector<int,4>& globalViewport,const Vector<int,4>& currentViewport);

    void computeFor3DCamera(LayerLODUnit *layerLODUnit,const Coord &eye,
                            const Matrix<float, 4> transformMatrix,
                            const Vector<int,4>& globalViewport,
                            const Vector<int,4>& currentViewport);

    void setInputData(GlGraphInputData *newInputData);

    virtual GlLODCalculator *clone() {
      GlQuadTreeLODCalculator *newCalculator=new GlQuadTreeLODCalculator();
      newCalculator->setScene(*glScene);
      newCalculator->setInputData(inputData);
      return newCalculator;
    }

  protected :

    void update(PropertyInterface *property);
    void treatEvent(const Event &ev);
    void observableDestroyed(Observable *){}
    void afterSetNodeValue(PropertyInterface*,const node n);
    void afterSetEdgeValue(PropertyInterface*,const edge e);
    void afterSetAllNodeValue(PropertyInterface*);
    void afterSetAllEdgeValue(PropertyInterface*);
    void addNode(Graph *,const node ){setHaveToCompute();}
    void addEdge(Graph *,const edge ){setHaveToCompute();}
    void delNode(Graph *,const node ){setHaveToCompute();}
    void delEdge(Graph *,const edge ){setHaveToCompute();}
    void addLocalProperty(Graph*, const std::string &name);
    void delLocalProperty(Graph*, const std::string &name);
    void destroy(Graph *);
    void destroy(const Camera *);

    void removeObservers();
    void addObservers();

    void initCamerasObservers();
    void clearCamerasObservers();

    void setHaveToCompute();

    std::vector<QuadTreeNode<unsigned int> *> nodesQuadTree;
    std::vector<QuadTreeNode<unsigned int> *> edgesQuadTree;
    std::vector<QuadTreeNode<GlSimpleEntity *> *> entitiesQuadTree;
    std::vector<std::vector<SimpleEntityLODUnit> > simpleEntities;

    bool haveToCompute;

    BoundingBox nodesGlobalBoundingBox;
    BoundingBox edgesGlobalBoundingBox;
    BoundingBox entitiesGlobalBoundingBox;

    std::vector<Camera *> cameras;
    std::map<GlLayer*,std::pair<Camera*, Camera> > layerToCamera;
    Camera *currentCamera;
    Graph *currentGraph;
    PropertyInterface *layoutProperty;
    PropertyInterface *sizeProperty;
    PropertyInterface *selectionProperty;

    int quadTreesVectorPosition;
    int simpleEntitiesVectorPosition;
  };

}

#endif // Tulip_QTQUADTREELODCALCULATOR_H
