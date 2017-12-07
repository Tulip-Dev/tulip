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

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
    // property
    "Input metric whose values will be mapped to sizes.",

    // input
    "If not all dimensions (width, height, depth) are checked below, the dimensions not computed "
    "are copied from this property.",

    // width, height, depth
    "Each checked dimension is adjusted to represent property, each unchecked dimension is copied "
    "from input.",

    // min
    "Gives the minimum value of the range of computed sizes.",

    // max
    "Gives the maximum value of the range of computed sizes.",

    // Mapping type
    "Type of mapping."
    "<ul><li>true: linear mapping (min value of property is mapped to min size, max to max size, "
    "and a linear interpolation is used in between.)</li>"
    "<li>false: uniform quantification (the values of property are sorted, and the same size "
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

class MetricSizeMapping : public SizeAlgorithm {
public:
  PLUGININFORMATION(
      "Size Mapping", "Auber", "08/08/2003",
      "Maps the sizes of the graph elements onto the values of a given numeric property.", "2.1",
      "Size")
  MetricSizeMapping(const PluginContext *context)
      : SizeAlgorithm(context), entryMetric(nullptr), entrySize(nullptr), xaxis(true), yaxis(true),
        zaxis(true), mappingType(true), min(1), max(10), range(0), shift(0) {
    addInParameter<NumericProperty *>("property", paramHelp[0], "viewMetric");
    addInParameter<SizeProperty>("input", paramHelp[1], "viewSize");
    addInParameter<bool>("width", paramHelp[2], "true");
    addInParameter<bool>("height", paramHelp[2], "true");
    addInParameter<bool>("depth", paramHelp[2], "false");
    addInParameter<double>("min size", paramHelp[3], "1");
    addInParameter<double>("max size", paramHelp[4], "10");
    addInParameter<bool>("type", paramHelp[5], "true");
    addInParameter<StringCollection>(TARGET_TYPE, paramHelp[6], TARGET_TYPES, true,
                                     "nodes <br> edges");
    addInParameter<StringCollection>("area proportional", paramHelp[7],
                                     "Area Proportional;Quadratic/Cubic", true,
                                     "Area Proportional <br> Quadratic/Cubic");

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
    mappingType = true;
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
      dataSet->get("type", mappingType);
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

    if (!mappingType) {
      tmp = entryMetric->copyProperty(graph);
      tmp->uniformQuantification(300);
      entryMetric = tmp;
    }

    pluginProgress->showPreview(false);

    if (targetType.getCurrent() == NODES_TARGET) {
      shift = entryMetric->getNodeDoubleMin(graph);

      // compute size of nodes
      const vector<node> &nodes = graph->nodes();
      NodeStaticProperty<Size> nodeSize(graph);
      nodeSize.copyFromProperty(entrySize);
      unsigned nbNodes = nodes.size();
#if defined(_OPENMP)
#pragma omp parallel for
#endif
      for (OMP_ITER_TYPE i = 0; i < nbNodes; ++i) {
        node n = nodes[i];
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
      }
      nodeSize.copyToProperty(result);
    } else {
      shift = entryMetric->getEdgeDoubleMin(graph);
      // compute size of edges
      vector<edge> edges = graph->edges();
      EdgeStaticProperty<Size> edgeSize(graph);
      unsigned nbEdges = edges.size();
#if defined(_OPENMP)
#pragma omp parallel for
#endif
      for (OMP_ITER_TYPE i = 0; i < nbEdges; ++i) {
        edge e = edges[i];
        double sizos = min + (entryMetric->getEdgeDoubleValue(e) - shift) * (max - min) / range;
        edgeSize[e][0] = float(sizos);
        edgeSize[e][1] = float(sizos);
      }
      edgeSize.copyToProperty(result);
    }

    if (!mappingType)
      delete tmp;

    return true;
  }

private:
  NumericProperty *entryMetric;
  SizeProperty *entrySize;
  bool xaxis, yaxis, zaxis, mappingType;
  double min, max;
  double range;
  double shift;
  std::string proportional;
  StringCollection targetType;
};

PLUGIN(MetricSizeMapping)
