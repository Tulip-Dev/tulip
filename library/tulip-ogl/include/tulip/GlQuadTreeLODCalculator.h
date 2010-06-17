/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
  class TLP_GL_SCOPE GlQuadTreeLODCalculator : public GlCPULODCalculator, public GraphObserver, public Observer, public GlSceneObserver {

  public:

    GlQuadTreeLODCalculator();
    ~GlQuadTreeLODCalculator();

    void setScene(GlScene *scene);

    bool needEntities();
    void setNeedEntities(bool);

    void addSimpleEntityBoundingBox(unsigned long entity,const BoundingBox& bb);
    void addNodeBoundingBox(unsigned int id,const BoundingBox& bb);
    void addEdgeBoundingBox(unsigned int id,const BoundingBox& bb);

    void compute(const Vector<int,4>& globalViewport,const Vector<int,4>& currentViewport);

    void computeFor3DCamera(LayerLODUnit *layerLODUnit,const Coord &eye,
                            const Matrix<float, 4> transformMatrix,
                            const Vector<int,4>& globalViewport,
                            const Vector<int,4>& currentViewport);

    void removeObservers();
    void addObservers();

    void setInputData(GlGraphInputData *newInputData);

    void update(std::set<Observable *>::iterator begin ,std::set<Observable *>::iterator end);
    void observableDestroyed(Observable *){}
    void addNode(Graph *,const node ){setHaveToCompute();}
    void addEdge(Graph *,const edge ){setHaveToCompute();}
    void delNode(Graph *,const node ){setHaveToCompute();}
    void delEdge(Graph *,const edge ){setHaveToCompute();}
    void addLocalProperty(Graph*, const std::string &name);
    void delLocalProperty(Graph*, const std::string &name);
    void destroy(Graph *);

    void addLayer(GlScene*, const std::string&, GlLayer*){setHaveToCompute();}
    void delLayer(GlScene*, const std::string&, GlLayer*){setHaveToCompute();}
    void modifyLayer(GlScene*, const std::string&, GlLayer*){setHaveToCompute();}
    void modifyEntity(GlScene *,GlSimpleEntity *){setHaveToCompute();}

    virtual GlLODCalculator *clone() {
      GlQuadTreeLODCalculator *newCalculator=new GlQuadTreeLODCalculator();
      newCalculator->setScene(scene);
      newCalculator->setInputData(inputData);
      return newCalculator;
    }

  protected :

    void setHaveToCompute();

    GlScene *scene;

    std::vector<QuadTreeNode<unsigned int> *> nodesQuadTree;
    std::vector<QuadTreeNode<unsigned int> *> edgesQuadTree;
    std::vector<QuadTreeNode<unsigned int> *> nodesSelectedQuadTree;
    std::vector<QuadTreeNode<unsigned int> *> edgesSelectedQuadTree;
    std::vector<QuadTreeNode<unsigned long> *> entitiesQuadTree;
    std::vector<std::vector<SimpleEntityLODUnit> > simpleEntities;

    bool haveToCompute;

    GlGraphInputData *inputData;

    BoundingBox nodesGlobalBoundingBox;
    BoundingBox edgesGlobalBoundingBox;
    BoundingBox entitiesGlobalBoundingBox;

    std::vector<std::pair<Camera*,Camera> > cameras;
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
