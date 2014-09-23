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
#include <tulip/MouseBoxZoomer.h>
#include <tulip/NodeLinkDiagramComponent.h>

#include "../utils/StandardInteractorPriority.h"
#include "../utils/ViewNames.h"

using namespace tlp;

/** \brief Tulip interactor to do a rectangle zoom
 *
 */
class InteractorRectangleZoom  : public NodeLinkDiagramComponentInteractor {

public:
  PLUGININFORMATION("InteractorRectangleZoom", "Tulip Team", "01/04/2009", "Rectangle Zoom Interactor", "1.0", "Navigation")
  /**
   * Default constructor
   */
  InteractorRectangleZoom(const tlp::PluginContext*):NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_zoom.png","Zoom on rectangle") {
    setPriority(StandardInteractorPriority::ZoomOnRectangle);
    setConfigurationWidgetText(QString("<h3>Zoom on rectangle</h3>")+
                               "Zoom on selected rectangle.<br><b>Mouse left</b> down indicates the first corner, <b>Mouse left</b> up indicates the opposite corner.");
  }

  /**
   * Construct chain of responsibility
   */
  void construct() {
    push_back(new MousePanNZoomNavigator);
    push_back(new MouseBoxZoomer);
  }

  QCursor cursor() const {
    return QCursor(Qt::CrossCursor);
  }

  bool isCompatible(const std::string &viewName) const {
    return ((viewName==NodeLinkDiagramComponent::viewName)
            ||(viewName==ViewName::HistogramViewName)
            ||(viewName==ViewName::MatrixViewName)
            ||(viewName==ViewName::ParallelCoordinatesViewName)
            ||(viewName==ViewName::PixelOrientedViewName)
            ||(viewName==ViewName::ScatterPlot2DViewName)
           );
  }


};

PLUGIN(InteractorRectangleZoom)
