//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 23/10/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
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

  /** /brief Tulip scene class
   * 
   * Tulip scene class
   */
  class TLP_GL_SCOPE GlScene : public GlObservableScene {
  
  public:
     
    GlScene();

    /**
     * Init scene's OpenGL parameters
     */
    void initGlParameters();

    /**
     * Draw the scene
     */
    void draw();

    /**
     * Center scene
     */
    void centerScene();

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
    bool selectEntities(SelectionFlag type, int x, int y, int h, int w,GlLayer *layer,std::vector<GlEntity *>& selectedEntities);

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
     * Return the layer with name : name
     */
    GlLayer* getLayer(const std::string& name);
    
    /**
     * Return the layer list
     */
    std::vector<std::pair<std::string, GlLayer*> >* getLayersList() {return &layersList;}

    /**
     * Get XML description of the scene and children and store it in out string
     */
    void getXML(std::string &out);

    /**
     * Set scene's data and children with a XML 
     */
    void setWithXML(std::string &in,Graph *graph);

    //************************************************************
    /* To Remove */
    void addGlGraphCompositeInfo(GlLayer* layer,GlGraphComposite *glGraphComposite) {this->graphLayer=layer;this->glGraphComposite=glGraphComposite;}
    GlGraphComposite* getGlGraphComposite() {return glGraphComposite;}
    GlLayer* getGraphLayer() {return graphLayer;}

    GlLayer* getSelectionLayer() {return selectionLayer;}

    Camera* getCamera() {return getLayer("Main")->getCamera();}
    void setCamera(Camera* camera) {getLayer("Main")->setCamera(*camera);}

  private:

    std::vector<std::pair<std::string,GlLayer *> > layersList;
    GlLayer* selectionLayer;
    Vector<int, 4> viewport;
    Color backgroundColor;
    bool viewLabel;
    bool viewOrtho;

    GlGraphComposite *glGraphComposite;
    GlLayer *graphLayer;
    
  };
  
}

#endif // Tulip_GLSCENE_H
