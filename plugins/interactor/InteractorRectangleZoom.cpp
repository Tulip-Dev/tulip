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
#include <tulip/NodeLinkDiagramComponentInteractor.h>
#include <tulip/MouseBoxZoomer.h>
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/StandardInteractorPriority.h>

#include "../utils/PluginNames.h"

using namespace tlp;

/** \brief Tulip interactor to do a rectangle zoom
 *
 */
class InteractorRectangleZoom : public NodeLinkDiagramComponentInteractor {

public:
  PLUGININFORMATION("InteractorRectangleZoom", "Tulip Team", "01/04/2009",
                    "Rectangle Zoom Interactor", "1.0", "Navigation")
  /**
   * Default constructor
   */
  InteractorRectangleZoom(const tlp::PluginContext *)
      : NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_zoom.png", "Zoom on rectangle",
                                           StandardInteractorPriority::ZoomOnRectangle) {}

  /**
   * Construct chain of responsibility
   */
  void construct() override {
    setConfigurationWidgetText(
        QString("<h3>Zoom on rectangle</h3>") +
        "<u>Zoom on selected rectangle</u><br/><br/><b>Mouse left</b> down indicates "
        "the first corner.<br/> <b>Mouse left</b> up indicates the opposite "
        "corner.<br/> <b>Double Mouse left</b> click to center the view.<br/><br/>" +
        "<u>Navigation in the graph</u><br/><br/>" +
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
    push_back(new MouseBoxZoomer);
  }

  QCursor cursor() const override {
    return QCursor(Qt::CrossCursor);
  }

  bool isCompatible(const std::string &viewName) const override {
    return ((viewName == NodeLinkDiagramComponent::viewName) ||
            (viewName == ViewName::HistogramViewName) || (viewName == ViewName::MatrixViewName) ||
            (viewName == ViewName::ParallelCoordinatesViewName) ||
            (viewName == ViewName::PixelOrientedViewName) ||
            (viewName == ViewName::ScatterPlot2DViewName));
  }
};

PLUGIN(InteractorRectangleZoom)
