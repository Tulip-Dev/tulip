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
#include <algorithm>
#include <tulip/TulipPluginHeaders.h>

using namespace std;
using namespace tlp;


class CompNodes {

public :
  Graph * graph;
  CompNodes(Graph *g):graph(g) {
  }

  bool operator()(const node u, const node v) {
    unsigned int du = graph->deg(u), dv = graph->deg(v);

    if (du == dv)
      return u.id > v.id;

    return du > dv;
  }
};


/*@{*/
/** \file
 *  \brief  An implementation of the Welsh and Powell algorithm
 *
 * This plugin is an implementation of the Welsh and Powell
 * graph coloring algorithm. It computes a set of integer measures
 * for the nodes of a graph in such a way that the measures assigned to 2
 * adjacent nodes are always different.
 *
 * (see http://en.wikipedia.org/wiki/Graph_coloring for more details)
 *
 *  <b>HISTORY</b>
 *
 *  - 2005 Version 1.0: Initial release
 *  by David Auber, LaBRI, University Bordeaux I, France
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
*/
class WelshPowell:public DoubleAlgorithm {
public:

  PLUGININFORMATION("Welsh & Powell","David Auber","03/01/2005","Nodes coloring measure,<br/>values assigned to adjacent nodes are always different.","1.0", "Graph")

  WelshPowell(const tlp::PluginContext *context):DoubleAlgorithm(context) {}

  bool hasNeightboColoredWith(const node n ,const int color, const MutableContainer<int> &colors) {
    node u;
    forEach(u, graph->getInOutNodes(n))

    if(colors.get(u.id) == color)
      return true;

    return false;
  }

  void colorize() {

    vector<node> toSort(graph->numberOfNodes());
    node n;
    unsigned int i = 0;
    forEach(n,graph->getNodes())
    toSort[i++]=n;
    CompNodes cmp(graph);
    sort(toSort.begin(),toSort.end(),cmp);

    MutableContainer<int> colors;
    colors.setAll(-1);
    int currentColor = 0;
    unsigned int numberOfColoredNodes = 0;

    while(numberOfColoredNodes != graph->numberOfNodes()) {
#ifndef NDEBUG
      cerr << "nbColored :"  << numberOfColoredNodes << endl;
#endif

      for(unsigned int i=0; i < toSort.size(); ++i) {
#ifndef NDEBUG
        cerr << "i:" <<  i << endl;
#endif

        if((colors.get(toSort[i].id) == -1) && (!hasNeightboColoredWith(toSort[i], currentColor, colors))) {
#ifndef NDEBUG
          cerr << "new node found color : " << currentColor << endl;
#endif
          colors.set(toSort[i].id, currentColor);
          result->setNodeValue(toSort[i], currentColor);
          ++numberOfColoredNodes;
        }
      }

      ++currentColor;
    }
  }


  bool run() {
    colorize();

    return true;
  }
};
/*@}*/
PLUGIN(WelshPowell)
