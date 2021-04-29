/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include "SOMViewInteractor.h"
#include <tulip/MouseInteractors.h>
#include <tulip/MouseSelector.h>
#include <tulip/MouseShowElementInfo.h>
#include "EditColorScaleInteractor.h"
#include "ThresholdInteractor.h"

#include "../../utils/StandardInteractorPriority.h"
#include "../../utils/PluginNames.h"

using namespace std;
using namespace tlp;

PLUGIN(SOMViewNavigation)
PLUGIN(SOMViewSelection)
PLUGIN(SOMViewProperties)
PLUGIN(SOMViewThreshold)

SOMViewInteractor::SOMViewInteractor(const QString &iconPath, const QString &text,
                                     const unsigned int priority)
    : NodeLinkDiagramComponentInteractor(iconPath, text, priority) {}

bool SOMViewInteractor::isCompatible(const std::string &viewName) const {
  return (viewName == ViewName::SOMViewName);
}

SOMViewNavigation::SOMViewNavigation(PluginContext *)
    : SOMViewInteractor(":/tulip/gui/icons/i_navigation.png", "Navigate",
                        StandardInteractorPriority::Navigation) {}

void SOMViewNavigation::construct() {
  push_back(new MouseNKeysNavigator());
  push_back(new EditColorScaleInteractor());
}

SOMViewSelection::SOMViewSelection(PluginContext *)
    : SOMViewInteractor(":/tulip/gui/icons/i_selection.png", "Select",
                        StandardInteractorPriority::RectangleSelection) {}

void SOMViewSelection::construct() {
  push_back(new MouseSelector());
  push_back(new MousePanNZoomNavigator());
  push_back(new EditColorScaleInteractor());
}

SOMViewProperties::SOMViewProperties(PluginContext *)
    : SOMViewInteractor(":/tulip/gui/icons/i_select.png", "Properties",
                        StandardInteractorPriority::GetInformation) {}

void SOMViewProperties::construct() {
  push_back(new MouseShowElementInfo());
  push_back(new MousePanNZoomNavigator());
  push_back(new EditColorScaleInteractor());
}

SOMViewThreshold::SOMViewThreshold(PluginContext *)
    : SOMViewInteractor(":/i_slider.png", "Threshold Selection",
                        StandardInteractorPriority::ViewInteractor1) {}

void SOMViewThreshold::construct() {
  setConfigurationWidgetText(QString(
      "<H1>Threshold Interactor</H1><p>This interactor is used to select nodes with a value "
      "between those indicated by the two sliders</p><p>Move the each slider to change the "
      "bound.</p><p>Press the Ctrl button to add the new threshold selection to the current "
      "selection. If Ctrl is not pressed the old selection will be replaced</p>"));
  push_back(new MouseNKeysNavigator());
  push_back(new ThresholdInteractor());
}
