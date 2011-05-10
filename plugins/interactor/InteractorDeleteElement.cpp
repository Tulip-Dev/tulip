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

/** \brief Tulip interactor to delete an element
 *
 */
class InteractorDeleteElement  : public NodeLinkDiagramComponentInteractor {

public:

  /**
   * Default constructor
   */
  InteractorDeleteElement():NodeLinkDiagramComponentInteractor(":/tulip/qt/icons/i_del.png","Delete nodes or edges"){
    setPriority(1);
    setConfigurationWidgetText(QString("<h3>Delete interactor</h3>")+
                   "<b>Mouse left</b> click on an element to delete it");
  }

  /**
   * Construct chain of responsibility
   */
  void construct(){
    pushInteractorComponent(new MousePanNZoomNavigator);
    pushInteractorComponent(new MouseElementDeleter);
  }
};

INTERACTORPLUGIN(InteractorDeleteElement, "InteractorDeleteElement", "Tulip Team", "01/04/2009", "Delete Element Interactor", "1.0");
