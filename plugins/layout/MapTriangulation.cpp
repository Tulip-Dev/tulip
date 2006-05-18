#include <tulip/ForEach.h>
#include <tulip/PlanarConMap.h>
#include <tulip/FaceIterator.h>
#include <tulip/PlanarityTest.h>
#include <tulip/BiconnectedTest.h>
#include <list>
#include <set>

using namespace std;

namespace {
  //==========================================================================
  struct UEdge {
    UEdge(const node uu, const node vv) :
      u(std::min(uu.id, vv.id)),
      v(std::max(uu.id, vv.id)) {
      assert (u.id < v.id);
    }
    bool operator < (const UEdge &e) const {
      if (u.id < e.u.id) return true;
      if (u.id > e.u.id) return false;
      if (v.id < e.v.id) return true;
      return false;
    } 
    node u, v;
  };
  //==========================================================================
  void treatFace(SuperGraph* graph, list<node> &face, vector<edge>& addedEdges, set<UEdge> &existEdge) {
    if (face.size()<4) return;
    while(face.size()>3) {
      //      cerr << "*" << flush;
      list<node>::const_iterator it = face.begin();
      node n = *it;
      ++it;
      ++it;
      if (n != *it && existEdge.find(UEdge(n, *it)) == existEdge.end() ) {
	edge newEdge = graph->addEdge(n, *it);
	addedEdges.push_back(newEdge);
	existEdge.insert(UEdge(n, *it));
	face.push_back(n);
	face.pop_front();
	face.pop_front();
      }
      else {
	face.push_back(n);
	face.pop_front();
      }
    }
  }
  //==========================================================================
  void initEdge(SuperGraph *graph, set<UEdge> &existEdge) {
    existEdge.clear();
    Iterator<edge> *it = graph->getEdges();
    while(it->hasNext()) {
      edge e = it->next();
      node u = graph->source(e);
      node v = graph->target(e);
      existEdge.insert(UEdge(u,v));
    } delete it;
  }
}
//==========================================================================
void saveNodesOfFaces(PlanarConMap *map, vector<list<node> > &faces) {
  Face f;
  forEach(f, map->getFaces()) {
    list<node> tmp;
    node n;
    forEach(n , map->getFaceNodes(f)) {
      tmp.push_back(n);
    }
    faces.push_back(tmp);
  }
}
//==========================================================================
namespace tlp {
  void triangulate(SuperGraph *graph, vector<edge>& addedEdges) {
    if (!PlanarityTest::isPlanar(graph)) {
      cerr << "[ERROR] : " << __PRETTY_FUNCTION__ << " : the graph must be planar" << endl;
    }
    BiconnectedTest::makeBiconnected(graph, addedEdges);
    PlanarConMap *map = new PlanarConMap(graph);
    //    map->makePlanar();
    set<UEdge> existEdge;
    initEdge(map, existEdge);
    vector<list<node> > faces;
    saveNodesOfFaces(map, faces);
    delete map;
    vector<list<node> >::iterator it;
    for (it = faces.begin(); it != faces.end(); ++it) {
      treatFace(graph, *it, addedEdges, existEdge);
    }
  }
}
//==========================================================================
