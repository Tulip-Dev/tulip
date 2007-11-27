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

#include <tulip/tulipconf.h>

#include "tulip/GlLayer.h"
#include "tulip/GlGraphComposite.h"
#include "tulip/GlSelectSceneVisitor.h"

namespace tlp {

  class TLP_GL_SCOPE GlScene {
  
  public:
     
    GlScene();

    void initGlParametters();

    void draw();
    
    void addLayer(GlLayer *layer) {layersList.push_back(layer);layer->setScene(this);}

    void centerScene();
    void zoomXY(int step, const int x, const int y);
    void zoom(float factor,const Coord& dest);
    void zoom(int step);
    void translateCamera(const int x, const int y, const int z);
    void rotateScene(const int x, const int y, const int z);

    bool selectEntities(SelectionFlag type, int x, int y, int h, int w,std::vector<GlEntity *>& selectedEntities);

    void outputSVG(unsigned int size,const std::string& filename);
    void outputEPS(unsigned int size,const std::string& filename);
    unsigned char * getImage();

    void setViewport(Vector<int, 4> newViewport) {viewport=newViewport;}
    void setViewport(int x, int y, int width, int height) {viewport[0]=x;viewport[1]=y;viewport[2]=width;viewport[3]=height;}
    Vector<int, 4> getViewport() {return viewport;}
    void setBackgroundColor(const Color& color) {backgroundColor=color;}
    Color getBackgroundColor() {return backgroundColor;}
    void setViewOrtho(bool viewOrtho){this->viewOrtho=viewOrtho;}
    bool isViewOrtho() {return viewOrtho;}
    void setViewLabel(bool viewLabel){this->viewLabel=viewLabel;}

    void addGlGraphCompositeInfo(GlLayer* layer,GlGraphComposite *glGraphComposite) {this->graphLayer=layer;this->glGraphComposite=glGraphComposite;}
    GlGraphComposite* getGlGraphComposite() {return glGraphComposite;}
    GlLayer* getGraphLayer() {return graphLayer;}

    GlLayer* getSelectionLayer() {return selectionLayer;}

    Camera* getCamera() {return layersList[0]->getCamera();}
    void setCamera(Camera* camera) {layersList[0]->setCamera(*camera);}
    GlLayer* getLayer() {return layersList[0];}

  private:

    std::vector<GlLayer *> layersList;
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
