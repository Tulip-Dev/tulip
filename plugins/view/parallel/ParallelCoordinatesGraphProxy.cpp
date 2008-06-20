//-*-c++-*-
/*
 Author: Didier Bathily, Nicolas Bellino, Jonathan Dubois, Christelle Jolly, Antoine Lambert, Nicolas Sorraing

 Email : didier.bathily@etu.u-bordeaux1.fr, nicolas.bellino@etu.u-bordeaux1.fr, jonathan.dubois@etu.u-bordeaux1.fr, christelle.jolly@etu.u-bordeaux1.fr, antoine.lambert@etu.u-bordeaux1.fr, nicolas.sorraing@etu.u-bordeaux1.fr

 Last modification : 03/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#include <tulip/ColorProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/ForEach.h>
#include <tulip/IntegerProperty.h>
#include <tulip/DoubleProperty.h>

#include "ParallelCoordinatesGraphProxy.h"




using namespace std;
namespace tlp {

ParallelCoordinatesGraphProxy::ParallelCoordinatesGraphProxy(Graph *g, DataLocation location):GraphDecorator(g), dataLocation(location) {
  fillPropertiesVector();
}

void ParallelCoordinatesGraphProxy::fillPropertiesVector() {
  Iterator<string> * it = getLocalProperties();
  string propertyName;

  propertiesList.clear();
  while (it->hasNext()) {
    propertyName = it->next();
    if (propertyName.find("view") == string::npos) {
      propertiesList.push_back(propertyName);
    }
  }
}

int ParallelCoordinatesGraphProxy::getNumberOfSelectedProperties(){

return selectedProxy.size();
}

list<string> ParallelCoordinatesGraphProxy::getSelectedProperties(){
  return selectedProxy;
}

void ParallelCoordinatesGraphProxy::setSelectedProperties(list<string>& l){
  selectedProxy=l;
}

DataLocation ParallelCoordinatesGraphProxy::getDataLocation(){
  return dataLocation;
}

unsigned int ParallelCoordinatesGraphProxy::getDataCount() {
  if (getDataLocation() == NODES) {
    return numberOfNodes();
  } else {
    return numberOfEdges();
  }
}

Color ParallelCoordinatesGraphProxy::getDataColor(unsigned int dataId) {
  if (getDataLocation() == NODES) { 
    return getLocalProperty<ColorProperty>("viewColor")->getNodeValue(node(dataId));
  } else {
    return getLocalProperty<ColorProperty>("viewColor")->getEdgeValue(edge(dataId));
  }
}

string ParallelCoordinatesGraphProxy::getDataTexture(unsigned int dataId) {
  if (getDataLocation() == NODES) { 
    return getLocalProperty<StringProperty>("viewTexture")->getNodeValue(node(dataId));
  } else {
    return getLocalProperty<StringProperty>("viewTexture")->getEdgeValue(edge(dataId));
  }
}

list<string> ParallelCoordinatesGraphProxy::getAllProperties() {
  fillPropertiesVector();
  return propertiesList;
}

unsigned int ParallelCoordinatesGraphProxy::getDataMaxId() {
  unsigned int maxId = 0;
  if (dataLocation == NODES) {
    node n;
    forEach(n, getNodes()) {
      if (n.id > maxId)
	maxId = n.id;
    }
  } else {
    edge e;
    forEach(e, getEdges()) {
      if (e.id > maxId)
	maxId = e.id;
    }
  }
  return (maxId + 1);
}

bool ParallelCoordinatesGraphProxy::isDataSelected(unsigned int dataId) {
  if (getDataLocation() == NODES) {
    return  getLocalProperty<BooleanProperty>("viewSelection")->getNodeValue(node(dataId));
  } else {
    return  getLocalProperty<BooleanProperty>("viewSelection")->getEdgeValue(edge(dataId));
  }
}

void ParallelCoordinatesGraphProxy::setDataSelected(unsigned int dataId, bool dataSelected) {
  if (getDataLocation() == NODES) {
    getLocalProperty<BooleanProperty>("viewSelection")->setNodeValue(node(dataId), dataSelected);
  } else {
    getLocalProperty<BooleanProperty>("viewSelection")->setEdgeValue(edge(dataId), dataSelected);
  }
}

Size ParallelCoordinatesGraphProxy::getDataViewSize(unsigned int dataId) {
  if (getDataLocation() == NODES) {
    return getLocalProperty<SizeProperty>("viewSize")->getNodeValue(node(dataId));
  } else {
    return getLocalProperty<SizeProperty>("viewSize")->getEdgeValue(edge(dataId));
  }
}

void ParallelCoordinatesGraphProxy::pushObserver(GraphObserver *obs) {
  graph_component->addObserver(obs);
}

void ParallelCoordinatesGraphProxy::popObserver(GraphObserver *obs) {
  graph_component->removeObserver(obs);
}

void ParallelCoordinatesGraphProxy::fillVectorWithData(void *pVector, string property) {  
  if (existLocalProperty(property)) {
    string propertyTypeName = getProperty(property)->getTypename();
    if (getDataLocation() == NODES) {
      node n;
      forEach(n, getNodes()) {
	if (propertyTypeName == "string") { 
	  vector<string> *v = (vector<string> *) pVector;
	  (*v)[n.id] = getLocalProperty<StringProperty>(property)->getNodeValue(n);
	} else if (propertyTypeName == "int") {
	  vector<int> *v = (vector<int> *) pVector;
	  (*v)[n.id] = getLocalProperty<IntegerProperty>(property)->getNodeValue(n);
	} else if (propertyTypeName == "double") {
	  vector<double> *v = (vector<double> *) pVector;
	  (*v)[n.id] = getLocalProperty<DoubleProperty>(property)->getNodeValue(n);
	}
      }
    } else {
      edge e;
      forEach(e, getEdges()) {
	if (propertyTypeName == "string") { 
	  vector<string> *v = (vector<string> *) pVector;
	  (*v)[e.id] = getLocalProperty<StringProperty>(property)->getEdgeValue(e);
	} else if (propertyTypeName == "int") {
	  vector<int> *v = (vector<int> *) pVector;
	  (*v)[e.id] = getLocalProperty<IntegerProperty>(property)->getEdgeValue(e);
	} else if (propertyTypeName == "double") {
	  vector<double> *v = (vector<double> *) pVector;
	  (*v)[e.id] = getLocalProperty<DoubleProperty>(property)->getEdgeValue(e);
	}
      }
    }
  }
}

}
