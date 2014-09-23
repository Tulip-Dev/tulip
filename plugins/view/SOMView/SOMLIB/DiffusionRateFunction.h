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

#ifndef NEIGHBORHOODDECREASINGFUNCTION_H_
#define NEIGHBORHOODDECREASINGFUNCTION_H_

#include "TimeDecreasingFunction.h"

/**
 * @brief Interface for distance rate computation functions.
 * Interface for distance rate computation functions. Distance rate is used in learning rate computation to handle the distance factor.
 * These objects take a TimeDecreasingFunction as the diffusion coefficient evolve with distance but also with time.
 * See SOM algorithm for more details on time coefficient in SOM computation.
 */
class DiffusionRateFunction {
public:
  DiffusionRateFunction(TimeDecreasingFunction* decreasingFunction);
  virtual ~DiffusionRateFunction();
  /**
   * Compute the diffusion rate in function of the current time and the distance.
   * @param distance The distance between the BMU and the node. This distance is the number of node to go through from BMU to current node.
   * @param currentIteration The current iteration number.
   * @param maxIteration The maximum iteration number.
   * @param inputSampleSize The sample size.
   * @return the diffusion rate.
   */
  virtual double computeSpaceRate(unsigned int distance,
                                  unsigned int currentIteration, unsigned int maxIteration,
                                  unsigned int inputSampleSize) = 0;
  TimeDecreasingFunction* getTimeDecreasingFunction();
  void setTimeDecreasingFunction(TimeDecreasingFunction* timeFunction);
protected:
  TimeDecreasingFunction *timeFunction;
};

#endif /* NEIGHBORHOODDECREASINGFUNCTION_H_ */
