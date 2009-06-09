#include "tulip/GlMainViewInteractor.h"

#include "tulip/MouseInteractors.h"
#include "tulip/MouseNodeBuilder.h"
#include "tulip/MouseEdgeBuilder.h"
#include "tulip/MouseShowElementInfos.h"
#include "tulip/MouseSelector.h"
#include "tulip/MouseSelectionEditor.h"
#include "tulip/MouseMagicSelector.h"
#include "tulip/MouseBoxZoomer.h"
#include "tulip/MouseEdgeSelector.h"
#include "tulip/MouseEdgeBendEditor.h"

using namespace tlp;
using namespace std;

NodeLinkDiagramComponentInteractor::NodeLinkDiagramComponentInteractor(const QString &iconPath, const QString &text):InteractorChainOfResponsibility(iconPath,text){

}

INTERACTORPLUGIN(InteractorNavigation, "InteractorNavigation", "Tulip Team", "01/04/2009", "Navigation Interactor", "1.0");
INTERACTORPLUGIN(InteractorGetInformation, "InteractorGetInformation", "Tulip Team", "01/04/2009", "Get Information Interactor", "1.0");
INTERACTORPLUGIN(InteractorSelection, "InteractorSelection", "Tulip Team", "01/04/2009", "Selection Interactor", "1.0");
INTERACTORPLUGIN(InteractorSelectionModifier, "InteractorSelectionModifier", "Tulip Team", "01/04/2009", "Selection Modifier Interactor", "1.0");
INTERACTORPLUGIN(InteractorMagicSelection, "InteractorMagicSelection", "Tulip Team", "01/04/2009", "Magic Selection Interactor", "1.0");
INTERACTORPLUGIN(InteractorRectangleZoom, "InteractorRectangleZoom", "Tulip Team", "01/04/2009", "Rectangle Zoom Interactor", "1.0");
INTERACTORPLUGIN(InteractorDeleteElement, "InteractorDeleteElement", "Tulip Team", "01/04/2009", "Delete Element Interactor", "1.0");
INTERACTORPLUGIN(InteractorAddNode, "InteractorAddNode", "Tulip Team", "01/04/2009", "Add Node Interactor", "1.0");
INTERACTORPLUGIN(InteractorAddEdge, "InteractorAddEdge", "Tulip Team", "01/04/2009", "Add Edge Interactor", "1.0");
INTERACTORPLUGIN(InteractorEditEdgeBends, "InteractorEditEdgeBends", "Tulip Team", "01/04/2009", "Edit Edge Bends Interactor", "1.0");

InteractorNavigation::InteractorNavigation():NodeLinkDiagramComponentInteractor(":/i_navigation.png","Navigate in graph"){
  setPriority(5);
  setConfigurationWidgetText(QString("<h3>Navigation interactor</h3>")+
			     "Navigate in the graph<br><br>"+
			     "<b>Ctrl + Mouse up/down</b> : zoom<br>"+
			     "<b>Ctrl + Mouse left/right</b> : z rotation<br>"+
			     "<b>Shift + Mouse</b> : rotation<br>"+
			     "<b>Key up/down</b> : up/down<br>"+
			     "<b>Key left/right</b> : left/right<br>"+
			     "<b>Key page up/down</b> : zoom<br>"+
			     "<b>Key insert</b> : rotate<br>");
}

void InteractorNavigation::construct(){
  pushInteractorComponent(new MouseNKeysNavigator);
}

InteractorGetInformation::InteractorGetInformation():NodeLinkDiagramComponentInteractor(":/i_select.png","Get information on nodes/edges"){
  setPriority(4);
  setConfigurationWidgetText(QString("<h3>Get information interactor</h3>")+
			     "Click on an element to display properties of this element");
}

void InteractorGetInformation::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseShowElementInfos);
}

InteractorSelection::InteractorSelection():NodeLinkDiagramComponentInteractor(":/i_selection.png","Select nodes/edges in a rectangle"){
  setPriority(3);
  setConfigurationWidgetText(QString("<h3>Selection interactor</h3>")+
			     "Select on rectangle<br><br>"+
			     "<b>Ctrl + Mouse</b> : add/remove from selection<br>"+
			     "<b>Shift + Mouse</b> : remove to selection");
}

void InteractorSelection::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseSelector);
}

InteractorSelectionModifier::InteractorSelectionModifier():NodeLinkDiagramComponentInteractor(":/i_move.png","Move/Reshape selection"){
  setPriority(3);
  setConfigurationWidgetText(QString("<h3>Selection modifier interactor</h3>")+
			     "Modify selection<br><br>"+
			     "<b>Circle</b> : rotation<br>"+
			     "<b>Square</b> : scale<br>"+
			     "<b>Arrow</b> : scale<br>"+
			     "<b>Click on center</b> : translation");
}

void InteractorSelectionModifier::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseSelector);
  pushInteractorComponent(new MouseSelectionEditor);
}

InteractorMagicSelection::InteractorMagicSelection():NodeLinkDiagramComponentInteractor(":/i_magic.png","Selection of reachable elements with equal value"){
  setPriority(3);
  setConfigurationWidgetText(QString("<h3>Magic selection interactor</h3>")+
			     "Select neighbour elements with same metric");
}

void InteractorMagicSelection::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseMagicSelector);
}

InteractorRectangleZoom::InteractorRectangleZoom():NodeLinkDiagramComponentInteractor(":/i_zoom.png","Zoom on rectangle"){
  setPriority(2);
  setConfigurationWidgetText(QString("<h3>Rectangle zoom interactor</h3>")+
			     "Zoom on rectangle");
}

void InteractorRectangleZoom::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseBoxZoomer);
}

InteractorDeleteElement::InteractorDeleteElement():NodeLinkDiagramComponentInteractor(":/i_del.png","Delete nodes or edges"){
  setPriority(1);
  setConfigurationWidgetText(QString("<h3>Delete interactor</h3>")+
			     "Delete graph element");
}

void InteractorDeleteElement::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseElementDeleter);
}

InteractorAddNode::InteractorAddNode():NodeLinkDiagramComponentInteractor(":/i_addnode.png","Add nodes"){
  setPriority(1);
  setConfigurationWidgetText(QString("<h3>Add node interactor</h3>")+
			     "Add a node in the graph");
}

void InteractorAddNode::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseNodeBuilder);
}

InteractorAddEdge::InteractorAddEdge():NodeLinkDiagramComponentInteractor(":/i_addedge.png","Add edges"){
  setPriority(1);
  setConfigurationWidgetText(QString("<h3>Add edge interactor</h3>")+
			     "Add an edge in the graph. If you don't click on a node, an edge bend is create");
}

void InteractorAddEdge::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseNodeBuilder);
  pushInteractorComponent(new MouseEdgeBuilder);
}

InteractorEditEdgeBends::InteractorEditEdgeBends():NodeLinkDiagramComponentInteractor(":/i_bends.png","Edit edge bends"){
  setPriority(1);
  setConfigurationWidgetText(QString("<h3>Edit edge bends interactor</h3>")+
			     "Modify edge bends<br><br>"+
			     "<b>Shift</b> : add bend<br>"+
			     "<b>Ctrl</b> : delete bend");
}

void InteractorEditEdgeBends::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseEdgeSelector);
  pushInteractorComponent(new MouseEdgeBendEditor);
}

