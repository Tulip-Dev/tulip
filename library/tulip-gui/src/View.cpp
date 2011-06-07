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
#include "tulip/View.h"

using namespace std;

namespace tlp {
  bool View::createPicture(const std::string& pictureName, int width, int height, bool center, int zoom, int xOffset, int yOffset) {
    return savePicture(pictureName,width,height,center,zoom,xOffset,yOffset);
  }

  bool View::savePicture(const std::string& , int , int , bool , int , int , int ){
    return false;
  }

  QImage View::createPicture(int, int, bool, int, int, int) {
    return QImage();
  }
}
