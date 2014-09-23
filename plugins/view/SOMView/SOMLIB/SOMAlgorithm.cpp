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

#include <ctime>

#include "SOMAlgorithm.h"
#include "SOMMap.h"
#include "InputSample.h"

#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlyphManager.h>
#include <tulip/ForEach.h>
#include <tulip/TlpTools.h>

using namespace tlp;
using namespace std;

SOMAlgorithm::SOMAlgorithm(TimeDecreasingFunction* learningRateFunction, DiffusionRateFunction* diffusionRateFunction) :

  learningRateFunction(learningRateFunction), diffusionRateFunction(diffusionRateFunction) {

  //Init default degenerescence functions if user don't do it
  if (this->learningRateFunction == NULL)
    this->learningRateFunction = new TimeDecreasingFunctionSimple(0.7);

  if (this->diffusionRateFunction == NULL)
    this->diffusionRateFunction = new DiffusionRateFunctionSimple(
      new TimeDecreasingFunctionSimple(0.7), 3);

}

SOMAlgorithm::~SOMAlgorithm() {
  delete learningRateFunction;
  delete diffusionRateFunction;

}

#define RANDNUM(max) (int)(max*(rand()/(RAND_MAX+1.0)))

void SOMAlgorithm::run(SOMMap* map, InputSample& inputSample, unsigned int nTimes,
                       tlp::PluginProgress *pluginProgress) {
  //Map initialisation
  if (pluginProgress)
    pluginProgress->setComment("Initialization");

  initMap(map, inputSample, pluginProgress);

  if (pluginProgress)
    pluginProgress->setComment("Training");

  trainNInputSample(map, inputSample, nTimes, pluginProgress);

  //registering modification
  map->registerModification(inputSample.getListenedProperties());

}

/**
 * For each node in the map use a random value
 */
void SOMAlgorithm::initMap(SOMMap* map, InputSample& inputSample,
                           tlp::PluginProgress *pluginProgress) {
  // initialize a random sequence according the given seed
  tlp::initRandomSequence();

  node n;
  int numberOfNode = map->numberOfNodes();
  int currentNumberOfNode = 0;
  Iterator<node> *nodeIterator = inputSample.getRandomNodeOrder();
  forEach(n,map->getNodes()) {
    if (!nodeIterator->hasNext()) {
      delete nodeIterator;
      nodeIterator = inputSample.getRandomNodeOrder();
    }

    //Ramdom choice
    map->setWeight(n, inputSample.getWeight(nodeIterator->next()));

    if (pluginProgress) {
      //Increase the number of iterators why?
      pluginProgress->progress(currentNumberOfNode, numberOfNode);
    }

    ++currentNumberOfNode;
  }
  delete nodeIterator;
}

void SOMAlgorithm::trainNInputSample(SOMMap* map, InputSample& inputSample, unsigned int nTimes,
                                     tlp::PluginProgress *pluginProgress) {
  train(map, inputSample, nTimes * inputSample.getSampleSize(), pluginProgress);
}

void SOMAlgorithm::train(SOMMap* map, InputSample& inputSample, unsigned int maxIteration,
                         tlp::PluginProgress *pluginProgress) {
  assert( learningRateFunction);
  assert( diffusionRateFunction);
  unsigned int currentIteration = 0;
  Iterator<node>* nodeIterator = inputSample.getRandomNodeOrder();

  while (currentIteration < maxIteration) {
    //Find BMU
    double dist;

    if (!nodeIterator->hasNext()) {
      delete nodeIterator;
      nodeIterator = inputSample.getRandomNodeOrder();
    }

    const DynamicVector<double>& currentInputVector = inputSample.getWeight(
          nodeIterator->next());

    node bmu = findBMU(map, currentInputVector, dist);

    //Environment modification
    assert(map->isElement(bmu));

    propagateModification(map, currentInputVector, bmu, currentIteration, maxIteration,
                          inputSample.getSampleSize());

    //Next Iteration
    ++currentIteration;

    if (pluginProgress)
      pluginProgress->progress(currentIteration, maxIteration);
  }

  delete nodeIterator;
}

node SOMAlgorithm::findBMU(SOMMap* map, const DynamicVector<double>& input, double &dist) {
  vector<node> matchList;
  node n;
  double bestDist;
  Iterator<node> *gridNodes = map->getNodes();
  //take the first to init the comparaison
  n = gridNodes->next();
  matchList.push_back(n);
  bestDist = input.dist(map->getWeight(n));

  while (gridNodes->hasNext()) {
    n = gridNodes->next();
    double newDist = input.dist(map->getWeight(n));

    if (newDist < bestDist) {
      bestDist = newDist;
      matchList.clear();
      matchList.push_back(n);
    }
    else if (newDist == bestDist) {
      matchList.push_back(n);
    }
  }

  delete gridNodes;

  dist = bestDist;
  assert(!matchList.empty());

  if (matchList.size() == 1) {
    n = matchList.front();
  }
  else {
    //Take randomly a vector in the matchlist.
    unsigned int num = RANDNUM(matchList.size()-1);
    assert(num < matchList.size());
    n = matchList[num];
  }

  assert(n.isValid());
  assert(map->isElement(n));
  return n;
}

void SOMAlgorithm::propagateModification(SOMMap* map, const DynamicVector<double>& input, node bmu,
    unsigned int currentIteration, unsigned int maxIteration, unsigned int sampleSize) {

  //Parcour en largeur
  MutableContainer<bool> seen;
  seen.setAll(false);
  MutableContainer<int> distance;
  distance.setAll(0);
  deque<node> toVisit;

  toVisit.push_back(bmu);
  seen.set(bmu.id, true);

  //Computing learning rate for this propagation
  double learningRate = learningRateFunction->computeCurrentTimeRate(currentIteration,
                        maxIteration, sampleSize);

  if (learningRate == 0)
    return;

  //Treatment of the deque
  while (!toVisit.empty()) {
    node current = toVisit.front();
    toVisit.pop_front();

    assert(current.isValid());
    assert(map->isElement(current));
    //Treat its value
    DynamicVector<double> weight = map->getWeight(current);
    assert(weight.getSize() != 0);
    double diffusionRate = diffusionRateFunction->computeSpaceRate(distance.get(current.id),
                           currentIteration, maxIteration, sampleSize);

    //Diffusion function
    weight = weight + ((input - weight) * learningRate * diffusionRate);
    map->setWeight(current, weight);

    //Mark neighborhood
    //If the diffusion rate is equal to 0 no need to propagate modification
    if (diffusionRate > 0) {
      node neighbor;
      forEach   (neighbor,map->getInOutNodes(current)) {
        //not already treated
        if (!seen.get(neighbor.id)) {
          seen.set(neighbor.id, true);
          distance.set(neighbor.id, distance.get(current.id) + 1);
          toVisit.push_back(neighbor);
        }
      }
    }
  }

}

void SOMAlgorithm::computeMapping(SOMMap* map, InputSample& inputSample, std::map<tlp::node,
                                  std::set<tlp::node> >& mappingTab, double& medDist, unsigned int& maxElement) {

  double cumDist = 0;
  double dist;
  maxElement = 0;

  node n;

  forEach(n,inputSample.getNodes()) {
    node somNode = findBMU(map, inputSample.getWeight(n), dist);
    cumDist += dist;
    mappingTab[somNode].insert(n);

    if (mappingTab[somNode].size() > maxElement)
      maxElement = mappingTab[somNode].size();
  }
  medDist = cumDist / inputSample.getSampleSize();
}

