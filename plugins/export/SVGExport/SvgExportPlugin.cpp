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
#include "ReadGraph.h"
#include "ExportSvg.h"

#include <tulip/ExportModule.h>
#include <tulip/LayoutProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
    // color interpolation
    "Indicates if edge color interpolation has to be used.",

    // size interpolation
    "Indicates if edge size interpolation has to be used.",

    // edge extremities
    "Indicates if edge extremities have to be exported.",

    // Background color
    "Specifies the background color of the SVG file.",

    // human readable
    "Adds line-breaks and indentation to empty sections between elements"
    " (ignorable whitespace). The main purpose of this parameter is to split"
    " the data into several lines, and to increase readability for a human reader."
    " Be careful, this adds a large amount of data to the output file.",

    // node labels
    "Specifies if node labels have to be exported.",

    // edge labels
    "Specifies if edge labels have to be exported.",

    // metanode labels
    "Specifies if node and edge labels inside metanodes have to be exported.",

    // woff2
    "Uses Web Open Font Format version 2 (woff2) to reduce generated file length. This format is "
    "supported in almost all recent Internet browsers."};

class SvgExport : public tlp::ExportModule {

public:
  PLUGININFORMATION("SVG Export", "Sami Gasri, Charles-Antoine Lami, Bruno Pinaud", "16/07/2013",
                    "<p>Supported extensions: svg, svgz (compressed svg).</p><p>Exports a graph "
                    "visualization in a SVG formatted file.</p>",
                    "1.9", "File")

  std::string icon() const {
    return ":/tulip/graphperspective/icons/32/export_svg.png";
  }

  string fileExtension() const {
    return "svg";
  }

  list<string> gzipFileExtensions() const {
    list<string> ext;
    ext.push_back("svgz");
    return ext;
  }

  SvgExport(tlp::PluginContext *context) : tlp::ExportModule(context) {
    addInParameter<bool>("Edge color interpolation", paramHelp[0], "false");
    addInParameter<bool>("Edge size interpolation", paramHelp[1], "true");
    addInParameter<bool>("Edge extremities", paramHelp[2], "false");
    addInParameter<Color>("Background color", paramHelp[3], "(255,255,255,255)");
    addInParameter<bool>("Makes SVG output human readable", paramHelp[4], "true");
    addInParameter<bool>("Export node labels", paramHelp[5], "true");
    addInParameter<bool>("Export edge labels", paramHelp[6], "false");
    addInParameter<bool>("Export metanode labels", paramHelp[7], "false");
    addInParameter<bool>("Use Web Open Font Format v2", paramHelp[8], "false");
  }

  bool exportGraph(ostream &os) {
    pluginProgress->showPreview(false);
    bool autoformatting(true), woff2(false);

    if (dataSet != nullptr) {
      dataSet->get("Makes SVG output human readable", autoformatting);
      dataSet->get("Use Web Open Font Format v2", woff2);
    }

    ExportSvg svg(pluginProgress, os, autoformatting, woff2); // We call our first concrete builder
    bool ret = ReadGraph::readGraph(graph, dataSet, pluginProgress, svg);

    if (!ret && autoformatting) {
      pluginProgress->setError(pluginProgress->getError() +
                               "<br/><br/>Human readable output is on. This adds a large amount of "
                               "data to the output file. Try to disable it and try again.");
    }

    return ret;
  }
};
PLUGIN(SvgExport)
