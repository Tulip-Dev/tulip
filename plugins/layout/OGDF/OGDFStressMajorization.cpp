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

#include <ogdf/energybased/StressMinimization.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

#include <tulip/StringCollection.h>
#include <tulip/NumericProperty.h>

// comments below have been extracted from OGDF/src/energybased/StressMajorizationSimple.cpp
/** \addtogroup layout */

/**
 * Implementation of stress-majorization layout algorithm.
 *
 * \author Karsten Klein
 *
 * \par License:
 * This is part of the Open Graph Drawing Framework (OGDF).
 * Copyright (C) 2005-2009
 *
 * \par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 or 3 as published by the Free Software Foundation
 * and appearing in the files LICENSE_GPL_v2.txt and
 * LICENSE_GPL_v3.txt included in the packaging of this file.
 *
 * \par
 * In addition, as a special exception, you have permission to link
 * this software with the libraries of the COIN-OR Osi project
 * (http://www.coin-or.org/projects/Osi.xml), all libraries required
 * by Osi, and all LP-solver libraries directly supported by the
 * COIN-OR Osi project, and distribute executables, as long as
 * you follow the requirements of the GNU General Public License
 * in regard to all of the software in the executable aside from these
 * third-party libraries.
 *
 * \par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * \par
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * \see  http://www.gnu.org/copyleft/gpl.html
 ***************************************************************/

static const char *paramHelp[] = {
    // terminationCriterion
    "Tells which TERMINATION_CRITERIA should be used.",

    // fixXCoordinates
    "Tells whether the x coordinates are allowed to be modified or not.",

    // fixYCoordinates
    "Tells whether the y coordinates are allowed to be modified or not.",

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

class OGDFStressMajorization : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Stress Majorization (OGDF)", "Karsten Klein", "12/11/2007",
                    "Implements an alternative to force-directed layout which is a distance-based "
                    "layout realized by the stress majorization approach. ",
                    "2.0", "Force Directed")
  OGDFStressMajorization(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ogdf::StressMinimization()) {
    addInParameter<StringCollection>("terminationCriterion", paramHelp[0],
                                     "None;PositionDifference;Stress", true,
                                     "None <br> PositionDifference <br> Stress");
    addInParameter<bool>("fixXCoordinates", paramHelp[1], "false");
    addInParameter<bool>("fixYCoordinates", paramHelp[2], "false");
    addInParameter<bool>("hasInitialLayout", paramHelp[3], "false");
    addInParameter<bool>("layoutComponentsSeparately", paramHelp[4], "false");
    addInParameter<int>("numberOfIterations", paramHelp[5], "200");
    addInParameter<double>("edgeCosts", paramHelp[6], "100");
    addInParameter<bool>("useEdgeCostsProperty", paramHelp[7], "false");
    addInParameter<tlp::NumericProperty *>("edgeCostsProperty", paramHelp[8], "viewMetric");
  }

  ~OGDFStressMajorization() override {}

  void beforeCall() override {
    ogdf::StressMinimization *stressm = static_cast<ogdf::StressMinimization *>(ogdfLayoutAlgo);

    if (dataSet != nullptr) {
      double dval = 0;
      int ival = 0;
      bool bval = false;
      StringCollection sc;
      tlp::NumericProperty *edgeCosts = graph->getProperty<tlp::DoubleProperty>("viewMetric");

      if (dataSet->get("terminationCriterion", sc)) {
        if (sc.getCurrentString() == "PositionDifference") {
          stressm->convergenceCriterion(ogdf::StressMinimization::POSITION_DIFFERENCE);
        } else if (sc.getCurrentString() == "Stress") {
          stressm->convergenceCriterion(ogdf::StressMinimization::STRESS);
        } else {
          stressm->convergenceCriterion(ogdf::StressMinimization::NONE);
        }
      }

      if (dataSet->get("fixXCoordinates", bval)) {
        stressm->fixXCoordinates(bval);
      }

      if (dataSet->get("fixYCoordinates", bval)) {
        stressm->fixXCoordinates(bval);
      }

      if (dataSet->get("hasInitialLayout", bval)) {
        stressm->hasInitialLayout(bval);
      }

      if (dataSet->get("layoutComponentsSeparately", bval)) {
        stressm->layoutComponentsSeparately(bval);
      }

      if (dataSet->get("numberOfIterations", ival)) {
        stressm->setIterations(ival);
      }

      if (dataSet->get("edgeCosts", dval)) {
        stressm->setEdgeCosts(dval);
      }

      if (dataSet->get("useEdgeCostsProperty", bval)) {
        stressm->useEdgeCostsAttribute(bval);

        if (bval) {
          dataSet->get("edgeCostsProperty", edgeCosts);
          tlpToOGDF->copyTlpNumericPropertyToOGDFEdgeLength(edgeCosts);
        }
      }
    }
  }
};

PLUGIN(OGDFStressMajorization)
