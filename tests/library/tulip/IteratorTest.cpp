/**
 *
 *This file is part of Tulip (https://tulip.labri.fr)
 *
 *Authors: David Auber and the Tulip development Team
 *from LaBRI, University of Bordeaux
 *
 *Tulip is free software; you can redistribute it and/or modify
 *it under the terms of the GNU Lesser General Public License
 *as published by the Free Software Foundation, either version 3
 *of the License, or (at your option) any later version.
 *
 *Tulip is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *See the GNU General Public License for more details.
 *
 */

#include <tulip/Graph.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StableIterator.h>
#include <tulip/FilterIterator.h>
#include <tulip/SortIterator.h>
#include <tulip/UniqueIterator.h>
#include <tulip/ConversionIterator.h>
#include <tulip/ConcatIterator.h>
#include <tulip/StlIterator.h>

#include <set>

#include "IteratorTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(IteratorTest);

const unsigned int numberOfNodes = 4;

void IteratorTest::setUp() {
  graph = tlp::newGraph();
  idMetric = graph->getProperty<tlp::DoubleProperty>("idMetric");
  for (unsigned int i = 0; i < numberOfNodes; ++i) {
    tlp::node n = graph->addNode();
    idMetric->setNodeValue(n, n.id);
  }
}

void IteratorTest::tearDown() {
  delete graph;
}

void IteratorTest::testIterator() {
  std::set<tlp::node> nodesSet;
  for (tlp::node n : graph->getNodes()) {
    nodesSet.insert(n);
  }
  CPPUNIT_ASSERT_EQUAL(numberOfNodes, static_cast<unsigned int>(nodesSet.size()));
}

void IteratorTest::testFilterIterator() {
  std::set<tlp::node> nodesSet;
  for (tlp::node n :
       tlp::filterIterator(graph->getNodes(), [](const tlp::node &n) { return n.id % 2 == 0; })) {
    nodesSet.insert(n);
  }
  CPPUNIT_ASSERT_EQUAL(numberOfNodes / 2, static_cast<unsigned int>(nodesSet.size()));
}

void IteratorTest::testSortIterator() {
  std::vector<int> inputVec = {2, 1, 3, 0};
  auto cmpFunc = [](const int &a, const int &b) { return a > b; };
  std::vector<int> sortedVec;
  for (int i : tlp::sortIterator(inputVec, cmpFunc)) {
    sortedVec.push_back(i);
  }
  std::vector<int> expected = {3, 2, 1, 0};
  for (size_t i = 0; i < sortedVec.size(); ++i) {
    CPPUNIT_ASSERT_EQUAL(expected[i], sortedVec[i]);
  }
}

void IteratorTest::testUniqueIterator() {
  std::vector<int> inputVec = {2, 0, 2, 5, 5, 0};

  std::vector<int> uniqueVec;
  for (int i : tlp::uniqueIterator(inputVec)) {
    uniqueVec.push_back(i);
  }
  std::vector<int> expected = {2, 0, 5};
  for (size_t i = 0; i < uniqueVec.size(); ++i) {
    CPPUNIT_ASSERT_EQUAL(expected[i], uniqueVec[i]);
  }

  auto check = [](int v) { return v % 2 == 0; };

  uniqueVec.clear();
  for (int i : tlp::uniqueIterator(inputVec, check)) {
    uniqueVec.push_back(i);
  }
  expected = {2, 0, 5, 5};
  for (size_t i = 0; i < uniqueVec.size(); ++i) {
    CPPUNIT_ASSERT_EQUAL(expected[i], uniqueVec[i]);
  }
}

void IteratorTest::testConversionIterator() {
  auto intToStr = [&](int i) { return std::to_string(i); };
  std::vector<int> inputVec = {1, 2, 3, 4};

  std::vector<std::string> convertedVec;
  for (const std::string &s : tlp::conversionIterator<std::string>(inputVec, intToStr)) {
    convertedVec.push_back(s);
  }
  std::vector<std::string> expected = {"1", "2", "3", "4"};
  for (size_t i = 0; i < convertedVec.size(); ++i) {
    CPPUNIT_ASSERT_EQUAL(expected[i], convertedVec[i]);
  }
}

void IteratorTest::testConcatIterator() {
  std::vector<std::string> inputVec1 = {"a", "b", "c"};
  std::vector<std::string> inputVec2 = {"d", "e", "f"};

  std::vector<std::string> concatVec;
  for (const std::string &s :
       tlp::concatIterator(tlp::stlIterator(inputVec1), tlp::stlIterator(inputVec2))) {
    concatVec.push_back(s);
  }

  std::vector<std::string> expected = inputVec1;
  expected.insert(expected.end(), inputVec2.begin(), inputVec2.end());
  for (size_t i = 0; i < concatVec.size(); ++i) {
    CPPUNIT_ASSERT_EQUAL(expected[i], concatVec[i]);
  }
}

void IteratorTest::testStableIterator() {
  unsigned int notStableIterated = 0;
  unsigned int stableIterated = 0;
  tlp::Iterator<tlp::node> *stableIt = tlp::stableIterator(graph->getNodes());
  graph->clear();
  notStableIterated = tlp::iteratorCount(graph->getNodes());
  stableIterated = tlp::iteratorCount(stableIt);
  CPPUNIT_ASSERT_EQUAL(notStableIterated, 0u);
  CPPUNIT_ASSERT_EQUAL(stableIterated, numberOfNodes);
}

void IteratorTest::testIteratorCount() {
  CPPUNIT_ASSERT_EQUAL(graph->numberOfNodes(), tlp::iteratorCount(graph->getNodes()));
}

void IteratorTest::testIteratorMap() {
  tlp::iteratorMap(graph->getNodes(),
                   [this](const tlp::node &n) { idMetric->setNodeValue(n, n.id + 1); });
  for (tlp::node n : graph->getNodes()) {
    CPPUNIT_ASSERT_EQUAL(n.id + 1.0, idMetric->getNodeValue(n));
  }
}

void IteratorTest::testIteratorReduce() {
  unsigned int refVal = 0;
  for (tlp::node n : graph->getNodes()) {
    refVal += n.id;
  }
  unsigned int val = tlp::iteratorReduce(
      graph->getNodes(), 0u, [](unsigned int curVal, const tlp::node &n) { return curVal + n.id; });
  CPPUNIT_ASSERT_EQUAL(refVal, val);
}

void IteratorTest::testIteratorCountCheck() {
  for (unsigned int i = 0; i < numberOfNodes; ++i) {
    CPPUNIT_ASSERT(tlp::iteratorCountCheck(graph->getNodes(), i + 1));
  }
  CPPUNIT_ASSERT(tlp::iteratorEmpty(graph->getEdges()));
}

void IteratorTest::testIteratorToStlContainers() {
  std::vector<tlp::node> vNodes = tlp::iteratorVector(graph->getNodes());
  std::set<tlp::node> sNodes = tlp::iteratorSet(graph->getNodes());
  std::list<tlp::node> lNodes = tlp::iteratorList(graph->getNodes());

  for (auto n : graph->nodes()) {
    CPPUNIT_ASSERT(std::find(lNodes.begin(), lNodes.end(), n) != lNodes.end());
    CPPUNIT_ASSERT(std::find(vNodes.begin(), vNodes.end(), n) != vNodes.end());
    CPPUNIT_ASSERT(sNodes.find(n) != sNodes.end());
  }
}
