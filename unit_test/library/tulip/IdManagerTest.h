/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#ifndef TLPBIEXTCLUSTEROPERATION
#define TLPBIEXTCLUSTEROPERATION

#include <string>
#include <tulip/Graph.h>
#include "../../../library/tulip/include/tulip/IdManager.h"
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

namespace tlp {

class IdManagerTest : public CppUnit::TestFixture {

public:
  void setUp() {idManager = new IdManager(); }
  void tearDown(){ delete idManager;}
  void testIsFree();
  void testFragmentation();
  void testGetFree();
  void testIterate();

  static CppUnit::Test *suite();

private:
  IdManager *idManager;
};

}
#endif
