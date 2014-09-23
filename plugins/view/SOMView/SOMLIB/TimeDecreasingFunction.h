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

#ifndef TIMEDECREASINGFUNCTION_H_
#define TIMEDECREASINGFUNCTION_H_

/**
 * @brief Interface for building time rate function for learning coefficient computation.
 * Interface for building time rate function for learning coefficient computation. Compute the time coefficient in function of the current interation, the max iteration and the input sample size.
 * See SOM algorithm for more details on time coefficient in SOM computation.
 */
class TimeDecreasingFunction {
public:
  TimeDecreasingFunction() {
  }
  virtual ~TimeDecreasingFunction() {
  }
  ;
  /**
   * Return the time coefficient for the given parameters.
   * @param currentIteration The current iteration.
   * @param maxIteration The maximum iteration number.
   * @param inputSampleSize The size of the input sample.
   * @return
   */
  virtual double computeCurrentTimeRate(unsigned int currentIteration,
                                        unsigned int maxIteration, unsigned int inputSampleSize)=0;
};

#endif /* TIMEDECREASINGFUNCTION_H_ */
