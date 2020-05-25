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
#include <ogdf/energybased/FastMultipoleEmbedder.h>
#include <ogdf/packing/ComponentSplitterLayout.h>

#include <tulip2ogdf/OGDFLayoutPluginBase.h>

using namespace tlp;
using namespace ogdf;

static const char *paramHelp[] = {
    // number of iterations
    "The maximum number of iterations.",

    // number of coefficients
    "The number of coefficients for the expansions.",

    // randomize layout
    "If true, the initial layout will be randomized.",

    // default node size
    "The default node size.",

    // default edge length
    "The default edge length.",

    // number of threads
    "The number of threads to use during the computation of the layout."

};

class OGDFFastMultipoleEmbedder : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Fast Multipole Embedder (OGDF)", "Martin Gronemann", "12/11/2007",
                    "Implements the fast multipole embedder layout algorithm of Martin Gronemann. "
                    "It uses the same repulsive forces as FM³ of Hachul and Jünger, but slightly "
                    "modified attractive forces.",
                    "1.0", "Force Directed")
  OGDFFastMultipoleEmbedder(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ogdf::ComponentSplitterLayout()) {
    addInParameter<int>("number of iterations", paramHelp[0], "100");
    addInParameter<int>("number of coefficients", paramHelp[1], "5");
    addInParameter<bool>("randomize layout", paramHelp[2], "true");
    addInParameter<double>("default node size", paramHelp[3], "20.0");
    addInParameter<double>("default edge length", paramHelp[4], "1.0");
    addInParameter<int>("number of threads", paramHelp[5], "2");
  }

  void beforeCall() override {

    ComponentSplitterLayout *csl = static_cast<ComponentSplitterLayout *>(ogdfLayoutAlgo);
    // ComponentSplitterLayout takes ownership of the FastMultipoleEmbedder instance
    FastMultipoleEmbedder *fme = new FastMultipoleEmbedder();
    csl->setLayoutModule(fme);

    if (dataSet != nullptr) {
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

    // ensure the input graph is simple as the layout failed in non multi-threaded mode otherwise
    // ogdf::makeSimple(tlpToOGDF->getOGDFGraph());
  }
};

PLUGIN(OGDFFastMultipoleEmbedder)
