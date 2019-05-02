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
#ifndef _ECCENTRICITYMETRIC_H
#define _ECCENTRICITYMETRIC_H

#include <tulip/DoubleProperty.h>
#include <tulip/NumericProperty.h>

/** \addtogroup metric */

/** This plugin compute the eccentricity/closeness centrality of each node
 *
 * Eccentricity is the maximum distance to go from a node to all others.
 * In this version the Eccentricity value can be normalized
 * (1 means that a node is one of the most eccentric in the network,
 * 0 means that a node is on the centers of the network).
 *
 * More information  about the use of eccentricity metric can be found in :
 *
 * Visone: Analysis and visualization of social networks. \n
 * "Book. Graph Drawing Software. (Ed. Michael Junger & Petra Mutzel", \n
 * Authors : Ulrik Brandes and Dorothea Wagner. \n
 * "2004", \n
 * pages 321-340.
 *
 *
 * Closeness Centrality is the mean of shortest-paths lengths from a node to others.
 * The normalized values are computed using the reciprocal of the sum of these distances
 * (see "http://en.wikipedia.org/wiki/Closeness_(graph_theory)#Closeness_centrality" for more
 * details).
 *
 *  \note The complexity of the algorithm is O(|V| * |E|) time and O(1) space
 *        for unweighted graphs and O(|V| * |E| \log |V|) time for weighted graphs.
 *
 *   <b>HISTORY</b>
 *
 *   - 18/06/2004 Version 2.0: Normalisation and Closeness Centrality
 *   - 27/04/2019 Version 2.1: Weighted version
 */
class EccentricityMetric : public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION("Eccentricity", "Auber/Munzner", "18/06/2004",
                    "Computes the eccentricity/closeness centrality of each node.<br>"
                    "<b>Eccentricity</b> is the maximum distance to go from a node to all others. "
                    "In this version the Eccentricity value can be normalized (1 means that a node "
                    "is one of the most eccentric in the network, 0 means that a node is on the "
                    "centers of the network).<br>"
                    "<b>Closeness Centrality</b> is the mean of shortest-paths lengths from a node "
                    "to others. The normalized values are computed using the reciprocal of the sum "
                    "of these distances.",
                    "2.1", "Graph")
  EccentricityMetric(const tlp::PluginContext *context);
  ~EccentricityMetric() override;
  bool run() override;
  double compute(unsigned int nPos);

private:
  bool allPaths;
  bool norm;
  bool directed;
  tlp::NumericProperty *weight;
};

#endif
