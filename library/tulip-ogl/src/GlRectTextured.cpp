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
#include "tulip/GlRectTextured.h"

using namespace std;

namespace tlp {
  GlRectTextured::GlRectTextured() {
    cout << "GlRectTextured deprecated : use Gl2DRect or GlRect instead "<< endl;
  }

  GlRectTextured::GlRectTextured(float top,float bottom,float left, float right,
				 const std::string& textureName,
				 bool inPercent)
    :Gl2DRect(top,bottom,left,right,textureName,inPercent)
  {
    cout << "GlRectTextured deprecated : use Gl2DRect or GlRect instead "<< endl;
  }

  GlRectTextured::GlRectTextured(float bottom,float left,float height, float width,const std::string& textureName,bool xInv, bool yInv)
    :Gl2DRect(bottom,left,height,width,textureName,xInv,yInv)
  {
    cout << "GlRectTextured deprecated : use Gl2DRect or GlRect instead "<< endl;
  }
}
