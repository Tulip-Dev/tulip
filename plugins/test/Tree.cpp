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

#include <tulip/TreeTest.h>
#include <tulip/GraphTest.h>
#include <tulip/GraphTools.h>
#include <tulip/BooleanProperty.h>

class DirectedTreeTest : public tlp::GraphTest {
public:
  PLUGININFORMATION("Directed Tree", "Tulip team", "18/04/2012", "Tests whether a graph is a directed tree or not.", "1.0", "Topological Test")
  DirectedTreeTest(const tlp::PluginContext* context) : tlp::GraphTest(context) {
  }

  virtual bool test() {
    return tlp::TreeTest::isTree(graph);
  }

};
PLUGIN(DirectedTreeTest)

class FreeTreeTest : public tlp::GraphTest {
public:
  PLUGININFORMATION("Free Tree", "Tulip team", "18/04/2012", "Tests whether a graph is a free tree or not.", "1.0", "Topological Test")
  FreeTreeTest(const tlp::PluginContext* context) : tlp::GraphTest(context) {
  }

  virtual bool test() {
    return tlp::TreeTest::isFreeTree(graph);
  }

};
PLUGIN(FreeTreeTest)

class MakeDirectedTree : public tlp::Algorithm {
public:
  PLUGININFORMATION("Make Directed Tree", "Tulip team", "18/04/2012", "Makes a graph a directed tree.", "1.0", "Topology Update")
  MakeDirectedTree(const tlp::PluginContext* context) : tlp::Algorithm(context) {
  }

  tlp::node root;

  virtual bool check(std::string& errorMsg) {
    if (!tlp::TreeTest::isFreeTree(graph)) {
      errorMsg = "The graph is not topologically a tree";
      return false;
    }

    tlp::node n;
    forEach(n, graph->getProperty<tlp::BooleanProperty>("viewSelection")->getNodesEqualTo(true)) {
      if (root.isValid()) {
        tlp::error() << "Only one root node must be selected." << std::endl;
        return false;
      }

      root = n;
    }

    if (!root.isValid())
      root = graphCenterHeuristic(graph);

    return true;
  }

  virtual bool run() {
    tlp::TreeTest::makeRootedTree(graph, root);
    return true;
  }

};
PLUGIN(MakeDirectedTree)
