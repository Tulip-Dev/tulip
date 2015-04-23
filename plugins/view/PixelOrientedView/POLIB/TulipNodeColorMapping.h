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

#ifndef TULIPNODECOLORMAPPING_H_
#define TULIPNODECOLORMAPPING_H_

#include <tulip/Graph.h>
#include <string>

#include "ColorFunction.h"
#include "potypes.h"


namespace pocore {

class TulipNodeColorMapping : public ColorFunction {

public :

  TulipNodeColorMapping(tlp::Graph *graph);

  RGBA getColor(const double&, const unsigned int itemId) const;

private :

  tlp::Graph *graph;
};

}

#endif /* TULIPNODECOLORMAPPING_H_ */
