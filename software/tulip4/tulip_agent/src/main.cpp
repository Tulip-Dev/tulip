#include <QtCore/QLocale>
#include <QtGui/QApplication>

#include <tulip/PluginLister.h>
#include <tulip/PluginLibraryLoader.h>
#include <tulip/TlpTools.h>

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

  mainWindow->startApp();
  return tulip_agent.exec();
}
