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

#ifndef SOMALGORITHM_H_
#define SOMALGORITHM_H_

#include <vector>
#include <tulip/Graph.h>

#include "DynamicVector.h"

#include "DiffusionRateFunctionSimple.h"
#include "TimeDecreasingFunctionSimple.h"


namespace tlp {
class SOMMap;
class InputSample;

/**
 * @brief Class used to perform all the operations needed to compute SOM.
 */
class SOMAlgorithm {
public:
  /**
   * Build the SOMAlgorithm object used to compute Self Organizing Map with the given time and space function.
   * @param learningRateFunction The function used to compute the learning rate in function of time.
   * @param diffusionRateFunction The function used to compute the distance rate in function of time.
   */
  SOMAlgorithm(TimeDecreasingFunction* learningRateFunction = NULL,
               DiffusionRateFunction* diffusionRateFunction = NULL);

  virtual ~SOMAlgorithm();

  TimeDecreasingFunction* getLearningRateFunction() {
    return learningRateFunction;
  }
  void setLearningRateFunction(TimeDecreasingFunction* function) {
    learningRateFunction = function;
  }
  DiffusionRateFunction* getDiffusionRateFunction() {
    return diffusionRateFunction;
  }
  void setDiffusionRateFunction(DiffusionRateFunction* function) {
    diffusionRateFunction = function;
  }

  /**
   * Init the given SOM with the given sample. At this time set all the weight node of the map with a weight take randomly in the input sample.
   * @param map The SOM
   * @param inputSample The input samples.
   * @param pluginProgress The plugin progress.
   */
  void initMap(SOMMap* map, InputSample& inputSample,
               tlp::PluginProgress *pluginProgress = NULL);

  /**
   * Train the SOM with the given sample. Use numberOfIteration time the entire inputSample to train the SOM. See the train function for more details.
   * @param map The SOM.
   * @param inputSample The input Sample.
   * @param numberOfIteration The number of time that the input sample will be used to train the map.
   * @param pluginProgress
   */
  void trainNInputSample(SOMMap* map, InputSample& inputSample,
                         unsigned int numberOfIteration, tlp::PluginProgress *pluginProgress =
                           NULL);

  /**
   * Train the SOM with the given sample (update value of each SOM node's weight with the input sample see SOM algorithm for more details.). Training process has this order
   * while currentNumber of iteration < maxIteration :
   *    Take a random vector in the input sample.
   *    Find the node with the less euclidian distance in the SOM.
   *    Propagate modification on the map.
   *
   * @param map The SOM
   * @param inputSample The input sample
   * @param maxIteration The number of iteration
   * @param pluginProgress
   */
  void train(SOMMap* map, InputSample& inputSample, unsigned int maxIteration,
             tlp::PluginProgress *pluginProgress = NULL);

  /**
   * Return a node with the smallest euclidian distance between its weight vector and the given input vector. If there is one or more node with the smallest distance choose one randomly.
   * @param map The SOM.
   * @param input The input vector.
   * @param dist The euclidian distance between input vector and selcted node weight.
   * @return
   */
  node findBMU(SOMMap* map, const DynamicVector<double>& input, double &dist);

  /**
   * Propagate modification on the SOM. Compute the learning coefficent in function of the current iteration and the distance between the first updated node and the modified one.
   * Propagate modification while learning rate is not null.
   * @param map The SOM.
   * @param input The input vector used to modify SOM map nodes.
   * @param bmu The BMU node. It's the start point for covering SOM.
   * @param currentTime The current iteration.
   * @param maxTime The maximum iteration number.
   * @param sampleSize The number of sample.
   */
  void propagateModification(SOMMap* map, const DynamicVector<double>& input,
                             node bmu, unsigned int currentTime, unsigned int maxTime,
                             unsigned int sampleSize);

  /**
   * Function use to automatically perform initialisation and training on a SOM with a given sample of vector.
   * @param map The SOM.
   * @param inputSample
   * @param nTimes The number of time that the input sample will be used to train the map.
   * @param pluginProgress
   */
  void run(SOMMap* map, InputSample& inputSample, unsigned int nTimes,
           tlp::PluginProgress *pluginProgress = NULL);

  /**
   * Perform the mapping operation of each node of the input sample on the center tab. At the end each vector of the input sample will be associated with a SOM node.
   * @param map The SOM
   * @param inputSample
   * @param mappingTab The tab used to perform the mapping between input nodes and SOM nodes. For each node in the map correspond a table of linked input node.
   * @param medDist The medium distance of the mapping.
   * @param maxElement The maximum number of element linked with a SOM node.
   */
  void computeMapping(SOMMap* map, InputSample& inputSample, std::map<
                      tlp::node, std::set<tlp::node> >& mappingTab, double& medDist,
                      unsigned int& maxElement);

protected:

  TimeDecreasingFunction* learningRateFunction;
  DiffusionRateFunction* diffusionRateFunction;

};
}
#endif /* SOMALGORITHM_H_ */
