#include <QtCore/QLocale>
#include <QtGui/QApplication>
#include <QtDBus/QDBusConnection>

#include <tulip/PluginLister.h>
#include <tulip/PluginLibraryLoader.h>
#include <tulip/TlpTools.h>
#include <QtGui/QMessageBox>

#include "TulipAgentServiceAdaptor.h"
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

#if defined(__APPLE__)
#include <sys/types.h>
#include <signal.h>
#endif

#ifdef interface
#undef interface
#endif

void updatePlateform() {
//  tlp::PluginManager::removePlugins();

//  QDir stagingDirectory(tlp::getPluginStagingDirectory());
//  if(!stagingDirectory.exists())
//    stagingDirectory.mkpath(tlp::getPluginStagingDirectory());
//  tlp::PluginManager::unpackPlugins(tlp::getPluginStagingDirectory());

  //update the updater
//  const QString updaterLocation = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/updater";

//  //run the updater
//  QDir upgradeDir(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/update");

//  if(!upgradeDir.entryList(QDir::NoDotAndDotDot).empty()) {
//    //we unpack the archives in-place, the contents will be copied later by a script
//    tlp::PluginProgress* progress = new tlp::SimplePluginProgress();
//    QStringList filters;
//    filters << "*.zip";
//    foreach(const QFileInfo& pluginArchive, upgradeDir.entryInfoList(filters)) {
//      QuaZIPFacade::unzip(tlp::getPluginLocalInstallationDir(), pluginArchive.absoluteFilePath(), progress);

//      if(!progress->getError().empty()) {
//        //TODO proper error reporting
//        std::cout << progress->getError() << std::endl;
//      }
//      else {
//        QFile::remove(pluginArchive.absoluteFilePath());
//      }
//    }

//    //launch the updater and quit, the updater will re-launch Tulip
//    QFileInfo tulipExecutable(argv[0]);
//#ifdef WIN32
//    int result = QProcess::execute(tulipExecutable.canonicalPath() + "/updater.bat");
//#else
//    int result = QProcess::execute(tulipExecutable.canonicalPath() + "/updater.sh");
//#endif
//    exit(0);
//  }
}

#if defined(__APPLE__)
void appleInit() {
  // allows to load qt imageformats plugin
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/..");

  // Switch the current directory to ensure that libdbus is loaded
  QString currentPath = QDir::currentPath();
  QDir::setCurrent(QApplication::applicationDirPath() + "/../Frameworks");

  // Manually run the dbus daemon and retrieve infos
  QProcess dbus_daemon;
  dbus_daemon.setProcessChannelMode(QProcess::MergedChannels);
  dbus_daemon.start(QApplication::applicationDirPath() + "/dbus-launch");
  // Retrieve session bus address and update environment variables
  dbus_daemon.waitForReadyRead(-1);
  QRegExp sessionBusAddressRegexp("^DBUS_SESSION_BUS_ADDRESS\\=(unix\\:[^\\n\\r]*).*");

  if (sessionBusAddressRegexp.exactMatch(dbus_daemon.readLine()))
    setenv("DBUS_SESSION_BUS_ADDRESS",sessionBusAddressRegexp.cap(1).toStdString().c_str(),1);

  // Retrieve dbus_daemon PID to be able to kill it when application ends.
  QRegExp dbusPidRegexp("DBUS_SESSION_BUS_PID\\=([0-9]*).*");
  pid_t dbusPid = 0;

  if (dbusPidRegexp.exactMatch(dbus_daemon.readLine()))
    dbusPid = dbusPidRegexp.cap(1).toLong();
}
#endif

void checkTulipRunning() {
  // There can be only one tulip_agent running at a time on the same system.
  // To ensure that no agent is already running, we check the existence of an org.labri.Tulip service on the session bus.
  if (QDBusConnection::sessionBus().interface()->isServiceRegistered("org.labri.Tulip").value()) {
    qWarning("Tulip agent is already running. Note that you can only have one at a time on your system. Please check your system tray to display the tulip agent window.");
    QMessageBox::critical(0,QObject::trUtf8("Error when starting Tulip"),
                          QObject::trUtf8("Tulip agent is already running. Note that you can only have one at a time on your system. Please check your system tray to display the tulip agent window."));
    exit(1);
  }
}

void loadPlugins() {

}

int main(int argc, char **argv) {
  QApplication tulip_agent(argc, argv);
  tulip_agent.setApplicationName(QObject::trUtf8("Tulip"));
  QLocale::setDefault(QLocale(QLocale::English));

  // revert previous remote locations
  foreach(const QString& remoteLocation, TulipSettings::instance().remoteLocations())
    tlp::PluginManager::addRemoteLocation(remoteLocation);

  updatePlateform();

#if defined(__APPLE__)
  appleInit()
#endif

  checkTulipRunning();

#if defined(__APPLE__) // revert current directory
  QDir::setCurrent(currentPath);
#endif

  tlp::initTulipLib(QApplication::applicationDirPath().toStdString().c_str());
  //TODO find a cleaner way to achieve this (QDesktopServices is part of QtGui, so it does not belong in TlpTools)
  tlp::TulipPluginsPath = QString(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/plugins/lib/tulip/").toStdString() + tlp::PATH_DELIMITER + tlp::TulipPluginsPath;


  // Load plugins
  PluginLoaderDispatcher *dispatcher = new PluginLoaderDispatcher();
  TulipSplashScreen *splashScreen = new TulipSplashScreen();
  PluginLoaderReporter *errorReport = new PluginLoaderReporter();
  dispatcher->registerLoader(errorReport);
  dispatcher->registerLoader(splashScreen);
  tlp::PluginLibraryLoader::loadPlugins(dispatcher);
  tlp::PluginListerInterface::checkLoadedPluginsDependencies(dispatcher);
  delete dispatcher;
  delete splashScreen;


  // Main window
  TulipMainWindow *mainWindow = TulipMainWindow::instance();
  mainWindow->pluginsCenter()->reportPluginErrors(errorReport->errors());
  delete errorReport;

  // Register D-Bus service
  new TulipAgentServiceAdaptor(mainWindow);
  bool dbusRegisterServiceOk = QDBusConnection::sessionBus().registerService("org.labri.Tulip");
  bool dbusRegisterObjectOk = QDBusConnection::sessionBus().registerObject("/",mainWindow);

  if (!dbusRegisterServiceOk)
    qWarning() << "D-Bus registration of service org.labri.Tulip failed.";
  if (!dbusRegisterObjectOk)
    qWarning() << "D-Bus registration of object / over service org.labri.Tulip failed.";

  mainWindow->show();
  int result = tulip_agent.exec();

#if defined(__APPLE__)
  // manually killing D-Bus process
  qWarning() << "Terminating D-Bus at PID: " << dbusPid;
  kill(dbusPid,SIGKILL);
#endif

#ifdef MEMORYCHECKER_ON
  memory_checker_print_report();
#endif // MEMORYCHECKER_ON

  return result;
}
