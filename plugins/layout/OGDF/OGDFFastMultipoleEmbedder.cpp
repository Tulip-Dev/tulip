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
#include <ogdf/energybased/FastMultipoleEmbedder.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

// comments below have been extracted from OGDF/src/energybased/FastMultipoleEmbedder.cpp
/** \addtogroup layout */

/**
 * Implementation of class FastMultipoleEmbedder.
 *
 * \author Martin Gronemann
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
class OGDFFastMultipoleEmbedder : public OGDFLayoutPluginBase {

public:

  PLUGININFORMATION("Fast Multipole Embedder (OGDF)","Martin Gronemann","12/11/2007","Ok","1.0","Force Directed")
  OGDFFastMultipoleEmbedder(const tlp::PluginContext* context) :OGDFLayoutPluginBase(context, new ogdf::FastMultipoleEmbedder()) {
    addInParameter<int>("number of iterations",
                        HTML_HELP_OPEN()
                        HTML_HELP_DEF( "type", "int" )
                        HTML_HELP_BODY()
                        "The maximum number of iterations."
                        HTML_HELP_CLOSE(),
                        "100");
    addInParameter<int>("number of coefficients",
                        HTML_HELP_OPEN()
                        HTML_HELP_DEF( "type", "int" )
                        HTML_HELP_BODY()
                        "The number of coefficients for the expansions."
                        HTML_HELP_CLOSE(),
                        "5");
    addInParameter<bool>("randomize layout",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "bool" )
                         HTML_HELP_BODY()
                         "If true, the initial layout will be randomized."
                         HTML_HELP_CLOSE(),
                         "true");
    addInParameter<double>("default node size",
                           HTML_HELP_OPEN()
                           HTML_HELP_DEF( "type", "double" )
                           HTML_HELP_BODY()
                           "The default node size."
                           HTML_HELP_CLOSE(),
                           "20.0");
    addInParameter<double>("default edge length",
                           HTML_HELP_OPEN()
                           HTML_HELP_DEF( "type", "double" )
                           HTML_HELP_BODY()
                           "The default edge length. "
                           HTML_HELP_CLOSE(),
                           "40.0");
    addInParameter<int>("number of threads",
                        HTML_HELP_OPEN()
                        HTML_HELP_DEF( "type", "int" )
                        HTML_HELP_BODY()
                        "The number of threads to use during the computation of the layout."
                        HTML_HELP_CLOSE(),
                        "3");
  }

  ~OGDFFastMultipoleEmbedder() {}

  void beforeCall() {
    ogdf::FastMultipoleEmbedder *fme = static_cast<ogdf::FastMultipoleEmbedder*>(ogdfLayoutAlgo);

    if (dataSet != NULL) {
      double dval = 0;
      int ival = 0;
      bool bval = 0;

      if (dataSet->get("number of iterations", ival))
        fme->setNumIterations(ival);

      if (dataSet->get("number of coefficients", ival))
        fme->setMultipolePrec(ival);

      if (dataSet->get("number of threads", ival))
        fme->setNumberOfThreads(ival);

      if (dataSet->get("default node size", dval))
        fme->setDefaultNodeSize(dval);

      if (dataSet->get("default edge length", dval))
        fme->setDefaultEdgeLength(dval);

      if (dataSet->get("randomize layout", bval))
        fme->setRandomize(bval);
    }
  }

};


PLUGIN(OGDFFastMultipoleEmbedder)
