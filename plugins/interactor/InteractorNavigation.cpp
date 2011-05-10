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

#include <tulip/MouseInteractors.h>

#include "NodeLinkDiagramComponentInteractor.h"

using namespace tlp;

/** \brief Tulip interactor to navigate on the graph
 *
 */
class InteractorNavigation  : public NodeLinkDiagramComponentInteractor {

public:

  /**
   * Default constructor
   */
  InteractorNavigation():NodeLinkDiagramComponentInteractor(":/tulip/qt/icons/i_navigation.png","Navigate in graph"){
    setPriority(5);
    setConfigurationWidgetText(QString("<h3>Navigation interactor</h3>")+
                               "3D Navigation in the graph<br><br>"+
                               "Translation: <ul><li><b>Mouse left</b> down + moves</li><li>or <b>Arrow</b> keys down</li></ul>" +
                               "X or Y rotation: <ul><li><b>Shift + Mouse left</b> down + up/down or left/right moves</li></ul>" +
#if !defined(__APPLE__)
                               "Z rotation: <ul><li><b>Ctrl + Mouse left</b> down + left/right moves</li><li> or <b>Insert</b> key</li></ul>" +
                               "Zoom/Unzoom: <ul><li><b>Ctrl + Mouse left</b> down + up/down moves</li><li> or <b>Pg up/Pg down</b> keys</li></ul>"
#else
                               "Z rotation: <ul><li><b>Alt + Mouse left</b> down + left/right moves</li><li> or <b>Insert</b> key</li></ul>" +
                               "Zoom/Unzoom: <ul><li><b>Alt + Mouse left</b> down + up/down moves</li><li> or <b>Pg up/Pg down</b> keys</li></ul>"
#endif
                               );
  }

  /**
   * Construct chain of responsibility
   */
  void construct(){
    pushInteractorComponent(new MouseNKeysNavigator);
  }

  QCursor getCursor(){return QCursor(Qt::OpenHandCursor);}

};

INTERACTORPLUGIN(InteractorNavigation, "InteractorNavigation", "Tulip Team", "01/04/2009", "Navigation Interactor", "1.0");
