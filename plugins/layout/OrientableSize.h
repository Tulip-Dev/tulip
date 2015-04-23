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
#ifndef ORIENTABLESIZE_H
#define ORIENTABLESIZE_H

#include <tulip/Size.h>
class OrientableSizeProxy;

class OrientableSize: public tlp::Size {
public:

  OrientableSize(OrientableSizeProxy* fatherParam, const float width = 0,
                 const float height = 0, const float depth = 0);
  OrientableSize(OrientableSizeProxy* fatherParam, const tlp::Size& size);

  void set(const float width = 0, const float height = 0,
           const float depth = 0);
  void set(const tlp::Size& size);

  void setW(const float width);
  void setH(const float height);
  void setD(const float depth);

  float getW() const;
  float getH() const;
  float getD() const;

  void get(float* width, float* height, float* depth) const;

protected:
  OrientableSizeProxy* father;
};

#endif
