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

#include <tulip/GlSceneObserver.h>
#include <tulip/GlLayer.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlSelectSceneVisitor.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlyphManager.h>
#include <tulip/GlTextureManager.h>

namespace tlp {

  /** \brief Storage class for Z ordering
   *
   */
  struct EntityWithDistance {

    EntityWithDistance(const double &dist,EntityLODUnit *entity)
      :distance(dist),entity(entity),isComplexEntity(false){
    }
    EntityWithDistance(const double &dist,ComplexEntityLODUnit *entity,bool isNode)
      :distance(dist),entity(entity),isComplexEntity(true),isNode(isNode){
    }

    double distance;
    EntityLODUnit *entity;
    bool isComplexEntity;
    bool isNode;
  };

  /** \brief Tulip scene class
   *
   * The GlScene class is the core of the tulip rendering system
   * This class is used to render entities and graph in OpenGL
   * If you want to render entities and graph, you have to use GlLayer system. You just have to create GlLayer and add entities in.
   * \warning GlScene work with only one graph, if you add a GlGraphComposite you have to call the addGlGraphCompositeInfo(GlLayer *, GlGraphComposite *)
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
   * glScene.addGlGraphCompositeInfo(mainLayer,graphComposite);
   * glScene.centerScene();
   * glScene.draw();
   * \endcode
   *
   * If you want to create a widget with a visualisation is better to use GlMainWidget class (this class use a GlScene inside)
   */
  class TLP_GL_SCOPE GlScene : public Observable {

  public:
    /** \brief Constructor
     * By default GlScene use a GlQuadTreeLODCalculator to compute LOD but you can change this default lod calculator, to do that : put your calculator in constructor parameters
     */
    GlScene(GlLODCalculator *calculator=NULL);

    ~GlScene();

    /**
     * Init scene's OpenGL parameters
     */
    void initGlParameters();

    /**
     * Prerender meta nodes
     */
    void prerenderMetaNodes();

    /**
     * Draw the scene
     */
    void draw();

    /**
     * Center scene
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
    void computeAjustSceneToSize(int width, int height, Coord *center, Coord *eye, float *sceneRadius, float *xWhiteFactor, float *yWhiteFactor,BoundingBox *sceneBoundingBox=NULL);

    /**
     * Ajust camera to have entities near borders
     */
    void ajustSceneToSize(int width, int height);

    /**
     * Zoom to x,y
     */
    void zoomXY(int step, const int x, const int y);

    /**
     * Zoom
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
     * Rotate camera by (x,y,z)
     */
    void rotateScene(const int x, const int y, const int z);

    /**
     * Select entities with selection flag : type (SelectSimpleEntities,SelectNodes,SelectEdges)
     * Select at position x,y with heigh : h and width : w
     * Select in GlLayer : layer
     * And store result in selectedEntities vector
     */
    bool selectEntities(RenderingEntitiesFlag type, int x, int y, int h, int w,GlLayer *layer,std::vector<unsigned long>& selectedEntities);

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
     */
    void setViewport(Vector<int, 4> &newViewport) {viewport=newViewport;}

    /**
     * Set the viewport of the scene with 4 int
     */
    void setViewport(int x, int y, int width, int height) {viewport[0]=x;viewport[1]=y;viewport[2]=width;viewport[3]=height;}

    /**
     * Get the viewport of the scene
     */
    Vector<int, 4> getViewport() {return viewport;}

    /**
     * Set the background color of the scene
     */
    void setBackgroundColor(const Color& color) {backgroundColor=color;}

    /**
     * Get the background color of the scene
     */
    Color getBackgroundColor() {return backgroundColor;}

    /**
     * Set if scene is render in orthogonal mode
     */
    void setViewOrtho(bool viewOrtho){this->viewOrtho=viewOrtho;}

    /**
     * Scene is render in orthogonal mode ?
     */
    bool isViewOrtho() {return viewOrtho;}

    /**
     * Render the nodes/edges label
     */
    void setViewLabel(bool viewLabel){this->viewLabel=viewLabel;}

    /**
     * Add a layer in the scene
     */
    void addLayer(GlLayer *layer);

    /**
     * Add a layer just before layer with given name
     * Return true if insert is ok and false if layer with given name is not find
     */
    bool insertLayerBefore(GlLayer *layer,const std::string &name);

    /**
     * Add a layer just after layer with given name
     * Return true if insert is ok and false if layer with given name is not find
     */
    bool insertLayerAfter(GlLayer *layer,const std::string &name);

    /**
     * Return the layer with name : name
     */
    GlLayer* getLayer(const std::string& name);

    /**
     * Remove the layer with name and delete it (if bool deleteLayer == true)
     */
    void removeLayer(const std::string& name,bool deleteLayer);

    /**
     * Remove a layer and delete it (if bool deleteLayer == true)
     */
    void removeLayer(GlLayer *layer,bool deleteLayer);

    /**
     * Return the layer list
     */
    std::vector<std::pair<std::string, GlLayer*> >* getLayersList() {return &layersList;}

    /**
     * Clear layers list
     */
    void clearLayersList() { layersList.clear();}

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
     * Return the bouding box of the scene
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

    //************************************************************
    /* To Remove */
    void addGlGraphCompositeInfo(GlLayer* layer,GlGraphComposite *glGraphComposite) {
      this->graphLayer=layer;
      this->glGraphComposite=glGraphComposite;
      if(glGraphComposite)
        lodCalculator->setInputData(glGraphComposite->getInputData());
    }
    GlGraphComposite* getGlGraphComposite() {return glGraphComposite;}
    GlLayer* getGraphLayer() {return graphLayer;}

    Camera& getCamera() {return getLayer("Main")->getCamera();}
    void setCamera(const Camera& camera) {getLayer("Main")->setCamera(camera);}

    /**
     * If true, color buffer will not be cleared before drawing the scene.
     */
    void setNoClearBackground(bool clear) {noClearBackground = clear;}


    bool getNoClearBackground() const {return noClearBackground;}

  private:

    std::vector<std::pair<std::string,GlLayer *> > layersList;
    GlLODCalculator *lodCalculator;
    Vector<int, 4> viewport;
    int viewportZoom;
    int xDecViewport;
    int yDecViewport;
    Color backgroundColor;
    bool viewLabel;
    bool viewOrtho;
    bool displayEdgesInLastRendering;

    GlGraphComposite *glGraphComposite;
    GlLayer *graphLayer;

    bool noClearBackground;

  };

}

#endif // Tulip_GLSCENE_H
