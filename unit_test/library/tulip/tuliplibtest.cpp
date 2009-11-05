//#include <cppunit/ui/qt/TestRunner.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <tulip/TlpTools.h>

int main( int argc, char** argv ) {
  tlp::initTulipLib();

  //CPPUNIT_NS::QtUi::TestRunner runner;
  CPPUNIT_NS::TextUi::TestRunner runner;
  runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest() );
  runner.run( );
  return 0;
}
