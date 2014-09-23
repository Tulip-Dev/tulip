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
#include <tulip/ForEach.h>
#include <tulip/tulipconf.h>
#include <tulip/TulipPluginHeaders.h>
#include <math.h>

using namespace std;
using namespace tlp;


static const char * paramHelp[] = {
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "double" ) \
  HTML_HELP_BODY() \
  "Choose a damping factor in ]0,1[" \
  HTML_HELP_CLOSE(),
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "bool" ) \
  HTML_HELP_DEF( "default", "true" )  \
  HTML_HELP_BODY() \
  "indicate if the graph should be considered as directed or not" \
  HTML_HELP_CLOSE(),
};

/*@{*/
/** \file
 *  \brief  An implementation of the PageRank metric
 *
 * This plugin is an implementation of the PageRank metric.
 * First designed by Larry Page and Sergey Brin, it is a link analysis
 * algorithm that assigns a measure to each node of an 'hyperlinked' graph.
 *
 * (see http://en.wikipedia.org/wiki/PageRank for more details)
 *
 *  <b>HISTORY</b>
 *
 *  - 2007 Version 1.0: Initial release
 *  by Mohamed Bouklit, LaBRI, University Bordeaux I, France
 *  - 2010 Version 1.1: rewrite to work properly with subgraph
 *  by David Auber, LaBRI, University Bordeaux I, France
 *  - 2011 Version 2.0: fix computation for undirected graph
 *  by François Queyroi, LaBRI, University Bordeaux I, France
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
*/
/*@}*/
struct PageRank : public DoubleAlgorithm {

  PLUGININFORMATION("Page Rank","Mohamed Bouklit & David Auber","16/12/10", "Nodes measure<br/>used for links analysis.","2.0","Graph")

  PageRank(const PluginContext *context) : DoubleAlgorithm(context) {
    addInParameter<double>("d", paramHelp[0], "0.85");
    addInParameter<bool>("directed", paramHelp[1], "true");
  }

  bool run() {
    double d = 0.85;
    bool directed = true;

    if ( dataSet!=0 ) {
      dataSet->get("d",d);
      dataSet->get("directed",directed);
    }

    if(d <= 0 || d >= 1) return false;


    double nbNodes = graph->numberOfNodes();

    // Initialize the PageRank
    MutableContainer<double> R;
    MutableContainer<double> R2;
    R.setAll( 1. / nbNodes);


    for(unsigned int k=0; k < 15*log(nbNodes); ++k) {

      //cout<<"[PageRank iteration "<<k<<" ] "<<endl;
      R2.setAll(0.);

      edge e;
      forEach(e, graph->getEdges()) {
        const std::pair<node, node> eEnds = graph->ends(e);
        node src = eEnds.first;

        if(directed) {
          node tgt = eEnds.second;
          double prev = R2.get(tgt);
          R2.set(tgt, prev + R.get(src) / double(graph->outdeg(src)));
        }
        else {
          node tgt = eEnds.second;
          double prev = R2.get(tgt);
          R2.set(tgt, prev + R.get(src) / double(graph->deg(src)));

          prev = R2.get(src);
          R2.set(src, prev + R.get(tgt) / double(graph->deg(tgt)));
        }
      }

      node n;
      forEach(n, graph->getNodes())
      R2.set(n, d * R2.get(n));

      double mu = 0.0;
      forEach(n, graph->getNodes())
      mu += R.get(n) - R2.get(n);

      forEach(n, graph->getNodes())
      R2.set(n, R2.get(n) + mu*1.0/nbNodes);

      /*double delta = 0.0;
      forEach(n, graph->getNodes())

      if (R.get(n) > R2.get(n))
        delta += R.get(n) - R2.get(n);
      else
      delta += R2.get(n) - R.get(n);*/

      forEach(n, graph->getNodes())
      R.set(n, R2.get(n));

    }

    node n;
    forEach(n, graph->getNodes()) {
      result->setNodeValue(n, R.get(n));
    }

    return true;
  }
};

PLUGIN(PageRank)
