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
#include <cassert>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include "IdManagerTest.h"

using namespace std;
using namespace tlp;

CPPUNIT_TEST_SUITE_REGISTRATION( IdManagerTest );

//==========================================================
void IdManagerTest::testFragmentation() {
  for (unsigned int i = 0; i <1000; ++i) {
    CPPUNIT_ASSERT_EQUAL(i, idManager->get());
  }

  for (unsigned int i = 1; i <100; ++i) {
    idManager->free(i);
  }

  CPPUNIT_ASSERT_EQUAL((size_t)99, idManager->state.freeIds.size());
  idManager->free(0);
  CPPUNIT_ASSERT_EQUAL((size_t)0, idManager->state.freeIds.size());

  for (unsigned int i = 900; i <999; ++i) {
    idManager->free(i);
  }

  CPPUNIT_ASSERT_EQUAL((size_t)99, idManager->state.freeIds.size());
  idManager->free(999);
  CPPUNIT_ASSERT_EQUAL((size_t)100, idManager->state.freeIds.size());
}
//==========================================================
void IdManagerTest::testGetFree() {
  for (unsigned int i = 0; i <1000; ++i) {
    CPPUNIT_ASSERT_EQUAL(i, idManager->get());
  }

  for (unsigned int i = 0; i <500; ++i) {
    idManager->free(i*2);
  }

  for (unsigned int i = 1; i < 500; ++i) {
#ifdef TLP_NO_IDS_REUSE
    CPPUNIT_ASSERT_EQUAL(++maxId, idManager->get());
#else
    CPPUNIT_ASSERT_EQUAL(i*2, idManager->get());
#endif
  }

  for (unsigned int i = 100; i <= 200; ++i) {
    idManager->free(i);
  }

  for (unsigned int i = 100; i <= 200; ++i) {
#ifdef TLP_NO_IDS_REUSE
    CPPUNIT_ASSERT_EQUAL(++maxId, idManager->get());
#else
    CPPUNIT_ASSERT_EQUAL(i, idManager->get());
#endif
  }
}
//==========================================================
void IdManagerTest::testIsFree() {
  for (unsigned int i = 0; i <1000; ++i) {
    idManager->get();
  }

  for (unsigned int i = 0; i <500; ++i) {
    idManager->free(i*2);
  }

  for (unsigned int i = 0; i < 500; ++i) {
    CPPUNIT_ASSERT(idManager->is_free(i * 2));
    CPPUNIT_ASSERT(!idManager->is_free(i * 2 + 1));
  }

  CPPUNIT_ASSERT(idManager->is_free(1200));
}
//==========================================================
void IdManagerTest::testIterate() {
  for (unsigned int i = 0; i <1000; ++i) {
    idManager->get();
  }

  Iterator<unsigned int>* it = idManager->getIds<unsigned int>();
  unsigned int id = 0;

  while(it->hasNext()) {
    CPPUNIT_ASSERT_EQUAL(id, it->next());
    ++id;
  }

  delete it;

  for (unsigned int i = 0; i <500; ++i) {
    idManager->free(i*2);
  }

  it = idManager->getIds<unsigned int>();
  id = 0;

  while(it->hasNext()) {
    CPPUNIT_ASSERT_EQUAL(2u * id + 1u, it->next());
    ++id;
  }

  delete it;

  for (unsigned int i = 0; i < 500; ++i) {
    CPPUNIT_ASSERT(idManager->is_free(i * 2));
    CPPUNIT_ASSERT(!idManager->is_free(i * 2 + 1));
  }

  CPPUNIT_ASSERT(idManager->is_free(1200));
}
