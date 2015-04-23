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
#include <tulip/MouseSelector.h>
#include <tulip/NodeLinkDiagramComponent.h>

#include "../utils/StandardInteractorPriority.h"
#include "../utils/ViewNames.h"

using namespace tlp;

/** \brief Tulip interactor to select par of the graph
 *
 */
class InteractorSelection  : public NodeLinkDiagramComponentInteractor {

public:
  PLUGININFORMATION("InteractorSelection", "Tulip Team", "01/04/2009", "Selection Interactor", "1.0", "Modification")
  /**
   * Default constructor
   */
  InteractorSelection(const tlp::PluginContext*):NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_selection.png","Select nodes/edges in a rectangle") {
    setPriority(StandardInteractorPriority::RectangleSelection);
    setConfigurationWidgetText(QString("<h3>Selection nodes/edges in a rectangle</h3>")+
                               "Draw selection rectangle.<br/><b>Mouse left</b> down indicates the first corner, <b>Mouse left</b> up indicates the opposite corner,<br/>all graph elements instersecting the rectangle are selected<br/><br/>"+
#if !defined(__APPLE__)
                               "Add to selection: <ul><li><b>Ctrl + Mouse left</b> click on an element</li></ul>" +
#else
                               "Add/Remove from selection: <ul><li><b>Alt + Mouse left</b> click</li></ul>"+
#endif
                               "Remove from selection: <ul><li><b>Shift + Mouse</b> click</li></ul>");
  }

  /**
   * Construct chain of responsibility
   */
  void construct() {
    push_back(new MousePanNZoomNavigator);
    push_back(new MouseSelector);
  }

  QCursor cursor() const {
    return Qt::CrossCursor;
  }

  bool isCompatible(const std::string &viewName) const {
    return ((viewName==NodeLinkDiagramComponent::viewName)
            ||(viewName==ViewName::GoogleMapsViewName)
            ||(viewName==ViewName::HistogramViewName)
            // at this time the selection is not properly managed
            // in the MatrixView so we disable it
            ||(viewName==ViewName::MatrixViewName)
            ||(viewName==ViewName::PixelOrientedViewName)
            ||(viewName==ViewName::ScatterPlot2DViewName)
           );
  }

};

PLUGIN(InteractorSelection)
