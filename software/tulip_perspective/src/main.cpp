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
#include <tulip/InteractorManager.h>

#include <iostream>
using namespace std;
using namespace tlp;

void usage(const QString &error) {
  int returnCode = 0;
  if (!error.isEmpty()) {
    cerr << "Error: " << error.toStdString() << endl;
    returnCode = 1;
  }
  cout << "Usage: tulip_perspective [OPTION] [FILE]" << endl
       << "Run a Tulip Perspective plugin into its dedicated process." << endl
       << "If Tulip main process is already running, embedded perspective will run into managed mode." << endl << endl
       << "FILE: a Tulip project file. If a file is specified, the --perspective flag will be ignored and tulip_perspective will look into the project's meta-informations to find the correct perspective to launch." << endl
       << "List of OPTIONS:" << endl
       << "  --perspective=<perspective_name>\tWill use the perspective specified by perspective_name. Perspective will be run with no project file. If a project file has been specified using the FILE option, this flag will be ignored." << endl
       << "  --help\tDisplays this help message and ignores other options." << endl;
  exit(returnCode);
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

  TulipAgentCommunicator *communicator = new TulipAgentCommunicator("org.labri.Tulip","/",QDBusConnection::sessionBus(),0);

  if (!(QDBusConnection::sessionBus().interface()->isServiceRegistered("org.labri.Tulip").value()))
    qWarning("No org.labri.Tulip service on session bus. Falling back to headless mode.");

#if defined(__APPLE__) // revert current directory
  QDir::setCurrent(currentPath);
#endif

  // Init Tulip and load plugins
  tlp::initTulipLib(QApplication::applicationDirPath().toStdString().c_str());
  tlp::PluginLibraryLoader::loadPlugins();
  tlp::PluginListerInterface::checkLoadedPluginsDependencies(0);
  tlp::InteractorManager::getInst().loadInteractorPlugins();

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
    else if (a == "--help")
      usage("");
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

    if (!project->isValid())
      usage("Failed to open project file " + projectFilePath + ": " + project->lastError());

    perspectiveName = project->perspective();
    delete progress;
  }

  // Initialize main window.
  QMainWindow *mainWindow = new QMainWindow();
  mainWindow->setVisible(false);

  // Create perspective
  PerspectiveContext context;
  context.mainWindow = mainWindow;

  Perspective *perspective = StaticPluginLister<Perspective,PerspectiveContext>::getPluginObject(perspectiveName.toStdString(), context);
  if (!perspective)
    usage("Failed to create perspective: " + perspectiveName);

  // Connect perspective and communicator
  QObject::connect(perspective,SIGNAL(showTulipWelcomeScreen()),communicator,SLOT(ShowWelcomeScreen()));
  QObject::connect(perspective,SIGNAL(showTulipPluginsCenter()),communicator,SLOT(ShowPluginsCenter()));
  QObject::connect(perspective,SIGNAL(showTulipAboutPage()),communicator,SLOT(ShowAboutPage()));
  QObject::connect(perspective,SIGNAL(openProject(QString)),communicator,SLOT(OpenProject(QString)));
  QObject::connect(perspective,SIGNAL(openProjectWith(QString,QString)),communicator,SLOT(OpenProjectWith(QString,QString)));
  QObject::connect(perspective,SIGNAL(createPerspective(QString)),communicator,SLOT(CreatePerspective(QString)));
  QObject::connect(perspective,SIGNAL(addPluginRepository(QString)),communicator,SLOT(AddPluginRepository(QString)));
  QObject::connect(perspective,SIGNAL(removePluginRepository(QString)),communicator,SLOT(RemovePluginRepository(QString)));

  if (project)
    perspective->construct(project);
  else
    perspective->construct();

  return tulip_perspective.exec();
}
