#include <tulip/Graph.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StableIterator.h>
#include <tulip/FilterIterator.h>
#include <tulip/SortIterator.h>
#include <tulip/StlIterator.h>
#include <tulip/UniqueIterator.h>
#include <tulip/ConversionIterator.h>

#include <set>

#include "IteratorTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( IteratorTest );

const unsigned int numberOfNodes = 4;

void IteratorTest::setUp() {
  graph = tlp::newGraph();
  idMetric = graph->getProperty<tlp::DoubleProperty>("idMetric");
  for (unsigned int i = 0 ; i < numberOfNodes ; ++i) {
    tlp::node n = graph->addNode();
    idMetric->setNodeValue(n, n.id);
  }
}

void IteratorTest::tearDown() {
  delete graph;
}

void IteratorTest::testIterator() {
  std::set<tlp::node> nodesSet;
  for(tlp::node n : graph->getNodes()) {
    nodesSet.insert(n);
  }
  CPPUNIT_ASSERT_EQUAL(numberOfNodes, static_cast<unsigned int>(nodesSet.size()));
}

void IteratorTest::testFilterIterator() {
  std::set<tlp::node> nodesSet;
  for(tlp::node n : tlp::filterIterator(graph->getNodes(),
                                        [](const tlp::node &n) {return n.id%2 == 0;})) {
    nodesSet.insert(n);
  }
  CPPUNIT_ASSERT_EQUAL(numberOfNodes/2, static_cast<unsigned int>(nodesSet.size()));
}

void IteratorTest::testSortIterator() {
  std::vector<int> inputVec = {2, 1, 3, 0};
  auto cmpFunc = [](const int &a, const int &b) {
    return a > b;
  };
  std::vector<int> sortedVec;
  for(int i : tlp::sortIterator(tlp::stlIterator(inputVec), cmpFunc)) {
    sortedVec.push_back(i);
  }
  std::vector<int> expected = {3, 2, 1, 0};
  for (size_t i = 0 ; i < sortedVec.size() ; ++i) {
    CPPUNIT_ASSERT_EQUAL(expected[i], sortedVec[i]);
  }
}

void IteratorTest::testUniqueIterator() {
  std::vector<int> inputVec = {2, 0, 2, 5, 5, 0};

  std::vector<int> uniqueVec;
  for(int i : tlp::uniqueIterator(tlp::stlIterator(inputVec))) {
    uniqueVec.push_back(i);
  }
  std::vector<int> expected = {2, 0, 5};
  for (size_t i = 0 ; i < uniqueVec.size() ; ++i) {
    CPPUNIT_ASSERT_EQUAL(expected[i], uniqueVec[i]);
  }
}

void IteratorTest::testConversionIterator() {
  std::ostringstream oss;
  auto intToStr = [&](int i) {oss.str(""); oss << i; return oss.str();};
  std::vector<int> inputVec = {1, 2, 3, 4};

  std::vector<std::string> convertedVec;
  for(const std::string &s : tlp::conversionIterator<std::string>(tlp::stlIterator(inputVec), intToStr)) {
    convertedVec.push_back(s);
  }
  std::vector<std::string> expected = {"1", "2", "3", "4"};
  for (size_t i = 0 ; i < convertedVec.size() ; ++i) {
    CPPUNIT_ASSERT_EQUAL(expected[i], convertedVec[i]);
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
                   [this](const tlp::node &n) {
                     idMetric->setNodeValue(n, n.id+1);
                   });
  for(tlp::node n : graph->getNodes()) {
    CPPUNIT_ASSERT_EQUAL(n.id + 1.0, idMetric->getNodeValue(n));
  }
}

void IteratorTest::testIteratorReduce() {
  unsigned int refVal = 0;
  for(tlp::node n : graph->getNodes()) {
    refVal += n.id;
  }
  unsigned int val = tlp::iteratorReduce(graph->getNodes(), 0u,
                                         [](unsigned int curVal, const tlp::node &n) {
                                           return curVal + n.id;
                                         });
  CPPUNIT_ASSERT_EQUAL(refVal, val);

}
