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
#ifndef TULIP_COORD_H
#define TULIP_COORD_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <tulip/tulipconf.h>
#include <tulip/VectorCast.h>

namespace tlp {

  /**
   * @brief This class represents a point in 3-D space, with float precision.
   **/
  class TLP_SCOPE Coord : public tlp::VectorCast<float, 3, tlp::Coord> {
  public:
    /**
    * @brief Copy constructor. 
    *
    * @param v A tlp::Coord to copy.
    **/
    inline explicit Coord(const tlp::Vector<float,3> &v):VectorCast<float, 3, tlp::Coord>(v) {
    }

    /**
    * @brief Default constructor. The values default on 0 if not filled.
    *
    * @param xx The X value of this Coord. Defaults to 0.
    * @param yy The Y value of this Coord. Defaults to 0.
    * @param zz The Z value of this Coord. Defaults to 0.
    **/
    inline explicit Coord(const float xx = 0, const float yy = 0, const float zz = 0) {
      set(xx, yy, zz);
    }

    /**
    * @brief Sets the value of the X, Y, and Z value of this Coordinate.
    *
    * @param xx The X value of this Coord. Defaults to 0.f.
    * @param yy The Y value of this Coord. Defaults to 0.f.
    * @param zz The Z value of this Coord. Defaults to 0.f.
    * @return void
    **/
    inline void set(const float xx= 0.f, const float yy= 0.f, const float zz= 0.f) {
      (*this)[0] = xx;
      (*this)[1] = yy;
      (*this)[2] = zz;
    }

    /**
    * @brief Copy the values of the parameter coordinate into this coordinate.
    *
    * @param c A Coord to copy.
    * @return void
    **/
    inline void set(const Coord& c) {
      (*this) = c;
    }

    inline void setX(float xx) {(*this)[0]=xx;}
    inline void setY(float yy) {(*this)[1]=yy;}
    inline void setZ(float zz) {(*this)[2]=zz;}

    inline float getX() const {return (*this)[0];}
    inline float getY() const {return (*this)[1];}
    inline float getZ() const {return (*this)[2];}
    
    /**
     * @brief Modifies the values of the parameters to the vaules of this Coordinate's composants.
     *
     * @param xx This Coord's X value.
     * @param yy This Coord's Y value.
     * @param zz This Coord's Z value.
     * @return void
     **/
    inline void get(float &xx,float &yy,float &zz) const {
      xx=(*this)[0];yy=(*this)[1];zz=(*this)[2];
    }
  };

#ifdef _MSC_VER //visual studio needs to export all uses of dllExport (TLP_SCOPE) classes
  template class TLP_SCOPE VectorCast<float, 3, Coord>;
  template struct TLP_SCOPE Array<tlp::Coord,4>;
#endif
}

#endif
