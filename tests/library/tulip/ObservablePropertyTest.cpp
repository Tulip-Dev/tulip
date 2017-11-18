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

#include <tulip/ForEach.h>
#include "ObservablePropertyTest.h"
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>

using namespace std;
using namespace tlp;

const unsigned int NB_NODES = 4;
const unsigned int EDGE_RATIO = 3;
const unsigned int NB_EDGES = EDGE_RATIO * NB_NODES;

#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION(ObservablePropertyTest);

// these classes will capture
// everything that will happen to our properties
// synchronously or asynchronously
class ObserverPTest : public Observable {
public:
  std::set<Observable *> observables;

  ObserverPTest() {}

  void reset() {
    observables.clear();
  }

  unsigned int nbObservables() {
    return observables.size();
  }

  bool found(Observable *obs) {
    return observables.find(obs) != observables.end();
  }

  void treatEvents(const vector<Event> &events) {
    if (events[0].type() == Event::TLP_DELETE) {
      observables.insert(events[0].sender());
    } else {
      reset();

      for (unsigned int i = 0; i < events.size(); ++i) {
        observables.insert(events[i].sender());
      }
    }
  }
};

static ObserverPTest *observer;

// this class will capture
// everything that will happen to our properties
class PropertyObserverTest : public Observable {
public:
  std::set<PropertyInterface *> properties;
  node lastNode;
  edge lastEdge;

  PropertyObserverTest() {}

  void reset() {
    properties.clear();
  }

  unsigned int nbProperties() {
    return properties.size();
  }

  bool found(PropertyInterface *prop) {
    return properties.find(prop) != properties.end();
  }

  node getNode() {
    return lastNode;
  }

  edge getEdge() {
    return lastEdge;
  }

  void beforeSetNodeValue(PropertyInterface *prop, const node n) {
    properties.insert(prop);
    lastNode = n;
  }

  virtual void beforeSetEdgeValue(PropertyInterface *prop, const edge e) {
    properties.insert(prop);
    lastEdge = e;
  }

  virtual void beforeSetAllNodeValue(PropertyInterface *prop) {
    properties.insert(prop);
  }
  virtual void beforeSetAllEdgeValue(PropertyInterface *prop) {
    properties.insert(prop);
  }
  virtual void destroy(PropertyInterface *prop) {
    properties.insert(prop);
  }
  virtual void treatEvent(const Event &evt) {
    const PropertyEvent *propEvt = dynamic_cast<const PropertyEvent *>(&evt);

    if (propEvt) {
      PropertyInterface *prop = propEvt->getProperty();

      switch (propEvt->getType()) {
      case PropertyEvent::TLP_BEFORE_SET_NODE_VALUE:
        beforeSetNodeValue(prop, propEvt->getNode());
        return;

      case PropertyEvent::TLP_BEFORE_SET_EDGE_VALUE:
        beforeSetEdgeValue(prop, propEvt->getEdge());
        return;

      case PropertyEvent::TLP_BEFORE_SET_ALL_NODE_VALUE:
        beforeSetAllNodeValue(prop);
        return;

      case PropertyEvent::TLP_BEFORE_SET_ALL_EDGE_VALUE:
        beforeSetAllEdgeValue(prop);
        return;
      default:
        return;
      }
    } else {
      PropertyInterface *prop = dynamic_cast<PropertyInterface *>(evt.sender());

      if (prop && evt.type() == Event::TLP_DELETE)
        destroy(prop);
    }
  }
};

static PropertyObserverTest *pObserver;

#define DOUBLE_PROP 2
#define INTEGER_PROP 3
#define LAYOUT_PROP 4
#define SIZE_PROP 5
//==========================================================
void ObservablePropertyTest::setUp() {
#ifndef NDEBUG
  // choose the seed to ease the debugging
  tlp::setSeedOfRandomSequence(1);
  tlp::initRandomSequence();
#endif

  graph = tlp::newGraph();

  props[0] = graph->getProperty<BooleanProperty>("boolProp");
  props[1] = graph->getProperty<ColorProperty>("colorProp");
  props[2] = graph->getProperty<DoubleProperty>("doubleProp");
  props[3] = graph->getProperty<IntegerProperty>("intProp");
  props[4] = graph->getProperty<LayoutProperty>("layoutProp");
  props[5] = graph->getProperty<SizeProperty>("sizeProp");
  props[6] = graph->getProperty<StringProperty>("stringProp");

  vector<node> nodes;

  for (unsigned int i = 0; i < NB_NODES; ++i)
    nodes.push_back(graph->addNode());

  for (unsigned int i = 0; i < NB_EDGES; ++i)
    graph->addEdge(nodes[randomUnsignedInteger(NB_NODES - 1)],
                   nodes[randomUnsignedInteger(NB_NODES - 1)]);

  observer = new ObserverPTest();
  pObserver = new PropertyObserverTest();

  addObservers();
}

//==========================================================
void ObservablePropertyTest::tearDown() {
  delete graph;
  delete observer;
  delete pObserver;
}

void ObservablePropertyTest::setNodeValue(PropertyInterface *prop, const char *val, bool all,
                                          bool found1, bool found2) {
  observer->reset();
  CPPUNIT_ASSERT(observer->nbObservables() == 0);
  pObserver->reset();
  CPPUNIT_ASSERT(pObserver->nbProperties() == 0);

  if (all) {
    prop->setAllNodeStringValue(string(val));
    CPPUNIT_ASSERT(observer->nbObservables() == (found1 ? 1 : 0));
    CPPUNIT_ASSERT(observer->found(prop) == found1);
    CPPUNIT_ASSERT(pObserver->nbProperties() == (found2 ? 1 : 0));
    CPPUNIT_ASSERT(pObserver->found(prop) == found2);
  } else {
    node n;
    forEach(n, graph->getNodes()) {
      prop->setNodeStringValue(n, string(val));
      CPPUNIT_ASSERT(observer->nbObservables() == (found1 ? 1 : 0));
      CPPUNIT_ASSERT(observer->found(prop) == found1);
      CPPUNIT_ASSERT(pObserver->nbProperties() == (found2 ? 1 : 0));
      CPPUNIT_ASSERT(pObserver->found(prop) == found2);
      CPPUNIT_ASSERT(!found2 || pObserver->getNode() == n);
    }
  }
}

void ObservablePropertyTest::setEdgeValue(PropertyInterface *prop, const char *val, bool all,
                                          bool found1, bool found2) {
  observer->reset();
  CPPUNIT_ASSERT(observer->nbObservables() == 0);
  pObserver->reset();
  CPPUNIT_ASSERT(pObserver->nbProperties() == 0);

  if (all) {
    prop->setAllEdgeStringValue(string(val));
    CPPUNIT_ASSERT(observer->nbObservables() == (found1 ? 1 : 0));
    CPPUNIT_ASSERT(observer->found(prop) == found1);
    CPPUNIT_ASSERT(pObserver->nbProperties() == (found2 ? 1 : 0));
    CPPUNIT_ASSERT(pObserver->found(prop) == found2);
  } else {
    edge e;
    forEach(e, graph->getEdges()) {
      prop->setEdgeStringValue(e, string(val));
      CPPUNIT_ASSERT(observer->nbObservables() == (found1 ? 1 : 0));
      CPPUNIT_ASSERT(observer->found(prop) == found1);
      CPPUNIT_ASSERT(pObserver->nbProperties() == (found2 ? 1 : 0));
      CPPUNIT_ASSERT(pObserver->found(prop) == found2);
      CPPUNIT_ASSERT(!found2 || pObserver->getEdge() == e);
    }
  }
}

//==========================================================
void ObservablePropertyTest::addObservers() {
  CPPUNIT_ASSERT(observer->nbObservables() == 0);
  CPPUNIT_ASSERT(pObserver->nbProperties() == 0);

  for (unsigned int i = 0; i < 7; ++i) {
    props[i]->addObserver(observer);
    CPPUNIT_ASSERT(observer->nbObservables() == 0);
    props[i]->addListener(pObserver);
    CPPUNIT_ASSERT(pObserver->nbProperties() == 0);
  }
}
//==========================================================
void ObservablePropertyTest::testAddObserver() {
  CPPUNIT_ASSERT(observer->nbObservables() == 0);
  CPPUNIT_ASSERT(pObserver->nbProperties() == 0);

  for (unsigned int i = 0; i < 7; ++i) {
    CPPUNIT_ASSERT(props[i]->countObservers() == 1);
    // CPPUNIT_ASSERT(props[i]->countPropertyObservers() == 1); //same as above
  }
}
//==========================================================
void ObservablePropertyTest::testSynchronousSetNodeValue() {
  setNodeValue(props[0], "true", false, true);
  setNodeValue(props[1], "(255, 255, 255, 0)", false, true);
  setNodeValue(props[2], "1.0", false, true);
  setNodeValue(props[3], "1", false, true);
  setNodeValue(props[4], "(0.0, 0.0, 0.0)", false, true);
  setNodeValue(props[5], "(0.0, 0.0, 0.0)", false, true);
  setNodeValue(props[6], "tulip", false, true);
}

//==========================================================
void ObservablePropertyTest::testSynchronousSetEdgeValue() {
  setEdgeValue(props[0], "true", false, true);
  setEdgeValue(props[1], "(255, 255, 255, 0)", false, true);
  setEdgeValue(props[2], "1.0", false, true);
  setEdgeValue(props[3], "1", false, true);
  setEdgeValue(props[4], "()", false, true);
  setEdgeValue(props[5], "(0.0, 0.0, 0.0)", false, true);
  setEdgeValue(props[6], "tulip", false, true);
}

//==========================================================
void ObservablePropertyTest::testAsynchronousSetNodeValue() {
  Observable::holdObservers();
  setNodeValue(props[0], "true", false, false);
  setNodeValue(props[1], "(255, 255, 255, 0)", false, false);
  setNodeValue(props[2], "1.0", false, false);
  setNodeValue(props[3], "1", false, false);
  setNodeValue(props[4], "(0.0, 0.0, 0.0)", false, false);
  setNodeValue(props[5], "(0.0, 0.0, 0.0)", false, false);
  setNodeValue(props[6], "tulip", false, false);
  Observable::unholdObservers();
  CPPUNIT_ASSERT(observer->nbObservables() == 7);

  for (unsigned int i = 0; i < 7; ++i)
    CPPUNIT_ASSERT(observer->found(props[i]));
}

//==========================================================
void ObservablePropertyTest::testAsynchronousSetEdgeValue() {
  Observable::holdObservers();
  setEdgeValue(props[0], "true", false, false);
  setEdgeValue(props[1], "(255, 255, 255, 0)", false, false);
  setEdgeValue(props[2], "1.0", false, false);
  setEdgeValue(props[3], "1", false, false);
  setEdgeValue(props[4], "()", false, false);
  setEdgeValue(props[5], "(0.0, 0.0, 0.0)", false, false);
  setEdgeValue(props[6], "tulip", false, false);
  Observable::unholdObservers();
  CPPUNIT_ASSERT(observer->nbObservables() == 7);

  for (unsigned int i = 0; i < 7; ++i)
    CPPUNIT_ASSERT(observer->found(props[i]));
}

//==========================================================
void ObservablePropertyTest::testSynchronousSetAllNodeValue() {
  setNodeValue(props[0], "true", true, true);
  setNodeValue(props[1], "(255, 255, 255, 0)", true, true);
  setNodeValue(props[2], "1.0", true, true);
  setNodeValue(props[3], "1", true, true);
  setNodeValue(props[4], "(0.0, 0.0, 0.0)", true, true);
  setNodeValue(props[5], "(0.0, 0.0, 0.0)", true, true);
  setNodeValue(props[6], "tulip", true, true);
}

//==========================================================
void ObservablePropertyTest::testSynchronousSetAllEdgeValue() {
  setEdgeValue(props[0], "true", true, true);
  setEdgeValue(props[1], "(255, 255, 255, 0)", true, true);
  setEdgeValue(props[2], "1.0", true, true);
  setEdgeValue(props[3], "1", true, true);
  setEdgeValue(props[4], "()", true, true);
  setEdgeValue(props[5], "(0.0, 0.0, 0.0)", true, true);
  setEdgeValue(props[6], "tulip", true, true);
}

//==========================================================
void ObservablePropertyTest::testAsynchronousSetAllNodeValue() {
  Observable::holdObservers();
  setNodeValue(props[0], "true", true, false);
  setNodeValue(props[1], "(255, 255, 255, 0)", true, false);
  setNodeValue(props[2], "1.0", true, false);
  setNodeValue(props[3], "1", true, false);
  setNodeValue(props[4], "(0.0, 0.0, 0.0)", true, false);
  setNodeValue(props[5], "(0.0, 0.0, 0.0)", true, false);
  setNodeValue(props[6], "tulip", true, false);
  Observable::unholdObservers();
  CPPUNIT_ASSERT(observer->nbObservables() == 7);

  for (unsigned int i = 0; i < 7; ++i)
    CPPUNIT_ASSERT(observer->found(props[i]));
}

//==========================================================
void ObservablePropertyTest::testAsynchronousSetAllEdgeValue() {
  Observable::holdObservers();
  setEdgeValue(props[0], "true", true, false);
  setEdgeValue(props[1], "(255, 255, 255, 0)", true, false);
  setEdgeValue(props[2], "1.0", true, false);
  setEdgeValue(props[3], "1", true, false);
  setEdgeValue(props[4], "()", true, false);
  setEdgeValue(props[5], "(0.0, 0.0, 0.0)", true, false);
  setEdgeValue(props[6], "tulip", true, false);
  Observable::unholdObservers();
  CPPUNIT_ASSERT(observer->nbObservables() == 7);

  for (unsigned int i = 0; i < 7; ++i)
    CPPUNIT_ASSERT(observer->found(props[i]));
}

//==========================================================
void ObservablePropertyTest::testSynchronousDelete() {
  CPPUNIT_ASSERT(observer->nbObservables() == 0);
  CPPUNIT_ASSERT(pObserver->nbProperties() == 0);

  for (unsigned int i = 0; i < 7; ++i) {
    PropertyInterface *prop = props[i];
    graph->delLocalProperty(prop->getName());
    CPPUNIT_ASSERT(observer->nbObservables() == i + 1);
    CPPUNIT_ASSERT(observer->found(prop));
    CPPUNIT_ASSERT(pObserver->nbProperties() == i + 1);
    CPPUNIT_ASSERT(pObserver->found(prop));
  }
}

//==========================================================
void ObservablePropertyTest::testAsynchronousDelete() {
  CPPUNIT_ASSERT(observer->nbObservables() == 0);
  CPPUNIT_ASSERT(pObserver->nbProperties() == 0);
  Observable::holdObservers();

  for (unsigned int i = 0; i < 7; ++i) {
    PropertyInterface *prop = props[i];
    graph->delLocalProperty(prop->getName());
    // deletion is not asynchronous
    CPPUNIT_ASSERT(observer->nbObservables() == i + 1);
    CPPUNIT_ASSERT(observer->found(prop));
    CPPUNIT_ASSERT(pObserver->nbProperties() == i + 1);
    CPPUNIT_ASSERT(pObserver->found(prop));
  }

  Observable::unholdObservers();
}

//==========================================================
void ObservablePropertyTest::testRemoveObserver() {
  CPPUNIT_ASSERT(observer->nbObservables() == 0);
  CPPUNIT_ASSERT(pObserver->nbProperties() == 0);

  for (unsigned int i = 0; i < 7; ++i) {
    props[i]->removeObserver(observer);
    props[i]->removeListener(pObserver);
    CPPUNIT_ASSERT(props[i]->countObservers() == 0);
    // CPPUNIT_ASSERT(props[i]->countPropertyObservers() == 0); same as above
  }

  // no more notification
  setNodeValue(props[0], "true", true, false, false);
  setNodeValue(props[1], "(255, 255, 255, 0)", true, false, false);
  setNodeValue(props[2], "1.0", true, false, false);
  setNodeValue(props[3], "1", true, false, false);
  setNodeValue(props[4], "(0.0, 0.0, 0.0)", true, false, false);
  setNodeValue(props[5], "(0.0, 0.0, 0.0)", true, false, false);
  setNodeValue(props[6], "tulip", true, false, false);
  setEdgeValue(props[0], "true", true, false, false);
  setEdgeValue(props[1], "(255, 255, 255, 0)", true, false, false);
  setEdgeValue(props[2], "1.0", true, false, false);
  setEdgeValue(props[3], "1", true, false, false);
  setEdgeValue(props[4], "()", true, false, false);
  setEdgeValue(props[5], "(0.0, 0.0, 0.0)", true, false, false);
  setEdgeValue(props[6], "tulip", true, false, false);

  for (unsigned int i = 0; i < 7; ++i) {
    graph->delLocalProperty(props[i]->getName());
    CPPUNIT_ASSERT(observer->nbObservables() == 0);
    CPPUNIT_ASSERT(pObserver->nbProperties() == 0);
  }
}

//==========================================================
void ObservablePropertyTest::testObserverWhenRemoveObservable() {
  CPPUNIT_ASSERT(props[0]->countListeners() == 1);
  CPPUNIT_ASSERT(props[0]->countObservers() == 1);
  PropertyObserverTest *pObserverTmp = new PropertyObserverTest();
  props[0]->addListener(pObserverTmp);
  CPPUNIT_ASSERT(props[0]->countListeners() == 2);
  CPPUNIT_ASSERT(props[0]->countObservers() == 1);
  delete pObserverTmp;
  CPPUNIT_ASSERT(props[0]->countListeners() == 1);
  CPPUNIT_ASSERT(props[0]->countObservers() == 1);
}

void ObservablePropertyTest::testNoPropertiesEventsAfterGraphClear() {
  // create a clone subgraph with a local property
  Graph *clone = graph->addCloneSubGraph("clone");
  PropertyInterface *cloneProp = clone->getProperty<DoubleProperty>("cloneProp");
  // listen to local property of clone subgraph
  cloneProp->addListener(pObserver);

  // clear the clone subgraph (remove nodes/edges)
  clone->clear();
  // check that no properties events have been received
  CPPUNIT_ASSERT(pObserver->nbProperties() == 0);

  // delete the local property
  clone->delLocalProperty(cloneProp->getName());
  // reset the property observer as it just received the notification that a property has been
  // deleted
  pObserver->reset();

  // clear the graph (remove nodes/edges/subgraphs)
  graph->clear();
  // check that no properties events have been received
  CPPUNIT_ASSERT(pObserver->nbProperties() == 0);
}

//==========================================================
CppUnit::Test *ObservablePropertyTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("Tulip lib : Graph");
  suiteOfTests->addTest(new CppUnit::TestCaller<ObservablePropertyTest>(
      "Add an observer", &ObservablePropertyTest::testAddObserver));
  suiteOfTests->addTest(new CppUnit::TestCaller<ObservablePropertyTest>(
      "synchronous setNodeValue", &ObservablePropertyTest::testSynchronousSetNodeValue));
  suiteOfTests->addTest(new CppUnit::TestCaller<ObservablePropertyTest>(
      "synchronous setEdgeValue", &ObservablePropertyTest::testSynchronousSetEdgeValue));
  suiteOfTests->addTest(new CppUnit::TestCaller<ObservablePropertyTest>(
      "asynchronous setNodeValue", &ObservablePropertyTest::testAsynchronousSetNodeValue));
  suiteOfTests->addTest(new CppUnit::TestCaller<ObservablePropertyTest>(
      "asynchronous setEdgeValue", &ObservablePropertyTest::testAsynchronousSetEdgeValue));
  suiteOfTests->addTest(new CppUnit::TestCaller<ObservablePropertyTest>(
      "synchronous setAllNodeValue", &ObservablePropertyTest::testSynchronousSetAllNodeValue));
  suiteOfTests->addTest(new CppUnit::TestCaller<ObservablePropertyTest>(
      "synchronous setAllEdgeValue", &ObservablePropertyTest::testSynchronousSetAllEdgeValue));
  suiteOfTests->addTest(new CppUnit::TestCaller<ObservablePropertyTest>(
      "asynchronous setAllNodeValue", &ObservablePropertyTest::testAsynchronousSetAllNodeValue));
  suiteOfTests->addTest(new CppUnit::TestCaller<ObservablePropertyTest>(
      "asynchronous setAllEdgeValue", &ObservablePropertyTest::testAsynchronousSetAllEdgeValue));
  suiteOfTests->addTest(new CppUnit::TestCaller<ObservablePropertyTest>(
      "synchronous delete", &ObservablePropertyTest::testSynchronousDelete));
  suiteOfTests->addTest(new CppUnit::TestCaller<ObservablePropertyTest>(
      "asynchronous delete", &ObservablePropertyTest::testAsynchronousDelete));
  suiteOfTests->addTest(new CppUnit::TestCaller<ObservablePropertyTest>(
      "removeObserver", &ObservablePropertyTest::testRemoveObserver));
  suiteOfTests->addTest(new CppUnit::TestCaller<ObservablePropertyTest>(
      "observerWhenRemoveObservable", &ObservablePropertyTest::testObserverWhenRemoveObservable));
  suiteOfTests->addTest(new CppUnit::TestCaller<ObservablePropertyTest>(
      "noPropertiesEventsAfterGraphClear",
      &ObservablePropertyTest::testNoPropertiesEventsAfterGraphClear));
  return suiteOfTests;
}
//==========================================================
