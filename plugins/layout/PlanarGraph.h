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
#include <vector>
class PlanarConMap;
class SuperGraph;

namespace tlp {
/**
 * This function triangulate a planar graph. (ie. add edges in the graph until
 * each face of the planar map contains only 3 edges.
 * The input graph must be planr in order to call this function.
 */
void triangulate(SuperGraph *map, std::vector<edge>& addedEdges);
/**
 * Build the canonical ordering of a planar triangulated graph. For information
 * about canonical ordering the user must reffer to the planar graph drawing book.
 */
void canonicalOrdering(SuperGraph *graph, std::vector<node> &order);
}

