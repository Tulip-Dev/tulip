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
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/planarity/EmbedderMaxFace.h>
#include <ogdf/planarity/EmbedderMaxFaceLayers.h>
#include <ogdf/planarity/EmbedderMinDepth.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFace.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>
#include <ogdf/planarity/EmbedderMinDepthPiTa.h>
#include <ogdf/planarity/EmbedderOptimalFlexDraw.h>
#include <ogdf/planarity/SimpleEmbedder.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

#include <tulip/StringCollection.h>

// comments below have been extracted from OGDF/src/planarity/PlanarizationLayout.cpp
/** \addtogroup layout */

/**
 * Declaration of class PlanarizationLayout.
 *
 * \author Carsten Gutwenger
 *
 * \par License:
 * This file is part of the Open Graph Drawing Framework (OGDF).
 *
 * \par
 * Copyright (C)<br>
 * See README.txt in the root directory of the OGDF installation for details.
 *
 * \par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 or 3 as published by the Free Software Foundation;
 * see the file LICENSE.txt included in the packaging of this file
 * for details.
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

#define ELT_EMBEDDER "Embedder"
#define ELT_EMBEDDER_LIST                                                                          \
  "SimpleEmbedder;EmbedderMaxFace;EmbedderMaxFaceLayers;EmbedderMinDepth;EmbedderMinDepthMaxFace;" \
  "EmbedderMinDepthMaxFaceLayers;EmbedderMinDepthPiTa" //;EmbedderOptimalFlexDraw"
#define ELT_EMBEDDER_SIMPLE 0
#define ELT_EMBEDDER_MAXFACE 1
#define ELT_EMBEDDER_MAXFACELAYERS 2
#define ELT_EMBEDDER_MINDEPTH 3
#define ELT_EMBEDDER_MINDEPTHMAXFACE 4
#define ELT_EMBEDDER_MINDEPTHMAXFACELAYERS 5
#define ELT_EMBEDDER_MINDEPTHPITA 6
#define ELT_EMBEDDER_OPTIMALFLEXDRAW 7

static const char *embedderValuesDescription =
    "SimpleEmbedder <i>(Planar graph embedding from the algorithm of Boyer and Myrvold)</i><br>"
    "EmbedderMaxFace <i>(Planar graph embedding with maximum external face)</i><br>"
    "EmbedderMaxFaceLayers <i>(Planar graph embedding with maximum external face, plus layers "
    "approach)</i><br>"
    "EmbedderMinDepth <i>(Planar graph embedding with minimum block-nesting depth)</i><br>"
    "EmbedderMinDepthMaxFace <i>(Planar graph embedding with minimum block-nesting depth and "
    "maximum external face)</i><br>"
    "EmbedderMinDepthMaxFaceLayers <i>(Planar graph embedding with minimum block-nesting depth and "
    "maximum external face, plus layers approach)</i><br>"
    "EmbedderMinDepthPiTa <i>(Planar graph embedding with minimum block-nesting depth for given "
    "embedded blocks)</i>"
    //"EmbedderOptimalFlexDraw <i>(Planar graph embedding with minimum cost)</i>"
    ;

static const char *paramHelp[] = {
    // page ratio
    "Sets the option page ratio.",

    // Embedder
    "The result of the crossing minimization step is a planar graph, in which crossings are "
    "replaced by dummy nodes. The embedder then computes a planar embedding of this planar graph."};

class OGDFPlanarizationLayout : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Planarization Layout (OGDF)", "Carsten Gutwenger", "12/11/2007",
                    "The planarization approach for drawing graphs.", "1.0", "Planar")
  OGDFPlanarizationLayout(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ogdf::PlanarizationLayout()) {
    addInParameter<double>("page ratio", paramHelp[0], "1.1");
    addInParameter<StringCollection>(ELT_EMBEDDER, paramHelp[1], ELT_EMBEDDER_LIST, true,
                                     embedderValuesDescription);
  }

  ~OGDFPlanarizationLayout() override {}

  void beforeCall() override {
    ogdf::PlanarizationLayout *pl = static_cast<ogdf::PlanarizationLayout *>(ogdfLayoutAlgo);

    if (dataSet != nullptr) {
      double dval = 0;
      StringCollection sc;

      if (dataSet->get("page ratio", dval))
        pl->pageRatio(dval);

      if (dataSet->get(ELT_EMBEDDER, sc)) {
        if (sc.getCurrent() == ELT_EMBEDDER_MAXFACE) {
          pl->setEmbedder(new ogdf::EmbedderMaxFace());
        } else if (sc.getCurrent() == ELT_EMBEDDER_MAXFACELAYERS) {
          pl->setEmbedder(new ogdf::EmbedderMaxFaceLayers());
        } else if (sc.getCurrent() == ELT_EMBEDDER_MINDEPTH) {
          pl->setEmbedder(new ogdf::EmbedderMinDepth());
        } else if (sc.getCurrent() == ELT_EMBEDDER_MINDEPTHMAXFACE) {
          pl->setEmbedder(new ogdf::EmbedderMinDepthMaxFace());
        } else if (sc.getCurrent() == ELT_EMBEDDER_MINDEPTHMAXFACELAYERS) {
          pl->setEmbedder(new ogdf::EmbedderMinDepthMaxFaceLayers());
        } else if (sc.getCurrent() == ELT_EMBEDDER_MINDEPTHPITA) {
          pl->setEmbedder(new ogdf::EmbedderMinDepthPiTa());
          // EmbedderOptimalFlexDraw embedder segfaults every time, so disable it for the moment
          //        } else if (sc.getCurrent() == ELT_EMBEDDER_OPTIMALFLEXDRAW) {
          //          pl->setEmbedder(new ogdf::EmbedderOptimalFlexDraw());

        } else {
          pl->setEmbedder(new ogdf::SimpleEmbedder());
        }
      }
    }
  }
};

PLUGIN(OGDFPlanarizationLayout)
