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

#include "PixelOrientedInteractors.h"
#include "PixelOrientedViewNavigator.h"
#include "PixelOrientedView.h"
#include <tulip/StandardInteractorPriority.h>

#include "../../utils/PluginNames.h"

using namespace std;

namespace tlp {

bool PixelOrientedInteractorNavigation::isCompatible(const std::string &viewName) const {
  return (viewName == ViewName::PixelOrientedViewName);
}

PLUGIN(PixelOrientedInteractorNavigation)

PixelOrientedInteractorNavigation::PixelOrientedInteractorNavigation(const PluginContext *)
    : InteractorViewExplorer(
          QString("<p>This interactor allows to navigate in the pixel oriented view.</p>") +
              "<p>When there is more than one graph properties selected, the corresponding pixel oriented "
              "previews are generated and displayed in a matrix form. By <b>double clicking on a pixel oriented preview, "
              "this one is displayed in fullscreen </b> in a more detailed way and the others interactors "
              "become available. To go back to the pixel oriented previews matrix, double click anywhere "
              "in the view.</p>" +
              "When the mouse cursor looks like <img src=\":/tulip/gui/icons/i_information.png\">, "
              "indicating it is on top of a graph element (node or edge), "
              "<b>Mouse left</b> click to display a panel showing the element properties.<br/>"
              "As the properties panel is displayed, <b>Mouse left</b> click in a property row to edit the "
              "corresponding value.<br/>"
              "The visible properties can be filtered using the list of properties displayed in the "
              "<b>Options</b> tab.<br/>"
              "If none is filtered, when the element properties panel is displayed, the display of the "
              "visual rendering properties can be then toggled using a dedicated check box." +
              "<p>Otherwise, this interactor offers the same functionalities as the one in the \"Node "
              "Link Diagram view\".<br/>The commands are described below:</p>" +
              "Translation: <ul><li><b>Mouse left</b> down + moves</li><li>or <b>Arrow</b> keys </li></ul>"
#if !defined(__APPLE__)
              "Zoom/Unzoom: <ul><li><b>Mouse wheel</b> up/down</li><li> or <b>Ctrl + Mouse left</b> down "
              "+ up/down moves</li><li> or <b>Pg up/Pg down</b> keys</li></ul>"
#else
              "Zoom/Unzoom: <ul><li><b>Mouse wheel</b> down/up</li><li> or <b>⌥ + Mouse left</b> down "
              "+ up/down moves</li><li> or <b>Pg up/Pg down</b> keys</li></ul>"
#endif
          ,
          new PixelOrientedViewNavigator) {
}
} // namespace tlp
