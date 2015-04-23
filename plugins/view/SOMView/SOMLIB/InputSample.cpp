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

#include <algorithm>
#include "InputSample.h"
#include <tulip/Iterator.h>
#include <tulip/StlIterator.h>
#include <tulip/ForEach.h>
using namespace std;
using namespace tlp;

InputSample::InputSample(Graph *graph) :
  rootGraph(graph) {
  if (rootGraph) {
    //mWeightTab.setAll(DynamicVector<double> ());
    mWeightTab.clear();
  }

  initGraphObs();

  usingNormalizedValues=true;
}

InputSample::~InputSample() {
  clearGraphObs();
  clearPropertiesObs();
}

InputSample::InputSample(Graph *graph, const vector<string>& propertiesToListen) :
  rootGraph(graph) {
  //mWeightTab.setAll(DynamicVector<double> ());
  mWeightTab.clear();

  setPropertiesToListen(propertiesToListen);
  initGraphObs();

  usingNormalizedValues=true;
}

void InputSample::setGraph(tlp::Graph *graph) {

  clearGraphObs();
  rootGraph = graph;
  //rootGraph->addObserver(this);
  //mWeightTab.setAll(DynamicVector<double> ());
  mWeightTab.clear();

  setPropertiesToListen(vector<string> (propertiesNameList));

  initGraphObs();
}

void InputSample::setGraph(tlp::Graph *graph,
                           const std::vector<std::string>& propertiesToListen) {
  clearGraphObs();
  setGraph(graph);
  setPropertiesToListen(propertiesToListen);
  initGraphObs();
}

void InputSample::buildPropertyVector(
  const std::vector<std::string>& propertiesToListen) {
  propertiesNameList.clear();
  propertiesList.clear();
  PropertyInterface *property;

  for (vector<string>::const_iterator it = propertiesToListen.begin(); it
       != propertiesToListen.end(); ++it) {
    if (rootGraph->existProperty(*it)) {
      property = rootGraph->getProperty((*it));
      string type = property->getTypename();

      if (type.compare("double") == 0) {
        propertiesNameList.push_back(*it);
        propertiesList.push_back((DoubleProperty*) property);
      }
      else {
        cerr << __PRETTY_FUNCTION__ << ":" << __LINE__ << " "
             << "Type not managed" << endl;
      }
    }
  }

  if(usingNormalizedValues) {
    updateAllMeanValues();
    updateAllSDValues();
  }
}


void InputSample::setPropertiesToListen(
  const vector<string>& propertiesToListen) {
  if (rootGraph) {
    clearPropertiesObs();
    buildPropertyVector(propertiesToListen);
    //Clear all the cache
    //mWeightTab.setAll(DynamicVector<double> ());
    mWeightTab.clear();
    initPropertiesObs();
  }
}

std::vector<std::string> InputSample::getListenedProperties() {
  return propertiesNameList;
}

node InputSample::getNodeNumber(unsigned int i) {
  if (rootGraph) {
    node n;
    unsigned int num = 0;
    Iterator<node> *nIt = rootGraph->getNodes();
    n = nIt->next();

    while (num < i) {
      assert(nIt->hasNext());
      n = nIt->next();
      ++num;
    }

    delete nIt;
    return n;
  }
  else
    return node();
}

unsigned int InputSample::getNumberForNode(tlp::node no) {
  assert(rootGraph && rootGraph->isElement(no));
  node n;
  unsigned int num = 0;
  Iterator<node> *nIt = rootGraph->getNodes();
  n = nIt->next();

  while (n != no) {
    assert(nIt->hasNext());
    n = nIt->next();
    ++num;
  }

  delete nIt;
  return num;
}

void InputSample::buildNodeVector(node n) {
  DynamicVector<double> nodeVec(propertiesList.size());
  nodeVec.fill(0);
  unsigned int propNum = 0;

  if(usingNormalizedValues) {
    for (; propNum < propertiesList.size(); ++propNum) {
      nodeVec[propNum] = normalize(propertiesList[propNum]->getNodeValue(n),propNum);
    }
  }
  else {
    for (; propNum < propertiesList.size(); ++propNum) {
      nodeVec[propNum] = propertiesList[propNum]->getNodeValue(n);
    }
  }

  //mWeightTab.set(n.id, nodeVec);
  mWeightTab[n.id] = nodeVec;
}

double InputSample::normalize(double val, unsigned propNum) {
  if(propNum<meanProperties.size() && propNum<sdProperties.size())
    return (val-meanProperties[propNum])/sdProperties[propNum];

  return val;
}

double InputSample::unnormalize(double val, unsigned propNum) {
  if(propNum<meanProperties.size() && propNum<sdProperties.size())
    return val*sdProperties[propNum]+meanProperties[propNum];

  return val;
}

const DynamicVector<double>& InputSample::getWeight(tlp::node n) {
  if (rootGraph && propertiesList.empty()) {
    std::cerr << __PRETTY_FUNCTION__ << ":" << __LINE__ << " "
              << "Warning no properties specified" << std::endl;
    assert(false);
    //return DynamicVector<double>();
  }

  DynamicVector<double> vec;

  if (/*!mWeightTab.getIfNotDefaultValue(n.id, vec)*/mWeightTab.find(n.id)
      == mWeightTab.end()) {
    buildNodeVector(n);
  }

  //return mWeightTab.getReference(n.id);

  return mWeightTab[n.id];
}

void InputSample::initGraphObs() {
  if (rootGraph)
    rootGraph->addListener(this);
}
void InputSample::clearGraphObs() {
  if (rootGraph)
    rootGraph->removeListener(this);
}

void InputSample::initPropertiesObs() {
  for (vector<DoubleProperty*>::iterator it = propertiesList.begin(); it
       != propertiesList.end(); ++it) {
    (*it)->addObserver(this);
  }
}
void InputSample::clearPropertiesObs() {
  for (vector<DoubleProperty*>::iterator it = propertiesList.begin(); it
       != propertiesList.end(); ++it) {
    (*it)->removeObserver(this);
  }
}

void InputSample::update(std::set<Observable *>::iterator begin, std::set<
                         Observable *>::iterator end) {
  bool updated = false;

  //A property has been updated clear the cache
  for (std::set<Observable *>::iterator it = begin; it != end; ++it) {
    unsigned int propNum=0;

    for (std::vector<tlp::DoubleProperty*>::iterator itP =
           propertiesList.begin(); itP != propertiesList.end(); ++itP) {
      if ((*it) == (*itP)) {
        //mWeightTab.setAll(DynamicVector<double> ());
        mWeightTab.clear();

        if(usingNormalizedValues) {
          updateMeanValue(propNum);
          updateSDValue(propNum);
        }

        updated = true;

        // notify observers
        if (hasOnlookers())
          sendEvent(Event(*this, Event::TLP_MODIFICATION));

        break;
      }

      propNum++;
    }

    if (updated)
      break;
  }

}

void InputSample::addNode(Graph*, const node n) {
  if(usingNormalizedValues) {
    for(unsigned int i=0; i<propertiesList.size(); ++i) {
      meanProperties[i]=(double(rootGraph->numberOfNodes()-1)*meanProperties[i]+propertiesList[i]->getNodeValue(n))/double(rootGraph->numberOfNodes());
      updateSDValue(i);
    }
  }

  // notify observers
  if (hasOnlookers())
    sendEvent(Event(*this, Event::TLP_MODIFICATION));
}
void InputSample::delNode(Graph*, const node n) {
  if(usingNormalizedValues) {
    for(unsigned int i=0; i<propertiesList.size(); ++i) {
      meanProperties[i]=(double(rootGraph->numberOfNodes()+1)*meanProperties[i]-propertiesList[i]->getNodeValue(n))/double(rootGraph->numberOfNodes());
      updateSDValue(i);
    }
  }

  if (mWeightTab.find(n.id) != mWeightTab.end()) {
    mWeightTab.erase(n.id);
  }

  // notify observers
  if (hasOnlookers())
    sendEvent(Event(*this, Event::TLP_MODIFICATION));
}
void InputSample::delLocalProperty(Graph*, const std::string& propName) {
  unsigned int i = 0;

  for (; i < propertiesNameList.size(); ++i) {
    if (propertiesNameList[i].compare(propName) == 0) {
      propertiesNameList.erase(propertiesNameList.begin() + i);
      propertiesList.erase(propertiesList.begin() + i);
      meanProperties.erase(meanProperties.begin()+i);
      sdProperties.erase(meanProperties.begin()+i);
      //mWeightTab.setAll(DynamicVector<double> ());
      mWeightTab.clear();

      // notify observers
      if (hasOnlookers())
        sendEvent(Event(*this, Event::TLP_MODIFICATION));

      break;
    }
  }
}

tlp::Iterator<tlp::node>* InputSample::getNodes() {
  if (rootGraph)
    return rootGraph->getNodes();
  else
    return NULL;
}

tlp::Iterator<tlp::node>* InputSample::getRandomNodeOrder() {
  if (rootGraph) {
    randomVector.clear();
    node n;
    forEach(n,rootGraph->getNodes()) {
      //Random placing nodes
      randomVector.push_back(n);
    }
    random_shuffle(randomVector.begin(), randomVector.end());

    return new StlIterator<node, vector<node>::iterator> (randomVector.begin(),
           randomVector.end());
  }
  else
    return NULL;
}

void InputSample::updateMeanValue(unsigned int propNum) {
  assert(propNum<propertiesList.size());
  DoubleProperty* property = propertiesList[propNum];
  double mean=0.0;
  node n;
  forEach(n,rootGraph->getNodes()) {
    mean+=property->getNodeValue(n);
  }
  meanProperties[propNum]=mean/double(rootGraph->numberOfNodes());

}

void InputSample::updateSDValue(unsigned int propNum) {
  assert(propNum<propertiesList.size());

  if(rootGraph->numberOfNodes()<=1) {
    sdProperties[propNum]=1.0;
    return;
  }

  DoubleProperty* property = propertiesList[propNum];
  double sd=0.0;
  node n;
  forEach(n,rootGraph->getNodes())
  sd+=pow(property->getNodeValue(n)-meanProperties[propNum],2.0);

  if(sd<=0.0) {
    sdProperties[propNum]=1.0;
    return;
  }

  sdProperties[propNum]=sqrt(sd/double(rootGraph->numberOfNodes()-1));
}

void InputSample::updateAllMeanValues() {
  meanProperties.resize(propertiesList.size(),0.0);

  for(unsigned int i=0; i<propertiesList.size(); ++i) {
    updateMeanValue(i);
  }
}

void InputSample::updateAllSDValues() {
  sdProperties.resize(propertiesList.size(),1.0);

  for(unsigned int i=0; i<propertiesList.size(); ++i) {
    updateSDValue(i);
  }
}


double InputSample::getMeanProperty(const std::string& propertyName) {
  unsigned int i = findIndexForProperty(propertyName);

  if(i<meanProperties.size())
    return meanProperties[i];

  return 0.0;
}

double InputSample::getSDProperty(const std::string& propertyName) {
  unsigned int i = findIndexForProperty(propertyName);

  if(i<sdProperties.size())
    return sdProperties[i];

  return 1.0;
}

bool InputSample::isUsingNormalizedValues() const {
  return usingNormalizedValues;
}

void InputSample::setUsingNormalizedValues(bool norm) {
  if(norm!=usingNormalizedValues)
    mWeightTab.clear();

  usingNormalizedValues=norm;

  if(usingNormalizedValues) {
    updateAllMeanValues();
    updateAllSDValues();
  }
}


unsigned InputSample::findIndexForProperty(const std::string& propertyName)const {
  for (unsigned int i=0; i < propertiesNameList.size(); ++i) {
    if (propertiesNameList[i].compare(propertyName) == 0) {
      return i;
    }
  }

  return UINT_MAX;
}
