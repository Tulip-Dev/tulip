#include <deque>
#include <list>
#include <limits.h>

#include "tulip/GraphMeasure.h"
#include "tulip/Reflect.h"
#include "tulip/Graph.h"
#include "tulip/DoubleProperty.h"
#include "tulip/ForEach.h"


using namespace std;
using namespace tlp;
namespace {
  inline Iterator<node> *getIt(Graph *sg, node n, int direction) {
    switch(direction) {
    case 0:
      return sg->getOutNodes(n);
    case 1:
      return sg->getInNodes(n);
    case 2:
      return sg->getInOutNodes(n);
    default:
      cerr << __PRETTY_FUNCTION__ << "serious bug...";
      return 0;
    }
    return NULL;
  }
  inline Iterator<node> *getAncIt(Graph *sg, node n, int direction) {
    switch(direction) {
    case 1:
      return sg->getOutNodes(n);
    case 0:
      return sg->getInNodes(n);
    case 2:
      return sg->getInOutNodes(n);
    default:
      cerr << __PRETTY_FUNCTION__ << "serious bug...";
      return 0;
    }
    return NULL;
  }
}
//================================================================
unsigned int tlp::maxDistance(Graph *sg, node n, MutableContainer<unsigned int> &distance, int direction) {
  deque<node> fifo;
  MutableContainer<bool> visited;
  visited.setAll(false);
  distance.setAll(sg->numberOfNodes());
  fifo.push_back(n);
  visited.set(n.id, true);
  distance.set(n.id, 0);
  unsigned int maxDist = 0;
  while(!fifo.empty()) {
    node current = fifo.front();
    fifo.pop_front();
    node itn;
    unsigned int itnDist = distance.get(current.id) + 1;
    forEach(itn, getIt(sg, current, direction)) {
      if (!visited.get(itn.id)) {
	fifo.push_back(itn);
	visited.set(itn.id, true);
	distance.set(itn.id, itnDist);
	maxDist = std::max(maxDist, itnDist);
      }
    }
  }
  return maxDist;
}
//================================================================
//Warning the algorithm is not optimal
bool tlp::averagePathLength(Graph *sg, double& sumPath, PluginProgress *pluginProgress) {
  int nbNodes = sg->numberOfNodes();
  node n;
  int steps = 0;
  forEach(n, sg->getNodes()) {
    MutableContainer<unsigned int> distance;
    if (pluginProgress && ((++steps % 100) == 0)) {
	pluginProgress->progress(steps, nbNodes);
	if (pluginProgress->state() !=TLP_CONTINUE)
	  return false;
    }
    sumPath += (double) tlp::maxDistance(sg, n, distance, 2 /* inout */);
  }
  if (pluginProgress)
    pluginProgress->progress(nbNodes, nbNodes);
  sumPath /= (nbNodes * (nbNodes - 1));
  return true;
}
//================================================================
bool tlp::averageCluster(Graph *sg, double &sum, PluginProgress * pluginProgress) {
  sum=0;
  DataSet data;
  data.set("depth",1);
  string errMsg;
  DoubleProperty *cluster = new DoubleProperty(sg);
  if (!sg->computeProperty("Cluster", cluster, errMsg, pluginProgress, &data))
    return false;
  Iterator<node>*itN=sg->getNodes();
  while (itN->hasNext())
    sum += cluster->getNodeValue(itN->next());
  delete itN;
  delete cluster;
  sum /= double(sg->numberOfNodes());
  return true;
}
//================================================================
unsigned int tlp::maxDegree(Graph *sg) {
  unsigned int maxdeg = 0;
  Iterator<node> *itN=sg->getNodes();
  while (itN->hasNext())
    maxdeg = std::max(maxdeg , sg->deg(itN->next()));
  delete itN;
  return maxdeg;
}
//================================================================
unsigned int tlp::minDegree(Graph *sg) {
  unsigned int mindeg = sg->numberOfNodes();
  Iterator<node> *itN=sg->getNodes();
  while (itN->hasNext())
    mindeg = std::min(mindeg , sg->deg(itN->next()));
  delete itN;
  return mindeg;
}
//================================================================
