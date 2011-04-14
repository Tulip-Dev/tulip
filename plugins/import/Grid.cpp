/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include <tulip/TulipPlugin.h>
#include <math.h>

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
        "This parameter defines the grid's node width."
        HTML_HELP_CLOSE(),

        // height
        HTML_HELP_OPEN()
        HTML_HELP_DEF( "type", "unsigned int" )
        HTML_HELP_DEF( "default", "10" )
        HTML_HELP_BODY()
        "This parameter defines the grid's node height."
        HTML_HELP_CLOSE(),
        //Connectivity
        HTML_HELP_OPEN()
        HTML_HELP_DEF( "type", "StringCollection" )
        HTML_HELP_DEF( "values", "4 <br> 6 <br> 8" )
        HTML_HELP_DEF( "default", "4" )
        HTML_HELP_BODY()
        "This parameter defines the connectivity number of each node."
        HTML_HELP_CLOSE(),
        // tore
        HTML_HELP_OPEN()
        HTML_HELP_DEF( "type", "bool" )
        HTML_HELP_DEF( "default", "false" )
        HTML_HELP_BODY()
        "This parameter defines if the opposite node in the grid are connected. In a 4 connectivity the resulting object is a torus."
        HTML_HELP_CLOSE(),
        // Line spacing
        HTML_HELP_OPEN()
        HTML_HELP_DEF( "type", "double" )
        HTML_HELP_DEF( "default", "1.0" )
        HTML_HELP_BODY()
        "This parameter defines the spacing between each node in the grid." };
}
/** \addtogroup import */
/*@{*/
/// Grid - Import a grid.
/** This plugin enables to create a grid.
 *
 *  User can specify the connectivity of each nodes in the grid, spacing between nodes and if opposite nodes are connected.
 */
class Grid: public ImportModule {
 public:
  Grid(AlgorithmContext context) :
    ImportModule(context) {
    addParameter<unsigned int> ("width", paramHelp[0], "10");
    addParameter<unsigned int> ("height", paramHelp[1], "10");
    addParameter<StringCollection> ("connectivity", paramHelp[2], "4;6;8");
    addParameter<bool> ("oppositeNodesConnected", paramHelp[3], "false");
    addParameter<double> ("spacing", paramHelp[4], "1.0");
  }
  ~Grid() {
  }

  void buildRow(vector<node> &row, unsigned int height, int conn, bool isTore,
      double spacing) {
    LayoutProperty *layout = graph->getProperty<LayoutProperty> ("viewLayout");
    unsigned int width = row.size();

    //Used for conn == 6
    double r = 0.5;
    double h = sqrt((r * r) - ((r / 2) * (r / 2)));
    double hHeight = cos(DEGTORAD(60)) * r;
    double shift = 0;
    //If height is even introduce a shift
    if (height % 2 == 0) {
      shift += h;
    } else {
      shift += 0;
    }
    for (unsigned int i = 0; i < width; ++i) {
      row[i] = graph->addNode();
      if (conn == 6) {

        layout ->setNodeValue(row[i], Coord(i * 2 * h + shift + i * spacing,
            height * (1.0 - hHeight + spacing), 0));
      } else
        layout ->setNodeValue(row[i], Coord(i * (1.0 + spacing), height * (1.0
            + spacing), 0));
    }
    //At this time the first edge is alway the good
    for (unsigned int i = 0; i < width - 1; ++i)
      graph->addEdge(row[i], row[i + 1]);

    if (isTore)
      graph->addEdge(row[width - 1], row[0]);
  }

  void connectRow(vector<node> &row1, unsigned int row1H, vector<node> &row2,
      unsigned int, int conn, bool isTore) {
    assert(row1.size()==row2.size());
    unsigned int width = row1.size();
    for (unsigned int i = 0; i < width; ++i) {
      graph->addEdge(row1[i], row2[i]);

      if (conn == 8) {
        if (i > 0) {
          graph ->addEdge(row1[i], row2[i - 1]);
        } else if (isTore) {
          graph->addEdge(row1[i], row2[width - 1]);
        }
        if (i < width - 1) {
          graph ->addEdge(row1[i], row2[i + 1]);
        } else if (isTore) {
          graph->addEdge(row1[i], row2[0]);
        }
      }
      if (conn == 6) {
        //In this case the row1 height must be even in order to ensure right connectivity in the hexagonal grid.
        if (row1H % 2 == 0) {
          if (i < width - 1)
            graph->addEdge(row1[i], row2[i + 1]);
          else if (isTore)
            graph->addEdge(row1[i], row2[0]);
        } else {
          if (i > 0)
            graph->addEdge(row1[i], row2[i - 1]);
          else if (isTore)
            graph->addEdge(row1[i], row2[width - 1]);
        }
      }

    }
  }


  bool import(const string &) {
    unsigned int width = 10;
    unsigned int height = 10;
    bool isTore = false;
    int conn = 4;
    StringCollection connectivity;
    double spacing = 1.0;
    if (dataSet != 0) {
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
      if(isTore && height%2 == 1){
	if (pluginProgress)
	  pluginProgress->setError("Error : cannot connect opposite nodes in an hexagonal grid with odd height");
	else
	  std::cerr << __PRETTY_FUNCTION__ << ":" << __LINE__ << " Error : cannot connect opposite nodes in an hexagonal grid with odd height"  <<std::endl;
        return false;
      }
    } else
      conn = 8;

    // graph is predimensioned according the parameters
    graph->reserveNodes(height * width);

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
    graph->reserveEdges(nbEdges);
 
    vector<vector<node> > grid;
    grid.resize(height);

    grid[0].resize(width);
    buildRow(grid[0], 0, conn, isTore, spacing);
    for (unsigned int i = 1; i < height; ++i) {
      grid[i].resize(width);

      buildRow(grid[i], i, conn, isTore, spacing);
      connectRow(grid[i - 1], i - 1, grid[i], i, conn, isTore);
    }

    if (isTore) {
      connectRow(grid[height - 1], height - 1, grid[0], 0, conn, isTore);
    }

    return true;
  }
};
/*@}*/
IMPORTPLUGINOFGROUP(Grid,"Grid","Jonathan Dubois","02/12/2003","","2.0","Graphs")
