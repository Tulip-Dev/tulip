#include <QtCore/QLocale>
#include <QtGui/QApplication>
#include <QtDBus/QDBusConnection>

#include <tulip/PluginLister.h>
#include <tulip/PluginLibraryLoader.h>
#include <tulip/TlpTools.h>

#include "TulipAgentServiceAdaptor.h"
#include "TulipMainWindow.h"
#include "PluginLoaderReporter.h"
#include "PluginLoaderDispatcher.h"
#include "TulipSplashScreen.h"
#include "PluginsCenter.h"

#if defined(__APPLE__)
#include <QtCore/QDir>
#include <sys/types.h>
#include <signal.h>
#endif

int main(int argc, char **argv) {
  QApplication tulip_agent(argc, argv);
  tulip_agent.setApplicationName(QObject::trUtf8("Tulip Agent"));
  QLocale::setDefault(QLocale(QLocale::English));

#if defined(__APPLE__) // allows to load qt imageformats plugin
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/..");

  // Switch the current directory to ensure that libdbus is loaded
  QString currentPath = QDir::currentPath();
  QDir::setCurrent(QApplication::applicationDirPath() + "/../Frameworks");

  // Manually run the dbus daemon
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
#endif

  // There can be only one tulip_agent running at a time on the same system.
  // To ensure that no agent is already running, we check the existence of an org.labri.Tulip service on the session bus.
  if (QDBusConnection::sessionBus().interface()->isServiceRegistered("org.labri.Tulip").value()) {
    qWarning("Tulip agent is already running. Note that you can only have one at a time on your system. Please check your system tray to display the tulip agent window.");
    exit(1);
  }

#if defined(__APPLE__) // revert current directory
  QDir::setCurrent(currentPath);
#endif

  tlp::initTulipLib(QApplication::applicationDirPath().toStdString().c_str());

  // Check dependencies
  PluginLoaderDispatcher *dispatcher = new PluginLoaderDispatcher();
  TulipSplashScreen *splashScreen = new TulipSplashScreen();
  PluginLoaderReporter *errorReport = new PluginLoaderReporter();
  dispatcher->registerLoader(errorReport);
  dispatcher->registerLoader(splashScreen);

  tlp::PluginLibraryLoader::loadPlugins(dispatcher);
  tlp::PluginListerInterface::checkLoadedPluginsDependencies(dispatcher);

  delete dispatcher;
  delete splashScreen;

  TulipMainWindow *mainWindow = new TulipMainWindow;
  QMap<QString,QString> errorsMap(errorReport->errors());
  for(QMap<QString,QString>::iterator it = errorsMap.begin(); it != errorsMap.end(); ++it)
    mainWindow->pluginsCenter()->reportPluginError(it.key(), it.value());

  delete errorReport;

  // Register D-Bus service
  new TulipAgentService(mainWindow);
  bool dbusRegisterServiceOk = QDBusConnection::sessionBus().registerService("org.labri.Tulip");
  bool dbusRegisterObjectOk = QDBusConnection::sessionBus().registerObject("/",mainWindow);
  if (!dbusRegisterServiceOk)
    qWarning() << "D-Bus registration of service org.labri.Tulip failed.";
  if (!dbusRegisterObjectOk)
    qWarning() << "D-Bus registration of object / over service org.labri.Tulip failed.";

  mainWindow->startApp();

  int result = tulip_agent.exec();
#if defined(__APPLE__) // manually kill D-Bus process
  qWarning() << "Terminating D-Bus at PID: " << dbusPid;
  kill(dbusPid,SIGKILL);
#endif
  return result;
}
