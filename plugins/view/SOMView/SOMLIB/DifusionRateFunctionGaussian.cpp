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

#include "DifusionRateFunctionGaussian.h"
#include <cmath>
#include <tulip/tulipconf.h>

DifusionRateFunctionGaussian::DifusionRateFunctionGaussian(
  TimeDecreasingFunction* decreasingFunction) : DiffusionRateFunction(decreasingFunction) {
}

DifusionRateFunctionGaussian::~DifusionRateFunctionGaussian() {
}

double DifusionRateFunctionGaussian::computeSpaceRate(unsigned int distance,
    unsigned int currentIteration, unsigned int maxIteration,
    unsigned int inputSampleSize) {

  double timeVal = timeFunction->computeCurrentTimeRate(currentIteration,
                   maxIteration, inputSampleSize);
  double value = (sqrt((distance * distance))) / (2 * (timeVal * timeVal));

  return exp(-value);
}
