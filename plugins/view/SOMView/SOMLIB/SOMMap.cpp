/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include "SOMMap.h"
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/GlyphManager.h>
#include <tulip/ForEach.h>
#include <tulip/tulipconf.h>
#include <tulip/StringCollection.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

/*SOMMap::SOMMap(Graph* root, unsigned int width, unsigned int height) :
 tlp::GraphDecorator(root), width(width), height(height) {
 }*/

SOMMap::SOMMap(Graph* root, unsigned int width, unsigned int height,
               SOMMapConnectivity connectivity, bool oppositeConnected) :
  tlp::GraphDecorator(root), width(width), height(height), connectivity(connectivity),
  oppositeConnected(oppositeConnected), graphCreated(false) {
  initMap();
}
SOMMap::SOMMap(unsigned int width, unsigned int height, SOMMapConnectivity connectivity,
               bool oppositeConnected) :
  tlp::GraphDecorator(newGraph()), width(width), height(height), connectivity(connectivity),
  oppositeConnected(oppositeConnected), graphCreated(true) {
  initMap();
}

void SOMMap::initMap() {
  //Map creation
  //If width and height are valid create a new grid
  if(width != 0 && height != 0) {
    DataSet gridDataSet;
    gridDataSet.set("width", width);
    gridDataSet.set("height", height);
    //Node Connectivity
    StringCollection connectivityCollection;

    switch (connectivity) {
    case four:
      connectivityCollection.push_back("4");
      connectivityCollection.setCurrent("4");
      break;

    case six:
      connectivityCollection.push_back("6");
      connectivityCollection.setCurrent("6");
      break;

    case eight:
      connectivityCollection.push_back("8");
      connectivityCollection.setCurrent("8");
      break;

    default:
      std::cerr << __PRETTY_FUNCTION__ << ":" << __LINE__ << " " << "connectivity unknown"
                << std::endl;
      assert(false);
    }

    gridDataSet .set("connectivity", connectivityCollection);
    gridDataSet.set("oppositeNodesConnected", oppositeConnected);
    //Suppress spacing
    gridDataSet.set("spacing", (double) 0.0);
    graph_component = importGraph("Grid", gridDataSet, NULL, graph_component);
  }

  assert(graph_component != NULL);
  IntegerProperty *nodeShape = graph_component->getProperty<IntegerProperty> ("viewShape");

  switch (connectivity) {
  case four:
  case eight:
    nodeShape->setAllNodeValue(NodeShape::Square);
    break;

  case six:
    nodeShape->setAllNodeValue(NodeShape::Hexagon);
    break;
  }

}

SOMMap::~SOMMap() {
  graph_component->clear();

  if (graphCreated) {
    delete graph_component;
  }
}

DynamicVector<double>& SOMMap::getWeight(tlp::node n) {
  return nodeToNodeVec[n];
}
const DynamicVector<double> SOMMap::getWeight(const tlp::node& n) const {

  std::map<tlp::node, DynamicVector<double> >::const_iterator it = nodeToNodeVec.find(n);

  if (it != nodeToNodeVec.end()) {
    return (*it).second;
  }
  else
    return DynamicVector<double> ();

}

void SOMMap::setWeight(tlp::node n, const DynamicVector<double>& weight) {
  nodeToNodeVec[n] = weight;
}

void SOMMap::registerModification(const vector<string>&propertiesToListen) {

  //Get all properties
  vector<PropertyInterface*> properties;

  for (vector<string>::const_iterator it = propertiesToListen.begin(); it
       != propertiesToListen.end(); ++it) {
    //If the properties don't exist create it
    if (!existProperty((*it))) {
#ifndef NDEBUG
      std::cerr << __PRETTY_FUNCTION__ << ":" << __LINE__ << " "
                << "Property doesn't exist creating new" << std::endl;
#endif
      DoubleProperty *property = new DoubleProperty(this);
      addLocalProperty((*it), property);
      properties.push_back(property);
    }
    else
      properties.push_back(getProperty((*it)));
  }

  //Store all the value from the DynamicVectors in the properties
  tlp::node n;
  forEach(n,getNodes()) {
    assert(propertiesToListen.size()== nodeToNodeVec[n].getSize());

    for (unsigned int propertyNumber = 0; propertyNumber < properties.size(); ++propertyNumber) {

      //If the property is double no need to convert
      if (properties[propertyNumber]->getTypename().compare("double") == 0) {
        ((DoubleProperty*) properties[propertyNumber])->setNodeValue(n,
            nodeToNodeVec[n][propertyNumber]);
      }
      else {
        std::cerr << __PRETTY_FUNCTION__ << ":" << __LINE__
                  << " unmanaged type " << properties[propertyNumber]->getTypename()
                  << std::endl;
        assert(false);
      }

    }
  }
}
tlp::node SOMMap::getNodeAt(unsigned int pos) {
  return getNodeAt(pos % height, (unsigned int) floor(pos / height));

}
tlp::node SOMMap::getNodeAt(unsigned int x, unsigned int y) {
  //assert(x<width && y < height);
  if (x >= width || y >= height)
    return node();

  unsigned int currenty = 0;
  unsigned int currentx = 0;
  //Get first node
  node n;
  Iterator<node> *it = graph_component->getNodes();
  n = it->next();
  delete it;

  //Move on the line
  while (currenty != y) {
    n = graph_component->getOutNode(n, 2);
    ++currenty;
  }

  while (currentx != x) {
    n = graph_component->getOutNode(n, 1);
    ++currentx;
  }

  return n;
}

bool SOMMap::getPosForNode(tlp::node n, unsigned int &x, unsigned int &y) {
  //assert(n.isValid() && graph_component->isElement(n));
  if (!n.isValid() || !graph_component->isElement(n))
    return false;

  x = n.id % width;
  y = n.id / width;
  return true;
}
