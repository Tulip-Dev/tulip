/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#include <tulip/ColorAlgorithm.h>
#include <tulip/ColorProperty.h>

static const char * paramHelp[] = {
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "ColorProperty" ) \
  HTML_HELP_DEF( "default", "\"viewColor\"" ) \
  HTML_HELP_BODY() \
  "This parameter indicates the property to compute." \
  HTML_HELP_CLOSE(),
};

tlp::ColorAlgorithm::ColorAlgorithm (const tlp::AlgorithmContext &context):tlp::Algorithm(context), colorResult(NULL) {
  addOutParameter<tlp::ColorProperty>("result", paramHelp[0],
                                      "viewColor");

  if (dataSet != NULL)
    dataSet->get("result", colorResult);
}

tlp::ColorAlgorithm::~ColorAlgorithm() {
}
