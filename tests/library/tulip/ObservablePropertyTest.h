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
#ifndef TLPOBSERVABLEPROPERTYTEST
#define TLPOBSERVABLEPROPERTYTEST

#include <tulip/Graph.h>
#include <tulip/TlpTools.h>
#include <tulip/AbstractProperty.h>

#include "CppUnitIncludes.h"

class ObservablePropertyTest : public CppUnit::TestFixture {
private:
  tlp::Graph *graph;
  tlp::PropertyInterface *props[7];

public:
  void setUp();
  void tearDown();

  void testAddObserver();
  void testSynchronousSetNodeValue();
  void testAsynchronousSetNodeValue();
  void testSynchronousSetAllNodeValue();
  void testAsynchronousSetAllNodeValue();
  void testSynchronousSetEdgeValue();
  void testAsynchronousSetEdgeValue();
  void testSynchronousSetAllEdgeValue();
  void testAsynchronousSetAllEdgeValue();
  void testSynchronousDelete();
  void testAsynchronousDelete();
  void testRemoveObserver();
  void testObserverWhenRemoveObservable();
  void testNoPropertiesEventsAfterGraphClear();

  void setNodeValue(tlp::PropertyInterface*, const char*, bool, bool, bool = true);
  void setEdgeValue(tlp::PropertyInterface*, const char*, bool, bool, bool = true);
  void addObservers();

  static CppUnit::Test *suite();
};

#endif
