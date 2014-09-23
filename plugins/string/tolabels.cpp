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

#include <tulip/TulipPluginHeaders.h>

using namespace tlp;

namespace {
const char * paramHelp[] = {
  // property
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "PropertyInterface" ) \
  HTML_HELP_BODY() \
  "Property to copy on labels" \
  HTML_HELP_CLOSE(),
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "BooleanProperty" ) \
  HTML_HELP_DEF( "default", "none" ) \
  HTML_HELP_BODY() \
  "Set of elements for which to set the labels." \
  HTML_HELP_CLOSE(),
  // on nodes
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "bool" ) \
  HTML_HELP_BODY() \
  "Copy nodes values" \
  HTML_HELP_CLOSE(),
  // property
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "bool" ) \
  HTML_HELP_BODY() \
  "Copy edges values" \
  HTML_HELP_CLOSE()
};
}

class ToLabels: public tlp::StringAlgorithm {
public:
  PLUGININFORMATION("To labels","Ludwig Fiolka","2012/03/16","Map a property on nodes/edges labels","1.0","")
  ToLabels(const tlp::PluginContext* context): StringAlgorithm(context) {
    addInParameter<PropertyInterface*>("input",paramHelp[0],"viewMetric",true);
    addInParameter<BooleanProperty>("selection",paramHelp[1],"",false);
    addInParameter<bool>("nodes",paramHelp[2],"true");
    addInParameter<bool>("edges",paramHelp[3],"true");
  }

  bool run() {
    PropertyInterface* input = NULL;
    BooleanProperty* selection = NULL;
    bool onNodes = true;
    bool onEdges = true;

    if (dataSet != NULL) {
      dataSet->get("input",input);
      dataSet->get("selection",selection);
      dataSet->get("nodes",onNodes);
      dataSet->get("edges",onEdges);
    }

    if (onNodes) {
      pluginProgress->setComment("Copying nodes values");
      int step=0,max_step = graph->numberOfNodes();
      node n;
      forEach(n, selection ? selection->getNonDefaultValuatedNodes() : graph->getNodes()) {
        pluginProgress->progress(step++,max_step);
        result->setNodeValue(n,input->getNodeStringValue(n));
      }
    }

    if (onEdges) {
      pluginProgress->setComment("Copying edges values");
      int step=0,max_step = graph->numberOfEdges();
      edge e;
      forEach(e, selection ? selection->getNonDefaultValuatedEdges() : graph->getEdges()) {
        pluginProgress->progress(step++,max_step);
        result->setEdgeValue(e,input->getEdgeStringValue(e));
      }
    }

    return true;
  }
};

PLUGIN(ToLabels)
