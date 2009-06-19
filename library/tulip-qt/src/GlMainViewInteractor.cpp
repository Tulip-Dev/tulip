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
			     "3D Navigation in the graph<br><br>"+
			     "Translation: <ul><li><b>Mouse left</b> down + moves</li><li>or <b>Arrow</b> keys down</li></ul>" +
			     "X or Y rotation: <ul><li><b>Shift + Mouse left</b> down + up/down or left/right moves</li></ul>" +
#if !defined(__APPLE__) 
			     "Z rotation: <ul><li><b>Ctrl + Mouse left</b> down + left/right moves</li><li> or <b>Insert</b> key</li></ul>" +
			     "Zoom/Unzoom: <ul><li><b>Ctrl + Mouse left</b> down + up/down moves</li><li> or <b>Pg up/Pg down</b> keys</li></ul>"
#else
			     "Z rotation: <ul><li><b>Alt + Mouse left</b> down + left/right moves</li><li> or <b>Insert</b> key</li></ul>" +
			     "Zoom/Unzoom: <ul><li><b>Alt + Mouse left</b> down + up/down moves<li></li> or <b>Pg up/Pg down</b> keys</li></ul>"
#endif
);
}

void InteractorNavigation::construct(){
  pushInteractorComponent(new MouseNKeysNavigator);
}

InteractorGetInformation::InteractorGetInformation():NodeLinkDiagramComponentInteractor(":/i_select.png","Get information on nodes/edges"){
  setPriority(4);
  setConfigurationWidgetText(QString("<h3>Get information interactor</h3>")+
			     "<b>Mouse left</b> click on an element to display its properties");
}

void InteractorGetInformation::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseShowElementInfos);
}

InteractorSelection::InteractorSelection():NodeLinkDiagramComponentInteractor(":/i_selection.png","Select nodes/edges in a rectangle"){
  setPriority(3);
  setConfigurationWidgetText(QString("<h3>Selection interactor</h3>")+
			     "Select on rectangle.<br/><b>Mouse left</b> down indicates the first corner, <b>Mouse left</b> up indicates the opposite corner.<br/><br/>"+
#if !defined(__APPLE__) 
			     "Add/Remove from selection: <ul><li><b>Ctrl + Mouse left</b> click on an element</li></ul>" +
#else
			     "Add/Remove from selection: <ul><li><b>Alt + Mouse left</b> click</li></ul>"+
#endif
			     "Remove from selection: <ul><li><b>Shift + Mouse</b> click</li></ul>");
}

void InteractorSelection::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseSelector);
}

InteractorSelectionModifier::InteractorSelectionModifier():NodeLinkDiagramComponentInteractor(":/i_move.png","Move/Reshape selection"){
  setPriority(3);
  setConfigurationWidgetText(QString("<h3>Selection modifier interactor</h3>")+
			     "Modify selection<br/><br/>"+
			     "Resize: <ul><li><b>Mouse left</b> down on triangle + moves</li></ul>" +
			     "Rotate: <ul><li><b>Mouse left</b> down on circle + moves</li></ul>" +
			     "Resize & Rotate: <ul><li><b>Mouse left</b> down on suqare + moves</li></ul>" +
			     "Translate: <ul><li><b>Mouse left</b> down inside rectangle + moves</li></ul>");
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
			     "Zoom on selected rectangle.<br><b>Mouse left</b> down indicates the first corner, <b>Mouse left</b> up indicates the opposite corner.");
}

void InteractorRectangleZoom::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseBoxZoomer);
}

InteractorDeleteElement::InteractorDeleteElement():NodeLinkDiagramComponentInteractor(":/i_del.png","Delete nodes or edges"){
  setPriority(1);
  setConfigurationWidgetText(QString("<h3>Delete interactor</h3>")+
			     "<b>Mouse left</b> click on an element to delete it");
}

void InteractorDeleteElement::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseElementDeleter);
}

InteractorAddNode::InteractorAddNode():NodeLinkDiagramComponentInteractor(":/i_addnode.png","Add nodes"){
  setPriority(1);
  setConfigurationWidgetText(QString("<h3>Add node interactor</h3>")+
			     "<b>Mouse left</b> click to add a node in the graph");
}

void InteractorAddNode::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseNodeBuilder);
}

InteractorAddEdge::InteractorAddEdge():NodeLinkDiagramComponentInteractor(":/i_addedge.png","Add edges"){
  setPriority(1);
  setConfigurationWidgetText(QString("<h3>Add edge interactor</h3>")+
			     "<b>Mouse left</b> click on the source node,<br/>then <b>Mouse left</b> click on the target node.<br/>Any <b>Mouse left</b> click outside a node will add an edge bend.");
}

void InteractorAddEdge::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseNodeBuilder);
  pushInteractorComponent(new MouseEdgeBuilder);
}

InteractorEditEdgeBends::InteractorEditEdgeBends():NodeLinkDiagramComponentInteractor(":/i_bends.png","Edit edge bends"){
  setPriority(1);
  setConfigurationWidgetText(QString("<h3>Edit edge bends interactor</h3>")+
			     "Modify edge bends<br/><br/>"+
			     "Select edge: <ul><li>use rectangle selection</li></ul>" +
			     "Translate bend: <ul><li><b>Mouse left</b> down on a selected bend + moves</li></ul>" +
			     "Add bend: <ul><li><b>Shift + Mouse left</b> click on the selected edge</li></ul>"+
#if !defined(__APPLE__) 
			     "Delete bend: <ul><li><b>Ctrl + Mouse left</b> click</li></ul>"
#else
			     "Delete bend: <ul><li><b>Alt + Mouse left</b> click</li></ul>"
#endif
			     );
}

void InteractorEditEdgeBends::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseEdgeSelector);
  pushInteractorComponent(new MouseEdgeBendEditor);
}

