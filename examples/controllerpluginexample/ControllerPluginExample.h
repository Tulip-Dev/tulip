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
