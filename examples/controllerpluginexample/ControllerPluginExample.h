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
#ifndef CONTROLLERPLUGINEXAMPLE_H
#define CONTROLLERPLUGINEXAMPLE_H

#include <tulip/ControllerViewsManager.h>
#include <tulip/DataSet.h>
#include <tulip/Graph.h>
#include <tulip/AbstractProperty.h>
#include <tulip/Observable.h> 

// For this example we construct an simple controller who display a simple node link diagram view
// This Controller use implementation of ControllerViewsManager
// And observe graph and properties of graph
class ControllerPluginExample: public tlp::ControllerViewsManager, public tlp::Observer {

public:

  virtual ~ControllerPluginExample();

  // This function is call when tulip want to open it
  virtual void attachMainWindow(tlp::MainWindowFacade facade);

  // This function is call when tulip want to attach data on this controller 
  virtual void setData(tlp::Graph *graph=0,tlp::DataSet dataSet=tlp::DataSet());

  // This function is call when tulip want to save this controller
  virtual void getData(tlp::Graph **graph, tlp::DataSet *data);

  // Return the current graph of this controller
  virtual tlp::Graph *getGraph();

protected :

  // This function is call by Observable
  // In setData, we add this controller to graph observer and properties observer
  // So this function is call when the graph is modified or when a property is modified
  virtual void update( std::set< tlp::Observable * >::iterator begin, std::set< tlp::Observable * >::iterator end);

  // This function is need by Oberver class
  virtual void observableDestroyed(tlp::Observable*){}

 private:
  tlp::Graph *graph;
  tlp::View *nodeLinkView;
};

#endif
