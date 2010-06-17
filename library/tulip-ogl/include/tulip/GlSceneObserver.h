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
  class GlSimpleEntity;

  /** \brief An observer to the scene
   * An observer to the scene who observe layers
   */
  class TLP_GL_SCOPE GlSceneObserver {
  public:
    virtual ~GlSceneObserver() {}
    /**
     * This function is call when we add a new layer
     */
    virtual void addLayer(GlScene*, const std::string&, GlLayer*){}
    /**
     * This function is call when we remove a layer
     */
    virtual void delLayer(GlScene*, const std::string&, GlLayer*){}
    /**
     * This function is call when we add an entity to the layer
     */
    virtual void modifyLayer(GlScene*, const std::string&, GlLayer*){}
    /**
     * This function is call when we modify an entity
     */
    virtual void modifyEntity(GlScene *,GlSimpleEntity *){}
  };

  /**
   * Observable scene
   */
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
    void notifyModifyEntity(GlScene *,GlSimpleEntity *entity);

  protected:

    mutable std::set<GlSceneObserver*> observers;
  };

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

