#include <fstream>
#include <sstream>
#include <math.h>
#include <tulip/TulipPlugin.h>
#include <vector>

using namespace std;
using namespace tlp;

const unsigned int MAX_SIZE = 10000;

enum ValType { TLP_DOUBLE = 0, TLP_STRING = 1, TLP_NOVAL = 2, TLP_NOTHING = 3 };
namespace {
  const char * paramHelp[] = {
    // filename
    HTML_HELP_OPEN()				    \
    HTML_HELP_DEF( "type", "pathname" )		    \
    HTML_HELP_BODY()						      \
    "This parameter defines the file pathname to import."	      \
    HTML_HELP_CLOSE(),
  };
}

/** \addtogroup import */
/*@{*/
/// AdjacencyMatrixImport.cpp - Import a graph coded with matrix.
/** This plugin enables to import a graph coded with a matrix
 *
 *  File format: 
 *   
 *  The input format of this plugin is an ascii file where each
 *  line represents a row of the matrix.
 *  In each row, cells must be separated by a space.
 *  
 *  Let M(i,j) be a cell of the matrix :
 *       - if i==j we define the value of a node.
 *       - if i!=j  we define a directed edge between node[i] and node[j]
 *
 *  If M(i,j) is real value (0, .0, -1, -1.0), it is stored in the
 *  viewMetric property of the graph. <br>
 *  If M(i,j) is a string, it is stored in the
 *  viewLabel property of the graph. <br>
 *  If M(i,j) == @ an edge will be created without value <br>
 *  If M(i,j) == # no edge will be created between node[i] and node[j]
 *
 *  EXEMPLE 1 : 
 *  <br>A 
 *  <br># B
 *  <br># # C
 *  <br>Define  a graph with 3 nodes (with labels A B C) and without edge.
 *
 *  EXEMPLE 2 :
 *  <br>A
 *  <br>@ B
 *  <br>@ @ C
 *  <br>Define a simple complete graph with 3 nodes (with labels A B C) and no label (or value) on its edges
 *
 *  EXEMPLE 3 :
 *  <br>A # 5
 *  <br>@ B
 *  <br># @ C
 *  <br>Define a graph with 3 nodes and 3 edges, the edge between A and C has the value 5
 *
 *  <b>HISTORY</b>
 *
 *  Version 0.1: 14 February: First version replace old adjacency matrix format
 *  included in Tulip (no more supported)
 *
 *  <b>TODO :</b>
 *
 *  Add tests in order to prevent bad file format.
 *
 *  \author David Auber University of Bordeaux I (LaBRI) France \n
 *  Email:auber@tulip-software.org
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 *
 */
class AdjacencyMatrixImport:public ImportModule {
public:
  AdjacencyMatrixImport(ClusterContext context):ImportModule(context){
    addParameter<string>("filename",paramHelp[0]);
  }
  ~AdjacencyMatrixImport(){}
  vector<node> nodes;

  bool import(const string &name) {
    string name2;
    dataSet->get("filename",name2);
    std::ifstream in(name2.c_str());
    unsigned int curLine=0;
    DoubleProperty *metric = graph->getProperty<DoubleProperty>("viewMetric");
    StringProperty *stringP = graph->getProperty<StringProperty>("viewLabel");
    while (!in.eof()) {
      char line[MAX_SIZE];
      in.getline(line,MAX_SIZE);
      stringstream lines(line);
      unsigned int curNode = 0;
      while (lines.good()) {
	string valString;
	double valDouble;
	ValType type;
	if ( lines >> valString) {
	  const char *start= valString.c_str();
	  char *res;
	  double valDouble = strtod(start,&res);
	  if (res!=start) 
	    type = TLP_DOUBLE;
	  else {
	    type = TLP_STRING;
	    if (valString == "@")  type = TLP_NOVAL;
	    if (valString == "#")  type = TLP_NOTHING;
	  }

	  if ( curNode >= nodes.size() || curLine >= nodes.size()) {
	    nodes.push_back(graph->addNode());
	  }

	  if (curNode == curLine) {  
	    switch (type) {
	    case TLP_DOUBLE:
	      metric->setNodeValue(nodes[curNode],valDouble);
	      break;
	    case TLP_STRING:
	      stringP->setNodeValue(nodes[curNode],valString);
	      break;
	    }
	  }
	  else {
	    edge e;
	    switch (type) {
	    case TLP_DOUBLE:
	      e=graph->addEdge(nodes[curLine],nodes[curNode]);
	      metric->setEdgeValue(e,valDouble);
	      break;
	    case TLP_STRING:
	      e=graph->addEdge(nodes[curLine],nodes[curNode]);
	      stringP->setEdgeValue(e,valString);
	      break;
	    case TLP_NOVAL:
	      e=graph->addEdge(nodes[curLine],nodes[curNode]);
	      break;
	    }
	  }
	  curNode++;
	}
      }
      curLine++;
    }
    return true;
  }
};
/*@}*/
IMPORTPLUGINOFGROUP(AdjacencyMatrixImport,"Adjacency Matrix","Auber David", "9/02/2004","0","0","1","File")

