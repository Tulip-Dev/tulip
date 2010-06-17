/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include <stdlib.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include "MutableContainerTest.h"
#include <tulip/Iterator.h>
#include <fstream>

using namespace std;
using namespace tlp;

#include <cppunit/extensions/HelperMacros.h>
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
  CPPUNIT_ASSERT_EQUAL(VECT, mutDouble->state);
  CPPUNIT_ASSERT_EQUAL(unsigned(0)   , mutDouble->elementInserted);
  for (unsigned int i=0; i<1000; ++i) {
    mutDouble->set(100+i,13.0);
    CPPUNIT_ASSERT_EQUAL(VECT, mutDouble->state);
    CPPUNIT_ASSERT_EQUAL(i+1 , mutDouble->elementInserted);
  }
  mutDouble->setAll(10.0);
  CPPUNIT_ASSERT_EQUAL(VECT, mutDouble->state);
  CPPUNIT_ASSERT_EQUAL(unsigned(0)   , mutDouble->elementInserted);
  for (unsigned int i=0; i<1000; ++i) {
    mutDouble->set(10000+i*2,13.0);
    CPPUNIT_ASSERT_EQUAL(VECT, mutDouble->state);
    CPPUNIT_ASSERT_EQUAL(i+1 , mutDouble->elementInserted);
  }
  for (unsigned int i=0; i<1000; ++i) {
    mutDouble->set(10000+i*2,13.0);
    CPPUNIT_ASSERT_EQUAL(VECT, mutDouble->state);
    CPPUNIT_ASSERT_EQUAL(unsigned(1000) , mutDouble->elementInserted);
  }
  mutDouble->setAll(10.0);
  CPPUNIT_ASSERT_EQUAL(VECT, mutDouble->state);
  CPPUNIT_ASSERT_EQUAL(unsigned(0)   , mutDouble->elementInserted);
  mutDouble->set(1000, 345);
  mutDouble->set(10000, 345);
  CPPUNIT_ASSERT_EQUAL(HASH, mutDouble->state);
  CPPUNIT_ASSERT_EQUAL(unsigned(2)   , mutDouble->elementInserted);
  for (unsigned int i=1001; i<10000; ++i) {
    mutDouble->set(i, 345);
    CPPUNIT_ASSERT_EQUAL(unsigned(i+2-1000) , mutDouble->elementInserted);
  }
  CPPUNIT_ASSERT_EQUAL(VECT, mutDouble->state);
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
  
  CPPUNIT_ASSERT( itB->next() == 10 );
  CPPUNIT_ASSERT( itD->next() == 10 );
  CPPUNIT_ASSERT( itS->next() == 10 );
  
  CPPUNIT_ASSERT( itB->next() == 15 );
  CPPUNIT_ASSERT( itD->next() == 15 );
  CPPUNIT_ASSERT( itS->next() == 15 );

  CPPUNIT_ASSERT( itB->next() == 17 );
  CPPUNIT_ASSERT( itD->next() == 17 );
  CPPUNIT_ASSERT( itS->next() == 17 );

  delete itB;
  delete itD;
  delete itS;
  itB = mutBool->findAll(false);
  CPPUNIT_ASSERT(itB == NULL);

  itD = mutDouble->findAll(10.0);
  CPPUNIT_ASSERT(itD == NULL);

  itS = mutString->findAll("David");
  CPPUNIT_ASSERT(itS == NULL);

}
//==========================================================
void MutableContainerTest::testSetAll() {
  mutBool->setAll(true);
  mutDouble->setAll(10.0);
  mutString->setAll("David");
  for (unsigned int i=0;i<NBTEST;++i) {
    CPPUNIT_ASSERT( mutBool->get(i) == true );
    CPPUNIT_ASSERT( mutDouble->get(i) == 10.0 );
    CPPUNIT_ASSERT( mutString->get(i) == string("David") );
  }
  
  mutBool->setAll(false);
  mutDouble->setAll(-33.0);
  mutString->setAll("Sophie");
  for (unsigned int i=0;i<NBTEST;++i) {
    CPPUNIT_ASSERT( mutBool->get(i) == false );
    CPPUNIT_ASSERT( mutDouble->get(i) == -33.0 );
    CPPUNIT_ASSERT( mutString->get(i) == string("Sophie") );
  }

  //New test bug at index 0
  MutableContainer<int> vect;
  vect.setAll(-1);
  CPPUNIT_ASSERT( vect.get(1) == -1 );
  vect.set(1,1);
  CPPUNIT_ASSERT( vect.get(1) ==  1 );
  CPPUNIT_ASSERT( vect.get(2) == -1 );
  CPPUNIT_ASSERT( vect.get(0) == -1 );

  MutableContainer<int> vect2;
  vect2.setAll(0);
  vect2.set(0,0);
  CPPUNIT_ASSERT( vect2.get(0) ==  0 );
  vect2.set(1,0);
  CPPUNIT_ASSERT( vect2.get(1) ==  0 );
}
//==========================================================
void MutableContainerTest::testSetGet() {
  mutBool->setAll(false);
  mutDouble->setAll(-33.0);
  mutDouble->set(100, 10);
  mutDouble->set(1000000, 20);
  CPPUNIT_ASSERT( mutDouble->get(100) == 10 );
  CPPUNIT_ASSERT( mutDouble->get(1000000) == 20 );
  mutDouble->set(1000000,-33.0);
  for (unsigned int i=101; i<1000; ++i) {
    mutDouble->set(i,i);
    CPPUNIT_ASSERT( mutDouble->get(i) == i );
  }
  for (unsigned int i=101; i<999; ++i) {
    mutDouble->set(i,-33.0);
  }
  CPPUNIT_ASSERT( mutDouble->get(100) == 10 );

  mutString->setAll("Sophie");
  for (unsigned int i=0; i<NBTEST*10; ++i) {
    unsigned int rando=rand()%NBTEST;
    mutBool->set(rando,true);
    mutDouble->set(rando, rando);
    mutString->set(rando, string("David"));
    CPPUNIT_ASSERT( mutBool->get(rando) == true );
    CPPUNIT_ASSERT( mutDouble->get(rando) == rando );
    CPPUNIT_ASSERT( mutString->get(rando) == string("David") );
  }
  mutBool->setAll(true);
  for (unsigned int i=0;i<NBTEST*10;++i) {
    unsigned int rando=rand()%NBTEST;
    mutBool->set(rando, false);
    CPPUNIT_ASSERT( mutBool->get(rando) == false );
  }
}
//==========================================================
CppUnit::Test * MutableContainerTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : MutableContainer" );
  suiteOfTests->addTest( new CppUnit::TestCaller<MutableContainerTest>( 
								       "test setAll", 
								       &MutableContainerTest::testSetAll ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<MutableContainerTest>( 
								       "test set/get", 
								       &MutableContainerTest::testSetGet ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<MutableContainerTest>( 
								       "test find all", 
								       &MutableContainerTest::testFindAll ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<MutableContainerTest>( 
								       "test state of the mutable", 
								       &MutableContainerTest::testCompression ) );
  return suiteOfTests;
}
//==========================================================
