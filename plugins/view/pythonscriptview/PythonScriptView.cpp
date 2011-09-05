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
#include <QtCore/QTime>
#include <QtCore/QTextStream>
#include <QtCore/QRegExp>
#include <QtGui/QInputDialog>

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

#include <sstream>

using namespace tlp;
using namespace std;

const string updateVisualizationFunc =
  "import tuliputils\n"
  "\n"
  "def updateVisualization(centerViews = True) :\n"
  "\ttuliputils.updateVisualization(centerViews)\n"
  "\n"
  "\n";

const string pauseScriptFunc =
  "import tuliputils\n"
  "\n"
  "def pauseScript() :\n"
  "\ttuliputils.pauseRunningScript()\n"
  "\n"
  "\n";



std::string cleanPropertyName(const std::string &propertyName) {
  std::string ret(propertyName);
  std::replace(ret.begin(), ret.end(), ' ', '_');
  int i = 0;

  while (pythonReservedCharacters[i]) {
    ret.erase(std::remove(ret.begin(), ret.end(), pythonReservedCharacters[i++]), ret.end());
  }

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
  oss << "#   * Ctrl + Space  : run script." << endl;
  oss << "#   * Ctrl + F  : find selected text." << endl;
  oss << "#   * Ctrl + R  : replace selected text." << endl << endl;

  oss << "from tulip import *" << endl << endl;;

  oss << "# the updateVisualization(centerViews = True) function can be called" << endl;
  oss << "# during script execution to update the opened views" << endl << endl;

  oss << "# the pauseScript() function can also be called to pause the script execution." << endl;
  oss << "# To resume the script execution, you will have to click on the \"Run script \" button." << endl << endl;

  oss << "# the main(graph) function must be defined " << endl;
  oss << "# to run the script on the current graph" << endl << endl;

  oss << "def main(graph) : " << endl;
  Iterator<PropertyInterface*> *itProps = graph->getObjectProperties();

  while (itProps->hasNext()) {
    PropertyInterface *prop = itProps->next();

    if (dynamic_cast<DoubleProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getDoubleProperty(\"" << prop->getName() << "\")" << endl;
    }

    if (dynamic_cast<LayoutProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getLayoutProperty(\"" << prop->getName() << "\")" << endl;
    }

    if (dynamic_cast<IntegerProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getIntegerProperty(\"" << prop->getName() << "\")" << endl;
    }

    if (dynamic_cast<StringProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getStringProperty(\"" << prop->getName() << "\")" << endl;
    }

    if (dynamic_cast<SizeProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getSizeProperty(\"" << prop->getName() << "\")" << endl;
    }

    if (dynamic_cast<BooleanProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getBooleanProperty(\"" << prop->getName() << "\")" << endl;
    }

    if (dynamic_cast<ColorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getColorProperty(\"" << prop->getName() << "\")" << endl;
    }

    if (dynamic_cast<GraphProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getGraphProperty(\"" << prop->getName() << "\")" << endl;
    }

    if (dynamic_cast<DoubleVectorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getDoubleVectorProperty(\"" << prop->getName() << "\")" << endl;
    }

    if (dynamic_cast<CoordVectorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getCoordVectorProperty(\"" << prop->getName() << "\")" << endl;
    }

    if (dynamic_cast<IntegerVectorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getIntegerVectorProperty(\"" << prop->getName() << "\")" << endl;
    }

    if (dynamic_cast<SizeVectorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getSizeVectorProperty(\"" << prop->getName() << "\")" << endl;
    }

    if (dynamic_cast<BooleanVectorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getBooleanVectorProperty(\"" << prop->getName() << "\")" << endl;
    }

    if (dynamic_cast<ColorVectorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(prop->getName()) << " =  graph.getColorVectorProperty(\"" << prop->getName() << "\")" << endl;
    }
  }

  oss << "\n\tfor n in graph.getNodes():" << endl;
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
    pluginClass = "tlp.PythonImportModule";
    tlpType = "Import";
  }
  else {
    pluginClass = "tlp.PythonExportModule";
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
    textStream << "\t\treturn (True, \"Ok\")" << endl << endl;
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
    textStream << "\tdef exportGraph(self):" << endl;
    textStream << "\t\t# This method is called to export a graph." << endl;
    textStream << "\t\t# The graph to export is accessible through the \"graph\" class attribute" << endl;
    textStream << "\t\t# (see documentation of class tlp.Graph)." << endl << endl;
    textStream << "\t\t# The parameters provided by the user are stored in a Tulip DataSet " << endl;
    textStream << "\t\t# and can be accessed through the \"dataSet\" class attribute" << endl ;
    textStream << "\t\t# (see documentation of class tlp.DataSet)." << endl << endl;
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

static bool scriptPaused = false;

VIEWPLUGIN(PythonScriptView, "Python Script view", "Antoine Lambert", "04/2010", "Python Script View", "0.7");

PythonScriptView::PythonScriptView() : pythonInterpreter(NULL),graph(NULL), scriptStopped(false), runningScript(false) {}

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

  // hack to get a pointer on the cluster hierarchy widget
  // This way, we can update it after executing a script (there is some refresh issue otherwise)
  MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());
  clusterTreeWidget = NULL;

  if (mainController) {
    QWidget *mainWindow = mainController->getMainWindowFacade()->getParentWidget();
    QObjectList childWidgets = mainWindow->children();

    while (!childWidgets.empty()) {
      QObject *obj = childWidgets.front();
      clusterTreeWidget = dynamic_cast<SGHierarchyWidget *>(obj);

      if (clusterTreeWidget) {
        break;
      }
      else {
        childWidgets.pop_front();
        childWidgets += obj->children();
      }
    }
  }

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

  viewWidget->pythonShellWidget->setCurrentGraph(graph);

  if (viewWidget->mainScriptsTabWidget->count() == 0) {

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
        mainScriptLoaded = loadScript(mainScript.c_str());

        if (!mainScriptLoaded) {
          string mainScriptSrc;
          oss.str("");
          oss << "main_script_src" << i;

          if (mainScriptsDataSet.get(oss.str(), mainScriptSrc)) {
            int mainScriptId = viewWidget->addMainScriptEditor();
            editedMainScripts[mainScriptId] = "";
            PythonCodeEditor *codeEditor = viewWidget->getMainScriptEditor(mainScriptId);
            // TLPParser seems to replace the tab character with four white spaces when reading the content of the TLP file, don't know why
            // Anyway, replace the original tab character in order to have a correct indentation when setting the script text to the code editor
            replaceAll(mainScriptSrc, "    ", "\t");
            codeEditor->setText(QString::fromStdString(mainScriptSrc));
            viewWidget->mainScriptsTabWidget->setTabText(mainScriptId, "[no file]");
            viewWidget->mainScriptsTabWidget->setTabToolTip(mainScriptId, "string main script, don't forget to save the current graph or\n save script to file to not lose modifications to source code.");
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
        mainScriptLoaded = loadScript(filename.c_str());
      }

      if (!mainScriptLoaded) {
        editedMainScripts[viewWidget->addMainScriptEditor()] = "";
        PythonCodeEditor *codeEditor = viewWidget->getMainScriptEditor(0);

        if (dataSet.get("script code", scriptCode)) {
          replaceAll(scriptCode, "    ", "\t");
          codeEditor->setText(QString::fromStdString(scriptCode));
          viewWidget->mainScriptsTabWidget->setTabText(0, "[no file]");
          viewWidget->mainScriptsTabWidget->setTabToolTip(0, "string main script, don't forget to save the current graph or\n save script to file to not lose modifications to source code.");
        }
        else {
          codeEditor->setText(getDefaultScriptCode(pythonInterpreter->getPythonVersion(), graph).c_str());
          viewWidget->mainScriptsTabWidget->setTabText(0, "[no file]");
          viewWidget->mainScriptsTabWidget->setTabToolTip(0, "string main script, don't forget to save the current graph or\n save script to file to not lose modifications to source code.");
        }
      }

    }

    pythonInterpreter->setConsoleWidget(viewWidget->consoleOutputWidget);

    DataSet modulesDataSet;

    if (dataSet.get("modules", modulesDataSet)) {
      int i = 0;
      string module;
      ostringstream oss;
      oss << "module" << i;

      while (modulesDataSet.get(oss.str(), module)) {
        bool moduleLoaded = loadModule(module.c_str());

        if (!moduleLoaded) {
          string moduleSrc;
          oss.str("");
          oss << "module_src" << i;

          if (modulesDataSet.get(oss.str(), moduleSrc)) {
            replaceAll(moduleSrc, "    ", "\t");
            QFileInfo fileInfo(module.c_str());
            loadModuleFromSrcCode(fileInfo.fileName().toStdString(), moduleSrc);
          }
        }

        oss.str("");
        oss << "module" << ++i;
      }
    }

    pythonInterpreter->setDefaultConsoleWidget();

  }

  this->graph = graph;
}

void PythonScriptView::getData(Graph **graph,DataSet *dataSet) {
  *graph = this->graph;

  dataSet->set("main script file", editedMainScripts[viewWidget->mainScriptsTabWidget->currentIndex()]);
  string scriptCode = viewWidget->getCurrentMainScriptCode();
  dataSet->set("script code", scriptCode);

  map<int, string>::const_iterator it = editedMainScripts.begin();
  DataSet mainScriptsDataSet;
  int i = 0;

  for ( ; it != editedMainScripts.end() ; ++it) {
    ostringstream oss;
    oss << "main_script" << i;
    mainScriptsDataSet.set(oss.str(), it->second);
    oss.str("");
    oss << "main_script_src" << i++;
    mainScriptsDataSet.set(oss.str(), viewWidget->getMainScriptCode(it->first));
  }

  mainScriptsDataSet.set("main_script_id", viewWidget->mainScriptsTabWidget->currentIndex());
  dataSet->set("main_scripts", mainScriptsDataSet);

  it = editedModules.begin();
  DataSet modulesDataSet;
  i = 0;

  for ( ; it != editedModules.end() ; ++it) {
    ostringstream oss;
    oss << "module" << i;
    modulesDataSet.set(oss.str(), it->second);
    oss.str("");
    oss << "module_src" << i++;
    modulesDataSet.set(oss.str(), viewWidget->getModuleCode(it->first));
  }

  dataSet->set("modules", modulesDataSet);
}

QTime timer;

int tracefunc(PyObject *, PyFrameObject *, int what, PyObject *) {

  if (what == PyTrace_LINE) {
    if (!scriptPaused && timer.elapsed() >= 50) {
      if (QApplication::hasPendingEvents())
        QApplication::processEvents();

      timer.start();
    }

    while (scriptPaused) {
      if (timer.elapsed() >= 50) {
        if (QApplication::hasPendingEvents())
          QApplication::processEvents();

        timer.start();
      }
    }

  }

  return 0;
}

void PythonScriptView::executeCurrentScript() {

  if (scriptPaused) {
    Observable::holdObservers();
    scriptPaused = false;
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


    int curMainScriptId = viewWidget->mainScriptsTabWidget->currentIndex();

    if (editedMainScripts[curMainScriptId] != "") {
      saveScript();
    }

    saveAllModules();
    reloadAllModules();

    map<int, string>::const_iterator it;

    for (it = editedMainScripts.begin() ; it != editedMainScripts.end() ; ++it) {
      PythonCodeEditor *codeEditor = static_cast<PythonCodeEditor *>(viewWidget->mainScriptsTabWidget->widget(it->first));
      codeEditor->clearErrorIndicator();
    }

    for (it = editedModules.begin() ; it != editedModules.end() ; ++it) {
      PythonCodeEditor *codeEditor = static_cast<PythonCodeEditor *>(viewWidget->modulesTabWidget->widget(it->first));
      codeEditor->clearErrorIndicator();
    }

    graph->push();
    Observable::holdObservers();

    pythonInterpreter->setTraceFunction(tracefunc);
    pythonInterpreter->runString(viewWidget->getCurrentMainScriptCode().c_str());

    viewWidget->scriptStatusLabel->setText("Executing script ...");
    viewWidget->progressBar->setRange(0,0);


    viewWidget->runScriptButton->setEnabled(false);
    viewWidget->stopScriptButton->setEnabled(true);
    viewWidget->pauseScriptButton->setEnabled(true);

    timer.start();

    bool scriptExecOk = pythonInterpreter->runGraphScript("__main__", "main", graph);

    pythonInterpreter->setTraceFunction(NULL);
    viewWidget->stopScriptButton->setEnabled(false);
    viewWidget->runScriptButton->setEnabled(true);
    viewWidget->pauseScriptButton->setEnabled(false);

    if (scriptExecOk) {
      viewWidget->scriptStatusLabel->setText("Script execution has succeed");
      pythonInterpreter->runString("del main");

      if (clusterTreeWidget)
        clusterTreeWidget->update();

    }
    else {

      viewWidget->scriptStatusLabel->setText("Script execution has failed");

      if (!scriptStopped) {
        QRegExp rx("^.*File.*\"(.*)\".*line.*(\\d+).*$");

        map<string, vector<int> > errorLines;
        QString consoleOutput = pythonInterpreter->getStandardErrorOutput().c_str();
        QStringList outputLines = consoleOutput.split("\n");

        for (int i = 0 ; i < outputLines.count() - 1 ; ++i) {
          int pos = 0;

          while ((pos = rx.indexIn(outputLines[i], pos)) != -1) {
            string file = rx.cap(1).toStdString();
            int line = rx.cap(2).toInt();
            errorLines[file].push_back(line);
            pos += rx.matchedLength();
          }

        }

        for (size_t i = 0 ; i < errorLines["<string>"].size() ; ++i) {
          if (errorLines["<string>"][i] > 1) {
            viewWidget->getCurrentMainScriptEditor()->indicateScriptCurrentError(errorLines["<string>"][i]-1);
          }
        }

        for (it = editedModules.begin() ; it != editedModules.end() ; ++it) {
          if (errorLines.find(it->second) != errorLines.end()) {
            const vector<int> &linesErrorNumbers = errorLines[it->second];
            PythonCodeEditor *codeEditor = static_cast<PythonCodeEditor *>(viewWidget->modulesTabWidget->widget(it->first));

            for (size_t i = 0 ; i < linesErrorNumbers.size() ; ++i) {
              codeEditor->indicateScriptCurrentError(linesErrorNumbers[i]-1);
            }
          }
        }
      }

      graph->pop();
    }

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


void PythonScriptView::stopCurrentScript() {
  scriptStopped = true;
  scriptPaused = false;
  pythonInterpreter->stopCurrentScript();
}

void PythonScriptView::pauseCurrentScript() {
  Observable::unholdObservers();
  scriptPaused = true;
  viewWidget->pauseScriptButton->setEnabled(false);
  viewWidget->runScriptButton->setEnabled(true);
  viewWidget->progressBar->setRange(0,100);
  viewWidget->progressBar->reset();
  viewWidget->scriptStatusLabel->setText("Script execution is paused.");
}

void PythonScriptView::newScript() {
  int editorId = viewWidget->addMainScriptEditor();
  editedMainScripts[editorId] = "";
  viewWidget->getMainScriptEditor(editorId)->setText(getDefaultScriptCode(pythonInterpreter->getPythonVersion(), graph).c_str());
  viewWidget->mainScriptsTabWidget->setTabText(editorId, "[no file]");
  viewWidget->mainScriptsTabWidget->setTabToolTip(editorId, "string main script, don't forget to save the current graph or\n save script to file to not lose modifications to source code.");
}

void PythonScriptView::loadScript() {
  QString fileName = QFileDialog::getOpenFileName(viewWidget, "Open Main Script","", "Python script (*.py)");
  loadScript(fileName);
}

bool PythonScriptView::loadScript(const QString &fileName) {
  QFile file(fileName);
  QFileInfo fileInfo(file);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return false;

  QString scriptCode;

  while (!file.atEnd()) {
    scriptCode += file.readLine();
  }

  int editorId = viewWidget->addMainScriptEditor();
  editedMainScripts[editorId] = fileInfo.absoluteFilePath().toStdString();
  viewWidget->getMainScriptEditor(editorId)->setText(scriptCode);
  viewWidget->mainScriptsTabWidget->setTabText(editorId, fileInfo.fileName());
  viewWidget->mainScriptsTabWidget->setTabToolTip(editorId, fileInfo.absoluteFilePath());

  file.close();

  return true;
}

void PythonScriptView::saveScript() {
  QString fileName;
  int curMainEditorId = viewWidget->mainScriptsTabWidget->currentIndex();
  QString mainScriptFileName = editedMainScripts[curMainEditorId].c_str();

  if (mainScriptFileName == "")
    fileName = QFileDialog::getSaveFileName(viewWidget, tr("Save Main Script"),"","Python script (*.py)");
  else
    fileName = mainScriptFileName;

  if (fileName != "") {
    if (!fileName.endsWith(".py"))
      fileName += ".py";

    QFile file(fileName);
    QFileInfo fileInfo(file);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
      return;

    editedMainScripts[curMainEditorId] = fileInfo.absoluteFilePath().toStdString();
    QTextStream out(&file);
    out << viewWidget->getCurrentMainScriptCode().c_str();
    viewWidget->mainScriptsTabWidget->setTabText(curMainEditorId, fileInfo.fileName());
    file.close();
  }
}

void PythonScriptView::loadModule() {
  QString fileName = QFileDialog::getOpenFileName(viewWidget, "Open Module","", "Python script (*.py)");
  loadModule(fileName);
}


bool PythonScriptView::loadModule(const QString &fileName) {
  QFile file(fileName);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return false;

  QFileInfo fileInfo(file);
  QString moduleName(fileInfo.fileName());
  QString modulePath(fileInfo.absolutePath());

  QString scriptCode;

  while (!file.atEnd()) {
    scriptCode += file.readLine();
  }

  file.close();

  int editorId = viewWidget->addModuleEditor();
  PythonCodeEditor *codeEditor = viewWidget->getModuleEditor(editorId);

  pythonInterpreter->addModuleSearchPath(modulePath.toStdString());

  editedModules[editorId] = fileInfo.absoluteFilePath().toStdString();

  codeEditor->setText(scriptCode);

  viewWidget->modulesTabWidget->setTabText(editorId, fileInfo.fileName());
  viewWidget->modulesTabWidget->setTabToolTip(editorId, fileInfo.absoluteFilePath());

  pythonInterpreter->reloadModule(moduleName.replace(".py", "").toStdString());

  return true;
}

bool PythonScriptView::loadModuleFromSrcCode(const std::string &moduleName, const std::string &moduleSrcCode) {
  int editorId = viewWidget->addModuleEditor();
  PythonCodeEditor *codeEditor = viewWidget->getModuleEditor(editorId);
  editedModules[editorId] = moduleName;
  codeEditor->setText(moduleSrcCode.c_str());
  viewWidget->modulesTabWidget->setTabText(editorId, moduleName.c_str());
  viewWidget->modulesTabWidget->setTabToolTip(editorId, "string module, don't forget to save the current graph or\n save module to file to not lose modifications to source code.");

  pythonInterpreter->registerNewModuleFromString(QString(moduleName.c_str()).replace(".py", "").toStdString(),  moduleSrcCode);

  return true;
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

  int editorId = viewWidget->addModuleEditor();
  viewWidget->modulesTabWidget->setTabToolTip(editorId, fileInfo.absoluteFilePath());
  pythonInterpreter->addModuleSearchPath(modulePath.toStdString());
  viewWidget->modulesTabWidget->setTabText(editorId, fileInfo.fileName());

  editedModules[editorId] = fileInfo.absoluteFilePath().toStdString();

  file.close();
}

void PythonScriptView::newStringModule() {
  bool ok;
  QString moduleName = QInputDialog::getText(this->viewWidget, "New string module  ", "module name :", QLineEdit::Normal,"", &ok);

  if (ok && !moduleName.isEmpty()) {

    if (!moduleName.endsWith(".py"))
      moduleName += ".py";

    int editorId = viewWidget->addModuleEditor();
    viewWidget->modulesTabWidget->setTabText(editorId, moduleName);
    editedModules[editorId] = moduleName.toStdString();

    viewWidget->modulesTabWidget->setTabToolTip(editorId, "string module, don't forget to save the current graph or\n save module to file to not lose modifications to source code.");
  }
}

void PythonScriptView::saveModuleToFile() {
  QString fileName = QFileDialog::getSaveFileName(viewWidget, tr("Set Module filename"),"","Python script (*.py)");

  if (!fileName.isEmpty()) {
    if (!fileName.endsWith(".py"))
      fileName += ".py";

    int tabIdx = viewWidget->modulesTabWidget->currentIndex();
    editedModules[tabIdx] = fileName.toStdString();
    saveModule(tabIdx);
  }
}

void PythonScriptView::saveModule() {
  int curModule = viewWidget->modulesTabWidget->currentIndex();

  if (curModule == -1)
    return;

  saveModule(curModule);
}

void PythonScriptView::loadPythonPlugin() {
  QString fileName = QFileDialog::getOpenFileName(viewWidget, "Open Tulip Python plugin","", "Python script (*.py)");
  QFile file(fileName);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return ;

  QFileInfo fileInfo(file);
  QString moduleName(fileInfo.fileName());
  QString modulePath(fileInfo.absolutePath());

  QString pluginCode;

  while (!file.atEnd()) {
    pluginCode += file.readLine();
  }

  file.close();

  QString s = "tulipplugins.register";
  int idx1 = pluginCode.indexOf(s);

  if (idx1 == -1) {
    QMessageBox::critical(viewWidget, "Error", "This file does not seem to contain the source code of a Tulip Python plugin.");
  }
  else {
    idx1 += s.length();
    int idx2 = pluginCode.indexOf("Plugin", idx1);
    QString tlpType = pluginCode.mid(idx1, idx2 - idx1);
    QString pluginType = pluginCode.mid(idx1, idx2 - idx1);
    QString pluginClass = "";
    QString pluginClassName = "";
    QString pluginName = "";

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
      pluginClass = "tlp.PythonImportModule";
    }
    else {
      pluginType = "Export";
      pluginClass = "tlp.PythonExportModule";
    }

    ostringstream oss;
    oss << "^.*class (.*)\\(" << pluginClass.toStdString() << "\\).*$";
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

    if (pluginClassName == "" || pluginName == "") {
      QMessageBox::critical(viewWidget, "Error", "Unable to retrieve the plugin class name and the plugin name from the source code\n.");
    }
    else {
      int editorId = viewWidget->addPluginEditor();
      PythonCodeEditor *codeEditor = viewWidget->getPluginEditor(editorId);
      codeEditor->setText(pluginCode);
      pythonInterpreter->addModuleSearchPath(modulePath.toStdString());
      viewWidget->pluginsTabWidget->setTabToolTip(editorId, fileInfo.absoluteFilePath());
      viewWidget->pluginsTabWidget->setTabText(editorId, QString("[") + pluginType + QString("] ") + fileInfo.fileName());
      editedPlugins[editorId] = fileInfo.absoluteFilePath().toStdString();
      editedPluginsClassName[editedPlugins[editorId]] = pluginClassName.toStdString();
      editedPluginsType[editedPlugins[editorId]] = pluginType.toStdString();
      editedPluginsName[editedPlugins[editorId]] = pluginName.toStdString();
    }
  }
}

void PythonScriptView::savePythonPlugin() {
  int tabIdx = viewWidget->pluginsTabWidget->currentIndex();

  if (tabIdx == -1)
    return;

  QString moduleNameExt = viewWidget->pluginsTabWidget->tabText(tabIdx);
  QString moduleName;

  if (moduleNameExt[moduleNameExt.size() - 1] == '*')
    moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 4);
  else
    moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 3);

  viewWidget->pluginsTabWidget->setTabText(tabIdx, moduleName+".py");
  QFile file(editedPlugins[tabIdx].c_str());
  QFileInfo fileInfo(file);

  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream out(&file);
    out << viewWidget->getPluginCode(tabIdx).c_str();
    file.close();
    viewWidget->pluginsTabWidget->setTabToolTip(tabIdx, fileInfo.absoluteFilePath());
  }
}

void PythonScriptView::saveModule(int tabIdx) {
  if (editedModules.find(tabIdx) != editedModules.end()) {
    QString moduleNameExt = viewWidget->modulesTabWidget->tabText(tabIdx);
    QString moduleName;

    if (moduleNameExt[moduleNameExt.size() - 1] == '*')
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 4);
    else
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 3);

    pythonInterpreter->deleteModule(moduleName.toStdString());
    viewWidget->modulesTabWidget->setTabText(tabIdx, moduleName+".py");
    QFile file(editedModules[tabIdx].c_str());
    QFileInfo fileInfo(file);

    if (fileInfo.fileName() != editedModules[tabIdx].c_str() && file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      QTextStream out(&file);
      out << viewWidget->getModuleCode(tabIdx).c_str();
      file.close();
      viewWidget->modulesTabWidget->setTabToolTip(tabIdx, fileInfo.absoluteFilePath());
    }
  }
}

void PythonScriptView::saveAllModules() {
  map<int, string>::const_iterator it;

  for (it = editedModules.begin() ; it != editedModules.end() ; ++it) {
    saveModule(it->first);

  }
}

void PythonScriptView::reloadAllModules() {
  map<int, string>::const_iterator it;

  for (it = editedModules.begin() ; it != editedModules.end() ; ++it) {
    QsciScintilla *codeEditor = static_cast<QsciScintilla *>(viewWidget->modulesTabWidget->widget(it->first));
    QString moduleNameExt = viewWidget->modulesTabWidget->tabText(it->first);
    QString moduleName;

    if (moduleNameExt[moduleNameExt.size() - 1] == '*')
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 4);
    else
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 3);

    pythonInterpreter->deleteModule(moduleName.toStdString());
    QFileInfo fileInfo((it->second).c_str());

    if (fileInfo.fileName() == (it->second).c_str()) {
      pythonInterpreter->registerNewModuleFromString(moduleName.toStdString(),  codeEditor->text().toStdString());
    }
    else {
      pythonInterpreter->addModuleSearchPath(fileInfo.absolutePath().toStdString());
      pythonInterpreter->reloadModule(moduleName.toStdString());
    }
  }
}

bool PythonScriptView::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvt = static_cast<QKeyEvent *>(event);

    if (keyEvt->modifiers() == Qt::ControlModifier && keyEvt->key() == Qt::Key_S) {
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
    else if (obj == viewWidget->getCurrentMainScriptEditor() && keyEvt->modifiers() == Qt::ControlModifier && keyEvt->key() == Qt::Key_Space) {
      executeCurrentScript();
      return true;
    }
  }

  return false;
}

void PythonScriptView::closeModuleTabRequested(int tab) {
  editedModules.erase(tab);
  QWidget *editorWidget = viewWidget->modulesTabWidget->widget(tab);
  viewWidget->modulesTabWidget->removeTab(tab);
  delete editorWidget;
  std::map<int, std::string> editedModulesCp(editedModules);
  std::map<int, std::string>::iterator it;

  for (it = editedModules.begin() ; it != editedModules.end() ; ++it) {
    if (it->first > tab) {
      editedModulesCp[it->first - 1] = editedModules[it->first];
      editedModulesCp.erase(it->first);
    }
  }

  editedModules = editedModulesCp;
}

void PythonScriptView::closeMainScriptTabRequested(int tab) {
  editedMainScripts.erase(tab);
  QWidget *editorWidget = viewWidget->mainScriptsTabWidget->widget(tab);
  viewWidget->mainScriptsTabWidget->removeTab(tab);
  delete editorWidget;
  std::map<int, std::string> editedMainScriptsCp(editedMainScripts);
  std::map<int, std::string>::iterator it;

  for (it = editedMainScripts.begin() ; it != editedMainScripts.end() ; ++it) {
    if (it->first > tab) {
      editedMainScriptsCp[it->first - 1] = editedMainScripts[it->first];
      editedMainScriptsCp.erase(it->first);
    }
  }

  editedMainScripts = editedMainScriptsCp;
}

void PythonScriptView::closePluginTabRequested(int tab) {
  editedPluginsClassName.erase(editedPlugins[tab]);
  editedPluginsType.erase(editedPlugins[tab]);
  editedPluginsName.erase(editedPlugins[tab]);
  editedPlugins.erase(tab);
  QWidget *editorWidget = viewWidget->pluginsTabWidget->widget(tab);
  viewWidget->pluginsTabWidget->removeTab(tab);
  delete editorWidget;
  std::map<int, std::string> editedPluginsCp(editedPlugins);
  std::map<int, std::string>::iterator it;

  for (it = editedPlugins.begin() ; it != editedPlugins.end() ; ++it) {
    if (it->first > tab) {
      editedPluginsCp[it->first - 1] = editedPlugins[it->first];
      editedPluginsCp.erase(it->first);
    }
  }

  editedPlugins = editedPluginsCp;
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

    int editorId = viewWidget->addPluginEditor();
    viewWidget->pluginsTabWidget->setTabToolTip(editorId, fileInfo.absoluteFilePath());
    pythonInterpreter->addModuleSearchPath(modulePath.toStdString());
    viewWidget->pluginsTabWidget->setTabText(editorId, QString("[") + pluginCreationDialog.getPluginType() + QString("] ") + fileInfo.fileName());

    editedPlugins[editorId] = fileInfo.absoluteFilePath().toStdString();
    editedPluginsClassName[editedPlugins[editorId]] = pluginCreationDialog.getPluginClassName().toStdString();
    editedPluginsType[editedPlugins[editorId]] = pluginCreationDialog.getPluginType().toStdString();
    editedPluginsName[editedPlugins[editorId]] = pluginCreationDialog.getPluginName().toStdString();
    QString pluginSkeleton = getTulipPythonPluginSkeleton(pluginCreationDialog.getPluginClassName(),
                             pluginCreationDialog.getPluginType(),
                             pluginCreationDialog.getPluginName(),
                             pluginCreationDialog.getPluginAuthor(),
                             pluginCreationDialog.getPluginDate(),
                             pluginCreationDialog.getPluginInfos(),
                             pluginCreationDialog.getPluginRelease(),
                             pluginCreationDialog.getPluginGroup());

    viewWidget->getPluginEditor(editorId)->setText(pluginSkeleton);
    savePythonPlugin();
  }
}

template <typename T>
static bool pluginExists(std::string pluginName) {
  std::map< std::string, tlp::TemplateFactoryInterface* >::iterator it = tlp::TemplateFactoryInterface::allFactories->begin();

  for (; it != tlp::TemplateFactoryInterface::allFactories->end() ; ++it) {
    if (it->first == tlp::demangleTlpClassName(typeid(T).name()) && it->second->pluginExists(pluginName)) {
      return true;
    }
  }

  return false;
}

template <typename T>
static void removePlugin(std::string pluginName) {
  std::map< std::string, tlp::TemplateFactoryInterface* >::iterator it = tlp::TemplateFactoryInterface::allFactories->begin();

  for (; it != tlp::TemplateFactoryInterface::allFactories->end() ; ++it) {
    if (it->first == tlp::demangleTlpClassName(typeid(T).name())) {
      it->second->removePlugin(pluginName);
      return;
    }
  }
}

static void removePluginAndUpdateGui(const string &pluginName, const string &pluginType) {
  if (pluginType == "General") {
    if (pluginExists<tlp::Algorithm>(pluginName)) {
      removePlugin<tlp::Algorithm>(pluginName);
    }
  }
  else if (pluginType == "Layout") {
    if (pluginExists<tlp::LayoutAlgorithm>(pluginName)) {
      removePlugin<tlp::LayoutAlgorithm>(pluginName);
    }
  }
  else if (pluginType == "Size") {
    if (pluginExists<tlp::SizeAlgorithm>(pluginName)) {
      removePlugin<tlp::SizeAlgorithm>(pluginName);
    }
  }
  else if (pluginType == "Color") {
    if (pluginExists<tlp::ColorAlgorithm>(pluginName)) {
      removePlugin<tlp::ColorAlgorithm>(pluginName);
    }
  }
  else if (pluginType == "Measure") {
    if (pluginExists<tlp::DoubleAlgorithm>(pluginName)) {
      removePlugin<tlp::DoubleAlgorithm>(pluginName);
    }
  }
  else if (pluginType == "Selection") {
    if (pluginExists<tlp::BooleanAlgorithm>(pluginName)) {
      removePlugin<tlp::BooleanAlgorithm>(pluginName);
    }
  }
  else if (pluginType == "Import") {
    if (pluginExists<tlp::ImportModule>(pluginName)) {
      removePlugin<tlp::ImportModule>(pluginName);
    }
  }
  else if (pluginType == "Export") {
    if (pluginExists<tlp::ExportModule>(pluginName)) {
      removePlugin<tlp::ExportModule>(pluginName);
    }
  }

  PythonInterpreter::getInstance()->runString("tuliputils.updatePluginsMenus()");
}

void PythonScriptView::registerPythonPlugin() {
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

  PythonInterpreter::getInstance()->runString("import tulipplugins;tulipplugins.setTestMode(True)");
  PythonInterpreter::getInstance()->reloadModule(moduleName.toStdString());
  PythonInterpreter::getInstance()->runString("tulipplugins.setTestMode(False)");
  ostringstream oss;
  string pluginType = editedPluginsType[editedPlugins[tabIdx]];

  if (pluginType == "General" || pluginType == "Import" || pluginType == "Export") {
    oss << "plugin = " << moduleName.toStdString() << "." << editedPluginsClassName[editedPlugins[tabIdx]] << "(tlp.AlgorithmContext())";
  }
  else {
    oss << "plugin = " << moduleName.toStdString() << "." << editedPluginsClassName[editedPlugins[tabIdx]] << "(tlp.PropertyContext())";
  }

  pythonInterpreter->setConsoleWidget(viewWidget->consoleOutputWidget);
  viewWidget->consoleOutputWidget->clear();
  removePluginAndUpdateGui(editedPluginsName[editedPlugins[tabIdx]], pluginType);

  if (PythonInterpreter::getInstance()->runString(oss.str())) {
    QList<int> sizes;
    sizes.push_back(viewWidget->height());
    sizes.push_back(0);
    viewWidget->splitter->setSizes(sizes);
    PythonInterpreter::getInstance()->reloadModule(moduleName.toStdString());
    viewWidget->pluginStatusLabel->setText("Plugin has been successfully registered.");

  }
  else {
    QList<int> sizes;
    sizes.push_back(viewWidget->height() - 200);
    sizes.push_back(200);
    viewWidget->splitter->setSizes(sizes);
    viewWidget->pluginStatusLabel->setText("Plugin registration has failed.");
  }

  pythonInterpreter->setDefaultConsoleWidget();

}
