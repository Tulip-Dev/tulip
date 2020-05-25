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
#include <ogdf/misclayout/BalloonLayout.h>
#include <ogdf/packing/ComponentSplitterLayout.h>

#include <tulip2ogdf/OGDFLayoutPluginBase.h>

using namespace tlp;
using namespace ogdf;

static const char *paramHelp[] = {
    // Even angles
    "Subtrees may be assigned even angles or angles depending on their size."};

class OGDFBalloon : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION(
      "Balloon (OGDF)", "Karsten Klein", "13/11/2007",
      "Computes a radial (balloon) layout based on a spanning tree.<br/>The "
      "algorithm is partially based on the papers:<br/><b>On Balloon Drawings of Rooted "
      "Trees</b> by Lin and Yen<br/><b>Interacting with Huge Hierarchies: Beyond "
      "Cone Trees</b> by Carriere and Kazman. ",
      "1.4", "Hierarchical")
  OGDFBalloon(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ogdf::ComponentSplitterLayout()) {
    addInParameter<bool>("Even angles", paramHelp[0], "false", false);
  }

  void beforeCall() override {
    ogdf::ComponentSplitterLayout *csl =
        static_cast<ogdf::ComponentSplitterLayout *>(ogdfLayoutAlgo);
    // ComponentSplitterLayout takes ownership of the BalloonLayout instance
    BalloonLayout *balloon = new BalloonLayout();
    csl->setLayoutModule(balloon);

    if (dataSet != nullptr) {
      bool val = false;

      if (dataSet->get("Even angles", val))
        balloon->setEvenAngles(val);
    }
  }
};

PLUGIN(OGDFBalloon)
