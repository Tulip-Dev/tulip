//-*-c++-*-
#ifndef MOUSEOBJECT_H
#define MOUSEOBJECT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#if (QT_REL == 3)
#include <qcursor.h>
#else
#include <QtGui/qcursor.h>
#endif

#include "GWInteractor.h"
namespace tlp {

/** \addtogroup Mouse_interactor */ 
/*@{*/
/** An interactor class allowing to zoom and pan using the mouse wheel.
 */
class TLP_QT_SCOPE MousePanNZoomNavigator:public GWInteractor {
public:
  MousePanNZoomNavigator(){}
  ~MousePanNZoomNavigator(){}
  bool eventFilter(QObject *, QEvent *);
  GWInteractor *clone() { return new MousePanNZoomNavigator(); }
};

/** An interactor class used to delete a graph element on mouse left click
 */
class TLP_QT_SCOPE MouseElementDeleter:public GWInteractor {
public:
  MouseElementDeleter(){}
  ~MouseElementDeleter(){}
  bool eventFilter(QObject *, QEvent *);
  GWInteractor *clone() { return new MouseElementDeleter(); }
};
  
/** An interactor class to translate/rotate using keys or mouse movements,
    or to zoom and pan using the mouse wheel or
 */
class TLP_QT_SCOPE MouseNKeysNavigator: public MousePanNZoomNavigator {
private:
  GWInteractor::ID currentMouseID;
  
public:
  MouseNKeysNavigator() : currentMouseID(GWInteractor::invalidID){}
  ~MouseNKeysNavigator(){}
  bool eventFilter(QObject *, QEvent *);
  GWInteractor *clone() { return new MouseNKeysNavigator(); }
};
/*@}*/

}
#endif
