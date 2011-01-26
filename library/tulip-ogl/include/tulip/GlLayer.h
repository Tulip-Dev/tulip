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
   * A layer is an entity with a camera and a GlComposite
   * Layers are used with GlScene : you can add a layer to a scene and a scene can have many layers
   * You have two constructor for GlLayer : one with a camera pointer and one without
   *  The constructor without camera pointer create a layer with a new camera and delete this camera at the destruction
   *  The constructor with camera pointer create a layer and use the camera pointer but you have the responsibility of camera destruction
   */
  class TLP_GL_SCOPE GlLayer {

  public:

    /**
     * Layer constructor : construct a layer with his name
     *  A new camera is created for this layer and this camera will be deleted in the GlLayer destructor
     */
    GlLayer(const std::string& name,bool workingLayer=false);

    /**
     * Layer constructor : construct a layer with his name and use the camera : camera
     *  You have the responsibility of camera destruction
     */
    GlLayer(const std::string& name,Camera *camera,bool workingLayer=false);

    /**
     * Destructor
     */
    ~GlLayer();

    /**
     * Set the scene where the layer is
     */
    void setScene(GlScene *scene) {this->scene=scene;camera->setScene(scene);}

    /**
     * Return the scene where the layer is
     */
    GlScene *getScene() {return scene;}

    /**
     * Return the layer's name
     */
    std::string getName() {return name;}

    /**
     * Set the layer's camera
     * GlLayer now use a copy of the camera parameters
     */
    void setCamera(const Camera& camera);

    /**
     * Set the layer's camera
     * GlLayer now use camera parameters and you have the resposibility of camera destruction
     */
    void setSharedCamera(Camera *camera);

    /**
     * Replace the layer's camera with a new 2D one
     */
    void set2DMode();

    /**
     * Return the layer's camera
     */
    Camera &getCamera() {return *camera;}

    /**
     * Set if the layer is visible
     */
    void setVisible(bool visible) {composite.setVisible(visible);}

    /**
     * Return if the layer is visible
     */
    bool isVisible() {return composite.isVisible();}

    /**
     * Add an entity to GlComposite of the layer
     */
    void addGlEntity(GlSimpleEntity *entity,const std::string& name);

    /**
     * Remove entity with name : key
     * This entity is not deleted
     */
    void deleteGlEntity(const std::string &key);

    /**
     * Remove entity
     * This entity is not deleted
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
     * Entities are not deleted
     */
    void clear() {composite.reset(false);}

    /**
     * return if this layer is a working layer
     *  this property doesn't change the layer, but for example working layers are not displayed in LayerManagerWidget
     */
    bool isAWorkingLayer(){return workingLayer;}

    /**
     * return if this layer use a shared camera
     */
    bool useSharedCamera(){return sharedCamera;}

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

    GlComposite composite;
    GlScene *scene;

    Camera *camera;
    bool sharedCamera;

    bool workingLayer;

  };

}

#endif // Tulip_GLLAYER_H
