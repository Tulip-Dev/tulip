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

  /// Class for coordinates in 3D
  class TLP_SCOPE Coord : public tlp::VectorCast<float, 3, tlp::Coord> {
  public:
    // default constructor (x,y,z set to 0)
    //   inline Coord() {set(0,0,0);}//{ memset(array, 0, sizeof(array)); }
    // this constructor gives the same value to x,y,z
    //    inline Coord(const float);

    inline explicit Coord(const tlp::Vector<float,3> &v):VectorCast<float, 3, tlp::Coord>(v) {
    }

    inline explicit Coord(const float xx = 0, const float yy = 0, const float zz = 0)
    {set(xx, yy, zz);}

    inline void set(const float xx= 0.f,
		    const float yy= 0.f,
		    const float zz= 0.f)
    {(*this)[0] = xx; (*this)[1] = yy, (*this)[2] = zz;}

    inline void set(const Coord& c) {
      (*this) = c;
    }

    inline void setX(float xx) {(*this)[0]=xx;}
    inline void setY(float yy) {(*this)[1]=yy;}
    inline void setZ(float zz) {(*this)[2]=zz;}

    inline float getX() const {return (*this)[0];}
    inline float getY() const {return (*this)[1];}
    inline float getZ() const {return (*this)[2];}
    inline void get(float &xx,float &yy,float &zz) const {
      xx=(*this)[0];yy=(*this)[1];zz=(*this)[2];
    }
  };
}

#endif
