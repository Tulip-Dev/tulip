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

#ifndef Tulip_GLSCENEOBSERVER_H
#define Tulip_GLSCENEOBSERVER_H

#include <set>
#include <string>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

namespace tlp {

  class GlLayer;
  class GlScene;

  class TLP_GL_SCOPE GlSceneObserver { 
  public:
    virtual ~GlSceneObserver() {}
    virtual void addLayer(GlScene*, const std::string&, GlLayer*){}
    virtual void delLayer(GlScene*, const std::string&, GlLayer*){}
    virtual void modifyLayer(GlScene*, const std::string&, GlLayer*){}
  };
  
  class TLP_GL_SCOPE GlObservableScene {
  public:
    virtual ~GlObservableScene() {}
    /**
     * Register a new observer
     */
    void addObserver(GlSceneObserver *) const;
    /**
     * Returns the number of observers
     */
    unsigned int countObservers();
    /**
     * Remove an observer
     */
    void removeObserver(GlSceneObserver *) const;
    /**
     * Remove all observers
     */
    void removeObservers();
    
    void notifyAddLayer(GlScene *scene,const std::string& name, GlLayer* layer);
    void notifyDelLayer(GlScene *scene,const std::string& name, GlLayer* layer);
    void notifyModifyLayer(GlScene *scene,const std::string& name, GlLayer* layer);

  protected:

    mutable std::set<GlSceneObserver*> observers;
  };
  /*@}*/
  
  inline void GlObservableScene::addObserver(GlSceneObserver *obs) const {
    observers.insert(obs); 
  }
  
  inline unsigned int GlObservableScene::countObservers() { 
    return observers.size(); 
  }
  
  inline void GlObservableScene::removeObserver(GlSceneObserver *item) const{  
    observers.erase(item);
  }
  
  inline void GlObservableScene::removeObservers() { 
    observers.clear(); 
  }
}

#endif

