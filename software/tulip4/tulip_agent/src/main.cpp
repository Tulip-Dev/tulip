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

int main(int argc, char **argv) {
  QApplication tulip_agent(argc, argv);
  tulip_agent.setApplicationName(QObject::trUtf8("Tulip Agent"));
  QLocale::setDefault(QLocale(QLocale::English));

#if defined(__APPLE__) // allows to load qt imageformats plugin
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/..");
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

  return tulip_agent.exec();
}
