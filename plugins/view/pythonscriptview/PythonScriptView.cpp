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

#include "PythonInterpreter.h"
#include "PythonScriptView.h"
#include "PythonCodeEditor.h"
#include "PluginCreationDialog.h"

#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtCore/QTextStream>
#include <QtCore/QRegExp>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>

#include <tulip/Graph.h>
#include <tulip/DoubleProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/GraphProperty.h>
#include <tulip/SGHierarchyWidget.h>
#include <tulip/MainController.h>
#include <tulip/Algorithm.h>
#include <tulip/ImportModule.h>
#include <tulip/ExportModule.h>
#include <tulip/TemplateFactory.h>

#include <sstream>

using namespace tlp;
using namespace std;

const string updateVisualizationFunc =
  "import tuliputils\n"
  "\n"
  "def updateVisualization(centerViews = True):\n"
  "\ttuliputils.updateVisualization(centerViews)\n"
  "\n"
  "\n";

const string pauseScriptFunc =
  "import tuliputils\n"
  "\n"
  "def pauseScript():\n"
  "\ttuliputils.pauseRunningScript()\n"
  "\n"
  "\n";

const string runGraphScriptFunc =
  "import tuliputils\n"
  "\n"
  "def runGraphScript(scriptFile, graph):\n"
  "\ttuliputils.runGraphScript(scriptFile, graph)\n"
  "\n"
  "\n";

void strReplace(std::string& str, const std::string& oldStr, const std::string& newStr) {
  size_t pos = 0;

  while((pos = str.find(oldStr, pos)) != std::string::npos) {
    str.replace(pos, oldStr.length(), newStr);
    pos += newStr.length();
  }
}

std::string cleanPropertyName(const std::string &propertyName) {
  std::string ret(propertyName);
  std::replace(ret.begin(), ret.end(), ' ', '_');
  size_t i = 0;

  while (pythonReservedCharacters[i]) {
    ret.erase(std::remove(ret.begin(), ret.end(), pythonReservedCharacters[i++]), ret.end());
  }

  i = 0;

  while (pythonKeywords[i]) {
    if (ret == pythonKeywords[i++]) {
      ret += "_";
      break;
    }
  }

  std::string builtinModName = "__builtin__";

  if (atof(PythonInterpreter::getInstance()->getPythonVersion().c_str()) >= 3.0) {
    builtinModName = "builtins";
  }

  PythonInterpreter::getInstance()->runString(std::string("import ") + builtinModName);
  static std::vector<std::string> builtinDictContent = PythonInterpreter::getInstance()->getObjectDictEntries(builtinModName);

  for (i = 0 ; i < builtinDictContent.size() ; ++i) {
    if (ret == builtinDictContent[i]) {
      ret += "_";
      break;
    }
  }

  strReplace(ret, "\"", "");

  return ret;
}

std::string getDefaultScriptCode(const string &pythonVersion, Graph *graph) {
  ostringstream oss;

  oss << "# Powered by Python " << pythonVersion << endl << endl;
  oss << "# To cancel the modifications performed by the script" << endl;
  oss << "# on the current graph, click on the undo button." << endl << endl;

  oss << "# Some useful keyboards shortcuts : " << endl;
  oss << "#   * Ctrl + D : comment selected lines." << endl;
  oss << "#   * Ctrl + Shift + D  : uncomment selected lines." << endl;
  oss << "#   * Ctrl + I : indent selected lines." << endl;
  oss << "#   * Ctrl + Shift + I  : unindent selected lines." << endl;
  oss << "#   * Ctrl + Return  : run script." << endl;
  oss << "#   * Ctrl + F  : find selected text." << endl;
  oss << "#   * Ctrl + R  : replace selected text." << endl;
  oss << "#   * Ctrl + Space  : show auto-completion dialog." << endl << endl;

  oss << "from tulip import *" << endl << endl;;

  oss << "# the updateVisualization(centerViews = True) function can be called" << endl;
  oss << "# during script execution to update the opened views" << endl << endl;

  oss << "# the pauseScript() function can be called to pause the script execution." << endl;
  oss << "# To resume the script execution, you will have to click on the \"Run script \" button." << endl << endl;

  oss << "# the runGraphScript(scriptFile, graph) function can be called to launch another edited script on a tlp.Graph object." << endl;
  oss << "# The scriptFile parameter defines the script name to call (in the form [a-zA-Z0-9_]+.py)" << endl << endl;

  oss << "# the main(graph) function must be defined " << endl;
  oss << "# to run the script on the current graph" << endl << endl;

  oss << "def main(graph): " << endl;
  Iterator<PropertyInterface*> *itProps = graph->getObjectProperties();

  while (itProps->hasNext()) {
    PropertyInterface *prop = itProps->next();

    std::string cleanPropName = prop->getName();
    strReplace(cleanPropName, "\"", "\\\"");

    if (dynamic_cast<DoubleProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getDoubleProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<LayoutProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getLayoutProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<IntegerProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getIntegerProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<StringProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getStringProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<SizeProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getSizeProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<BooleanProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getBooleanProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<ColorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getColorProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<GraphProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getGraphProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<DoubleVectorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getDoubleVectorProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<CoordVectorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getCoordVectorProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<IntegerVectorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getIntegerVectorProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<SizeVectorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getSizeVectorProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<BooleanVectorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getBooleanVectorProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<ColorVectorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getColorVectorProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<StringVectorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getStringVectorProperty(\"" << cleanPropName << "\")" << endl;
    }
  }

  oss << "\n\tfor n in graph.getNodes():" << endl;

  if (atof(pythonVersion.c_str()) >= 3.0)
    oss << "\t\tprint(n)" << endl;
  else
    oss << "\t\tprint n" << endl;

  return oss.str();
}

QString getTulipPythonPluginSkeleton(const QString &pluginClassName, const QString &pluginType,
                                     const QString &pluginName, const QString &pluginAuthor,
                                     const QString &pluginDate, const QString &pluginInfos,
                                     const QString &pluginRelease, const QString &pluginGroup) {

  QString pluginClass;
  QString tlpType;

  if (pluginType == "General") {
    pluginClass = "tlp.Algorithm";
    tlpType = "Algorithm";
  }
  else if (pluginType == "Layout") {
    pluginClass = "tlp.LayoutAlgorithm";
    tlpType = "Layout";
  }
  else if (pluginType == "Size") {
    pluginClass = "tlp.SizeAlgorithm";
    tlpType = "Size";
  }
  else if (pluginType == "Measure") {
    pluginClass = "tlp.DoubleAlgorithm";
    tlpType = "Double";
  }
  else if (pluginType == "Color") {
    pluginClass = "tlp.ColorAlgorithm";
    tlpType = "Color";
  }
  else if (pluginType == "Selection") {
    pluginClass = "tlp.BooleanAlgorithm";
    tlpType = "Boolean";
  }
  else if (pluginType == "Import") {
    pluginClass = "tlp.ImportModule";
    tlpType = "Import";
  }
  else {
    pluginClass = "tlp.ExportModule";
    tlpType = "Export";
  }

  QString pluginSkeleton;
  QTextStream textStream(&pluginSkeleton);

  textStream << "from tulip import *" << endl;
  textStream << "import tulipplugins" << endl << endl;

  textStream << "class " << pluginClassName << "(" << pluginClass << "):" << endl;
  textStream << "\tdef __init__(self, context):" << endl;
  textStream << "\t\t" << pluginClass << ".__init__(self, context)"<< endl;
  textStream << "\t\t# you can add parameters to the plugin here through the following syntax"<< endl;
  textStream << "\t\t# self.add<Type>Parameter(\"<paramName>\", \"<paramDoc>\", \"<paramDefaultValue>\")" << endl;
  textStream << "\t\t# (see documentation of class tlp.WithParameter to see what types of parameters are supported)"<< endl << endl;

  if (pluginType != "Import" && pluginType != "Export") {

    textStream << "\tdef check(self):" << endl;
    textStream << "\t\t# This method is called before applying the algorithm on the input graph." << endl;
    textStream << "\t\t# You can perform some precondition checks here." << endl;
    textStream << "\t\t# See comments in the run method to know how to access to the input graph." << endl << endl;
    textStream << "\t\t# Must return a tuple (boolean, string). First member indicates if the algorithm can be applied" << endl;
    textStream << "\t\t# and the second one can be used to provide an error message" << endl;
    textStream << "\t\treturn (True, \"\")" << endl << endl;
    textStream << "\tdef run(self):" << endl;
    textStream << "\t\t# This method is the entry point of the algorithm when it is called" << endl;
    textStream << "\t\t# and must contain its implementation." << endl << endl;
    textStream << "\t\t# The graph on which the algorithm is applied can be accessed through" << endl;
    textStream << "\t\t# the \"graph\" class attribute (see documentation of class tlp.Graph)." << endl << endl;
    textStream << "\t\t# The parameters provided by the user are stored in a Tulip DataSet " << endl;
    textStream << "\t\t# and can be accessed through the \"dataSet\" class attribute" << endl ;
    textStream << "\t\t# (see documentation of class tlp.DataSet)." << endl << endl;

    if (pluginType == "Layout") {
      textStream << "\t\t# The result of this layout algorithm must be stored in the" << endl;
      textStream << "\t\t# layout property accessible through the \"layoutResult\" class attribute" << endl;
      textStream << "\t\t# (see documentation to know how to work with graph properties)." << endl << endl;
    }
    else if (pluginType == "Size") {
      textStream << "\t\t# The result of this size algorithm must be stored in the" << endl;
      textStream << "\t\t# size property accessible through the \"sizeResult\" class attribute" << endl;
      textStream << "\t\t# (see documentation to know how to work with graph properties)." << endl << endl;
    }
    else if (pluginType == "Measure") {
      textStream << "\t\t# The result of this measure algorithm must be stored in the" << endl;
      textStream << "\t\t# double property accessible through the \"doubleResult\" class attribute" << endl;
      textStream << "\t\t# (see documentation to know how to work with graph properties)." << endl << endl;
    }
    else if (pluginType == "Color") {
      textStream << "\t\t# The result of this color algorithm must be stored in the" << endl;
      textStream << "\t\t# color property accessible through the \"colorResult\" class attribute" << endl;
      textStream << "\t\t# (see documentation to know how to work with graph properties)." << endl << endl;
    }
    else if (pluginType == "Selection") {
      textStream << "\t\t# The result of this selection algorithm must be stored in the" << endl;
      textStream << "\t\t# boolean property accessible through the \"booleanResult\" class attribute" << endl;
      textStream << "\t\t# (see documentation to know how to work with graph properties)." << endl << endl;
    }

    textStream << "\t\t# The method must return a boolean indicating if the algorithm" << endl;
    textStream << "\t\t# has been successfully applied on the input graph." << endl;
    textStream << "\t\treturn True" << endl << endl;

  }
  else if (pluginType == "Import") {
    textStream << "\tdef importGraph(self):" << endl;
    textStream << "\t\t# This method is called to import a new graph." << endl;
    textStream << "\t\t# An empty graph to populate is accessible through the \"graph\" class attribute" << endl;
    textStream << "\t\t# (see documentation of class tlp.Graph)." << endl << endl;
    textStream << "\t\t# The parameters provided by the user are stored in a Tulip DataSet " << endl;
    textStream << "\t\t# and can be accessed through the \"dataSet\" class attribute" << endl ;
    textStream << "\t\t# (see documentation of class tlp.DataSet)." << endl << endl;
    textStream << "\t\t# The method must return a boolean indicating if the" << endl;
    textStream << "\t\t# graph has been successfully imported." << endl;
    textStream << "\t\treturn True" << endl << endl;
  }
  else if (pluginType == "Export") {
    textStream << "\tdef exportGraph(os, self):" << endl;
    textStream << "\t\t# This method is called to export a graph." << endl;
    textStream << "\t\t# The graph to export is accessible through the \"graph\" class attribute" << endl;
    textStream << "\t\t# (see documentation of class tlp.Graph)." << endl << endl;
    textStream << "\t\t# The parameters provided by the user are stored in a Tulip DataSet " << endl;
    textStream << "\t\t# and can be accessed through the \"dataSet\" class attribute" << endl ;
    textStream << "\t\t# (see documentation of class tlp.DataSet)." << endl << endl;
    textStream << "\t\t# The os parameter is an output file stream (initialized by the Tulip GUI" << endl;
    textStream << "\t\t# or by the tlp.exportGraph function.)." << endl;
    textStream << "\t\t# To write data to the file, you have to use the following syntax :" << endl << endl;
    textStream << "\t\t# write the number of nodes and edges to the file" << endl;
    textStream << "\t\t# os << self.graph.numberOfNodes() << \"\n\"" << endl;
    textStream << "\t\t# os << self.graph.numberOfEdges() << \"\n\"" << endl << endl;
    textStream << "\t\t# The method must return a boolean indicating if the" << endl;
    textStream << "\t\t# graph has been successfully exported." << endl;
    textStream << "\t\treturn True" << endl << endl;
  }

  textStream << "# The line below does the magic to register the plugin to the plugin database" << endl;
  textStream << "# and updates the GUI to make it accessible through the menus." << endl;

  if (pluginGroup == "") {
    textStream << "tulipplugins.register" << tlpType << "Plugin(\"" << pluginClassName << "\", \""
               << pluginName << "\", \"" << pluginAuthor << "\", \"" << pluginDate << "\", \""
               << pluginInfos << "\", \"" <<  pluginRelease <<"\")" << endl;
  }
  else {
    textStream << "tulipplugins.register" << tlpType << "PluginOfGroup(\"" << pluginClassName << "\", \""
               << pluginName << "\", \"" << pluginAuthor << "\", \"" << pluginDate << "\", \""
               << pluginInfos << "\", \"" <<  pluginRelease << "\", \"" << pluginGroup << "\")" << endl;
  }

  return pluginSkeleton;
}



VIEWPLUGIN(PythonScriptView, "Python Script view", "Antoine Lambert", "04/2010", "Python Script View", "0.7");

PythonScriptView::PythonScriptView() : pythonInterpreter(NULL),graph(NULL), scriptStopped(false), runningScript(false), dontTreatFocusIn(false) {}

PythonScriptView::~PythonScriptView() {}

QWidget *PythonScriptView::construct(QWidget *parent) {
  QWidget *widget = AbstractView::construct(parent);
  viewWidget = new PythonScriptViewWidget(this, widget);
  widget->resize(550,700);
  connect(viewWidget->runScriptButton, SIGNAL(clicked()), this, SLOT(executeCurrentScript()));
  connect(viewWidget->pauseScriptButton, SIGNAL(clicked()), this, SLOT(pauseCurrentScript()));
  connect(viewWidget->stopScriptButton, SIGNAL(clicked()), this, SLOT(stopCurrentScript()));
  connect(viewWidget->newMainScriptAction, SIGNAL(triggered()), this, SLOT(newScript()));
  connect(viewWidget->loadMainScriptAction, SIGNAL(triggered()), this, SLOT(loadScript()));
  connect(viewWidget->saveMainScriptAction, SIGNAL(triggered()), this, SLOT(saveScript()));
  connect(viewWidget->newStringModuleAction, SIGNAL(triggered()), this, SLOT(newStringModule()));
  connect(viewWidget->newFileModuleAction, SIGNAL(triggered()), this, SLOT(newFileModule()));
  connect(viewWidget->loadModuleAction, SIGNAL(triggered()), this, SLOT(loadModule()));
  connect(viewWidget->saveModuleAction, SIGNAL(triggered()), this, SLOT(saveModuleToFile()));
  connect(viewWidget->newPluginAction, SIGNAL(triggered()), this, SLOT(newPythonPlugin()));
  connect(viewWidget->loadPluginAction, SIGNAL(triggered()), this, SLOT(loadPythonPlugin()));
  connect(viewWidget->savePluginAction, SIGNAL(triggered()), this, SLOT(savePythonPlugin()));
  connect(viewWidget->registerPluginButton, SIGNAL(clicked()), this, SLOT(registerPythonPlugin()));
  connect(viewWidget->mainScriptsTabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeMainScriptTabRequested(int)));
  connect(viewWidget->modulesTabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeModuleTabRequested(int)));
  connect(viewWidget->pluginsTabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closePluginTabRequested(int)));

  setCentralWidget(viewWidget);
  pythonInterpreter = PythonInterpreter::getInstance();
  pythonInterpreter->runString(updateVisualizationFunc);
  pythonInterpreter->runString(pauseScriptFunc);
  pythonInterpreter->runString(runGraphScriptFunc);

  clusterTreeWidget = NULL;

  return widget;
}

string& replaceAll(string& context, const string& from, const string& to) {
  size_t lookHere = 0;
  size_t foundHere;

  while((foundHere = context.find(from, lookHere)) != string::npos) {
    context.replace(foundHere, from.size(), to);
    lookHere = foundHere + to.size();
  }

  return context;
}


void PythonScriptView::setData(Graph *graph,DataSet dataSet) {

  this->graph = graph;

  viewWidget->setGraph(graph);

  if (viewWidget->mainScriptsTabWidget->count() == 0) {

    viewWidget->tabWidget->setCurrentIndex(1);
    pythonInterpreter->setConsoleWidget(viewWidget->consoleOutputWidget);
    pythonInterpreter->clearOutputBuffers();
    DataSet modulesDataSet;

    if (dataSet.get("modules", modulesDataSet)) {
      int i = 0;
      string module;
      ostringstream oss;
      oss << "module" << i;

      while (modulesDataSet.get(oss.str(), module)) {
        bool moduleLoaded = loadModule(module.c_str(), false);

        if (!moduleLoaded) {
          string moduleSrc;
          oss.str("");
          oss << "module_src" << i;

          if (modulesDataSet.get(oss.str(), moduleSrc)) {
            replaceAll(moduleSrc, "    ", "\t");
            QFileInfo fileInfo(module.c_str());
            loadModuleFromSrcCode(fileInfo.fileName().toUtf8().data(),
                                  moduleSrc);
          }
        }

        oss.str("");
        oss << "module" << ++i;
      }
    }

    string scriptCode;
    string filename;
    bool mainScriptLoaded = false;
    DataSet mainScriptsDataSet;

    if (dataSet.get("main_scripts", mainScriptsDataSet)) {
      int i = 0;
      string mainScript;
      ostringstream oss;
      oss << "main_script" << i;

      while (mainScriptsDataSet.get(oss.str(), mainScript)) {
        mainScriptLoaded = loadScript(mainScript.c_str(), false);

        if (!mainScriptLoaded) {
          string mainScriptSrc;
          oss.str("");
          oss << "main_script_src" << i;

          if (mainScriptsDataSet.get(oss.str(), mainScriptSrc)) {
            int mainScriptId = viewWidget->addMainScriptEditor();
            PythonCodeEditor *codeEditor = viewWidget->getMainScriptEditor(mainScriptId);
            // TLPParser seems to replace the tab character with four white spaces when reading the content of the TLP file, don't know why
            // Anyway, replace the original tab character in order to have a correct indentation when setting the script text to the code editor
            replaceAll(mainScriptSrc, "    ", "\t");
            codeEditor->setPlainText(QString::fromUtf8(mainScriptSrc.c_str()));

            if (mainScript != "") {
              QFileInfo fileInfo(mainScript.c_str());
              viewWidget->mainScriptsTabWidget->setTabText(mainScriptId, fileInfo.fileName());
            }
            else
              viewWidget->mainScriptsTabWidget->setTabText(mainScriptId, "[no file]");

            viewWidget->mainScriptsTabWidget->setTabToolTip(mainScriptId, "string main script, don't forget to save the current graph or\n save the script to a file to not lose modifications to source code.");
          }
        }

        oss.str("");
        oss << "main_script" << ++i;
      }

      int mainScriptId = 0;
      mainScriptsDataSet.get("main_script_id", mainScriptId);

      viewWidget->mainScriptsTabWidget->setCurrentIndex(mainScriptId);

    }
    else {

      if (dataSet.get("main script file", filename)) {
        mainScriptLoaded = loadScript(filename.c_str(), false);
      }

      if (!mainScriptLoaded) {
        int editorId = viewWidget->addMainScriptEditor();
        PythonCodeEditor *codeEditor = viewWidget->getMainScriptEditor(editorId);
        codeEditor->setFileName("");
        QFileInfo fileInfo(QString::fromUtf8(filename.c_str()));

        if (dataSet.get("script code", scriptCode)) {
          replaceAll(scriptCode, "    ", "\t");
          codeEditor->setPlainText(QString::fromUtf8(scriptCode.c_str()));

          if (filename != "")
            viewWidget->mainScriptsTabWidget->setTabText(0, fileInfo.fileName());
          else
            viewWidget->mainScriptsTabWidget->setTabText(0, "[no file]");

          viewWidget->mainScriptsTabWidget->setTabToolTip(0, "string main script, don't forget to save the current graph or\n save the script to a file to not lose modifications to source code.");
        }
        else {
          codeEditor->setPlainText(getDefaultScriptCode(pythonInterpreter->getPythonVersion(), graph).c_str());
          viewWidget->mainScriptsTabWidget->setTabText(0, "[no file]");
          viewWidget->mainScriptsTabWidget->setTabToolTip(0, "string main script, don't forget to save the current graph or\n save the script to a file to not lose modifications to source code.");
        }
      }

    }

    viewWidget->tabWidget->setCurrentIndex(2);

    DataSet pluginsDataSet;

    if (dataSet.get("plugins", pluginsDataSet)) {
      int i = 0;
      string plugin;
      ostringstream oss;
      oss << "plugin" << i;

      while (pluginsDataSet.get(oss.str(), plugin)) {
        bool pluginLoaded = loadPythonPlugin(plugin.c_str(), false);

        if (!pluginLoaded) {
          string pluginSrc;
          oss.str("");
          oss << "plugin_src" << i;

          if (pluginsDataSet.get(oss.str(), pluginSrc)) {
            replaceAll(pluginSrc, "    ", "\t");
            QFileInfo fileInfo(QString::fromUtf8(plugin.c_str()));
            loadPythonPluginFromSrcCode(fileInfo.fileName().toUtf8().data(),
                                        pluginSrc, false);
          }
        }

        oss.str("");
        oss << "plugin" << ++i;
      }
    }

    indicateErrors();
    pythonInterpreter->setDefaultConsoleWidget();
    viewWidget->tabWidget->setCurrentIndex(0);
  }

  // hack to get a pointer on the cluster hierarchy widget
  // This way, we can update it after executing a script (there is some refresh issue otherwise)
  MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());

  if (mainController) {
    QWidget *mainWindow = mainController->getMainWindowFacade()->getParentWidget();
    QObjectList childWidgets = mainWindow->children();

    while (!childWidgets.empty()) {
      QObject *obj = childWidgets.front();
      clusterTreeWidget = dynamic_cast<SGHierarchyWidget *>(obj);

      if (clusterTreeWidget && graph->getRoot() == clusterTreeWidget->getGraph()->getRoot()) {
        break;
      }
      else {
        childWidgets.pop_front();
        childWidgets += obj->children();
      }
    }
  }
}

void PythonScriptView::getData(Graph **graph,DataSet *dataSet) {
  *graph = this->graph;

  PythonCodeEditor *codeEditor = viewWidget->getCurrentMainScriptEditor();

  if (codeEditor) {
    dataSet->set("main script file", codeEditor->getFileName());
    string scriptCode = viewWidget->getCurrentMainScriptCode();
    dataSet->set("script code", scriptCode);
  }

  DataSet mainScriptsDataSet;

  for (int i = 0 ; i < viewWidget->mainScriptsTabWidget->count() ; ++i) {
    string scriptFile = viewWidget->getMainScriptEditor(i)->getFileName().toUtf8().data();

    if (scriptFile != "")
      saveScript(i);

    ostringstream oss;
    oss << "main_script" << i;
    mainScriptsDataSet.set(oss.str(), scriptFile);
    oss.str("");
    oss << "main_script_src" << i;
    mainScriptsDataSet.set(oss.str(), viewWidget->getMainScriptCode(i));
  }

  mainScriptsDataSet.set("main_script_id", viewWidget->mainScriptsTabWidget->currentIndex());
  dataSet->set("main_scripts", mainScriptsDataSet);


  DataSet modulesDataSet;

  for (int i = 0 ; i < viewWidget->modulesTabWidget->count() ; ++i) {
    string moduleFile = viewWidget->getModuleEditor(i)->getFileName().toUtf8().data();

    if (moduleFile != "")
      saveModule(i);

    ostringstream oss;
    oss << "module" << i;
    modulesDataSet.set(oss.str(), moduleFile);
    oss.str("");
    oss << "module_src" << i;
    modulesDataSet.set(oss.str(), viewWidget->getModuleCode(i));
  }

  dataSet->set("modules", modulesDataSet);

  DataSet pluginsDataSet;

  for (int i = 0 ; i < viewWidget->pluginsTabWidget->count() ; ++i) {
    string pluginFile = viewWidget->getPluginEditor(i)->getFileName().toUtf8().data();

    if (pluginFile != "")
      savePythonPlugin(i);

    ostringstream oss;
    oss << "plugin" << i;
    pluginsDataSet.set(oss.str(), pluginFile);
    oss.str("");
    oss << "plugin_src" << i;
    pluginsDataSet.set(oss.str(), viewWidget->getPluginCode(i));
  }

  dataSet->set("plugins", pluginsDataSet);
}

void PythonScriptView::executeCurrentScript() {

  if (pythonInterpreter->isScriptPaused()) {
    Observable::holdObservers();
    pythonInterpreter->pauseCurrentScript(false);
    viewWidget->runScriptButton->setEnabled(false);
    viewWidget->pauseScriptButton->setEnabled(true);
    viewWidget->scriptStatusLabel->setText("Executing script ...");
    viewWidget->progressBar->setRange(0,0);
    return;
  }

  if (!pythonInterpreter->isRunningScript() && viewWidget->mainScriptsTabWidget->count() > 0) {

    runningScript = true;

    pythonInterpreter->setConsoleWidget(viewWidget->consoleOutputWidget);
    viewWidget->consoleOutputWidget->clear();
    pythonInterpreter->clearOutputBuffers();
    clearErrorIndicators();

    string scriptFileName = viewWidget->getCurrentMainScriptEditor()->getFileName().toUtf8().data();

    if (scriptFileName == "") {
      scriptFileName = "<unnamed script>";
    }

    saveImportAllScripts();

    saveAllModules();

    pythonInterpreter->setConsoleWidget(viewWidget->consoleOutputWidget);

    if (!reloadAllModules() || !pythonInterpreter->runString(viewWidget->getCurrentMainScriptCode().c_str(), scriptFileName)) {
      indicateErrors();
      return;
    }

    graph->push();
    Observable::holdObservers();

    pythonInterpreter->setProcessQtEventsDuringScriptExecution(true);

    viewWidget->scriptStatusLabel->setText("Executing script ...");
    viewWidget->progressBar->setRange(0,0);

    viewWidget->runScriptButton->setEnabled(false);
    viewWidget->stopScriptButton->setEnabled(true);
    viewWidget->pauseScriptButton->setEnabled(true);

    QApplication::processEvents();

    bool scriptExecOk = pythonInterpreter->runGraphScript("__main__", "main", graph, scriptFileName);

    pythonInterpreter->setProcessQtEventsDuringScriptExecution(false);
    viewWidget->stopScriptButton->setEnabled(false);
    viewWidget->runScriptButton->setEnabled(true);
    viewWidget->pauseScriptButton->setEnabled(false);

    if (scriptExecOk) {
      viewWidget->scriptStatusLabel->setText("Script execution has succeed");
      pythonInterpreter->runString("del main");
    }
    else {

      viewWidget->scriptStatusLabel->setText("Script execution has failed");

      if (!scriptStopped) {
        indicateErrors();
      }

      graph->pop();
    }

    if (clusterTreeWidget)
      clusterTreeWidget->update();

    viewWidget->progressBar->setRange(0,100);
    viewWidget->progressBar->reset();

    pythonInterpreter->setDefaultConsoleWidget();

    if (Observable::observersHoldCounter() > 0)
      Observable::unholdObservers();

    // some external modules (like numpy) overrides the SIGINT handler at import
    // reinstall the default one, otherwise Tulip can not be interrupted by hitting Ctrl-C in a console
    pythonInterpreter->setDefaultSIGINTHandler();

    scriptStopped = false;
    runningScript = false;

  }
  else {
    QMessageBox::information(viewWidget->getCurrentMainScriptEditor(), "Script execution not allowed", "The Python interpreter already execute a script. You must wait for its termination or stop its execution before running a new script.");
  }
}

void PythonScriptView::indicateErrors() {
  QRegExp rx("^.*File.*\"(.*)\".*line.*(\\d+).*$");
  QRegExp rx2("^.*File.*\"(.*)\".*line.*(\\d+).*in (.*)$");

  map<string, vector<int> > errorLines;
  QString consoleOutput = pythonInterpreter->getStandardErrorOutput().c_str();
  QStringList outputLines = consoleOutput.split("\n");

  for (int i = 0 ; i < outputLines.count() - 1 ; ++i) {
    int pos = 0;

    while ((pos = rx.indexIn(outputLines[i], pos)) != -1) {
      rx2.indexIn(outputLines[i], pos);

      if (rx2.cap(3) != "tlpimporthook") {
        string file = rx.cap(1).toStdString();
        int line = rx.cap(2).toInt();
        errorLines[file].push_back(line);
      }

      pos += rx.matchedLength();
    }

  }

  if (errorLines.find("<unnamed script>") != errorLines.end()) {
    for (size_t i = 0 ; i < errorLines["<unnamed script>"].size() ; ++i) {
      if (errorLines["<unnamed script>"][i] > 1) {
        viewWidget->getCurrentMainScriptEditor()->indicateScriptCurrentError(errorLines["<unnamed script>"][i]-1);
      }
    }
  }

  map<int, string>::const_iterator it;

  for (int i = 0 ; i < viewWidget->mainScriptsTabWidget->count() ; ++i) {
    std::string moduleFile = viewWidget->getMainScriptEditor(i)->getFileName().toUtf8().data();

    if (errorLines.find(moduleFile) != errorLines.end()) {
      const vector<int> &linesErrorNumbers = errorLines[moduleFile];
      PythonCodeEditor *codeEditor = viewWidget->getMainScriptEditor(i);

      for (size_t i = 0 ; i < linesErrorNumbers.size() ; ++i) {
        codeEditor->indicateScriptCurrentError(linesErrorNumbers[i]-1);
      }
    }
  }

  for (int i = 0 ; i < viewWidget->modulesTabWidget->count() ; ++i) {
    std::string moduleFile = viewWidget->getModuleEditor(i)->getFileName().toUtf8().data();

    if (errorLines.find(moduleFile) != errorLines.end()) {
      const vector<int> &linesErrorNumbers = errorLines[moduleFile];
      PythonCodeEditor *codeEditor = viewWidget->getModuleEditor(i);

      for (size_t i = 0 ; i < linesErrorNumbers.size() ; ++i) {
        codeEditor->indicateScriptCurrentError(linesErrorNumbers[i]-1);
      }
    }
  }

  for (int i = 0 ; i < viewWidget->pluginsTabWidget->count() ; ++i) {
    std::string moduleFile = viewWidget->getPluginEditor(i)->getFileName().toUtf8().data();

    if (errorLines.find(moduleFile) != errorLines.end()) {
      const vector<int> &linesErrorNumbers = errorLines[moduleFile];
      PythonCodeEditor *codeEditor = viewWidget->getPluginEditor(i);

      for (size_t i = 0 ; i < linesErrorNumbers.size() ; ++i) {
        codeEditor->indicateScriptCurrentError(linesErrorNumbers[i]-1);
      }
    }
  }
}

void PythonScriptView::clearErrorIndicators() {
  map<int, string>::const_iterator it;

  for (int i = 0 ; i < viewWidget->mainScriptsTabWidget->count() ; ++i) {
    PythonCodeEditor *codeEditor = viewWidget->getMainScriptEditor(i);
    codeEditor->clearErrorIndicator();
  }

  for (int i = 0 ; i < viewWidget->modulesTabWidget->count() ; ++i) {
    PythonCodeEditor *codeEditor = viewWidget->getModuleEditor(i);
    codeEditor->clearErrorIndicator();
  }

  for (int i = 0 ; i < viewWidget->pluginsTabWidget->count() ; ++i) {
    PythonCodeEditor *codeEditor = viewWidget->getPluginEditor(i);
    codeEditor->clearErrorIndicator();
  }
}

void PythonScriptView::stopCurrentScript() {
  scriptStopped = true;
  pythonInterpreter->stopCurrentScript();
}

void PythonScriptView::pauseCurrentScript() {
  Observable::unholdObservers();
  pythonInterpreter->pauseCurrentScript();
  viewWidget->pauseScriptButton->setEnabled(false);
  viewWidget->runScriptButton->setEnabled(true);
  viewWidget->progressBar->setRange(0,100);
  viewWidget->progressBar->reset();
  viewWidget->scriptStatusLabel->setText("Script execution is paused.");
}

void PythonScriptView::newScript() {
  int editorId = viewWidget->addMainScriptEditor();
  viewWidget->getMainScriptEditor(editorId)->setPlainText(getDefaultScriptCode(pythonInterpreter->getPythonVersion(), graph).c_str());
  viewWidget->mainScriptsTabWidget->setTabText(editorId, "[no file]");
  viewWidget->mainScriptsTabWidget->setTabToolTip(editorId, "string main script, don't forget to save the current graph or\n save the script to a file to not lose modifications to source code.");
}

void PythonScriptView::loadScript() {
  QString fileName = QFileDialog::getOpenFileName(viewWidget, "Open Main Script","", "Python script (*.py)");
  loadScript(fileName);
}

bool PythonScriptView::loadScript(const QString &fileName, bool clear) {
  QFile file(findFile(fileName));
  QFileInfo fileInfo(file);

  if (!file.exists())
    return false;

  file.open(QIODevice::ReadOnly | QIODevice::Text);
  QString scriptCode;

  while (!file.atEnd()) {
    scriptCode += file.readLine();
  }

  lastModifiedFile[fileInfo.absoluteFilePath()] = fileInfo.lastModified();

  int editorId = viewWidget->addMainScriptEditor(fileInfo.absoluteFilePath());
  viewWidget->getMainScriptEditor(editorId)->setPlainText(scriptCode);
  viewWidget->mainScriptsTabWidget->setTabText(editorId, fileInfo.fileName());
  viewWidget->mainScriptsTabWidget->setTabToolTip(editorId, fileInfo.absoluteFilePath());

  file.close();

  QString modulePath = fileInfo.absolutePath();
  pythonInterpreter->addModuleSearchPath(modulePath.toUtf8().data());
  pythonInterpreter->setConsoleWidget(viewWidget->consoleOutputWidget);

  if (clear) {
    viewWidget->consoleOutputWidget->clear();
    pythonInterpreter->clearOutputBuffers();
  }

  clearErrorIndicators();
  pythonInterpreter->reloadModule(fileInfo.fileName().replace(".py", "").toUtf8().data());
  indicateErrors();
  pythonInterpreter->setDefaultConsoleWidget();

  return true;
}

void PythonScriptView::saveScript() {
  saveScript(viewWidget->mainScriptsTabWidget->currentIndex());
}

void PythonScriptView::saveScript(int tabIdx, bool clear) {
  if (tabIdx >=0 && tabIdx < viewWidget->mainScriptsTabWidget->count()) {
    QString fileName;
    QString mainScriptFileName = viewWidget->getMainScriptEditor(tabIdx)->getFileName();


    if (mainScriptFileName == "") {
      QString dir = "";
      QString tabText = viewWidget->mainScriptsTabWidget->tabText(tabIdx);

      if (!tabText.startsWith("[")) {
        dir = tabText;
      }

      fileName = QFileDialog::getSaveFileName(viewWidget, tr("Save Main Script"),dir,"Python script (*.py)");
    }
    else
      fileName = mainScriptFileName;

    if (fileName != "") {
      if (!fileName.endsWith(".py"))
        fileName += ".py";

      QFile file(fileName);
      QFileInfo fileInfo(file);

      file.open(QIODevice::WriteOnly | QIODevice::Text);
      viewWidget->getMainScriptEditor(tabIdx)->setFileName(fileInfo.absoluteFilePath());
      QTextStream out(&file);
      out << viewWidget->getMainScriptCode(tabIdx).c_str();
      viewWidget->mainScriptsTabWidget->setTabText(tabIdx, fileInfo.fileName());
      viewWidget->mainScriptsTabWidget->setTabToolTip(tabIdx, fileInfo.absoluteFilePath());
      file.close();

      QString modulePath = fileInfo.absolutePath();
      pythonInterpreter->addModuleSearchPath(modulePath.toUtf8().data());

      pythonInterpreter->setConsoleWidget(viewWidget->consoleOutputWidget);

      if (clear) {
        viewWidget->consoleOutputWidget->clear();
        pythonInterpreter->clearOutputBuffers();
      }

      clearErrorIndicators();
      pythonInterpreter->reloadModule(fileInfo.fileName().replace(".py", "").toUtf8().data());
      indicateErrors();
      pythonInterpreter->setDefaultConsoleWidget();

      lastModifiedFile[fileName] = fileInfo.lastModified();
    }
  }
}

void PythonScriptView::saveImportAllScripts() {
  for (int i = 0 ; i < viewWidget->mainScriptsTabWidget->count() ; ++i) {
    PythonCodeEditor *codeEditor = viewWidget->getMainScriptEditor(i);

    if (codeEditor->getFileName() != "") {
      saveScript(i, false);
    }
    else {
      QString tabText = viewWidget->mainScriptsTabWidget->tabText(i);

      if (tabText.contains(".py")) {
        tabText.replace(".py", "");
        pythonInterpreter->registerNewModuleFromString(tabText.toUtf8().data(), viewWidget->getMainScriptCode(i));
        pythonInterpreter->runString(string("import ") + tabText.toUtf8().data());
      }
    }
  }
}

void PythonScriptView::loadModule() {
  QString fileName = QFileDialog::getOpenFileName(viewWidget, "Open Module","", "Python script (*.py)");
  loadModule(fileName);
}

QString PythonScriptView::findFile(const QString &filePath) {
  if (filePath.isEmpty()) {
    return "";
  }

  QFileInfo fileInfo(filePath);
  QString filepath = fileInfo.absolutePath();
  QString filename = fileInfo.fileName();

  if (fileInfo.exists()) {
    return filePath;
  }
  else if (filename != "" && graph) {
    std::string tlpFile;

    if (graph->getRoot()->getAttribute("file", tlpFile)) {
      QFileInfo fileInfoTlp(QString::fromUtf8(tlpFile.c_str()));
      QString newfilepath = fileInfoTlp.absolutePath() + "/" + filename;
      fileInfo = QFileInfo(newfilepath);

      if (fileInfo.exists()) {
        return newfilepath;
      }
      else {
        QStringList pathSaved = filepath.split("/");
        QStringList pathTlp = fileInfoTlp.absolutePath().split("/");
        QString basePath = "";

        for (int i = 0 ; i < pathTlp.size() ; ++i) {
          basePath += (pathTlp[i] + "/");

          for (int j = 0 ; j < pathSaved.size()-1 ; ++j) {
            QString testPath = basePath;

            for (int k = j ; k < pathSaved.size() ; ++k) {
              testPath += (pathSaved[k] + "/");
            }

            testPath += filename;
            fileInfo = QFileInfo(testPath);

            if (fileInfo.exists()) {
              return testPath;
            }
          }
        }
      }
    }
  }

  return "";
}

bool PythonScriptView::loadModule(const QString &fileName, bool clear) {
  QFile file(findFile(fileName));

  if (!file.exists())
    return false;

  file.open(QIODevice::ReadOnly | QIODevice::Text);
  QFileInfo fileInfo(file);
  QString moduleName(fileInfo.fileName());
  QString modulePath(fileInfo.absolutePath());

  QString scriptCode;

  while (!file.atEnd()) {
    scriptCode += file.readLine();
  }

  file.close();

  lastModifiedFile[fileInfo.absoluteFilePath()] = fileInfo.lastModified();

  int editorId = viewWidget->addModuleEditor(fileInfo.absoluteFilePath());
  PythonCodeEditor *codeEditor = viewWidget->getModuleEditor(editorId);

  pythonInterpreter->addModuleSearchPath(modulePath.toUtf8().data());

  codeEditor->setPlainText(scriptCode);

  viewWidget->modulesTabWidget->setTabText(editorId, fileInfo.fileName());
  viewWidget->modulesTabWidget->setTabToolTip(editorId, fileInfo.absoluteFilePath());

  codeEditor->analyseScriptCode(true);

  pythonInterpreter->setConsoleWidget(viewWidget->consoleOutputWidget);

  if (clear) {
    viewWidget->consoleOutputWidget->clear();
    pythonInterpreter->clearOutputBuffers();
  }

  clearErrorIndicators();
  reloadAllModules();
  saveImportAllScripts();
  indicateErrors();
  pythonInterpreter->setDefaultConsoleWidget();

  return true;
}

bool PythonScriptView::loadModuleFromSrcCode(const std::string &moduleName, const std::string &moduleSrcCode) {
  int editorId = viewWidget->addModuleEditor();
  PythonCodeEditor *codeEditor = viewWidget->getModuleEditor(editorId);
  codeEditor->setFileName(moduleName.c_str());
  codeEditor->setPlainText(moduleSrcCode.c_str());

  viewWidget->modulesTabWidget->setTabText(editorId, moduleName.c_str());
  viewWidget->modulesTabWidget->setTabToolTip(editorId, "string module, don't forget to save the current graph or\n save the module to a file to not lose modifications to source code.");

  bool ret = pythonInterpreter->registerNewModuleFromString(QString(moduleName.c_str()).replace(".py", "").toUtf8().data(),  moduleSrcCode);

  if (ret) {
    codeEditor->analyseScriptCode(true);
  }

  return ret;
}

void PythonScriptView::newFileModule() {
  QString fileName = QFileDialog::getSaveFileName(viewWidget, tr("Set Module filename"),"","Python script (*.py)");

  if (fileName.isEmpty())
    return;

  if (!fileName.endsWith(".py"))
    fileName += ".py";

  QFile file(fileName);

  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  QFileInfo fileInfo(file);
  QString moduleName(fileInfo.fileName());
  QString modulePath(fileInfo.absolutePath());

  lastModifiedFile[fileName] = fileInfo.lastModified();

  int editorId = viewWidget->addModuleEditor(fileInfo.absoluteFilePath());
  viewWidget->modulesTabWidget->setTabToolTip(editorId, fileInfo.absoluteFilePath());
  pythonInterpreter->addModuleSearchPath(modulePath.toUtf8().data());
  viewWidget->modulesTabWidget->setTabText(editorId, fileInfo.fileName());

  file.close();
}

void PythonScriptView::newStringModule() {
  bool ok;
  QString moduleName = QInputDialog::getText(this->viewWidget, "New string module  ", "module name :", QLineEdit::Normal,"", &ok);

  if (ok && !moduleName.isEmpty()) {

    if (!moduleName.endsWith(".py"))
      moduleName += ".py";

    int editorId = viewWidget->addModuleEditor(moduleName);
    viewWidget->modulesTabWidget->setTabText(editorId, moduleName);

    viewWidget->modulesTabWidget->setTabToolTip(editorId, "string module, don't forget to save the current graph or\n save module to file to not lose modifications to source code.");
  }
}

void PythonScriptView::saveModuleToFile() {
  QString fileName = QFileDialog::getSaveFileName(viewWidget, tr("Set Module filename"),"","Python script (*.py)");

  if (!fileName.isEmpty()) {
    if (!fileName.endsWith(".py"))
      fileName += ".py";

    int tabIdx = viewWidget->modulesTabWidget->currentIndex();
    viewWidget->getModuleEditor(tabIdx)->setFileName(fileName);
    saveModule(tabIdx);
  }
}

void PythonScriptView::saveModule() {
  int curModule = viewWidget->modulesTabWidget->currentIndex();

  if (curModule == -1)
    return;

  saveModule(curModule, true);
}

bool PythonScriptView::checkAndGetPluginInfosFromSrcCode(const QString &pluginCode, QString &pluginName, QString &pluginClassName, QString &pluginType, QString &pluginClass) {
  QString s = "tulipplugins.register";
  int idx1 = pluginCode.indexOf(s);

  if (idx1 == -1) {
    return false;
  }
  else {
    idx1 += s.length();
    int idx2 = pluginCode.indexOf("Plugin", idx1);
    QString tlpType = pluginCode.mid(idx1, idx2 - idx1);
    pluginType = pluginCode.mid(idx1, idx2 - idx1);
    pluginClass = "";
    pluginClassName = "";
    pluginName = "";

    if (tlpType == "Algorithm") {
      pluginClass = "tlp.Algorithm";
      pluginType = "General";
    }
    else if (tlpType == "Layout") {
      pluginClass = "tlp.LayoutAlgorithm";
      pluginType = tlpType;
    }
    else if (tlpType == "Size") {
      pluginType = tlpType;
      pluginClass = "tlp.SizeAlgorithm";
    }
    else if (tlpType == "Color") {
      pluginType = tlpType;
      pluginClass = "tlp.ColorAlgorithm";
    }
    else if (tlpType == "Double") {
      pluginType = "Measure";
      pluginClass = "tlp.DoubleAlgorithm";
    }
    else if (tlpType == "Boolean") {
      pluginType = "Selection";
      pluginClass = "tlp.BooleanAlgorithm";
    }
    else if (tlpType == "Import") {
      pluginType = tlpType;
      pluginClass = "tlp.ImportModule";
    }
    else {
      pluginType = "Export";
      pluginClass = "tlp.ExportModule";
    }

    ostringstream oss;
    oss << "^.*class ([a-zA-Z_][a-zA-Z0-9_]*)\\(" << pluginClass.toStdString() << "\\).*$";
    QRegExp rx(oss.str().c_str());

    if (rx.indexIn(pluginCode) != -1) {
      pluginClassName = rx.cap(1);
    }

    oss.str("");
    oss << "^.*register" << tlpType.toStdString() << "Plugin.*\\(.*,.*\"([^,]+)\",.*$";
    rx.setPattern(oss.str().c_str());

    if (rx.indexIn(pluginCode) != -1) {
      pluginName = rx.cap(1);
    }
  }

  return true;
}

void PythonScriptView::loadPythonPlugin() {
  QString fileName = QFileDialog::getOpenFileName(viewWidget, "Open Tulip Python plugin","", "Python script (*.py)");
  loadPythonPlugin(fileName);
}

bool PythonScriptView::loadPythonPlugin(const QString &fileName, bool clear) {

  QFile file(findFile(fileName));

  if (!file.exists())
    return false;

  file.open(QIODevice::ReadOnly | QIODevice::Text);
  QFileInfo fileInfo(file);
  QString moduleName(fileInfo.fileName());
  QString modulePath(fileInfo.absolutePath());

  QString pluginCode;

  while (!file.atEnd()) {
    pluginCode += file.readLine();
  }

  file.close();

  QString pluginType = "";
  QString pluginClass = "";
  QString pluginClassName = "";
  QString pluginName = "";

  if (checkAndGetPluginInfosFromSrcCode(pluginCode, pluginName, pluginClassName, pluginType, pluginClass)) {
    if (pluginClassName == "" || pluginName == "") {
      QMessageBox::critical(viewWidget, "Error", "Unable to retrieve the plugin class name and the plugin name from the source code\n.");
      return false;
    }
    else {
      lastModifiedFile[fileInfo.absolutePath()] = fileInfo.lastModified();
      int editorId = viewWidget->addPluginEditor(fileInfo.absoluteFilePath());
      PythonCodeEditor *codeEditor = viewWidget->getPluginEditor(editorId);
      codeEditor->setPlainText(pluginCode);
      pythonInterpreter->addModuleSearchPath(modulePath.toUtf8().data());
      viewWidget->pluginsTabWidget->setTabToolTip(editorId, fileInfo.absoluteFilePath());
      viewWidget->pluginsTabWidget->setTabText(editorId, QString("[") + pluginType + QString("] ") + fileInfo.fileName());
      string pluginFile = fileInfo.absoluteFilePath().toUtf8().data();
      editedPluginsClassName[pluginFile] = pluginClassName.toStdString();
      editedPluginsType[pluginFile] = pluginType.toStdString();
      editedPluginsName[pluginFile] = pluginName.toStdString();
      registerPythonPlugin(clear);
    }
  }
  else {
    QMessageBox::critical(viewWidget, "Error", "The file " + fileName + " does not seem to contain the source code of a Tulip Python plugin.");
    return false;
  }

  return true;
}

bool PythonScriptView::loadPythonPluginFromSrcCode(const std::string &moduleName, const std::string &pluginSrcCode, bool clear) {
  QString pluginType = "";
  QString pluginClass = "";
  QString pluginClassName = "";
  QString pluginName = "";

  if (checkAndGetPluginInfosFromSrcCode(pluginSrcCode.c_str(), pluginName, pluginClassName, pluginType, pluginClass)) {
    if (pluginName != "" && pluginClassName != "") {
      int editorId = viewWidget->addPluginEditor(moduleName.c_str());
      PythonCodeEditor *codeEditor = viewWidget->getPluginEditor(editorId);
      codeEditor->setPlainText(pluginSrcCode.c_str());
      viewWidget->pluginsTabWidget->setTabToolTip(editorId, "string plugin, don't forget to save the current graph or\n save the plugin to a file to not lose modifications to source code.");
      viewWidget->pluginsTabWidget->setTabText(editorId, QString("[") + pluginType + QString("] ") + moduleName.c_str());
      string pluginFile = moduleName;
      editedPluginsClassName[pluginFile] = pluginClassName.toStdString();
      editedPluginsType[pluginFile] = pluginType.toStdString();
      editedPluginsName[pluginFile] = pluginName.toStdString();
      registerPythonPlugin(clear);
      return true;
    }
  }

  return false;
}

void PythonScriptView::savePythonPlugin() {
  savePythonPlugin(viewWidget->pluginsTabWidget->currentIndex());
}

void PythonScriptView::savePythonPlugin(int tabIdx) {

  if (tabIdx >= 0 && tabIdx < viewWidget->pluginsTabWidget->count()) {

    QString moduleNameExt = viewWidget->pluginsTabWidget->tabText(tabIdx);
    QString moduleName;

    if (moduleNameExt[moduleNameExt.size() - 1] == '*')
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 4);
    else
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 3);

    viewWidget->pluginsTabWidget->setTabText(tabIdx, moduleName+".py");
    QFile file(viewWidget->getPluginEditor(tabIdx)->getFileName());
    QFileInfo fileInfo(file);

    if (file.exists() && file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      QTextStream out(&file);
      out << viewWidget->getPluginCode(tabIdx).c_str();
      file.close();
      viewWidget->pluginsTabWidget->setTabToolTip(tabIdx, fileInfo.absoluteFilePath());
      lastModifiedFile[viewWidget->getPluginEditor(tabIdx)->getFileName()] = fileInfo.lastModified();
    }
  }
}

void PythonScriptView::saveModule(int tabIdx, const bool reload) {
  if (tabIdx >= 0 && tabIdx < viewWidget->modulesTabWidget->count()) {
    QString moduleNameExt = viewWidget->modulesTabWidget->tabText(tabIdx);
    QString moduleName;

    if (moduleNameExt[moduleNameExt.size() - 1] == '*')
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 4);
    else
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 3);

    pythonInterpreter->deleteModule(moduleName.toUtf8().data());
    viewWidget->modulesTabWidget->setTabText(tabIdx, moduleName+".py");
    QFile file(viewWidget->getModuleEditor(tabIdx)->getFileName());
    QFileInfo fileInfo(file);

    if (fileInfo.fileName() != viewWidget->getModuleEditor(tabIdx)->getFileName() && file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      QTextStream out(&file);
      out << viewWidget->getModuleCode(tabIdx).c_str();
      file.close();
      viewWidget->modulesTabWidget->setTabToolTip(tabIdx, fileInfo.absoluteFilePath());
      lastModifiedFile[viewWidget->getModuleEditor(tabIdx)->getFileName()] = fileInfo.lastModified();
    }
  }

  if (reload && !pythonInterpreter->isRunningScript()) {
    pythonInterpreter->setConsoleWidget(viewWidget->consoleOutputWidget);
    viewWidget->consoleOutputWidget->clear();
    pythonInterpreter->clearOutputBuffers();
    clearErrorIndicators();
    reloadAllModules();
    saveImportAllScripts();
    indicateErrors();
    pythonInterpreter->setDefaultConsoleWidget();
  }
}

void PythonScriptView::saveAllModules() {
  map<int, string>::const_iterator it;

  for (int i = 0 ; i < viewWidget->modulesTabWidget->count() ; ++i) {
    saveModule(i);

  }
}

bool PythonScriptView::reloadAllModules() {

  bool ret = true;

  for (int i = 0 ; i < viewWidget->modulesTabWidget->count() ; ++i) {
    QString moduleNameExt = viewWidget->modulesTabWidget->tabText(i);
    QString moduleName;

    if (moduleNameExt[moduleNameExt.size() - 1] == '*')
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 4);
    else
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 3);

    pythonInterpreter->deleteModule(moduleName.toUtf8().data());
    QFileInfo fileInfo(viewWidget->getModuleEditor(i)->getFileName());

    if (fileInfo.fileName() == viewWidget->getModuleEditor(i)->getFileName()) {
      ret = ret && pythonInterpreter->registerNewModuleFromString(moduleName.toUtf8().data(),  viewWidget->getModuleCode(i));
    }
    else {
      pythonInterpreter->addModuleSearchPath(fileInfo.absolutePath().toUtf8().data());
      ret = ret && pythonInterpreter->reloadModule(moduleName.toUtf8().data());
    }
  }

  return ret;
}

void PythonScriptView::reloadCodeInEditorIfNeeded(PythonCodeEditor *codeEditor, QTabWidget *tabWidget, int index) {
  QString fileName = codeEditor->getFileName();

  if (fileName != "") {
    QFileInfo fileInfo(fileName);

    if (fileInfo.exists() && fileInfo.lastModified() != lastModifiedFile[fileName]) {
      QFile file(fileName);
      file.open(QIODevice::ReadOnly | QIODevice::Text);
      QString scriptCode;

      while (!file.atEnd()) {
        scriptCode += file.readLine();
      }

      lastModifiedFile[fileName] = fileInfo.lastModified();

      if (scriptCode != codeEditor->getCleanCode() && QMessageBox::question(codeEditor, "File changed on disk", QString("The file ") + fileName + " has been modified by another editor. Do you want to reload it ?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes) {
        codeEditor->setPlainText(scriptCode);
        tabWidget->setTabText(index, fileInfo.fileName());
      }
    }
  }
}

bool PythonScriptView::eventFilter(QObject *obj, QEvent *event) {
  Qt::KeyboardModifier modifier = Qt::ControlModifier;
#ifdef __APPLE__
  modifier = Qt::MetaModifier;
#endif

  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvt = static_cast<QKeyEvent *>(event);

    if (keyEvt->modifiers() == modifier && keyEvt->key() == Qt::Key_S) {
      if (obj == viewWidget->getCurrentMainScriptEditor()) {
        saveScript();
        return true;
      }
      else if (obj == viewWidget->getCurrentModuleEditor()) {
        saveModule();
        return true;
      }
      else if (obj == viewWidget->getCurrentPluginEditor()) {
        savePythonPlugin();
        return true;
      }

    }
    else if (obj == viewWidget->getCurrentMainScriptEditor() && keyEvt->modifiers() == modifier && keyEvt->key() == Qt::Key_Return) {
      executeCurrentScript();
      return true;
    }
  }
  else if (event->type() == QEvent::FocusIn && !dontTreatFocusIn) {

    dontTreatFocusIn = true;

    for (int i = 0 ; i < viewWidget->mainScriptsTabWidget->count() ; ++i) {
      reloadCodeInEditorIfNeeded(viewWidget->getMainScriptEditor(i), viewWidget->mainScriptsTabWidget, i);
    }

    for (int i = 0 ; i < viewWidget->modulesTabWidget->count() ; ++i) {
      reloadCodeInEditorIfNeeded(viewWidget->getModuleEditor(i), viewWidget->modulesTabWidget, i);
    }

    for (int i = 0 ; i < viewWidget->pluginsTabWidget->count() ; ++i) {
      reloadCodeInEditorIfNeeded(viewWidget->getPluginEditor(i), viewWidget->pluginsTabWidget, i);
    }

    dontTreatFocusIn = false;

    return true;
  }

  return false;
}

void PythonScriptView::closeModuleTabRequested(int tab) {
  QWidget *editorWidget = viewWidget->modulesTabWidget->widget(tab);
  dontTreatFocusIn = true;
  delete editorWidget;
  dontTreatFocusIn = false;
}

void PythonScriptView::closeMainScriptTabRequested(int tab) {
  QWidget *editorWidget = viewWidget->mainScriptsTabWidget->widget(tab);
  dontTreatFocusIn = true;
  delete editorWidget;
  dontTreatFocusIn = false;
}

void PythonScriptView::closePluginTabRequested(int tab) {
  std::string pluginFile = viewWidget->getPluginEditor(tab)->getFileName().toUtf8().data();
  editedPluginsClassName.erase(pluginFile);
  editedPluginsType.erase(pluginFile);
  editedPluginsName.erase(pluginFile);
  QWidget *editorWidget = viewWidget->pluginsTabWidget->widget(tab);
  dontTreatFocusIn = true;
  delete editorWidget;
  dontTreatFocusIn = false;
}

void PythonScriptView::newPythonPlugin() {
  PluginCreationDialog pluginCreationDialog(viewWidget);

  if (pluginCreationDialog.exec() == QDialog::Accepted) {
    QFile file(pluginCreationDialog.getPluginFileName());

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
      return;

    QFileInfo fileInfo(file);
    QString moduleName(fileInfo.fileName());
    QString modulePath(fileInfo.absolutePath());

    int editorId = viewWidget->addPluginEditor(fileInfo.absoluteFilePath());
    viewWidget->pluginsTabWidget->setTabToolTip(editorId, fileInfo.absoluteFilePath());
    pythonInterpreter->addModuleSearchPath(modulePath.toUtf8().data());
    viewWidget->pluginsTabWidget->setTabText(editorId, QString("[") + pluginCreationDialog.getPluginType() + QString("] ") + fileInfo.fileName());

    string pluginFile = fileInfo.absoluteFilePath().toUtf8().data();
    editedPluginsClassName[pluginFile] = pluginCreationDialog.getPluginClassName().toStdString();
    editedPluginsType[pluginFile] = pluginCreationDialog.getPluginType().toStdString();
    editedPluginsName[pluginFile] = pluginCreationDialog.getPluginName().toStdString();
    QString pluginSkeleton = getTulipPythonPluginSkeleton(pluginCreationDialog.getPluginClassName(),
                             pluginCreationDialog.getPluginType(),
                             pluginCreationDialog.getPluginName(),
                             pluginCreationDialog.getPluginAuthor(),
                             pluginCreationDialog.getPluginDate(),
                             pluginCreationDialog.getPluginInfos(),
                             pluginCreationDialog.getPluginRelease(),
                             pluginCreationDialog.getPluginGroup());

    viewWidget->getPluginEditor(editorId)->setPlainText(pluginSkeleton);
    savePythonPlugin();
  }
}


void PythonScriptView::registerPythonPlugin(bool clear) {
  int tabIdx = viewWidget->pluginsTabWidget->currentIndex();

  if (tabIdx == -1)
    return;

  savePythonPlugin();

  QString moduleNameExt = viewWidget->pluginsTabWidget->tabText(tabIdx);
  moduleNameExt = moduleNameExt.mid(moduleNameExt.lastIndexOf("]") + 2);
  QString moduleName;

  if (moduleNameExt[moduleNameExt.size() - 1] == '*')
    moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 4);
  else
    moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 3);

  moduleName = moduleName.replace(".py", "");

  pythonInterpreter->setConsoleWidget(viewWidget->consoleOutputWidget);

  if (clear) {
    pythonInterpreter->clearOutputBuffers();
    viewWidget->consoleOutputWidget->clear();
  }

  clearErrorIndicators();

  pythonInterpreter->deleteModule(moduleName.toUtf8().data());

  PythonInterpreter::getInstance()->runString("import tulipplugins;tulipplugins.setTestMode(True)");

  bool codeOk = false;
  QFileInfo fileInfo(viewWidget->getPluginEditor(tabIdx)->getFileName());

  if (fileInfo.fileName() == viewWidget->getPluginEditor(tabIdx)->getFileName()) {
    codeOk = PythonInterpreter::getInstance()->registerNewModuleFromString(moduleName.toUtf8().data(), viewWidget->getPluginCode(tabIdx));
  }
  else {
    codeOk = PythonInterpreter::getInstance()->reloadModule(moduleName.toUtf8().data());
  }

  PythonInterpreter::getInstance()->runString("tulipplugins.setTestMode(False)");
  ostringstream oss;
  string pluginFile = viewWidget->getPluginEditor(tabIdx)->getFileName().toUtf8().data();
  string pluginType = editedPluginsType[pluginFile];

  oss << "import " << moduleName.toUtf8().data() << endl;
  oss << "plugin = " << moduleName.toUtf8().data() << "." << editedPluginsClassName[pluginFile] << "(tlp.AlgorithmContext())";

  if (codeOk && PythonInterpreter::getInstance()->runString(oss.str())) {
    QList<int> sizes;
    sizes.push_back(viewWidget->height());
    sizes.push_back(0);
    viewWidget->splitter->setSizes(sizes);

    if (fileInfo.fileName() == viewWidget->getPluginEditor(tabIdx)->getFileName()) {
      PythonInterpreter::getInstance()->registerNewModuleFromString(moduleName.toUtf8().data(), viewWidget->getPluginCode(tabIdx));
    }
    else {
      PythonInterpreter::getInstance()->reloadModule(moduleName.toUtf8().data());
    }

    viewWidget->pluginStatusLabel->setText("Plugin has been successfully registered.");

  }
  else {
    QList<int> sizes;
    sizes.push_back(viewWidget->height() - 200);
    sizes.push_back(200);
    viewWidget->splitter->setSizes(sizes);
    viewWidget->pluginStatusLabel->setText("Plugin registration has failed.");
    indicateErrors();
  }

  pythonInterpreter->setDefaultConsoleWidget();

}
