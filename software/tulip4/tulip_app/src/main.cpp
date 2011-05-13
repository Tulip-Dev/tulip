#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QtGui/QApplication>
#include <QtCore/QLocale>

#include <tulip/TlpTools.h>
#include <tulip/PluginLister.h>

#include "TulipSplashScreen.h"
#include "TulipMainWindow.h"

int main(int argc, char **argv) {
  QApplication tulip(argc, argv);
  QLocale::setDefault(QLocale(QLocale::English));

#if defined(__APPLE__) // allows to load qt imageformats plugin
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/..");
#endif

  // Initialize tulip libraries and load plugins
  tlp::initTulipLib(const_cast<char *>(QApplication::applicationDirPath().toStdString().c_str()));
  TulipSplashScreen *splashScreen = new TulipSplashScreen();
  splashScreen->show();

  tlp::loadPlugins(splashScreen); // library side plugins
  tlp::loadPlugins(splashScreen, "/interactors"); // interactors plugins
  tlp::loadPlugins(splashScreen, "/glyphs"); // glyphs plugins
  tlp::loadPlugins(splashScreen, "/view"); // view plugins
  tlp::loadPlugins(splashScreen, "/controller"); // controller plugins

  tlp::PluginListerInterface::checkLoadedPluginsDependencies(splashScreen);

  QMap<QString,QString> abortedPlugins = splashScreen->abortedPlugins();
  delete splashScreen;

  TulipMainWindow *mainWindow = new TulipMainWindow;
  mainWindow->startApp();
  mainWindow->setPluginsErrors(abortedPlugins);

  return tulip.exec();
}
