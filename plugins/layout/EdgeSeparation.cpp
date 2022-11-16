
#include <tulip/LayoutProperty.h>
#include <tulip/SimpleTest.h>
#include <tulip/SizeProperty.h>
#include <tulip/StaticProperty.h>
#include <stdexcept>

using namespace tlp;
using namespace std;

static const char *paramHelp[] = {
    // gap
    "The gap between multiple edges.",
    // size
    "The property hodling the size of nodes."};

class EdgeSeparation : public tlp::LayoutAlgorithm {
public:
  PLUGININFORMATION(
      "Edge Separation", "Tulip dev team", "16/11/2022",
      "This plugin separates multiple edges between two nodes. By default, Tulip draw them one on top of the others. "
      " This plugin separates edges by adding bends.",
      "1.0", "");

  EdgeSeparation(const tlp::PluginContext *context) : LayoutAlgorithm(context) {
    addInParameter<double>("gap", paramHelp[0], "0.5");
    addInParameter<SizeProperty>("Size", paramHelp[1], "viewSize");

    // result needs to be an inout parameter
    // in order to preserve the original values of non targeted elements
    parameters.setDirection("result", INOUT_PARAM);
  }

  bool check(string &err) override {
    if (!SimpleTest::hasParallelEdges(graph, false)) {
      err = "The graph has no multiple edges between any pair of nodes. Nothing to do.";
      return false;
    }

    return true;
  }

  bool run() override {
    double gap = 0.5;
    SizeProperty *size = graph->getProperty<SizeProperty>("viewSize");
    pluginProgress->showPreview(false);
    if (dataSet != nullptr) {
      dataSet->get("gap", gap);
      dataSet->get("Size", size);
    }

    result->setAllEdgeValue(vector<Coord>());
    NodeStaticProperty<bool> treatedNodes(graph);
    treatedNodes.setAll(false);

    // Iterate over all nodes
    for (const auto currentNode : graph->nodes()) {
      int step = 0, max_step = graph->numberOfNodes();

      // if this node has not been treated before we proceed
      if (treatedNodes[currentNode] == false) {

        if ((++step % 100) == 0) {
          ProgressState state = pluginProgress->progress(step, max_step);

          if (state != TLP_CONTINUE)
            return state != TLP_CANCEL;
        }
        // mark this node as treated
        treatedNodes[currentNode] = true;

        // this will hold the subset of edges connected to a node we are interested in.
        // as key we have the nodes that are linked to the current node by at least one edge
        // as value we have the set of edges linking the current node and the key node
        map<node, set<edge>> nodesEdges;

        // we iterate over all the edges of this node
        for (auto currentEdge : graph->getInOutEdges(currentNode)) {
          // we get the node at the other end of the current edge
          const node opposite = graph->opposite(currentEdge, currentNode);
          if (treatedNodes[opposite] == true) {
            break;
          }

          // check whether a set has already been inserted, if not insert it.
          if (nodesEdges.find(opposite) == nodesEdges.end()) {
            std::set<edge> temp;
            nodesEdges.insert(pair<node, std::set<edge>>(opposite, temp));
          }
          // add this edge to the list of edges linked to this node
          nodesEdges.at(opposite).insert(currentEdge);
        }

        // now we look at each of these edges
        for (const auto &mapIterator : nodesEdges) {
          // we get the coordinates of the current node and its opposite
          Coord currentNodecoord = result->getNodeValue(currentNode);
          Coord oppositeNodecoord = result->getNodeValue(mapIterator.first);

          // we normalize this vector, so we will be able to use the scalar product to determine the
          // ways the edges will go
          Coord vector = currentNodecoord - oppositeNodecoord;
          vector /= vector.norm();
          Coord normal(-vector.getY(), vector.getX(), vector.getY());

          // initialize how far from the original edge the edges will go
          const tlp::Size &s = size->getEdgeValue(*mapIterator.second.begin());

          float distance = s.getW() + gap;
          float xPlus = distance;
          float xMinus = -distance;

          // do nothing for one edge
          if (mapIterator.second.size() == 1)
            continue;
          // for each edge linking these two nodes (the current node and the current key of the
          // nodesEdges map), we separate the edges
          for (const auto &processingEdge : mapIterator.second) {
            bool isInEdge = graph->target(processingEdge) == mapIterator.first;
            // if this edge is an in edge, it will go "up", else it will go "down" of the current
            // edge.
            float x = isInEdge ? xPlus : xMinus;
            std::vector<Coord> newEdge;

            // we add control points, or waypoints to the edge at 1/4 of its length, and another one
            // at 3/4 (so we will see more clearly than if we used only one control point) also, this
            // is way prettier when using bezier curves :)
            Coord quarterControlPoint(currentNodecoord +
                                      (oppositeNodecoord - currentNodecoord) / 4.0f + normal * x);
            Coord threeQuartersControlPoint(currentNodecoord +
                                            (oppositeNodecoord - currentNodecoord) * 3.0f / 4.0f +
                                            normal * x);

            // depending which kind of edge we encountered, we increment the distance the next edge
            // will be from the original edge.
            if (isInEdge)
              xPlus += 0.5;
            else
              xMinus -= 0.5;

            // we add the control points in the right order, so that we don't get an adge going to
            // the 3/4 waypoint, then the 1/4 waypoint, then to the opposite node.
            if (currentNode == graph->source(processingEdge)) {
              newEdge.push_back(quarterControlPoint);
              newEdge.push_back(threeQuartersControlPoint);
            } else {
              newEdge.push_back(threeQuartersControlPoint);
              newEdge.push_back(quarterControlPoint);
            }

            // now we can finally set the value on the edge.
            result->setEdgeValue(processingEdge, newEdge);
          }
        }
      }
    }

    return true;
  }
};

PLUGIN(EdgeSeparation)
