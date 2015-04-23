/**
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
#ifndef ORIENTABLECOORD_H
#define ORIENTABLECOORD_H

#include <tulip/Coord.h>
class OrientableLayout;

class OrientableCoord: public tlp::Coord {
public:
  OrientableCoord(OrientableLayout* fatherParam, const float x = 0,
                  const float y = 0, const float z = 0);
  OrientableCoord(OrientableLayout* fatherParam, const tlp::Coord& v);

  void  set(const float x = 0, const float y = 0, const float z = 0);
  void  set(const tlp::Coord& v);

  void  setX(float x);
  void  setY(float y);
  void  setZ(float z);

  float getX() const;
  float getY() const;
  float getZ() const;

  void  setInvertedX(float x);
  void  setInvertedY(float y);
  void  setInvertedZ(float z);

  float getInvertedX() const;
  float getInvertedY() const;
  float getInvertedZ() const;

protected:
  OrientableLayout* father;
};

#endif
