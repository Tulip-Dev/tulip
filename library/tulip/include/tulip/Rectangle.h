//@TLPGEOLICENCE#
#ifndef TLP_RECTANGLE_H
#define TLP_RECTANGLE_H
#include <tulip/Vector.h>
#include <tulip/BoundingBox.h>

namespace tlp {

    /**
   * \addtogroup basic
   */ 
    /*@{*/
    /**
     * \brief class for rectangle
     *
     * Enables to both create and manipulate a 2D Axis Aligned Rectangle
     *
     * Author : <a href="www.tulip-software.org>Tulip team</a>
     */
    template<typename Obj>
    struct Rectangle : public Array<Vector<Obj,2>,2> {
        /**
          * Create a new invalid rectangle
          */
        Rectangle() {
            (*this)[0].fill(1);
            (*this)[1].fill(-1);
        }
        /**
          * Create a new rectangle with
          * (*this)[0] = min = (xmin, ymin);
          * (*this)[1] = max = (xmax, ymax);
          * Validity is tested in debug mode;
          */
        Rectangle(const Obj xmin, const Obj ymin, const Obj xmax, const Obj ymax) {
            (*this)[0][0] = xmin;
            (*this)[0][1] = ymin;
            (*this)[1][0] = xmax;
            (*this)[1][1] = ymax;
            assert(isValid());
        }
        /**
          * Create a new Rectangle from a Bounding Box correct conversion from 3D -> 2D
          * validity of the new Rectangle is tested in debug mode
          */
        Rectangle(const tlp::BoundingBox &b) {
            (*this)[0][0] = b[0][0];
            (*this)[0][1] = b[0][1];
            (*this)[1][0] = b[1][0];
            (*this)[1][1] = b[1][1];
            assert(isValid());
        }

       /**
       * create a new Rectangle
       * Validity is checked in debug mode
       */
        Rectangle(const Vector<Obj,2> &min, const Vector<Obj,2> &max) {
            (*this)[0] = min;
            (*this)[1] = max;
            assert(isValid());
        }
        /**
       * @return true if r intersect "this".
       */
	bool intersect(const Rectangle &r) const {
            assert(this->isValid());
            assert(r.isValid());
	    
            if ((*this)[0][0] > r[1][0])  return false;
            if ((*this)[1][0] < r[0][0])  return false;
            if ((*this)[0][1] > r[1][1])  return false;
            if ((*this)[1][1] < r[0][1])  return false;

            return true;
	}
	/**
       * @return the true if there is an intersection else false, the intersection parameter is used to stored the Rectangle pf intersection (if it exists). 
       */
	bool intersect(const Rectangle &r, Rectangle &intersection) const {
            assert(this->isValid());
            assert(r.isValid());
            if (!this->intersect(r)) return false;

            intersection[0][0] = std::max((*this)[0][0] , r[0][0]);
            intersection[1][0] = std::min((*this)[1][0] , r[1][0]);
            intersection[0][1] = std::max((*this)[0][1] , r[0][1]);
            intersection[1][1] = std::min((*this)[1][1] , r[1][1]);

            return true;
	}
	/**
	* @return true if the Rectangle is well define [0] min corner, [1] max corner.
	*/
        bool isValid() const {
            return (*this)[0][0] <= (*this)[1][0] && (*this)[0][1] <= (*this)[1][1];
	}
        /**
        * Return true if point is stricly inside the AARectangle
        * validity of the current rectangle is tested in debug mode
        */
        bool isInside(const Vector<Obj, 2> &p) const {
            assert(isValid());
            if (p[0] > (*this)[1][0]) return false;
            if (p[0] < (*this)[0][0]) return false;
            if (p[1] > (*this)[1][1]) return false;
            if (p[1] < (*this)[0][1]) return false;
            return true;
        }
        /**
        * @return true if r is inside or equal to the AARectangle
        * Validity of Rectangles is tested in debug mode
        */
        bool isInside(const Rectangle &r) const{
            assert(isValid());
            assert(r.isValid());
            if ((*this)[0] == r[0] && (*this)[1] == r[1]) return true;
            if (this->isInside(r[0]) && this->isInside(r[1])) return true;
            return false;
        }

        /**
        * Translate "this" by vector v
        * validity of the Rectangle is tested in debubg
        */
        void translate(const tlp::Vector<Obj,2> &v) {
            assert(isValid());
            (*this)[0] += v;
            (*this)[1] += v;
        }
    };
    /*@}*/
}
#endif
