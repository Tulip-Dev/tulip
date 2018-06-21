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
#include <tulip/PropertyAlgorithm.h>
#include <tulip/StringCollection.h>
#include <tulip/NumericProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StaticProperty.h>
#include <tulip/ParallelTools.h>

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
    // property
    "Input metric whose values will be mapped to sizes.",

    // input
    "If not all dimensions (width, height, depth) are checked below, the dimensions not computed "
    "are copied from this property.",

    // width
    "Adjusts width (along x axis) to represent the chosen property. If not chosen, the dimension "
    "is copied "
    "from input.",

    // height
    "Adjusts height (along y axis) to represent the chosen property. If not chosen, the dimension "
    "is copied "
    "from input.",

    //  depth
    "Adjusts depth (along z axis) to represent the chosen property. If not chosen, the dimension "
    "is copied "
    "from input.",

    // min
    "Gives the minimum value of the range of computed sizes.",

    // max
    "Gives the maximum value of the range of computed sizes.",

    // Mapping type
    "Type of mapping."
    "<ul><li>linear mapping (min value of property is mapped to min size, max to max size, "
    "and a linear interpolation is used in between.)</li>"
    "<li>uniform quantification (the values of property are sorted, and the same size "
    "increment is used between consecutive values).</li></ul>",

    // node/edge
    "Whether sizes are computed for nodes or for edges.",

    // proportional mapping
    "The mapping can either be area/volume proportional, or square/cubic;"
    "i.e. the areas/volumes will be proportional, or the dimensions (width, height and depth) will "
    "be."};

// error msg for invalid range value
static const string rangeSizeErrorMsg = "max size must be greater than min size";
static const string rangeMetricErrorMsg = "All values are the same";
static const string AREA_PROPORTIONAL = "Area Proportional";
/** \addtogroup size */

/// Size Mapping - Compute size of elements according to a metric.
/** This plugin enables to set the size of the graph's elements
 *  according to a metric.
 *
 */

#define TARGET_TYPE "target"
#define TARGET_TYPES "nodes;edges"
#define NODES_TARGET 0
#define EDGES_TARGET 1
#define MAPPING_TYPE "type"
#define MAPPING_TYPES "linear;uniform"
#define LINEAR_MAPPING 0
#define UNIFORM_MAPPING 1

class MetricSizeMapping : public SizeAlgorithm {
public:
  PLUGININFORMATION(
      "Size Mapping", "Auber", "08/08/2003",
      "Maps the size of the graph elements onto the values of a given numeric property.", "2.1",
      "Size")
  MetricSizeMapping(const PluginContext *context)
      : SizeAlgorithm(context), entryMetric(nullptr), entrySize(nullptr), xaxis(true), yaxis(true),
        zaxis(true), linearType(true), min(1), max(10), range(0), shift(0) {
    addInParameter<NumericProperty *>("property", paramHelp[0], "viewMetric");
    addInParameter<SizeProperty>("input", paramHelp[1], "viewSize");
    addInParameter<bool>("width", paramHelp[2], "true");
    addInParameter<bool>("height", paramHelp[3], "true");
    addInParameter<bool>("depth", paramHelp[4], "false");
    addInParameter<double>("min size", paramHelp[5], "1");
    addInParameter<double>("max size", paramHelp[6], "10");
    addInParameter<StringCollection>(MAPPING_TYPE, paramHelp[7], MAPPING_TYPES, true,
                                     "linear <br/> uniform");
    addInParameter<StringCollection>(TARGET_TYPE, paramHelp[8], TARGET_TYPES, true,
                                     "nodes <br/> edges");
    addInParameter<StringCollection>("area proportional", paramHelp[7],
                                     "Area Proportional;Quadratic/Cubic", true,
                                     "Area Proportional <br/> Quadratic/Cubic");

    // result needs to be an inout parameter
    // in order to preserve the original values of non targetted elements
    // i.e if "target" = "nodes", the values of edges must be preserved
    // and if "target" = "edges", the values of nodes must be preserved
    parameters.setDirection("result", INOUT_PARAM);
  }

  bool check(std::string &errorMsg) override {
    xaxis = yaxis = zaxis = true;
    min = 1;
    max = 10;
    proportional = "Area Proportional";
    entryMetric = graph->getProperty<DoubleProperty>("viewMetric");
    entrySize = graph->getProperty<SizeProperty>("viewSize");
    linearType = true;
    StringCollection mapping;
    StringCollection proportionalType;
    targetType.setCurrent(NODES_TARGET);

    if (dataSet != nullptr) {
      dataSet->get("property", entryMetric);
      dataSet->get("input", entrySize);
      dataSet->get("width", xaxis);
      dataSet->get("height", yaxis);
      dataSet->get("depth", zaxis);
      dataSet->get("min size", min);
      dataSet->get("max size", max);
      // for compatibility with old parameter type
      if (dataSet->getTypeName(MAPPING_TYPE) == dataSet->getTypeName<bool>())
        dataSet->get(MAPPING_TYPE, linearType);
      else {
        dataSet->get(MAPPING_TYPE, mapping);
        linearType = mapping.getCurrent() == LINEAR_MAPPING;
      }
      dataSet->get(TARGET_TYPE, targetType);
      dataSet->get("area proportional", proportionalType);
      proportional = proportionalType.getCurrentString();
      // old parameter name and behavior
      if (dataSet->exists("node/edge")) {
        bool nodeoredge = true;
        dataSet->get("node/edge", nodeoredge);
        targetType.setCurrent(nodeoredge ? NODES_TARGET : EDGES_TARGET);
      }
    }

    if (min >= max) {
      errorMsg = rangeSizeErrorMsg;
      return false;
    }

    if (targetType.getCurrent() == NODES_TARGET)
      range = entryMetric->getNodeDoubleMax(graph) - entryMetric->getNodeDoubleMin(graph);
    else
      range = entryMetric->getEdgeDoubleMax(graph) - entryMetric->getEdgeDoubleMin(graph);

    if (range == 0) {
      errorMsg = rangeMetricErrorMsg;
      return false;
    }

    if (!xaxis && !yaxis && !zaxis) {
      errorMsg = "You need at least one axis to map on.";
      return false;
    }

    if (proportional == AREA_PROPORTIONAL) {
      max = max * max;
    }

    return true;
  }

  bool run() override {
    NumericProperty *tmp = nullptr;

    if (!linearType) {
      tmp = entryMetric->copyProperty(graph);
      tmp->uniformQuantification(300);
      entryMetric = tmp;
    }

    pluginProgress->showPreview(false);

    if (targetType.getCurrent() == NODES_TARGET) {
      shift = entryMetric->getNodeDoubleMin(graph);

      // compute size of nodes
      NodeStaticProperty<Size> nodeSize(graph);
      nodeSize.copyFromProperty(entrySize);

      TLP_PARALLEL_MAP_NODES(graph, [&](const node &n) {
        double sizos = 0;

        if (proportional == AREA_PROPORTIONAL) {
          const double power = 1.0 / (float(xaxis) + float(yaxis) + float(zaxis));
          sizos =
              min + pow((entryMetric->getNodeDoubleValue(n) - shift) * (max - min) / range, power);
        } else {
          sizos = min + (entryMetric->getNodeDoubleValue(n) - shift) * (max - min) / range;
        }

        if (xaxis)
          nodeSize[n][0] = float(sizos);

        if (yaxis)
          nodeSize[n][1] = float(sizos);

        if (zaxis)
          nodeSize[n][2] = float(sizos);
      });
      nodeSize.copyToProperty(result);
    } else {
      shift = entryMetric->getEdgeDoubleMin(graph);
      // compute size of edges
      EdgeStaticProperty<Size> edgeSize(graph);

      TLP_PARALLEL_MAP_EDGES(graph, [&](const edge &e) {
        double sizos = min + (entryMetric->getEdgeDoubleValue(e) - shift) * (max - min) / range;
        edgeSize[e][0] = float(sizos);
        edgeSize[e][1] = float(sizos);
      });
      edgeSize.copyToProperty(result);
    }

    if (!linearType)
      delete tmp;

    return true;
  }

private:
  NumericProperty *entryMetric;
  SizeProperty *entrySize;
  bool xaxis, yaxis, zaxis, linearType;
  double min, max;
  double range;
  double shift;
  std::string proportional;
  StringCollection targetType;
};

PLUGIN(MetricSizeMapping)
