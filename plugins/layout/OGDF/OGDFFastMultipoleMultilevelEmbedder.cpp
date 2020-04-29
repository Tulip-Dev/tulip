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

#include <tulip/ConnectedTest.h>

#include <tulip2ogdf/OGDFLayoutPluginBase.h>

using namespace tlp;
using namespace ogdf;

static const char *paramHelp[] = {
    // number of threads
    "The number of threads to use during the computation of the layout.",

    // multilevel nodes bound
    "The bound for the number of nodes in a multilevel step."};

class OGDFFastMultipoleMultiLevelEmbedder : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Fast Multipole Multilevel Embedder (OGDF)", "Martin Gronemann", "12/11/2007",
                    "The FMME layout algorithm is a variant of multilevel, force-directed layout, "
                    "which utilizes various tools to speed up the computation.",
                    "1.1", "Multilevel")
  OGDFFastMultipoleMultiLevelEmbedder(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ogdf::ComponentSplitterLayout()),
        fmme(new ogdf::FastMultipoleMultilevelEmbedder()) {
    addInParameter<int>("number of threads", paramHelp[0], "2");
    addInParameter<int>("multilevel nodes bound", paramHelp[1], "10");

    ogdf::ComponentSplitterLayout *csl =
        static_cast<ogdf::ComponentSplitterLayout *>(ogdfLayoutAlgo);
    // ComponentSplitterLayout takes ownership of the FastMultipoleMultilevelEmbedder instance
    csl->setLayoutModule(fmme);
  }

  ~OGDFFastMultipoleMultiLevelEmbedder() override {}

  void beforeCall() override {

    if (dataSet != nullptr) {
      int ival = 0;

      if (dataSet->get("number of threads", ival))
        fmme->maxNumThreads(ival);

      if (dataSet->get("multilevel nodes bound", ival))
        fmme->multilevelUntilNumNodesAreLess(ival);
    }

    // ensure the input graph is simple as the layout failed in non multi-threaded mode otherwise
    //  ogdf::makeSimple(tlpToOGDF->getOGDFGraph());
  }

private:
  ogdf::FastMultipoleMultilevelEmbedder *fmme;
};

PLUGIN(OGDFFastMultipoleMultiLevelEmbedder)
