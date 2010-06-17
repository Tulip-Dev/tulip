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
#ifndef Tulip_GLSCENE_H
#define Tulip_GLSCENE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include "tulip/GlSceneObserver.h"
#include "tulip/GlLayer.h"
#include "tulip/GlGraphComposite.h"
#include "tulip/GlSelectSceneVisitor.h"
#include "tulip/GlDisplayListManager.h"
#include "tulip/GlyphManager.h"
#include "tulip/GlTextureManager.h"

namespace tlp {

  /** /brief Storage class for Z ordering
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

  /** /brief Tulip scene class
   *
   * Tulip scene class
   */
  class TLP_GL_SCOPE GlScene : public GlObservableScene {

  public:

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
     */
    void computeAjustSceneToSize(int width, int height, Coord *center, Coord *eye, float *sceneRadius, float *xWhiteFactor, float *yWhiteFactor);

    /**
     * Ajust camera to have entities near borders
     */
    void ajustSceneToSize(int width, int height);

    /**
     * Ajust camera for empty scene
     */
    void ajustCameraToEmptyScene();

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
     * Output the scene in SVG
     */
    void outputEPS(unsigned int size,const std::string& filename);

    /**
     * Return the RGB image of OpenGL view
     */
    unsigned char * getImage();

    /**
     * Set the viewport of the scene with a vector
     */
    void setViewport(Vector<int, 4> newViewport) {viewport=newViewport;}

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
      calculator->setScene(this);
    }

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

    Camera* getCamera() {return getLayer("Main")->getCamera();}
    void setCamera(Camera* camera) {getLayer("Main")->setCamera(*camera);}

  private:

    std::vector<std::pair<std::string,GlLayer *> > layersList;
    GlLODCalculator *lodCalculator;
    Vector<int, 4> viewport;
    Color backgroundColor;
    bool viewLabel;
    bool viewOrtho;

    GlGraphComposite *glGraphComposite;
    GlLayer *graphLayer;

  };

}

#endif // Tulip_GLSCENE_H
