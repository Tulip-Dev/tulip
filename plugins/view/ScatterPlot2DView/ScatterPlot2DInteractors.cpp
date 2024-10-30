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
#include <tulip/MouseShowElementInfo.h>
#include <tulip/GraphElementModel.h>

#include "ScatterPlot2DInteractors.h"
#include "ScatterPlot2DViewNavigator.h"
#include "ScatterPlotTrendLine.h"
#include "ScatterPlotCorrelCoeffSelector.h"
#include "ScatterPlot2DView.h"
#include "ScatterPlot2D.h"
#include "ScatterPlotCorrelCoeffSelectorOptionsWidget.h"

#include "../../utils/StandardInteractorPriority.h"
#include "../../utils/PluginNames.h"

using namespace std;

namespace tlp {

ScatterPlot2DInteractor::ScatterPlot2DInteractor(const QString &iconPath, const QString &text,
                                                 const unsigned int priority)
    : NodeLinkDiagramComponentInteractor(iconPath, text, priority) {}

bool ScatterPlot2DInteractor::isCompatible(const std::string &viewName) const {
  return (viewName == ViewName::ScatterPlot2DViewName);
}

PLUGIN(ScatterPlot2DInteractorNavigation)
PLUGIN(ScatterPlot2DInteractorTrendLine)
PLUGIN(ScatterPlot2DInteractorCorrelCoeffSelector)
PLUGIN(ScatterPlot2DInteractorGetInformation)

ScatterPlot2DInteractorNavigation::ScatterPlot2DInteractorNavigation(const tlp::PluginContext *)
    : ScatterPlot2DInteractor(":/tulip/gui/icons/i_navigation.png", "Navigate in view",
                              StandardInteractorPriority::Navigation) {}

void ScatterPlot2DInteractorNavigation::construct() {
  setConfigurationWidgetText(
                             QString("<h3>View navigation interactor</h3>") +
      "<p>This interactor allows to navigate in the scatter plot view.</p>" +
      "<p>When there is more than one graph properties selected, the corresponding scatter plots "
      "previews are generated and displayed in a matrix form. By <b>double clicking on a scatter "
      "plot, " +
      "this one is displayed in fullscreen </b> in a more detailed way and the others interactors "
      "become available. To go back to the scatter plots matrix, double click anywhere in the "
      "view.</p>" +
      "<p>Otherwise, this interactor offers the same functionalities as the one in the \"Node "
      "Link Diagram view\". The commands are described below:</p>" +
        "Translation: <ul><li><b>Mouse left</b> down + moves</li><li>or <b>Arrow</b> keys "
        "</li></ul>" +
        "X or Y rotation: <ul><li><b>Shift + Mouse left</b> down + up/down or left/right "
        "moves</li></ul>" +
#if !defined(__APPLE__)
        "Z rotation: <ul><li><b>Ctrl + Mouse left</b> down + left/right moves</li><li> or "
        "<b>Insert</b> key</li></ul>" +
        "Zoom/Unzoom: <ul><li><b>Mouse wheel</b> up/down</li><li> or <b>Ctrl + Mouse left</b> down "
        "+ up/down moves</li><li> or <b>Pg "
        "up/Pg down</b> keys</li></ul>"
#else
        "Z rotation: <ul><li><b>⌥ + Mouse left</b> down + left/right moves</li><li> or "
        "<b>Insert</b> key</li></ul>" +
        "Translation: <ul><li><b>Arrow</b> keys</li></ul>" +
        "Zoom/Unzoom: <ul><li><b>Mouse wheel</b> down/up</li><li> or <b>⌥ + Mouse left</b> down "
        "+ up/down moves</li><li> or <b>Pg up/Pg "
        "down</b> keys</li></ul>"
#endif
);
  push_back(new ScatterPlot2DViewNavigator);
  push_back(new MouseNKeysNavigator);
}

ScatterPlot2DInteractorTrendLine::ScatterPlot2DInteractorTrendLine(const PluginContext *)
    : ScatterPlot2DInteractor(":/i_scatter_trendline.png", "Trend line",
                              StandardInteractorPriority::ViewInteractor1) {}

void ScatterPlot2DInteractorTrendLine::construct() {
  push_back(new ScatterPlotTrendLine);
  push_back(new MousePanNZoomNavigator);
}

ScatterPlot2DInteractorCorrelCoeffSelector::ScatterPlot2DInteractorCorrelCoeffSelector(
    const tlp::PluginContext *)
    : ScatterPlot2DInteractor(":/tulip/gui/icons/i_magic.png", "Correlation Coefficient Selector",
                              StandardInteractorPriority::ViewInteractor2),
      optionsWidget(nullptr) {}

void ScatterPlot2DInteractorCorrelCoeffSelector::construct() {
  optionsWidget = new ScatterPlotCorrelCoeffSelectorOptionsWidget();
  push_back(new ScatterPlotCorrelCoeffSelector(optionsWidget));
  push_back(new MousePanNZoomNavigator);
}

ScatterPlot2DInteractorCorrelCoeffSelector::~ScatterPlot2DInteractorCorrelCoeffSelector() {
  delete optionsWidget;
}

QWidget *ScatterPlot2DInteractorCorrelCoeffSelector::configurationOptionsWidget() const {
  return optionsWidget;
}

/**
 * We define a specific interactor to show element graph info
 */
class ScatterPlot2DMouseShowElementInfo : public MouseShowElementInfo {
  ScatterPlot2DView *scp2DView;

public:
  ScatterPlot2DMouseShowElementInfo() : MouseShowElementInfo(), scp2DView(nullptr) {}
  ~ScatterPlot2DMouseShowElementInfo() override {}

  void viewChanged(View *v) override {
    scp2DView = static_cast<ScatterPlot2DView *>(v);
    MouseShowElementInfo::viewChanged(v);
  }

protected:
  /**
   * @brief buildModel create and returns the model to visualize edit elements parameters.
   * @param elementType the type of the element can be NODE or EDGE
   * @param elementId elementId the id of the element
   * @param parent the parent for the model creation.
   * @return
   */
  QAbstractItemModel *buildModel(ElementType elementType, unsigned int elementId,
                                 QObject *parent) const override {
    if (scp2DView->getDataLocation() == EDGE) {
      elementId = scp2DView->getMappedId(elementId);
      return new GraphEdgeElementModel(scp2DView->graph(), elementId, parent);
    }

    return MouseShowElementInfo::buildModel(elementType, elementId, parent);
  }

  /**
   * @brief elementName returns the title of the element.
   * @param elementType the type of the element can be NODE or EDGE
   * @param elementId the id of the element
   * @return
   */
  QString elementName(ElementType elementType, unsigned int elementId) const override {
    if (scp2DView->getDataLocation() == EDGE) {
      elementId = scp2DView->getMappedId(elementId);
      return QString("Edge") + " #" + QString::number(elementId);
    }

    return MouseShowElementInfo::elementName(elementType, elementId);
  }
};

ScatterPlot2DInteractorGetInformation::ScatterPlot2DInteractorGetInformation(
    const tlp::PluginContext *)
    : NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_select.png",
                                         "Display node or edge properties",
                                         StandardInteractorPriority::GetInformation) {}

void ScatterPlot2DInteractorGetInformation::construct() {
  setConfigurationWidgetText(
      QString("<h3>Display node or edge properties</h3>") +
      "When the mouse cursor looks like <img src=\":/tulip/gui/icons/i_information.png\">, "
      "indicating it is on top of a graph element (node or edge), "
      "<b>Mouse left</b> click to display a panel showing the element properties.<br/>"
      "to display a panel showing its properties.<br/>"
      "As the panel is displayed, <b>Mouse left</b> click in a property row to edit the "
      "corresponding value.<br/><br/>"
#if defined(__APPLE__)
      "<b>Mouse wheel</b> down/up can be used to zoom/unzoom the view."
#else
      "<b>Mouse wheel</b> up/down can be used to zoom/unzoom the view."
#endif
  );
  push_back(new MousePanNZoomNavigator);
  push_back(new ScatterPlot2DMouseShowElementInfo);
}

bool ScatterPlot2DInteractorGetInformation::isCompatible(const std::string &viewName) const {
  return (viewName == ViewName::ScatterPlot2DViewName);
}
} // namespace tlp
