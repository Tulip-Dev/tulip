#ifndef GRAPHOBSERVABLE_H
#define GRAPHOBSERVABLE_H
#include <ext/slist>
#include "tulip/Node.h"
#include "tulip/Edge.h"

namespace tlp {

struct Graph;
 struct PropertyInterface;
 class ObservableGraph;
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
class  TLP_SCOPE GraphObserver {
   stdext::slist<ObservableGraph *> observables;
  bool updateObservables;
 public:
  GraphObserver(bool manageObservables = true)
   :updateObservables(manageObservables) {}
  virtual ~GraphObserver();
  virtual void addNode(Graph *,const node ){}
  virtual void addEdge(Graph *,const edge ){}
  virtual void delNode(Graph *,const node ){}
  virtual void delEdge(Graph *,const edge ){}
  virtual void reverseEdge(Graph *,const edge ){}
  virtual void destroy(Graph *){}
  virtual void addSubGraph(Graph *, Graph *){}
  virtual void delSubGraph(Graph *, Graph *){}
  virtual void addLocalProperty(Graph*, const std::string&){}
  virtual void delLocalProperty(Graph*, const std::string&){}
  virtual void beforeSetAttribute(Graph*, const std::string&) {}
  virtual void afterSetAttribute(Graph*, const std::string&) {}
  virtual void removeAttribute(Graph*, const std::string&) {}

  void addObservable(ObservableGraph *);
  void removeObservable(ObservableGraph *);
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
class  TLP_SCOPE ObservableGraph {
  friend class GraphObserver;

 public:
  virtual ~ObservableGraph() {removeGraphObservers();}
  /**
   * Register a new observer
   */
  void addGraphObserver(GraphObserver *) const;
  /**
   * Returns the number of observers
   */
  unsigned int countGraphObservers();
  /**
   * Remove an observer
   */
  void removeGraphObserver(GraphObserver *) const;
  /**
   * Remove all observers
   */
  void removeGraphObservers();

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
  void notifyBeforeSetAttribute(Graph*, const std::string&);
  void notifyAfterSetAttribute(Graph*, const std::string&);
  void notifyRemoveAttribute(Graph*, const std::string&);
  void notifyDestroy(Graph*);
  void removeOnlyGraphObserver(GraphObserver *) const;
  mutable stdext::slist<GraphObserver*> observers;
};
/*@}*/

inline unsigned int ObservableGraph::countGraphObservers() { 
  return observers.size(); 
}

inline void ObservableGraph::removeGraphObserver(GraphObserver *item) const {
  observers.remove(item);
  item->removeObservable((ObservableGraph*)this);
}

inline void ObservableGraph::removeOnlyGraphObserver(GraphObserver *item) const {
  observers.remove(item);
}

}

#endif

