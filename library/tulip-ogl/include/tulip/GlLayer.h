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
    
    GlLayer(const std::string& name);

    void setScene(GlScene *scene) {this->scene=scene;camera.setScene(scene);}
    GlScene *getScene() {return scene;}
    
    std::string getName() {return name;}

    void setCamera(Camera *camera) {this->camera=*camera;}
    void setCamera(const Camera& camera) {this->camera=camera;} 
    Camera *getCamera() {return &camera;}

    void setVisible(bool visible) {this->visible=visible;}
    bool isVisible() {return visible;}
    void setStencil(bool stencil) {this->stencil=stencil;}
    bool stencilTest() {return stencil;}

    void addGlEntity(GlSimpleEntity *entity,const std::string& name);
    void deleteGlEntity(const std::string &key);
    void deleteGlEntity(GlSimpleEntity *entity);
    GlSimpleEntity* findGlEntity(const std::string &key);
    std::map<std::string, GlSimpleEntity*> *getDisplays();

    GlComposite *getComposite() {return &composite;}

    void acceptVisitor(GlSceneVisitor *visitor);

    void clear() {composite.clear();}

  private:

    std::string name;

    bool visible;
    bool stencil;

    GlComposite composite;
    GlScene *scene;

    Camera camera;

  };
  
}

#endif // Tulip_GLLAYER_H
