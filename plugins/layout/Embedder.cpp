#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/Graph.h>
#include <tulip/TulipPlugin.h>

#include "EmbedderEmbedGraph.h"

#if (__GNUC__ < 3)
#  include <hash_map>
#else
#  include <ext/hash_map>
   using stdext::hash_map;
#endif

using namespace tlp;

/** \addtogroup layout */
/*@{*/
/// Embedder.cpp - An implementation of the HDE algorithm.
/**
 * This plugin implements HDE:
 * "Graph Drawing by High-Dimensional Embedding"
 * Proceedings of 10th Int. Symp. Graph Drawing (GD'02),
 * Lecture Notes in Computer Science, Vol. 2528, 
 * Springer Verlag, pp. 207--219, 2002. 
 * Authors: D. Harel and Y. Koren,
 *
 * From the sources of Embedder (reference implementation of the mentionned article)
 *
 * See http://www.research.att.com/~yehuda/ for both publication and source code
 *
 * Integration in Tulip by B.Mathieu
 */
class Embedder : public LayoutAlgorithm {
public:
  Embedder(const PropertyContext &);
  ~Embedder();
  bool run();
private:
  hash_map<unsigned int, node> nodemap;
  hash_map<node, unsigned int> rmap;
};

LAYOUTPLUGINOFGROUP(Embedder, "Embedder (HDE)", "Bertrand Mathieu", "27/05/2003", "Ok", "1.0", "Force Directed");

using namespace std;
using namespace tlp;


Embedder::Embedder(const PropertyContext &context) : LayoutAlgorithm(context) {
}

Embedder::~Embedder() {}

bool Embedder::run() {
  unsigned int numberOfNodes = (unsigned int) graph->numberOfNodes();
  vtx_data *sg = new vtx_data[numberOfNodes];
  int *edges = new int[2 * graph->numberOfEdges() + numberOfNodes];
  float *ewgts = new float[2 * graph->numberOfEdges() + numberOfNodes];

  nodemap.clear();
  nodemap.resize(numberOfNodes);
  rmap.clear();
  rmap.resize(numberOfNodes);

  Iterator<node> *it = graph->getNodes();
  for (unsigned int i=0; i < numberOfNodes; ++i) {
    node n = it->next();
    nodemap[i] = n;
    rmap[n] = i;
  } delete it;

  for (unsigned int i=0; i < numberOfNodes; ++i) {
    node n = nodemap[i];
    sg[i].nedges = graph->deg(n) + 1;
    sg[i].edges = edges++;
    sg[i].ewgts = ewgts++;
    sg[i].edges[0] = i;
    sg[i].ewgts[0] = -1 * graph->deg(n);
    
    Iterator<node> *tmp = graph->getInOutNodes(n);
    while (tmp->hasNext()) {
      node tmpn = tmp->next();
      *edges = rmap[tmpn];
      *ewgts = 1.0;
      ++edges;
      ++ewgts;
    } delete tmp;
  }

  DistType **coords = NULL;
  double **dcoords = new double *[2];
  dcoords[0] = new double[numberOfNodes*2];
  dcoords[1] = &(dcoords[0][numberOfNodes]);
  
  embed_graph(sg, numberOfNodes, 50, coords);
  center_coordinate(coords, numberOfNodes, 50);
  PCA(coords, 50, numberOfNodes, dcoords, 0, 1, true);
  
  delete [] sg[0].edges;
  delete [] sg[0].ewgts;
  delete [] sg;
  delete [] coords[0];
  delete [] coords;

  for (hash_map<node, unsigned int>::const_iterator it = rmap.begin(); it != rmap.end(); ++it) {
    unsigned int index = it->second;
    layoutResult->setNodeValue(it->first, Coord(dcoords[0][index], dcoords[1][index]));
  }

  delete [] dcoords[0];
  delete [] dcoords;
  return true;
}
