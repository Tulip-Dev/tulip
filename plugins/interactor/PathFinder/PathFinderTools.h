/*
 * PathFinderTools.h
 *
 *  Created on: Apr 14, 2010
 *      Author: Ludwig Fiolka
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
}

/**
 * Computes the enclosing circle of a graph's node using the layout,size etc properties specified in the input data.
 * @param circle The resulting circle.
 * @param inputData The graph's input data
 * @param n the node.
 * @return true if the circle has been successfully computed.
 */
bool getNodeEnclosingCircle(tlp::Circlef &circle, tlp::GlGraphInputData *inputData, tlp::node n);

/**
 * Computes the enclosing circle of a graph's edge bends using the layout,size etc properties specified in the input data.
 * @param circle The resulting circle.
 * @param inputData The graph's input data
 * @param n the node.
 * @return true if the circle has been successfully computed.
 */
bool getEdgeEnclosingCircle(tlp::Circlef &circle, tlp::GlGraphInputData *inputData, tlp::edge n);

#endif /* PATHFINDERTOOLS_H_ */
