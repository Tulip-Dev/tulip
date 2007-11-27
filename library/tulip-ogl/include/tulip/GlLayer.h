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
  
  class TLP_GL_SCOPE GlLayer {
  
  public:
    
    GlLayer();

    void setScene(GlScene *scene) {this->scene=scene;camera.setScene(scene);}
    GlScene *getScene() {return scene;}
    
    void setCamera(Camera *camera) {this->camera=*camera;}
    void setCamera(const Camera& camera) {this->camera=camera;} 
    Camera *getCamera() {return &camera;}

    void setVisible(bool visible) {this->visible=visible;}
    bool isVisible() {return visible;}

    void addGlEntity(GlEntity *entity,const std::string& name) {composite.addGlEntity(entity,name);}
    void deleteGlEntity(const std::string &key) {composite.deleteGlEntity(key);}
    void deleteGlEntity(GlEntity *entity) {composite.deleteGlEntity(entity);}
    GlEntity* findGlEntity(const std::string &key) {return composite.findGlEntity(key);}

    void acceptVisitor(GlSceneVisitor *visitor);

    void clear() {composite.clear();}

  private:

    bool visible;

    GlComposite composite;
    GlScene *scene;

    Camera camera;

  };
  
}

#endif // Tulip_GLLAYER_H
