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

namespace {
#include "dotImportParser.h"
#include "dotImportLexer.h"
}

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
/// DotImport.cpp - Import a graph coded in dot format
/** This plugin enables to import a graph coded with in dot format
 *
 *  File format: 
 *     [ http://www.research.att.com/sw/tools/graphviz/ ]
 *
 *	First (quick) support of the AT&T DOT language
 *        - main graph entities are extracted (node/edges)
 *	  - subgraphs are not already supported
 *  	  - several attributes (node & edge) are supported
 *	  - based on a modified grammar file available with the graphviz' softwares
 *	  - this parser can be largely optimized ...
 *
 *  \author 08/02/2004 - Gerald Gainant (aka maGicG)
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 */
class DotImport:public ImportModule {
public:
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

