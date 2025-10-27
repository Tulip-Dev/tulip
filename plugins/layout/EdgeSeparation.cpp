
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
    "The property holding the size of edges."};

class EdgeSeparation : public tlp::LayoutAlgorithm {
  vector<edge> multipleEdges;

public:
  PLUGININFORMATION("Multiple Edges Separation", "Tulip dev team", "16/11/2022",
                    "This plugin separates multiple edges existing between each pair of "
                    "nodes. Since, by default, multiple edges are drawn one on top of "
                    "the other, this plugin separates their drawing by adding bends.",
                    "1.0", "");

  EdgeSeparation(const tlp::PluginContext *context)
      // set second parameter of the constructor below to true because
      // result needs to be an inout parameter
      // in order to preserve the original values of non targeted elements
      : LayoutAlgorithm(context, true) {
    addInParameter<double>("gap", paramHelp[0], "0.5");
    addInParameter<SizeProperty>("edge size", paramHelp[1], "viewSize");

    // result needs to be an inout parameter
    // in order to preserve the original values of non targeted elements
    parameters.setDirection("result", INOUT_PARAM);
  }

  bool check(string &err) override {
    // get multiples edges
    SimpleTest::simpleTest(graph, &multipleEdges, nullptr, false);

    if (multipleEdges.empty()) {
      err = "The graph has no multiple edges between any pair of nodes.\nNothing to do.";
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
      dataSet->get("edge size", size);
    }

    int nbMultiples = multipleEdges.size();
    for (int i = 0; i < nbMultiples; ++i) {
      edge curMe = multipleEdges[i];
      // curMe can be invalid, if i is the offset
      // of an edge already treated.
      // cf the end of the loop
      if (curMe.isValid() == false)
        continue;
      auto ends = graph->ends(curMe);
      // do nothing if curMe is a loop
      if ((ends.first == ends.second))
        continue;
      // we get the coordinates of the current node and its opposite
      Coord sCoord = result->getNodeValue(ends.first);
      Coord tCoord = result->getNodeValue(ends.second);
      // we normalize this vector, so we will be able to use the scalar product
      // to determine the ways the edges will go
      Coord vect = sCoord - tCoord;
      vect /= vect.norm();
      Coord normal(-vect.getY(), vect.getX(), vect.getZ());

      // initialize how far from the original edge the edges will go
      const tlp::Size &s = size->getEdgeValue(curMe);
      float distance = s.getW() + gap;
      float xPlus = distance;
      float xMinus = -distance;

      auto edges = graph->getEdges(ends.first, ends.second, false);
      // for each edge linking these two ends nodes, we separate the edges
      for (auto e : edges) {
        bool isInEdge = graph->target(e) == ends.first;
        // if this edge is an in edge, it will go "up"
        // else it will go "down" of the current edge.
        float x = isInEdge ? xPlus : xMinus;
        std::vector<Coord> bends(2);

        // we add control points, or waypoints to the edge at 1/4 of its length
        // and another one at 3/4 (so we will see more clearly then
        // if we used only one control point) this is prettier when
        // using bezier curves
        Coord cp1_4(sCoord + (tCoord - sCoord) / 4.0f + normal * x);
        Coord cp3_4(sCoord + (tCoord - sCoord) * 3.0f / 4.0f + normal * x);

        // depending which kind of edge we encountered, we increment the distance the next edge
        // will be from the original edge.
        if (isInEdge)
          xPlus += 0.5;
        else
          xMinus -= 0.5;

        // we add the control points in the right order
        // so that we don't get an edge going to
        // the 3/4 waypoint, then the 1/4 waypoint, then to the opposite node.
        if (isInEdge) {
          bends[0] = cp3_4;
          bends[1] = cp1_4;
        } else {
          bends[0] = cp1_4;
          bends[1] = cp3_4;
        }
        // now we can finally set the value on the edge
        result->setEdgeValue(e, bends);
        if (e != curMe) {
          // as e can be found in multipleEdges
          // replace it by an invalid edge
          for (int j = i + 1; j < nbMultiples; ++j)
            if (e == multipleEdges[j]) {
              multipleEdges[j] = edge();
              break;
            }
        }
      }
    }
    return true;
  }
};

PLUGIN(EdgeSeparation)
