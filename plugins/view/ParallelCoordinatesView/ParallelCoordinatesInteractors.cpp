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

#include "ParallelCoordinatesInteractors.h"

#include <tulip/MouseInteractors.h>
#include <tulip/Interactor.h>

#include "ParallelCoordsElementHighlighter.h"
#include "ParallelCoordsAxisSwapper.h"
#include "ParallelCoordsAxisSliders.h"
#include "ParallelCoordsAxisBoxPlot.h"
#include "ParallelCoordsElementsSelector.h"
#include "ParallelCoordsElementShowInfos.h"
#include "ParallelCoordinatesView.h"
#include "ParallelCoordsAxisSpacer.h"

#include "../../utils/StandardInteractorPriority.h"
#include "../../utils/ViewNames.h"

using namespace std;
using namespace tlp;


ParallelCoordinatesInteractor::ParallelCoordinatesInteractor(const QString &iconPath, const QString &text):NodeLinkDiagramComponentInteractor(iconPath,text) {}

bool ParallelCoordinatesInteractor::isCompatible(const string &viewName) const {
  return (viewName == ViewName::ParallelCoordinatesViewName);
}

PLUGIN(InteractorHighLighter)
PLUGIN(InteractorAxisSwapper)
PLUGIN(InteractorAxisSliders)
PLUGIN(InteractorBoxPlot)
PLUGIN(InteractorParallelCoordsSelection)
PLUGIN(InteractorShowElementInfos)
PLUGIN(InteractorAxisSpacer)

InteractorParallelCoordsSelection::InteractorParallelCoordsSelection(const tlp::PluginContext *):ParallelCoordinatesInteractor(":/tulip/gui/icons/i_selection.png","Select elements") {
  setConfigurationWidgetText(QString("<html><head>")
                             +"<title></title>"
                             +"</head>"
                             +"<body>"
                             +"<h3>Elements selection interactor</h3>"
                             +"<p>This interactor allows to select elements on the parallel coordinates view by using the mouse.</p>"
                             +"<p>By <b>left clicking</b> in the drawing, all elements located under the mouse pointer will be selected.</p>"
                             +"<p>It is also possible to <b>select a set of elements by defining a rectangular area</b> with the mouse. To do so, click on the mouse left button and hold it, move the mouse to define a rectangular area and release the left button to select the elements in that area.</p>"
                             +"<p>To <b>insert new elements</b> in the current selection, hold the <b>control key</b> and click on the new elements to add.</p>"
                             +"<p>To <b>remove elements</b> in the current selection, hold the <b>shift key</b> and click on the elements to delete from selection.</p>"
                             +"<p>To reset the current selection, position the mouse cursor so that there is no elements under it and do a left click"
                             +"</body>"
                             +"</html>");
  setPriority(StandardInteractorPriority::RectangleSelection);
}

void InteractorParallelCoordsSelection::construct() {
  push_back(new ParallelCoordsElementsSelector);
  push_back(new MousePanNZoomNavigator);
}

InteractorHighLighter::InteractorHighLighter(const tlp::PluginContext *):ParallelCoordinatesInteractor(":/i_element_highlighter.png","Highlight elements") {
  setConfigurationWidgetText(QString("<html><head>")
                             +"<title></title>"
                             +"</head>"
                             +"<body>"
                             +"<h3>Elements highlighter interactor</h3>"
                             +"<p>This interactor allows to highlight elements in the parallel coordinates view. When elements are highlighted, they keep their original colors while the non highlighted ones get a real low alpha channel value (they become tranlucent). The default alpha channel value for non highlighted elements is set to 10 but it can be modified in the draw configuration tab (setting this value to 0 will make the non highlighted elements disappear from drawing). </p>"
                             +"<p>By <b>left clicking</b> in the drawing, all the elements under the mouse pointer will be highlighted.</p>"
                             +"<p>It is also possible to <b>define a rectangular area by doing a left click and holding it</b> while moving the mouse. By releasing the mouse left button, all the elements located in the defined area will be highlighted. </p>"
                             +"<p>To <b>add elements</b> to the current set of highlighted ones, hold the <b>control key</b> while clicking on the new elements to highlight.</p>"
                             +"<p>To <b>remove elements</b> from the current set of highlighted ones, hold the <b>shift key</b> while clicking on the elements to remove.</p>"
                             +"<p>To reset the set of highlighted elements, position the mouse cursor so that there is no elements under it and do a left click. This operation can also be done by right clicking in the drawing and select the \"Reset highlighted elements\" entry in the popup menu which appears.</p>"
                             +"<p>To select the highlighted elements, do a right click in the drawing and choose the \"Select highlighted elements\" entry in the popup menu which appears.</p>"
                             +"</body>"
                             +"</html>");
  setPriority(StandardInteractorPriority::ViewInteractor1);
}

void InteractorHighLighter::construct() {
  push_back(new ParallelCoordsElementHighlighter);
  push_back(new MousePanNZoomNavigator);
}

InteractorAxisSwapper::InteractorAxisSwapper(const tlp::PluginContext *):ParallelCoordinatesInteractor(":/i_axis_swapper.png","Axis swapper") {
  setConfigurationWidgetText(QString("<html>")
                             +"<head>"
                             +"<title></title>"
                             +"</head>"
                             +"<body>"
                             +"<h3>Axis swapper interactor</h3>"
                             +"<p>This interactor allows to swap two axis with the mouse in the parallel coordinates drawing.</p>"
                             +"<p>To do so, put the mouse pointer under the axis you want to swap, a translucent blue rectangle will be drawn to indicate that you can click to move the axis. Once the pointer is under the axis, do a left click and keep the mouse button pressed while you're dragging the axis. To swap the axis with an other, release the mouse button when a translucent green rectangle appears around the other axis to swap.</p>"
                             +"</body>"
                             +"</html>");
  setPriority(StandardInteractorPriority::ViewInteractor2);
}

void InteractorAxisSwapper::construct() {
  push_back(new ParallelCoordsAxisSwapper);
  push_back(new MousePanNZoomNavigator);
}

InteractorAxisSliders::InteractorAxisSliders(const tlp::PluginContext *):ParallelCoordinatesInteractor(":/i_axis_sliders.png","Axis sliders") {
  setConfigurationWidgetText(QString("<html>")
                             +"<head>"
                             +"<title></title>"
                             +"</head>"
                             +"<body>"
                             +"<h3>Axis sliders interactor</h3>"
                             +"<p>This interactor allows to select a range on a particular axis with the help of sliders and highlight all the data located in that range. It aims to easily filter the data.</p>"
                             +"<p>To use the axis sliders, <b>put the mouse pointer under the slider you want to move</b> (its outline color will change), do a <b>left-click and drag the slider along the axis</b>. Release the mouse button when the slider is at the wanted position, the data located between the two axis sliders will be automatically highlighted.</p>"
                             +"<p>It is also possible to <b>drag the range defined by two axis sliders</b>, by putting the mouse pointer between them (a translucent rectangle will appear) and drag and drop it along the axis.</p>"
                             +"<p>It is possible to filter the current highlighted elements set on other dimensions. To do so, hold the <b>control key</b> while using the axis sliders (they will be colored in orange to indicate that multi-filtering is activated). In that case, the data filtering will be applied on the result of the previous ones if any, not on the entire dataset.</p>"
                             +"<p>It is also possible to add elements to the current highlighted elements set. By holding the <b>shift key</b> while using a slider (all sliders will be colored in light green when pressing this key), the elements located in the slider range will be added to the current highlighted elements set."
                             +"<p>The sliders of the axis whose range has been modified will be colored in blue to indicate on which dimensions the data filtering is made. The other axis sliders will also move automatically to show in which ranges the highlighted data are included on the other dimensions.</p>"
                             +"</body>"
                             +"</html>");
  setPriority(StandardInteractorPriority::ViewInteractor3);
}

void InteractorAxisSliders::construct() {
  push_back(new ParallelCoordsAxisSliders);
  push_back(new MousePanNZoomNavigator);
}

InteractorBoxPlot::InteractorBoxPlot(const tlp::PluginContext *):ParallelCoordinatesInteractor(":/i_axis_boxplot.png","Axis box plot") {
  setConfigurationWidgetText(QString("<html>")
                             +"<head>"
                             +"<title></title>"
                             +"</head>"
                             +"<body>"
                             +"<h3>Axis boxplot interactor</h3>"
                             +"<p>This interactor draw a boxplot above each quantitative axis in the drawing. In descriptive statistics, a boxplot is a convenient way of graphically depicting groups of numerical data through their five-number summaries (the bottom outlier, first quartile (Q1), median (Q2), third (Q3), and the top outlier). The image below illustrates the way to read a box plot.</p>"
                             +"<p>"
                             +"<img src=\":/boxplot_legend.png\" width=\"180\" height=\"240\" border=\"0\" alt=\"\">"
                             +"</p>"
                             +"<p>Boxplots can be useful to display differences between populations without making any assumptions of the underlying statistical distribution. The spacings between the different parts of the box help indicate the degree of dispersion (spread) and skewness in the data, and identify outliers.</p>"
                             +"<p>It is possible to <b>highlight data</b> included in the following <b>axis box plot range</b> :"
                             +"<ul>"
                             +"<li>[Bottom Outlier, First Quartile]</li>"
                             +"<li>[First Quartile, Median]</li>"
                             +"<li>[Median, Third Quartile]</li>"
                             +"<li>[First Quartile, Third Quartile] (= interquartile range) </li>"
                             +"<li>[Third Quartile, Top Outlier]</li>"
                             +"</ul>"
                             +"To do so, <b>put the mouse pointer between the two bounds of the wanted range</b>, a translucent rectangle will be drawn to indicate it is selected, and do a <b>left-click to highlight data</b>. To highlight the data included in the interquartile range, put the mouse pointer near the median line and the interquartile range will be selected.</p>"
                             +"</body>"
                             +"</html>");
  setPriority(StandardInteractorPriority::ViewInteractor4);
}

void InteractorBoxPlot::construct() {
  push_back(new ParallelCoordsAxisBoxPlot);
  push_back(new MousePanNZoomNavigator);
}

InteractorShowElementInfos::InteractorShowElementInfos(const tlp::PluginContext *) : ParallelCoordinatesInteractor(":/tulip/gui/icons/i_select.png", "Get information on nodes/edges") {
  setConfigurationWidgetText(QString("<html>")
                             +"<head>"
                             +"<title></title>"
                             +"</head>"
                             +"<body>"
                             +"<h3>Show element properties interactor</h3>"
                             +"<p>This interactor allows to view the properties associated to an element by clicking on it. Tulip will display all available properties of that node/edge using the Element tab of the Graph Editor sub-window</p>"
                             +"</body>"
                             +"</html>");
  setPriority(StandardInteractorPriority::GetInformation);
}

void InteractorShowElementInfos::construct() {
  push_back(new ParallelCoordsElementShowInfos);
  push_back(new MousePanNZoomNavigator);
}

InteractorAxisSpacer::InteractorAxisSpacer(const tlp::PluginContext *) : ParallelCoordinatesInteractor(":/i_axis_spacer.png", "Modify space between consecutive axis") {
  setConfigurationWidgetText(QString("<html>")
                             +"<head>"
                             +"<title></title>"
                             +"</head>"
                             +"<body>"
                             +"<h3>Axis spacer interactor</h3>"
                             +"<p>This interactor allows to modify the space between two consecutive axis.</p>"
                             +"<p>Put the mouse pointer under an axis, a red rectangle will appear to indicate the axis is selected. Do a left click and keep the mouse button pressed while you're dragging the axis to modify its position. The axis can not be moved further that its neighbors.</p>"
                             +"<p>By double clicking anywhere in the view, the axis positions will be reset to the default ones and the axis will we equally spaced.</p>"
                             +"<p>The axis postions will also be reset to default when the number of selected dimensions changes.</p>"
                             +"</body>"
                             +"</html>");
  setPriority(StandardInteractorPriority::ViewInteractor5);
}

void InteractorAxisSpacer::construct() {
  push_back(new MousePanNZoomNavigator);
  push_back(new ParallelCoordsAxisSpacer());
}
