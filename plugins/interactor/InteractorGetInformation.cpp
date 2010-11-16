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
#include <tulip/MouseShowElementInfos.h>

#include "NodeLinkDiagramComponentInteractor.h"

using namespace tlp;

/** \brief Tulip interactor to get information about an element of the graph
 *
 */
class InteractorGetInformation  : public NodeLinkDiagramComponentInteractor {

public:

  /**
   * Default constructor
   */
  InteractorGetInformation():NodeLinkDiagramComponentInteractor(":/i_select.png","Get information on nodes/edges"){
    setPriority(4);
    setConfigurationWidgetText(QString("<h3>Get information interactor</h3>")+
                   "<b>Mouse left</b> click on an element to display its properties");
  }

  /**
   * Construct chain of responsibility
   */
  void construct(){
    pushInteractorComponent(new MousePanNZoomNavigator);
    pushInteractorComponent(new MouseShowElementInfos);
  }
};

INTERACTORPLUGIN(InteractorGetInformation, "InteractorGetInformation", "Tulip Team", "01/04/2009", "Get Information Interactor", "1.0");
