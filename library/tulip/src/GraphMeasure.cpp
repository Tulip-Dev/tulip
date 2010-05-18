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
  distance.setAll(UINT_MAX);
  fifo.push_back(n);
  distance.set(n.id, 0);
  unsigned int maxDist = 0;
  while(!fifo.empty()) {
    node current = fifo.front();
    fifo.pop_front();
    node itn;
    unsigned int itnDist = distance.get(current.id) + 1;
    forEach(itn, getIt(sg, current, direction)) {
      if (distance.get(itn.id) == UINT_MAX) {
	fifo.push_back(itn);
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
  sumPath = 0;
  int nbNodes = sg->numberOfNodes();
  if (nbNodes == 1)
    return true;
  node n;
  int steps = 0;
  MutableContainer<unsigned int> distance;
  forEach(n, sg->getNodes()) {
    if (pluginProgress && ((++steps % 100) == 0)) {
	pluginProgress->progress(steps, nbNodes);
	if (pluginProgress->state() !=TLP_CONTINUE)
	  return false;
    }
    tlp::maxDistance(sg, n, distance, 2);
    Iterator<node>*itN=sg->getNodes();
    while (itN->hasNext()) {
      node itn=itN->next();
      unsigned int d =  distance.get(itn.id);
      if (itn != n && d != UINT_MAX)
	sumPath += d;
    } delete itN;
  }
  if (pluginProgress)
    pluginProgress->progress(nbNodes, nbNodes);
  sumPath /= (nbNodes * (nbNodes - 1));
  return true;
  /*
  sumPath=0;
  int nbNodes = sg->numberOfNodes();
  if (nbNodes == 1)
    return true;
  int steps = 0;
  list<node> fifo;
  DoubleProperty *mark = new DoubleProperty(sg);
  Iterator<node>*itN=sg->getNodes();
  while (itN->hasNext()) {
    if (pluginProgress && ((++steps % 100) == 0)) {
	pluginProgress->progress(steps, nbNodes);
	if (pluginProgress->state() !=TLP_CONTINUE)
	  return false;
    }
    mark->setAllNodeValue(0);
    node itn=itN->next();
    fifo.clear();
    fifo.push_back(itn);
    while(!fifo.empty()) {
      node current = fifo.front();
      fifo.pop_front();
      Iterator<node> *itNei=sg->getInOutNodes(current);
      double value = mark->getNodeValue(current) + 1;
      while (itNei->hasNext()) {
	node nei = itNei->next();
	if (mark->getNodeValue(nei)==0 && nei!=itn)  {
	  mark->setNodeValue(nei, value);
	  sumPath += value;
	  fifo.push_back(nei);
	}
      } delete itNei;
    }
  } delete itN;
  delete mark;
  sumPath /= double(nbNodes *(nbNodes - 1));
  if (pluginProgress)
    pluginProgress->progress(nbNodes, nbNodes);
    return true; */
}
//================================================================
bool tlp::averageCluster(Graph *sg, double &sum, PluginProgress * pluginProgress) {
  sum=0;
  DoubleProperty cluster(sg);
  if (!tlp::computeClusterMetric(sg, &cluster, 1, pluginProgress))
    return false;
  Iterator<node>*itN=sg->getNodes();
  while (itN->hasNext())
    sum += cluster.getNodeValue(itN->next());
  delete itN;
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
static void clusterBuildSubGraph(Graph *graph, node n, node startNode,
				 set<node> &selected, unsigned int depth) {
  deque<node> fifo;
  Graph * sg = graph;
  MutableContainer<bool> visited;
  MutableContainer<unsigned int> distance;
  visited.setAll(false);
  distance.setAll(sg->numberOfNodes());
  fifo.push_back(n);
  visited.set(n.id, true);
  distance.set(n.id, 0);
  while(!fifo.empty()) {
    node current = fifo.front();
    fifo.pop_front();
    if (distance.get(current.id) < depth) {
      Iterator<node> *itN = sg->getInOutNodes(current);
      while(itN->hasNext()) {
	node itn = itN->next();
	if (!visited.get(itn.id)) {
	  fifo.push_back(itn);
	  selected.insert(itn);
	  visited.set(itn.id,true);
	  distance.set(itn.id, distance.get(current.id) + 1);
	}
      } delete itN;
    }
  }
}
//=================================================
static double clusterGetEdgeValue(Graph *graph, DoubleProperty *prop, const edge e ) {
  pair<node, node> eEnds = graph->ends(e);
  const double& v1 = prop->getNodeValue(eEnds.first);
  const double& v2 = prop->getNodeValue(eEnds.second);
  if (v1*v1 + v2*v2 > 0)
    return 1.-fabs(v1 - v2)/sqrt(v1*v1 + v2*v2);
  return 0.;
}
//=================================================
static double clusterGetNodeValue(Graph *graph, const node n, unsigned int maxDepth) {
  set<node> reachableNodes;
  clusterBuildSubGraph(graph, n, n, reachableNodes, maxDepth);
  double nbEdge=0; //e(N_v)*2$
  for (set<node>::iterator itSN=reachableNodes.begin();itSN!=reachableNodes.end();++itSN) {
    node itn=*itSN;
    Iterator<edge> *itE=graph->getInOutEdges(itn);
    while (itE->hasNext()) {
      pair<node, node> eEnds = graph->ends(itE->next());
      if ( (reachableNodes.find(eEnds.first)!=reachableNodes.end()) && 
	   (reachableNodes.find(eEnds.second)!=reachableNodes.end())) {
	nbEdge++;
      }
    } delete itE;
  }
  
  double nNode= reachableNodes.size(); //$|N_v|$
  if (reachableNodes.size()>1) {
    double result = double(nbEdge)/(nNode*(nNode-1));
    return result; //$e(N_v)/(\frac{k*(k-1)}{2}}$
  }
  else
    return 0;
}
//=================================================
bool tlp::computeClusterMetric(Graph *graph, DoubleProperty *prop, unsigned int maxDepth,
			     PluginProgress *) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  node n;
  forEach(n, graph->getNodes())
    prop->setNodeValue(n, clusterGetNodeValue(graph, n, maxDepth));
  edge e;
  forEach(e, graph->getEdges())
    prop->setEdgeValue(e, clusterGetEdgeValue(graph, prop, e));
  return true;
}
//==================================================
bool tlp::computeDagLevelMetric (Graph *graph, DoubleProperty *prop, PluginProgress *) {
  MutableContainer<unsigned int> totreat;
  deque<node> fifo;
  //===============================================
  Iterator<node> *itN = graph->getNodes();
  while(itN->hasNext()) {
    node itn=itN->next();
    unsigned int indegree = graph->indeg(itn);
    if (indegree==0) {
      fifo.push_back(itn);
      prop->setNodeValue(itn,0);
    }
    else
      totreat.set(itn.id, indegree - 1);
  } delete itN;
  //==============================================
  while (!fifo.empty()) {
    node current = fifo.front();
    fifo.pop_front();
    itN=graph->getOutNodes(current);
    while(itN->hasNext()) {
      node child=itN->next();
      if (totreat.get(child.id)>0)
	totreat.set(child.id,totreat.get(child.id)-1);
      else {
	prop->setNodeValue(child, prop->getNodeValue(current)+1);
	fifo.push_back(child);
      }
    } delete itN;
  }
  //==============================================
  return true;
}
