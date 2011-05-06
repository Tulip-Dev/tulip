#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QtGui/QApplication>
#include <QtCore/QLocale>

#include <tulip/TlpTools.h>
#include <tulip/GlyphManager.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include <tulip/InteractorManager.h>
#include <tulip/InteractorManager.h>
#include <tulip/ViewPluginsManager.h>
#include <tulip/ControllerPluginsManager.h>

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
  tlp::loadPlugins(splashScreen);
  tlp::GlyphManager::getInst().loadPlugins(splashScreen);
  tlp::EdgeExtremityGlyphManager::getInst().loadPlugins(splashScreen);
  tlp::InteractorManager::getInst().loadPlugins(splashScreen);
  tlp::ViewPluginsManager::getInst().loadPlugins(splashScreen);
  tlp::ControllerPluginsManager::getInst().loadPlugins(splashScreen);

  QMap<QString,QString> abortedPlugins = splashScreen->abortedPlugins();
  delete splashScreen;

  TulipMainWindow *mainWindow = new TulipMainWindow;
  mainWindow->startApp();
  mainWindow->setPluginsErrors(abortedPlugins);

  return tulip.exec();
}
