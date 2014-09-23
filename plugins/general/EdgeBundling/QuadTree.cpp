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

#include <tulip/MutableContainer.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/DoubleProperty.h>
#include "QuadTree.h"
using namespace tlp;
using namespace std;

void QuadTreeBundle::compute(Graph *g, double splitRatio, tlp::LayoutProperty *layout, tlp::SizeProperty *size) {
  QuadTreeBundle tmp;
  tmp.splitRatio = splitRatio;
  tmp.createQuadTree(g, layout, size);
}
//=====================================
node QuadTreeBundle::splitEdge(node a, node b) {
  const Coord &cA = layout->getNodeValue(a);
  const Coord &cB = layout->getNodeValue(b);
  Coord center = (cA + cB)/2.0f;
  center[2] = 0;
  Vec2D tmp;
  tmp[0] = center[0];
  tmp[1] = center[1];

  MapVecNode::const_iterator itn = mapN.find(tmp);

  if (itn != mapN.end()) {
    return itn->second;
  }

  node n  = graph->addNode();
  resultNode.push_back(n);
  layout->setNodeValue(n, center);
  mapN[tmp] = n;
  return n;
}
//=====================================
bool QuadTreeBundle::isIn(const Coord &p, const Coord &a, const Coord &b) {
  if (p[0] < a[0]) return false;

  if (p[0] > b[0]) return false;

  if (p[1] < a[1]) return false;

  if (p[1] > b[1]) return false;

  return true;
}
//=====================================
void QuadTreeBundle::elmentSplitting(const Coord &a, const Coord &b, const vector<node> &input,
                                     vector<node> &in, vector<node> &out
                                    ) {
  if (!((a[0] < b[0]) && (a[1] < b[1])))
    throw TulipException("2 nodes have the same position.\nTry to apply the \"Fast Overlap Removal\" algorithm before.");

  in.clear();
  out.clear();
  vector<node>::const_iterator it = input.begin();

  for(; it!= input.end(); ++it) {
    const Coord &tmp = layout->getNodeValue(*it);

    if (isIn(tmp, a,b))
      in.push_back(*it);
    else
      out.push_back(*it);
  }
}
//=====================================

static int iii = 0;

void QuadTreeBundle::recQuad(const node a, const node b, const node c, const node d,
                             const vector<node> &input
                            ) {


  const Coord& cA = layout->getNodeValue(a);
  const Coord& cC = layout->getNodeValue(c);

//  if (input.size() == 0) { // && (cA - cC).norm() < (minSize/splitRatio)) {
//    //node n = graph->addNode();
//    //      resultNode.push_back(n);
//    //layout->setNodeValue(n, (cA + cC) / 2.0);
//    return;
//  }
//
//  if (input.size() == 1) { // && (cA - cC).norm() < (minSize/(splitRatio * 2.)) ) {
//    graph->addEdge(input[0], a);
//    graph->addEdge(input[0], b);
//    graph->addEdge(input[0], c);
//    graph->addEdge(input[0], d);
//    return;
//  }

  if ((input.size() == 0) && (cA - cC).norm() < (minSize/splitRatio)) {
    node n = graph->addNode();
    //    resultNode.push_back(n);
    layout->setNodeValue(n, (cA + cC) / 2.0f);
    return;
  }

  if ((input.size() == 1) && (cA - cC).norm() < (minSize/(splitRatio * 2.f))) {
    return;
  }

  node f = splitEdge( a, b );
  node g = splitEdge( b, c );
  node h = splitEdge( d, c );
  node i = splitEdge( a, d );



  const Coord& cF = layout->getNodeValue(f);
  const Coord& cG = layout->getNodeValue(g);
  const Coord& cI = layout->getNodeValue(i);

  //create nodes
  /**
      a---------b          a----f----b
      |         |          |    |    |
      |         |          |    |    |
      |         |          i----e----g
      |         |   =>     |    |    |
      |         |          |    |    |
      d---------c          d----h----c
   */
  node e = graph->addNode();
  resultNode.push_back(e);
  Coord cE = (cI + cG)/2.0f;
  cE[2] = 0;
  layout->setNodeValue(e, cE);

  Vec2D tmp;
  tmp[0] = cE[0];
  tmp[1] = cE[1];
  mapN[tmp] = e;

  /**
      a---------b          a----f----b
      |         |          |    |    |
      |         |          |    |    |
      |         |          i----e----g
      |         |   =>     |    |    |
      |         |          |    |    |
      d---------c          d----h----c
   */


//  graph->addEdge(a, f);
//  graph->addEdge(f, b);
//  graph->addEdge(b, g);
//  graph->addEdge(g, c);
//  graph->addEdge(c, h);
//  graph->addEdge(h, d);
//  graph->addEdge(d, i);
//  graph->addEdge(i, a);
//  graph->addEdge(i, e);
//  graph->addEdge(e, g);
//  graph->addEdge(f, e);
//  graph->addEdge(e, h);

  //split elements in each cell
  vector<node> in, out, out2;

  elmentSplitting(cA, cE, input, in, out);

  ++iii;
  recQuad(a,f,e,i, in);
  //-----------
  elmentSplitting(cF, cG, out, in, out2);
  recQuad(f,b,g,e, in );
  //-----------
  elmentSplitting(cE, cC, out2, in, out);
  recQuad(e,g,c,h, in);
  //-----------
  recQuad(i,e,h,d, out);
}
//========================================================
void QuadTreeBundle::createQuadTree(Graph *graph, tlp::LayoutProperty *lay, tlp::SizeProperty *siz) {
  //create the border of the Quadtree
  nbNodesInOriginalGraph = graph->numberOfNodes();

  if (!lay)
    layout = graph->getProperty<LayoutProperty>("viewLayout");
  else
    layout = lay;

  if (!siz)
    size   = graph->getProperty<SizeProperty>("viewSize");
  else
    size = siz;

  rot = graph->getProperty<DoubleProperty>("viewRotation");
  ntype = graph->getProperty<DoubleProperty>("nodetype");
  this->graph = graph;
  //
  //  size->scale(Coord(1000.,1000.,1000.));
  //  layout->scale(Coord(1000.,1000.,1000.));
  //  layout->center();
  BoundingBox bb = tlp::computeBoundingBox(graph, layout, size, rot);

  //change the bbbox to have a aspect ratio of 1
  float width  = bb[1][0] - bb[0][0];
  float height = bb[1][1] - bb[0][1];
  bb[0][0] -= width / 10.;
  bb[1][0] += width / 10.;

  bb[0][1] -= height / 10.;
  bb[1][1] += height / 10.;

  minSize = (bb[1] - bb[0]).norm();

  if (width > height) {
    double ratio = width / height;
    double center = (bb[1][1] + bb[0][1]) / 2.;
    bb[1][1] = (bb[1][1] - center) * ratio + center;
    bb[0][1]  = (bb[0][1] - center) * ratio + center;
  }

  if (width < height) {
    double ratio = height / width;
    double center = (bb[1][0] + bb[0][0]) / 2.;
    bb[1][0] = (bb[1][0] - center) * ratio + center;
    bb[0][0]  = (bb[0][0] - center) * ratio + center;
  }


  vector<node> input;
  node k;
  forEach(k, graph->getNodes()) {
    input.push_back(k);
  }


  node a = graph->addNode();
  node b = graph->addNode();
  node c = graph->addNode();
  node d = graph->addNode();

//  graph->addEdge(a, b);
//  graph->addEdge(b, c);
//  graph->addEdge(c, d);
//  graph->addEdge(d, a);

  /*
  resultNode.push_back(a);
  resultNode.push_back(b);
  resultNode.push_back(c);
  resultNode.push_back(d);
   */
  assert(bb[0][0] < bb[1][0]);
  assert(bb[0][1] < bb[1][1]);

  layout->setNodeValue(a, Coord(bb[0][0], bb[0][1], 0));
  layout->setNodeValue(c, Coord(bb[1][0], bb[1][1], 0));

  Coord cB(bb[1][0], bb[0][1] , 0);
  Coord cD(bb[0][0] , bb[1][1], 0);

  layout->setNodeValue(b, cB);
  layout->setNodeValue(d, cD);

  recQuad(a, b, c, d, input);



  for(size_t i = 0; i<resultNode.size(); ++i) {
    graph->delNode(resultNode[i], true);
  }


}
