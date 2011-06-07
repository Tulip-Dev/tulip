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
#include <time.h>
#include <math.h>
#include <climits>
#include <tulip/TulipPlugin.h>


using namespace std;
using namespace tlp;
using namespace stdext;

namespace {
  const char * paramHelp[] = {
    // nodes
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "unsigned int" ) \
    HTML_HELP_DEF( "default", "30" ) \
    HTML_HELP_BODY() \
    "This parameter defines the number of nodes used to build the planar graph." \
    HTML_HELP_CLOSE(),
  };
}
namespace {
struct Triangle {
  Triangle(node a, node b, node c):
    a(a),b(b),c(c) {
  }
  node a,b,c;
};
}
//=============================================================
/** \addtogroup import */
/*@{*/
/// Planar Graph - Import of planer graph
/** This plugin enables to create a planar graph
 *
 *  User can specify the number of nodes.
 */
class PlanarGraph:public ImportModule {
public:
  PlanarGraph(AlgorithmContext context):ImportModule(context) {
    addParameter<unsigned int>("nodes", paramHelp[0], "30");
  }
  ~PlanarGraph(){}
  
  bool import() {
    unsigned int nbNodes  = 30;
    if (dataSet!=0) {
      dataSet->get("nodes", nbNodes);
    }

    if (nbNodes < 3) nbNodes = 3;
    srand(clock()); 
    LayoutProperty *newLayout = graph->getLocalProperty<LayoutProperty>("viewLayout");
    SizeProperty  *newSize   = graph->getLocalProperty<SizeProperty>("viewSize");
    newSize->setAllNodeValue(Size(1.0,1.0,1.0));    

    vector<Triangle> faces;
    Triangle f(graph->addNode(),
	       graph->addNode(),
	       graph->addNode());
    faces.push_back(f);
    graph->addEdge(f.a, f.b);
    graph->addEdge(f.b, f.c);
    graph->addEdge(f.c, f.a);
    float val = nbNodes;	
    newLayout->setNodeValue(f.a, Coord(-val, -val, 0));
    newLayout->setNodeValue(f.b, Coord(0, val, 0));
    newLayout->setNodeValue(f.c, Coord(val, -val, 0));
    unsigned int nb = 3;
    while(nb<nbNodes) {
      //choose a Triangle randomly
      unsigned int i = rand()%faces.size(); 
      Triangle f = faces[i];
      node n = graph->addNode();
      Coord tmp = newLayout->getNodeValue(f.a) +
	newLayout->getNodeValue(f.b) +
	newLayout->getNodeValue(f.c);
      tmp /= 3.0;
      newLayout->setNodeValue(n, tmp);
	
      //Split the triangle in three part
      graph->addEdge(n, f.a);
      graph->addEdge(n, f.b);
      graph->addEdge(n, f.c);
      //add the three new Triangle, remove the old one(replace)
      Triangle f1(f.a, f.b, n);
      Triangle f2(f.b, f.c, n);
      Triangle f3(f.c, f.a, n);
      faces[i] = f1;
      faces.push_back(f2);
      faces.push_back(f3);
      ++nb;
    }
    return  pluginProgress->state()!=TLP_CANCEL;
  }
};
/*@}*/
IMPORTPLUGINOFGROUP(PlanarGraph,"Planar Graph","Auber","25/06/2005","","1.0","Graphs")
