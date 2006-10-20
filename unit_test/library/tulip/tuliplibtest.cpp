#include <qapplication.h>
//#include <cppunit/ui/qt/TestRunner.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <tulip/TlpTools.h>

int main( int argc, char** argv ) {
  QApplication app( argc, argv );

  tlp::initTulipLib();
  tlp::loadPlugins();   // library side plugins  

  //CPPUNIT_NS::QtUi::TestRunner runner;
  CPPUNIT_NS::TextUi::TestRunner runner;
  runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest() );
  runner.run( );
  return 0;
}

/*
#include <iostream>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include "MutableContainerTest.h"
#include "SelectionProxyTest.h"
#include "BiconnexTestTest.h"
#include "SuperGraphTest.h"
#include "ExtendedClusterOperationTest.h"
#include "PluginsTest.h"
#include "MatrixTest.h"
#include "TlpImportExportTest.h"
using namespace std;
int main( int argc, char **argv) {
  CppUnit::TextUi::TestRunner runner;
  runner.addTest( SuperGraphTest::suite() );
  runner.addTest( MutableContainerTest::suite() );
  runner.addTest( SelectionProxyTest::suite() );
  runner.addTest( BiconnexTestTest::suite() );
  //  runner.addTest( PluginsTest::suite() );
  runner.addTest( ExtendedClusterOperationTest::suite() );
  runner.addTest( SuperGraphMapTest::suite());
  runner.addTest( FaceIteratorTest::suite());
  runner.addTest( MatrixTest::suite());
  runner.addTest( TlpImportExportTest::suite());
  runner.run();
  return EXIT_SUCCESS;
}
*/
