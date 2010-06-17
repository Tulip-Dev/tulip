/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include "ControllerPluginExample.h"

#include <tulip/View.h>

using namespace std;
using namespace tlp;


// VIEWPLUGIN(ClassName, "ControllerName", "Authors", "date", "Long controller plugin name", "plugin_release");
CONTROLLERPLUGIN(ControllerPluginExample, "ControllerPluginExample", "Author","12/02/2009","Controller Plugin Example", "1.0");

ControllerPluginExample::~ControllerPluginExample() {
  // when we delete this controller, we remove it of observer
  if(graph){
    Iterator<PropertyInterface*> *it = graph->getObjectProperties();
    while (it->hasNext()) {
      PropertyInterface* tmp = it->next();
      tmp->removeObserver(this);
    } delete it;

    graph->removeObserver(this);
  }
}

void ControllerPluginExample::attachMainWindow(tlp::MainWindowFacade facade) {
  // Call the attachMainWindow of ControllerViewsManager
  ControllerViewsManager::attachMainWindow(facade);
}

void ControllerPluginExample::setData(Graph *graph, DataSet dataSet) {
  // When we setData, we create a Node link diagram view
  nodeLinkView=ControllerViewsManager::createView("Node Link Diagram view",graph,dataSet,true,QRect(0,0,100,100),true);
  this->graph = graph;

  // We set observer to observe properties and graph
  Iterator<PropertyInterface*> *it = graph->getObjectProperties();
  while (it->hasNext()) {
    PropertyInterface* tmp = it->next();
    tmp->addObserver(this);
  } delete it;

  graph->addObserver(this);
}

void ControllerPluginExample::getData(Graph **graph, DataSet *data) {
  *graph = this->graph;
}

Graph *ControllerPluginExample::getGraph() {
  return this->graph;
}

void ControllerPluginExample::update ( std::set< tlp::Observable * >::iterator begin, std::set< tlp::Observable * >::iterator end) {
  // When graph or property is modified, we draw the view
  nodeLinkView->draw();
}
