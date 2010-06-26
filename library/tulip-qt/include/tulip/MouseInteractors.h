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
#ifndef MOUSEOBJECT_H
#define MOUSEOBJECT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <QtGui/qcursor.h>

#include "tulip/InteractorComponent.h"

namespace tlp {

/** \addtogroup Mouse_interactor */
/*@{*/
/** An interactor class allowing to zoom and pan using the mouse wheel.
 */
class TLP_QT_SCOPE MousePanNZoomNavigator:public InteractorComponent {
public:
  MousePanNZoomNavigator(){}
  ~MousePanNZoomNavigator(){}
  bool eventFilter(QObject *, QEvent *);
  InteractorComponent *clone() { return new MousePanNZoomNavigator(); }
};

/** An interactor class used to delete a graph element on mouse left click
 */
class TLP_QT_SCOPE MouseElementDeleter:public InteractorComponent {
public:
  MouseElementDeleter(){}
  ~MouseElementDeleter(){}
  bool eventFilter(QObject *, QEvent *);
  InteractorComponent *clone() { return new MouseElementDeleter(); }
};

/** An interactor class to translate/rotate using keys or mouse movements,
    or to zoom and pan using the mouse wheel or
 */
class TLP_QT_SCOPE MouseNKeysNavigator: public MousePanNZoomNavigator {
private:
  InteractorComponent *currentSpecInteractorComponent;

public:
  MouseNKeysNavigator() : currentSpecInteractorComponent(NULL){}
  ~MouseNKeysNavigator(){}
  bool eventFilter(QObject *, QEvent *);
  InteractorComponent *clone() { return new MouseNKeysNavigator(); }
};
/*@}*/

}
#endif
