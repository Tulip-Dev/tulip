/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#ifndef VIEW3DBSERVER_H
#define VIEW3DBSERVER_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <set>

//=========================================================

/**
 * The Observer pattern is described in the lecture notes and pp293-304 
 * of Design Patterns by Gamma, Helm, Johnson, and Vlissides. It is a 
 * framework for handling state dependency between observer and observed 
 * object.
 */
namespace tlp {

struct GlGraph;

class GlGraphObserver {
 public:
  virtual ~GlGraphObserver() {}
  virtual void moveCamera(GlGraph *){}
  virtual void moveObject(GlGraph *){}
  virtual void draw(GlGraph *){}
  virtual void destroy(GlGraph *){}
};
/*@}*/
}

#ifndef DOXYGEN_NOTFOR_USER
namespace std {
  /// for use in some STL sets
  template<>
    struct less<tlp::GlGraphObserver *> {
    size_t operator()(const tlp::GlGraphObserver * obs1,const tlp::GlGraphObserver *obs2) const {
      return (unsigned long)obs1<(unsigned long)obs2;
    }
  };
}
#endif //DOXYGEN_NOTFOR_USER

namespace tlp {

/** \addtogroup graphs */ 
/*@{*/
/**
 */
class ObservableGlGraph {
 public:
  virtual ~ObservableGlGraph() {}
  /**
   * Register a new observer
   */
  void addObserver(GlGraphObserver *);
  /**
   * Returns the number of observers
   */
  unsigned int countObservers();
  /**
   * Remove an observer
   */
  void removeObserver(GlGraphObserver *);
  /**
   * Remove all observers
   */
  void removeObservers();

 protected:
  void notifyMoveCamera(GlGraph *);
  void notifyMoveObject(GlGraph *);
  void notifyDraw(GlGraph *);
  void notifyDestroy(GlGraph *);
  std::set<GlGraphObserver*> observers;
};

inline void ObservableGlGraph::addObserver(GlGraphObserver *obs) {
  observers.insert(obs); 
}

inline unsigned int ObservableGlGraph::countObservers() { 
  return observers.size(); 
}

inline void ObservableGlGraph::removeObserver(GlGraphObserver *item) {  
  observers.erase(item);
}

inline void ObservableGlGraph::removeObservers() { 
  observers.clear(); 
}

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif

