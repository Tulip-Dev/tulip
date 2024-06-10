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
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/planarity/EmbedderMaxFace.h>
#include <ogdf/planarity/EmbedderMaxFaceLayers.h>
#include <ogdf/planarity/EmbedderMinDepth.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFace.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>
#include <ogdf/planarity/EmbedderMinDepthPiTa.h>
#include <ogdf/planarity/EmbedderOptimalFlexDraw.h>
#include <ogdf/planarity/SimpleEmbedder.h>

#include <tulip2ogdf/OGDFLayoutPluginBase.h>

#include <tulip/StringCollection.h>

#define EMBEDDER "Embedder"
#define EMBEDDER_LIST                                                                              \
  "simple;max face;max face layers;min depth;min depth max face;"                                  \
  "min depth max face layers;min depth PiTa;optimal FlexDraw"
#define EMBEDDER_MAXFACE 1
#define EMBEDDER_MAXFACELAYERS 2
#define EMBEDDER_MINDEPTH 3
#define EMBEDDER_MINDEPTHMAXFACE 4
#define EMBEDDER_MINDEPTHMAXFACELAYERS 5
#define EMBEDDER_MINDEPTHPITA 6
#define EMBEDDER_OPTIMALFLEXDRAW 7

using namespace tlp;
using namespace ogdf;

static const char *embedderValuesDescription =
    "simple <i>(embedding from the algorithm of Boyer and Myrvold)</i><br>"
    "max face <i>(embedding with maximum external face)</i><br>"
    "max face layers <i>(embedding with maximum external face, plus layers "
    "approach)</i><br>"
    "min depth <i>(embedding with minimum block-nesting depth)</i><br>"
    "min depth max face <i>(embedding with minimum block-nesting depth and "
    "maximum external face)</i><br>"
    "min depth max face layers <i>(embedding with minimum block-nesting depth and "
    "maximum external face, plus layers approach)</i><br>"
    "min depth PiTa <i>(embedding with minimum block-nesting depth for given "
    "embedded blocks)</i>"
    "optimal FlexDraw <i>(Planar graph embedding with minimum cost)</i>";

static const char *paramHelp[] = {
    // page ratio
    "Sets the option page ratio.",

    // min clique size
    "If preprocessing of cliques is considered, this option determines the minimum size of cliques "
    "to search for",

    // Embedder
    "The result of the crossing minimization step is a planar graph, in which crossings are "
    "replaced by dummy nodes. The embedder then computes a planar embedding of this planar graph.",

    // crossings
    "Returns the number of crossings in the computed layout."};

class OGDFPlanarizationLayout : public OGDFLayoutPluginBase {

  ogdf::PlanarizationLayout *pl;

public:
  PLUGININFORMATION("Planarization Layout (OGDF)", "Carsten Gutwenger", "12/11/2007",
                    "The planarization approach for drawing graphs.", "1.1", "Planar")
  OGDFPlanarizationLayout(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, context ? new ogdf::PlanarizationLayout() : nullptr),
        pl(static_cast<ogdf::PlanarizationLayout *>(ogdfLayoutAlgo)) {
    addInParameter<double>("page ratio", paramHelp[0], "1.1");
    addInParameter<int>("min clique size", paramHelp[1], "3");
    addInParameter<StringCollection>("embedder", paramHelp[2], EMBEDDER_LIST, true,
                                     embedderValuesDescription);
    addOutParameter<int>("number of crossings", paramHelp[3]);
  }

  ~OGDFPlanarizationLayout() override {}

  void beforeCall() override {
    if (dataSet != nullptr) {
      double dval = 0;
      int clique_size = 3;
      StringCollection sc;

      if (dataSet->get("page ratio", dval))
        pl->pageRatio(dval);

      if (dataSet->get("min clique size", clique_size))
        pl->minCliqueSize(clique_size);

      if (dataSet->get("embedder", sc)) {
        switch (sc.getCurrent()) {
        case EMBEDDER_MAXFACE:
          pl->setEmbedder(new ogdf::EmbedderMaxFace());
          break;
        case EMBEDDER_MAXFACELAYERS:
          pl->setEmbedder(new ogdf::EmbedderMaxFaceLayers());
          break;
        case EMBEDDER_MINDEPTH:
          pl->setEmbedder(new ogdf::EmbedderMinDepth());
          break;
        case EMBEDDER_MINDEPTHMAXFACE:
          pl->setEmbedder(new ogdf::EmbedderMinDepthMaxFace());
          break;
        case EMBEDDER_MINDEPTHMAXFACELAYERS:
          pl->setEmbedder(new ogdf::EmbedderMinDepthMaxFaceLayers());
          break;
        case EMBEDDER_MINDEPTHPITA:
          pl->setEmbedder(new ogdf::EmbedderMinDepthPiTa());
          break;
        case EMBEDDER_OPTIMALFLEXDRAW:
          pl->setEmbedder(new ogdf::EmbedderOptimalFlexDraw());
          break;
        default:
          pl->setEmbedder(new ogdf::SimpleEmbedder());
        }
      }
    }
  }

  void afterCall() override {
    dataSet->set("number of crossings", pl->numberOfCrossings());
  }
};

PLUGIN(OGDFPlanarizationLayout)
