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
#ifndef Tulip_GLSCENE_H
#define Tulip_GLSCENE_H

#include <tulip/tulipconf.h>

#include <tulip/Graph.h>
#include <tulip/GlLODCalculator.h>
#include <tulip/GlSceneObserver.h>
#include <tulip/GlLayer.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTextureManager.h>

namespace tlp {

/** \brief Structure to store selected entities
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

protected :

  GlSimpleEntity *simpleEntity;
  unsigned int complexEntityId;
  SelectedEntityType entityType;
  Graph *complexEntityGraph;
};


/** \brief Tulip scene class
 *
 * The GlScene class is the core of the tulip rendering system
 * This class is used to render entities and graph in OpenGL
 * If you want to render entities and graph, you have to use GlLayer system. You just have to create GlLayer and add entities in.
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
   * By default GlScene use a GlCPULODCalculator to compute LOD but you can change this default lod calculator, to do that : put your calculator in constructor parameters
   * Available calculators are : GlCPULODCalculator and GlQuadTreeLODCalculator
   */
  GlScene(GlLODCalculator *calculator=NULL);

  ~GlScene();

  /**
   * Init scene's OpenGL parameters, this function is call when you do a draw
   */
  void initGlParameters();

  /**
   * Draw the scene, this function is the most important function of GlScene. If you want to render a scene into an OpenGL widget : call this function
   */
  void draw();

  /**
   * Center scene to have all the visibles entities displayed
   */
  void centerScene();

  /**
   * Compute informations for ajustSceneToSize
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
   */
  void ajustSceneToSize(int width, int height);

  /**
   * Zoom to given x,y screen coordinates
   */
  void zoomXY(int step, const int x, const int y);

  /**
   * Zoom to given world coordinate
   * \warning factor parameter isn't be used
   */
  void zoom(float factor,const Coord& dest);

  /**
   * Zoom
   */
  void zoom(int step);

  /**
   * Translate camera by (x,y,z)
   */
  void translateCamera(const int x, const int y, const int z);

  /**
   * Rotate camera by (x,y,z) with :
   *  x : rotation over X axis in degree
   *  y : rotation over Y axis in degree
   *  z : rotation over Z axis in degree
   */
  void rotateScene(const int x, const int y, const int z);

  /**
   * Select entities with selection flag : type (SelectSimpleEntities,SelectNodes,SelectEdges)
   * Select at position x,y with heigh : h and width : w
   * Select in GlLayer : layer
   * And store result in selectedEntities vector
   */
  bool selectEntities(RenderingEntitiesFlag type, int x, int y, int h, int w,GlLayer *layer,std::vector<SelectedEntity>& selectedEntities);

  /**
   * Output the scene in SVG
   */
  void outputSVG(unsigned int size,const std::string& filename);

  /**
   * Output the scene in EPS
   */
  void outputEPS(unsigned int size,const std::string& filename);

  /**
   * Return the RGB image of OpenGL view
   */
  unsigned char * getImage();

  /**
   * Set the viewport of the scene with a vector
   * The viewport must be in many case the size of the widget containing the scene
   */
  void setViewport(Vector<int, 4> &newViewport) {
    viewport=newViewport;
  }

  /**
   * Set the viewport of the scene with 4 int
   * The viewport must be in many case the size of the widget containing the scene
   */
  void setViewport(int x, int y, int width, int height) {
    viewport[0]=x;
    viewport[1]=y;
    viewport[2]=width;
    viewport[3]=height;
  }

  /**
   * Get the viewport of the scene
   * The viewport must be in many case the size of the widget containing the scene
   */
  Vector<int, 4> getViewport() const {
    return viewport;
  }

  /**
   * Set the background color of the scene
   */
  void setBackgroundColor(const Color& color) {
    backgroundColor=color;
  }

  /**
   * Get the background color of the scene
   */
  Color getBackgroundColor() const {
    return backgroundColor;
  }

  /**
   * Set if scene is render in orthogonal mode
   */
  void setViewOrtho(bool viewOrtho) {
    this->viewOrtho=viewOrtho;
  }

  /**
   * Scene is render in orthogonal mode ?
   */
  bool isViewOrtho() {
    return viewOrtho;
  }

  /**
   * Create a layer with the given name in the scene
   * Now the scene have the ownership of this GlLayer
   * so you don't have to delete this GlLayer
   */
  GlLayer *createLayer(const std::string &name);

  /**
   * Create a layer with the given name in the scene just before layer with given name
   * Return NULL if the layer with beforeLayerWithName is not find
   * Now the scene have the ownership of this GlLayer
   * so you don't have to delete this GlLayer
   */
  GlLayer *createLayerBefore(const std::string &layerName,const std::string &beforeLayerWithName);

  /**
   * Create a layer with the given name in the scene just after layer with given name
   * Return NULL if the layer with beforeLayerWithName is not find
   * Now the scene have the ownership of this GlLayer
   * so you don't have to delete this GlLayer
   */
  GlLayer *createLayerAfter(const std::string &layerName,const std::string &afterLayerWithName);

  /**
   * Add an existing layer in the scene
   * Now the scene have the ownership of this GlLayer
   * so you don't have to delete this GlLayer
   */
  void addExistingLayer(GlLayer *layer);

  /**
   * Add an existing layer in the scene just before layer with given name
   * Return false if the layer with beforeLayerWithName is not find
   * Now the scene have the ownership of this GlLayer
   * so you don't have to delete this GlLayer
   */
  bool addExistingLayerBefore(GlLayer *layer, const std::string &beforeLayerWithName);

  /**
   * Add an existing layer in the scene just after layer with given name
   * Return false if the layer with beforeLayerWithName is not find
   * Now the scene have the ownership of this GlLayer
   * so you don't have to delete this GlLayer
   */
  bool addExistingLayerAfter(GlLayer *layer, const std::string &afterLayerWithName);

  /**
   * Return the layer with name : name
   */
  GlLayer *getLayer(const std::string& name);

  /**
   * Remove the layer with name
   * This GlLayer is automaticaly delete
   * If you want to keep this GlLayer you can put false to deleteLayer parameters
   * but after that you have the ownership of the GlLayer
   */
  void removeLayer(const std::string& name,bool deleteLayer=true);

  /**
   * Remove the layer with name
   * This GlLayer is automaticaly delete
   * If you want to keep this GlLayer you can put false to deleteLayer parameters
   * but after that you have the ownership of the GlLayer
   */
  void removeLayer(GlLayer *layer,bool deleteLayer=true);

  /**
   * Return the layer list
   */
  std::vector<std::pair<std::string, GlLayer*> >* getLayersList() {
    return &layersList;
  }

  /**
   * Clear layers list
   * Layers will not be deleted in this function
   */
  void clearLayersList() {
    layersList.clear();
  }

  /**
   * This function is called by GlLayer and GlComposite to send layer modification event
   */
  void notifyModifyLayer(const std::string &name,GlLayer *layer);

  /**
   * This function is called by GlComposite to send entity modification event
   */
  void notifyModifyEntity(GlSimpleEntity *entity);

  /**
   * Get XML description of the scene and children and store it in out string
   */
  void getXML(std::string &out);

  void getXMLOnlyForCameras(std::string &out);

  /**
   * Set scene's data and children with a XML
   */
  void setWithXML(std::string &in,Graph *graph);

  /**
   * Return lod calculator used to render this scene
   */
  GlLODCalculator *getCalculator() {
    return lodCalculator;
  }

  /**
   * Set a new lod calculator used to render this scene
   */
  void setCalculator(GlLODCalculator *calculator) {
    lodCalculator=calculator;
    calculator->setScene(*this);
  }

  /**
   * Return the bouding box of the scene (in 3D coordinates)
   * \warning This bounding box is compute in rendering, so if you add an entity in a layer the bounding box include this entity if a draw is call
   */
  BoundingBox getBoundingBox();

  /**
   * Set a zoom level and a xDec/yDec, this parameters change viewport of the scene
   */
  void setViewportZoom(int zoom=1,int xDec=0, int yDec=0);

  /**
   * Get a zoom level and a xDec/yDec, this parameters change viewport of the scene
   */
  void getViewportZoom(int &zoom, int &xDec, int &yDec);

  /**
   * This function is automaticaly call when a GlGraphComposite is added in a layer in the scene
   * You don't have to call this function
   */
  void glGraphCompositeAdded(GlLayer *layer,GlGraphComposite *composite);

  /**
   * This function is automaticaly call when a GlGraphComposite is added in a layer in the scene
   * You don't have to call this function
   */
  void glGraphCompositeRemoved(GlLayer *layer,GlGraphComposite *composite);

  /**
   * Return the current GlGraphComposite used by the scene
   */
  GlGraphComposite* getGlGraphComposite() {
    return glGraphComposite;
  }

  /**
   * Return the layer containing the current GlGraphComposite
   */
  GlLayer* getGraphLayer() {
    return graphLayer;
  }

  /**
   * At default the most important layer is the layer where the graph is visualized
   * This function return the camera of this layer
   */
  Camera& getGraphCamera() {
    assert(graphLayer!=NULL);
    return graphLayer->getCamera();
  }

  /**
   * At default the most important layer is the layer where the graph is visualized
   * This function set the camera of this layer
   */
  void setGraphCamera(Camera* camera) {
    assert(graphLayer!=NULL);
    graphLayer->setCamera(camera);
  }

  /**
   * If false, color buffer will not be cleared before drawing the scene.
   */
  void setClearBufferAtDraw(bool clear) {
    clearBufferAtDraw = clear;
  }


  bool getClearBufferAtDraw() const {
    return clearBufferAtDraw;
  }

private:

  std::vector<std::pair<std::string,GlLayer *> > layersList;
  GlLODCalculator *lodCalculator;
  Vector<int, 4> viewport;
  int viewportZoom;
  int xDecViewport;
  int yDecViewport;
  Color backgroundColor;
  bool viewOrtho;

  GlGraphComposite *glGraphComposite;
  GlLayer *graphLayer;

  bool clearBufferAtDraw;

};

}

#endif // Tulip_GLSCENE_H
