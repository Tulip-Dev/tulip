#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QtGui/QApplication>
#include <QtCore/QLocale>

#include <tulip/TlpTools.h>
#include <tulip/GlyphManager.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include <tulip/InteractorManager.h>
#include <tulip/ViewPluginsManager.h>
#include <tulip/ControllerPluginsManager.h>

#include "TulipSplashScreen.h"

int main(int argc, char **argv) {
  QApplication tulip(argc, argv);
  QLocale::setDefault(QLocale(QLocale::English));

#if defined(__APPLE__) // allows to load qt imageformats plugin
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/..");
#endif

  tlp::initTulipLib(const_cast<char *>(QApplication::applicationDirPath().toStdString().c_str()));

  tlp::TulipSplashScreen splashScreen;
  splashScreen.show();

  tlp::loadPlugins(&splashScreen);
  tlp::GlyphManager::getInst().loadPlugins(&splashScreen);
  tlp::EdgeExtremityGlyphManager::getInst().loadPlugins(&splashScreen);
  tlp::InteractorManager::getInst().loadPlugins(&splashScreen);
  tlp::ViewPluginsManager::getInst().loadPlugins(&splashScreen);
  tlp::ControllerPluginsManager::getInst().loadPlugins(&splashScreen);

//  //add local plugins installation path. This is an application behavior, not a library one, and should stay here.
//  tlp::TulipPluginsPath = tlp::PluginInfo::pluginsDirName + tlp::PATH_DELIMITER + tlp::TulipPluginsPath;

//  TulipApp *mainWindow = new TulipApp();
//  QRect screenRect = tulip.desktop()->availableGeometry();
//  if (screenRect.height() > 890) {
//    QRect wRect = mainWindow->geometry();
//    int delta = (870 - wRect.height())/2;
//    wRect.setTop(wRect.top() - delta);
//    wRect.setBottom(wRect.bottom() + delta);
//    mainWindow->setGeometry(wRect);
//  }
//  mainWindow->startTulip();


  return tulip.exec();
}
