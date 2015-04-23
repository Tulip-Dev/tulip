#ifndef READGRAPH_H
#define READGRAPH_H


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

#include <ostream>
#include <tulip/LayoutProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/SizeProperty.h>

class RepresentExport;

namespace tlp {
class Graph;
class DataSet;
class PluginProgress;
}

// Director
class ReadGraph {
  RepresentExport *_representExport;

  friend std::ostream& operator<<(std::ostream &os, const ReadGraph &r);
  // This method retrieve the result after the analysis
  void getExport(std::ostream &os) const;

public:
  ReadGraph(tlp::Graph *graph, tlp::DataSet *ds, tlp::PluginProgress *pp, RepresentExport *r);

private:
  void treatEdges(tlp::Graph *graph, tlp::PluginProgress *pp, RepresentExport *r,unsigned &i, const int nb_elements, tlp::SizeProperty *sizes, tlp::ColorProperty *colors, tlp::LayoutProperty *layout, tlp::IntegerProperty *shape, tlp::IntegerProperty *srcanchorshape, tlp::IntegerProperty *tgtanchorshape, tlp::StringProperty *label, tlp::ColorProperty *labelcolor, bool edge_color_interpolation, bool edge_extremities);
  void treatNodes(tlp::Graph *graph, tlp::PluginProgress *pp, RepresentExport *r, unsigned &i, const int nb_elements, tlp::SizeProperty *sizes, tlp::ColorProperty *colors, tlp::LayoutProperty *layout, tlp::IntegerProperty *shape, tlp::DoubleProperty *rotation, tlp::DoubleProperty *borderwidth, tlp::StringProperty *label,tlp::ColorProperty *labelcolor, tlp::ColorProperty *bordercolor, std::vector<tlp::node> &metanodeVertices);
};

std::ostream& operator<<(std::ostream &os, const ReadGraph &r);

#endif // READGRAPH_H
