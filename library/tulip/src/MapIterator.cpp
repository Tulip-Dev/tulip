#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/MapIterator.h"

using namespace std;
using namespace tlp;

edge tlp::nextFaceEdge(Graph *g, edge e, node n) {
  EdgeMapIterator it(g, e, n);
  edge result;
  if (it.hasNext())
    result = it.next();
  return result;
}

NodeMapIterator::NodeMapIterator(Graph *sg, node source, node target) {
  assert(cloneIt.empty());
  bool start=true;
  Iterator<node> *itIn=sg->getInOutNodes(target);
  while (itIn->hasNext()) {
    node tmp=itIn->next();
    if ((start) && (tmp==source)) {
      start=false;
      itStl=cloneIt.begin();
    }
    else {
      if (start)
	cloneIt.push_back(tmp);
      else 
	cloneIt.insert(itStl,tmp);
    }
  } delete itIn;
  itStl=cloneIt.begin();
}

NodeMapIterator::~NodeMapIterator(){};

node NodeMapIterator::next() {
  node tmp=*itStl;
  ++itStl;
  return tmp;
}

bool NodeMapIterator::hasNext() {
  return (itStl!=cloneIt.end());
}

//=========================================

EdgeMapIterator::EdgeMapIterator(Graph *sg, edge source, node target) {
  adj.resize(sg->deg(target));
  finished = false;
  treat =0;
  pos = 0;
  Iterator<edge> *it=sg->getInOutEdges(target);
  while (it->hasNext()) {
    edge e = it->next();
    if (e == source) pos = treat + 1;
    adj[treat++] = e;
  } delete it;
}

EdgeMapIterator::~EdgeMapIterator(){}

edge EdgeMapIterator::next() {
  --treat;
  pos %= adj.size();
  return adj[pos++];
}

bool EdgeMapIterator::hasNext() {
  return (treat>0);
}
