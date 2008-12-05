#ifndef DOXYGEN_NOTFOR_DEVEL
#ifndef TLPGRAPHRECORDER_H
#define TLPGRAPHRECORDER_H


#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif
#include <string>
#include <set>
#include <vector>

#include "tulip/Node.h"
#include "tulip/Edge.h"
#include "tulip/ObservableGraph.h"
#include "tulip/ObservableProperty.h"
#include "tulip/Graph.h"
#include "tulip/AbstractProperty.h"
#include "tulip/IdManager.h"

namespace std {
  template<>
    struct less<tlp::Graph*> {
      size_t  operator()(const tlp::Graph* g1, const tlp::Graph* g2) const { 
	// the id order corresponds to the creation order
	// so when dealing with a set<Graph*> this will ensure that
	// we encounter a supergraph before its subgraphs
	return g1->getId() < g2->getId();
      }
    };
}

namespace tlp {

  struct EdgeRecord {
    node source;
    node target;
    std::set<tlp::Graph*> graphs;

    EdgeRecord() {}
    EdgeRecord(Graph* g, node s, node t) : source(s), target(t) {
      graphs.insert(g);
    }
  };

  struct PropertyRecord {
    PropertyInterface* prop;
    std::string name;

    PropertyRecord(PropertyInterface* p, std::string str)
      :prop(p), name(str) {}
  };
}

namespace std {
  template<>
    struct less<tlp::PropertyRecord> {
      size_t  operator()(const tlp::PropertyRecord& rp1, const tlp::PropertyRecord& rp2) const {
	return rp1.name < rp2.name;
      }
    };
}

namespace tlp {
  class GraphImpl;

  class GraphUpdatesRecorder :public GraphObserver, public PropertyObserver {
    friend class GraphImpl;
    //
#if !defined(NDEBUG)
    bool recordingStopped;
#endif
    bool updatesReverted;

    // one set of Graph* par added node
    stdext::hash_map<node, std::set<Graph*> > addedNodes;
    // source + target + set of Graph* par added edge
    stdext::hash_map<edge, EdgeRecord> addedEdges;
    // one set of Graph* par deleted node
    stdext::hash_map<node, std::set<Graph*> >  deletedNodes;
    // source + target + set of Graph* par deleted edge
    stdext::hash_map<edge, EdgeRecord> deletedEdges;
    // one set of reverted edges per graph
    std::set<edge> revertedEdges;
    // one set for old edge containers
    stdext::hash_map<node, std::vector<edge> > oldContainers;
    // one set for new edge containers
    stdext::hash_map<node, std::vector<edge> > newContainers;

    // copy of nodeIds at start time
    IdManager oldNodeIdManager;
    // copy of edgeIds at start time
    IdManager oldEdgeIdManager;
    // copy of nodeIds at stop time
    IdManager newNodeIdManager;
    // copy of edgeIds at stop time
    IdManager newEdgeIdManager;

    // one set of added sub graphs per graph
    stdext::hash_map<unsigned long, std::set<Graph*> > addedSubGraphs;
    // one set of deleted sub graphs per graph
    stdext::hash_map<unsigned long, std::set<Graph *> > deletedSubGraphs;

    // one set of added properties per graph
    stdext::hash_map<unsigned long,  std::set<PropertyRecord> > addedProperties;
    // pne set of deleted properties per graph
    stdext::hash_map<unsigned long,  std::set<PropertyRecord> > deletedProperties;
    // the old default node value for each updated property
    stdext::hash_map<unsigned long, std::string> oldNodeDefaultValues;
    // the new default node value for each updated property
    stdext::hash_map<unsigned long, std::string> newNodeDefaultValues;
    // the old default edge value for each updated property
    stdext::hash_map<unsigned long, std::string> oldEdgeDefaultValues;
    // the new default edge value for each updated property
    stdext::hash_map<unsigned long, std::string> newEdgeDefaultValues;

    // the old node values for each updated property
    stdext::hash_map<unsigned long, stdext::hash_map<node, std::string> > oldNodeValues;
    // the new node value for each updated property
    stdext::hash_map<unsigned long, stdext::hash_map<node, std::string> >  newNodeValues;

    // the old edge values for each updated property
    stdext::hash_map<unsigned long, stdext::hash_map<edge, std::string> > oldEdgeValues;
    // the new edge value for each property
    stdext::hash_map<unsigned long, stdext::hash_map<edge, std::string> > newEdgeValues;

    // real deletion of deleted objects (properties, sub graphs)
    // during the recording of updates thes objects are removed from graph
    // structures but not really 'deleted'
    void deleteDeletedObjects();
    // record of a node's edges container before/after modification
    void recordEdgeContainer(stdext::hash_map<node, std::vector<edge> >&,
			     GraphImpl*, node);
    // remove an edge from a node's edges container
    void removeFromEdgeContainer(stdext::hash_map<node, std::vector<edge> >& containers, edge e, node n);

    // record a node value in a node values container
    void recordNodeValue(PropertyInterface*, const node,
			 stdext::hash_map<unsigned long,
			 stdext::hash_map<node, std::string> >&,
			 bool);

    // same for edge
    void recordEdgeValue(PropertyInterface*, const edge,
			 stdext::hash_map<unsigned long,
			 stdext::hash_map<edge, std::string> >&,
			 bool);

    // start of recording (push)
    void startRecording(GraphImpl*); 
    // end of recording
    // push an other recorder or pop this one
    void stopRecording(Graph*);
    // restart of recording (unpop)
    void restartRecording(Graph*); 
    // perform undo/redo updates
    void doUpdates(GraphImpl*, bool undo);

  public:
    GraphUpdatesRecorder();
    ~GraphUpdatesRecorder();

    // GraphObserver interface
    // addedNodes
    void addNode(Graph* g, node n);

    // addedEdges
    void addEdge(Graph* g, edge e);

    // deletedNodes
    void delNode(Graph* g, node n);

    // deletedEdges
    void delEdge(Graph* g, edge e);

    // revertedEdges
    void reverseEdge(Graph* g,  edge e);

    // addedSubGraphs
    void addSubGraph(Graph* g, Graph* sg);

    // deletedSubGraphs
    void delSubGraph(Graph* g, Graph* sg);

    // addedProperties
    void addLocalProperty(Graph* g, const std::string& name);

    // deletedProperties
    void delLocalProperty(Graph* g, const std::string& name);

    // PropertyObserver Interface
    // oldNodeValues
    void beforeSetNodeValue(PropertyInterface* p, const node n);
            
    // newNodeValues
    void afterSetNodeValue(PropertyInterface* p, const node n);

    // oldNodeDefaultValues
    void beforeSetAllNodeValue(PropertyInterface* p);
            
    // newNodeDefaultValues
    void afterSetAllNodeValue(PropertyInterface* p);

    // oldEdgeValues
    void beforeSetEdgeValue(PropertyInterface* p, const edge e);
            
    // newEdgeValues
    void afterSetEdgeValue(PropertyInterface* p, const edge e);

    // oldEdgeDefaultValues
    void beforeSetAllEdgeValue(PropertyInterface* p);
            
    // newEdgeDefaultValues
    void afterSetAllEdgeValue(PropertyInterface* p);
  };
}


#endif // TLPGRAPHRECORDER_H
#endif // DOXYGEN_NOTFOR_DEVEL
