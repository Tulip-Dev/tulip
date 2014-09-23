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

#ifndef INPUTSAMPLE_H_
#define INPUTSAMPLE_H_

#include <map>
#include <vector>
#include <tulip/Graph.h>
#include <tulip/Observable.h>
#include <tulip/DoubleProperty.h>
#include "DynamicVector.h"

/**
  * Handle all the the samples data used to train the SOM. Take a graph and a vector of DoubleProperties to use it as sample data.
  * This object can handle standard or normalized values.
  **/
namespace tlp {
class InputSample: public tlp::Observable {
public:
  InputSample(tlp::Graph *graph = NULL);
  InputSample(tlp::Graph *graph,
              const std::vector<std::string>& propertiesToListen);

  void setGraph(tlp::Graph *graph);
  void setGraph(tlp::Graph *graph,
                const std::vector<std::string>& propertiesToListen);

  void setPropertiesToListen(const std::vector<std::string>& propertiesToListen);
  std::vector<std::string> getListenedProperties();
  virtual ~InputSample();

  const DynamicVector<double>& getWeight(tlp::node n);

  tlp::Iterator<tlp::node>* getNodes();
  tlp::Iterator<tlp::node>* getRandomNodeOrder();

  inline unsigned int getSampleSize() const {
    assert(rootGraph);
    return rootGraph->numberOfNodes();
  }
  inline unsigned int getDimensionOfSample() const {
    return propertiesList.size();
  }
  inline tlp::Graph* getGraph() {
    return rootGraph;
  }

  void update(std::set<tlp::Observable *>::iterator begin, std::set<
              tlp::Observable *>::iterator end);
  void observableDestroyed(Observable *) {
  }
  ;

  void addNode(tlp::Graph *, const tlp::node);
  void delNode(tlp::Graph *, const tlp::node);
  void delLocalProperty(tlp::Graph*, const std::string&);

  double getMeanProperty(const std::string& propertyName);
  double getSDProperty(const std::string& propertyName);

  bool isUsingNormalizedValues() const;
  void setUsingNormalizedValues(bool);

  /**
    * Search the index of the property with the given name.
    **/
  unsigned findIndexForProperty(const std::string& propertyName)const;

  //recover normalized value for val according to meanProperties[propNum] and sdProperties[propNum]
  double normalize(double val,unsigned propNum);
  //convinience function
  double normalize(double val,const std::string& propertyName) {
    return normalize(val,findIndexForProperty(propertyName));
  }
  //recover unnormalized value for val according to meanProperties[propNum] and sdProperties[propNum]
  double unnormalize(double val,unsigned propNum);
  double unnormalize(double val,const std::string& propertyName) {
    return unnormalize(val,findIndexForProperty(propertyName));
  }


protected:

  void initGraphObs();
  void clearGraphObs();

  void initPropertiesObs();
  void clearPropertiesObs();

  void buildPropertyVector(const std::vector<std::string>& propertiesToListen);

  //Compute the mean value of propertiesList[i] and store it in meanProperties[i]
  void updateMeanValue(unsigned int);
  //Compute the standard deviation value of propertiesList[i] and store it in sdProperties[i]
  void updateSDValue(unsigned int);
  //Compute the mean values of all propertiesList and store them in meanProperties
  void updateAllMeanValues();
  //Compute the standard deviation values of all propertiesList and store them in sdProperties
  void updateAllSDValues();

  //void buildNodeVector(unsigned int i);
  void buildNodeVector(tlp::node n);

  tlp::node getNodeNumber(unsigned int i);
  unsigned int getNumberForNode(tlp::node n);

  //Data sources
  tlp::Graph *rootGraph;

  //Cache for data avoid to create a somVector each time we need
  //tlp::MutableContainer<DynamicVector<double> > mWeightTab;
  std::map<unsigned int ,DynamicVector<double> > mWeightTab;

  //Container used to store random list
  std::vector<tlp::node> randomVector;

  //The number of properties to listen
  std::vector<std::string> propertiesNameList;
  std::vector<tlp::DoubleProperty*> propertiesList;


  //The mean of each property
  std::vector<double> meanProperties;
  //The standard deviation of each property
  std::vector<double> sdProperties;

  //Indicates whether or not normalized values should be used
  bool usingNormalizedValues;

};
}
#endif /* INPUTSAMPLE_H_ */
