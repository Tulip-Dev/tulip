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

#ifndef PYTHONSCRIPTVIEW_H_
#define PYTHONSCRIPTVIEW_H_

#include <QObject>
#include <QMap>
#include <tulip/ViewWidget.h>

class QTabWidget;

namespace tlp {
class PythonInterpreter;
class PythonCodeEditor;
}

class PythonScriptViewWidget;

class PythonScriptView : public tlp::ViewWidget {

  Q_OBJECT

  PythonScriptViewWidget *_viewWidget;
  tlp::PythonInterpreter *_pythonInterpreter;
  tlp::Graph *_graph;

  bool _scriptStopped;
  bool _runningScript;

public :

  PLUGININFORMATION("Python Script view", "Antoine Lambert", "04/2010",
                    "<p>This view aims to manipulate and modify a Tulip graph through the use of Python scripts."
                    "Bindings for the Tulip-Core C++ library have been developed in order to offer its features "
                    "to the Python world. The main functionalities have been wrapped, the most important ones being "
                    "graph and properties manipulation (for instance : adding nodes/edges to a graph, creating a property, "
                    "getting/setting property values associated to graph elements, ...) but also the ability to call "
                    "algorithms.</p>"
                    "<p>The view allows to write a script that will operate on the graphs currently loaded in Tulip."
                    "Its graphical interface contains the following components :<ul>"
                    "   <li>a nice script editor with syntax highlighting and auto-completion</li>"
                    "   <li>some buttons to start / stop the current edited script</li>"
                    "   <li>a console output widget</li></ul>"
                    , "0.7", "")

  PythonScriptView(tlp::PluginContext *);
  ~PythonScriptView();

  std::string icon() const {
    return ":/icons/python.png";
  }

  void setupWidget();

  void graphChanged(tlp::Graph *);

  void setState(const tlp::DataSet&);

  tlp::DataSet state() const;

  tlp::Graph* getGraph() {
    return _graph;
  }

  bool eventFilter(QObject *, QEvent *);

  bool isRunningScript() const {
    return _runningScript;
  }

  void draw() {}

  void graphDeleted(tlp::Graph* parent) {
    _graph = NULL;
    ViewWidget::graphDeleted(parent);
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
  void saveModule();
  void checkErrors(bool clear=true);

private :

  bool loadScript(const QString &fileName, bool clear=true);
  void saveScript(int tabIdx, bool clear=true);
  bool loadModule(const QString &fileName, bool clear=true);
  bool loadModuleFromSrcCode(const QString &moduleName, const QString &moduleSrcCode);

  void saveModule(int tabIdx, const bool reload=false);
  void saveAllModules();
  bool reloadAllModules() const;
  void indicateErrors() const;
  void clearErrorIndicators() const;
  QString findFile(const QString &filePath);

};

#endif /* PYTHONSCRIPTVIEW_H_ */
