/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#ifndef GOOGLEMAPSVIEWINTERACTORS_H_
#define GOOGLEMAPSVIEWINTERACTORS_H_

#include <tulip/GLInteractor.h>
#include <tulip/MouseInteractors.h>
#include "GoogleMapsView.h"

using namespace tlp;

class GoogleMapViewInteractor : public GLInteractorComposite {

public :

  GoogleMapViewInteractor(const QString &iconPath, const QString &text);

  bool isCompatible(const std::string &viewName) const;

};

class GoogleMapViewNavigator : public MouseNKeysNavigator {

public :

  GoogleMapViewNavigator();
  ~GoogleMapViewNavigator();

  bool eventFilter(QObject *, QEvent *);
  bool draw(GlMainWidget *) {
    return false;
  }

  bool compute(GlMainWidget *) {
    return false;
  }
  void viewChanged(View *);

protected :

  int x,y;
  bool inRotation;

};

class GoogleMapViewInteractorNavigation : public GoogleMapViewInteractor {

public :

  PLUGININFORMATION("InteractorNavigationGMV", "Tulip Team", "01/04/2009", " GMV Navigation Interactor", "1.0","Navigation")

  GoogleMapViewInteractorNavigation(const PluginContext *);

  void construct();

  QWidget *configurationWidget() const;
  virtual unsigned int priority() const;
};

#endif /* GOOGLEMAPSVIEWINTERACTORS_H_ */
