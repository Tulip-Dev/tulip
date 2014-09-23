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

#ifndef HILBERTLAYOUT_H
#define HILBERLAYOUT_H

#include <string>

#include "LayoutFunction.h"

namespace pocore {
class HilbertLayout : public LayoutFunction {
public:
  HilbertLayout(unsigned char order);
  Vec2i  project (const unsigned int id) const;
  unsigned int unproject (const Vec2i& ) const;
private:
  unsigned char order;
  int shift;
};
}
#endif //HILBERTLAYOUT_H
