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
class OGDFStressMajorization : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Stress Majorization (OGDF)","Karsten Klein","12/11/2007",
                    "Implements an alternative to force-directed layout which is a distance-based layout realized by the stress majorization approach. ","2.0","Force Directed")
  OGDFStressMajorization(const tlp::PluginContext* context) :OGDFLayoutPluginBase(context, new ogdf::StressMinimization()) {
    addInParameter<StringCollection>("terminationCriterion",
                                     HTML_HELP_OPEN()
                                     HTML_HELP_DEF( "type", "StringCollection" )
                                     HTML_HELP_DEF("values", "- None <br/>"
                                         "- PositionDifference <br/>"
                                         "- Stress </i>")
                                     HTML_HELP_DEF( "default", "None" )
                                     HTML_HELP_BODY()
                                     "Tells which TERMINATION_CRITERIA should be used."
                                     HTML_HELP_CLOSE(),
                                     "None;PositionDifference;Stress");
    addInParameter<bool>("fixXCoordinates",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "bool" )
                         HTML_HELP_DEF( "default", "false" )
                         HTML_HELP_BODY()
                         "Tells whether the x coordinates are allowed to be modified or not."
                         HTML_HELP_CLOSE(),
                         "false");
    addInParameter<bool>("fixYCoordinates",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "bool" )
                         HTML_HELP_DEF( "default", "false" )
                         HTML_HELP_BODY()
                         "Tells whether the y coordinates are allowed to be modified or not."
                         HTML_HELP_CLOSE(),
                         "false");
    addInParameter<bool>("hasInitialLayout",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "bool" )
                         HTML_HELP_DEF( "default", "false" )
                         HTML_HELP_BODY()
                         "Tells whether the current layout should be used or the initial layout needs to be computed."
                         HTML_HELP_CLOSE(),
                         "false");
    addInParameter<bool>("layoutComponentsSeparately",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "bool" )
                         HTML_HELP_DEF( "default", "false" )
                         HTML_HELP_BODY()
                         "Sets whether the graph's components should be layouted separately or a dummy distance should be used for nodes within different components."
                         HTML_HELP_CLOSE(),
                         "false");
    addInParameter<int>("numberOfIterations",
                        HTML_HELP_OPEN()
                        HTML_HELP_DEF( "type", "int" )
                        HTML_HELP_DEF( "default", "200" )
                        HTML_HELP_BODY()
                        "Sets a fixed number of iterations for stress majorization. If the new value is smaller or equal 0 the default value (200) is used."
                        HTML_HELP_CLOSE(),
                        "200");
    addInParameter<double>("edgeCosts",
                           HTML_HELP_OPEN()
                           HTML_HELP_DEF( "type", "double" )
                           HTML_HELP_DEF( "default", "100" )
                           HTML_HELP_BODY()
                           "Sets the desired distance between adjacent nodes. If the new value is smaller or equal 0 the default value (100) is used."
                           HTML_HELP_CLOSE(),
                           "100");
    addInParameter<bool>("useEdgeCostsProperty",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "bool" )
                         HTML_HELP_DEF( "default", "false" )
                         HTML_HELP_BODY()
                         "Tells whether the edge costs are uniform or defined in an edge costs property."
                         HTML_HELP_CLOSE(),
                         "false");
    addInParameter<tlp::NumericProperty*>("edgeCostsProperty",
                                          HTML_HELP_OPEN()
                                          HTML_HELP_DEF( "type", "NumericProperty" )
                                          HTML_HELP_DEF( "default", "viewMetric" )
                                          HTML_HELP_BODY()
                                          "The numeric property that holds the desired cost for each edge."
                                          HTML_HELP_CLOSE(),
                                          "viewMetric");

  }

  ~OGDFStressMajorization() {}

  void beforeCall() {
    ogdf::StressMinimization *stressm = static_cast<ogdf::StressMinimization*>(ogdfLayoutAlgo);

    if (dataSet != NULL) {
      double dval = 0;
      int ival = 0;
      bool bval = false;
      StringCollection sc;
      tlp::NumericProperty *edgeCosts = graph->getProperty<tlp::DoubleProperty>("viewMetric");

      if (dataSet->get("terminationCriterion", sc)) {
        if (sc.getCurrentString() == "PositionDifference") {
          stressm->convergenceCriterion(ogdf::StressMinimization::POSITION_DIFFERENCE);
        }
        else if (sc.getCurrentString() == "Stress") {
          stressm->convergenceCriterion(ogdf::StressMinimization::STRESS);
        }
        else {
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
