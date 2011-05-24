#include <QtGui/QApplication>
#include <QtCore/QLocale>

#include <tulip/TlpTools.h>
#include <tulip/PluginLister.h>

#include "TulipSplashScreen.h"
#include "TulipMainWindow.h"

#include <tulip/TulipProject.h>
#include <iostream>
using namespace std;

int main(int argc, char **argv) {
  QApplication tulip_app(argc, argv);
  tulip_app.setApplicationName("tulip");
  QLocale::setDefault(QLocale(QLocale::English));

#if defined(__APPLE__) // allows to load qt imageformats plugin
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/..");
#endif

  // Initialize tulip libraries and load plugins
  tlp::initTulipLib(QApplication::applicationDirPath().toStdString().c_str());
  TulipSplashScreen *splashScreen = new TulipSplashScreen();
  splashScreen->show();

  tlp::PluginLibraryLoader::loadPlugins(splashScreen); // library side plugins
  tlp::PluginLibraryLoader::loadPlugins(splashScreen, "/interactors"); // interactors plugins
  tlp::PluginLibraryLoader::loadPlugins(splashScreen, "/glyphs"); // glyphs plugins
  tlp::PluginLibraryLoader::loadPlugins(splashScreen, "/view"); // view plugins
  tlp::PluginLibraryLoader::loadPlugins(splashScreen, "/controller"); // controller plugins

  tlp::PluginListerInterface::checkLoadedPluginsDependencies(splashScreen);

  QMap<QString,QString> abortedPlugins = splashScreen->abortedPlugins();
  delete splashScreen;

  TulipMainWindow *mainWindow = new TulipMainWindow;
  mainWindow->startApp();
  mainWindow->setPluginsErrors(abortedPlugins);

  return tulip_app.exec();
}
