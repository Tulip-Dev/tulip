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
#include "ReadGraph.h"
#include "RepresentExport.h"

#include <tulip/GlyphManager.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include <tulip/GlGraphStaticData.h>
#include <tulip/DrawingTools.h>
#include <tulip/TlpQtTools.h>
#include <tulip/StringProperty.h>
#include <tulip/GlEdge.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlGraphRenderingParameters.h>

using namespace std;
using namespace tlp;


// Director
ReadGraph::ReadGraph(Graph *graph, tlp::DataSet *ds, tlp::PluginProgress *pp, RepresentExport *r):_representExport(r) {

  // We declare the properties
  LayoutProperty *layout = graph->getProperty<LayoutProperty>("viewLayout");
  ColorProperty *colors = graph->getProperty<ColorProperty>("viewColor");
  IntegerProperty *shape =graph->getProperty<IntegerProperty>("viewShape");
  IntegerProperty *srcanchorshape =graph->getProperty<IntegerProperty>("viewSrcAnchorShape");
  IntegerProperty *tgtanchorshape =graph->getProperty<IntegerProperty>("viewTgtAnchorShape");
  SizeProperty  *sizes = graph->getProperty<SizeProperty>("viewSize");
  StringProperty *label = graph->getProperty<StringProperty>("viewLabel");
  ColorProperty *labelcolor = graph->getProperty<ColorProperty>("viewLabelColor");
  ColorProperty *bordercolor = graph->getProperty<ColorProperty>("viewBorderColor");
  DoubleProperty *borderwidth = graph->getProperty<DoubleProperty>("viewBorderWidth");
  DoubleProperty *rotation =  graph->getProperty<DoubleProperty>("viewRotation");
  bool edge_color_interpolation = false;
  bool edge_extremities = false;

  if(ds!=NULL) {
    ds->get("Element's layout property", layout);
    ds->get("Element's color property", colors);
    ds->get("Element's shape property", shape);
    ds->get("Element's SrcAnchorShape property", srcanchorshape);
    ds->get("Element's TgtAnchorShape property", tgtanchorshape);
    ds->get("Element's size property", sizes);
    ds->get("Element's label property", label);
    ds->get("Element's label color property", labelcolor);
    ds->get("Element's border color property", bordercolor);
    ds->get("Element's border width property", borderwidth);
    ds->get("Element's rotation property", rotation);
    ds->get("Edge color interpolation", edge_color_interpolation);
    ds->get("Edge extremities", edge_extremities);
  }

  // Finding graph's size
  BoundingBox graphbb = tlp::computeBoundingBox(graph,layout, sizes, rotation);

  // Writing the header of the file
  r->writeHeader(graphbb);
  r->writeGraph(graphbb);

  // We will start by analysing the edges for a better display of the image : the foreground and the background
  int nb_elements = graph->numberOfEdges()+graph->numberOfNodes();
  unsigned i=0;

  // Analysing edges
  treatEdges(graph, pp, r, i,  nb_elements, sizes, colors, layout, shape, srcanchorshape, tgtanchorshape, label, labelcolor, edge_color_interpolation, edge_extremities);

  // Analysing nodes
  std::vector<tlp::node> metanodeVertices;
  treatNodes(graph, pp, r, i,  nb_elements, sizes, colors, layout, shape, rotation, borderwidth, label, labelcolor, bordercolor, metanodeVertices);



  // Analysing meta-nodes

  std::vector<int> transformationVertices;
  int indice_Transform = 0;
  unsigned sizeFirstVertice = 2* metanodeVertices.size();

  while(!metanodeVertices.empty()) {
    std::vector<tlp::node> subMetanodeVertices;

    for(vector<node>::const_iterator it = metanodeVertices.begin(); it != metanodeVertices.end(); ++it) {
      node metanode = *it;
      Graph *metagraph = graph->getNodeMetaInfo(metanode);
      BoundingBox metagraphbb = tlp::computeBoundingBox(metagraph, layout, sizes, rotation);
      Coord coord_meta_node = layout->getNodeValue(metanode);
      Size size_meta_node = sizes->getNodeValue(metanode);

      // We compute the scale
      float scale = min(size_meta_node.width()/(metagraphbb.width()*1.4),size_meta_node.height()/(metagraphbb.height()*1.4));

      if (scale >= 1)
        scale *= 0.64f;

      // We compute the transformation on X and add him to our vertice
      transformationVertices.push_back(-graphbb.center().getX() + graphbb.width()/2 - metagraphbb.center().getX()*(scale-1) + coord_meta_node.getX() - metagraphbb.center().getX());
      // We compute the transformation on Y and add him to our vertice
      transformationVertices.push_back(graphbb.center().getY() + graphbb.height()/2 + metagraphbb.center().getY()*(scale-1) - coord_meta_node.getY() + metagraphbb.center().getY());

      vector<int>::const_iterator tran = transformationVertices.begin();

      r->writeMetaGraph(tran[indice_Transform], tran[indice_Transform+1], scale);
      indice_Transform +=2;

      // Analysing edges in the metanode
      treatEdges(metagraph, pp, r, i,  nb_elements, sizes, colors, layout, shape, srcanchorshape, tgtanchorshape, label, labelcolor, edge_color_interpolation, edge_extremities);

      // Analysing nodes in the metanode
      treatNodes(metagraph, pp, r, i,  nb_elements, sizes, colors, layout, shape, rotation, borderwidth, label, labelcolor, bordercolor, subMetanodeVertices);

      if(transformationVertices.size() > sizeFirstVertice)
        cerr << "Metanode in a metanode not working properly" << endl;
    }

    indice_Transform = 0;
    metanodeVertices = subMetanodeVertices;
  }

  // Writing the end of the file
  r->writeEnd();
}

void ReadGraph::treatEdges(Graph *graph, tlp::PluginProgress *pp, RepresentExport *r, unsigned &i, const int nb_elements, tlp::SizeProperty *sizes, tlp::ColorProperty *colors, tlp::LayoutProperty *layout, tlp::IntegerProperty *shape,tlp::IntegerProperty *srcanchorshape, tlp::IntegerProperty *tgtanchorshape, tlp::StringProperty *label,tlp::ColorProperty *labelcolor, bool edge_color_interpolation, bool edge_extremities) {
  pp->setComment("Exporting edges...");
  r->groupEdge();

  edge e;
  unsigned int id_src_shape = 0;
  unsigned int id_tgt_shape = 0;
  unsigned int id_src_grad = 0;
  unsigned int id_tgt_grad = 0;
  /*unsigned int& id_src_gradient = id_src_grad;
    unsigned int& id_tgt_gradient = id_tgt_grad;*/
  tlp::GlGraphRenderingParameters rp;
  tlp::GlGraphInputData inputData(graph, &rp);
  GlEdge glEdge(0);
  forEach(e, graph->getEdges()) {
    pp->progress(++i,nb_elements);
    const pair<node, node>& ends = graph->ends(e);
    r->startEdge(e.id);
    Size s = sizes->getEdgeValue(e);

    glEdge.id = e.id;
    std::vector<Coord> edgeVertices;
    glEdge.getVertices(&inputData, edgeVertices);


    // Edges extremities
    EdgeExtremityShape::EdgeExtremityShapes src_anchor_shape_type = EdgeExtremityShape::None;
    EdgeExtremityShape::EdgeExtremityShapes tgt_anchor_shape_type = EdgeExtremityShape::None;

    if(edge_extremities) {
      src_anchor_shape_type = static_cast<EdgeExtremityShape::EdgeExtremityShapes>(srcanchorshape->getEdgeValue(e));
      tgt_anchor_shape_type = static_cast<EdgeExtremityShape::EdgeExtremityShapes>(tgtanchorshape->getEdgeValue(e));
    }

    if(src_anchor_shape_type != EdgeExtremityShape::None || tgt_anchor_shape_type != EdgeExtremityShape::None) {
      r->exportEdgeExtremity(id_src_shape, id_tgt_shape, src_anchor_shape_type, tgt_anchor_shape_type, colors->getEdgeValue(e), id_src_grad, id_tgt_grad, edgeVertices[0], edgeVertices[edgeVertices.size() - 1], sizes->getNodeValue(ends.first), sizes->getNodeValue(ends.second));
    }

    // Get edge type
    if(!edge_color_interpolation) {
      r->exportEdge (static_cast<EdgeShape::EdgeShapes>(shape->getEdgeValue(e)),
                     layout->getEdgeValue(e),
                     colors->getEdgeValue(e),
                     ((s.getH()+s.getW())/2)+1,
                     src_anchor_shape_type,
                     id_src_shape,
                     tgt_anchor_shape_type,
                     id_tgt_shape,
                     edgeVertices
                    );
    }
    else {
      r->exportEdge (e.id,
                     static_cast<EdgeShape::EdgeShapes>(shape->getEdgeValue(e)),
                     layout->getEdgeValue(e),
                     colors->getNodeValue(ends.first),
                     colors->getNodeValue(ends.second),
                     ((s.getH()+s.getW())/2)+1,
                     src_anchor_shape_type,
                     id_src_shape,
                     tgt_anchor_shape_type,
                     id_tgt_shape,
                     edgeVertices
                    );
    }

    Coord c = edgeVertices[edgeVertices.size()/2] + edgeVertices[edgeVertices.size()/2 - 1];
    r->addLabel("edge", label->getEdgeValue(e), labelcolor->getEdgeValue(e), c/=2, sizes->getEdgeValue(e));
    r->endEdge();

    if(src_anchor_shape_type != EdgeExtremityShape::None)
      ++id_src_shape;

    if(tgt_anchor_shape_type != EdgeExtremityShape::None)
      ++id_tgt_shape;

    if(src_anchor_shape_type == EdgeExtremityShape::Sphere)
      ++id_src_grad;

    if(src_anchor_shape_type == EdgeExtremityShape::GlowSphere)
      id_src_grad +=2;

    if(tgt_anchor_shape_type == EdgeExtremityShape::Sphere)
      ++id_tgt_grad;

    if(tgt_anchor_shape_type == EdgeExtremityShape::GlowSphere)
      id_tgt_grad +=2;
  }

  // Ending the group of edges
  r->endGroupEdge();
}

void ReadGraph::treatNodes(Graph *graph, tlp::PluginProgress *pp, RepresentExport *r, unsigned &i, const int nb_elements, tlp::SizeProperty *sizes, tlp::ColorProperty *colors, tlp::LayoutProperty *layout, tlp::IntegerProperty *shape, tlp::DoubleProperty *rotation, tlp::DoubleProperty *borderwidth, tlp::StringProperty *label,tlp::ColorProperty *labelcolor, tlp::ColorProperty *bordercolor, std::vector<tlp::node> &metanodeVertices) {
  pp->setComment("Exporting nodes...");
  r->groupNode();
  node n;

  forEach(n, graph->getNodes()) {
    if(graph->isMetaNode(n))
      metanodeVertices.push_back(n);

    Coord c = layout->getNodeValue(n);
    Size s = sizes->getNodeValue(n);
    pp->progress(++i,nb_elements);
    // Never change the call order of the methods below
    r->startNode(n.id);
    r->addColor(colors->getNodeValue(n));

    if(rotation->getNodeValue(n) != 0)
      r->addRotation(rotation->getNodeValue(n),c);

    if(borderwidth->getNodeValue(n) != 0)
      r->addBorder(bordercolor->getNodeValue(n), borderwidth->getNodeValue(n));

    r->addShape(static_cast<NodeShape::NodeShapes>(shape->getNodeValue(n)), c, s);
    r->addLabel("node", label->getNodeValue(n), labelcolor->getNodeValue(n), c, s);
    r->endNode();
  }

  // Ending the group of nodes
  r->endGroupNode();
  r->writeEndGraph();
}

void ReadGraph::getExport(std::ostream &os) const {
  _representExport->getData(os);
}

std::ostream& operator<<(std::ostream &os, const ReadGraph &r) {
  r.getExport(os);
  return os;
}
