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

#include <tulip/TreeTest.h>
#include <tulip/GraphTest.h>
#include <tulip/BooleanProperty.h>

class DirectedTreeTest : public tlp::GraphTest {
public:
  PLUGININFORMATIONS("Simple Tree Test", "Tulip team", "18/04/2012", "Tests whether a graph is a simple tree or not.", "1.0", "Tree")
  DirectedTreeTest(const tlp::PluginContext* context) : tlp::GraphTest(context) {
  }
  
  virtual bool run() {
    return tlp::TreeTest::isTree(graph);
  }
  
};
PLUGIN(DirectedTreeTest)

class FreeTreeTest : public tlp::GraphTest {
public:
  PLUGININFORMATIONS("Free Tree Test", "Tulip team", "18/04/2012", "Tests whether a graph is a free tree or not.", "1.0", "Tree")
  FreeTreeTest(const tlp::PluginContext* context) : tlp::GraphTest(context) {
  }
  
  virtual bool run() {
    return tlp::TreeTest::isFreeTree(graph);
  }
  
};
PLUGIN(FreeTreeTest)

class MakeRootedTree : public tlp::Algorithm {
public:
  PLUGININFORMATIONS("Make Rooted Tree", "Tulip team", "18/04/2012", "Makes a graph a rooted tree.", "1.0", "Tree")
  MakeRootedTree(const tlp::PluginContext* context) : tlp::Algorithm(context) {
  }
  
  virtual bool run() {
    
    tlp::BooleanProperty* selection = graph->getProperty<tlp::BooleanProperty>("viewSelection");
    tlp::Iterator<tlp::node>* it = selection->getNodesEqualTo(true);
    tlp::node root = it->next();
    if(it->hasNext())
      return false;
    
    tlp::TreeTest::makeRootedTree(graph, root);
    return true;
  }
  
};
PLUGIN(MakeRootedTree)
