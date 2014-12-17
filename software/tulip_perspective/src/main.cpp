/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#include <QString>
#include <QDir>
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif

#include <CrashHandling.h>

#include <tulip/TulipException.h>
#include <tulip/TulipRelease.h>
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
#include <tulip/TlpQtTools.h>
#include <tulip/TulipSettings.h>
#include <tulip/WorkspacePanel.h>
#include <tulip/View.h>
#include <tulip/GlMainView.h>
#include <tulip/GlMainWidget.h>

#include "TulipPerspectiveMainWindow.h"

#include <iostream>

#ifdef WIN32
#include <windows.h>
#endif

#ifdef interface
#undef interface
#endif

using namespace std;
using namespace tlp;

struct PluginLoaderToProgress: public PluginLoader {
  PluginProgress* _progress;

  int max_step;
  int step;

  virtual void start(const std::string &path) {
    step=0;
    _progress->setComment("Entering " + path);
  }

  virtual void finished(bool state,const std::string &msg) {
    if (state)
      _progress->setComment("Plugin successfully loaded");
    else
      _progress->setComment(msg);
  }

  virtual void numberOfFiles(int n) {
    max_step = n;
  }

  virtual void loading(const std::string &filename) {
    step++;
    _progress->progress(step,max_step);
    _progress->setComment("Loading " + filename);
  }

  virtual void loaded(const tlp::Plugin*, const std::list <tlp::Dependency>&) {}
  virtual void aborted(const std::string& fileName,const  std::string& errorMsg) {
    std::cerr << "[Warning] Failed to import " << fileName << ": " << errorMsg << std::endl;
  }
};

void usage(const QString &error) {
  int returnCode = 0;

  if (!error.isEmpty()) {
    QMessageBox::warning(0,"Error",error);
    returnCode = 1;
  }

  cout << "Usage: tulip_perspective [OPTION] [FILE]" << endl
       << "Run a Tulip Perspective plugin into its dedicated process." << endl
       << "If Tulip main process is already running, embedded perspective will run into managed mode." << endl << endl
       << "FILE: a Tulip project file to open. The perspective to use will be read from the project meta-information. If the \"--perspective\" flag is used, tulip_perspective will try to open the file with the given perspective (the project meta-information are ignored)." << endl
       << "List of OPTIONS:" << endl
       << "  --perspective=<perspective_name> (-p perspective_name)\tStart the perspective specified by perspective_name." << endl
       << "  --geometry=<X,Y,width,height>\tSets the given rectangle as geometry for the main window." << endl
       << "  --title=<title>\tDisplay a specific name in the loading dialog." << endl
       << "  --icon=<relative path>\tChoose the icon in the loading dialog by providing a path relative to Tulip bitmap directory." << endl
       << "  --help (-h)\tDisplays this help message and ignores other options." << endl << endl
       << "Available perspectives:" << endl;
  list<string> perspectives = PluginLister::instance()->availablePlugins<Perspective>();

  for(list<string>::const_iterator it=perspectives.begin(); it!=perspectives.end(); ++it) {
    cout << *it;

    if((*it)!=perspectives.back())
      cout << ", ";
  }

  cerr << endl;

  exit(returnCode);
}

int main(int argc,char **argv) {
  start_crash_handler();

  QString appName("Tulip ");
  QString iconPath;

  // show patch number only if needed
  if (TULIP_INT_VERSION % 10)
    appName += TULIP_VERSION;
  else
    appName += TULIP_MM_VERSION;

  QApplication tulip_perspective(argc, argv);
  // the applicationName below is used to identify the location
  // of downloaded plugins, so it must be the same as in
  // tulip/main.cpp
  tulip_perspective.setApplicationName(appName);

#if defined(__APPLE__)
  // allows to load qt imageformats plugin
  QApplication::addLibraryPath(QApplication::applicationDirPath() + "/..");
#endif

  // Check arguments
  QString perspectiveName,projectFilePath;
  QVariantMap extraParams;
  QRect windowGeometry;
  QString title = appName;
  PerspectiveContext* context = new PerspectiveContext();

  QRegExp perspectiveRegexp("^\\-\\-perspective=(.*)");
  QRegExp pRegexp("^\\-p");
  QRegExp titleRegexp("^\\-\\-title=(.*)");
  QRegExp iconRegexp("^\\-\\-icon=(.*)");
  QRegExp portRegexp("^\\-\\-port=([0-9]*)");
  QRegExp idRegexp("^\\-\\-id=([0-9]*)");
  QRegExp geometryRegexp("^\\-\\-geometry=([0-9]*)\\,([0-9]*)\\,([0-9]*)\\,([0-9]*)");
  QRegExp extraParametersRegexp("^\\-\\-([^=]*)=(.*)");

  QStringList args = QApplication::arguments();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
  QString dumpPath = QDir(QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0)).filePath("tulip_perspective-0.log");
#else
  QString dumpPath = QDir(QDesktopServices::storageLocation(QDesktopServices::TempLocation)).filePath("tulip_perspective-0.log");
#endif
  setDumpPath(dumpPath.toStdString());

  for(int i=1; i < args.size(); ++i) {
    QString a = args[i];

    if ((a == "--help")||(a=="-h")) {
      usage("");
    }
    else if (perspectiveRegexp.exactMatch(a)) {
      perspectiveName = perspectiveRegexp.cap(1);
    }
    else if(pRegexp.exactMatch(a)) {
      perspectiveName = args[++i];
    }
    else if (titleRegexp.exactMatch(a)) {
      title = titleRegexp.cap(1);
    }
    else if (iconRegexp.exactMatch(a)) {
      iconPath = iconRegexp.cap(1);
    }
    else if (geometryRegexp.exactMatch(a)) {
      windowGeometry = QRect(geometryRegexp.cap(1).toInt(),geometryRegexp.cap(2).toInt(),geometryRegexp.cap(3).toInt(),geometryRegexp.cap(4).toInt());
    }
    else if (portRegexp.exactMatch(a)) {
      context->tulipPort = portRegexp.cap(1).toUInt();
    }
    else if (idRegexp.exactMatch(a)) {
      context->id = idRegexp.cap(1).toUInt();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
      QString dumpPath = QDir(QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0)).filePath("tulip_perspective-" + idRegexp.cap(1) + ".log");
#else
      QString dumpPath = QDir(QDesktopServices::storageLocation(QDesktopServices::TempLocation)).filePath("tulip_perspective-" + idRegexp.cap(1) + ".log");
#endif
      setDumpPath(dumpPath.toStdString());
    }
    else if(extraParametersRegexp.exactMatch(a)) {
      extraParams[extraParametersRegexp.cap(1)] = extraParametersRegexp.cap(2);
    }
    else {
      projectFilePath = a;
    }
  }

  // Create perspective's window
  TulipPerspectiveProcessMainWindow *mainWindow = new TulipPerspectiveProcessMainWindow(title);
  mainWindow->setVisible(false);

  // Progress bar dialog
  SimplePluginProgressDialog *progress = new SimplePluginProgressDialog(mainWindow);
  progress->setStopButtonVisible(false);
  progress->setCancelButtonVisible(false);
  progress->showPreview(false);
  progress->resize(500,progress->height());
  progress->setComment(QString("Initializing ") + title);
  progress->setWindowTitle(title);
  progress->progress(0,100);



  initTulipLib(QApplication::applicationDirPath().toUtf8().data());
  QIcon icon = progress->windowIcon();

  if (! iconPath.isEmpty()) {
    QString iconFullPath = QString::fromUtf8(TulipBitmapDir.c_str()) + iconPath;
    QIcon tmp(iconFullPath);

    if (tmp.pixmap(QSize(16,16)).isNull() == false)
      icon = tmp;
    else
      usage("Could not load icon : " + iconFullPath);
  }

  progress->setWindowIcon(icon);
  progress->show();

  TulipProject *project = NULL;
  QString error;

  if(!projectFilePath.isEmpty() && !QFileInfo(projectFilePath).exists()) {
    usage("File "+projectFilePath+" not found");
  }

  if (!projectFilePath.isEmpty() && projectFilePath.endsWith(".tlpx")) {
    project = TulipProject::openProject(projectFilePath,progress);

    if (!project->isValid()) {
      error = project->lastError();
      delete project;
      project = NULL;
    }
  }

  if (project == NULL) {
    context->externalFile = projectFilePath;
    project = TulipProject::newProject();
  }
  else if (perspectiveName.isEmpty()) {
    perspectiveName = project->perspective();

    if(perspectiveName.isEmpty())
      error = "No perspective given on the command line or in the project file";
  }

  if (perspectiveName.isEmpty()) {
    usage("Could not determine the perspective to launch: \n"+ error);
  }

  context->project = project;
  context->parameters = extraParams;
  project->setPerspective(perspectiveName);


  // Init tulip
  PluginLoaderToProgress* loader = new PluginLoaderToProgress();
  loader->_progress = progress;

  try {
    tlp::initTulipSoftware(loader);
  }
  catch(tlp::TulipException& e) {
    QMessageBox::warning(0,"Error", e.what());
    exit(1);
  }

  delete loader;

  // Initialize main window.
  progress->progress(100,100);
  progress->setComment("Setting up GUI (this can take some time)");
  context->mainWindow = mainWindow;

  // Create perspective object
  Perspective *perspective = PluginLister::instance()->getPluginObject<Perspective>(perspectiveName.toStdString(), context);

  if (perspective==NULL) {
    usage("Cannot open perspective: " + perspectiveName + "\nWrong plugin type or plugin not found.");
  }

  Perspective::setInstance(perspective);
  mainWindow->setProject(project);

  perspective->start(progress);

  mainWindow->projectFileChanged(projectFilePath);

  delete progress;

  mainWindow->setWindowIcon(icon);
  mainWindow->show();

  // the delay of geometry update until perspective execution
  // seems to ensure that the four parameters (x,y,w,h)
  // are taken into account
  if (windowGeometry.isValid())
    mainWindow->setGeometry(windowGeometry);

  int result = tulip_perspective.exec();
  delete perspective;
  delete mainWindow;

  // We need to clear allocated Qt buffers and QGlWidget to remove a segfault when we close tulip
  QGlBufferManager::clearBuffers();
  GlMainWidget::clearFirstQGLWidget();


#ifdef MEMORYCHECKER_ON
  memory_checker_print_report();
#endif // MEMORYCHECKER_ON
  return result;
}
