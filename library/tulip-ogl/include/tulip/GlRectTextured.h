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

#ifndef Tulip_GLRECTTEXTURED_H
#define Tulip_GLRECTTEXTURED_H

#include "tulip/Gl2DRect.h"

namespace tlp {
  /** \addtogroup GlEntities
   * Create a rectangle with a texture
   */
  class TLP_GL_SCOPE GlRectTextured : public Gl2DRect {

  public:
    GlRectTextured();

    /**
     * Constructor : with top/bottom/left/right coords, textureName and if inPercent is true coords are in percent
     */
    GlRectTextured(float top,float bottom,float left, float right,
        const std::string& textureName,
        bool inPercent=false);

    /**
     * Constructor : with bottom/left height,width coords, textureName and if xInv/yInv is true coords viewport - coord
     */
    GlRectTextured(float bottom,float left,float height, float width,
           const std::string& textureName,
           bool xInv, bool yInv);

    virtual ~GlRectTextured() {}

  };
}
#endif
