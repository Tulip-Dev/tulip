/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <QDesktopWidget>
#include <QStandardPaths>

#include <CrashHandling.h>

#include <tulip/TulipException.h>
#include <tulip/TulipRelease.h>
#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>
#include <tulip/TulipProject.h>
#include <tulip/SimplePluginProgressWidget.h>
#include <tulip/PluginLister.h>
#include <tulip/Perspective.h>
#include <tulip/TlpQtTools.h>
#include <tulip/TulipSettings.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlOffscreenRenderer.h>
#include <tulip/GlTextureManager.h>

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

struct PluginLoaderToProgress : public PluginLoader {
  PluginProgress *_progress;
  bool _debug_output;
  int max_step;
  int step;

  PluginLoaderToProgress(PluginProgress *progress = nullptr, bool debug_output = false)
      : _progress(progress), _debug_output(debug_output), max_step(0), step(0) {}

  void start(const std::string &path) override {
    step = 0;
    _progress->setComment("Entering " + path);
    if (_debug_output)
      tlp::debug() << "Entering " << path << std::endl;
  }

  void finished(bool state, const std::string &msg) override {
    if (state) {
      _progress->setComment("Plugins successfully loaded");
      if (_debug_output)
        tlp::debug() << "Plugins successfully loaded" << std::endl;
    } else {
      _progress->setComment(msg);
      if (_debug_output)
        tlp::debug() << msg << std::endl;
    }
  }

  void numberOfFiles(int n) override {
    max_step = n;
  }

  void loading(const std::string &filename) override {
    step++;
    _progress->progress(step, max_step);
    _progress->setComment("Loading " + filename);
    if (_debug_output)
      tlp::debug() << "Loading " << filename << std::endl;
  }

  void loaded(const tlp::Plugin *plugin, const std::list<tlp::Dependency> &) override {
    if (_debug_output)
      tlp::debug() << "  - Plugin '" << plugin->name() << "' registered" << std::endl;
  }

  void aborted(const std::string &fileName, const std::string &errorMsg) override {
    tlp::error() << "[Warning] Failed to load " << fileName << ": " << errorMsg << std::endl;
  }
};

void usage(const QString &error) {
  int returnCode = 0;

  if (!error.isEmpty()) {
    QMessageBox::warning(nullptr, "Error", error);
    returnCode = 1;
  }

  cout
      << "Usage: tulip_perspective [OPTION] [FILE]" << endl
      << "Run a Tulip Perspective plugin into its dedicated process." << endl
      << "If Tulip main process is already running, embedded perspective will run into managed "
         "mode."
      << endl
      << endl
      << "FILE: a Tulip project file to open. The perspective to use will be read from the project "
         "meta-information. If the \"--perspective\" flag is used, tulip_perspective will try to "
         "open the file with the given perspective (the project meta-information is ignored)."
      << endl
      << "List of OPTIONS:" << endl
      << "  --perspective=<perspective_name> (-p perspective_name)\tStart the perspective "
         "specified by perspective_name."
      << endl
      << "  --geometry=<X,Y,width,height>\tSet the given rectangle as geometry for the main window."
      << endl
      << "  --title=<title>\tDisplay a specific name in the loading dialog." << endl
      << "  --icon=<relative path>\tChoose the icon in the loading dialog by providing a path "
         "relative to Tulip bitmap directory."
      << endl
      << "  --help (-h)\tDisplay this help message and ignore other options." << endl
      << endl
      << "Other options (written as --<option_name>=<value>) will be passed to the perspective."
      << endl
      << "Available perspectives:" << endl;
  tlp::initTulipSoftware();
  list<string> perspectives = PluginLister::availablePlugins<Perspective>();

  for (auto &name : perspectives) {
    cout << " - " << name << endl;
    string usage_str("");
    auto p = PluginLister::getPluginObject<Perspective>(name);
    p->usage(usage_str);
    delete p;
    cout << "   " << usage_str << endl;
    cout << endl;
  }

  exit(returnCode);
}

SimplePluginProgressDialog *createProgress(QString &iconPath, QString &title) {
  SimplePluginProgressDialog *progress = new SimplePluginProgressDialog();
  progress->setStopButtonVisible(false);
  progress->setCancelButtonVisible(false);
  progress->showPreview(false);

  progress->resize(500, std::min(50, progress->height()));
  progress->setComment(QString("Initializing ") + title);
  progress->setWindowTitle(title);
  progress->progress(0, 100);

  QIcon icon = progress->windowIcon();

  if (!iconPath.isEmpty()) {
    QString iconFullPath = tlpStringToQString(TulipBitmapDir) + iconPath;
    QIcon tmp(iconFullPath);

    if (tmp.pixmap(QSize(16, 16)).isNull() == false)
      icon = tmp;
    else
      usage("Could not load icon: " + iconFullPath);
  }

  progress->setWindowIcon(icon);
  progress->show();

  return progress;
}

int main(int argc, char **argv) {

  CrashHandling::installCrashHandler();

  QString title("Tulip ");
  QString iconPath;

  // show patch number only if needed
  if (TULIP_INT_VERSION % 10)
    title += TULIP_VERSION;
  else
    title += TULIP_MM_VERSION;

  // Enables resource sharing between the OpenGL contexts
  QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);
  // Enables high-DPI scaling on X11 or Windows platforms
  // Enabled on MacOSX with NSHighResolutionCapable key in Info.plist file
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);

  QApplication tulip_perspective(argc, argv);
  // the applicationName below is used to identify the location
  // of downloaded plugins, so it must be the same as in
  // tulip/main.cpp
  tulip_perspective.setApplicationName(title);

  // Check arguments
  QString perspectiveName, projectFilePath;
  QVariantMap extraParams;
  QRect windowGeometry;
  PerspectiveContext *context = new PerspectiveContext();

  QRegExp perspectiveRegexp("^\\-\\-perspective=(.*)");
  QRegExp pRegexp("^\\-p");
  QRegExp titleRegexp("^\\-\\-title=(.*)");
  QRegExp iconRegexp("^\\-\\-icon=(.*)");
  QRegExp portRegexp("^\\-\\-port=([0-9]*)");
  QRegExp idRegexp("^\\-\\-id=([0-9]*)");
  QRegExp geometryRegexp("^\\-\\-geometry=([0-9]*)\\,([0-9]*)\\,([0-9]*)\\,([0-9]*)");
  QRegExp debugPluginLoadRegExp("^\\-debug_plugin_load");
  bool debugPluginLoad = false;
  QRegExp extraParametersRegexp("^\\-\\-([^=]*)=(.*)");

  QStringList args = QApplication::arguments();

  for (int i = 1; i < args.size(); ++i) {
    QString a = args[i];

    if ((a == "--help") || (a == "-h")) {
      usage("");
    } else if (perspectiveRegexp.exactMatch(a)) {
      perspectiveName = perspectiveRegexp.cap(1);
    } else if (pRegexp.exactMatch(a)) {
      perspectiveName = args[++i];
    } else if (titleRegexp.exactMatch(a)) {
      title = titleRegexp.cap(1);
    } else if (iconRegexp.exactMatch(a)) {
      iconPath = iconRegexp.cap(1);
    } else if (geometryRegexp.exactMatch(a)) {
      windowGeometry = QRect(geometryRegexp.cap(1).toInt(), geometryRegexp.cap(2).toInt(),
                             geometryRegexp.cap(3).toInt(), geometryRegexp.cap(4).toInt());
    } else if (portRegexp.exactMatch(a)) {
      context->tulipPort = portRegexp.cap(1).toUInt();
    } else if (debugPluginLoadRegExp.exactMatch(a))
      debugPluginLoad = true;
    else if (idRegexp.exactMatch(a)) {
      context->id = idRegexp.cap(1).toUInt();
      QString dumpPath = QDir(QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0))
                             .filePath("tulip_perspective-" + idRegexp.cap(1) + ".log");
      CrashHandling::setDumpPath(QStringToTlpString(dumpPath));
    } else if (extraParametersRegexp.exactMatch(a)) {
      extraParams[extraParametersRegexp.cap(1)] = extraParametersRegexp.cap(2);
    } else {
      projectFilePath = a;
    }
  }

  initTulipLib(QStringToTlpString(QApplication::applicationDirPath()).c_str());

#ifdef _MSC_VER
  // Add path to Tulip pdb files generated by Visual Studio (for configurations Debug and
  // RelWithDebInfo)
  // It allows to get a detailed stack trace when Tulip crashes.
  CrashHandling::setExtraSymbolsSearchPaths(tlp::TulipShareDir + "pdb");
#endif

  // create Progress bar dialog
  SimplePluginProgressDialog *progress = createProgress(iconPath, title);

  // Init tulip
  try {
    PluginLoaderToProgress loader(progress, debugPluginLoad);
    tlp::initTulipSoftware(&loader);
  } catch (tlp::TulipException &e) {
    QMessageBox::warning(nullptr, "Error", e.what());
    exit(1);
  }

  int result;
  bool start = true;
  while (start) {
    QFileInfo fileInfo(projectFilePath);

    if (!projectFilePath.isEmpty() && (!fileInfo.exists() || fileInfo.isDir())) {
      usage("File " + projectFilePath + " not found or is a directory");
    }

    if (!progress)
      progress = createProgress(iconPath, title);

    TulipProject *project = nullptr;
    if (!projectFilePath.isEmpty() && projectFilePath.endsWith(".tlpx")) {
      project = TulipProject::openProject(projectFilePath, progress);
      if (perspectiveName.isEmpty())
        perspectiveName = project->perspective();
    } else {
      context->externalFile = projectFilePath;
      project = TulipProject::newProject();
    }
    if (perspectiveName.isEmpty()) {
      // set Tulip as default perspective
      perspectiveName = "Tulip";
    }

    context->project = project;
    context->parameters = extraParams;
    project->setPerspective(perspectiveName);
    // Initialize main window.
    progress->progress(100, 100);
    progress->setComment("Setting up GUI (this can take some time)");
    // Create perspective's window
    TulipPerspectiveProcessMainWindow *mainWindow = new TulipPerspectiveProcessMainWindow(title);
    mainWindow->setVisible(false);

    context->mainWindow = mainWindow;

    // Create perspective object
    Perspective *perspective = PluginLister::getPluginObject<Perspective>(
        tlp::QStringToTlpString(perspectiveName), context);

    if (perspective == nullptr) {
      usage("Cannot open perspective: " + perspectiveName +
            "\nWrong plugin type or plugin not found.");
    }

    Perspective::setInstance(perspective);
    mainWindow->setProject(project);

    perspective->start(progress);

    mainWindow->projectFileChanged(projectFilePath);

    mainWindow->setWindowIcon(progress->windowIcon());
    mainWindow->show();

    delete progress;
    progress = nullptr;

    // the delay of geometry update until perspective execution
    // seems to ensure that the four parameters (x,y,w,h)
    // are taken into account
    if (windowGeometry.isValid()) {
      mainWindow->setGeometry(windowGeometry);
    } else {
      mainWindow->move(0, 0);
      mainWindow->resize(QDesktopWidget().availableGeometry(mainWindow).size() * 0.9);
    }

    TulipSettings::setFirstRun(false);
    TulipSettings::setFirstTulipMMRun(false);
    result = tulip_perspective.exec();

    if ((start = perspective->needRestart())) {
      // current geometry will be restored at the next start up
      windowGeometry = mainWindow->frameGeometry();
      // and the current project file too
      projectFilePath = project->projectFile();
    }

    delete perspective;
    delete mainWindow;
  }
  delete context;

  // We need to clear allocated OpenGL resources
  // to remove a segfault when we close tulip
  GlTextureManager::deleteAllTextures();
  delete GlOffscreenRenderer::getInstance();

  return result;
}
