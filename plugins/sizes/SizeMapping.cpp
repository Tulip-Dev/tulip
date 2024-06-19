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
#include <tulip/PropertyAlgorithm.h>
#include <tulip/StringCollection.h>
#include <tulip/DoubleProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StaticProperty.h>
#include <tulip/GraphParallelTools.h>

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
    // property
    "Metric whose values will be mapped to sizes.",

    // input
    "If not all dimensions (width, height, depth) are checked below, the dimensions not computed are copied from this property.",

    // width
    "Adjusts size on x-axis to map the metric. If not, the size is copied from the \"input\" property.<br/>For edges, this is the size on the source node side.",

    // height
    "Adjusts size on y-axis to map the metric. If not, the size is copied from the \"input\" property.<br/>For edges, this is the size on the target node side.",

    // depth
    "Adjusts size on z-axis to map the metric. If not, the size is copied from \"input\" property.<br/>Not used for edges.",

    // min
    "Minimum value of the range of computed sizes.<br/>A value of 0 would be better for edges.",

    // max
    "Maximum value of the range of computed sizes.<br/>A value of 1 would be better for edges.",

    // Mapping type
    "Type of mapping."
    "<ul><li>linear mapping (min value of property is mapped to min size, max to max size, "
    "and a linear interpolation is used in between.)</li>"
    "<li>uniform quantification (the values of property are sorted, and the same size "
    "increment is used between consecutive values).</li></ul>",

    // node/edge
    "Whether sizes are computed for nodes or for edges.",

    // mapping proportionality
    "For nodes only, the mapping can be either area/volume proportional, which means that the areas/volumes will be proportional, or dimensions proportional which means that the width, height and depth will be."};

// error msg for invalid range value
static const string rangeSizeErrorMsg = "max size must be greater than min size";
static const string rangeMetricErrorMsg = "All values are the same";
#define AREA_PROPORTIONAL 0
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
      "Maps the size of the graph elements onto the values of a given numeric property.", "2.2",
      "Size")
  MetricSizeMapping(const PluginContext *context)
      // set second parameter of the constructor below to true because
      // result needs to be an inout parameter
      // in order to preserve the original values of non targeted elements
      // i.e if "target" = "nodes", the values of edges must be preserved
      // and if "target" = "edges", the values of nodes must be preserved
      : SizeAlgorithm(context, true), entryMetric(nullptr), entrySize(nullptr), xaxis(true),
        yaxis(true), zaxis(true), linearType(true), min(1), max(10), range(0), shift(0) {
    addInParameter<NumericProperty *>("metric", paramHelp[0], "viewMetric");
    addInParameter<SizeProperty>("input", paramHelp[1], "viewSize");
    addInParameter<bool>("width", paramHelp[2], "true");
    addInParameter<bool>("height", paramHelp[3], "true");
    addInParameter<bool>("depth", paramHelp[4], "false");
    addInParameter<double>("min size", paramHelp[5], "1");
    addInParameter<double>("max size", paramHelp[6], "10");
    addInParameter<StringCollection>(MAPPING_TYPE, paramHelp[7], MAPPING_TYPES, true,
                                     "linear<br/>uniform");
    addInParameter<StringCollection>(TARGET_TYPE, paramHelp[8], TARGET_TYPES, true,
                                     "nodes<br/>edges");
    addInParameter<StringCollection>("mapping proportionality", paramHelp[9],
                                     "area/volume;dimensions", true, "area/volume<br/>dimensions");
  }

  bool check(std::string &errorMsg) override {
    xaxis = yaxis = zaxis = true;
    min = 1;
    max = 10;
    proportional = AREA_PROPORTIONAL;
    entryMetric = graph->getProperty<DoubleProperty>("viewMetric");
    entrySize = graph->getProperty<SizeProperty>("viewSize");
    linearType = true;
    StringCollection mapping;
    StringCollection proportionalType;
    targetType.setCurrent(NODES_TARGET);

    if (dataSet != nullptr) {
      dataSet->get("metric", entryMetric);
      dataSet->get("input", entrySize);
      dataSet->get("width", xaxis);
      dataSet->get("height", yaxis);
      dataSet->get("depth", zaxis);
      dataSet->get("min size", min);
      dataSet->get("max size", max);
      dataSet->get(MAPPING_TYPE, mapping);
      linearType = mapping.getCurrent() == LINEAR_MAPPING;
      dataSet->get(TARGET_TYPE, targetType);
      dataSet->get("mapping proportionality", proportionalType);
      proportional = proportionalType.getCurrent();
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
      errorMsg = "You need at least one dimension (width, height or depth) to map on.";
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
      if (!xaxis || !yaxis || !zaxis)
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
      if (!xaxis || !yaxis)
        edgeSize.copyFromProperty(entrySize);

      TLP_PARALLEL_MAP_EDGES(graph, [&](const edge &e) {
        double sizos = min + (entryMetric->getEdgeDoubleValue(e) - shift) * (max - min) / range;

        if (xaxis)
          edgeSize[e][0] = float(sizos);

        if (yaxis)
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
  unsigned int proportional;
  StringCollection targetType;
};

PLUGIN(MetricSizeMapping)
