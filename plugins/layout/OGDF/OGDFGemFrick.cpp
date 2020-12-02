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

#include <ogdf/energybased/GEMLayout.h>

#include <tulip2ogdf/OGDFLayoutPluginBase.h>

#include <tulip/StringCollection.h>

#define ELT_ATTRACTIONFORMULA "Attraction formula"
#define ELT_ATTRACTIONFORMULALIST "Fruchterman/Reingold;GEM"

static const char *paramHelp[] = {
    // number of rounds
    "The maximal number of rounds per node.",

    // minimal temperature
    "The minimal temperature.",

    // initial temperature
    "The initial temperature to x; must be >= minimalTemperature.",

    // gravitational constant
    "Gravitational constant parameter.",

    // desired length
    "The desired edge length to x; must be >= 0.",

    // maximal disturbance
    "The maximal disturbance to x; must be >= 0.",

    // rotation angle
    "The opening angle for rotations to x (0 <= x <= pi / 2).",

    // oscillation angle
    "Sets the opening angle for oscillations to x (0 <= x <= pi / 2).",

    // rotation sensitivity
    "The rotation sensitivity to x (0 <= x <= 1).",

    // oscillation sensitivity
    "The oscillation sensitivity to x (0 <= x <= 1).",

    // Attraction formula
    "The formula for attraction.",

    // minDistCC
    "The minimal distance between connected components.",

    // pageRatio
    "The page ratio used for packing connected components."};

using namespace tlp;

class OGDFGemFrick : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION(
      "GEM Frick (OGDF)", "Christoph Buchheim", "15/11/2007",
      "OGDF implementation of the GEM-2d layout algorithm first published as:<br/>"
      " <b>A fast, adaptive layout algorithm for undirected graphs</b>, A. Frick, A. "
      "Ludwig, and H. Mehldau, Graph Drawing'94, Volume 894 of Lecture Notes in "
      "Computer Science (1995), doi: <a "
      "href=\"https://doi.org/10.1007/3-540-58950-3_393\">10.1007/3-540-58950-3_393</a>",
      "1.1", "Force Directed")
  OGDFGemFrick(const tlp::PluginContext *context);
  ~OGDFGemFrick() override;

  void beforeCall() override;
};

OGDFGemFrick::OGDFGemFrick(const tlp::PluginContext *context)
    : OGDFLayoutPluginBase(context, context ? new ogdf::GEMLayout() : nullptr) {
  addInParameter<int>("number of rounds", paramHelp[0], "30000");
  addInParameter<double>("minimal temperature", paramHelp[1], "0.005");
  addInParameter<double>("initial temperature", paramHelp[2], "12.0");
  addInParameter<double>("gravitational constant", paramHelp[3], "0.0625");
  addInParameter<double>("desired length", paramHelp[4], "5.0");
  addInParameter<double>("maximal disturbance", paramHelp[5], "0.0");
  addInParameter<double>("rotation angle", paramHelp[6], "1.04719755");
  addInParameter<double>("oscillation angle", paramHelp[7], "1.57079633");
  addInParameter<double>("rotation sensitivity", paramHelp[8], "0.01");
  addInParameter<double>("oscillation sensitivity", paramHelp[9], "0.3");
  addInParameter<StringCollection>(ELT_ATTRACTIONFORMULA, paramHelp[10], ELT_ATTRACTIONFORMULALIST,
                                   true, "Fruchterman/Reingold <br> GEM");
  addInParameter<double>("minDistCC", paramHelp[11], "20");
  addInParameter<double>("pageRatio", paramHelp[12], "1.0");
}

OGDFGemFrick::~OGDFGemFrick() {}

void OGDFGemFrick::beforeCall() {
  ogdf::GEMLayout *gem = static_cast<ogdf::GEMLayout *>(ogdfLayoutAlgo);

  if (dataSet != nullptr) {
    int ival = 0;
    double dval = 0;
    StringCollection sc;

    if (dataSet->get("number of rounds", ival)) {
      gem->numberOfRounds(ival);
    }

    if (dataSet->get("minimal temperature", dval)) {
      gem->minimalTemperature(dval);
    }

    if (dataSet->get("initial temperature", dval)) {
      gem->initialTemperature(dval);
    }

    if (dataSet->get("gravitational constant", dval)) {
      gem->gravitationalConstant(dval);
    }

    if (dataSet->get("desired length", dval)) {
      gem->desiredLength(dval);
    }

    if (dataSet->get("maximal disturbance", dval)) {
      gem->maximalDisturbance(dval);
    }

    if (dataSet->get("rotation angle", dval)) {
      gem->rotationAngle(dval);
    }

    if (dataSet->get("oscillation angle", dval)) {
      gem->oscillationAngle(dval);
    }

    if (dataSet->get("rotation sensitivity", dval)) {
      gem->rotationSensitivity(dval);
    }

    if (dataSet->get("oscillation sensitivity", dval)) {
      gem->oscillationSensitivity(dval);
    }

    if (dataSet->get(ELT_ATTRACTIONFORMULA, sc)) {
      gem->attractionFormula(sc.getCurrent() + 1);
    }

    if (dataSet->get("minDistCC", dval))
      gem->minDistCC(dval);

    if (dataSet->get("pageRatio", dval))
      gem->pageRatio(dval);
  }
}

PLUGIN(OGDFGemFrick)
