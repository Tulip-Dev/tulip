/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include "TulipAgentCommunicator.h"

#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <QtGui/QMessageBox>
#include <QtGui/QDesktopServices>

#include <tulip/PluginLibraryLoader.h>
#include <tulip/PluginLister.h>
#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>
#include <tulip/TulipProject.h>
#include <tulip/SimplePluginProgressWidget.h>
#include <tulip/PluginLister.h>
#include <tulip/Perspective.h>
#include <tulip/Interactor.h>
#include <tulip/GlyphManager.h>
#include <tulip/EdgeExtremityGlyphManager.h>
#include <tulip/QGlBufferManager.h>

#include "TulipPerspectiveMainWindow.h"

#include <CrashHandling.h>

#include <iostream>

#ifdef interface
#undef interface
#endif

using namespace std;
using namespace tlp;

#include <tulip/WorkspacePanel.h>
#include <tulip/View.h>
#include <tulip/GlMainView.h>
#include <tulip/GlMainWidget.h>
void usage(const QString &error) {
  int returnCode = 0;

  if (!error.isEmpty()) {
    QMessageBox::warning(0,"Error",error);
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
  start_crash_handler();
  QApplication tulip_perspective(argc, argv);
  tulip_perspective.setApplicationName(QObject::trUtf8("Tulip"));
  QLocale::setDefault(QLocale(QLocale::English));

  TulipPerspectiveProcessMainWindow *mainWindow = new TulipPerspectiveProcessMainWindow();
  mainWindow->setVisible(false);

  SimplePluginProgressDialog *progress = new SimplePluginProgressDialog(mainWindow);
  progress->setWindowTitle(QObject::trUtf8("Tulip"));
  progress->resize(500,progress->height());
  progress->show();

  progress->progress(0,100);
  progress->setComment(QObject::trUtf8("Initializing D-Bus").toStdString());

#if defined(__APPLE__)
  // allows to load qt imageformats plugin
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/..");
  // Switch the current directory to ensure that libdbus is loaded
  QString currentPath = QDir::currentPath();
  QDir::setCurrent(QApplication::applicationDirPath() + "/../Frameworks");
#endif

  TulipAgentCommunicator *communicator = new TulipAgentCommunicator("org.labri.Tulip","/",QDBusConnection::sessionBus(),0);

#if defined(__APPLE__) // revert current directory
  QDir::setCurrent(currentPath);
#endif

  // Init Tulip and load plugins
  progress->progress(25,100);
  progress->setComment(QObject::trUtf8("Loading plugins").toStdString());
  tlp::initTulipLib(QApplication::applicationDirPath().toUtf8().data());
  tlp::TulipPluginsPath = QString(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/plugins/lib/tulip/").toStdString() +
                          tlp::PATH_DELIMITER + tlp::TulipPluginsPath +
                          tlp::PATH_DELIMITER + tlp::getPluginLocalInstallationDir().toStdString();

  tlp::PluginLibraryLoader::loadPlugins();
  tlp::PluginLister::checkLoadedPluginsDependencies(0);
  tlp::InteractorLister::initInteractorsDependencies();
  tlp::GlyphManager::getInst().loadGlyphPlugins();
  tlp::EdgeExtremityGlyphManager::getInst().loadGlyphPlugins();

  // Check arguments
  progress->progress(60,100);
  progress->setComment(QObject::trUtf8("Cheking arguments").toStdString());
  QString perspectiveName,projectFilePath;
  QVariantMap extraParams;

  QRegExp perspectiveRegexp("^\\-\\-perspective=(.*)");
  QRegExp extraParametersRegexp("^\\-\\-([^=]*)=(.*)");
  QString a;
  int i=0;
  foreach(a,QApplication::arguments()) {
    if (i++ == 0)
      continue;

    if (perspectiveRegexp.exactMatch(a))
      perspectiveName = perspectiveRegexp.cap(1);
    else if (a == "--help")
      usage("");
    else if(extraParametersRegexp.exactMatch(a))
      extraParams[extraParametersRegexp.cap(1)] = extraParametersRegexp.cap(2);
    else if (projectFilePath.isNull())
      projectFilePath = a;
  }

  TulipProject *project = NULL;
  QString error;

  if (!projectFilePath.isNull()) {
    project = TulipProject::openProject(projectFilePath,progress);

    if (!project->isValid()) {
      error = project->lastError();
      std::cerr << error.toStdString() << std::endl;
      delete project;
      project = NULL;
    }
  }

  PerspectiveContext* context = new PerspectiveContext();

  if (project == NULL) {
    context->externalFile = projectFilePath;
    project = TulipProject::newProject();
  }
  else if (perspectiveName.isNull()) {
    perspectiveName = project->perspective();
  }

  context->project = project;
  context->parameters = extraParams;

  if (perspectiveName.isNull())
    usage("Could not determine the perspective to launch." + error);

  project->setPerspective(perspectiveName);
  // Initialize main window.
  progress->progress(100,100);
  progress->setComment("Setting up GUI (this can take some time)");
  context->mainWindow = mainWindow;

  // Create perspective object
  Perspective *perspective = PluginLister::instance()->getPluginObject<Perspective>(perspectiveName.toStdString(), context);
  Perspective::setInstance(perspective);

  if (!perspective)
    usage("Failed to create perspective: " + perspectiveName);

  mainWindow->setPerspective(perspective);

  // Connect perspective and communicator
  QObject::connect(perspective,SIGNAL(showTulipWelcomeScreen()),communicator,SLOT(ShowWelcomeScreen()));
  QObject::connect(perspective,SIGNAL(showTulipPluginsCenter()),communicator,SLOT(ShowPluginsCenter()));
  QObject::connect(perspective,SIGNAL(showTulipAboutPage()),communicator,SLOT(ShowAboutPage()));
  QObject::connect(perspective,SIGNAL(showOpenProjectWindow()),communicator,SLOT(ShowOpenProjectWindow()));
  QObject::connect(perspective,SIGNAL(openProject(QString)),communicator,SLOT(OpenProject(QString)));
  QObject::connect(perspective,SIGNAL(openProjectWith(QString,QString,const QVariantMap&)),communicator,SLOT(OpenProjectWith(QString,QString,const QVariantMap&)));
  QObject::connect(perspective,SIGNAL(createPerspective(QString,const QVariantMap&)),communicator,SLOT(CreatePerspective(QString,const QVariantMap&)));
  QObject::connect(perspective,SIGNAL(addPluginRepository(QString)),communicator,SLOT(AddPluginRepository(QString)));
  QObject::connect(perspective,SIGNAL(removePluginRepository(QString)),communicator,SLOT(RemovePluginRepository(QString)));
  QObject::connect(perspective,SIGNAL(showTrayMessage(QString,QString,uint,uint)),communicator,SLOT(ShowTrayMessage(QString,QString,uint,uint)));
  QObject::connect(communicator,SIGNAL(Terminate()),mainWindow,SLOT(close()));

  communicator->EnableCrashHandling(project->absoluteRootPath(),QApplication::applicationPid());

  QString title("Tulip [" + perspectiveName + "]");

  if (project) {
    title += " - ";

    if (!project->name().isNull())
      title += project->name();
    else
      title += projectFilePath;
  }

#ifndef NDEBUG
  title += " - [ Debug mode enabled ]";
#endif

  perspective->start(progress);
  mainWindow->setWindowTitle(title);

  int result = tulip_perspective.exec();
  delete perspective;

  // We need to clear allocated Qt buffers to remove a segfault when we close tulip
  QGlBufferManager::clearBuffers();


#ifdef MEMORYCHECKER_ON
  memory_checker_print_report();
#endif // MEMORYCHECKER_ON
  return result;
}
