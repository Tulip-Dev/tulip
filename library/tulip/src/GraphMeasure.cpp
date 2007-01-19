#include <deque>
#include <list>

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
double tlp::averagePathLength(Graph *sg) {
  double sumPath = 0;
  int nbNodes = sg->numberOfNodes();
  node n;
  forEach(n, sg->getNodes()) {
    MutableContainer<unsigned int> distance;
    sumPath += (double) tlp::maxDistance(sg, n, distance, 2 /* inout */);
  }
  return sumPath/(nbNodes * (nbNodes - 1));
}
//================================================================
double tlp::averageCluster(Graph *sg) {
  DataSet data;
  data.set("depth",1);
  bool result;
  string errMsg;
  DoubleProperty *cluster = new DoubleProperty(sg);
  result = sg->computeProperty("Cluster",cluster,errMsg,0,&data);
  double sum=0;
  Iterator<node>*itN=sg->getNodes();
  while (itN->hasNext())
    sum += cluster->getNodeValue(itN->next());
  delete itN;
  delete cluster;
  return sum /= double(sg->numberOfNodes());
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
