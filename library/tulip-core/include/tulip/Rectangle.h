/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
///@cond DOXYGEN_HIDDEN

//@TLPGEOLICENCE#
#ifndef TLP_RECTANGLE_H
#define TLP_RECTANGLE_H
#include <tulip/Vector.h>
#include <tulip/BoundingBox.h>

namespace tlp {

/**
 * @ingroup Structures
 * \brief class for rectangle
 *
 * Enables to both create and manipulate a 2D Axis Aligned Rectangle
 *
 * Author : <a href="www.tulip-software.org>Tulip team</a>
 */
template<typename Obj, typename OTYPE = double>
struct Rectangle : public Array<Vector<Obj,2, OTYPE>,2> {
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
    * \warning the rectangle must be valid (tested in debug mode)
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
    * \warning the rectangle must be valid (tested in debug mode)
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
  * \warning the rectangle must be valid (tested in debug mode)
  */
  Rectangle(const Vector<Obj,2, OTYPE> &min, const Vector<Obj,2, OTYPE> &max) {
    (*this)[0] = min;
    (*this)[1] = max;
    assert(isValid());
  }
  /**
         * @return true if r intersect "this".
         * \warning the rectangle must be valid (tested in debug mode)
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
       * \warning the rectangle must be valid (tested in debug mode)
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
  * \warning the rectangle must be valid (tested in debug mode)
  */
  bool isInside(const Vector<Obj, 2, OTYPE> &p) const {
    assert(isValid());

    if (p[0] > (*this)[1][0]) return false;

    if (p[0] < (*this)[0][0]) return false;

    if (p[1] > (*this)[1][1]) return false;

    if (p[1] < (*this)[0][1]) return false;

    return true;
  }
  /**
  * @return true if r is inside or equal to the AARectangle
  * \warning the rectangle must be valid (tested in debug mode)
  */
  bool isInside(const Rectangle &r) const {
    assert(isValid());
    assert(r.isValid());

    if ((*this)[0] == r[0] && (*this)[1] == r[1]) return true;

    if (this->isInside(r[0]) && this->isInside(r[1])) return true;

    return false;
  }

  /**
  * Translate "this" by vector v
  * \warning the rectangle must be valid (tested in debug mode)
  */
  void translate(const tlp::Vector<Obj,2, OTYPE> &v) {
    assert(isValid());
    (*this)[0] += v;
    (*this)[1] += v;
  }
  /**
    * Return the width of the rectangle
    * \warning the rectangle must be valid (tested in debug mode)
    */
  Obj width() const {
    assert(isValid());
    return (*this)[1][0] - (*this)[0][0];
  }
  /**
    * Return the height of the rectangle
    * \warning the rectangle must be valid (tested in debug mode)
    */
  Obj height() const {
    assert(isValid());
    return (*this)[1][1] - (*this)[0][1];
  }
  /**
    * Return the surface of the rectangle
    * \warning the rectangle must be valid (tested in debug mode)
    */
  Obj surface() const {
    assert(isValid());
    return height() * width();
  }
  /**
    * Return the aspect ratio of the reactangle
    * a value between [0..1]
    * \warning the rectangle must be valid (tested in debug mode)
    */
  Obj aspectRatio() const {
    assert(isValid());

    if (std::max(height(), width()) < std::numeric_limits<Obj>::epsilon())
      return 0.;

    return std::min(height(), width()) / std::max(height(), width());
  }
  /**
    * Return the center of a rectangle
    * \warning the rectangle must be valid (tested in debug mode)
    */
  Vector<Obj, 2, OTYPE> center() const {
    assert(isValid());
    return ((*this)[0] + (*this)[1]) / Obj(2);
  }

};

typedef Rectangle<double, long double> Rectd;
typedef Rectangle<float, double> Rectf;
typedef Rectangle<int, double> Recti;
typedef Rectangle<unsigned int, double> Rectui;



}
#endif
///@endcond
