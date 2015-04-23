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
#include <tulip/NodeLinkDiagramComponentInteractor.h>
#include <tulip/NodeLinkDiagramComponent.h>

#include "../utils/StandardInteractorPriority.h"
#include "../utils/ViewNames.h"

using namespace tlp;

/** \brief Tulip interactor to navigate on the graph
 *
 */
class InteractorNavigation  : public NodeLinkDiagramComponentInteractor {

public:
  PLUGININFORMATION("InteractorNavigation", "Tulip Team", "01/04/2009", "Navigation Interactor", "1.0", "Navigation")
  /**
   * Default constructor
   */
  InteractorNavigation(const tlp::PluginContext*):NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_navigation.png","Navigate in graph") {
    setPriority(StandardInteractorPriority::Navigation);
    setConfigurationWidgetText(QString("<h3>Navigate in graph</h3>")+
                               "3D Navigation in the graph<br/><br/>"+
                               "Translation: <ul><li><b>Mouse left</b> down + moves</li><li>or <b>Arrow</b> keys down</li></ul>" +
                               "X or Y rotation: <ul><li><b>Shift + Mouse left</b> down + up/down or left/right moves</li></ul>" +
#if !defined(__APPLE__)
                               "Z rotation: <ul><li><b>Ctrl + Mouse left</b> down + left/right moves</li><li> or <b>Insert</b> key</li></ul>" +
                               "Zoom/Unzoom: <ul><li><b>Ctrl + Mouse left</b> down + up/down moves</li><li> or <b>Pg up/Pg down</b> keys</li></ul>"
#else
                               "Z rotation: <ul><li><b>Alt + Mouse left</b> down + left/right moves</li><li> or <b>Insert</b> key</li></ul>" +
                               "Zoom/Unzoom: <ul><li><b>Alt + Mouse left</b> down + up/down moves</li><li> or <b>Pg up/Pg down</b> keys</li></ul>"
#endif
                               + "Meta node navigation: <ul><li><b>double Mouse left click</b> go inside the metanode</li>"+
                               "<li><b>Ctrl + double Mouse left click</b> go outside the metanode</li></ul>"
                              );
  }

  /**
   * Construct chain of responsibility
   */
  void construct() {
    push_back(new MouseNKeysNavigator);
  }

  QCursor cursor() const {
    return QCursor(Qt::OpenHandCursor);
  }

  bool isCompatible(const std::string &viewName) const {
    return ((viewName==NodeLinkDiagramComponent::viewName)
            ||(viewName==ViewName::MatrixViewName)
            ||(viewName==ViewName::ParallelCoordinatesViewName)
           );
  }

};

PLUGIN(InteractorNavigation)
