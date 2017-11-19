/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <cstring>
#include <cerrno>
#include <tulip/tuliphash.h>
#include <tulip/TulipPluginHeaders.h>
#include "dotImportCLUT.h"

using namespace std;
using namespace tlp;

#ifdef _WIN32
#define uint unsigned int
#include <utf8.h>
#endif

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif
namespace {
#include "dotImportParser.h"
#include "dotImportLexer.h"
}
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

static const char *paramHelp[] = {
    // filename
    "The dot file to import."};

/** \addtogroup import */

/** This plugin enables to import a graph coded with in dot format
 *
 *  File format:
 *     [ http://www.research.att.com/sw/tools/graphviz/ ]
 *
 *  First (quick) support of the AT&T DOT language
 *    - main graph entities are extracted (node/edges)
 *    - subgraphs are not already supported
 *    - several attributes (node & edge) are supported
 *    - based on a modified grammar file available with the graphviz software
 *    - this parser can be largely optimized ...
 *
 */
class DotImport : public ImportModule {
public:
  PLUGININFORMATION("graphviz", "Gerald Gainant", "01/03/2004",
                    "<p>Supported extensions: dot</p><p>Imports a new graph from a file in the dot "
                    "input format.</p>",
                    "1.0", "File")
  std::list<std::string> fileExtensions() const override {
    std::list<std::string> l;
    l.push_back("dot");
    return l;
  }
  DotImport(tlp::PluginContext *context) : ImportModule(context) {
    addInParameter<string>("file::filename", paramHelp[0], "");
  }
  ~DotImport() override {}

  std::string icon() const override {
    return ":/tulip/graphperspective/icons/32/import_graphviz.png";
  }

  bool importGraph() override {

    // Open input stream
    string fn;
    dataSet->get("file::filename", fn);
#ifndef WIN32
    FILE *fd = fopen(fn.c_str(), "r");
#else
    wstring wfn;
    utf8::utf8to16(fn.begin(), fn.end(), back_inserter(wfn));
    FILE *fd = _wfopen(wfn.c_str(), L"r");
#endif

    if (!fd) {
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
    yyrestart(fd);
    yyparse();

    fclose(fd);

    return _dotyy.pStatus != TLP_CANCEL;
  }
};

PLUGIN(DotImport)
