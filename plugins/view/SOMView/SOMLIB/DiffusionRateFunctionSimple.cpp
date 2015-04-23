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

#include "DiffusionRateFunctionSimple.h"
DiffusionRateFunctionSimple::DiffusionRateFunctionSimple(
  TimeDecreasingFunction* timeFunction, unsigned int neighborhood) :
  DiffusionRateFunction(timeFunction), neighborhoodMax(neighborhood) {

}

DiffusionRateFunctionSimple::~DiffusionRateFunctionSimple() {
  // TODO Auto-generated destructor stub
}

double DiffusionRateFunctionSimple::computeSpaceRate(unsigned int distance,
    unsigned int currentIteration, unsigned int maxIteration,
    unsigned int inputSampleSize) {

  if (distance <= neighborhoodMax)
    return 1 * timeFunction->computeCurrentTimeRate(currentIteration,
           maxIteration, inputSampleSize);
  else
    return 0;
}

