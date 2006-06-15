#ifndef GRAPHOBSERVABLE_H
#define GRAPHOBSERVABLE_H
#include <set>
#include "Node.h"
#include "Edge.h"
struct SuperGraph;
//=========================================================

/** \addtogroup graphs */ 
/*@{*/
/// Observer for Graph
/**
 * The Observer pattern is described in the lecture notes and pp293-304 
 * of Design Patterns by Gamma, Helm, Johnson, and Vlissides. It is a 
 * framework for handling state dependency between observer and observed 
 * object.
 */
class GraphObserver {
 public:
  virtual ~GraphObserver() {}
  virtual void addNode(SuperGraph *,const node ){}
  virtual void addEdge(SuperGraph *,const edge ){}
  virtual void delNode(SuperGraph *,const node ){}
  virtual void delEdge(SuperGraph *,const edge ){}
  virtual void reverseEdge(SuperGraph *,const edge ){}
  virtual void destroy(SuperGraph *){}
};
/*@}*/

namespace std {
  template <>
  struct less<GraphObserver *> {
    size_t operator()(const GraphObserver * obs1,const GraphObserver *obs2) const {
      return (unsigned long)obs1<(unsigned long)obs2;
    }
  };
}

/** \addtogroup graphs */ 
/*@{*/
/**
 */
/// Observable object for graph
class ObservableGraph {
 public:
  virtual ~ObservableGraph() {}
  /**
   * Register a new observer
   */
  void addObserver(GraphObserver *) const;
  /**
   * Return the number of observers
   */
  unsigned int countObservers();
  /**
   * Remove an observer
   */
  void removeObserver(GraphObserver *) const;
  /**
   * Remove all observers
   */
  void removeObservers();

 protected:
  void notifyAddNode(SuperGraph *,const node n);
  void notifyAddEdge(SuperGraph *,const edge e);
  void notifyDelNode(SuperGraph *,const node n);
  void notifyDelEdge(SuperGraph *,const edge e);
  void notifyReverseEdge(SuperGraph *,const edge e);
  void notifyDestroy(SuperGraph *);
  mutable std::set<GraphObserver*> observers;
};
/*@}*/

inline void ObservableGraph::addObserver(GraphObserver *obs) const {
  observers.insert(obs); 
}

inline unsigned int ObservableGraph::countObservers() { 
  return observers.size(); 
}

inline void ObservableGraph::removeObserver(GraphObserver *item) const{  
  observers.erase(item);
}

inline void ObservableGraph::removeObservers() { 
  observers.clear(); 
}

#endif

