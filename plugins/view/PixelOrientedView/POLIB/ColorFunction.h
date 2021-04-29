/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#ifndef COLORFUNCTION_H
#define COLORFUNCTION_H

#include "potypes.h"

namespace pocore {
class ColorFunction {
public:
  virtual ~ColorFunction() {}
  virtual RGBA getColor(const double &value, const unsigned int itemId) const = 0;
};
} // namespace pocore
#endif // COLORFUNCTION_H
