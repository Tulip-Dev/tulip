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

#include <vector>

#include <tulip/InteractorViewExplorer.h>

#include "../utils/PluginNames.h"

using namespace tlp;

class InteractorGetInformation : public InteractorViewExplorer {

public:
  /**
   * Default constructor
   */
  InteractorGetInformation(const tlp::PluginContext *) : InteractorViewExplorer() {}

  /**
   * Construct chain of responsibility
   */
  void construct() override {
    InteractorViewExplorer::construct();
    setConfigurationWidgetText(
        QString("<h3>Explore %1</h3>").arg(view()->name().c_str()) +
        "When the mouse cursor looks like <img src=\":/tulip/gui/icons/i_information.png\">, "
        "indicating it is on top of a graph element (node or edge), "
        "<b>Mouse left</b> click to display a panel showing the element properties.<br/>"
        "As the properties panel is displayed, <b>Mouse left</b> click in a property row to edit the "
        "corresponding value.<br/>"
        "The visible properties can be filtered using the list of properties displayed in the "
        "<b>Options</b> tab.<br/>"
        "If none is filtered, when the element properties panel is displayed, the display of the "
        "visual rendering properties can be then toggled using a dedicated check box.<br/><br/>" +
        (view()->name() == NodeLinkDiagramComponent::viewName ? "<u>3D" : "<u>2D") +
        " Navigation in the view</u><br/><br/>" +
        "Translation: <ul><li><b>Mouse left</b> down + moves</li><li>or <b>Arrow</b> keys "
        "</li></ul>" +
        (view()->name() == NodeLinkDiagramComponent::viewName
             ? "X or Y rotation: <ul><li><b>Shift + Mouse left</b> down + up/down or left/right "
               "moves</li></ul>"
             : "") +
#if !defined(__APPLE__)
        (view()->name() == NodeLinkDiagramComponent::viewName
             ? "Z rotation: <ul><li><b>Ctrl + Mouse left</b> down + left/right moves</li><li> or <b>Insert</b> key</li></ul>"
             : "") +
        "Zoom/Unzoom: <ul><li><b>Mouse wheel</b> up/down</li><li> or <b>Ctrl + Mouse left</b> down " +
#else
        (view()->name() == NodeLinkDiagramComponent::viewName
             ? "Z rotation: <ul><li><b>⌥ + Mouse left</b> down + left/right moves</li><li> or <b>Insert</b> key</li></ul>"
             : "") +
        "Zoom/Unzoom: <ul><li><b>Mouse wheel</b> down/up</li><li> or <b>⌥ + Mouse left</b> down "
#endif
        "+ up/down moves</li><li> or <b>Pg up/Pg down</b> keys</li></ul>" +
        (view()->name() == NodeLinkDiagramComponent::viewName
             ? (QString(
                    "Meta node navigation: <ul><li><b>double Mouse left</b> click go inside the metanode<br/>(press Shift to avoid element properties display)</li>") +
#if !defined(__APPLE__)
                "<li><b>Ctrl + Double Mouse left</b> click go outside the metanode</li></ul>"
#else
                "<li><b>⌥ or ⌘ + Double Mouse left</b> click go outside the metanode</li></ul>"
#endif
                )
             : ""));
  }

  bool isCompatible(const std::string &viewName) const override {
    return ((viewName == ViewName::MatrixViewName) ||
            (viewName == NodeLinkDiagramComponent::viewName));
  }
};

PLUGIN(InteractorGetInformation)
