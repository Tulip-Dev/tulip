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
#include <string.h>
#include <errno.h>
#include <tulip/tuliphash.h>
#include <tulip/TulipPlugin.h>
#include "dotImportCLUT.h"

using namespace std;
using namespace tlp;

#ifdef _WIN32
#define uint unsigned int
#endif

#ifdef _WIN32
#define uint unsigned int
#endif

namespace {
#include "dotImportParser.h"
#include "dotImportLexer.h"
}

namespace {
  static const char * paramHelp[] = {
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
/** This plugin enables to import a graph coded with in dot format
 *
 *  File format: 
 *     [ http://www.research.att.com/sw/tools/graphviz/ ]
 *
 *	First (quick) support of the AT&T DOT language
 *        - main graph entities are extracted (node/edges)
 *	  - subgraphs are not already supported
 *  	  - several attributes (node & edge) are supported
 *	  - based on a modified grammar file available with the graphviz software
 *	  - this parser can be largely optimized ...
 *
 */
class DotImport:public ImportModule {
public:
  DotImport(AlgorithmContext context):ImportModule(context){
    addParameter<string>("file::filename",paramHelp[0]);
  }
  ~DotImport(){}

  bool import() {

    // Open input stream
    string fn;
    dataSet->get( "file::filename", fn );
    FILE * fd = fopen( fn.c_str(), "r" );
    if( !fd ) {
      if (pluginProgress)
	pluginProgress->setError(strerror(errno));
      return false;
    }

    // Create & Init YY global data 
    DOT_YY _dotyy(fd, graph, pluginProgress);
    if (pluginProgress) {
      pluginProgress->showPreview(false);
      pluginProgress->progress(1, 100000);
    }
    dotyy = &_dotyy;
    yyrestart( fd );
    yyparse();

    fclose( fd );

    return _dotyy.pStatus != TLP_CANCEL;
  }
};
/*@}*/
IMPORTPLUGINOFGROUP(DotImport,"dot (graphviz)","Gerald Gainant", "01/03/2004","Import plugin for dot files",".01","File")

