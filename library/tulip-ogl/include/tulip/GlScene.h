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

#ifndef Tulip_GLSCENE_H
#define Tulip_GLSCENE_H

#include <tulip/tulipconf.h>
#include <tulip/GlLODCalculator.h>
#include <tulip/GlLayer.h>
#include <tulip/Color.h>
#include <tulip/Observable.h>

namespace tlp {

class GlSimpleEntity;
class Graph;
class GlLODCalculator;
class GlGraphComposite;

/**
 * @ingroup OpenGL
 * @brief Structure to store selected entities
 *
 * After a selection, objects of SelectedEntity is returned
 * To use this object the first thing to do is to call getEntity type to know the type of Entity
 * After that you can :
 *   - Get the GlSimpleEnity pointer (getSimpleEntity())
 *   - Get the id of node/edge and the graph associated (getComplexEntityId() and getComplexEntityGraph())
 *
 */
struct SelectedEntity {

  enum SelectedEntityType {
    UNKNOW_SELECTED = 0,
    NODE_SELECTED = 1,
    EDGE_SELECTED = 2,
    SIMPLE_ENTITY_SELECTED = 3
  };

  SelectedEntity():simpleEntity(NULL),complexEntityId((unsigned int)(-1)),entityType(UNKNOW_SELECTED),complexEntityGraph(NULL) {}
  SelectedEntity(GlSimpleEntity *entity):simpleEntity(entity),complexEntityId((unsigned int)(-1)),entityType(SIMPLE_ENTITY_SELECTED),complexEntityGraph(NULL) {}
  SelectedEntity(Graph *graph,unsigned int id,SelectedEntityType type):simpleEntity(NULL),complexEntityId(id),entityType(type),complexEntityGraph(graph) {}

  GlSimpleEntity *getSimpleEntity() const {
    assert(simpleEntity!=NULL);
    return simpleEntity;
  }

  unsigned int getComplexEntityId() const {
    assert(complexEntityId!=(unsigned int)(-1));
    return complexEntityId;
  }

  Graph *getComplexEntityGraph() const {
    assert(complexEntityGraph!=NULL);
    return complexEntityGraph;
  }

  SelectedEntityType getEntityType() const {
    return entityType;
  }
  /**
   * @brief getNode is a convenience method to perform the check on the selected element type and return the corresponding node object. It's equivalent to
   * @code
   * if(getComplexEntityType()==NODE_SELECTED){
   *    return node(getComplexEntityId())
   * }
   * @endcode
   * @return the selected node if the entity type is correct or an invalid node else.
   */
  node getNode()const {
    if(entityType == NODE_SELECTED) {
      return node(complexEntityId);
    }
    else {
      return node();
    }
  }

  /**
   * @brief getEdge is a convenience method to perform the check on the selected element type and return the corresponding edge object. It's equivalent to
   * @code
   * if(getComplexEntityType()==EDGE_SELECTED){
   *    return edge(getComplexEntityId())
   * }
   * @endcode
   * @return the selected edge if the entity type is correct or an invalid edge else.
   */
  edge getEdge()const {
    if(entityType == EDGE_SELECTED) {
      return edge(complexEntityId);
    }
    else {
      return edge();
    }
  }

protected :

  GlSimpleEntity *simpleEntity;
  unsigned int complexEntityId;
  SelectedEntityType entityType;
  Graph *complexEntityGraph;
};


/**
 * @ingroup OpenGL
 * @brief Tulip scene class
 *
 * The GlScene class is the core of the tulip rendering system
 * This class is used to render entities and graph in OpenGL
 *
 * If you want to render entities and graph, you have to use GlLayer system. You just have to create GlLayer and add GlEntity in.
 * If you create more than one GlLayer, layers are rendered one after one, so the first GlLayer added is rendered in first.
 * @see GlLayer
 * @see GlSimpleEntity
 *
 *
 * After adding layers you can do a centerScene() and a draw()
 *
 * \code
 * GlLayer *mainLayer=new GlLayer("Main");
 * GlGraphComposite *graphComposite=new GlGraphComposite(graph);
 * mainLayer->addGlEntity(graphComposite,"graph");
 * GlLayer *otherLayer=new GlLayer("Other");
 * GlCircle *circle=new GlCircle();
 * otherLayer->addGlEntity(circle,"circle");
 * glScene.addLayer(mainLayer);
 * glScene.addLayer(otherLayer);
 * glScene.centerScene();
 * glScene.draw();
 * \endcode
 *
 * If you want to create a widget with a visualisation is better to use GlMainWidget class (this class use a GlScene inside)
 */
class TLP_GL_SCOPE GlScene : public Observable {

public:
  /** \brief Constructor
   * Default scene is empty
   * @param calculator By default GlScene use a GlCPULODCalculator to compute LOD but you can change this default lod calculator, to do that : put your calculator in constructor parameters
   * Available calculators are : GlCPULODCalculator and GlQuadTreeLODCalculator
   */
  GlScene(GlLODCalculator *calculator=NULL);

  ~GlScene();

  /**
   * @brief Init scene's OpenGL parameters.
   * You don't have to call this function, it is call when you do a draw
   */
  void initGlParameters();

  /**
   * @brief Draw the scene
   * This function is the most important function of GlScene. If you want to render a scene into an OpenGL widget : call this function
   */
  void draw();

  /**
   * Center scene
   * After this function all entities are visible on the screen
   */
  void centerScene();

  /**
   * Compute information for ajustSceneToSize
   * \param width : request width
   * \param height : request height
   * \param center : the result center will be stored in (if center != NULL)
   * \param eye : the result eye will be stored in (if eye != NULL)
   * \param sceneRadius : the result sceneRadius will be stored in (if sceneRadius != NULL)
   * \param xWhiteFactor : xWhiteFactor is the white part on x borders (left and right), the result xWhiteFactor will be stored in (if xWhiteFactor != NULL)
   * \param yWhiteFactor : yWhiteFactor is the white part on y borders (top and bottom), the result yWhiteFactor will be stored in (if yWhiteFactor != NULL)
   * \param sceneBoundingBox : the result sceneBoundingBox will be stored in (if sceneBoundingBox != NULL)
   */
  void computeAjustSceneToSize(int width, int height, Coord *center, Coord *eye, float *sceneRadius, float *xWhiteFactor, float *yWhiteFactor,BoundingBox *sceneBoundingBox=NULL,float *zoomFactor=NULL);

  /**
   * Ajust camera to have entities near borders
   * @param width requested width
   * @param height requested height
   */
  void ajustSceneToSize(int width, int height);

  /**
   * @brief Zoom by step to given x,y screen coordinates
   * @param step of zoom
   */
  void zoomXY(int step, const int x, const int y);

  /**
   * @brief Zoom to given world coordinate
   * \warning factor parameter isn't be used
   */
  void zoom(float factor,const Coord& dest);

  /**
   * @brief Zoom by step
   * @param step of zoom
   */
  void zoom(int step);

  /**
   * @brief Zoom by factor
   * @param factor of zoom
   */
  void zoomFactor(float factor);

  /**
   * @brief Translate camera by (x,y,z)
   */
  void translateCamera(const int x, const int y, const int z);

  /**
   * @brief Rotate camera by (x,y,z)
   * @param x rotation over X axis in degree
   * @param y rotation over Y axis in degree
   * @param z rotation over Z axis in degree
   */
  void rotateScene(const int x, const int y, const int z);

  /**
   * @brief Select entities in scene
   * @param type Entities type to select (SelectSimpleEntities,SelectNodes,SelectEdges)
   * @param x screen coordinates
   * @param y screen coordinates
   * @param h height in screen coordinates
   * @param w width in screen coordinates
   * @param layer where the selection will be performed
   * @param selectedEntities the result of the selection is stored on it
   */
  bool selectEntities(RenderingEntitiesFlag type, int x, int y, int h, int w,GlLayer *layer,std::vector<SelectedEntity>& selectedEntities);

  /**
   * @brief Output the scene in SVG
   */
  void outputSVG(unsigned int size,const std::string& filename);

  /**
   * @brief Output the scene in EPS
   */
  void outputEPS(unsigned int size,const std::string& filename);

  /**
   * @brief Return the RGB image of GlScene
   */
  unsigned char * getImage();

  /**
   * @brief Set the viewport of the scene with a vector
   * The viewport must be in many case the size of the widget containing the scene
   */
  void setViewport(Vector<int, 4> &newViewport) {
    viewport=newViewport;
  }

  /**
   * @brief Set the viewport of the scene with 4 int
   * The viewport must be in many case the size of the widget containing the scene
   */
  void setViewport(int x, int y, int width, int height) {
    viewport[0]=x;
    viewport[1]=y;
    viewport[2]=width;
    viewport[3]=height;
  }

  /**
   * @brief Get the viewport of the scene
   * The viewport will be in many case the size of the widget containing the scene
   */
  Vector<int, 4> getViewport() const {
    return viewport;
  }

  /**
   * @brief Set the background color of the scene
   */
  void setBackgroundColor(const Color& color) {
    backgroundColor=color;
  }

  /**
   * @brief Get the background color of the scene
   */
  Color getBackgroundColor() const {
    return backgroundColor;
  }

  /**
   * @brief Set if scene is render in orthogonal mode
   */
  void setViewOrtho(bool viewOrtho) {
    this->viewOrtho=viewOrtho;
  }

  /**
   * @brief Scene is render in orthogonal mode ?
   */
  bool isViewOrtho() {
    return viewOrtho;
  }

  /**
   * @brief Create a layer with the given name in the scene
   * This layer is added to the layers list
   * Now the scene have the ownership of this GlLayer
   * so you don't have to delete this GlLayer
   */
  GlLayer *createLayer(const std::string &name);

  /**
   * @brief Create a layer with the given name in the scene just before layer with given name
   * This layer is added to the layers list
   * Return NULL if the layer with beforeLayerWithName is not find
   * Now the scene have the ownership of this GlLayer
   * so you don't have to delete this GlLayer
   */
  GlLayer *createLayerBefore(const std::string &layerName,const std::string &beforeLayerWithName);

  /**
   * @brief Create a layer with the given name in the scene just after layer with given name
   * This layer is added to the layers list
   * Return NULL if the layer with beforeLayerWithName is not find
   * Now the scene have the ownership of this GlLayer
   * so you don't have to delete this GlLayer
   */
  GlLayer *createLayerAfter(const std::string &layerName,const std::string &afterLayerWithName);

  /**
   * @brief Add an existing layer in the scene
   * Now the scene have the ownership of this GlLayer
   * so you don't have to delete this GlLayer
   */
  void addExistingLayer(GlLayer *layer);

  /**
   * @brief Add an existing layer in the scene just before layer with given name
   * Return false if the layer with beforeLayerWithName is not find
   * Now the scene have the ownership of this GlLayer
   * so you don't have to delete this GlLayer
   */
  bool addExistingLayerBefore(GlLayer *layer, const std::string &beforeLayerWithName);

  /**
   * @brief Add an existing layer in the scene just after layer with given name
   * Return false if the layer with beforeLayerWithName is not find
   * Now the scene have the ownership of this GlLayer
   * so you don't have to delete this GlLayer
   */
  bool addExistingLayerAfter(GlLayer *layer, const std::string &afterLayerWithName);

  /**
   * @brief Return the layer with name : name
   * Return NULL if the layer doesn't exist in the scene
   */
  GlLayer *getLayer(const std::string& name);

  /**
   * @brief Remove the layer with name
   * This GlLayer is automaticaly delete
   * If you want to keep this GlLayer you can put false to deleteLayer parameters
   * but after that you have the ownership of the GlLayer
   */
  void removeLayer(const std::string& name,bool deleteLayer=true);

  /**
   * @brief Remove the layer with name
   * This GlLayer is automaticaly delete
   * If you want to keep this GlLayer you can put false to deleteLayer parameters
   * but after that you have the ownership of the GlLayer
   */
  void removeLayer(GlLayer *layer,bool deleteLayer=true);

  /**
   * @brief Return the layer list
   */
  const std::vector<std::pair<std::string, GlLayer*> > &getLayersList() {
    return layersList;
  }

  /**
   * @brief Clear layers list
   * Layers will not be deleted in this function
   */
  void clearLayersList() {
    for(std::vector<std::pair<std::string,GlLayer*> >::iterator it=layersList.begin(); it!=layersList.end(); ++it)
      delete it->second;

    layersList.clear();
  }

  /**
   * @brief Get XML description of the scene and children and store it in out string
   */
  void getXML(std::string &out);

  /**
   * @brief Get XML description of cameras of the scene and store it in out string
   */
  void getXMLOnlyForCameras(std::string &out);

  /**
   * @brief Set scene's data and children with a XML
   */
  void setWithXML(std::string &in,Graph *graph);

  /**
   * @brief Return lod calculator used to render this scene
   */
  GlLODCalculator *getCalculator() {
    return lodCalculator;
  }

  /**
   * @brief Set a new lod calculator used to render this scene
   */
  void setCalculator(GlLODCalculator *calculator) {
    lodCalculator=calculator;
    calculator->setScene(*this);
  }

  /**
   * @brief Return the bouding box of the scene (in 3D coordinates)
   * \warning This bounding box is compute in rendering, so if you add an entity in a layer the bounding box include this entity if a draw is call
   */
  BoundingBox getBoundingBox();

  /**
   * @brief Return the current GlGraphComposite used by the scene
   */
  GlGraphComposite* getGlGraphComposite() {
    return glGraphComposite;
  }

  /**
   * @brief Return the layer containing the current GlGraphComposite
   */
  GlLayer* getGraphLayer() {
    return graphLayer;
  }

  /**
   * @brief By default the most important layer is the layer where the graph is visualized
   * This function return the camera of this layer
   */
  Camera& getGraphCamera() {
    assert(graphLayer!=NULL);
    return graphLayer->getCamera();
  }

  /**
   * @brief By default the most important layer is the layer where the graph is visualized
   * This function set the camera of this layer
   */
  void setGraphCamera(Camera* camera) {
    assert(graphLayer!=NULL);
    graphLayer->setCamera(camera);
  }

  /**
   * @brief Set if OpenGL buffer will be cleared at draw
   */
  void setClearBufferAtDraw(bool clear) {
    clearBufferAtDraw = clear;
  }

  /**
   * @brief If false, color buffer will not be cleared before drawing the scene.
   */
  bool getClearBufferAtDraw() const {
    return clearBufferAtDraw;
  }

  /**
   * @brief Set if OpenGL depth buffer will be cleared at draw
   */
  void setClearDepthBufferAtDraw(bool clear) {
    clearDepthBufferAtDraw = clear;
  }

  /**
   * @brief If false, depth buffer will not be cleared before drawing the scene.
   */
  bool getClearDepthBufferAtDraw() const {
    return clearDepthBufferAtDraw;
  }

  /**
   * @brief Set if OpenGL stencil buffer will be cleared at draw
   */
  void setClearStencilBufferAtDraw(bool clear) {
    clearStencilBufferAtDraw = clear;
  }

  /**
   * @brief If false, color buffer will not be cleared before drawing the scene.
   */
  bool getClearStencilBufferAtDraw() const {
    return clearStencilBufferAtDraw;
  }

private:

  std::vector<std::pair<std::string,GlLayer *> > layersList;
  GlLODCalculator *lodCalculator;
  Vector<int, 4> viewport;
  Color backgroundColor;
  bool viewOrtho;

  GlGraphComposite *glGraphComposite;
  GlLayer *graphLayer;

  bool clearBufferAtDraw;

  bool inDraw;

  bool clearDepthBufferAtDraw;

  bool clearStencilBufferAtDraw;

public:

  ///@cond DOXYGEN_HIDDEN

  /**
   * @brief You don't have to call this function
   * This function is automaticaly call when a GlGraphComposite is added in a layer in the scene
   * You don't have to call this function
   */
  void glGraphCompositeAdded(GlLayer *layer,GlGraphComposite *composite);

  /**
   * @brief You don't have to call this function
   * This function is automaticaly call when a GlGraphComposite is added in a layer in the scene
   * You don't have to call this function
   */
  void glGraphCompositeRemoved(GlLayer *layer,GlGraphComposite *composite);

  /**
   * @brief You don't have to call this function
   * This function is called by GlLayer and GlComposite to send layer modification event
   */
  void notifyModifyLayer(const std::string &name,GlLayer *layer);

  /**
   * @brief You don't have to call these functions
   * They are called by GlComposite to send entity modification event
   */
  void notifyModifyEntity(GlSimpleEntity *entity);
  void notifyDeletedEntity(GlSimpleEntity *entity);

  ///@endcond

};

}

#endif // Tulip_GLSCENE_H
