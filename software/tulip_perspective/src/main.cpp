#include "TulipAgentCommunicator.h"

#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtGui/QApplication>
#include <QtGui/QMainWindow>

#include <tulip/PluginLibraryLoader.h>
#include <tulip/PluginLister.h>
#include <tulip/TlpTools.h>
#include <tulip/TulipProject.h>
#include <tulip/SimplePluginProgressWidget.h>
#include <tulip/PluginLister.h>
#include <tulip/Perspective.h>

#include <iostream>
using namespace std;
using namespace tlp;

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

void usage(const QString &error) {

}

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

  if (perspectiveName.isNull() && projectFilePath.isNull())
    usage("Invalid arguments");

  // Initialize project
  TulipProject *project = NULL;
  if (!projectFilePath.isNull()) {
    SimplePluginProgressWidget *progress = new SimplePluginProgressWidget;
    progress->show();
    project = TulipProject::openProject(projectFilePath,progress);
    if (!project->isValid()) {
      usage("Failed to open project file " + projectFilePath + ": " + project->lastError());
      exit(1);
    }
    perspectiveName = project->perspective();
    delete progress;
  }

  // Initialize main window.
  QMainWindow *mainWindow = new QMainWindow();
  mainWindow->setVisible(false);

  // Construct perspective
  PerspectiveContext context;
  context.mainWindow = mainWindow;
  Perspective *perspective = StaticPluginLister<Perspective,PerspectiveContext>::getPluginObject(perspectiveName.toStdString(), context);
  if (!perspective) {
    usage("Failed to create perspective: " + perspectiveName);
    exit(1);
  }

  if (project)
    perspective->construct(project);
  else
    perspective->construct();

  // Connect perspective and communicator

  return tulip_perspective.exec();
}
