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
//#include <cppunit/ui/qt/TestRunner.h>
#include <cstdlib>
#include <fstream>

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <tulip/TlpTools.h>

int main(int argc, char** argv) {
  tlp::initTulipLib();

  //CPPUNIT_NS::QtUi::TestRunner runner;
  CPPUNIT_NS::TestResult controller;
  CPPUNIT_NS::TestResultCollector result;
  controller.addListener(&result);

  CPPUNIT_NS::TextUi::TestRunner runner;
  runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
  runner.run(controller);

  std::stringstream s;
  s << "cpptestresults";

  if(argc > 1) {
    s << "-" << argv[1];
  }

  s << ".xml";

  std::ofstream xmlFileOut(s.str().c_str());
  CPPUNIT_NS::XmlOutputter xmlOut(&result, xmlFileOut);
  xmlOut.write();

  CPPUNIT_NS::TextOutputter stdOut(&result, std::cout);
  stdOut.write();

  return result.wasSuccessful() ? EXIT_SUCCESS : EXIT_FAILURE;
}
