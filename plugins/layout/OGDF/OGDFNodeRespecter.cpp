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

#include <ogdf/energybased/NodeRespecterLayout.h>
#include <ogdf/basic/Math.h>

#include <tulip2ogdf/OGDFLayoutPluginBase.h>
#include <tulip/StringCollection.h>

using namespace tlp;
using namespace ogdf;

#define POSTPROCESSINGLIST "none;keep edges visible;complete"
#define POSTPROCESSINGNONE 0
#define POSTPROCESSINGKEEP 1
#define POSTPROCESSINGCOMPLETE 2

static const char *paramHelp[] = {
    // random initial placement
    "Use a random initial placement",

    // Post Processing
    "Sets whether unnecessary edge bends should be filtered out in a post-processing step.",

    // Bends Normalization Angle
    "Sets bends normalization angle in [0...Pi].",

    // number of iterations
    "The maximum number of iterations >=0.",

    // min temperature
    "The minimum temperature >= 0",

    // initial temperature
    "Sets the initial temperature > minimal temperature.",

    // Temperature Decrease
    "Sets temperature Decrease Offset in [0...1].",

    // Gravitation
    "Sets gravitation >= 0.",

    // Oscillation Angle
    "Sets oscillation angle in [0...Pi].",

    // min edge length
    "Sets minimum edge length > 0.",

    // Init Dummies Per Edge
    "Sets init dummies per edge >= 0.",

    // max dummies per edge
    "Sets dummies per edge > init dummies per edge.",

    // dummy insertion threshold
    "Sets dummy insertion threshold >= 1.",

    // max disturbance
    "Sets maximum disturbance >= 0.",

    // Repulsion Distance
    "Sets repulsion distance >= 0. By default, it is the double of minimal edge length",

    // Min Distance CC (double minDistCC)
    "Sets minimum distance between connected components (>= 0).",

    // page ratio
    "Sets page ratio > 0."

};

static const char *postProcessingValuesDescription =
    "none <i>(Keep all bends. )</i><br>"
    "keep edges visible <i>(Activate post-processing but keep all bends on multi-edges and "
    "self-loops (such that the corresponding edges are visible).)</i><br>"
    "complete <i>(Activate post-processing: Remove all bends that do not prevent edge-node "
    "intersections.)</i><br>";

class OGDFNodeRespecter : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION(
      "Node Respecter (OGDF)", "Max Isen", "12/11/2017",
      "This is a force-directed layout algorithm respecting the shapes and sizes of nodes."
      "It aims to minimize the number of node overlaps as well as the number of edges crossing "
      "through non-incident nodes."
      "In order to achieve this, the algorithm adapts its forces to the node sizes and bends edges "
      "around close-by nodes."
      "The edge bends are created by introducing dummy nodes into the graph, positioning all nodes "
      "according to forces acting upon them,"
      "filtering out unnecessary dummy nodes, and then replacing the remaining dummy nodes by edge "
      "bends."
      "The algorithm is documented in and was developed for the bachelor thesis:<br/>"
      "<b>Energy-Based Layout Algorithms for Graphs with Large Nodes</b>,<br/>Max Ilsen, University of Osnabrueck, 2017",
      "1.1", "Force Directed")
  OGDFNodeRespecter(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, context ? new ogdf::NodeRespecterLayout() : nullptr) {
    addInParameter<bool>("random initial placement", paramHelp[0], "true", false);
    addInParameter<StringCollection>("post processing", paramHelp[1], POSTPROCESSINGLIST, false,
                                     postProcessingValuesDescription);
    addInParameter<double>("bends normalization angle", paramHelp[2], to_string(Math::pi), false);
    addInParameter<int>("number of iterations", paramHelp[3], "30000", false);
    addInParameter<double>("min temperature", paramHelp[4], "1.0", false);
    addInParameter<double>("initial temperature", paramHelp[5], "10.0", false);
    addInParameter<double>("temperature decrease", paramHelp[6], "0.0", false);
    addInParameter<double>("gravitation", paramHelp[7], "0.0625", false);
    addInParameter<double>("oscillation angle", paramHelp[8], to_string(Math::pi_2), false);
    addInParameter<double>("min edge length", paramHelp[9],
                           to_string(LayoutStandards::defaultNodeSeparation()), false);
    addInParameter<int>("init dummies per edge", paramHelp[10], "1", false);
    addInParameter<int>("max dummies per edge", paramHelp[11], "3", false);
    addInParameter<double>("dummy insertion threshold", paramHelp[12], "5", false);
    addInParameter<double>("max disturbance", paramHelp[13], "0", false);
    addInParameter<double>("repulsion distance", paramHelp[14],
                           to_string(2 * LayoutStandards::defaultNodeSeparation()), false);
    addInParameter<double>("connected components spacing", paramHelp[15],
                           to_string(LayoutStandards::defaultCCSeparation()), false);
    addInParameter<double>("page ratio", paramHelp[16], "1.0", false);
  }

  void beforeCall() override {
    ogdf::NodeRespecterLayout *npl = static_cast<ogdf::NodeRespecterLayout *>(ogdfLayoutAlgo);
    if (dataSet != nullptr) {
      double dval = 0;
      int ival = 0;
      bool bval(false);
      StringCollection stringCollection;

      if (dataSet->get("random initial placement", bval))
        npl->setRandomInitialPlacement(bval);

      if (dataSet->get("post processing", stringCollection)) {
        switch (stringCollection.getCurrent()) {
        case POSTPROCESSINGNONE:
          npl->setPostProcessing(NodeRespecterLayout::PostProcessingMode::None);
          break;
        case POSTPROCESSINGKEEP:
          npl->setPostProcessing(NodeRespecterLayout::PostProcessingMode::KeepMultiEdgeBends);
          break;
        default:
          npl->setPostProcessing(NodeRespecterLayout::PostProcessingMode::Complete);
        }
      }

      if (dataSet->get("bends normalization angle", dval))
        npl->setBendNormalizationAngle(dval);

      if (dataSet->get("number of iterations", ival))
        npl->setNumberOfIterations(ival);

      if (dataSet->get("min temperature", dval))
        npl->setMinimalTemperature(dval);

      if (dataSet->get("initial temperature", dval))
        npl->setInitialTemperature(dval);

      if (dataSet->get("temperature decrease", dval))
        npl->setTemperatureDecreaseOffset(dval);

      if (dataSet->get("gravitation", dval))
        npl->setGravitation(dval);

      if (dataSet->get("oscillation angle", dval))
        npl->setOscillationAngle(dval);

      if (dataSet->get("min edge length", dval))
        npl->setDesiredMinEdgeLength(dval);

      if (dataSet->get("init dummies per edge", ival))
        npl->setInitDummiesPerEdge(ival);

      if (dataSet->get("max dummies per edge", ival))
        npl->setMaxDummiesPerEdge(ival);

      if (dataSet->get("dummy insertion threshold", dval))
        npl->setDummyInsertionThreshold(dval);

      if (dataSet->get("max disturbance", dval))
        npl->setMaxDisturbance(dval);

      if (dataSet->get("repulsion distance", dval))
        npl->setRepulsionDistance(dval);

      if (dataSet->get("connected components spacing", dval))
        npl->setMinDistCC(dval);

      if (dataSet->get("page ratio", dval))
        npl->setPageRatio(dval);
    }
  }
};

PLUGIN(OGDFNodeRespecter)
