/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include <tulip/MouseNodeBuilder.h>
#include <tulip/MouseEdgeBuilder.h>

#include "NodeLinkDiagramComponentInteractor.h"

using namespace tlp;

/** \brief Tulip interactor to add edges
 *
 */
class TLP_QT_SCOPE InteractorAddEdge  : public NodeLinkDiagramComponentInteractor {

public:

  /**
   * Default constructor
   */
  InteractorAddEdge():NodeLinkDiagramComponentInteractor(":/i_addedge.png","Add edges"){
    setPriority(1);
    setConfigurationWidgetText(QString("<h3>Add edge interactor</h3>")+
                   "<b>Mouse left</b> click on the source node,<br/>then <b>Mouse left</b> click on the target node.<br/>Any <b>Mouse left</b> click outside a node will add an edge bend.");
  }

  /**
   * Construct chain of responsibility
   */
  void construct(){
    pushInteractorComponent(new MousePanNZoomNavigator);
    pushInteractorComponent(new MouseNodeBuilder);
    pushInteractorComponent(new MouseEdgeBuilder);
  }
};

INTERACTORPLUGIN(InteractorAddEdge, "InteractorAddEdge", "Tulip Team", "01/04/2009", "Add Edge Interactor", "1.0");
