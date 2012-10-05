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

#ifndef PYTHONSCRIPTVIEW_H_
#define PYTHONSCRIPTVIEW_H_

#include "PythonScriptViewWidget.h"

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <tulip/ViewWidget.h>

class PythonInterpreter;


/** \file
 *  \brief Python script view
 *
 *  This view aims to manipulate and modify a Tulip graph through the use of Python scripts.
 *  Indeed, bindings for the Tulip C++ library have been developed in order to offer its features
 *  to the Python world. The main functionalities have been wrapped, the most important ones being
 *  graph and properties manipulation (for instance : adding nodes/edges to a graph, creating a property,
 *  getting/setting property values associated to graph elements, ...) but also the ability to call
 *  algorithms.
 *
 *  The view allows to write a script that will operate on the graph currently loaded in Tulip.
 *  Its graphical interface contains the following components :
 *    - a nice script editor with syntax highlighting and auto-completion
 *      - some buttons to start / stop the current edited script
 *      - a console output widget
 *
 *  Even if the scripting feature works quite well, the bindings are still in development so crashes
 *  could occur if one write a script that unfortunately corrupts the graph data model.
 */

class PythonScriptView : public tlp::ViewWidget {

  Q_OBJECT

public :

  PLUGININFORMATIONS("Python Script view", "Antoine Lambert", "04/2010", "Python Script View", "0.7", "")

  PythonScriptView(tlp::PluginContext *);
  ~PythonScriptView();

  void setupWidget();

  void graphChanged(tlp::Graph *graph);

  void setState(const tlp::DataSet&);

  tlp::DataSet state() const;

  tlp::Graph* getGraph() {
    return graph;
  }

  bool eventFilter(QObject *obj, QEvent *event);

  bool isRunningScript() const {
    return runningScript;
  }

  void draw(tlp::PluginProgress *) {}

  void graphDeleted() {
    this->graph = NULL;
  }

public slots:

  void pauseCurrentScript();

private slots :

  void newScript();
  void loadScript();
  void saveScript();
  void saveImportAllScripts();
  void executeCurrentScript();
  void stopCurrentScript();
  void newStringModule();
  void newFileModule();
  void loadModule();
  void saveModuleToFile();
  void newPythonPlugin();
  void loadPythonPlugin();
  void savePythonPlugin();

  void registerPythonPlugin(bool clear=true);

  void closeMainScriptTabRequested(int tab);
  void closeModuleTabRequested(int tab);
  void closePluginTabRequested(int tab);

private :

  bool loadScript(const QString &fileName, bool clear=true);
  void saveScript(int tabIdx, bool clear=true);
  bool loadModule(const QString &fileName, bool clear=true);
  bool loadModuleFromSrcCode(const std::string &moduleName, const std::string &moduleSrcCode);
  void saveModule();
  void saveModule(int tabIdx, const bool reload=false) ;
  bool loadPythonPlugin(const QString &fileName, bool clear=true);
  bool loadPythonPluginFromSrcCode(const std::string &moduleName, const std::string &pluginSrcCode, bool clear=true);
  void savePythonPlugin(int tabIdx) ;
  void saveAllModules();
  bool reloadAllModules() const;
  void indicateErrors() const;
  void clearErrorIndicators() const;
  bool checkAndGetPluginInfosFromSrcCode(const QString &pluginSrcCode, QString &pluginName, QString &pluginClassName, QString &pluginType, QString &pluginClass);
  void reloadCodeInEditorIfNeeded(PythonCodeEditor *codeEditor, QTabWidget *tabWidget, int index);
  QString findFile(const QString &filePath);

  PythonScriptViewWidget *viewWidget;
  PythonInterpreter *pythonInterpreter;
  tlp::Graph *graph;

  std::map<std::string, std::string> editedPluginsClassName;
  std::map<std::string, std::string> editedPluginsType;
  std::map<std::string, std::string> editedPluginsName;
  std::map<QString, QDateTime> lastModifiedFile;

  bool scriptStopped;
  bool runningScript;

  bool dontTreatFocusIn;

};

#endif /* PYTHONSCRIPTVIEW_H_ */
