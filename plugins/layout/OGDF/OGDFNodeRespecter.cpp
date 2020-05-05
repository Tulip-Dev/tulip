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

#include <ogdf/energybased/NodeRespecterLayout.h>
#include <ogdf/basic/Math.h>

#include <tulip2ogdf/OGDFLayoutPluginBase.h>

using namespace tlp;
using namespace ogdf;

#define ELT_POSTPROCESSING "Post Processing"
#define ELT_POSTPROCESSINGLIST "None;KeepMultiEdgeBends;Complete"
#define ELT_POSTPROCESSINGNONE 0
#define ELT_POSTPROCESSINGKEEP 1
#define ELT_POSTPROCESSINGCOMPLETE 2

static const char *paramHelp[] = {
    // random initial placement
    "Use a random initial placement",

    // Post Processing
    "Sets whether unnecessary edge bends should be filtered out in a post-processing step.",

    // Bends Normalization Angle
    "Sets bends normalization angle in [0...Pi].",

    // number of iterations
    "The maximum number of iterations >=0.",

    // Minimal Temperature
    "The minimal Temperature >= 0",

    // Initial Temperature
    "Sets the initial Temperature > Minimal Temperature.",

    // Temperature Decrease
    "Sets temperature Decrease Offset in [0...1].",

    // Gravitation
    "Sets Gravitation >= 0.",

    // Oscillation Angle
    "Sets Oscillation Angle in [0...Pi].",

    // Desired Minimal Edge Length
    "Sets desired Min Edge Length > 0.",

    // Init Dummies Per Edge
    "Sets init Dummies Per Edge >= 0.",

    // Maximal Dummies Per Edge
    "Sets Dummies Per Edge > init Dummies Per Edge.",

    // Dummy Insertion Threshold
    "Sets dummy Insertion Threshold >= 1.",

    // Maximum Disturbance
    "Sets max Disturbance >= 0.",

    // Repulsion Distance
    "Sets repulsion Distance >= 0. By default, it is the double of Desired Minimal Edge Length",

    // Min Distance CC (double minDistCC)
    "Sets min Distance between CC >= 0.",

    // Page Ratio
    "Sets Page Ratio > 0."

};

static const char *postProcesingValuesDescription =
    "None <i>(Keep all bends. )</i><br>"
    "KeepMultiEdgeBends <i>(Activate post-processing but keep all bends on multi-edges and "
    "self-loops (such that the corresponding edges are visible).)</i><br>"
    "Complete <i>(Activate post-processing: Remove all bends that do not prevent edge-node "
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
      "The algorithm is documented in and was developed for the bachelor thesis: Max Ilsen: "
      "Energy-Based Layout Algorithms for Graphs with Large Nodes. University of Osnabrueck, 2017",
      "1.0", "Force Directed")
  OGDFNodeRespecter(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ogdf::NodeRespecterLayout()) {
    addInParameter<bool>("random initial placement", paramHelp[0], "true", false);
    addInParameter<StringCollection>(ELT_POSTPROCESSING, paramHelp[1], ELT_POSTPROCESSINGLIST,
                                     false, postProcesingValuesDescription);
    addInParameter<double>("Bends Normalization Angle", paramHelp[2], to_string(Math::pi), false);
    addInParameter<int>("number of iterations", paramHelp[3], "30000", false);
    addInParameter<double>("Minimal Temperature", paramHelp[4], "1.0", false);
    addInParameter<double>("Initial Temperature", paramHelp[5], "10.0", false);
    addInParameter<double>("Temperature Decrease", paramHelp[6], "0.0", false);
    addInParameter<double>("Gravitation", paramHelp[7], "0.0625", false);
    addInParameter<double>("Oscillation Angle", paramHelp[8], to_string(Math::pi_2), false);
    addInParameter<double>("Desired Minimal Edge Length", paramHelp[9],
                           to_string(LayoutStandards::defaultNodeSeparation()), false);
    addInParameter<int>("Init Dummies Per Edge", paramHelp[10], "1", false);
    addInParameter<int>("Maximal Dummies Per Edge", paramHelp[11], "3", false);
    addInParameter<double>("Dummy Insertion Threshold", paramHelp[12], "5", false);
    addInParameter<double>("Maximum Disturbance", paramHelp[13], "0", false);
    addInParameter<double>("Repulsion Distance", paramHelp[14],
                           to_string(2 * LayoutStandards::defaultNodeSeparation()), false);
    addInParameter<double>("Min Distance CC", paramHelp[15],
                           to_string(LayoutStandards::defaultCCSeparation()), false);
    addInParameter<double>("Page Ratio", paramHelp[16], "1.0", false);
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

      if (dataSet->get(ELT_POSTPROCESSING, stringCollection)) {
        if (stringCollection.getCurrent() == ELT_POSTPROCESSINGNONE) {
          npl->setPostProcessing(NodeRespecterLayout::PostProcessingMode::None);
        } else if (stringCollection.getCurrent() == ELT_POSTPROCESSINGKEEP) {
          npl->setPostProcessing(NodeRespecterLayout::PostProcessingMode::KeepMultiEdgeBends);
        } else {
          npl->setPostProcessing(NodeRespecterLayout::PostProcessingMode::Complete);
        }
      }

      if (dataSet->get("Bends Normalization Angle", dval))
        npl->setBendNormalizationAngle(dval);

      if (dataSet->get("number of iterations", ival))
        npl->setNumberOfIterations(ival);

      if (dataSet->get("Minimal Temperature", dval))
        npl->setMinimalTemperature(dval);

      if (dataSet->get("Initial Temperature", dval))
        npl->setInitialTemperature(dval);

      if (dataSet->get("Temperature Decrease", dval))
        npl->setTemperatureDecreaseOffset(dval);

      if (dataSet->get("Gravitation", dval))
        npl->setGravitation(dval);

      if (dataSet->get("Oscillation Angle", dval))
        npl->setOscillationAngle(dval);

      if (dataSet->get("Desired Minimal Edge Length", dval))
        npl->setDesiredMinEdgeLength(dval);

      if (dataSet->get("Init Dummies Per Edge", ival))
        npl->setInitDummiesPerEdge(ival);

      if (dataSet->get("Maximal Dummies Per Edge", ival))
        npl->setMaxDummiesPerEdge(ival);

      if (dataSet->get("Dummy Insertion Threshold", dval))
        npl->setDummyInsertionThreshold(dval);

      if (dataSet->get("Maximum Disturbance", dval))
        npl->setMaxDisturbance(dval);

      if (dataSet->get("Repulsion Distance", dval))
        npl->setRepulsionDistance(dval);

      if (dataSet->get("Min Distance CC", dval))
        npl->setMinDistCC(dval);

      if (dataSet->get("Page Ratio", dval))
        npl->setPageRatio(dval);
    }
  }
};

PLUGIN(OGDFNodeRespecter)
