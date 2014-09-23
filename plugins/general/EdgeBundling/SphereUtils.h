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

#ifndef SPHERE_UTILS_H
#define SPHERE_UTILS_H

#include <tulip/Graph.h>
#include <tulip/LayoutProperty.h>

float centerOnOriginAndScale(tlp::Graph* graph, tlp::LayoutProperty * layout, float dist);

void addSphereGraph(tlp::Graph *graph, double radius);

void moveBendsToSphere(tlp::Graph *graph, float ray, tlp::LayoutProperty * layout);

#endif //SPHERE_UTILS_H
