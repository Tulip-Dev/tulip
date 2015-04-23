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

#ifndef PATHFINDERTOOLS_H_
#define PATHFINDERTOOLS_H_

#include <tulip/Circle.h>
#include <tulip/Node.h>
#include <tulip/Edge.h>

namespace tlp {
class GlGraphInputData;
class SizeProperty;
class LayoutProperty;


/**
 * Computes the enclosing circle of a graph's node using the layout,size etc properties specified in the input data.
 * @param circle The resulting circle.
 * @param inputData The graph's input data
 * @param n the node.
 * @return true if the circle has been successfully computed.
 */
bool getNodeEnclosingCircle(Circlef &circle, GlGraphInputData *inputData, node n);

/**
 * Computes the enclosing circle of a graph's edge bends using the layout,size etc properties specified in the input data.
 * @param circle The resulting circle.
 * @param inputData The graph's input data
 * @param n the node.
 * @return true if the circle has been successfully computed.
 */
bool getEdgeEnclosingCircle(Circlef &circle, GlGraphInputData *inputData, edge n);
}
#endif /* PATHFINDERTOOLS_H_ */
