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
#include <tulip/MouseSelector.h>
#include <tulip/MouseEdgeBendEditor.h>
#include <tulip/NodeLinkDiagramComponentInteractor.h>
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/StandardInteractorPriority.h>

#include "../utils/PluginNames.h"

using namespace tlp;

/** \brief Tulip interactor to edit edge bends
 *
 */
class InteractorEditEdgeBends : public NodeLinkDiagramComponentInteractor {

public:
  PLUGININFORMATION("InteractorEditEdgeBends", "Tulip Team", "01/04/2009",
                    "Edit Edge Bends Interactor", "1.0", "Modification")
  /**
   * Default constructor
   */
  InteractorEditEdgeBends(const tlp::PluginContext *)
      : NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_bends.png", "Edit edge bends",
                                           StandardInteractorPriority::EditEdgeBends) {}

  /**
   * Construct chain of responsibility
   */
  void construct() override {
    setConfigurationWidgetText(
        QString("<h3>Edit edge bends</h3>") + "<>Modify edge bends</u><br/><br/>" +
        "Select edge: <ul><li>use rectangle selection</li></ul>" +
        "Translate bend: <ul><li><b>Mouse left</b> down on a selected bend + moves</li></ul>" +
        "Change source node: <ul><li><b>Drag and drop circle</b> on source node</li></ul>" +
        "Change target node: <ul><li><b>Drag and drop triangle</b> on target node</li></ul>" +
        "Add bend: <ul><li><b>Double Mouse left</b> click on the selected "
        "edge</li></ul>" +
#if !defined(__APPLE__)
        "Delete bend: <ul><li><b>Ctrl + Mouse left</b> click on a selected bend</li></ul>"
#else
        "Delete bend: <ul><li><b>⌥ + Mouse left</b> click on a selected bend</li></ul>"
#endif
        + "<u>Navigation in the graph</u><br/><br/>" +
        "Translation: <ul><li><b>Arrow</b> keys</li></ul>" +
#if !defined(__APPLE__)
        "Zoom/Unzoom: <ul><li><b>Mouse wheel</b> up/down</li><li> or <b>Pg up/Pg "
        "down</b> keys</li></ul>"
#else
        "Zoom/Unzoom: <ul><li><b>Mouse wheel</b> down/up</li><li> or <b>Pg up/Pg "
        "down</b> keys</li></ul>"
#endif
    );
    push_back(new MouseNKeysNavigator(false));
    push_back(new MouseSelector(Qt::LeftButton, Qt::NoModifier, MouseSelector::EdgesOnly));
    push_back(new MouseEdgeBendEditor);
  }

  bool isCompatible(const std::string &viewName) const override {
    return (viewName == NodeLinkDiagramComponent::viewName);
  }
};

PLUGIN(InteractorEditEdgeBends)
