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
#include <tulip/TulipPluginHeaders.h>
#include <tulip/StringCollection.h>

#include <cmath>

using namespace std;
using namespace tlp;
#define DEGTORAD(x) (M_PI/180) * x

namespace {
const char
* paramHelp[] = {
  // width
  HTML_HELP_OPEN()
  HTML_HELP_DEF( "type", "unsigned int" )
  HTML_HELP_DEF( "default", "10" )
  HTML_HELP_BODY()
  "Grid's node width."
  HTML_HELP_CLOSE(),

  // height
  HTML_HELP_OPEN()
  HTML_HELP_DEF( "type", "unsigned int" )
  HTML_HELP_DEF( "default", "10" )
  HTML_HELP_BODY()
  "Grid's node height."
  HTML_HELP_CLOSE(),
  //Connectivity
  HTML_HELP_OPEN()
  HTML_HELP_DEF( "type", "StringCollection" )
  HTML_HELP_DEF( "values", "4 <br> 6 <br> 8" )
  HTML_HELP_DEF( "default", "4" )
  HTML_HELP_BODY()
  "Connectivity number of each node."
  HTML_HELP_CLOSE(),
  // tore
  HTML_HELP_OPEN()
  HTML_HELP_DEF( "type", "bool" )
  HTML_HELP_DEF( "default", "false" )
  HTML_HELP_BODY()
  "If true, opposite nodes on each side of the grid are connected. In a 4 connectivity the resulting object is a torus."
  HTML_HELP_CLOSE(),
  // Line spacing
  HTML_HELP_OPEN()
  HTML_HELP_DEF( "type", "double" )
  HTML_HELP_DEF( "default", "1.0" )
  HTML_HELP_BODY()
  "Spacing between nodes."
};
}
/** \addtogroup import */

/// Grid - Import of a grid.
/** This plugin enables to create a grid.
 *
 *  User can specify the connectivity of each nodes in the grid, spacing between nodes and if opposite nodes are connected.
 */
class Grid: public ImportModule {
public:
  PLUGININFORMATION("Grid","Jonathan Dubois","02/12/2003","Imports a new grid structured graph.","2.0","Graph")
  Grid(tlp::PluginContext* context) :
    ImportModule(context) {
    addInParameter<unsigned int> ("width", paramHelp[0], "10");
    addInParameter<unsigned int> ("height", paramHelp[1], "10");
    addInParameter<StringCollection> ("connectivity", paramHelp[2], "4;6;8");
    addInParameter<bool> ("oppositeNodesConnected", paramHelp[3], "false");
    addInParameter<double> ("spacing", paramHelp[4], "1.0");
  }
  ~Grid() {
  }

  void buildRow(const vector<node>& nodes, vector<pair<node, node> >& ends,
                unsigned int iRow, unsigned width,
                int conn, bool isTore, double spacing) {
    LayoutProperty *layout = graph->getProperty<LayoutProperty> ("viewLayout");

    //Used for conn == 6
    double r = 0.5;
    double h = sqrt((r * r) - ((r / 2) * (r / 2)));
    double hHeight = cos(DEGTORAD(60)) * r;
    double shift = 0;

    //If iRow is even introduce a shift
    if (iRow % 2 == 0) {
      shift += h;
    }
    else {
      shift += 0;
    }

    unsigned int iBegin = iRow * width;
    node previous, current;

    for (unsigned int i = 0; i < width; ++i) {
      current = nodes[iBegin + i];

      if (conn == 6) {
        layout->setNodeValue(current,
                             Coord(i * 2 * h + shift + i * spacing,
                                   iRow * (1.0 - hHeight + spacing), 0));
      }
      else
        layout->setNodeValue(current,
                             Coord(i * (1.0 + spacing), iRow * (1.0
                                   + spacing), 0));

      if (previous.isValid())
        ends.push_back(pair<node, node>(previous, current));

      previous = current;
    }

    if (isTore)
      ends.push_back(pair<node, node>(current, nodes[iBegin]));
  }

  void connectRow(const vector<node> &nodes, vector<pair<node, node> > &ends,
                  unsigned int row1, unsigned int row2, unsigned int width,
                  int conn, bool isTore) {
    unsigned int row1Begin = row1 * width;
    unsigned int row2Begin = row2 * width;

    for (unsigned int i = 0; i < width; ++i) {
      ends.push_back(pair<node, node>(nodes[row1Begin + i],
                                      nodes[row2Begin + i]));

      if (conn == 8) {
        if (i > 0) {
          ends.push_back(pair<node, node>(nodes[row1Begin + i],
                                          nodes[row2Begin + i - 1]));
        }
        else if (isTore) {
          ends.push_back(pair<node, node>(nodes[row1Begin + i],
                                          nodes[row2Begin + width - 1]));
        }

        if (i < width - 1) {
          ends.push_back(pair<node, node>(nodes[row1Begin + i],
                                          nodes[row2Begin + i + 1]));
        }
        else if (isTore) {
          ends.push_back(pair<node, node>(nodes[row1Begin + i],
                                          nodes[row2Begin]));
        }
      }

      if (conn == 6) {
        //In this case row1 must be even in order to ensure right connectivity in the hexagonal grid.
        if (row1 % 2 == 0) {
          if (i < width - 1)
            ends.push_back(pair<node, node>(nodes[row1Begin + i],
                                            nodes[row2Begin + i + 1]));
          else if (isTore)
            ends.push_back(pair<node, node>(nodes[row1Begin + i], nodes[row2Begin]));
        }
        else {
          if (i > 0)
            ends.push_back(pair<node, node>(nodes[row1Begin + i],
                                            nodes[row2Begin + i - 1]));
          else if (isTore)
            ends.push_back(pair<node, node>(nodes[row1Begin + i],
                                            nodes[row2Begin + width - 1]));
        }
      }
    }
  }


  bool importGraph() {
    unsigned int width = 10;
    unsigned int height = 10;
    bool isTore = false;
    int conn = 4;
    StringCollection connectivity;
    double spacing = 1.0;

    if (dataSet != NULL) {
      dataSet->get("width", width);
      dataSet->get("height", height);
      dataSet->get("oppositeNodesConnected", isTore);
      dataSet->get("spacing", spacing);
      dataSet->get("connectivity", connectivity);
    }

    if (width == 0) {
      if (pluginProgress)
        pluginProgress->setError(string("Error: width cannot be null"));

      return false;
    }

    if (height == 0) {
      if (pluginProgress)
        pluginProgress->setError(string("Error: height cannot be null"));

      return false;
    }

    if (spacing < 0.0) {
      if (pluginProgress)
        pluginProgress->setError(string("Error: spacing must be strictly positive"));

      return false;
    }

    if (connectivity.getCurrentString().compare("4") == 0)
      conn = 4;
    else if (connectivity.getCurrentString().compare("6") == 0) {
      conn = 6;

      if(isTore && height%2 == 1) {
        if (pluginProgress)
          pluginProgress->setError("Error : cannot connect opposite nodes in an hexagonal grid with odd height");
        else
          tlp::warning() << __PRETTY_FUNCTION__ << ":" << __LINE__ << " Error : cannot connect opposite nodes in an hexagonal grid with odd height" << std::endl;

        return false;
      }
    }
    else
      conn = 8;

    // graph is predimensioned according the parameters
    vector<node> nodes;
    nodes.reserve(height * width);
    graph->addNodes(height * width, nodes);

    // compute nb edges
    unsigned int nbEdges = height * (width - 1);

    if (isTore)
      nbEdges += height;

    // add the between rows connections to the in rows connections
    nbEdges += width * (height - 1);

    // more between rows connections
    if (conn >= 6) {
      nbEdges += (height - 1) * (width - 1);

      if (isTore)
        nbEdges += height - 1;
    }

    // more between row connections
    if (conn == 8) {
      nbEdges += (height - 1) * (width - 1);

      if (isTore)
        nbEdges += height - 1;
    }

    vector<pair<node, node> > ends;
    ends.reserve(nbEdges);
    vector<edge> edges;
    edges.reserve(nbEdges);

    buildRow(nodes, ends, 0, width, conn, isTore, spacing);

    for (unsigned int i = 1; i < height; ++i) {
      buildRow(nodes, ends, i, width, conn, isTore, spacing);
      connectRow(nodes, ends, i - 1, i, width, conn, isTore);
    }

    if (isTore) {
      connectRow(nodes, ends, height - 1, 0, width, conn, isTore);
    }

    graph->addEdges(ends, edges);

    return true;
  }
};

PLUGIN(Grid)
