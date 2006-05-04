
#include "tulip/Graph.h"

using namespace std;

#ifdef _WIN32 
#ifdef DLL_EXPORT
IdManager Graph::idManager;
#endif
#else
IdManager Graph::idManager;
#endif

ostream & operator << (ostream &os,const Graph *sp) {
  os << ";(nodes <node_id> <node_id> ...)" << endl;
  os << "(nodes ";
  Iterator<node> *itn=sp->getNodes();
  for (;itn->hasNext();) {
    os << itn->next().id;
    if (itn->hasNext()) os << " ";
  } delete itn;
  os << ")" << endl;
  os << ";(edge <edge_id> <source_id> <target_id>)" << endl;
  Iterator<edge> *ite=sp->getEdges();
  for (;ite->hasNext();) {
    edge e=ite->next();
    os << "(edge " << e.id << " " << sp->source(e).id << " " << sp->target(e).id << ")";
    if (ite->hasNext()) os << endl;
  } delete ite;
  os << endl;
  return os;
}
