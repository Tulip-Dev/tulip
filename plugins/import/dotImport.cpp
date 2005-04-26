//
// DOT language parser for the Tulip software
// [ http://www.research.att.com/sw/tools/graphviz/ ]
//
// 08/02/2004 - Gerald Gainant (aka maGicG)
//
//		First (quick) support of the AT&T DOT language
//		o main graph entities are extracted (node/edges)
//		o subgraphes are not already supported
//		o several attributes (node & edge) are supported
//		o based on a modified grammar file available with the graphviz' softwares
//		o this parser can be largely optimised ...
//


#include <fstream>
#include <sstream>
#include <math.h>
#include <tulip/TulipPlugin.h>
#include <tulip/hash_string.h>
#include <vector>
#include <string>
#include <ext/hash_map>
#include <dotImportCLUT.h>

using namespace std;


namespace
{

	#include "dotImportParser.h"
	#include "dotImportLexer.h"

}




/** \addtogroup import */
/*@{*/
/// DotImport.cpp - Import a graph coded with matrix.
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
 *  TODO :
 *  Add tests in order to prevent bad file format.
 *
 *  AUTHOR:
 *
 *  David Auber University of Bordeaux (LaBRI) France: Email:auber@tulip-software.org
 *  Version 0.1: 14 February: First version replace old adjacency matrix format included in Tulip 
 *  (no more supported)
 *
 *  LICENCE:
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 *
 */
 
 

namespace
{

	const char * paramHelp[] =
	{
		// filename
		HTML_HELP_OPEN() \
		HTML_HELP_DEF( "type", "pathname" ) \
		HTML_HELP_BODY() \
		"This parameter defines the file pathname to import." \
		HTML_HELP_CLOSE(),
	};

}


struct DotImport:public ImportModule {
  DotImport(ClusterContext context):ImportModule(context){
    addParameter<string>("filename",paramHelp[0]);
  }
  ~DotImport(){}

  bool import(const string &name) {

	// Open input stream
	string fn;
    dataSet->get( "filename", fn );
  	FILE * fd = fopen( fn.c_str(), "r" );
	if( !fd )
		return false;

	// Create & Init YY global data 
    DOT_YY _dotyy;
	_dotyy.sg = superGraph;

	dotyy = &_dotyy;
	yyrestart( fd );
	yyparse();

	fclose( fd );

    return true;
  }
};


/*@}*/
IMPORTPLUGIN(DotImport,"dot (graphviz)","Gerald Gainant", "01/03/2004","0","0","1")

