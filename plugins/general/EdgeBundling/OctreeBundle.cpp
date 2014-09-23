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
#include <tulip/SimpleTest.h>

#include "OctreeBundle.h"

using namespace tlp;
using namespace std;

void OctreeBundle::compute(Graph *g, double splitRatio, tlp::LayoutProperty *layout, tlp::SizeProperty *size) {
  OctreeBundle tmp;
  tmp.splitRatio = splitRatio;
  tmp.createOctree(g, layout, size);
}
//=====================================
node OctreeBundle::splitEdge(node a, node b) {
  const Coord &cA = layout->getNodeValue(a);
  const Coord &cB = layout->getNodeValue(b);
  Coord center = (cA + cB)/2.0f;

  MapVecNode::const_iterator itn = mapN.find(center);

  if (itn != mapN.end()) {
    return itn->second;
  }

  node n  = graph->addNode();
  resultNode.push_back(n);
  layout->setNodeValue(n, center);
  mapN[center] = n;
  return n;
}
//=====================================
bool OctreeBundle::isIn(const Coord &p, const Coord &a, const Coord &b, const Coord &c, const Coord &) {
  if (p[0] < a[0]) return false;

  if (p[0] > b[0]) return false;

  if (p[1] < a[1]) return false;

  if (p[1] > b[1]) return false;

  if (p[2] < a[2]) return false;

  if (p[2] > c[2]) return false;

  return true;
}
//=====================================
void OctreeBundle::elmentSplitting(const Coord &a, const Coord &b, const Coord &c, const Coord &d,
                                   const vector<node> &input,
                                   vector<node> &in, vector<node> &out
                                  ) {
  if (!((a[0] < b[0]) && (a[1] < b[1])))
    throw TulipException("2 nodes have the same position.\nTry to apply the \"Fast Overlap Removal\" algorithm before.");

  in.clear();
  out.clear();
  vector<node>::const_iterator it = input.begin();

  for(; it!= input.end(); ++it) {
    const Coord &tmp = layout->getNodeValue(*it);

    if (isIn(tmp, a, b, c, d))
      in.push_back(*it);
    else
      out.push_back(*it);
  }
}
//=====================================
node OctreeBundle::addNode(const tlp::Coord& pos) {
  MapVecNode::const_iterator itn = mapN.find(pos);

  if(itn == mapN.end()) {
    node n = graph->addNode();
    layout->setNodeValue(n, pos);
    mapN[pos] = n;
    return n;
  }

  return itn->second;
}
//=====================================
//static int iii = 0;
void OctreeBundle::cleanNode(const tlp::node) {
  /*    double dir[6];
      edge   best[6];
      edge e;
      forEach(e, graph->getInOutEdges(n)) {
          node tgt = graph->opposite(e, n);
          Coord vecU = layout->getNodeValue(tgt) - layout->getNodeValue(n);
          vecU /= vecu.norm();
          if(fabs)
      }
      */
}

void OctreeBundle::recQuad(const Coord fr[4], const Coord ba[4], const vector<node> &input) {

  node frN[4];
  node baN[4];

  for (int i=0; i<4; ++i) {
    frN[i] = addNode(fr[i]);
    baN[i] = addNode(ba[i]);
  }


  for (int i=0; i<4; ++i) {
    graph->addEdge(frN[i], baN[i]);
  }

  for (int i=0; i<4; ++i) {
    graph->addEdge(frN[i], frN[(i+1)%4]);
    graph->addEdge(baN[i], baN[(i+1)%4]);
  }


  if (input.size() == 1 && (fr[0] - ba[2]).norm() < (minSize/(splitRatio * 2.)) ) {
    node src = input[0];

    for (int i=0; i<4; ++i) {
      graph->addEdge(src   , baN[i]);
      graph->addEdge(src  , frN[i]);

    }

    return;
  }

  if (input.size() == 0 && (fr[0] - ba[2]).norm() < (minSize/(splitRatio)) )
    return;

  for (int i=0; i<4; ++i) {
    unvalidEdges.push_back(pair<node, node>(frN[i] , baN[i]));
  }

  for (int i=0; i<4; ++i) {
    unvalidEdges.push_back(pair<node, node>(frN[i], frN[(i+1)%4]));
    unvalidEdges.push_back(pair<node, node>(baN[i], baN[(i+1)%4]));
  }

  //create nodes
  /**

      e---------f          e----n----f
     /|        /|         /|        /|      /----i----/
    a---------b |        a----i----b |           |
    | |       | |        | |  |    | o           |
    | |       | |        | |  |    | |      x----x----x
    | |       | |        l----m----j |           |
    | h ------|-g  =>    | h--|p---|-g           |
    |/        |/         |/   |    |/       /----x----/
    d---------c          d----k----c

  */
  /**
  a b c d e f g h
    0 1 2 3 4 5 6 7

  */
  /*
          fr[0]        fr[0]+fr[1]   fr[0]+fr[2]    fr[0]+fr[3]     ||   fr[0]+ba[0]   fr[0]+ba[1]   fr[0]+ba[2]   fr[0]+ba[3]
          fr[1]+fr[0]  fr[1]         fr[1]+fr[2]    fr[1]+fr[3]     ||   fr[1]+ba[0]   fr[1]+ba[1]   fr[1]+ba[2]   fr[1]+ba[3]
          fr[2]+fr[0]  fr[2]+fr[3]   fr[2]          fr[2] + fr[3]   ||   fr[2]+ba[0]   fr[2]+ba[1]   fr[2]+ba[2]   fr[2]+ba[3]
          fr[3]+fr[0]  fr[3]+fr[1]   fr[3]+fr[2]    fr[3]           ||   fr[3]+ba[0]   fr[3]+ba[1]   fr[3]+ba[2]   fr[3]+ba[3]

          fr[0]+ba[0]   fr[0]+ba[1]   fr[0]+ba[2]   fr[0]+ba[3]     ||   ba[0]         ba[0]+fr[1]   ba[0]+fr[2]   ba[0]+ba[3]
          fr[1]+ba[0]   fr[1]+ba[1]   fr[1]+ba[2]   fr[1]+ba[3]     ||   ba[1]+ba[0]   ba[1]         ba[1]+ba[2]   ba[1]+ba[3]
          fr[2]+ba[0]   fr[2]+ba[1]   fr[2]+ba[2]   fr[2]+ba[3]     ||   ba[2]+ba[0]   ba[2]+ba[3]   ba[2]         fr[2]+ba[3]
          fr[3]+ba[0]   fr[3]+ba[1]   fr[3]+ba[2]   fr[3]+ba[3]     ||   ba[3]+ba[0]   ba[3]+ba[1]   ba[3]+ba[2]   ba[3]


      oc1 => a,   (a+b)/2,      (a+c)/2,     (a+d)/2,   || (a+e)/2,    (a+f)/2,     (a+g) /2, (a+h)/2
      oc2 => b;   (b+c)/2,      (b+d)/2,     (b+a)/2    || (b+f)/2,    (b+g)/2,     (b+h) /3, (b+e)/2
      pc2 => c,   (c+d)/2,      (c+a)/2,     (c+b)/2
     */
  Coord frontFace[4][4];
  Coord middleFace[4][4];
  Coord backFace[4][4];

  for (int i=0; i<4; ++i) {
    frontFace[i][i]         =  fr[i];
    frontFace[i][(i+1)%4]   = (fr[i] + fr[(i+1)%4]) / 2.f;
    frontFace[i][(i+2)%4]   = (fr[i] + fr[(i+2)%4]) / 2.f;
    frontFace[i][(i+3)%4]   = (fr[i] + fr[(i+3)%4]) / 2.f;

    middleFace[i][i]        = (fr[i] + ba[i]) / 2.f;
    middleFace[i][(i+1)%4]  = (fr[i] + ba[(i+1)%4]) / 2.f;
    middleFace[i][(i+2)%4]  = (fr[i] + ba[(i+2)%4]) / 2.f;
    middleFace[i][(i+3)%4]  = (fr[i] + ba[(i+3)%4]) / 2.f;

    backFace[i][i]          =  ba[i];
    backFace[i][(i+1)%4]    = (ba[i] + ba[(i+1)%4]) / 2.f;
    backFace[i][(i+2)%4]    = (ba[i] + ba[(i+2)%4]) / 2.f;
    backFace[i][(i+3)%4]    = (ba[i] + ba[(i+3)%4]) / 2.f;
  }

  vector<node> in, out, input2;
  input2 = input;

  for (int i=0; i<4; ++i) {
    elmentSplitting(frontFace[i][0], frontFace[i][2], middleFace[i][0], middleFace[i][2], input2, in, out);
    recQuad(frontFace[i], middleFace[i], in);
    input2 = out;

    if (i!=3) {
      elmentSplitting(middleFace[i][0], middleFace[i][2], backFace[i][0], backFace[i][2], input2, in, out);
      input2 = out;
      recQuad(middleFace[i], backFace[i], in);
    }
    else
      recQuad(middleFace[i], backFace[i], out);
  }

//
//    //split elements in each cell
//    vector<node> in, out, out2;
//    elmentSplitting(cA, cE, input, in, out);
//
//    ++iii;
//    recQuad(a,f,e,i, in);
//    //-----------
//    elmentSplitting(cF, cG, out, in, out2);
//    recQuad(f,b,g,e, in );
//    //-----------
//    elmentSplitting(cE, cC, out2, in, out);
//    recQuad(e,g,c,h, in);
//    //-----------
//    recQuad(i,e,h,d, out);
}
//========================================================
void OctreeBundle::createOctree(Graph *graph, tlp::LayoutProperty *lay, tlp::SizeProperty *siz) {
  //create the border of the Quadtree
  nbNodesInOriginalGraph = graph->numberOfNodes();
  layout = graph->getProperty<LayoutProperty>("viewLayout");
  size   = graph->getProperty<SizeProperty>("viewSize");
  rot = graph->getProperty<DoubleProperty>("viewRotation");
  ntype = graph->getProperty<DoubleProperty>("nodetype");

  if (lay)
    layout = lay;

  if (siz)
    size = siz;

  this->graph = graph;
//
//  size->scale(Coord(1000.,1000.,1000.));
//  layout->scale(Coord(1000.,1000.,1000.));
//  layout->center();
  BoundingBox bb = tlp::computeBoundingBox(graph, layout, size, rot);

  //change the bbbox to have a aspect ratio of 1
  float width  = bb[1][0] - bb[0][0];
  float height = bb[1][1] - bb[0][1];
  float depth  = bb[1][2] - bb[0][2];

  bb[0][0] -= width / 10.;
  bb[1][0] += width / 10.;

  bb[0][1] -= height / 10.;
  bb[1][1] += height / 10.;

  bb[0][2]  -= depth / 10.;
  bb[1][2] += depth / 10.;

  minSize = (bb[1] - bb[0]).norm();
  /*
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

  */
  vector<node> input;
  node k;
  forEach(k, graph->getNodes()) {
    input.push_back(k);
  }

  Coord fr[4];
  Coord ba[4];
  fr[0] = Coord(bb[0]);
  fr[1] = Coord(bb[1][0], bb[0][1] , bb[0][2]);
  fr[2] = Coord(bb[1][0], bb[1][1], bb[0][2]);
  fr[3] = Coord(bb[0][0] , bb[1][1], bb[0][2]);

  ba[0] = Coord(bb[0][0] , bb[0][1] , bb[1][2]);
  ba[1] = Coord(bb[1][0], bb[0][1] , bb[1][2]);
  ba[2] = Coord(bb[1][0], bb[1][1], bb[1][2]);
  ba[3] = Coord(bb[0][0] , bb[1][1], bb[1][2]);

  recQuad(fr, ba , input);


  for(size_t i=0; i<unvalidEdges.size(); ++i) {
    node src = unvalidEdges[i].first;
    node tgt = unvalidEdges[i].second;
    edge e;

    if ((e=graph->existEdge(src,tgt)).isValid())
      graph->delEdge(e, true);

    if ((e=graph->existEdge(tgt,src)).isValid())
      graph->delEdge(e, true);
  }

  vector<edge> tmpE;
  SimpleTest::makeSimple(graph, tmpE);
  /*
    for(size_t i = 0; i<resultNode.size(); ++i) {
        if (!graph->isElement(resultNode[i]))
            cout << "bugossss" << endl;
        graph->delAllNode(resultNode[i]);
    }
    */
}
