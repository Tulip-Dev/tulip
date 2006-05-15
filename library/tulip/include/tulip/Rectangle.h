//@TLPGEOLICENCE#
#ifndef TLP_GEO_RECTANGLE_H
#define TLP_GEO_RECTANGLE_H
#include <tulip/Vector.h>
namespace tlp {

  /**
   * \addtogroup basic
   */ 
  /*@{*/
    /**
     * \brief class for Rectangle
     *
     * Enables to both create and manipulate a Rectangle
     *
     * Author : <a href="mailto:auber@tulip-software.org>David Auber</A>
     * Version 0.0.1 24/01/2003 
     */
    template<typename Obj>
    struct Rectangle : public Array<Vector<Obj,2>,2> {
      /**
       * Return true if r intersect "this".
       */
      bool intersect(const Rectangle &r) const {
	return (*this)[0][0]<r[1][0] && (*this)[1][0]>r[0][0] && (*this)[0][1]<r[1][1] && (*this)[1][1]>r[0][1];
      }
      /**
       * Translate "this" by vector v
       */
      void translate(const tlp::Vector<Obj,2> &v) {
	(*this)[0]+=v;
	(*this)[1]+=v;
      }
    };
    /*@}*/

}
//#include "cxx/Rectangle.cxx"
#endif

