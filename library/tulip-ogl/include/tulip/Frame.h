/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#ifndef _FRAME_H_
#define _FRAME_H_
#ifndef DOXYGEN_NOTFOR_DEVEL

namespace tlp {

struct Frame{
  virtual void draw(float w_max, float& w) const=0;
  virtual void getBoundingBox(float w_max, float& h, float&w) const=0;
  virtual ~Frame()=0;
};

}

#endif //DOXYGEN_NOTFOR_DEVEL
#endif

