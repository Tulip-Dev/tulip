/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#include "SphereUtils.h"

#include <tulip/DrawingTools.h>
#include <tulip/SizeProperty.h>
#include <tulip/DoubleProperty.h>

using namespace tlp;
using namespace std;

float centerOnOriginAndScale(Graph *graph, LayoutProperty *layout, float dist) {
  graph->getProperty<SizeProperty>("viewSize")->setAllNodeValue(Size(0, 0, 0));
  BoundingBox bb =
      tlp::computeBoundingBox(graph, graph->getProperty<LayoutProperty>("viewLayout"),
                              graph->getProperty<SizeProperty>("viewSize"),
                              graph->getProperty<DoubleProperty>("viewRotation"), nullptr);
  Coord &&move_coord = (bb[0] + bb[1]) / (-2.f);
  layout->translate(move_coord, graph);
  float ray = (move_coord - bb[1]).norm();
  float scaleFactor = dist / ray;
  layout->scale(Coord(scaleFactor, scaleFactor, scaleFactor), graph);
  graph->getProperty<SizeProperty>("viewSize")->setAllNodeValue(Size(0.1f, 0.1f, 0.1f));
  return sqrt(ray * ray / 2.);
}

void moveBendsToSphere(Graph *graph, float ray, LayoutProperty *layout) {
  for (auto e : graph->edges()) {
    vector<Coord> bends;
    bends = layout->getEdgeValue(e);

    for (size_t i = 0; i < bends.size(); ++i) {
      Coord &c = bends[i];
      double dist = c.norm();
      c /= dist;
      c *= ray;
    }

    layout->setEdgeValue(e, bends);
  }

  for (auto n : graph->nodes()) {
    Coord c = layout->getNodeValue(n);
    c /= c.norm();
    c *= ray;
    layout->setNodeValue(n, c);
  }
}

static Coord getCoordFromPolar(double rayon, double a1, double a2) {
  a1 = a1 * M_PI / 2. / 90.;
  a2 = a2 * M_PI / 2. / 90.;
  float x, y, z;
  x = rayon * cos(a1) * sin(a2);
  y = rayon * sin(a1) * sin(a2);
  z = rayon * cos(a2);
  return Coord(x, y, z);
}

void addSphereGraph(Graph *graph, double radius) {
  LayoutProperty *layout = graph->getProperty<LayoutProperty>("viewLayout");
  double rho = 0;

  while (rho < 360) {
    double teta = 5;

    while (teta < 180) {
      layout->setNodeValue(graph->addNode(), getCoordFromPolar(radius, rho, teta));
      teta += 5.;
    }

    rho += 5.;
  }

  layout->setNodeValue(graph->addNode(), getCoordFromPolar(radius, 0, 0));
  layout->setNodeValue(graph->addNode(), getCoordFromPolar(radius, 0, 180));
}
