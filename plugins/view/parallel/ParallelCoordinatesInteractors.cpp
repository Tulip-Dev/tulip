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

using namespace std;
using namespace tlp;


ParallelCoordinatesInteractor::ParallelCoordinatesInteractor(const QString &iconPath, const QString &text):InteractorChainOfResponsibility(iconPath,text){}

bool ParallelCoordinatesInteractor::isCompatible(const string &viewName) {
	return (viewName == ParallelCoordinatesView::viewName);
}

INTERACTORPLUGINVIEWEXTENSION(ParallelCoordinatesInteractorNavigation,"ParallelCoordinatesNavigationInteractor","InteractorNavigation",ParallelCoordinatesView::viewName, "Tulip Team" ,"02/04/09","Parallel coordinates navigation interactor","1.0")
INTERACTORPLUGINVIEWEXTENSION(ParallelCoordinatesInteractorZoom,"ParallelCoordinatesZoomInteractor","InteractorRectangleZoom",ParallelCoordinatesView::viewName, "Tulip Team" ,"02/04/09","Parallel coordinates rectangle zoom interactor","1.0")

INTERACTORPLUGIN(InteractorHighLighter, "InteractorHighLiter", "Tulip Team", "02/04/2009", "Element Highliter Interactor", "1.0");
INTERACTORPLUGIN(InteractorAxisSwapper, "InteractorAxisSwapper", "Tulip Team", "02/04/2009", "Axis Swap Interactor", "1.0");
INTERACTORPLUGIN(InteractorAxisSliders, "InteractorAxisSliders", "Tulip Team", "02/04/2009", "Axis Sliders Interactor", "1.0");
INTERACTORPLUGIN(InteractorBoxPlot, "InteractorBoxPlot", "Tulip Team", "02/04/2009", "Box Plot Interactor", "1.0");
INTERACTORPLUGIN(InteractorParallelCoordsSelection, "InteractorParallelCoordsSelection", "Tulip Team", "02/04/2009", "Selection Interactor", "1.0");
INTERACTORPLUGIN(InteractorShowElementInfos, "InteractorShowElementInfos", "Tulip Team", "02/04/2009", "Show Element Infos Interactor", "1.0");

QString interactorParallelCoordsSelectionHelpText =
"<html>"
"<head>"
"<title></title>"
"</head>"
"<body>"
"<h3>Elements selection interactor</h3>"
"<p>This interactor allows to select elements on the parallel coordinates view by using the mouse.</p>"
"<p>By <b>left clicking</b> in the drawing, all elements located under the mouse pointer will be selected.</p>"
"<p>It is also possible to <b>select a set of elements by defining a rectangular area</b> with the mouse. To do so, click on the mouse left button and hold it, move the mouse to define a rectangular area and release the left button to select the elements in that area.</p>"
"<p>To <b>insert new elements</b> in the current selection, hold the <b>control key</b> and click on the new elements to add.</p>"
"<p>To <b>remove elements</b> in the current selection, hold the <b>shift key</b> and click on the elements to delete from selection.</p>"
"<p>To reset the current selection, position the mouse cursor so that there is no elements under it and do a left click"
"</body>"
"</html>";

QString interactorParallelEltShowInfosHelpText =
"<html>"
"<head>"
"<title></title>"
"</head>"
"<body>"
"<h3>Show element properties interactor</h3>"
"<p>This interactor allows to view the properties associated to an element by clicking on it. Tulip will display all available properties of that node/edge using the Element tab of the Graph Editor sub-window</p>"
"</body>"
"</html>";

QString interactorParallelCoordsHighlighterHelpText =
"<html>"
"<head>"
"<title></title>"
"</head>"
"<body>"
"<h3>Elements highlighter interactor</h3>"
"<p>This interactor allows to highlight elements in the parallel coordinates view. When elements are highlighted, they keep their original colors while the non highlighted ones get a real low alpha channel value (they become tranlucent). The default alpha channel value for non highlighted elements is set to 10 but it can be modified in the draw configuration tab (setting this value to 0 will make the non highlighted elements disappear from drawing). </p>"
"<p>By <b>left clicking</b> in the drawing, all the elements under the mouse pointer will be highlighted.</p>"
"<p>It is also possible to <b>define a rectangular area by doing a left click and holding it</b> while moving the mouse. By releasing the mouse left button, all the elements located in the defined area will be highlighted. </p>"
"<p>To <b>add elements</b> to the current set of highlighted ones, hold the <b>control key</b> while clicking on the new elements to highlight.</p>"
"<p>To <b>remove elements</b> from the current set of highlighted ones, hold the <b>shift key</b> while clicking on the elements to remove.</p>"
"<p>To reset the set of highlighted elements, position the mouse cursor so that there is no elements under it and do a left click. This operation can also be done by right clicking in the drawing and select the \"Reset highlighted elements\" entry in the popup menu which appears.</p>"
"<p>To select the highlighted elements, do a right click in the drawing and choose the \"Select highlighted elements\" entry in the popup menu which appears.</p>"
"</body>"
"</html>";

QString interactorParallelBoxPlotHelpText =
"<html>"
"<head>"
"<title></title>"
"</head>"
"<body>"
"<h3>Axis boxplot interactor</h3>"
"<p>This interactor draw a boxplot above each quantitative axis in the drawing. In descriptive statistics, a boxplot is a convenient way of graphically depicting groups of numerical data through their five-number summaries (the bottom outlier, first quartile (Q1), median (Q2), third (Q3), and the top outlier). The image below illustrates the way to read a box plot.</p>"
"<p>"
"<img src=\":/boxplot_legend.png\" width=\"180\" height=\"240\" border=\"0\" alt=\"\">"
"</p>"
"<p>Boxplots can be useful to display differences between populations without making any assumptions of the underlying statistical distribution. The spacings between the different parts of the box help indicate the degree of dispersion (spread) and skewness in the data, and identify outliers.</p>"
"<p>It is possible to <b>highlight data</b> included in the following <b>axis box plot range</b> :"
"<ul>"
"<li>[Bottom Outlier, First Quartile]</li>"
"<li>[First Quartile, Median]</li>"
"<li>[Median, Third Quartile]</li>"
"<li>[First Quartile, Third Quartile] (= interquartile range) </li>"
"<li>[Third Quartile, Top Outlier]</li>"
"</ul>"
"To do so, <b>put the mouse pointer between the two bounds of the wanted range</b>, a translucent rectangle will be drawn to indicate it is selected, and do a <b>left-click to highlight data</b>. To highlight the data included in the interquartile range, put the mouse pointer near the median line and the interquartile range will be selected.</p>"
"</body>"
"</html>";

QString interactorAxisSwapperHelpText =
"<html>"
"<head>"
"<title></title>"
"</head>"
"<body>"
"<h3>Axis swapper interactor</h3>"
"<p>This interactor allows to swap two axis with the mouse in the parallel coordinates drawing.</p>"
"<p>To do so, put the mouse pointer under the axis you want to swap, a translucent blue rectangle will be drawn to indicate that you can click to move the axis. Once the pointer is under the axis, do a left click and keep the mouse button pressed while you're dragging the axis. To swap the axis with an other, release the mouse button when a translucent green rectangle appears around the other axis to swap.</p>"
"</body>"
"</html>";


QString interactorAxisSlidersHelpText =
"<html>"
"<head>"
"<title></title>"
"</head>"
"<body>"
"<h3>Axis sliders interactor</h3>"
"<p>This interactor allows to select a range on a particular axis with the help of sliders and highlight all the data located in that range. It aims to easily filter the data.</p>"
"<p>To use the axis sliders, <b>put the mouse pointer under the slider you want to move</b> (its outline color will change), do a <b>left-click and drag the slider along the axis</b>. Release the mouse button when the slider is at the wanted position, the data located between the two axis sliders will be automatically highlighted.</p>"
"<p>It is also possible to <b>drag the range defined by two axis sliders</b>, by putting the mouse pointer between them (a translucent rectangle will appear) and drag and drop it along the axis.</p>"
"<p>Data <b>filtering can be made on several dimensions</b>. To do so, hold the <b>control key</b> while using the axis sliders (they will be colored in orange to indicate that multi-filtering is activated). In that case, the data filtering will be applied on the result of the previous ones if any, not on the entire dataset.</p>"
"<p>The sliders of the axis whose range has been modified will be colored in blue to indicate on which dimensions the data filtering is made. The other axis sliders will also move automatically to show in which ranges the highlighted data are included on the other dimensions.</p>"
"</body>"
"</html>";

InteractorParallelCoordsSelection::InteractorParallelCoordsSelection():ParallelCoordinatesInteractor(":/i_selection.png","Select elements"){
  setPriority(3);
}

void InteractorParallelCoordsSelection::construct(){
  setConfigurationWidgetText(interactorParallelCoordsSelectionHelpText);
  pushInteractorComponent(new ParallelCoordsElementsSelector);
  pushInteractorComponent(new MousePanNZoomNavigator);
}

InteractorHighLighter::InteractorHighLighter():ParallelCoordinatesInteractor(":/i_element_highlighter.png","Highlight elements"){
}

void InteractorHighLighter::construct(){
  setConfigurationWidgetText(interactorParallelCoordsHighlighterHelpText);
  pushInteractorComponent(new ParallelCoordsElementHighlighter);
  pushInteractorComponent(new MousePanNZoomNavigator);
}

InteractorAxisSwapper::InteractorAxisSwapper():ParallelCoordinatesInteractor(":/i_axis_swapper.png","Axis Swapper"){
}

void InteractorAxisSwapper::construct(){
  pushInteractorComponent(new ParallelCoordsAxisSwapper);
  pushInteractorComponent(new MousePanNZoomNavigator);
  setConfigurationWidgetText(interactorAxisSwapperHelpText);
}

InteractorAxisSliders::InteractorAxisSliders():ParallelCoordinatesInteractor(":/i_axis_sliders.png","Axis Sliders"){
}

void InteractorAxisSliders::construct(){
  pushInteractorComponent(new ParallelCoordsAxisSliders);
  pushInteractorComponent(new MousePanNZoomNavigator);
  setConfigurationWidgetText(interactorAxisSlidersHelpText);
}

InteractorBoxPlot::InteractorBoxPlot():ParallelCoordinatesInteractor(":/i_axis_boxplot.png","Axis Box Plot"){
}

void InteractorBoxPlot::construct(){
  pushInteractorComponent(new ParallelCoordsAxisBoxPlot);
  pushInteractorComponent(new MousePanNZoomNavigator);
  setConfigurationWidgetText(interactorParallelBoxPlotHelpText);
}

InteractorShowElementInfos::InteractorShowElementInfos() : ParallelCoordinatesInteractor(":/i_select.png", "Get information on nodes/edges") {
	setPriority(4);
}

void InteractorShowElementInfos::construct() {
	pushInteractorComponent(new ParallelCoordsElementShowInfos);
	pushInteractorComponent(new MousePanNZoomNavigator);
	setConfigurationWidgetText(interactorParallelEltShowInfosHelpText);
}

