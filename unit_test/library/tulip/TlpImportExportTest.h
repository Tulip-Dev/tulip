#ifndef TLPIMPORTEXPPORT
#define TLPIMPPORTEXPORT

#include <string>
#include <tulip/Graph.h>
#include "../../../library/tulip/include/tulip/TlpTools.h"
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

class TlpImportExportTest : public CppUnit::TestFixture {

public:
  void testLoad();
  void testImport();
  void testSave();
  void testExport();
  static CppUnit::Test *suite();
};

#endif
