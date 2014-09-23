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
#include <iostream>
#include <cstdlib>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <tulip/TlpTools.h>
#ifndef NDEBUG
#include <tulip/PluginLoaderTxt.h>
#endif
#include <tulip/PluginLibraryLoader.h>

static void loadTulipPluginsFromDir(const std::string &pluginsDir, tlp::PluginLoader *pluginLoader=NULL) {
    std::string oldTlpPluginsPath = tlp::TulipPluginsPath;
    tlp::TulipPluginsPath = pluginsDir;
    tlp::PluginLibraryLoader::loadPlugins(pluginLoader);
    tlp::TulipPluginsPath = oldTlpPluginsPath;
}

int main(int /* arg */, char ** /* argv */) {

    std::string tulipBuildDir = TULIP_BUILD_DIR;

    tlp::initTulipLib();
    tlp::PluginLoader* pLoader = NULL;
#ifndef NDEBUG
    tlp::PluginLoaderTxt loader;
    pLoader = &loader;
#endif

    loadTulipPluginsFromDir(tulipBuildDir + "/plugins/clustering", pLoader);
    loadTulipPluginsFromDir(tulipBuildDir + "/plugins/colors", pLoader);
    loadTulipPluginsFromDir(tulipBuildDir + "/plugins/export", pLoader);
    loadTulipPluginsFromDir(tulipBuildDir + "/plugins/import", pLoader);
    loadTulipPluginsFromDir(tulipBuildDir + "/plugins/layout", pLoader);
    loadTulipPluginsFromDir(tulipBuildDir + "/plugins/layout/FastOverlapRemoval", pLoader);
    loadTulipPluginsFromDir(tulipBuildDir + "/plugins/metric", pLoader);
    loadTulipPluginsFromDir(tulipBuildDir + "/plugins/selection", pLoader);
    loadTulipPluginsFromDir(tulipBuildDir + "/plugins/sizes", pLoader);


    //CPPUNIT_NS::QtUi::TestRunner runner;
    CPPUNIT_NS::TestResult controller;
    CPPUNIT_NS::TestResultCollector result;
    controller.addListener(&result);

    CPPUNIT_NS::TextUi::TestRunner runner;
    runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
    runner.run(controller);

    std::ofstream xmlFileOut("cpptestresults.xml");
    CPPUNIT_NS::XmlOutputter xmlOut(&result, xmlFileOut);
    xmlOut.write();

    CPPUNIT_NS::TextOutputter stdOut(&result, std::cout);
    stdOut.write();

    return result.wasSuccessful() ? EXIT_SUCCESS : EXIT_FAILURE;
}
