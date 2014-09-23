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
#ifndef ORIENTABLECONSTANT_H
#define ORIENTABLECONSTANT_H

enum orientationType {
  ORI_DEFAULT                     = 0,
  ORI_INVERSION_HORIZONTAL        = 1,
  ORI_INVERSION_VERTICAL          = 2,
  ORI_INVERSION_Z                 = 4,
  ORI_ROTATION_XY                 = 8
};

enum {
  UP_DOWN     = 0,
  DOWN_UP     = 1,
  RIGHT_LEFT  = 2,
  LEFT_RIGHT  = 3
};

orientationType maskFromPanel(bool reverseOrientation, bool reverseDirection);

#endif
