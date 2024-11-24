/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
 *
 * Authors: Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * This plug-in is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * This plug-in is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#include <tulip/TulipException.h>
#include <tulip/GraphTools.h>
#include <tulip/DrawingTools.h>
#include <tulip/StableIterator.h>
#include <tulip/MutableContainer.h>
#include <tulip/LayoutProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/DrawingTools.h>
#include <tulip/Rectangle.h>
#include <tulip/SimpleTest.h>
#include <tulip/ParallelTools.h>

#include "EdgeBundling.h"
#include "QuadTree.h"
#include "OctreeBundle.h"
#include "Dijkstra.h"
#include "BendsTools.h"
#include "SphereUtils.h"

#include <sstream>

using namespace std;
using namespace tlp;

//============================================

static const char *paramHelp[] = {
    // initial layout
    "The input layout of the graph.",

    // size
    "The input node sizes.",

    // grid graph
    "If true, a subgraph corresponding to the grid used for routing edges will be added.",

    // 3D layout
    "If true, a 3D input layout is assumed and 3D edge bundling "
    "will be performed. Warning: the generated grid graph will be much bigger "
    "and the algorithm execution time will be slower compared to the 2D case.",

    // sphere layout
    "If true, a spherical layout of the nodes is assumed. "
    "Edges will be then routed along the sphere surface.",

    // long edges
    "indicates how long edges will be routed. A value less than 1.0 "
    "will promote paths outside dense regions of the input graph drawing.",

    // split ratio
    "indicates the granularity of the grid that will be generated for "
    "routing edges. The higher its value, the more precise the grid is.",

    // iterations
    "gives the number of iterations of the edge bundling process. "
    "The higher its value, the more edges will be bundled.",

    // max thread
    "gives the number of threads to use for speeding up the edge bundling "
    "process. "
    "A value of 0 will use as much threads as processors on the host machine.",

    // edge node overlap
    "If true, edges can be routed on original nodes."};

//============================================
EdgeBundling::EdgeBundling(const PluginContext *context) : Algorithm(context) {
  addInParameter<LayoutProperty>("initial layout", paramHelp[0], "viewLayout");
  addInParameter<SizeProperty>("node size", paramHelp[1], "viewSize");
  addInParameter<bool>("grid graph", paramHelp[2], "false");
  addInParameter<bool>("3D layout", paramHelp[3], "false");
  addInParameter<bool>("sphere layout", paramHelp[4], "false");
  addInParameter<double>("long edges", paramHelp[5], "0.9");
  addInParameter<double>("split ratio", paramHelp[6], "10");
  addInParameter<unsigned int>("iterations", paramHelp[7], "2");
  addInParameter<unsigned int>("max thread", paramHelp[8], "0");
  addInParameter<bool>("edge node overlap", paramHelp[9], "false");
  addDependency("Voronoi diagram", "1.1");
}
//============================================
class SortNodes {
public:
  static NodeStaticProperty<double> *dist;
  bool operator()(const node a, const node b) const { // sort in deceresing order;
    double da, db;

    if ((da = dist->getNodeValue(a)) == (db = dist->getNodeValue(b)))
      return a.id > b.id;

    return da > db;
  }
};
NodeStaticProperty<double> *SortNodes::dist = nullptr;
//============================================
void updateLayout(node src, edge e, Graph *graph, LayoutProperty *layout,
                  const vector<node> &nBends, bool layout3D) {
  if (nBends.size() < 3)
    return;

  // if source and target nodes are at the same position, don't set bends to avoid visual artifacts
  // when rendering the graph
  if (layout->getNodeValue(nBends.front()).dist(layout->getNodeValue(nBends.back())) < 1e-5) {
    return;
  }

  vector<Coord> bends(nBends.size() - 2);
  bool sens = true;
  int i = 1;

  if (graph->source(e) == src) {
    i = nBends.size() - 2;
    sens = false;
  }

  for (unsigned int j = 0; j < bends.size(); ++j) {
    const Coord &coord = layout->getNodeValue(nBends[i]);

    if (!layout3D)
      (bends[j] = coord)[2] = 0;
    else
      bends[j] = coord;

    if (sens)
      ++i;
    else
      --i;
  }

  TLP_LOCK_SECTION(LAYOUT) {
    layout->setEdgeValue(e, bends);
  }
  TLP_UNLOCK_SECTION(LAYOUT);
}
//============================================
// fix all graph edge to 1 and all grid edge to 0 graph-grid edge 2 edge on the contour of a node 3
void EdgeBundling::fixEdgeType(EdgeStaticProperty<unsigned int> &ntype) {
  TLP_PARALLEL_MAP_EDGES_AND_INDICES(graph, [&](edge e, unsigned int i) {
    if (oriGraph->isElement(e)) {
      ntype[i] = 1;
    } else {
      auto ends = graph->ends(e);

      if (oriGraph->isElement(ends.first) || oriGraph->isElement(ends.second))
        ntype[i] = 2;
      else
        ntype[i] = 0;
    }
  });
}
//============================================
static void computeDik(Dijkstra &dijkstra, const Graph *const vertexCoverGraph,
                       const Graph *const oriGraph, const node n,
                       const EdgeStaticProperty<double> &mWeights, unsigned int optimizatioLevel) {
  set<node> focus;

  if (optimizatioLevel > 0) {
    for (auto ni : vertexCoverGraph->getInOutNodes(n))
      focus.insert(ni);
  }

  dijkstra.initDijkstra(oriGraph, n, mWeights, focus);
}
//==========================================================================
void EdgeBundling::computeDistances() {
  TLP_PARALLEL_MAP_NODES_AND_INDICES(oriGraph,
                                     [&](node n, unsigned int i) { computeDistance(n, i); });
}
//==========================================================================
void EdgeBundling::computeDistance(node n, unsigned int i) {
  double maxDist = 0;
  const Coord &nPos = layout->getNodeValue(n);
  for (auto n2 : vertexCoverGraph->getInOutNodes(n)) {
    double dist = (nPos - layout->getNodeValue(n2)).norm();
    maxDist += dist;
  }
  if (i != UINT_MAX)
    (*SortNodes::dist)[i] = maxDist;
  else
    (*SortNodes::dist)[n] = maxDist;
}
//============================================

bool EdgeBundling::run() {

  // no edges => do nothing
  if (graph->numberOfEdges() == 0)
    return true;

  optimizationLevel = 3;
  maxThread = 0;
  unsigned int MAX_ITER = 2;
  edgeNodeOverlap = false;
  longEdges = 0.9;
  splitRatio = 10;
  layout3D = false;
  bool sphereLayout = false;
  bool keepGrid = false;
  double dist = 50.0;

  SizeProperty *size = graph->getProperty<SizeProperty>("viewSize");
  layout = graph->getProperty<LayoutProperty>("viewLayout");

  if (dataSet != nullptr) {
    dataSet->get("long edges", longEdges);
    dataSet->get("split ratio", splitRatio);
    dataSet->get("iterations", MAX_ITER);
    dataSet->get("optimization", optimizationLevel);
    dataSet->get("edge node overlap", edgeNodeOverlap);
    dataSet->get("max thread", maxThread);
    dataSet->get("3D layout", layout3D);
    dataSet->get("grid graph", keepGrid);
    dataSet->get("sphere layout", sphereLayout);
    dataSet->get("initial layout", layout);
    dataSet->get("node size", size);
  }

  if (sphereLayout) {
    layout3D = true;
  }

  if (!layout3D) {
    // forbid edge bundling execution if the input layout is in 3D
    // and it has not been explicitly asked to use the 3D version
    // of the algorithm.
    auto lMin = layout->getMin(graph);
    auto lMax = layout->getMax(graph);
    if (lMin.z() != lMax.z()) {
      pluginProgress->setError("A 3D input layout has been detected while the default behavior "
                               "is to consider a 2D input layout. "
                               "You must set the \"3D_layout\" parameter to "
                               "\"true\" to explicitly use 3D edge bundling.");
      return false;
    }
  }

  string err;
  oriGraph = graph->addCloneSubGraph("Original Graph");

  // Make the graph simple
  std::vector<tlp::edge> removedEdges;
  tlp::SimpleTest::makeSimple(oriGraph, removedEdges);

  // we need to registered the graph nodes having the same position
  // in the same vector
  std::vector<std::vector<tlp::node>> samePositionNodes;

  try {
    // Grid graph computation first step : generate quad-tree/octree
    if (layout3D) {
      OctreeBundle::compute(graph, splitRatio, layout, size);
      // delete edges in reverse order to avoid
      // the use of a stable iterator
      const vector<edge> &edges = graph->edges();
      unsigned int sz = edges.size();
      while (sz) {
        auto e = edges[--sz];
        if (oriGraph->isElement(e))
          continue;

        graph->delEdge(e);
      }

      if (sphereLayout) {
        centerOnOriginAndScale(graph, layout, dist * 2.);
        addSphereGraph(graph, dist + 0.5 * dist);
        addSphereGraph(graph, dist - 0.2 * dist);
      }
    } else {
      // Preprocess the graph to ensure that two nodes does not have the same position
      // otherwise the quad tree computation will fail
      // clone graph
      Graph *workGraph = graph->addCloneSubGraph();
      // we use a hash map to ease the retrieve of the vector of the nodes
      // having the same position
      tlp_hash_map<std::string, std::pair<node, unsigned int>> clusters;

      // iterate on graph nodes
      for (auto n : graph->nodes()) {
        // get position
        const Coord &coord = layout->getNodeValue(n);
        // compute a key for coord (convert point to string representation)
        // Warning: because of float precision issues, we use a string key
        // instead of relying on the x, y exact values
        std::string key = tlp::PointType::toString(coord);

        tlp_hash_map<std::string, std::pair<node, unsigned int>>::iterator it = clusters.find(key);

        if (it == clusters.end())
          // register the first node at position represented by key
          clusters[key] = std::make_pair(n, UINT_MAX);
        else {
          std::pair<node, unsigned int> &infos = it->second;

          if (infos.second == UINT_MAX) {
            // we find a second node at the position represented by key
            // so it is time to create a vector to registered them
            std::vector<node> nodes(1, infos.first);
            samePositionNodes.push_back(nodes);
            infos.second = samePositionNodes.size() - 1;
          }

          // registered the current node in the vector of the nodes
          // having the same position
          samePositionNodes[infos.second].push_back(n);
          // delete it from the clone subgraph
          workGraph->delNode(n);
        }
      }

      // Execute the quad tree computation on the cleaned subgraph
      QuadTreeBundle::compute(workGraph, splitRatio, layout, size);
      // workGraph is no longer needed
      graph->delSubGraph(workGraph);
    }
  } catch (tlp::TulipException &e) {
    string errorMsg(e.what());
    pluginProgress->setError(errorMsg);
    return false;
  }

  // Grid graph computation second step : generate a voronoi diagram
  // from the original nodes positions and the centers of the previoulsy
  // computed quad-tree/octree cells
  tlp::DataSet voroDataSet;
  voroDataSet.set("connect", true);
  voroDataSet.set("original clone", false);

  if (!graph->applyAlgorithm("Voronoi diagram", err, &voroDataSet)) {
    pluginProgress->setError("'Voronoi diagram' failed");
    return false;
  }

  // If sphere mode, remove the grid nodes inside the sphere
  // as we only want to route on the sphere surface
  if (sphereLayout) {
    // delete nodes in reverse order to avoid
    // the use of a stable iterator
    const vector<node> &nodes = graph->nodes();
    unsigned int sz = nodes.size();
    while (sz) {
      auto n = nodes[--sz];
      if (oriGraph->isElement(n))
        continue;

      const Coord &c = layout->getNodeValue(n);

      if (c.norm() < 0.9 * dist) {
        graph->delNode(n, true);
      }
    }
  }

  if (maxThread == 0) {
    ThreadManager::setNumberOfThreads(ThreadManager::getNumberOfProcs());
  } else {
    ThreadManager::setNumberOfThreads(maxThread);
  }

  EdgeStaticProperty<unsigned int> ntype(graph);
  fixEdgeType(ntype);

  //==========================================================
  // get the freshly created Voronoi subgraph,
  // this should be the last one in the list but we prefer
  // to iterate on that latter in reverse order in case the
  // Voronoi plugin implementation changes
  for (auto i = graph->numberOfSubGraphs(); i > 0; --i) {
    auto sg = graph->getNthSubGraph(i - 1);
    if (sg->getName() == "Voronoi") {
      gridGraph = sg;
      break;
    }
  }
  gridGraph->setName("Grid Graph");
  // remove all original graph edges
  TLP_PARALLEL_MAP_EDGES_AND_INDICES(graph, [&](edge e, unsigned int i) {
    if (ntype[i] == 1 && gridGraph->isElement(e)) {
      gridGraph->delEdge(e);
    }
  });

  // If there was nodes at the same position, the voronoi diagram process
  // only considers one of them when connecting original graph nodes to
  // their enclosing cell vertices.
  // So connect the other ones to the enclosing cell vertices too
  // in order for the shortest path computation to work
  for (size_t i = 0; i < samePositionNodes.size(); ++i) {
    tlp::node rep;

    // get the nodes that has been connected to the voronoi cell vertices
    for (size_t j = 0; j < samePositionNodes[i].size(); ++j) {
      if (gridGraph->deg(samePositionNodes[i][j]) > 0) {
        rep = samePositionNodes[i][j];
        break;
      }
    }

    // connect the other nodes to the enclosing voronoi cell vertices
    // Warning: because no edge is added to the current node
    // we can use a basic iteration instead of a stable one
    for (auto n : gridGraph->getOutNodes(rep)) {
      for (size_t j = 0; j < samePositionNodes[i].size(); ++j) {
        if (samePositionNodes[i][j] == rep)
          continue;

        tlp::edge e = gridGraph->addEdge(samePositionNodes[i][j], n);
        ntype.addEdgeValue(e, 2);
      }
    }
  }

  // Initialization of grid edges weights
  //==========================================================
  EdgeStaticProperty<double> mWeights(graph);
  EdgeStaticProperty<double> mWeightsInit(graph);
  TLP_PARALLEL_MAP_EDGES_AND_INDICES(graph, [&](edge e, unsigned int i) {
    auto ends = graph->ends(e);
    const Coord &a = layout->getNodeValue(ends.first);
    const Coord &b = layout->getNodeValue(ends.second);
    double abNorm = (a - b).norm();
    double initialWeight = pow(abNorm, longEdges);

    if (ntype[i] == 2 && !edgeNodeOverlap)
      initialWeight = abNorm;

    mWeights[i] = mWeightsInit[i] = initialWeight;
  });

  //==========================================================

  EdgeStaticProperty<unsigned int> depth(graph);

  // Load the grid graph into an optimized structure for graph traversal
  Dijkstra::loadGraph(gridGraph);

  // Routing edges into bundles
  for (unsigned int iteration = 0; iteration < MAX_ITER; iteration++) {

    if (iteration < MAX_ITER - 1) {
      depth.setAll(0);
    }

    // used for optimizing the vertex cover problem
    vertexCoverGraph = oriGraph->addCloneSubGraph("vertexCoverGraph");

    MutableContainer<bool> edgeTreated;
    edgeTreated.setAll(false);
    NodeStaticProperty<double> distance(oriGraph);
    SortNodes::dist = &distance;
    computeDistances();
    set<node, SortNodes> orderedNodes;
    {
      for (auto n : vertexCoverGraph->nodes())
        orderedNodes.insert(n);
    }

    while (vertexCoverGraph->isEmpty() == false) {
      stringstream strm;
      strm << "Computing iteration " << iteration + 1 << "/" << MAX_ITER;
      pluginProgress->setComment(strm.str());
      unsigned int i = oriGraph->numberOfEdges() - vertexCoverGraph->numberOfEdges();

      if (((i % 10) == 0) &&
          (pluginProgress->progress(i, oriGraph->numberOfEdges()) != TLP_CONTINUE)) {
        oriGraph->delSubGraph(vertexCoverGraph);
        return pluginProgress->state() != TLP_CANCEL;
      }

      //====================================
      // Select the destination nodes. We do not have to compute
      // dijkstra for other nodes.
      vector<node> toTreatByThreads;
      set<node> blockNodes;
      vector<node> toDelete;

      for (auto n : orderedNodes) {
        if ((blockNodes.find(n) == blockNodes.end() || optimizationLevel < 3) &&
            (vertexCoverGraph->deg(n) > 0 || optimizationLevel < 2)) {
          bool addOk = true;

          if (vertexCoverGraph->deg(n) == 1 && optimizationLevel > 1) {
            Iterator<node> *it = vertexCoverGraph->getInOutNodes(n);
            node tmp = it->next();
            delete it;

            if (vertexCoverGraph->deg(tmp) != 1) {
              addOk = false;
            }
          }

          if (addOk) {
            toTreatByThreads.push_back(n);

            if ((optimizationLevel == 3) &&
                (toTreatByThreads.size() < ThreadManager::getNumberOfThreads())) {
              for (auto tmp : vertexCoverGraph->getInOutNodes(n))
                blockNodes.insert(tmp);
            }
          }
        }

        if (vertexCoverGraph->deg(n) == 0 && optimizationLevel > 1) {
          toDelete.push_back(n);
        }

        if (toTreatByThreads.size() >= ThreadManager::getNumberOfThreads())
          break;
      }

      if (optimizationLevel > 1)
        for (auto n : toDelete) {
          orderedNodes.erase(n);
          vertexCoverGraph->delNode(n);
        }

      forceEdgeTest = false;

      int nbThreads = toTreatByThreads.size();

      if (iteration < MAX_ITER - 1) {
        TLP_PARALLEL_MAP_INDICES(nbThreads, [&](unsigned int j) {
          node n = toTreatByThreads[j];
          Dijkstra dijkstra;

          if (edgeNodeOverlap)
            computeDik(dijkstra, vertexCoverGraph, nullptr, n, mWeights, optimizationLevel);
          else
            computeDik(dijkstra, vertexCoverGraph, oriGraph, n, mWeights, optimizationLevel);

          // for each edge of n compute the shortest paths in the grid
          for (auto e : vertexCoverGraph->getInOutEdges(n)) {
            node n2 = graph->opposite(e, n);

            if (optimizationLevel < 3 || forceEdgeTest) {
              bool stop = false;
              // when we are not using coloration edge can be treated two times
              TLP_LOCK_SECTION(EDGETREATED) {
                if (edgeTreated.get(e.id))
                  stop = true;

                edgeTreated.set(e.id, true);
              }
              TLP_UNLOCK_SECTION(EDGETREATED);

              if (stop) {
                continue;
              }
            }

            dijkstra.searchPaths(n2, depth);
          }
        });
      } else {
        TLP_PARALLEL_MAP_INDICES(nbThreads, [&](unsigned int j) {
          node n = toTreatByThreads[j];
          Dijkstra dijkstra;

          if (edgeNodeOverlap)
            computeDik(dijkstra, vertexCoverGraph, nullptr, n, mWeights, optimizationLevel);
          else
            computeDik(dijkstra, vertexCoverGraph, oriGraph, n, mWeights, optimizationLevel);

          // for each edge of n compute the shortest paths in the grid
          for (auto e : vertexCoverGraph->getInOutEdges(n)) {
            if (optimizationLevel < 3 || forceEdgeTest) {
              bool stop = false;
              // when we are not using colration edge can be treated two times
              TLP_LOCK_SECTION(EDGETREATED) {
                if (edgeTreated.get(e.id))
                  stop = true;

                edgeTreated.set(e.id, true);
              }
              TLP_UNLOCK_SECTION(EDGETREATED);

              if (stop)
                continue;
            }

            {
              /// bends
              vector<node> tmpV;
              dijkstra.searchPath(graph->opposite(e, n), tmpV);

              if (!layout3D)
                tmpV = BendsTools::bendsSimplification(tmpV, layout);

              updateLayout(n, e, graph, layout, tmpV, layout3D);
            }
          }
        });
      }

      for (auto n : toTreatByThreads) {
        vector<node> neighbors;
        for (auto n2 : vertexCoverGraph->getInOutNodes(n)) {
          neighbors.push_back(n2);
          orderedNodes.erase(n2);
        }
        orderedNodes.erase(n);
        vertexCoverGraph->delNode(n);

        for (auto n2 : neighbors) {
          computeDistance(n2);
          orderedNodes.insert(n2);
        }
      }
    }

    oriGraph->delSubGraph(vertexCoverGraph);

    // Adjust weights of routing grid.
    if (iteration < MAX_ITER - 1) {
      TLP_PARALLEL_MAP_EDGES(gridGraph, [&](edge e) {
        auto ePos = graph->edgePos(e);

        if (ntype.getEdgeValue(e) == 2 && !edgeNodeOverlap) {
          mWeights[ePos] = mWeightsInit[ePos];
        } else {
          // double avgdepth = weightFactor * depth.getEdgeValue(e) + 1.;
          double avgdepth = depth.getEdgeValue(e);

          if (avgdepth > 0)
            mWeights[ePos] = mWeightsInit[ePos] / (log(avgdepth) + 1);
          else
            mWeights[ePos] = mWeightsInit[ePos];
        }
      });
    }
  }

  // Reinsert multiple edges if any and update their layout
  for (auto re : removedEdges) {
    auto eEnds = graph->ends(re);

    if (eEnds.first == eEnds.second)
      oriGraph->addEdge(re);
    else {
      tlp::edge origEdge = oriGraph->existEdge(eEnds.first, eEnds.second);

      if (origEdge.isValid()) {
        oriGraph->addEdge(re);
        layout->setEdgeValue(re, layout->getEdgeValue(origEdge));
      } else {
        origEdge = oriGraph->existEdge(eEnds.second, eEnds.first);
        assert(origEdge.isValid());
        oriGraph->addEdge(re);
        std::vector<tlp::Coord> bends = layout->getEdgeValue(origEdge);
        std::reverse(bends.begin(), bends.end());
        layout->setEdgeValue(re, bends);
      }
    }
  }

  // If sphere mode, move the edge bends to the closest point on the sphere surface
  if (sphereLayout) {
    moveBendsToSphere(oriGraph, dist, layout);
  }

  if (!keepGrid) {
    // delete nodes in reverse order to avoid
    // the use of a stable iterator
    const vector<node> &nodes = graph->nodes();
    unsigned int sz = nodes.size();
    while (sz) {
      auto n = nodes[--sz];
      if (!oriGraph->isElement(n))
        graph->delNode(n, true);
    }
    graph->delAllSubGraphs(oriGraph);
    graph->delAllSubGraphs(gridGraph);
  }

  ThreadManager::setNumberOfThreads(ThreadManager::getNumberOfProcs());

  return true;
}
//============================================================

PLUGIN(EdgeBundling)
