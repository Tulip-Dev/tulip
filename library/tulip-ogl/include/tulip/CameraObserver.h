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

#ifndef Tulip_CAMERAOBSERVER_H
#define Tulip_CAMERAOBSERVER_H

#include <set>
#include <string>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

namespace tlp {

  class Camera;

  /** \brief An observer to the camera
   * This class is used to observe camera object
   *  - If the camera is deleted : destroy is call
   *  - If a parameter of camera is changed : pointOfViewModified is call
   * If you want to desactivate/activate the call of pointOfViewModified, you can call the setCameraTracking(bool) function (by default the camera is tracked)
   */
  class TLP_GL_SCOPE CameraObserver {
  public:

    virtual ~CameraObserver() {}
    /**
     * This function is call when we delete the camera (call in camera destructor)
     */
    virtual void destroy(Camera *){}

    /**
     * This function is call when the point of view of camera is modified
     */
    virtual void pointOfViewModified(Camera *){}

  };

  /**
   * Observable camera : internal class (don't use it)
   */
  class TLP_GL_SCOPE ObservableCamera {
  public:
    ObservableCamera();
    virtual ~ObservableCamera() {}
    /**
     * Register a new observer
     */
    void addObserver(CameraObserver *) const;
    /**
     * Returns the number of observers
     */
    unsigned int countObservers();
    /**
     * Remove an observer
     */
    void removeObserver(CameraObserver *) const;
    /**
     * Remove all observers
     */
    void removeObservers();

    void notifyDestroy(Camera *camera);

    void notifyPointOfViewModified(Camera *);

    /**
     * Call this function if you want to activate/desactivate the call of pointOfViewModified
     */
    void setCameraTracking(bool tracked);

    /**
     * return true if the camera is tracked
     */
    bool isTracked() {return tracked;}


  protected:

    bool tracked;

    mutable std::set<CameraObserver*> observers;
  };

  inline void ObservableCamera::addObserver(CameraObserver *obs) const {
    observers.insert(obs);
  }

  inline unsigned int ObservableCamera::countObservers() {
    return observers.size();
  }

  inline void ObservableCamera::removeObserver(CameraObserver *item) const{
    observers.erase(item);
  }

  inline void ObservableCamera::removeObservers() {
    observers.clear();
  }
}

#endif

