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

#include <ogdf/energybased/SpringEmbedderFRExact.h>
#include "tulip2ogdf/OGDFLayoutPluginBase.h"
#include <tulip/StringCollection.h>

// comments below have been extracted from OGDF/src/energybased/SpringEmbedderFRExact.cpp
/** \addtogroup layout */

/**
 * Implementation of Spring-Embedder (Fruchterman,Reingold)
 * algorithm with exact force computations.
 *
 * \author Carsten Gutwenger
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

#define ELT_COOLING "Cooling function"
#define ELT_COOLINGLIST "Factor;Logarithmic"
#define ELT_FACTOR 0
#define ELT_LOGARITHMIC 1

static const char *paramHelp[] = {
    // iterations
    "The number of iterations.",

    // noise
    "Sets the parameter noise.",

    // use node weights
    "Indicates if the node weights have to be used.",

    // node weights
    "The metric containing node weights.",

    // Cooling function
    "Sets the parameter cooling function",

    // ideal edge length
    "The ideal edge length.",

    // minDistCC
    "The minimal distance between connected components.",

    // pageRatio
    "The page ratio used for packing connected components.",

    // check convergence
    "Indicates if the convergence has to be checked.",

    // convergence tolerance
    "The convergence tolerance parameter."};

class OGDFFrutchermanReingold : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Frutcherman Reingold (OGDF)", "Stephan Hachul", "15/11/2007",
                    "Implements the Fruchterman and Reingold layout algorithm, first published "
                    "as:<br/><b>Graph Drawing by Force-Directed Placement</b>, Fruchterman, Thomas "
                    "M. J., Reingold, Edward M., Software – Practice & Experience (Wiley) Volume "
                    "21, Issue 11, pages 1129–1164, (1991)",
                    "1.1", "Force Directed")
  OGDFFrutchermanReingold(const tlp::PluginContext *context);
  ~OGDFFrutchermanReingold();

  void beforeCall();
};

/*Nom de la classe, Nom du plugins, nom de l'auteur,date de
 creation,information, realease, groupe*/
PLUGIN(OGDFFrutchermanReingold)

OGDFFrutchermanReingold::OGDFFrutchermanReingold(const tlp::PluginContext *context)
    : OGDFLayoutPluginBase(context, new ogdf::SpringEmbedderFRExact()) {
  addInParameter<int>("iterations", paramHelp[0], "1000");
  addInParameter<bool>("noise", paramHelp[1], "true");
  addInParameter<bool>("use node weights", paramHelp[2], "false");
  addInParameter<NumericProperty *>("node weights", paramHelp[3], "viewMetric");
  addInParameter<StringCollection>(ELT_COOLING, paramHelp[4], ELT_COOLINGLIST, true,
                                   "Factor<br> Logarithmic");
  addInParameter<double>("ideal edge length", paramHelp[5], "10.0");
  addInParameter<double>("minDistCC", paramHelp[6], "20.0");
  addInParameter<double>("pageRatio", paramHelp[7], "1.0");
  addInParameter<bool>("check convergence", paramHelp[8], "true");
  addInParameter<double>("convergence tolerance", paramHelp[9], "0.01");
}

OGDFFrutchermanReingold::~OGDFFrutchermanReingold() {}

void OGDFFrutchermanReingold::beforeCall() {
  ogdf::SpringEmbedderFRExact *sefr = static_cast<ogdf::SpringEmbedderFRExact *>(ogdfLayoutAlgo);

  if (dataSet != NULL) {
    int ival = 0;
    double dval = 0;
    bool bval = false;
    StringCollection sc;

    if (dataSet->get("iterations", ival))
      sefr->iterations(ival);

    if (dataSet->get("noise", bval))
      sefr->noise(bval);

    if (dataSet->get("minDistCC", dval))
      sefr->minDistCC(dval);

    if (dataSet->get("pageRatio", dval))
      sefr->pageRatio(dval);

    if (dataSet->get(ELT_COOLING, sc)) {
      if (sc.getCurrent() == ELT_FACTOR) {
        sefr->coolingFunction(ogdf::SpringEmbedderFRExact::cfFactor);
      } else {
        sefr->coolingFunction(ogdf::SpringEmbedderFRExact::cfLogarithmic);
      }
    }

    if (dataSet->get("use node weights", bval)) {
      sefr->nodeWeights(bval);

      NumericProperty *metric = NULL;

      if (bval && dataSet->get("node weights", metric)) {
        tlpToOGDF->copyTlpNumericPropertyToOGDFNodeWeight(metric);
      }
    }

    if (dataSet->get("check convergence", bval))
      sefr->checkConvergence(bval);

    if (dataSet->get("convergence tolerance", dval))
      sefr->convTolerance(dval);
  }
}
