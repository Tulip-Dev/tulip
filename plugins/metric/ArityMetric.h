/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#ifndef _ARITYMETRIC_H
#define _ARITYMETRIC_H

#include <tulip/TulipPlugin.h>

/** \addtogroup metric */
/*@{*/
/** This plugin compute the degree of each node
 *
 *  \note This plug-in exists to obtain a uniform interface in the 
 *  Tulip graph visualization software. To access to the degree
 *  of a node it is recommended to use directly the degree function
 *  available in each graph.
 *
 */
class ArityMetric:public tlp::DoubleAlgorithm { 
public:
  ArityMetric(const tlp::PropertyContext &);
  bool run();
};
/*@}*/
#endif
