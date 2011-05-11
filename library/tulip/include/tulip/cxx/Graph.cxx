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
#include <tulip/SimplePluginProgress.h>

namespace tlp {
  template <class PropertyAlgorithm> class PropertyFactory;
  template<class Property> class TemplateAlgorithm;
  template <class PropertyAlgorithm> class PropertyPluginLister;
}

//================================================================================
template<typename ATTRIBUTETYPE>
ATTRIBUTETYPE tlp::Graph::getAttribute(const std::string &name) const {
  const tlp::DataSet &data=getAttributes();
  ATTRIBUTETYPE tmp;
  data.get(name,tmp);
  return tmp;
}
//================================================================================
template<typename ATTRIBUTETYPE>
bool tlp::Graph::getAttribute(const std::string &name, ATTRIBUTETYPE& value) const{
  return getAttributes().get(name, value);
}
//================================================================================
template<typename ATTRIBUTETYPE>
void tlp::Graph::setAttribute(const std::string &name,const ATTRIBUTETYPE&value) {
  tlp::DataSet &data=getNonConstAttributes();
  notifyBeforeSetAttribute(name);
  data.set(name,value);
  notifyAfterSetAttribute(name);
}
//================================================================================
template<typename PropertyType>
PropertyType* tlp::Graph::getLocalProperty(const std::string &name) { 
  if (existLocalProperty(name)) {
    PropertyInterface* prop = getProperty(name);
    assert (dynamic_cast<PropertyType *>(prop)!=0);
    return dynamic_cast<PropertyType *>(prop);
  }
  else {
    PropertyType* prop = new PropertyType(this, name);
    this->addLocalProperty(name, prop);
    return prop;
  }
}
//====================================================================================
template<typename PropertyType>
PropertyType* tlp::Graph::getProperty(const std::string &name) {
  if (existProperty(name)) {
    tlp::PropertyInterface* prop = getProperty(name);
    assert (dynamic_cast<PropertyType *>(prop)!=0);
    return dynamic_cast<PropertyType *>(prop);
  }
  else {
    return getLocalProperty<PropertyType>(name);
  }
}
//====================================================================================
template<typename PropertyType>
bool tlp::Graph::computeProperty(const std::string &algorithm, PropertyType* prop,
				 std::string &msg,  tlp::PluginProgress *progress,
				 tlp::DataSet *data) {
  bool result;
  tlp::PropertyContext context;

  // check if this is a subgraph of prop->graph
  if (getRoot() != prop->graph) {
    tlp::Graph *currentGraph = this;
    while(currentGraph->getSuperGraph() != currentGraph) {
      if (currentGraph == prop->graph)
	break;
      currentGraph = currentGraph->getSuperGraph();
    }
    if (currentGraph != prop->graph)
      return false;
  }    

#ifndef NDEBUG
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
#endif
  if(circularCalls.find(prop) != circularCalls.end()) {
#ifndef NDEBUG
    std::cerr << "Circular call of " << __PRETTY_FUNCTION__ << " " << msg << std::endl;
#endif
    return false;
  }

  // nothing to do if the graph is empty
  if (numberOfNodes() == 0) {
    msg= "The graph is empty";
    return false;
  }

  tlp::PluginProgress *tmpProgress;
  if (progress==0) 
    tmpProgress=new tlp::SimplePluginProgress();
  else 
    tmpProgress=progress;

  context.pluginProgress = tmpProgress;
  context.graph = this;
  context.dataSet = data;
  
  tlp::Observable::holdObservers();
  circularCalls.insert(prop);
  tlp::PropertyContext tmpContext(context);
  tmpContext.propertyProxy = prop;
  typename PropertyType::PAlgorithm *tmpAlgo = tlp::PropertyPluginLister<tlp::TemplateAlgorithm<PropertyType> >::getPluginObject(algorithm, tmpContext);
  if (tmpAlgo != 0) {
    result = tmpAlgo->check(msg);
    if (result) {
      tmpAlgo->run();
    }
    delete tmpAlgo;
  }
  else {
    msg = "No algorithm available with this name";
    result=false;
  }
  circularCalls.erase(prop);
  tlp::Observable::unholdObservers();

  if (progress==0) delete tmpProgress;
  
  return result;
}
//====================================================================================
