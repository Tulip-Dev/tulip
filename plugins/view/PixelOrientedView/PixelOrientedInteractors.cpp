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

#include "PixelOrientedInteractors.h"
#include "PixelOrientedViewNavigator.h"
#include "PixelOrientedView.h"

#include "../../utils/StandardInteractorPriority.h"
#include "../../utils/ViewNames.h"

using namespace std;

namespace tlp {

PixelOrientedInteractor::PixelOrientedInteractor(const QString &iconPath, const QString &text) : NodeLinkDiagramComponentInteractor(iconPath, text) {}

bool PixelOrientedInteractor::isCompatible(const std::string &viewName) const  {
  return (viewName == ViewName::PixelOrientedViewName);
}

PLUGIN(PixelOrientedInteractorNavigation)

PixelOrientedInteractorNavigation::PixelOrientedInteractorNavigation(const PluginContext *) : PixelOrientedInteractor(":/tulip/gui/icons/i_navigation.png", "Navigate in view") {
  setConfigurationWidgetText(QString("<html><head>")
                             +"<title></title>"
                             +"</head>"
                             +"<body>"
                             +"<h3>View navigation interactor</h3>"
                             +"<p>This interactor allows to navigate in the pixel oriented view.</p>"
                             +"<p>When there is more than one graph properties selected, the corresponding pixel oriented previews are generated and displayed in a matrix form. By <b>double clicking on a pixel oriented preview, "
                             +"this one is displayed in fullscreen </b> in a more detailed way and the others interactors become available. To go back to the pixel oriented previews matrix, double click anywhere in the view.</p>"
                             +"<p>Otherwise, this interactor offers the same functionnalities as the one in the \"Node Link Diagram view\". The commands are described below :</p>"
                             +"<b>Ctrl + Mouse up/down</b> : zoom<br>"
                             +"<b>Ctrl + Mouse left/right</b> : z rotation<br>"
                             +"<b>Shift + Mouse</b> : rotation<br>"
                             +"<b>Key up/down</b> : up/down<br>"
                             +"<b>Key left/right</b> : left/right<br>"
                             +"<b>Key page up/down</b> : zoom<br>"
                             +"<b>Key insert</b> : rotate<br>"
                             +"</body>"
                             +"</html>");
  setPriority(StandardInteractorPriority::Navigation);
}

void PixelOrientedInteractorNavigation::construct() {
  push_back(new PixelOrientedViewNavigator);
  push_back(new MouseNKeysNavigator);
}

}
