/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
    "Specifies the background color.",

    // No background
    "Specifies if a background is needed.",

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
    "Specifies if node and edge labels inside metanodes have to be exported."};

class SvgExport : public tlp::ExportModule {

public:
  PLUGININFORMATION("SVG Export", "Sami Gasri, Charles-Antoine Lami, Bruno Pinaud", "16/07/2013",
                    "<p>Supported extensions: svg, svgz (compressed svg).</p><p>Exports a graph "
                    "visualization in a SVG formatted file.</p>",
                    "1.10", "File")

  std::string icon() const override {
    return ":/tulip/graphperspective/icons/32/export_svg.png";
  }

  SvgExport(tlp::PluginContext *context) : tlp::ExportModule(context, {"svg", "svgz"}) {
    addInParameter<bool>("edge color interpolation", paramHelp[0], "false");
    addInParameter<bool>("edge size interpolation", paramHelp[1], "true");
    addInParameter<bool>("edge extremities", paramHelp[2], "false");
    addInParameter<Color>("background color", paramHelp[3], "(255,255,255,255)");
    addInParameter<bool>("no background", paramHelp[4], "false");
    addInParameter<bool>("makes SVG output human readable", paramHelp[5], "true");
    addInParameter<bool>("export node labels", paramHelp[6], "true");
    addInParameter<bool>("export edge labels", paramHelp[7], "false");
    addInParameter<bool>("export metanode labels", paramHelp[8], "false");
  }

  bool exportGraph(ostream &os) override {
    pluginProgress->showPreview(false);
    bool autoformatting(true);

    if (dataSet != nullptr) {
      dataSet->get("makes SVG output human readable", autoformatting);
    }

    ExportSvg svg(pluginProgress, os, autoformatting); // We call our first concrete builder
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
