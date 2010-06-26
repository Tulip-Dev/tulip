/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include "InteractorPluginExample.h"

#include <QtGui/QMouseEvent>

#include <tulip/MouseInteractors.h>
#include <tulip/GlMainWidget.h>

using namespace std;
using namespace tlp;
 
// INTERACTORPLUGIN(ClassName, "InteractorName", "Authors", "date", "Long interactor plugin name", "plugin_release");
INTERACTORPLUGIN(InteractorPluginExample, "InteractorPluginExample", "Tulip developers", "30/11/2009", "Interactor Plugin Example", "0.1");

// InteractorChainOfResponsibility parameters :
//  - first : icon to use for this interactor (in this case we use Qt ressources system : see InteractorPluginRessources.qrc)
//  - second : name of the interactor (is visible in the tooltip of the interactor)
InteractorPluginExample::InteractorPluginExample():InteractorChainOfResponsibility(":/i_iconexample.png","Select one node/edge"){
  
  // Set the priority of the interactor 
  //   - priority is used to sort interactors icons on tool bar
  //     for example an interactor with 5 (or more) in priority appear at left side of the toolbar
  //     an interactor with 0 appear at right side
  setPriority(0);
  
  // Set the text appear on the configuration widget of this interactor
  setConfigurationWidgetText(QString("<h3>Example Interactor Plugin</h3>")+
			     "<b>Mouse left</b> click on an element to select it");
}

// Contruct the interactor
void InteractorPluginExample::construct(){
  // In this case we put two interactor component on this interactor
  //   - If InteractorPluginComponent don't catch the event it will be passed to the MouseNKeysNavigator 
  pushInteractorComponent(new MouseNKeysNavigator);
  pushInteractorComponent(new InteractorPluginComponent);
}

// Event filter of the interactor component (main function of the interactor component)
bool InteractorPluginComponent::eventFilter(QObject *widget, QEvent *e) {
  
  if (e->type() == QEvent::MouseButtonRelease) {
    QMouseEvent * qMouseEv = (QMouseEvent *) e;
    GlMainWidget *glMainWidget = (GlMainWidget *) widget;
    if (qMouseEv->button()== Qt::LeftButton){
      // Enter here if we have released the left button of the mouse

      // doSelect function return node/edge under the mouse
      node tmpNode;
      edge tmpEdge;
      ElementType type;
      bool result = glMainWidget->doSelect(qMouseEv->x(), qMouseEv->y(), type, tmpNode, tmpEdge);
      if (result) {
	// Enter here if we have node/edge under the mouse

	// Store selection property
	Graph *graph=glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
	string selectionPropertyName=glMainWidget->getScene()->getGlGraphComposite()->getInputData()->getElementSelectedPropName();
	BooleanProperty* selection=graph->getProperty<BooleanProperty>(selectionPropertyName);
	
	// Before do any think on the graph, we push the current state of the graph (this activate the undo/redo system)
	graph->push();

	// Deselect all nodes/edges
	selection->setAllNodeValue(false);
	selection->setAllEdgeValue(false);

	switch(type) {
	case NODE:
	  // Set selection at true for selected node
	  selection->setNodeValue(tmpNode, true);
	  break;
	case EDGE:
	  // Set selection at false for selected edge
	  selection->setEdgeValue(tmpEdge, true);
	  break;
	}
	
	// We have treated the event so we return true 
	// (this event will not be passed to others interactorComponent)
	return true;
      }
    } 
  }
  // We don't have treated the event so we return false
  // (this event will be passed to others interactorComponent)
  return false;
}
