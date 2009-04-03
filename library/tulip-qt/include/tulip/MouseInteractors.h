//-*-c++-*-
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
