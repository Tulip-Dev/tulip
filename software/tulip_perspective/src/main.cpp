#include "TulipAgentCommunicator.h"

#include <QtGui/QApplication>
#include <QtCore/QString>
#include <QtCore/QDir>

#include <tulip/PluginLibraryLoader.h>
#include <tulip/PluginLister.h>
#include <tulip/TlpTools.h>

#include <iostream>
using namespace std;

// This PluginLoader basically does nothing. It's only used to load plugins at startup.
class NoOpPluginLoader: public tlp::PluginLoader {
public:
  virtual void start(const std::string &) {}
  virtual void numberOfFiles(int) {}
  virtual void loading(const std::string &) {}
  virtual void loaded(const tlp::AbstractPluginInfo* infos, const std::list <tlp::Dependency>&) {}
  virtual void aborted(const std::string &,const  std::string &) {}
  virtual void finished(bool ,const std::string &) {}
};

int main(int argc,char **argv) {
  QApplication tulip_perspective(argc, argv);
  tulip_perspective.setApplicationName(QObject::trUtf8("Tulip"));
  QLocale::setDefault(QLocale(QLocale::English));

#if defined(__APPLE__)
  // allows to load qt imageformats plugin
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/..");
  // Switch the current directory to ensure that libdbus is loaded
  QString currentPath = QDir::currentPath();
  QDir::setCurrent(QApplication::applicationDirPath() + "/../Frameworks");
#endif

  TulipAgentCommunicator communicator("org.labri.Tulip","/",QDBusConnection::sessionBus(),0);

  bool headlessMode = QDBusConnection::sessionBus().interface()->isServiceRegistered("org.labri.Tulip").value();
  if (headlessMode)
    qWarning("no org.labri.Tulip service on session bus. Falling back to headless mode.");

#if defined(__APPLE__) // revert current directory
  QDir::setCurrent(currentPath);
#endif

  tlp::initTulipLib(QApplication::applicationDirPath().toStdString().c_str());

  // Check dependencies
  NoOpPluginLoader *loader = new NoOpPluginLoader;
  tlp::PluginLibraryLoader::loadPlugins(loader);
  tlp::PluginListerInterface::checkLoadedPluginsDependencies(loader);
  delete loader;

  // Check arguments
  QString perspectiveName,projectFilePath;
  QRegExp perspectiveRegexp("^\\-\\-perspective=(.*)");

  QString a;
  int i=0;
  foreach(a,QApplication::arguments()) {
    if (i++ == 0)
      continue;
    if (perspectiveRegexp.exactMatch(a))
      perspectiveName = perspectiveRegexp.cap(1);
    else if (projectFilePath.isNull())
      projectFilePath = a;
  }

  cout << perspectiveName.toStdString() << endl;
  cout << projectFilePath.toStdString() << endl;

  // Initialize main window.

  // Initialize project

  // Construct perspective

  // Connect perspective and communicator

  return tulip_perspective.exec();
}
