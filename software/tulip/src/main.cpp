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

#include <QtCore/QLocale>
#include <QtCore/QProcess>
#include <QtGui/QApplication>

#include <tulip/PluginLister.h>
#include <tulip/PluginLibraryLoader.h>
#include <tulip/TlpTools.h>
#include <QtGui/QMessageBox>

#include "TulipMainWindow.h"
#include "PluginLoaderReporter.h"
#include "PluginLoaderDispatcher.h"
#include "TulipSplashScreen.h"
#include "PluginsCenter.h"

#include <tulip/TlpQtTools.h>

#include <QtGui/QDesktopServices>
#include <QtCore/QDir>
#include <tulip/TulipSettings.h>
#include <tulip/PluginManager.h>
#include <tulip/QuaZIPFacade.h>

#include <CrashHandling.h>

#if defined(__APPLE__)
#include <sys/types.h>
#include <signal.h>
#endif

#ifdef interface
#undef interface
#endif

void updatePlateform(char* tulipPath) {
  tlp::PluginManager::removePlugins();
  tlp::PluginManager::unpackPlugins(tlp::getPluginStagingDirectory());

  //update the updater
  const QString updaterLocation = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/updater";

  //run the updater
  QDir upgradeDir(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/update");

  if(!upgradeDir.entryList(QDir::NoDotAndDotDot).empty()) {
    //we unpack the archives in-place, the contents will be copied later by a script
    tlp::PluginProgress* progress = new tlp::SimplePluginProgress();
    QStringList filters;
    filters << "*.zip";
    foreach(const QFileInfo& pluginArchive, upgradeDir.entryInfoList(filters)) {
      QuaZIPFacade::unzip(tlp::getPluginLocalInstallationDir(), pluginArchive.absoluteFilePath(), progress);

      if(!progress->getError().empty()) {
        //TODO proper error reporting
        std::cout << progress->getError() << std::endl;
      }
      else {
        QFile::remove(pluginArchive.absoluteFilePath());
      }
    }

    //launch the updater and quit, the updater will re-launch Tulip
    QFileInfo tulipExecutable(tulipPath);
#ifdef WIN32
    int result = QProcess::execute(tulipExecutable.canonicalPath() + "/updater.bat");
#else
    int result = QProcess::execute(tulipExecutable.canonicalPath() + "/updater.sh");
#endif
    exit(result);
  }
}

int main(int argc, char **argv) {
  start_crash_handler();
  QApplication tulip_agent(argc, argv);
  tulip_agent.setApplicationName(QObject::trUtf8("Tulip"));
  QLocale::setDefault(QLocale(QLocale::English));

  // revert previous remote locations
  foreach(const QString& remoteLocation, TulipSettings::instance().remoteLocations()) {
    tlp::PluginManager::addRemoteLocation(remoteLocation);
  }
  updatePlateform(argv[0]);

#if defined(__APPLE__)
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/../");
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/../lib/");
#endif

  tlp::initTulipLib(QApplication::applicationDirPath().toUtf8().data());
  //TODO find a cleaner way to achieve this (QDesktopServices is part of QtGui, so it does not belong in TlpTools)
  tlp::TulipPluginsPath = QString(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/plugins/lib/tulip/").toStdString() +
                          tlp::PATH_DELIMITER + tlp::TulipPluginsPath +
                          tlp::PATH_DELIMITER + tlp::getPluginLocalInstallationDir().toStdString();


  // Load plugins
  PluginLoaderDispatcher *dispatcher = new PluginLoaderDispatcher();
  TulipSplashScreen *splashScreen = new TulipSplashScreen();
  PluginLoaderReporter *errorReport = new PluginLoaderReporter();
  dispatcher->registerLoader(errorReport);
  dispatcher->registerLoader(splashScreen);
  tlp::PluginLibraryLoader::loadPlugins(dispatcher);
  tlp::PluginLister::checkLoadedPluginsDependencies(dispatcher);
  delete dispatcher;
  delete splashScreen;


  // Main window
  TulipMainWindow *mainWindow = TulipMainWindow::instance();
  mainWindow->pluginsCenter()->reportPluginErrors(errorReport->errors());
  delete errorReport;

  mainWindow->show();
  int result = tulip_agent.exec();

#ifdef MEMORYCHECKER_ON
  memory_checker_print_report();
#endif // MEMORYCHECKER_ON

  return result;
}
