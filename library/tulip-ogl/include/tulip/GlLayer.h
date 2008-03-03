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
#ifndef Tulip_GLLAYER_H
#define Tulip_GLLAYER_H

#include <tulip/tulipconf.h>

#include <tulip/Matrix.h>

#include "tulip/GlEntity.h"
#include "tulip/Camera.h"
#include "tulip/GlComposite.h"

namespace tlp {

  class GlScene;
  class GlSceneVisitor;
  
  /**
   * Layer class
   */
  class TLP_GL_SCOPE GlLayer {
  
  public:
    
    /**
     * Layer constructor : construct a layer with his name
     */
    GlLayer(const std::string& name);

    /**
     * Set the scene where the layer is
     */
    void setScene(GlScene *scene) {this->scene=scene;camera.setScene(scene);}
    
    /**
     * Return the scene where the layer is
     */
    GlScene *getScene() {return scene;}
    
    /**
     * Return the layer's name
     */
    std::string getName() {return name;}

    /**
     * Set the layer's camera with a pointer to a camera
     */
    void setCamera(Camera *camera) {this->camera=*camera;}
    
    /**
     * Set the layer's camera
     */
    void setCamera(const Camera& camera) {this->camera=camera;} 
    
    /**
     * Replace the layer's camera with a new 2D one
     */
    void set2DMode() {this->camera=Camera(NULL,false);}
    
    /**
     * Return the layer's camera
     */
    Camera *getCamera() {return &camera;}

    /**
     * Set if the layer is visible
     */
    void setVisible(bool visible) {this->visible=visible;}
    
    /**
     * Return if the layer is visible
     */
    bool isVisible() {return visible;}
    
    /**
     * Add an entity to GlComposite of the layer
     */
    void addGlEntity(GlSimpleEntity *entity,const std::string& name);
    
    /**
     * Delete entity with name : key
     */
    void deleteGlEntity(const std::string &key);
    
    /**
     * Delete entity 
     */
    void deleteGlEntity(GlSimpleEntity *entity);
    
    /**
     * Return entity with name : key
     */
    GlSimpleEntity* findGlEntity(const std::string &key);
    
    /**
     * Return the map of layer's entities
     */
    std::map<std::string, GlSimpleEntity*> *getDisplays();

    /**
     * function used by visitors to visit this layer
     */
    void acceptVisitor(GlSceneVisitor *visitor);

    /**
     * Return the GlComposite used by the layer
     */
    GlComposite *getComposite() {return &composite;}

    /**
     * Remove all entities of the layer
     */
    void clear() {composite.reset(false);}

    /**
     * Return the layer's data in XML
     */
    void getXML(xmlNodePtr rootNode);

    /**
     * Set the layer's data with XML
     */
    void setWithXML(xmlNodePtr rootNode);

  private:

    std::string name;

    bool visible;

    GlComposite composite;
    GlScene *scene;

    Camera camera;

  };
  
}

#endif // Tulip_GLLAYER_H
