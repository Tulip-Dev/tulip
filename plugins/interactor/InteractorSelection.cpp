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
#include <tulip/MouseSelector.h>
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/StandardInteractorPriority.h>

#include "../utils/PluginNames.h"

using namespace tlp;

/** \brief Tulip interactor to select par of the graph
 *
 */
class InteractorSelection : public NodeLinkDiagramComponentInteractor {

public:
  PLUGININFORMATION("InteractorSelection", "Tulip Team", "01/04/2009", "Selection Interactor",
                    "1.0", "Modification")
  /**
   * Default constructor
   */
  InteractorSelection(const tlp::PluginContext *)
      : NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_selection.png",
                                           "Select nodes/edges in a rectangle",
                                           StandardInteractorPriority::RectangleSelection) {}

  /**
   * Construct chain of responsibility
   */
  void construct() override {
    setConfigurationWidgetText(
        QString("<h3>Selection nodes/edges in a rectangle</h3>") +
        "<u>Draw selection rectangle</u><br/><b>Mouse left</b> down indicates the first corner, "
        "<b>Mouse "
        "left</b> up indicates the opposite corner,<br/>all graph elements instersecting the "
        "rectangle are selected<br/><br/>" +
#if !defined(__APPLE__)
        "Add to selection: <ul><li><b>Ctrl + Mouse left</b> down, drag and up</li></ul>" +
#else
        "Add to selection: <ul><li><b>⌥ + Mouse left</b> down, drag and up</li></ul>" +
#endif
        "Remove from selection: <ul><li><b>Shift + Mouse left</b> down, drag and up</li></ul>" +
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
    push_back(new MouseSelector);
  }

  QCursor cursor() const override {
    return Qt::CrossCursor;
  }

  bool isCompatible(const std::string &viewName) const override {
    return ((viewName == NodeLinkDiagramComponent::viewName) ||
            (viewName == ViewName::HistogramViewName) || (viewName == ViewName::MatrixViewName) ||
            (viewName == ViewName::PixelOrientedViewName) ||
            (viewName == ViewName::ScatterPlot2DViewName));
  }
};

PLUGIN(InteractorSelection)
