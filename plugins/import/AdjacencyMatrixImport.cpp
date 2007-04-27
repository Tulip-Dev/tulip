#include <fstream>
#include <sstream>
#include <math.h>
#include <tulip/TulipPlugin.h>
#include <vector>

using namespace std;
using namespace tlp;

const unsigned int MAX_SIZE = 10000;

enum ValType { TLP_DOUBLE = 0, TLP_STRING = 1, TLP_NOVAL = 2, TLP_NOTHING = 3, TLP_AND = 4 };
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
 *  Use & to set the viewMetric and viewLabel properties of a node or edge in the same time.
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
 *  <br>A # E & 5
 *  <br>@ B
 *  <br># @ C
 *  <br>Define a graph with 3 nodes and 3 edges, the edge between A and C is named E and has the value 5
 *
 *  <b>HISTORY</b>
 *
 *  Version 1.0: First version replace old adjacency matrix format
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
  AdjacencyMatrixImport(AlgorithmContext context):ImportModule(context){
    addParameter<string>("file::name",paramHelp[0]);
  }
  ~AdjacencyMatrixImport(){}
  vector<node> nodes;

  bool formatError(const char* s, int curLine) {
    std::stringstream ess;
    ess <<  "Error parsing '" << s << "' at line :" << curLine;
    pluginProgress->setError(ess.str());
    std::cerr << pluginProgress->getError() << std::endl;
    return false;
  }    

  bool import(const string &name) {
    string name2;
    dataSet->get("file::name",name2);
    std::ifstream in(name2.c_str());
    unsigned int curLine = 0;
    DoubleProperty *metric = graph->getProperty<DoubleProperty>("viewMetric");
    StringProperty *stringP = graph->getProperty<StringProperty>("viewLabel");
    while (!in.eof()) {
      char line[MAX_SIZE];
      in.getline(line,MAX_SIZE);
      stringstream lines(line);
      unsigned int curNode = 0;
      int itemFound = 0;
      edge e;
      while (lines.good()) {
	string valString;
	ValType type;
	if ( lines >> valString) {
	  const char *start= valString.c_str();
	  char *res;
	  double valDouble = strtod(start,&res);
	  if (res!=start) {
	    type = TLP_DOUBLE;
	    itemFound = (itemFound == 2) ? 0 : itemFound + 1;
	  } else if (valString == "&") {
	    if (itemFound != 1)
	      return formatError(valString.c_str(), curLine);
	    type = TLP_AND;
	    curNode--;
	    ++itemFound;
	    continue;
	  } else if (valString == "@") {
	    if (itemFound)
	      return formatError(valString.c_str(), curLine);
	    type = TLP_NOVAL;
	  } else if (valString == "#") {
	    if (itemFound)
	      return formatError(valString.c_str(), curLine);
	    type = TLP_NOTHING;
	  } else {
	    itemFound = (itemFound == 2) ? 0 : itemFound + 1;
	    type = TLP_STRING;
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
	    default:
	      return formatError(valString.c_str(), curLine);
	    }
	  }
	  else {
	    switch (type) {
	    case TLP_DOUBLE:
	      if (itemFound)
		e=graph->addEdge(nodes[curLine],nodes[curNode]);
	      metric->setEdgeValue(e,valDouble);
	      break;
	    case TLP_STRING:
	      if (itemFound)
		e=graph->addEdge(nodes[curLine],nodes[curNode]);
	      stringP->setEdgeValue(e,valString);
	      break;
	    case TLP_NOVAL:
	      e=graph->addEdge(nodes[curLine],nodes[curNode]);
	      break;
	    case TLP_NOTHING:
	      break;
	    default:
	      return formatError(valString.c_str(), curLine);
	    }
	  }
	  curNode++;
	}
      }
      if (itemFound == 2)
	formatError("&", curLine);
      curLine++;
    }
    return true;
  }
};
/*@}*/
IMPORTPLUGINOFGROUP(AdjacencyMatrixImport,"Adjacency Matrix","Auber David", "9/02/2004","","1.0","File")

