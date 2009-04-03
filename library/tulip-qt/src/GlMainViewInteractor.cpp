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
}

void InteractorNavigation::construct(){
  pushInteractorComponent(new MouseNKeysNavigator);
}

InteractorGetInformation::InteractorGetInformation():NodeLinkDiagramComponentInteractor(":/i_select.png","Get information on nodes/edges"){
  setPriority(4);
}

void InteractorGetInformation::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseShowElementInfos);
}

InteractorSelection::InteractorSelection():NodeLinkDiagramComponentInteractor(":/i_selection.png","Select nodes/edges in a rectangle"){
  setPriority(3);
}

void InteractorSelection::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseSelector);
}

InteractorSelectionModifier::InteractorSelectionModifier():NodeLinkDiagramComponentInteractor(":/i_move.png","Move/Reshape selection"){
  setPriority(3);
}

void InteractorSelectionModifier::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseSelector);
  pushInteractorComponent(new MouseSelectionEditor);
}

InteractorMagicSelection::InteractorMagicSelection():NodeLinkDiagramComponentInteractor(":/i_magic.png","Selection of reachable elements with equal value"){
  setPriority(3);
}

void InteractorMagicSelection::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseMagicSelector);
}

InteractorRectangleZoom::InteractorRectangleZoom():NodeLinkDiagramComponentInteractor(":/i_zoom.png","Zoom on rectangle"){
  setPriority(2);
}

void InteractorRectangleZoom::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseBoxZoomer);
}

InteractorDeleteElement::InteractorDeleteElement():NodeLinkDiagramComponentInteractor(":/i_del.png","Delete nodes or edges"){
  setPriority(1);
}

void InteractorDeleteElement::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseElementDeleter);
}

InteractorAddNode::InteractorAddNode():NodeLinkDiagramComponentInteractor(":/i_addnode.png","Add nodes"){
  setPriority(1);
}

void InteractorAddNode::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseNodeBuilder);
}

InteractorAddEdge::InteractorAddEdge():NodeLinkDiagramComponentInteractor(":/i_addedge.png","Add edges"){
  setPriority(1);
}

void InteractorAddEdge::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseNodeBuilder);
  pushInteractorComponent(new MouseEdgeBuilder);
}

InteractorEditEdgeBends::InteractorEditEdgeBends():NodeLinkDiagramComponentInteractor(":/i_bends.png","Edit edge bends"){
  setPriority(1);
}

void InteractorEditEdgeBends::construct(){
  pushInteractorComponent(new MousePanNZoomNavigator);
  pushInteractorComponent(new MouseEdgeSelector);
  pushInteractorComponent(new MouseEdgeBendEditor);
}

