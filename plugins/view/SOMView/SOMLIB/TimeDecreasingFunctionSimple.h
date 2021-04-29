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

#ifndef TIMEDECREASINGFUNCTIONSIMPLE_H_
#define TIMEDECREASINGFUNCTIONSIMPLE_H_

#include "TimeDecreasingFunction.h"

class TimeDecreasingFunctionSimple : public TimeDecreasingFunction {
public:
  TimeDecreasingFunctionSimple(double initialCoef);
  ~TimeDecreasingFunctionSimple() override;
  double computeCurrentTimeRate(unsigned int currentIteration, unsigned int maxIteration,
                                unsigned int inputSampleSize) override;

  double getInitialCoefficient() {
    return initialCoef;
  }

  void setInitialCoefficient(double coef) {
    initialCoef = coef;
  }

protected:
  double initialCoef;
};

#endif /* TIMEDECREASINGFUNCTIONSIMPLE_H_ */
