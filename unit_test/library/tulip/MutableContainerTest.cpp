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
#include <cstdlib>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include "MutableContainerTest.h"
#include <tulip/Iterator.h>
#include <fstream>

using namespace std;
using namespace tlp;

CPPUNIT_TEST_SUITE_REGISTRATION( MutableContainerTest );

//==========================================================
void MutableContainerTest::setUp() {
  mutBool    = new MutableContainer<bool>();
  mutDouble  = new MutableContainer<double>();
  mutString  = new MutableContainer<string>();
}
//==========================================================
void MutableContainerTest::tearDown() {
  delete mutBool;
  delete mutDouble;
  delete mutString;
}
//==========================================================
void MutableContainerTest::testCompression() {
  mutDouble->setAll(10.0);
  CPPUNIT_ASSERT_EQUAL(MutableContainer<double>::VECT, mutDouble->state);
  CPPUNIT_ASSERT_EQUAL(unsigned(0), mutDouble->elementInserted);

  for (unsigned int i=0; i<1000; ++i) {
    mutDouble->set(100+i,13.0);
    CPPUNIT_ASSERT_EQUAL(MutableContainer<double>::VECT, mutDouble->state);
    CPPUNIT_ASSERT_EQUAL(i+1 , mutDouble->elementInserted);
  }

  mutDouble->setAll(10.0);
  CPPUNIT_ASSERT_EQUAL(MutableContainer<double>::VECT, mutDouble->state);
  CPPUNIT_ASSERT_EQUAL(unsigned(0), mutDouble->elementInserted);

  for (unsigned int i=0; i<1000; ++i) {
    mutDouble->set(10000+i*2,13.0);
    CPPUNIT_ASSERT_EQUAL(MutableContainer<double>::VECT, mutDouble->state);
    CPPUNIT_ASSERT_EQUAL(i+1 , mutDouble->elementInserted);
  }

  for (unsigned int i=0; i<1000; ++i) {
    mutDouble->set(10000+i*2,13.0);
    mutDouble->add(10000+i*2, i);
    CPPUNIT_ASSERT_EQUAL(mutDouble->get(10000+i*2), 13. + i);
    CPPUNIT_ASSERT_EQUAL(MutableContainer<double>::VECT, mutDouble->state);
    CPPUNIT_ASSERT_EQUAL(unsigned(1000) , mutDouble->elementInserted);
  }

  mutDouble->setAll(10.0);
  CPPUNIT_ASSERT_EQUAL(MutableContainer<double>::VECT, mutDouble->state);
  CPPUNIT_ASSERT_EQUAL(unsigned(0), mutDouble->elementInserted);
  mutDouble->set(1000, 345);
  mutDouble->set(10000, 345);
  CPPUNIT_ASSERT_EQUAL(MutableContainer<double>::HASH, mutDouble->state);
  CPPUNIT_ASSERT_EQUAL(unsigned(2), mutDouble->elementInserted);
  mutDouble->add(1000, 1000.5);
  CPPUNIT_ASSERT_EQUAL(mutDouble->get(1000), 1345.5);
  mutDouble->add(10000, -1000.2);
  CPPUNIT_ASSERT_EQUAL(mutDouble->get(10000), 345. - 1000.2);
  mutDouble->add(100, 1000.45);
  CPPUNIT_ASSERT_EQUAL(mutDouble->get(100), 1010.45);

  for (unsigned int i=1001; i<10000; ++i) {
    mutDouble->set(i, 345);
    CPPUNIT_ASSERT_EQUAL(unsigned(i+3-1000) , mutDouble->elementInserted);
  }

  CPPUNIT_ASSERT_EQUAL(MutableContainer<double>::VECT, mutDouble->state);
  mutDouble->setAll(-1);
  std::ifstream in("sillyMutableData.txt");
  unsigned int id;

  while (in >> id) {
    double value;
    in >> value;
    mutDouble->set(id, value);
  }
}
//==========================================================
void MutableContainerTest::testFindAll() {
  mutBool->setAll(false);
  mutDouble->setAll(10.0);
  mutString->setAll("David");

  mutBool->set(10,true);
  mutBool->set(15,true);
  mutBool->set(17,true);

  mutDouble->set(10,13.0);
  mutDouble->set(15,13.0);
  mutDouble->set(17,13.0);

  mutString->set(10,"Sophie");
  mutString->set(15,"Sophie");
  mutString->set(17,"Sophie");

  Iterator<unsigned int> *itB;
  Iterator<unsigned int> *itD;
  Iterator<unsigned int> *itS;

  itB = mutBool->findAll(true);
  itD = mutDouble->findAll(13.0);
  itS = mutString->findAll("Sophie");
  CPPUNIT_ASSERT(itB->hasNext());
  CPPUNIT_ASSERT_EQUAL(10u, itB->next());
  CPPUNIT_ASSERT(itD->hasNext());
  CPPUNIT_ASSERT_EQUAL(10u, itD->next());
  CPPUNIT_ASSERT(itS->hasNext());
  CPPUNIT_ASSERT_EQUAL(10u, itS->next());

  CPPUNIT_ASSERT(itB->hasNext());
  CPPUNIT_ASSERT_EQUAL(15u, itB->next());
  CPPUNIT_ASSERT(itD->hasNext());
  CPPUNIT_ASSERT_EQUAL(15u, itD->next());
  CPPUNIT_ASSERT(itS->hasNext());
  CPPUNIT_ASSERT_EQUAL(15u, itS->next());

  CPPUNIT_ASSERT(itB->hasNext());
  CPPUNIT_ASSERT_EQUAL(17u, itB->next());
  CPPUNIT_ASSERT(itD->hasNext());
  CPPUNIT_ASSERT_EQUAL(17u, itD->next());
  CPPUNIT_ASSERT(itS->hasNext());
  CPPUNIT_ASSERT_EQUAL(17u, itS->next());

  delete itB;
  delete itD;
  delete itS;

  Iterator<unsigned int> *nullIterator = NULL;

  itB = mutBool->findAll(false);
  CPPUNIT_ASSERT_EQUAL(nullIterator, itB);

  itD = mutDouble->findAll(10.0);
  CPPUNIT_ASSERT_EQUAL(nullIterator, itD);

  itS = mutString->findAll("David");
  CPPUNIT_ASSERT_EQUAL(nullIterator, itS);
}
//==========================================================
void MutableContainerTest::testSetAll() {
  mutBool->setAll(true);
  mutDouble->setAll(10.0);
  mutString->setAll("David");

  for (unsigned int i=0; i<NBTEST; ++i) {
    CPPUNIT_ASSERT(mutBool->get(i));
    CPPUNIT_ASSERT_EQUAL(10.0, mutDouble->get(i));
    CPPUNIT_ASSERT_EQUAL(string("David"), mutString->get(i));
  }

  mutBool->setAll(false);
  mutDouble->setAll(-33.0);
  mutString->setAll("Sophie");

  for (unsigned int i=0; i<NBTEST; ++i) {
    CPPUNIT_ASSERT(!mutBool->get(i));
    CPPUNIT_ASSERT_EQUAL(-33.0, mutDouble->get(i));
    CPPUNIT_ASSERT_EQUAL(string("Sophie"), mutString->get(i));
  }

  //New test bug at index 0
  MutableContainer<int> vect;
  vect.setAll(-1);
  CPPUNIT_ASSERT_EQUAL(-1, vect.get(1));
  vect.set(1,1);
  CPPUNIT_ASSERT_EQUAL(1, vect.get(1));
  CPPUNIT_ASSERT_EQUAL(-1, vect.get(2));
  CPPUNIT_ASSERT_EQUAL(-1, vect.get(0));

  MutableContainer<int> vect2;
  vect2.setAll(0);
  vect2.set(0,0);
  CPPUNIT_ASSERT_EQUAL(0, vect2.get(0));
  vect2.set(1,0);
  CPPUNIT_ASSERT_EQUAL(0, vect2.get(1));
}
//==========================================================
void MutableContainerTest::testSetGet() {
  mutBool->setAll(false);
  mutDouble->setAll(-33.0);
  mutDouble->set(100, 10);
  mutDouble->set(1000000, 20);
  CPPUNIT_ASSERT_EQUAL(10.0, mutDouble->get(100));
  CPPUNIT_ASSERT_EQUAL(20.0, mutDouble->get(1000000));
  mutDouble->set(1000000,-33.0);

  for (unsigned int i=101; i<1000; ++i) {
    mutDouble->set(i,i);
    CPPUNIT_ASSERT_EQUAL((double)i, mutDouble->get(i));
  }

  for (unsigned int i=101; i<999; ++i) {
    mutDouble->set(i,-33.0);
  }

  CPPUNIT_ASSERT_EQUAL(10.0, mutDouble->get(100));

  mutString->setAll("Sophie");

  for (unsigned int i=0; i<NBTEST*10; ++i) {
    unsigned int rando=rand()%NBTEST;
    mutBool->set(rando,true);
    mutDouble->set(rando, rando);
    mutString->set(rando, string("David"));
    CPPUNIT_ASSERT(mutBool->get(rando));
    CPPUNIT_ASSERT_EQUAL((double)rando, mutDouble->get(rando));
    CPPUNIT_ASSERT_EQUAL(string("David"), mutString->get(rando));
  }

  mutBool->setAll(true);

  for (unsigned int i=0; i<NBTEST*10; ++i) {
    unsigned int rando=rand()%NBTEST;
    mutBool->set(rando, false);
    CPPUNIT_ASSERT(!mutBool->get(rando));
  }

  for (unsigned int i=0; i<NBTEST*10; ++i) {
    unsigned int rando=rand()%NBTEST;
    bool isNotDefault = true;
    mutBool->set(rando, true);
    CPPUNIT_ASSERT(mutBool->get(rando, isNotDefault));
    CPPUNIT_ASSERT(!isNotDefault);
    mutString->set(rando, string("Sophie"));
    CPPUNIT_ASSERT_EQUAL(string("Sophie"), mutString->get(rando, isNotDefault));
    CPPUNIT_ASSERT(!isNotDefault);
  }
}
