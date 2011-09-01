/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#ifndef Tulip_GRAPH3_H
#define Tulip_GRAPH3_H

#include <string>

#include <tulip/Node.h>
#include <tulip/tulipconf.h>

namespace tlp {
class Graph;
class DataSet;
class PluginProgress;

/**
 * @*brief Finds the first node whose input degree equals 0.
 *
 * @deprecated not object-oriented, should be moved into Graph. Also, return an invalid node if no such node is found to simplify function.
 *
 * @param graph The graph on which to look for such a node.
 * @param n The first node with input degree of 0.
 * @return bool Whether such a node exists.
 **/
TLP_SCOPE tlp::node _DEPRECATED getSource(tlp::Graph *g, tlp::node &n);

/**
 * @brief Applies an algorithm plugin, identified by its name.
 * Algorithm plugins are subclasses of the tlp::Algorithm interface.
 * Parameters are transmitted to the algorithm trough the DataSet.
 * To determine a plugin's parameters, you can either:
 *
 * * refer to its documentation
 *
 * * use buildDefaultDataSet on the plugin object if you have an instance of it
 *
 * * call getPluginParameters() with the name of the plugin on the right PluginLister
 * (there cannot be a static method that directly returns this from the plugin name as a name can be used for plugins of different types).
 *
 *
 * If an error occurs, a message describing the error should be stored in errorMessage.
 *
 * @param graph The graph to apply the algorithm on.
 * @param errorMessage A string that will be modified to contain an error message should an error occur.
 * @param dataSet The parameters to the algorithm. Defaults to NULL.
 * @param algorithm The algorithm to apply. Defaults to "any".
 * @param progress A PluginProgress to report the progress of the operation, as well as final state. Defaults to NULL.
 * @return bool Whether the algorithm was successfully applied.
 **/
TLP_SCOPE bool _DEPRECATED applyAlgorithm(Graph *graph, std::string &errorMessage, DataSet *dataSet=NULL,const std::string &algorithm = std::string("any"), PluginProgress *progress=NULL);
}

#endif
