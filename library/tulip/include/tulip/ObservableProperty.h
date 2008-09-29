#ifndef PROPERTYOBSERVABLE_H
#define PROPERTYOBSERVABLE_H
#include <set>
#include "tulip/Node.h"
#include "tulip/Edge.h"

namespace tlp {

struct PropertyInterface;
//=========================================================

/** \addtogroup graphs */ 
/*@{*/
/// Observer for Property
/**
 * The Observer pattern is described in the lecture notes and pp293-304 
 * of Design Patterns by Gamma, Helm, Johnson, and Vlissides. It is a 
 * framework for handling state dependency between observer and observed 
 * object.
 */
class PropertyObserver {
 public:
  virtual ~PropertyObserver() {}
  virtual void setNodeValue(PropertyInterface*, const node){}
  virtual void setEdgeValue(PropertyInterface*, const edge){}
  virtual void setAllNodeValue(PropertyInterface*){}
  virtual void setAllEdgeValue(PropertyInterface*){}
  virtual void destroy(PropertyInterface*){}
};
/*@}*/
}

#ifndef DOXYGEN_NOTFOR_DEVEL
/* namespace std {
  template <>
    struct less<tlp::PropertyObserver *> {
    size_t operator()(const tlp::PropertyObserver * obs1,const tlp::PropertyObserver *obs2) const {
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
/// Observable object for Property
class ObservableProperty {
 public:
  virtual ~ObservableProperty() {}
  /**
   * Register a new observer
   */
  void addPropertyObserver(PropertyObserver *) const;
  /**
   * Returns the number of observers
   */
  unsigned int countPropertyObservers();
  /**
   * Remove an observer
   */
  void removePropertyObserver(PropertyObserver *) const;
  /**
   * Remove all observers
   */
  void removePropertyObservers();

 protected:
  void notifySetNodeValue(PropertyInterface*,const node n);
  void notifySetEdgeValue(PropertyInterface*,const edge e);
  void notifySetAllNodeValue(PropertyInterface*);
  void notifySetAllEdgeValue(PropertyInterface*);
  void notifyDestroy(PropertyInterface*);
  mutable std::set<PropertyObserver*> observers;
};
/*@}*/

inline void ObservableProperty::addPropertyObserver(PropertyObserver *obs) const {
  observers.insert(obs); 
}

inline unsigned int ObservableProperty::countPropertyObservers() { 
  return observers.size(); 
}

inline void ObservableProperty::removePropertyObserver(PropertyObserver *item) const{  
  observers.erase(item);
}

inline void ObservableProperty::removePropertyObservers() { 
  observers.clear(); 
}

}

#endif

