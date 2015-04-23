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

#include "ScatterPlot2DInteractors.h"
#include "ScatterPlot2DViewNavigator.h"
#include "ScatterPlotTrendLine.h"
#include "ScatterPlotCorrelCoeffSelector.h"
#include "ScatterPlot2DView.h"
#include "ScatterPlotCorrelCoeffSelectorOptionsWidget.h"

#include "../../utils/StandardInteractorPriority.h"
#include "../../utils/ViewNames.h"

using namespace std;

namespace tlp {

ScatterPlot2DInteractor::ScatterPlot2DInteractor(const QString &iconPath, const QString &text) : NodeLinkDiagramComponentInteractor(iconPath, text) {}

bool ScatterPlot2DInteractor::isCompatible(const std::string &viewName) const {
  return (viewName == ViewName::ScatterPlot2DViewName);
}

PLUGIN(ScatterPlot2DInteractorNavigation)
PLUGIN(ScatterPlot2DInteractorTrendLine)
PLUGIN(ScatterPlot2DInteractorCorrelCoeffSelector)

ScatterPlot2DInteractorNavigation::ScatterPlot2DInteractorNavigation(const tlp::PluginContext *) : ScatterPlot2DInteractor(":/tulip/gui/icons/i_navigation.png", "Navigate in view") {
  setConfigurationWidgetText(QString("<html><head>")
                             +"<title></title>"
                             +"</head>"
                             +"<body>"
                             +"<h3>View navigation interactor</h3>"
                             +"<p>This interactor allows to navigate in the scatter plot view.</p>"
                             +"<p>When there is more than one graph properties selected, the corresponding scatter plots previews are generated and displayed in a matrix form. By <b>double clicking on a scatter plot, "
                             +"this one is displayed in fullscreen </b> in a more detailed way and the others interactors become available. To go back to the scatter plots matrix, double click anywhere in the view.</p>"
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

void ScatterPlot2DInteractorNavigation::construct() {
  push_back(new ScatterPlot2DViewNavigator);
  push_back(new MouseNKeysNavigator);
}

ScatterPlot2DInteractorTrendLine::ScatterPlot2DInteractorTrendLine(const PluginContext *) : ScatterPlot2DInteractor(":/i_scatter_trendline.png", "Trend line") {
  setPriority(StandardInteractorPriority::ViewInteractor1);
}

void ScatterPlot2DInteractorTrendLine::construct() {
  push_back(new ScatterPlotTrendLine);
  push_back(new MousePanNZoomNavigator);
}

ScatterPlot2DInteractorCorrelCoeffSelector::ScatterPlot2DInteractorCorrelCoeffSelector(const tlp::PluginContext *) : ScatterPlot2DInteractor(":/tulip/gui/icons/i_magic.png", "Correlation Coefficient Selector"),optionsWidget(NULL) {
  setPriority(StandardInteractorPriority::ViewInteractor2);
}

void ScatterPlot2DInteractorCorrelCoeffSelector::construct() {
  optionsWidget = new ScatterPlotCorrelCoeffSelectorOptionsWidget();
  push_back(new ScatterPlotCorrelCoeffSelector(optionsWidget));
  push_back(new MousePanNZoomNavigator);
}

ScatterPlot2DInteractorCorrelCoeffSelector::~ScatterPlot2DInteractorCorrelCoeffSelector() {
  delete optionsWidget;
}

QWidget *ScatterPlot2DInteractorCorrelCoeffSelector::configurationWidget() const {
  return optionsWidget;
}

}
