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
//@TLPGEOLICENCE#
#ifndef TLP_GEO_CIRCLE_H
#define TLP_GEO_CIRCLE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <vector>
#include <tulip/Vector.h>
namespace tlp {
  /**
   * \addtogroup basic
   */ 
  /*@{*/
    /**
     * \brief class for circle
     *
     * Enables to both create and manipulate a circle
     *
     * \author David Auber auber@tulip-software.org
     * \version 0.0.1 24/01/2003 
     */
    template<typename Obj>
    struct Circle : public Vector<Obj,2> {
      Circle(){}
      Circle(const Vector<Obj,2> &pos, Obj radius):Vector<Obj,2>(pos),radius(radius) {}
      Circle(const Circle &c):Vector<Obj,2>(c),radius(c.radius) {}
      Circle(Obj x,Obj y,Obj radius):radius(radius) { (*this)[0]=x; (*this)[1]=y; }
      /**
       * Translate "this" by vector v
       */
      void translate(const Vector<Obj,2> &v) {
	(*this)+=v;
      }
      /**
       * Give the instersction of "this" with c, return false if no intersection exist
       */
      bool intersection(const Circle<Obj> &, const Vector<Obj,2> &,  const Vector<Obj,2> &) {
	return true;
      }
      /**
       * Merges this circle with another circle; merging operation
       * consists in computing the smallest enclosing circle of the 
       * two circle and to store the result in "this".
       */
      Circle<Obj>& merge(const Circle<Obj> &c);
      /**
       * Radius of the circle
       */
      Obj radius;
      /**
       * Returns true if the circle is include in an other circle, false otherwise.
       */
      bool isIncludeIn(const Circle<Obj> & circle) const;
    };
    

    /**
     * Compute the optimum enclosing circle of 2 circles.
     */
    template<typename Obj>
    Circle<Obj> enclosingCircle(const Circle<Obj> &,const Circle<Obj> &);

    /**
     * Compute the optimum enclosing circle of a set of circles.
     */
    template<typename Obj>
    Circle<Obj> enclosingCircle(const std::vector< Circle<Obj> > & circles);
    /**
     * Compute an enclosing circle of a set of circles,
     * this algorithm is an aproximation of the smallest 
     * enclosing circle.
     */
    template<typename Obj>
    Circle<Obj> lazyEnclosingCircle(const std::vector< Circle<Obj> > & circles);
    /**
     * Write circle in a stream
     */
    template<typename Obj>
      std::ostream& operator<<(std::ostream &os,const Circle<Obj> &);
    /*@}*/
}

#include "cxx/Circle.cxx"
#endif
