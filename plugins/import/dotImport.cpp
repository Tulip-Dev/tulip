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
#include <string.h>
#include <errno.h>
#include <tulip/tuliphash.h>
#include <tulip/TulipPluginHeaders.h>
#include "dotImportCLUT.h"

using namespace std;
using namespace tlp;

#ifdef _WIN32
#define uint unsigned int
#include <utf8.h>
#endif

namespace {
#include "dotImportParser.h"
#include "dotImportLexer.h"
}

namespace {
static const char * paramHelp[] = {
  // filename
  HTML_HELP_OPEN()            \
  HTML_HELP_DEF( "type", "pathname" )       \
  HTML_HELP_BODY()                  \
  "File to import."       \
  HTML_HELP_CLOSE(),
};
}

/** \addtogroup import */

/** This plugin enables to import a graph coded with in dot format
 *
 *  File format:
 *     [ http://www.research.att.com/sw/tools/graphviz/ ]
 *
 *  First (quick) support of the AT&T DOT language
 *        - main graph entities are extracted (node/edges)
 *    - subgraphs are not already supported
 *      - several attributes (node & edge) are supported
 *    - based on a modified grammar file available with the graphviz software
 *    - this parser can be largely optimized ...
 *
 */
class DotImport:public ImportModule {
public:
  PLUGININFORMATION("graphviz","Gerald Gainant", "01/03/2004","Imports a new graph from a file (.dot) in the dot input format.","1.0","File")
  std::list<std::string> fileExtensions() const {
    std::list<std::string> l;
    l.push_back("dot");
    return l;
  }
  DotImport(tlp::PluginContext* context):ImportModule(context) {
    addInParameter<string>("file::filename",paramHelp[0],"");
  }
  ~DotImport() {}

  std::string icon() const {
    return ":/tulip/graphperspective/icons/32/import_graphviz.png";
  }

  bool importGraph() {

    // Open input stream
    string fn;
    dataSet->get( "file::filename", fn );
#ifndef WIN32
    FILE * fd = fopen( fn.c_str(), "r" );
#else
    wstring wfn;
    utf8::utf8to16(fn.begin(), fn.end(), back_inserter(wfn));
    FILE * fd = _wfopen( wfn.c_str(), L"r" );
#endif

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

PLUGIN(DotImport)
