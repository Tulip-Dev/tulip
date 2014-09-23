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
#include "Orientation.h"

//====================================================================
orientationType maskFromPanel(bool reverseOrientation, bool reverseDirection) {
  if (reverseOrientation)
    if (reverseDirection)
      return orientationType (ORI_ROTATION_XY | ORI_INVERSION_HORIZONTAL);
    else
      return ORI_ROTATION_XY;
  else if (reverseDirection)
    return ORI_INVERSION_VERTICAL;
  else
    return ORI_DEFAULT;
}
