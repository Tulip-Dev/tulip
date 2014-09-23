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

#include <ogdf/energybased/StressMajorizationSimple.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

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
  PLUGININFORMATION("Stress Majorization (OGDF)","Karsten Klein","12/11/2007","Ok","1.0","Force Directed")
  OGDFStressMajorization(const tlp::PluginContext* context) :OGDFLayoutPluginBase(context, new ogdf::StressMajorization()) {
    addInParameter<int>("iterations",
                        HTML_HELP_OPEN()
                        HTML_HELP_DEF( "type", "int" )
                        HTML_HELP_BODY()
                        "Sets a fixed number of iterations for stress majorization in main step."
                        HTML_HELP_CLOSE(),
                        "300");
    addInParameter<double>("stop tolerance",
                           HTML_HELP_OPEN()
                           HTML_HELP_DEF( "type", "double" )
                           HTML_HELP_BODY()
                           "The value for the stop tolerance, below which the system is regarded stable (balanced) and the optimization stopped. "
                           HTML_HELP_CLOSE(),
                           "0.001");
    addInParameter<bool>("used layout",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "bool" )
                         HTML_HELP_BODY()
                         "If set to true, the given layout is used for the initial positions."
                         HTML_HELP_CLOSE(),
                         "false");
    addInParameter<bool>("compute max iterations",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "bool" )
                         HTML_HELP_BODY()
                         "If set to true, number of iterations is computed depending on G."
                         HTML_HELP_CLOSE(),
                         "true");
    addInParameter<int>("global iterations",
                        HTML_HELP_OPEN()
                        HTML_HELP_DEF( "type", "int" )
                        HTML_HELP_BODY()
                        "The number of global iterations."
                        HTML_HELP_CLOSE(),
                        "50");
    addInParameter<int>("local iterations",
                        HTML_HELP_OPEN()
                        HTML_HELP_DEF( "type", "int" )
                        HTML_HELP_BODY()
                        "The number of local iterations."
                        HTML_HELP_CLOSE(),
                        "50");
    addInParameter<bool>("radial",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "bool" )
                         HTML_HELP_BODY()
                         "If set to true, radial constraints are added."
                         HTML_HELP_CLOSE(),
                         "false");
    addInParameter<bool>("upward",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "bool" )
                         HTML_HELP_BODY()
                         "If set to true, upward constraints are added. "
                         HTML_HELP_CLOSE(),
                         "false");
  }
  ~OGDFStressMajorization() {}

  void beforeCall() {
    ogdf::StressMajorization *stressm = static_cast<ogdf::StressMajorization*>(ogdfLayoutAlgo);

    if (dataSet != NULL) {
      double dval = 0;
      int ival = 0;
      bool bval = false;

      if (dataSet->get("iterations", ival))
        stressm->setIterations(ival);

      if (dataSet->get("stop tolerance", dval))
        stressm->setStopTolerance(dval);

      if (dataSet->get("use layout", bval))
        stressm->setUseLayout(bval);

      if (dataSet->get("compute max iterations", bval))
        stressm->computeMaxIterations(bval);

      if (dataSet->get("global iterations", ival))
        stressm->setMaxGlobalIterations(ival);

      if (dataSet->get("local iterations", ival))
        stressm->setMaxLocalIterations(ival);

      if (dataSet->get("radial", bval))
        stressm->radial(bval);

      if (dataSet->get("upward", bval))
        stressm->upward(bval);
    }
  }

};


PLUGIN(OGDFStressMajorization)
