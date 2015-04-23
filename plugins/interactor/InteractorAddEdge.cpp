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

#include <tulip/MouseInteractors.h>
#include <tulip/MouseNodeBuilder.h>
#include <tulip/MouseEdgeBuilder.h>
#include <tulip/NodeLinkDiagramComponentInteractor.h>
#include <tulip/NodeLinkDiagramComponent.h>

#include "../utils/StandardInteractorPriority.h"
#include "../utils/ViewNames.h"

using namespace tlp;

/** \brief Tulip interactor to add edges
 *
 */
class InteractorAddEdge  : public NodeLinkDiagramComponentInteractor {

public:
  PLUGININFORMATION("InteractorAddEdge", "Tulip Team", "01/04/2009", "Add nodes/edges Interactor", "1.0", "Modification")
  /**
   * Default constructor
   */
  InteractorAddEdge(const tlp::PluginContext*):NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_addedge.png","Add nodes/edges") {
    setPriority(StandardInteractorPriority::AddNodesOrEdges);
    setConfigurationWidgetText("<h3>Add nodes/edges</h3>To add a node: <b>Mouse left</b> click outside any node.<br/>To add an edge: <b>Mouse left</b> click on the source node,<br/>then <b>Mouse left</b> click on the target node.<br/>Any <b>Mouse left</b> click outside a node before the click on the target node will add an edge bend,<br/><b>Mouse middle</b> click will cancel the current edge construction.");
  }

  /**
   * Construct chain of responsibility
   */
  void construct() {
    push_back(new MousePanNZoomNavigator);
    push_back(new MouseNodeBuilder);
    push_back(new MouseEdgeBuilder);
  }

  QCursor cursor() const {
    return QCursor(Qt::PointingHandCursor);
  }

  bool isCompatible(const std::string &viewName) const {
    return ((viewName==NodeLinkDiagramComponent::viewName)
            ||(viewName==ViewName::GoogleMapsViewName));
  }

};

PLUGIN(InteractorAddEdge)
