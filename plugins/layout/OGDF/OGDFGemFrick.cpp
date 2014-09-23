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

#include <ogdf/energybased/GEMLayout.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

#include <tulip/StringCollection.h>

#define ELT_ATTRACTIONFORMULA "Attraction formula"
#define ELT_ATTRACTIONFORMULALIST "Fruchterman/Reingold;GEM"

// comments below have been extracted from OGDF/src/energybased/GEMLayout.cpp
/** \addtogroup layout */

/// An implementations of the GEM Layout.
/**
 * Fast force-directed layout algorithm (GEMLayout) based on Frick et al.'s algorithm
 *
 * \author Christoph Buchheim
 *
 * \par License:
 * This is part of the Open Graph Drawing Framework (OGDF).
 * Copyright (C) 2005-2007
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
class OGDFGemFrick : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("GEM Frick (OGDF)","Christoph Buchheim", "15/11/2007","Alpha","1.1", "Force Directed")
  OGDFGemFrick(const tlp::PluginContext* context);
  ~OGDFGemFrick();

  void beforeCall();

};


PLUGIN(OGDFGemFrick)

OGDFGemFrick::OGDFGemFrick(const tlp::PluginContext* context) : OGDFLayoutPluginBase(context,new ogdf::GEMLayout()) {
  addInParameter<int>("number of rounds",
                      HTML_HELP_OPEN()
                      HTML_HELP_DEF( "type", "int" )
                      HTML_HELP_BODY()
                      "The maximal number of rounds per node."
                      HTML_HELP_CLOSE(),
                      "30000");
  addInParameter<double>("minimal temperature",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "double" )
                         HTML_HELP_BODY()
                         "the minimal temperature ."
                         HTML_HELP_CLOSE(),
                         "0.005");
  addInParameter<double>("initial temperature",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "bool" )
                         HTML_HELP_BODY()
                         "The initial temperature to x; must be >= minimalTemperature."
                         HTML_HELP_CLOSE(),
                         "12.0");
  addInParameter<double>("gravitational constant",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "double" )
                         HTML_HELP_BODY()
                         "Gravitational constant parameter."
                         HTML_HELP_CLOSE(),
                         "0.0625");
  addInParameter<double>("desired length",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "double" )
                         HTML_HELP_BODY()
                         "The desired edge length to x; must be >= 0."
                         HTML_HELP_CLOSE(),
                         "5.0");
  addInParameter<double>("maximal disturbance",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "double" )
                         HTML_HELP_BODY()
                         "The maximal disturbance to x; must be >= 0."
                         HTML_HELP_CLOSE(),
                         "0.0");
  addInParameter<double>("rotation angle",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "double" )
                         HTML_HELP_BODY()
                         "The opening angle for rotations to x (0 <= x <= pi / 2)."
                         HTML_HELP_CLOSE(),
                         "1.04719755");
  addInParameter<double>("oscillation angle",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "double" )
                         HTML_HELP_BODY()
                         "Sets the opening angle for oscillations to x (0 <= x <= pi / 2)."
                         HTML_HELP_CLOSE(),
                         "1.57079633");
  addInParameter<double>("rotation sensitivity",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "double" )
                         HTML_HELP_BODY()
                         "The rotation sensitivity to x (0 <= x <= 1)."
                         HTML_HELP_CLOSE(),
                         "0.01");
  addInParameter<double>("oscillation sensitivity",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "double" )
                         HTML_HELP_BODY()
                         "The oscillation sensitivity to x (0 <= x <= 1)."
                         HTML_HELP_CLOSE(),
                         "0.3");
  addInParameter<StringCollection>(ELT_ATTRACTIONFORMULA,
                                   HTML_HELP_OPEN()
                                   HTML_HELP_DEF( "type", "StringCollection")
                                   HTML_HELP_DEF("values", "- Fruchterman/Reingold<br/>- GEM")
                                   HTML_HELP_DEF( "default", "Fruchterman/Reingold" )
                                   HTML_HELP_BODY()
                                   "The formula for attraction. "
                                   HTML_HELP_CLOSE(),
                                   ELT_ATTRACTIONFORMULALIST);
  addInParameter<double>("minDistCC",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "double" )
                         HTML_HELP_BODY()
                         "The minimal distance between connected components."
                         HTML_HELP_CLOSE(),
                         "20");
  addInParameter<double>("pageRatio",
                         HTML_HELP_OPEN()
                         HTML_HELP_DEF( "type", "double" )
                         HTML_HELP_BODY()
                         "The page ratio used for packing connected components."
                         HTML_HELP_CLOSE(),
                         "1.0");
}

OGDFGemFrick::~OGDFGemFrick() {

}

void OGDFGemFrick::beforeCall() {
  ogdf::GEMLayout *gem = static_cast<ogdf::GEMLayout*>(ogdfLayoutAlgo);

  if (dataSet != NULL) {
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

