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

#include <tulip/NumericProperty.h>
#include <tulip/SortIterator.h>

using namespace tlp;

Iterator<node> *NumericProperty::getSortedNodes(const Graph *sg, bool ascendingOrder) {
  if (!sg)
    sg = getGraph();

  return new SortNodeIterator(sg->getNodes(), this, ascendingOrder);
}

Iterator<edge> *NumericProperty::getSortedEdges(const Graph *sg, bool ascendingOrder) {
  if (!sg)
    sg = getGraph();

  return new SortEdgeIterator(sg->getEdges(), this, ascendingOrder);
}

Iterator<edge> *NumericProperty::getSortedEdgesBySourceValue(const Graph *sg, bool ascendingOrder) {
  if (!sg)
    sg = getGraph();

  return new SortSourceEdgeIterator(sg->getEdges(), sg, this, ascendingOrder);
}

Iterator<edge> *NumericProperty::getSortedEdgesByTargetValue(const Graph *sg, bool ascendingOrder) {
  if (!sg)
    sg = getGraph();

  return new SortTargetEdgeIterator(sg->getEdges(), sg, this, ascendingOrder);
}

Iterator<edge> *NumericProperty::getSortedEdgesByExtremitiesValues(const Graph *sg,
                                                                   bool ascendingOrder) {
  if (!sg)
    sg = getGraph();

  return new SortExtremitiesEdgeIterator(sg->getEdges(), sg, this, ascendingOrder);
}
