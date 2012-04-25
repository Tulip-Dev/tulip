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
#include <tulip/AbstractView.h>

class PythonInterpreter;

namespace tlp {
class SGHierarchyWidget;
}


/*@{*/
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

class PythonScriptView : public tlp::AbstractView {

  Q_OBJECT

public :

  PythonScriptView();
  ~PythonScriptView();

  QWidget *construct(QWidget *parent);

  void setData(tlp::Graph *graph, tlp::DataSet dataSet);

  void getData(tlp::Graph **graph, tlp::DataSet *dataSet);

  tlp::Graph* getGraph() {
    return graph;
  }
  void createPicture(const std::string&, int, int) {}
  void setGraph(tlp::Graph* graph) {
    setData(graph, tlp::DataSet());
  }
  void draw() {}
  void refresh() {}
  void init() {}

  bool eventFilter(QObject *obj, QEvent *event);

  bool isRunningScript() const {
    return runningScript;
  }

public slots:

  void pauseCurrentScript();

private slots :

  void newScript();
  void loadScript();
  void saveScript();
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
  void saveImportAllScripts();
  bool loadModule(const QString &fileName, bool clear = true);
  bool loadModuleFromSrcCode(const std::string &moduleName, const std::string &moduleSrcCode);
  void saveModule();
  void saveModule(int tabIdx, const bool reload=false);
  bool loadPythonPlugin(const QString &fileName, bool clear = true);
  bool loadPythonPluginFromSrcCode(const std::string &moduleName, const std::string &pluginSrcCode, bool clear=true);
  void savePythonPlugin(int tabIdx);
  void saveAllModules();
  bool reloadAllModules();
  void indicateErrors();
  void clearErrorIndicators();
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

  tlp::SGHierarchyWidget *clusterTreeWidget;
  bool scriptStopped;
  bool runningScript;

  bool dontTreatFocusIn;

};

#endif /* PYTHONSCRIPTVIEW_H_ */
