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
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <QtGui/QMessageBox>
#include <QtGui/QDesktopServices>

#include <CrashHandling.h>

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
      _progress->setComment("Plugin loaded sucessfully");
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
       << "FILE: a Tulip project file. If a file is specified, the --perspective flag will be ignored and tulip_perspective will look into the project's meta-informations to find the correct perspective to launch." << endl
       << "List of OPTIONS:" << endl
       << "  --perspective=<perspective_name>\tWill use the perspective specified by perspective_name. Perspective will be run with no project file. If a project file has been specified using the FILE option, this flag will be ignored." << endl
       << "  --geometry=<X,Y,width,height>\tSets the given rectangle as geometry for the main window." << endl
       << "  --help\tDisplays this help message and ignores other options." << endl;
  exit(returnCode);
}

int main(int argc,char **argv) {
  start_crash_handler();

  QString title("Tulip ");
  // show patch number only if needed
  if (TULIP_INT_RELEASE % 10)
    title += TULIP_RELEASE;
  else
    title += TULIP_MM_RELEASE;

  QApplication tulip_perspective(argc, argv);
  tulip_perspective.setApplicationName(title);

  // Create perspective's window
  TulipPerspectiveProcessMainWindow *mainWindow = new TulipPerspectiveProcessMainWindow();
  mainWindow->setVisible(false);

  // Progress bar dialog
  SimplePluginProgressDialog *progress = new SimplePluginProgressDialog(mainWindow);
  progress->setStopButtonVisible(false);
  progress->setCancelButtonVisible(false);
  progress->showPreview(false);
  progress->setWindowTitle(title);
  progress->resize(500,progress->height());
  progress->show();

  // Init tulip
  progress->progress(0,100);
  progress->setComment(QObject::trUtf8("Initializing tulip"));
  PluginLoaderToProgress* loader = new PluginLoaderToProgress();
  loader->_progress = progress;
  tlp::initTulipSoftware(loader);
  delete loader;

  // Check arguments
  progress->progress(60,100);
  progress->setComment(QObject::trUtf8("Cheking arguments").toStdString());
  QString perspectiveName,projectFilePath;
  QVariantMap extraParams;
  QRect windowGeometry;
  PerspectiveContext* context = new PerspectiveContext();

  QRegExp perspectiveRegexp("^\\-\\-perspective=(.*)");
  QRegExp portRegexp("^\\-\\-port=([0-9]*)");
  QRegExp idRegexp("^\\-\\-id=([0-9]*)");
  QRegExp geometryRegexp("^\\-\\-geometry=([0-9]*)\\,([0-9]*)\\,([0-9]*)\\,([0-9]*)");
  QRegExp extraParametersRegexp("^\\-\\-([^=]*)=(.*)");

  QStringList args = QApplication::arguments();

  QString dumpPath = QDir(QDesktopServices::storageLocation(QDesktopServices::TempLocation)).filePath("tulip_perspective-0.log");
  setDumpPath(dumpPath.toStdString());

  for(int i=1; i < args.size(); ++i) {
    QString a = args[i];

    if (perspectiveRegexp.exactMatch(a)) {
      perspectiveName = perspectiveRegexp.cap(1);
    }
    else if (geometryRegexp.exactMatch(a)) {
      windowGeometry = QRect(geometryRegexp.cap(1).toInt(),geometryRegexp.cap(2).toInt(),geometryRegexp.cap(3).toInt(),geometryRegexp.cap(4).toInt());
    }
    else if (portRegexp.exactMatch(a)) {
      context->tulipPort = portRegexp.cap(1).toUInt();
    }
    else if (idRegexp.exactMatch(a)) {
      context->id = idRegexp.cap(1).toUInt();
      QString dumpPath = QDir(QDesktopServices::storageLocation(QDesktopServices::TempLocation)).filePath("tulip_perspective-" + idRegexp.cap(1) + ".log");
      setDumpPath(dumpPath.toStdString());
    }
    else if (a == "--help") {
      usage("");
    }
    else if(extraParametersRegexp.exactMatch(a)) {
      extraParams[extraParametersRegexp.cap(1)] = extraParametersRegexp.cap(2);
    }
    else if (projectFilePath.isNull()) {
      projectFilePath = a;
    }
  }

  TulipProject *project = NULL;
  QString error;

  if (!projectFilePath.isNull() && projectFilePath.endsWith(".tlpx")) {
    project = TulipProject::openProject(projectFilePath,progress);

    if (!project->isValid()) {
      error = project->lastError();
      std::cerr << error.toStdString() << std::endl;
      delete project;
      project = NULL;
    }
  }


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

  title += QString(" [") + perspectiveName + "]";

  if (project) {
    if (!project->name().isNull())
      title += QString(" - ") + project->name();
    else if (!projectFilePath.isEmpty())
      title += QString(" - ") + projectFilePath;
  }

#ifndef NDEBUG
  title += " - [ Debug mode ]";
#endif

  perspective->start(progress);

  mainWindow->setWindowTitle(title);

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
