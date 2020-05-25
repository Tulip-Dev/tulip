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
#include <ogdf/packing/ComponentSplitterLayout.h>

#include <tulip2ogdf/OGDFLayoutPluginBase.h>

using namespace tlp;
using namespace ogdf;

class SameLayout : public ogdf::LayoutModule {

public:
  void call(GraphAttributes &GA) override {}
};

class OGDFTileToRowsPacking : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Tile To Rows Packing (OGDF)", "Carsten Gutwenger", "12/11/2007",
                    "The tile-to-rows algorithm for packing drawings of connected components.",
                    "1.0", "Misc")
  OGDFTileToRowsPacking(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ogdf::ComponentSplitterLayout()) {}

  void beforeCall() override {
    ogdf::ComponentSplitterLayout *csl =
        static_cast<ogdf::ComponentSplitterLayout *>(ogdfLayoutAlgo);
    // ComponentSplitterLayout takes ownership of the SameLayout instance
    csl->setLayoutModule(new SameLayout());
  }
};

PLUGIN(OGDFTileToRowsPacking)
