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

  bool hasNeighbourColoredWith(const node n, const int color) {
    node u;
    forEach(u, graph->getInOutNodes(n))

    if (result->getNodeValue(u) == color)
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

    result->setAllNodeValue(-1);
    int currentColor = 0;
    unsigned int numberOfColoredNodes = 0;
    unsigned int minIndex = 0;
    unsigned int maxIndex = toSort.size();

    while (numberOfColoredNodes != graph->numberOfNodes()) {
#ifndef NDEBUG
      cout << "nbColored :"  << numberOfColoredNodes << endl;
#endif
      unsigned int nextMaxIndex = minIndex;

      for(unsigned int i= minIndex; i < maxIndex; ++i) {
#ifndef NDEBUG
        cout << "i:" <<  i << endl;
#endif
        node n = toSort[i];

        if (result->getNodeValue(n) == -1) {
          if (!hasNeighbourColoredWith(n, currentColor)) {
#ifndef NDEBUG
            cout << "new node found color : " << currentColor << endl;
#endif
            result->setNodeValue(toSort[i], currentColor);
            ++numberOfColoredNodes;

            if (i == minIndex)
              ++minIndex;
          }
          else
            nextMaxIndex = i + 1;
        }
        else if (i == minIndex)
          ++minIndex;
      }

      maxIndex = nextMaxIndex;
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
