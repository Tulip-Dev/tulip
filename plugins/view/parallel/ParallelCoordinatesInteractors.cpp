#include "ParallelCoordinatesInteractors.h"

#include <tulip/MouseInteractors.h>
#include <tulip/Interactor.h>

#include "ParallelCoordsElementHighlighter.h"
#include "ParallelCoordsAxisSwapper.h"
#include "ParallelCoordsAxisSliders.h"
#include "ParallelCoordsAxisBoxPlot.h"
#include "ParallelCoordsElementsSelector.h"
#include "ParallelCoordsElementShowInfos.h"

using namespace std;
using namespace tlp;


ParallelCoordinatesInteractor::ParallelCoordinatesInteractor(const QString &iconPath, const QString &text):InteractorChainOfResponsibility(iconPath,text){

}

INTERACTORPLUGINVIEWEXTENSION(ParallelCoordinatesInteractorNavigation,"ParallelCoordinatesNavigationInteractor","InteractorNavigation","Parallel Coordinates view", "Tulip Team" ,"02/04/09","Parallel coordinates navigation interactor","1.0")
INTERACTORPLUGINVIEWEXTENSION(ParallelCoordinatesInteractorZoom,"ParallelCoordinatesZoomInteractor","InteractorRectangleZoom","Parallel Coordinates view", "Tulip Team" ,"02/04/09","Parallel coordinates rectangle zoom interactor","1.0")

INTERACTORPLUGIN(InteractorHighLighter, "InteractorHighLiter", "Tulip Team", "02/04/2009", "Element Highliter Interactor", "1.0");
INTERACTORPLUGIN(InteractorAxisSwapper, "InteractorAxisSwapper", "Tulip Team", "02/04/2009", "Axis Swap Interactor", "1.0");
INTERACTORPLUGIN(InteractorAxisSliders, "InteractorAxisSliders", "Tulip Team", "02/04/2009", "Axis Sliders Interactor", "1.0");
INTERACTORPLUGIN(InteractorBoxPlot, "InteractorBoxPlot", "Tulip Team", "02/04/2009", "Box Plot Interactor", "1.0");
INTERACTORPLUGIN(InteractorParallelCoordsSelection, "InteractorParallelCoordsSelection", "Tulip Team", "02/04/2009", "Selection Interactor", "1.0");
INTERACTORPLUGIN(InteractorShowElementInfos, "InteractorShowElementInfos", "Tulip Team", "02/04/2009", "Show Element Infos Interactor", "1.0");

InteractorParallelCoordsSelection::InteractorParallelCoordsSelection():ParallelCoordinatesInteractor(":/i_selection.png","Select elements"){
  setPriority(3);
}

void InteractorParallelCoordsSelection::construct(){
  pushInteractorComponent(new ParallelCoordsElementsSelector);
  pushInteractorComponent(new MousePanNZoomNavigator);
}

InteractorHighLighter::InteractorHighLighter():ParallelCoordinatesInteractor(":/i_element_highlighter.png","Highlight elements"){
}

void InteractorHighLighter::construct(){
  pushInteractorComponent(new ParallelCoordsElementHighlighter);
  pushInteractorComponent(new MousePanNZoomNavigator);
}

InteractorAxisSwapper::InteractorAxisSwapper():ParallelCoordinatesInteractor(":/i_axis_swapper.png","Axis Swapper"){
}

void InteractorAxisSwapper::construct(){
  pushInteractorComponent(new ParallelCoordsAxisSwapper);
  pushInteractorComponent(new MousePanNZoomNavigator);
}

InteractorAxisSliders::InteractorAxisSliders():ParallelCoordinatesInteractor(":/i_axis_sliders.png","Axis Sliders"){
}

void InteractorAxisSliders::construct(){
  pushInteractorComponent(new ParallelCoordsAxisSliders);
  pushInteractorComponent(new MousePanNZoomNavigator);
}

InteractorBoxPlot::InteractorBoxPlot():ParallelCoordinatesInteractor(":/i_axis_boxplot.png","Axis Box Plot"){
}

void InteractorBoxPlot::construct(){
  pushInteractorComponent(new ParallelCoordsAxisBoxPlot);
  pushInteractorComponent(new MousePanNZoomNavigator);
}

InteractorShowElementInfos::InteractorShowElementInfos() : ParallelCoordinatesInteractor(":/i_select.png", "Get information on nodes/edges") {
	setPriority(4);
}

void InteractorShowElementInfos::construct() {
	pushInteractorComponent(new ParallelCoordsElementShowInfos);
	pushInteractorComponent(new MousePanNZoomNavigator);
}

