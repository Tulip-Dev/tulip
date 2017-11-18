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
#include <cmath>
#include <limits>

#include <tulip/TulipPluginHeaders.h>
#include <tulip/Color.h>
#include <tulip/Vector.h>
#include <tulip/ColorScale.h>
#include <tulip/StringCollection.h>

#ifndef TULIP_BUILD_CORE_ONLY
#include "DoubleStringsListRelationDialog.h"
#endif

using namespace tlp;

static const char *paramHelp[] = {
    // type
    "If linear, logarithmic or uniform, the input property must be a <b>numeric</b> property. For "
    "the linear case, the minimum value is mapped to one end of the color scale, "
    "the maximum value is mapped to the other end, and a linear interpolation is used between both "
    "to compute the associated color. For the logarithmic case, graph elements values are first "
    "mapped in the [1, +inf[ range. "
    "Then the log of each mapped value is computed and used to compute the associated color of the "
    "graph element trough a linear interpolation between 0 and the log of the mapped maximum value "
    "of graph elements.<BR>"
    "If uniform, this is the same except for the interpolation: the values are sorted, numbered, "
    "and a linear interpolation is used on those numbers"
    "(in other words, only the order is taken into account, not the actual values).<BR>"
    "Finally, if enumerated, the input property can be of <b>any type</b>. Each possible value is "
    "mapped to a distinct color without any specific order.",

    // property
    "This property is used to get the values affected to graph items.",

    // target
    "Whether colors are computed for nodes or for edges.",

    // color scale
    "The color scale used to transform a node/edge property value into a color.",

    // override min
    "If true override the minimum value of the input property to keep coloring consistent across "
    "datasets.",

    // min
    "That value will be used to override the minimum one of the input property.",

    // override max
    "If true override the maximum value of the input property to keep coloring consistent across "
    "datasets.",

    // max
    "That value will be used to override the maximum one of the input property."};

#define ELT_TYPE "type"
#define ELT_TYPES "linear;uniform;enumerated;logarithmic"
#define LINEAR_ELT 0
#define UNIFORM_ELT 1
#define ENUMERATED_ELT 2
#define LOGARITHMIC_ELT 3

#define TARGET_TYPE "target"
#define TARGET_TYPES "nodes;edges"
#define NODES_TARGET 0
#define EDGES_TARGET 1

class ColorMapping : public ColorAlgorithm {
private:
  NumericProperty *entryMetric;
  StringCollection eltTypes;
  StringCollection targetType;
  ColorScale colorScale;
  Vector<float, 4> deltaRGBA;
  std::vector<std::pair<std::string, Color> > enumeratedMappingResultVector;
  std::map<std::string, std::vector<unsigned int> > mapMetricElements;
  double maxInput;
  double minInput;
  bool overrideMaxInput;
  bool overrideMinInput;

public:
  PLUGININFORMATION(
      "Color Mapping", "Mathiaut", "16/09/2010",
      "Colorizes the nodes or edges of a graph according to the values of a given property.", "2.2",
      "")
  ColorMapping(const tlp::PluginContext *context)
      : ColorAlgorithm(context), entryMetric(NULL), eltTypes(ELT_TYPES),
        maxInput(std::numeric_limits<double>::quiet_NaN()),
        minInput(std::numeric_limits<double>::quiet_NaN()), overrideMaxInput(false),
        overrideMinInput(false) {
    addInParameter<StringCollection>(ELT_TYPE, paramHelp[0], ELT_TYPES, true,
                                     "linear <br> uniform <br> enumerated <br> logarithmic");
    addInParameter<PropertyInterface *>("input property", paramHelp[1], "viewMetric");
    addInParameter<StringCollection>(TARGET_TYPE, paramHelp[2], TARGET_TYPES, true,
                                     "nodes <br> edges");
    addInParameter<ColorScale>("color scale", paramHelp[3], "");
    addInParameter<bool>("override minimum value", paramHelp[4], "false", false);
    addInParameter<double>("minimum value", paramHelp[5], "", false);
    addInParameter<bool>("override maximum value", paramHelp[6], "false", false);
    addInParameter<double>("maximum value", paramHelp[7], "", false);

    // result needs to be an inout parameter
    // in order to preserve the original values of non targetted elements
    // i.e if "target" = "nodes", the values of edges must be preserved
    // and if "target" = "edges", the values of nodes must be preserved
    parameters.setDirection("result", INOUT_PARAM);
  }

  //=========================================================
  ~ColorMapping() {}
  //=========================================================
  Color getColor(double value, double range) {
    if (range == 0)
      range = 1;

    if (value < 0)
      value = 0;

    if (value > range)
      value = range;

    return colorScale.getColorAtPos(value / range);
  }
  //=========================================================
  bool run() {

    eltTypes.setCurrent(LINEAR_ELT);
    targetType.setCurrent(NODES_TARGET);
    NumericProperty *metricS = NULL;
    PropertyInterface *metric = NULL;

    if (dataSet != NULL) {
      dataSet->get("input property", metric);
      dataSet->get(ELT_TYPE, eltTypes);
      dataSet->get(TARGET_TYPE, targetType);
      dataSet->get("override minimum value", overrideMinInput);
      dataSet->get("minimum value", minInput);
      dataSet->get("override maximum value", overrideMaxInput);
      dataSet->get("maximum value", maxInput);

      /// Dont allow NaN input
      if (overrideMaxInput &&
#if defined(_MSC_VER) && (_MSC_VER < 1800)
          isnan(minInput)
#else
          std::isnan(minInput)
#endif
              )
        minInput = 0;

      if (overrideMinInput &&
#if defined(_MSC_VER) && (_MSC_VER < 1800)
          isnan(maxInput)
#else
          std::isnan(maxInput)
#endif
              )
        maxInput = 0;

      if (overrideMinInput && overrideMaxInput) {
        /// check for impossible values
        if (minInput > maxInput)
          minInput = maxInput;

        if (maxInput < minInput)
          maxInput = minInput;
      }
    }

    if (metric == NULL)
      metricS = graph->getProperty<DoubleProperty>("viewMetric");
    else
      metricS = dynamic_cast<NumericProperty *>(metric);

    if (eltTypes.getCurrent() != ENUMERATED_ELT) {
      if (eltTypes.getCurrent() == LINEAR_ELT || eltTypes.getCurrent() == LOGARITHMIC_ELT) {
        entryMetric = metricS;
      } else {
        NumericProperty *tmp = metricS->copyProperty(graph);
        tmp->uniformQuantification(300);
        entryMetric = tmp;
      }

      // loop on nodes
      if (targetType.getCurrent() == NODES_TARGET && graph->numberOfNodes() != 0) {
        unsigned int maxIter = graph->numberOfNodes();
        unsigned int iter = 0;
        double minN = overrideMinInput ? minInput : entryMetric->getNodeDoubleMin(graph);
        double maxN = overrideMaxInput ? maxInput : entryMetric->getNodeDoubleMax(graph);

        if (eltTypes.getCurrent() == LOGARITHMIC_ELT) {
          maxN = log(1 + maxN - minN);
        }

        node n;
        forEach(n, graph->getNodes()) {
          double dd = entryMetric->getNodeDoubleValue(n);

          if (eltTypes.getCurrent() == LOGARITHMIC_ELT) {
            result->setNodeValue(n, getColor(log(dd + (1 - minN)), maxN));
          } else {
            result->setNodeValue(n, getColor(dd - minN, maxN - minN));
          }

          if ((iter % 100 == 0) && (pluginProgress->progress(iter, maxIter) != TLP_CONTINUE)) {
            if (eltTypes.getCurrent() == UNIFORM_ELT)
              delete entryMetric;

            return pluginProgress->state() != TLP_CANCEL;
          }

          ++iter;
        }
      }

      // loop on edges
      if (targetType.getCurrent() == EDGES_TARGET && graph->numberOfEdges() != 0) {
        unsigned int maxIter = graph->numberOfEdges();
        unsigned int iter = 0;

        double minE = overrideMinInput ? minInput : entryMetric->getEdgeDoubleMin(graph);
        double maxE = overrideMaxInput ? maxInput : entryMetric->getEdgeDoubleMax(graph);

        if (eltTypes.getCurrent() == LOGARITHMIC_ELT) {
          maxE = log(1 + maxE - minE);
        }

        edge e;
        forEach(e, graph->getEdges()) {
          double dd = entryMetric->getEdgeDoubleValue(e);

          if (eltTypes.getCurrent() == LOGARITHMIC_ELT) {
            result->setEdgeValue(e, getColor(log(dd + (1 - minE)), maxE));
          } else {
            result->setEdgeValue(e, getColor(dd - minE, maxE - minE));
          }

          if ((iter % 100 == 0) && (pluginProgress->progress(iter, maxIter) != TLP_CONTINUE)) {
            if (eltTypes.getCurrent() == UNIFORM_ELT)
              delete entryMetric;

            return pluginProgress->state() != TLP_CANCEL;
          }

          ++iter;
        }
      }

      if (eltTypes.getCurrent() == UNIFORM_ELT)
        delete entryMetric;
    } else {
      unsigned int maxIter = (targetType.getCurrent() == NODES_TARGET) ? graph->numberOfNodes()
                                                                       : graph->numberOfEdges();
      unsigned int iter = 0;

      for (std::vector<std::pair<std::string, Color> >::iterator it =
               enumeratedMappingResultVector.begin();
           it != enumeratedMappingResultVector.end(); ++it) {
        std::vector<unsigned int> *elements = &mapMetricElements[(*it).first];

        for (std::vector<unsigned>::iterator itE = elements->begin(); itE != elements->end();
             ++itE) {
          if (targetType.getCurrent() == NODES_TARGET)
            result->setNodeValue(node(*itE), (*it).second);
          else
            result->setEdgeValue(edge(*itE), (*it).second);

          if ((iter % 100 == 0) && (pluginProgress->progress(iter, maxIter) != TLP_CONTINUE)) {
            return pluginProgress->state() != TLP_CANCEL;
          }

          ++iter;
        }
      }
    }

    return true;
  }
  //=========================================================
  bool check(std::string &errorMsg) {

    PropertyInterface *metric = NULL;

    if (dataSet != NULL) {
      dataSet->get("input property", metric);
      dataSet->get(ELT_TYPE, eltTypes);
      dataSet->get(TARGET_TYPE, targetType);

      if (dataSet->exist("color scale")) {
        dataSet->get("color scale", colorScale);
      } else {
        // for backward compatibility with the old parameter name
        dataSet->get("colorScale", colorScale);
      }

      dataSet->get("maximum value", maxInput);
      dataSet->get("minimum value", minInput);
    }

    if (metric == NULL)
      metric = graph->getProperty<DoubleProperty>("viewMetric");

    if (eltTypes.getCurrent() == ENUMERATED_ELT) {
#ifndef TULIP_BUILD_CORE_ONLY

      if (targetType.getCurrent() == NODES_TARGET) {

        node n;
        forEach(n, graph->getNodes()) {
          std::string tmp = metric->getNodeStringValue(n);

          if (mapMetricElements.count(tmp) == 0)
            mapMetricElements[tmp] = std::vector<unsigned int>();

          mapMetricElements[tmp].push_back(n.id);
        }
      } else {

        edge e;
        forEach(e, graph->getEdges()) {
          std::string tmp = metric->getEdgeStringValue(e);

          if (mapMetricElements.count(tmp) == 0)
            mapMetricElements[tmp] = std::vector<unsigned int>();

          mapMetricElements[tmp].push_back(e.id);
        }
      }

      std::vector<std::string> enumeratedValues;

      for (std::map<std::string, std::vector<unsigned int> >::iterator it =
               mapMetricElements.begin();
           it != mapMetricElements.end(); ++it) {
        enumeratedValues.push_back(it->first);
      }

      std::map<float, Color> colorMap = colorScale.getColorMap();

      std::vector<Color> enumeratedColors;

      for (std::map<float, Color>::iterator it = colorMap.begin(); it != colorMap.end(); ++it) {
        if (enumeratedColors.empty() || it->second != enumeratedColors.back())
          enumeratedColors.push_back(it->second);
      }

      DoubleStringsListRelationDialog dialog(enumeratedValues, enumeratedColors);

      if (!dialog.exec()) {
        errorMsg += "Cancelled by user";
        return false;
      }

      dialog.getResult(enumeratedMappingResultVector);
#else
      errorMsg += "enumerated color mapping is not available";
      return false;
#endif
    } else {
      // check if input property is a NumericProperty
      if (!dynamic_cast<NumericProperty *>(metric)) {
        errorMsg += "For a linear, logarithmic or uniform color mapping,\nthe input property must "
                    "be a Double or Integer property";
        return false;
      }
    }

    return true;
  }
};

PLUGIN(ColorMapping)
