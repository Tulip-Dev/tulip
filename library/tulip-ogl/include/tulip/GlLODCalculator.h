/*
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#include <tulip/BoundingBox.h>

#include <vector>

namespace tlp {

class Camera;
class GlEntity;
class GlSimpleEntity;
class GlScene;
class GlGraphInputData;

enum RenderingEntitiesFlag {
  RenderingSimpleEntities =1,
  RenderingNodes = 2,
  RenderingEdges = 4,
  RenderingAll = 7,
  RenderingWithoutRemove = 8
};

struct EntityLODUnit {
  EntityLODUnit(const BoundingBox &boundingBox):boundingBox(boundingBox),lod(-1) {}
  BoundingBox boundingBox;
  float lod;
};

// struct to store simple entity lod
struct SimpleEntityLODUnit : public EntityLODUnit {
  SimpleEntityLODUnit(GlSimpleEntity *entity, const BoundingBox &boundingBox):
    EntityLODUnit(boundingBox),
    entity(entity) {
  }
  GlSimpleEntity *entity;
};

// struct to store complex entity (nodes/edges) lod
struct ComplexEntityLODUnit : public EntityLODUnit {
  ComplexEntityLODUnit(unsigned int id,const BoundingBox &boundingBox):EntityLODUnit(boundingBox),id(id) {}
  unsigned int id;
};

struct LayerLODUnit {
  std::vector<SimpleEntityLODUnit>  simpleEntitiesLODVector;
  std::vector<ComplexEntityLODUnit> nodesLODVector;
  std::vector<ComplexEntityLODUnit> edgesLODVector;
  Camera * camera;
};

typedef std::vector<LayerLODUnit> LayersLODVector;

/**
 * Class use to calculate lod of scene entities
 */
class TLP_GL_SCOPE GlLODCalculator {

public:

  GlLODCalculator():glScene(NULL),inputData(NULL),attachedLODCalculator(NULL) {}
  virtual ~GlLODCalculator() {}
  virtual GlLODCalculator *clone()=0;

  /**
   * Set scene use by this LOD calculator
   */
  virtual void setScene(GlScene &scene) {
    glScene=&scene;
  }

  /**
   * Set input data use to render
   */
  virtual void setInputData(const GlGraphInputData *inputData) {
    this->inputData=inputData;
  }

  /**
   * Set RenderingEntitiesFlag to : RenderingSimpleEntities,RenderingNodes,RenderingEdges,RenderingAll,RenderingWithoutRemove
   */
  virtual void setRenderingEntitiesFlag(RenderingEntitiesFlag flag) {
    renderingEntitiesFlag=flag;
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
  virtual void beginNewCamera(Camera* camera)=0;
  /**
   * Record a new simple entity in current camera context
   */
  virtual void addSimpleEntityBoundingBox(GlSimpleEntity *entity,const BoundingBox& bb)=0;
  /**
   * Record a new node in current camera context
   */
  virtual void addNodeBoundingBox(unsigned int id,const BoundingBox& bb)=0;
  /**
   * Record a new edge in current camera context
   */
  virtual void addEdgeBoundingBox(unsigned int id,const BoundingBox& bb)=0;

  /**
   * Reserve memory to store nodes LOD
   */
  virtual void reserveMemoryForNodes(unsigned int) {}

  /**
   * Reserve memory to store edges LOD
   */
  virtual void reserveMemoryForEdges(unsigned int) {}

  /**
   * Compute all lod
   */
  virtual void compute(const Vector<int,4>& globalViewport, const Vector<int,4>& currentViewport)=0;

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

  virtual BoundingBox getSceneBoundingBox()=0;

  void setAttachedLODCalculator(GlLODCalculator *calculator) {
    attachedLODCalculator=calculator;
  }

protected :

  GlScene *glScene;
  const GlGraphInputData *inputData;

  RenderingEntitiesFlag renderingEntitiesFlag;

  LayersLODVector layersLODVector;

  GlLODCalculator *attachedLODCalculator;
};

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLLODCALCULATOR_H
///@endcond
