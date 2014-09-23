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

#include "TimeDecreasingFunctionSimple.h"

TimeDecreasingFunctionSimple::TimeDecreasingFunctionSimple(double initialCoef) :
  initialCoef(initialCoef) {
}

TimeDecreasingFunctionSimple::~TimeDecreasingFunctionSimple() {
}

double TimeDecreasingFunctionSimple::computeCurrentTimeRate(
  unsigned int currentIteration, unsigned int,
  unsigned int inputSampleSize) {

  //Learning rate
  return initialCoef / (1 + (currentIteration / inputSampleSize));
}

