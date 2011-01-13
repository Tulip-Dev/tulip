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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cassert>
#include <iostream>
#include "tulip/CameraObserver.h"
#include "tulip/Camera.h"

using namespace std;

namespace tlp {

  ObservableCamera::ObservableCamera():tracked(true), inDestruction(false){
  }

  void ObservableCamera::setCameraTracking(bool tracked){
    this->tracked=tracked;
  }

  void ObservableCamera::notifyDestroy(Camera *camera) {
    inDestruction=true;
    set<CameraObserver *> copy(observers); //Used to preserve iteratros
    for (set<CameraObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
      (*itlObs)->destroy(camera);
  }

  void ObservableCamera::notifyPointOfViewModified(Camera *camera) {
    if(!tracked)
      return;

    set<CameraObserver *> copy(observers); //Used to preserve iteratros
    for (set<CameraObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
      (*itlObs)->pointOfViewModified(camera);
  }
}

