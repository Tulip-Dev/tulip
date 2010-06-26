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
#ifndef _RENDERER_H_
#define _RENDERER_H_
#ifndef DOXYGEN_NOTFOR_DEVEL

#include "Fonts.h"

namespace tlp {

class Renderer : public Fonts {
public:
  virtual ~Renderer() {}
  virtual void translate(float x, float y, float z) const =0;
  virtual void setColor(unsigned char r, unsigned char v, unsigned char b) const =0;
  virtual void drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const =0;
  void setMode(FontMode  m);
  FontMode getMode();
  void setDepth(float p);
  float getDepth();

 protected:  
  FontMode mode;
  float depth;
};

}
#endif //DOXYGEN_NOTFOR_DEVEL
#endif
