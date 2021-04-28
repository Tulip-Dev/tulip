/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include <QDir>
#include <QLocale>
#include <QProcess>

#include <QApplication>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTcpSocket>

#include <tulip/TlpQtTools.h>
#include <tulip/TulipRelease.h>
#include <tulip/TulipSettings.h>

#include <CrashHandling.h>

#include "FormPost.h"
#include "PluginsCenter.h"
#include "TulipMainWindow.h"
#include "TulipSplashScreen.h"
#include <tulip/SystemDefinition.h>

#if defined(__APPLE__)
#include <signal.h>
#include <sys/types.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif

#ifdef interface
#undef interface
#endif

void sendUsageStatistics() {
  QNetworkAccessManager *mgr = new QNetworkAccessManager;
  QObject::connect(mgr, SIGNAL(finished(QNetworkReply *)), mgr,
                   SLOT(deleteLater()));
  mgr->get(QNetworkRequest(
      QUrl(QString("http://tulip.labri.fr/TulipStats/tulip_stats.php?tulip=") +
           TULIP_VERSION + "&os=" + OS_PLATFORM)));
}

bool sendAgentMessage(int port, const QString &message) {
  bool result = true;

  QTcpSocket sck;
  sck.connectToHost(QHostAddress::LocalHost, port);
  sck.waitForConnected(1000);

  if (sck.state() == QAbstractSocket::ConnectedState) {
    sck.write(message.toUtf8());
    sck.flush();
  } else {
    result = false;
  }

  sck.close();
  return result;
}

void checkTulipRunning(const QString &perspName, const QString &fileToOpen,
                       bool showAgent) {
  QFile lockFile(
      QDir(
          QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0))
          .filePath("tulip.lck"));

  if (lockFile.exists() && lockFile.open(QIODevice::ReadOnly)) {
    QString agentPort = lockFile.readAll();
    bool ok;
    int n_agentPort = agentPort.toInt(&ok);

    if (ok && sendAgentMessage(n_agentPort, "HELLO\tHELLO")) {

      if (showAgent) {
        sendAgentMessage(n_agentPort, "SHOW_AGENT\tPROJECTS");
      }

      // if a file was passed as argument, forward it to the running instance
      if (!fileToOpen.isEmpty()) { // open the file passed as argument
        if (!perspName.isEmpty()) {
          sendAgentMessage(n_agentPort, "OPEN_PROJECT_WITH\t" + perspName +
                                            "\t" + fileToOpen);
        } else {
          sendAgentMessage(n_agentPort, "OPEN_PROJECT\t" + fileToOpen);
        }
      } else if (!perspName
                      .isEmpty()) { // open the perspective passed as argument
        sendAgentMessage(n_agentPort, "CREATE_PERSPECTIVE\t" + perspName);
      }

      exit(0);
    }
  }

  lockFile.close();
  lockFile.remove();
}

int main(int argc, char **argv) {
  CrashHandling::installCrashHandler();

  // Enables resource sharing between the OpenGL contexts
  QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);
  // Enables high-DPI scaling on X11 or Windows platforms
  // Enabled on MacOSX with NSHighResolutionCapable key in Info.plist file
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);

  QApplication tulip_agent(argc, argv);
  QString name("Tulip ");

  // show patch number only if needed
  if (TULIP_INT_VERSION % 10)
    name += TULIP_VERSION;
  else
    name += TULIP_MM_VERSION;

  // the applicationName below is used to identify the location
  // of downloaded plugins, so it must be the same as in
  // tulip_perspective/main.cpp
  tulip_agent.setApplicationName(name);

  // Parse arguments
  QRegExp perspectiveRegexp("^\\-\\-perspective=(.*)");
  QString perspName;
  QString fileToOpen;
  bool showAgent = true;

  for (int i = 1; i < QApplication::arguments().size(); ++i) {
    QString s = QApplication::arguments()[i];

    if (perspectiveRegexp.exactMatch(s)) {
      perspName = perspectiveRegexp.cap(1);
    } else if (s == "--no-show-agent") {
      showAgent = false;
    } else {
      fileToOpen = s;
    }
  }

  showAgent = showAgent || fileToOpen.isEmpty();

  checkTulipRunning(perspName, fileToOpen, showAgent);
  sendUsageStatistics();

  TulipSplashScreen splashScreen;
  tlp::initTulipSoftware(&splashScreen, true);

#ifdef _MSC_VER
  // Add path to Tulip pdb files generated by Visual Studio (for configurations
  // Debug and RelWithDebInfo) It allows to get a detailed stack trace when
  // Tulip crashes.
  CrashHandling::setExtraSymbolsSearchPaths(tlp::TulipShareDir + "pdb");
#endif

  // Main window
  TulipMainWindow *mainWindow = TulipMainWindow::instance();
  mainWindow->pluginsCenter()->reportPluginErrors(splashScreen.errors());

  mainWindow->show();
  splashScreen.finish(mainWindow);

  // Treat arguments
  if (!fileToOpen.isEmpty()) { // open the file passed as argument
    if (!perspName.isEmpty())
      mainWindow->openProjectWith(fileToOpen, perspName);
    else
      mainWindow->openProject(fileToOpen);
  } else if (!perspName.isEmpty()) { // open the perspective passed as argument
    mainWindow->createPerspective(perspName);
  }

  int result = tulip_agent.exec();

  QFile f(
      QDir(
          QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0))
          .filePath("tulip.lck"));
  f.remove();

  return result;
}
