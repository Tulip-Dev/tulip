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

#include <ogdf/energybased/StressMinimization.h>

#include <tulip2ogdf/OGDFLayoutPluginBase.h>

#include <tulip/StringCollection.h>
#include <tulip/NumericProperty.h>

using namespace tlp;
using namespace ogdf;

static const char *paramHelp[] = {
    // terminationCriterion
    "Tells which TERMINATION_CRITERIA should be used.",

    // fixXCoordinates
    "Tells whether the x coordinates are allowed to be modified or not.",

    // fixYCoordinates
    "Tells whether the y coordinates are allowed to be modified or not.",

    // fixZCoordinates
    "Tells whether the z coordinates are allowed to be modified or not.",

    // hasInitialLayout
    "Tells whether the current layout should be used or the initial layout needs to be computed.",

    // layoutComponentsSeparately
    "Sets whether the graph components should be layouted separately or a dummy distance should be "
    "used for nodes within different components.",

    // numberOfIterations
    "Sets a fixed number of iterations for stress majorization. If the new value is smaller or "
    "equal 0 the default value (200) is used.",

    // edgeCosts
    "Sets the desired distance between adjacent nodes. If the new value is smaller or equal 0 the "
    "default value (100) is used.",

    // useEdgeCostsProperty
    "Tells whether the edge costs are uniform or defined in an edge costs property.",

    // edgeCostsProperty
    "The numeric property that holds the desired cost for each edge."

};

#define TERMINATION_CRITERIA_LIST "none;position difference;stress"
#define POSITION_DIFFERENCE 1
#define STRESS 2

class OGDFStressMinimization : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Stress Minimization (OGDF)", "Karsten Klein", "12/11/2007",
                    "Implements an alternative to force-directed layout which is a distance-based "
                    "layout realized by the stress minimization via majorization algorithm. ",
                    "2.0", "Force Directed")
  OGDFStressMinimization(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, context ? new ogdf::StressMinimization() : nullptr) {
    addInParameter<StringCollection>("termination criterion", paramHelp[0],
                                     TERMINATION_CRITERIA_LIST, true,
                                     "none<br/>position difference<br/>stress");
    addInParameter<bool>("fix x coordinates", paramHelp[1], "false");
    addInParameter<bool>("fix y coordinates", paramHelp[2], "false");
    addInParameter<bool>("fix z coordinates", paramHelp[3], "false");
    addInParameter<bool>("has initial layout", paramHelp[4], "false");
    addInParameter<bool>("layout components separately", paramHelp[5], "false");
    addInParameter<int>("number of iterations", paramHelp[6], "200");
    addInParameter<double>("edge costs", paramHelp[7], "100");
    addInParameter<bool>("use edge costs property", paramHelp[8], "false");
    addInParameter<tlp::NumericProperty *>("edge costs property", paramHelp[9], "viewMetric");
  }

  ~OGDFStressMinimization() override {}

  void beforeCall() override {
    ogdf::StressMinimization *stressm = static_cast<ogdf::StressMinimization *>(ogdfLayoutAlgo);

    if (dataSet != nullptr) {
      double dval = 0;
      int ival = 0;
      bool bval = false;
      StringCollection sc;
      tlp::NumericProperty *edgeCosts = graph->getProperty<tlp::DoubleProperty>("viewMetric");

      if (dataSet->get("termination criterion", sc)) {
        switch (sc.getCurrent()) {
        case POSITION_DIFFERENCE:
          stressm->convergenceCriterion(
              StressMinimization::TerminationCriterion::PositionDifference);
          break;
        case STRESS:
          stressm->convergenceCriterion(StressMinimization::TerminationCriterion::Stress);
          break;
        default:
          stressm->convergenceCriterion(StressMinimization::TerminationCriterion::None);
        }
      }

      if (dataSet->get("fix x coordinates", bval)) {
        stressm->fixXCoordinates(bval);
      }

      if (dataSet->get("fix y coordinates", bval)) {
        stressm->fixYCoordinates(bval);
      }

      if (dataSet->get("fix z coordinates", bval)) {
        stressm->fixZCoordinates(bval);
      }

      if (dataSet->get("has initial layout", bval)) {
        stressm->hasInitialLayout(bval);
      }

      if (dataSet->get("layout components separately", bval)) {
        stressm->layoutComponentsSeparately(bval);
      }

      if (dataSet->get("number of iterations", ival)) {
        stressm->setIterations(ival);
      }

      if (dataSet->get("edge costs", dval)) {
        stressm->setEdgeCosts(dval);
      }

      if (dataSet->get("use edge costs property", bval)) {
        stressm->useEdgeCostsAttribute(bval);

        if (bval) {
          dataSet->get("edge costs property", edgeCosts);
          tlpToOGDF->copyTlpNumericPropertyToOGDFEdgeLength(edgeCosts);
        }
      }
    }
  }
};

PLUGIN(OGDFStressMinimization)
