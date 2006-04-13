#include <deque>
#include <list>

#include "tulip/GraphMeasure.h"
#include "tulip/Reflect.h"
#include "tulip/SuperGraph.h"
#include "tulip/Metric.h"


using namespace std;
namespace {
  inline Iterator<node> *getIt(SuperGraph *graph, node n, int direction) {
    switch(direction) {
    case 0:
      return graph->getOutNodes(n);
    case 1:
      return graph->getInNodes(n);
    case 2:
      return graph->getInOutNodes(n);
    default:
      cerr << __PRETTY_FUNCTION__ << "serious bug...";
      return 0;
    }
    return NULL;
  }
  inline Iterator<node> *getAncIt(SuperGraph *graph, node n, int direction) {
    switch(direction) {
    case 1:
      return graph->getOutNodes(n);
    case 0:
      return graph->getInNodes(n);
    case 2:
      return graph->getInOutNodes(n);
    default:
      cerr << __PRETTY_FUNCTION__ << "serious bug...";
      return 0;
    }
    return NULL;
  }
}
//================================================================
unsigned int tlp::maxDistance(SuperGraph *graph, node n, MutableContainer<unsigned int> &distance, int direction) {
  deque<node> fifo;
  MutableContainer<bool> visited;
  visited.setAll(false);
  distance.setAll(graph->numberOfNodes());
  fifo.push_back(n);
  visited.set(n.id, true);
  distance.set(n.id, 0);
  unsigned int maxDist = 0;
  while(!fifo.empty()) {
    node current = fifo.front();
    fifo.pop_front();
    Iterator<node> *itN = getIt(graph, current, direction);
    while(itN->hasNext()) {
      node itn = itN->next();
      if (!visited.get(itn.id)) {
	fifo.push_back(itn);
	visited.set(itn.id, true);
	distance.set(itn.id, distance.get(current.id) + 1);
	maxDist = std::max(maxDist, distance.get(current.id) + 1);
      }
    } delete itN;
  }
  return maxDist;
}
//================================================================
//Warning the algorithm is not optimal
double tlp::averagePathLength(SuperGraph *sg) {
  list<node> fifo;
  double sumPath=0;
  Metric *mark = new Metric(sg);
  Iterator<node>*itN=sg->getNodes();
  while (itN->hasNext()) {
    mark->setAllNodeValue(0);
    node itn=itN->next();
    fifo.clear();
    fifo.push_back(itn);
    while(!fifo.empty()) {
      node current=fifo.front();
      fifo.pop_front();
      Iterator<node> *itNei=sg->getInOutNodes(current);
      while (itNei->hasNext()) {
	node nei=itNei->next();
	if (mark->getNodeValue(nei)==0 && nei!=itn)  {
	  mark->setNodeValue(nei,mark->getNodeValue(current)+1);
	  sumPath+=mark->getNodeValue(current)+1;
	  fifo.push_back(nei);
	}
      } delete itNei;
    }
  } delete itN;
  delete mark;
  return sumPath/double(sg->numberOfNodes()*(sg->numberOfNodes()-1));
}
//================================================================
double tlp::averageCluster(SuperGraph *sg) {
  DataSet data;
  data.set("depth",1);
  bool result;
  string errMsg;
  Metric *cluster = new Metric(sg);
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
unsigned int tlp::maxDegree(SuperGraph *sg) {
  unsigned int maxdeg = 0;
  Iterator<node> *itN=sg->getNodes();
  while (itN->hasNext())
    maxdeg = std::max(maxdeg , sg->deg(itN->next()));
  delete itN;
  return maxdeg;
}
//================================================================
unsigned int tlp::minDegree(SuperGraph *sg) {
  unsigned int mindeg = sg->numberOfNodes();
  Iterator<node> *itN=sg->getNodes();
  while (itN->hasNext())
    mindeg = std::min(mindeg , sg->deg(itN->next()));
  delete itN;
  return mindeg;
}
//================================================================
