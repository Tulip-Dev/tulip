#ifndef GRAPHOBSERVABLE_H
#define GRAPHOBSERVABLE_H
#include <set>
#include "tulip/Node.h"
#include "tulip/Edge.h"

namespace tlp {

struct Graph;
 struct PropertyInterface;
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
  virtual void addNode(Graph *,const node ){}
  virtual void addEdge(Graph *,const edge ){}
  virtual void delNode(Graph *,const node ){}
  virtual void delEdge(Graph *,const edge ){}
  virtual void reverseEdge(Graph *,const edge ){}
  virtual void destroy(Graph *){}
  virtual void addSubGraph(Graph*, Graph *){}
  virtual void delSubGraph(Graph*, Graph*){}
  virtual void addLocalProperty(Graph*, const std::string&){}
  virtual void delLocalProperty(Graph*, const std::string&){}
};
/*@}*/
}

#ifndef DOXYGEN_NOTFOR_DEVEL
/*namespace std {
  template <>
    struct less<tlp::GraphObserver *> {
    size_t operator()(const tlp::GraphObserver * obs1,const tlp::GraphObserver *obs2) const {
      return (unsigned long)obs1<(unsigned long)obs2;
    }
  };
  }*/
#endif // DOXYGEN_NOTFOR_DEVEL

namespace tlp {

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
   * Returns the number of observers
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
  void notifyAddNode(Graph *,const node n);
  void notifyAddEdge(Graph *,const edge e);
  void notifyDelNode(Graph *,const node n);
  void notifyDelEdge(Graph *,const edge e);
  void notifyReverseEdge(Graph *,const edge e);
  void notifyAddSubGraph(Graph*, Graph*);
  void notifyDelSubGraph(Graph*, Graph*);
  void notifyAddLocalProperty(Graph*, const std::string&);
  void notifyDelLocalProperty(Graph*, const std::string&);
  void notifyDestroy(Graph *);
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

}

#endif

