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
#include <tulip/MouseBoxZoomer.h>

#include "NodeLinkDiagramComponentInteractor.h"

using namespace tlp;

/** \brief Tulip interactor to do a rectangle zoom
 *
 */
class InteractorRectangleZoom  : public NodeLinkDiagramComponentInteractor {

public:

  /**
   * Default constructor
   */
  InteractorRectangleZoom():NodeLinkDiagramComponentInteractor(":/tulip/qt/icons/i_zoom.png","Zoom on rectangle"){
    setPriority(2);
    setConfigurationWidgetText(QString("<h3>Rectangle zoom interactor</h3>")+
                   "Zoom on selected rectangle.<br><b>Mouse left</b> down indicates the first corner, <b>Mouse left</b> up indicates the opposite corner.");
  }

  /**
   * Construct chain of responsibility
   */
  void construct(){
    pushInteractorComponent(new MousePanNZoomNavigator);
    pushInteractorComponent(new MouseBoxZoomer);
  }

  QCursor getCursor(){return QCursor(Qt::CrossCursor);}
};

INTERACTORPLUGIN(InteractorRectangleZoom, "InteractorRectangleZoom", "Tulip Team", "01/04/2009", "Rectangle Zoom Interactor", "1.0");
