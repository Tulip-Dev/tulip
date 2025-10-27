/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <tulip/StandardInteractorPriority.h>

#include "../utils/PluginNames.h"

using namespace tlp;

/** \brief Tulip interactor to add edges
 *
 */
class InteractorAddEdge : public NodeLinkDiagramComponentInteractor {

public:
  PLUGININFORMATION("InteractorAddEdge", "Tulip Team", "01/04/2009", "Add nodes/edges Interactor",
                    "1.0", "Modification")
  /**
   * Default constructor
   */
  InteractorAddEdge(const tlp::PluginContext *)
      : NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_addedge.png", "Add nodes/edges",
                                           StandardInteractorPriority::AddNodesOrEdges) {}

  /**
   * Construct chain of responsibility
   */
  void construct() override {
    setConfigurationWidgetText(
        "<h3>Add nodes/edges</h3>Add a node: <ul><li><b>Mouse left</b> click "
        "outside any node</li></ul>Add an edge: <ul><li><b>Mouse left</b> click on "
        "the source node,<br/>then <b>Mouse left</b> click on the target "
        "node.<br/>Any <b>Mouse left</b> click outside a node before the "
        "click on the target node will add an edge bend,<br/><b>Mouse "
        "middle</b> click will cancel the current edge construction.</li></ul>"
        "<u>Navigation in the graph</u><br/><br/>"
        "Translation: <ul><li><b>Arrow</b> keys</li></ul>"
#if !defined(__APPLE__)
        "Zoom/Unzoom: <ul><li><b>Mouse wheel</b> up/down</li><li> or <b>Pg up/Pg "
        "down</b> keys</li></ul>"
#else
        "Zoom/Unzoom: <ul><li><b>Mouse wheel</b> down/up</li><li> or <b>Pg up/Pg "
        "down</b> keys</li></ul>"
#endif
    );
    push_back(new MouseNKeysNavigator(false));
    push_back(new MouseNodeBuilder);
    push_back(new MouseEdgeBuilder);
  }

  QCursor cursor() const override {
    return QCursor(Qt::PointingHandCursor);
  }

  bool isCompatible(const std::string &viewName) const override {
    return (viewName == NodeLinkDiagramComponent::viewName);
  }
};

PLUGIN(InteractorAddEdge)
