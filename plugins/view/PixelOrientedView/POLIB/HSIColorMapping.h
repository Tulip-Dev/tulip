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

#ifndef HSICOLORMAPPING_H_
#define HSICOLORMAPPING_H_

#include "HSIColorSpace.h"

#include "ColorFunction.h"

namespace pocore {

class HSIColorMapping : public ColorFunction {

public :

  HSIColorMapping(const HSI& from = HSI(1.5, 1.0, 0.4), const HSI& to = HSI(1.0, 1.0, 1.0));

  // value must be normalized
  RGBA getColor(const double& value, const unsigned int) const;

private :

  HSIColorScale hsiColorScale;

};

}
#endif /* HSICOLORMAPPING_H_ */

