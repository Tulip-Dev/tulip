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
#include <QtCore/QDir>

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QDesktopServices>
#include <QtNetwork/QTcpSocket>

#include <tulip/PluginLister.h>
#include <tulip/PluginLibraryLoader.h>
#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>
#include <tulip/TulipSettings.h>
#include <tulip/PluginManager.h>
#include <tulip/QuaZIPFacade.h>

#include <CrashHandling.h>

#include "TulipMainWindow.h"
#include "PluginLoaderReporter.h"
#include "PluginLoaderDispatcher.h"
#include "TulipSplashScreen.h"
#include "PluginsCenter.h"
#include "FormPost.h"
#include <tulip/SystemDefinition.h>

#if defined(__APPLE__)
#include <sys/types.h>
#include <signal.h>
#endif

#ifdef interface
#undef interface
#endif

void sendUsageStatistics() {
  QNetworkAccessManager* mgr = new QNetworkAccessManager;
  QObject::connect(mgr,SIGNAL(finished(QNetworkReply*)),mgr,SLOT(deleteLater()));
  mgr->get(QNetworkRequest(QUrl(QString("http://tulip.labri.fr/TulipStats/tulip_stats.php?tulip=") + TULIP_RELEASE + "&os=" + OS_PLATFORM)));
}

void checkTulipRunning() {
  QFile lockFile(QDir(QDesktopServices::storageLocation(QDesktopServices::TempLocation)).filePath("tulip.lck"));
  if (lockFile.exists() && lockFile.open(QIODevice::ReadOnly)) {
    QString agentPort = lockFile.readAll();
    bool ok;
    int n_agentPort = agentPort.toInt(&ok);
    if (ok) {
      QTcpSocket sck;
      sck.connectToHost(QHostAddress::LocalHost,n_agentPort);
      sck.waitForConnected(1000);
      if (sck.state() == QAbstractSocket::ConnectedState) {
        sck.write("SHOW_AGENT PROJECTS");
        sck.flush();
        sck.close();
        exit(0);
      }
    }
  }
  lockFile.close();
  lockFile.remove();
}

int main(int argc, char **argv) {
  start_crash_handler();
  QApplication tulip_agent(argc, argv);
  tulip_agent.setApplicationName("tulip");
  checkTulipRunning();

  TulipSettings::instance().applyProxySettings();

  if (TulipSettings::instance().isFirstRun()) {
    TulipSettings::instance().setFirstRun(false);
    TulipSettings::instance().addRemoteLocation(STABLE_LOCATION);
    TulipSettings::instance().addRemoteLocation(TESTING_LOCATION);
  }

  sendUsageStatistics();

  QDir::home().mkpath(tlp::localPluginsPath());
  QLocale::setDefault(QLocale(QLocale::English));

#if defined(__APPLE__)
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/../");
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/../lib/");
#endif

  foreach (const QString& plugin, tlp::PluginManager::markedForRemoval()) {
    QFile f(plugin);
    f.remove();
    //whether or not the removal succeeded, do not try again
    tlp::PluginManager::unmarkForRemoval(plugin);
  }

  tlp::initTulipLib(QApplication::applicationDirPath().toUtf8().data());
  //TODO find a cleaner way to achieve this (QDesktopServices is part of QtGui, so it does not belong in TlpTools)
  tlp::TulipPluginsPath = (tlp::localPluginsPath() + QDir::separator() + "lib" + QDir::separator() + "tulip").toStdString() +
                          tlp::PATH_DELIMITER +
                          tlp::TulipPluginsPath +
                          tlp::PATH_DELIMITER +
                          tlp::getPluginLocalInstallationDir().toStdString();

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

  QFile f(QDir(QDesktopServices::storageLocation(QDesktopServices::TempLocation)).filePath("tulip.lck"));
  f.remove();

  return result;
}
