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
#include <tulip/MouseSelector.h>
#include <tulip/MouseEdgeBendEditor.h>
#include <tulip/NodeLinkDiagramComponentInteractor.h>
#include <tulip/NodeLinkDiagramComponent.h>

#include "../utils/StandardInteractorPriority.h"
#include "../utils/ViewNames.h"

using namespace tlp;

/** \brief Tulip interactor to edit edge bends
 *
 */
class InteractorEditEdgeBends  : public NodeLinkDiagramComponentInteractor {

public:
  PLUGININFORMATION("InteractorEditEdgeBends", "Tulip Team", "01/04/2009", "Edit Edge Bends Interactor", "1.0", "Modification")
  /**
   * Default constructor
   */
  InteractorEditEdgeBends(const tlp::PluginContext*):NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_bends.png","Edit edge bends") {
    setPriority(StandardInteractorPriority::EditEdgeBends);
    setConfigurationWidgetText(QString("<h3>Edit edge bends</h3>")+
                               "Modify edge bends<br/><br/>"+
                               "Select edge: <ul><li>use rectangle selection</li></ul>" +
                               "Translate bend: <ul><li><b>Mouse left</b> down on a selected bend + moves</li></ul>" +
                               "Change source node: <ul><li><b>Drag and drop circle on source node</li></ul>"+
                               "Change target node: <ul><li><b>Drag and drop triangle on target node</li></ul>"+
                               "Add bend: <ul><li><b>Double click with mouse left</b> click on the selected edge</li></ul>"+
#if !defined(__APPLE__)
                               "Delete bend: <ul><li><b>Ctrl + Mouse left</b> click on a selected bend</li></ul>"
#else
                               "Delete bend: <ul><li><b>Alt + Mouse left</b> click on a selected bend</li></ul>"
#endif
                              );
  }

  /**
   * Construct chain of responsibility
   */
  void construct() {
    push_back(new MousePanNZoomNavigator);
    push_back(new MouseSelector);
    push_back(new MouseEdgeBendEditor);
  }

  bool isCompatible(const std::string &viewName) const {
    return ((viewName==NodeLinkDiagramComponent::viewName)
            ||(viewName==ViewName::GoogleMapsViewName)
           );
  }

};

PLUGIN(InteractorEditEdgeBends)
