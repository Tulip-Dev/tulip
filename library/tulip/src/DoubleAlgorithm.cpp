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
#include <tulip/DoubleAlgorithm.h>
#include <tulip/DoubleProperty.h>

static const char * paramHelp[] = {
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "DoubleProperty" ) \
  HTML_HELP_DEF( "default", "\"viewMetric\"" ) \
  HTML_HELP_BODY() \
  "This parameter indicates the property to compute." \
  HTML_HELP_CLOSE(),
};

tlp::DoubleAlgorithm::DoubleAlgorithm (const tlp::AlgorithmContext &context):tlp::Algorithm(context), doubleResult(NULL) {
  addOutParameter<tlp::DoubleProperty>("result", paramHelp[0],
                                       "viewMetric");

  if (dataSet != NULL)
    dataSet->get("result", doubleResult);
}

tlp::DoubleAlgorithm::~DoubleAlgorithm() {}
