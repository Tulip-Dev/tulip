/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
///@cond DOXYGEN_HIDDEN

#ifndef Tulip_GLLODCALCULATOR_H
#define Tulip_GLLODCALCULATOR_H
#ifndef DOXYGEN_NOTFOR_DEVEL

#include <climits>
#include <vector>

#include <tulip/BoundingBox.h>
#include <tulip/GlSceneVisitor.h>
#include <tulip/GlSimpleEntity.h>
#include <tulip/GlNode.h>
#include <tulip/GlEdge.h>
#include <tulip/GlLayer.h>

namespace tlp {

class Camera;
class GlEntity;
class GlSimpleEntity;
class GlScene;
class GlGraphInputData;

enum RenderingEntitiesFlag {
  RenderingSimpleEntities = 1,
  RenderingNodes = 2,
  RenderingEdges = 4,
  RenderingAll = 7,
  RenderingWithoutRemove = 8
};

struct EntityLODUnit {
  EntityLODUnit() : lod(-1) {}
  EntityLODUnit(const BoundingBox &boundingBox) : boundingBox(boundingBox), lod(-1) {}
  BoundingBox boundingBox;
  float lod;
};

// struct to store simple entity lod
struct SimpleEntityLODUnit : public EntityLODUnit {
  SimpleEntityLODUnit(GlSimpleEntity *entity = nullptr) : EntityLODUnit(), entity(entity) {}
  SimpleEntityLODUnit(GlSimpleEntity *entity, const BoundingBox &boundingBox)
      : EntityLODUnit(boundingBox), entity(entity) {}
  GlSimpleEntity *entity;

  void init(GlSimpleEntity *e, const BoundingBox &bb) {
    entity = e;
    boundingBox = bb;
  }
};

// struct to store complex entity (nodes/edges) lod
struct ComplexEntityLODUnit : public EntityLODUnit {
  ComplexEntityLODUnit(unsigned int id = UINT_MAX, unsigned int pos = UINT_MAX)
      : EntityLODUnit(), id(id), pos(pos) {}
  ComplexEntityLODUnit(unsigned int id, unsigned int pos, const BoundingBox &boundingBox)
      : EntityLODUnit(boundingBox), id(id), pos(pos) {}

  void init(unsigned int i, unsigned int p, const BoundingBox &bb) {
    id = i;
    pos = p;
    boundingBox = bb;
  }

  unsigned int id;
  unsigned int pos;
};

struct LayerLODUnit {
  std::vector<SimpleEntityLODUnit> simpleEntitiesLODVector;
  std::vector<ComplexEntityLODUnit> nodesLODVector;
  std::vector<ComplexEntityLODUnit> edgesLODVector;
  Camera *camera;
};

typedef std::vector<LayerLODUnit> LayersLODVector;

/**
 * Class use to calculate lod of scene entities
 */
class TLP_GL_SCOPE GlLODCalculator : public GlSceneVisitor {

public:
  GlLODCalculator() : glScene(nullptr), inputData(nullptr), attachedLODCalculator(nullptr) {}
  virtual ~GlLODCalculator() {}
  virtual GlLODCalculator *clone() = 0;

  /**
   * Visit a GlSimpleEntity
   */
  void visit(GlSimpleEntity *entity) override {
    addSimpleEntityBoundingBox(entity, entity->getBoundingBox());
  }

  /**
   * Visit a node
   */
  void visit(GlNode *glNode) override {
    addNodeBoundingBox(glNode->id, glNode->pos, glNode->getBoundingBox(inputData));
  }
  /**
   * Visit an Edge
   */
  void visit(GlEdge *glEdge) override {
    addEdgeBoundingBox(glEdge->id, glEdge->pos, glEdge->getBoundingBox(inputData));
  }
  /**
   * Visit a layer
   */
  void visit(GlLayer *layer) override {
    beginNewCamera(&layer->getCamera());
  }

  /**
   * Reserve memory to store nodes and edges LOD
   */
  void reserveMemoryForGraphElts(unsigned int /*nbNodes*/, unsigned int /*nbEdges*/) override {}
  /**
   * Set scene use by this LOD calculator
   */
  virtual void setScene(GlScene &scene) {
    glScene = &scene;
  }

  /**
   * Set input data use to render
   */
  virtual void setInputData(const GlGraphInputData *inputData) {
    this->inputData = inputData;
  }

  /**
   * Set RenderingEntitiesFlag to :
   * RenderingSimpleEntities,RenderingNodes,RenderingEdges,RenderingAll,RenderingWithoutRemove
   */
  virtual void setRenderingEntitiesFlag(RenderingEntitiesFlag flag) {
    renderingEntitiesFlag = flag;
  }

  /**
   * Return if the LODCalculator need to have entities to compute
   */
  virtual bool needEntities() {
    return true;
  }
  /**
   * Set if the LODCalculator need to have entities to compute
   */
  virtual void setNeedEntities(bool) {}
  /**
   * Begin a new camera
   */
  virtual void beginNewCamera(Camera *camera) = 0;
  /**
   * Record a new simple entity in current camera context
   */
  virtual void addSimpleEntityBoundingBox(GlSimpleEntity *entity, const BoundingBox &bb) = 0;
  /**
   * Record a new node in current camera context
   */
  virtual void addNodeBoundingBox(unsigned int id, unsigned int pos, const BoundingBox &bb) = 0;
  /**
   * Record a new edge in current camera context
   */
  virtual void addEdgeBoundingBox(unsigned int id, unsigned int pos, const BoundingBox &bb) = 0;

  /**
   * Compute all lod
   */
  virtual void compute(const Vector<int, 4> &globalViewport,
                       const Vector<int, 4> &currentViewport) = 0;

  /**
   * Return a pointer on LOD result
   */
  LayersLODVector &getResult() {
    return layersLODVector;
  }

  /**
   * Clear class data
   */
  virtual void clear() {
    layersLODVector.clear();
  }

  virtual BoundingBox getSceneBoundingBox() = 0;

  void setAttachedLODCalculator(GlLODCalculator *calculator) {
    attachedLODCalculator = calculator;
  }

protected:
  GlScene *glScene;
  const GlGraphInputData *inputData;

  RenderingEntitiesFlag renderingEntitiesFlag;

  LayersLODVector layersLODVector;

  GlLODCalculator *attachedLODCalculator;
};
} // namespace tlp

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLLODCALCULATOR_H
///@endcond
