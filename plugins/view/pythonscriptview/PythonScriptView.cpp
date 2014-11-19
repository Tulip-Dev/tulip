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

#include <tulip/PythonInterpreter.h>
#include <tulip/PythonCodeEditor.h>
#include <tulip/PythonVersionChecker.h>
#include <tulip/Graph.h>
#include <tulip/DoubleProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/GraphProperty.h>
#include <tulip/Algorithm.h>
#include <tulip/ImportModule.h>
#include <tulip/ExportModule.h>
#include <tulip/PluginLister.h>
#include <tulip/TlpQtTools.h>

#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QRegExp>
#include <QInputDialog>
#include <QMessageBox>

#include <QLabel>

#include <sstream>

#include "PythonScriptView.h"
#include "PythonScriptViewWidget.h"
#include "ui_PythonScriptViewWidget.h"

using namespace tlp;
using namespace std;

const QString updateVisualizationFunc =
  "import tuliputils\n"
  "\n"
  "def updateVisualization(centerViews = True):\n"
  "\ttuliputils.updateVisualization(centerViews)\n"
  "\n"
  "\n";

const QString pauseScriptFunc =
  "import tuliputils\n"
  "\n"
  "def pauseScript():\n"
  "\ttuliputils.pauseRunningScript()\n"
  "\n"
  "\n";

const QString runGraphScriptFunc =
  "import tuliputils\n"
  "\n"
  "def runGraphScript(scriptFile, graph):\n"
  "\ttuliputils.runGraphScript(scriptFile, graph)\n"
  "\n"
  "\n";

QString cleanPropertyName(const QString &propertyName) {
  QString ret(propertyName);
  ret.replace(' ', '_');
  int i = 0;

  while (PythonInterpreter::pythonReservedCharacters[i]) {
    ret.remove(PythonInterpreter::pythonReservedCharacters[i++]);
  }

  i = 0;

  while (PythonInterpreter::pythonKeywords[i]) {
    if (ret == PythonInterpreter::pythonKeywords[i++]) {
      ret += "_";
      break;
    }
  }

  QString builtinModName = "__builtin__";

  if (PythonInterpreter::getInstance()->getPythonVersion() >= 3.0) {
    builtinModName = "builtins";
  }

  PythonInterpreter::getInstance()->importModule(builtinModName);
  static QVector<QString> builtinDictContent = PythonInterpreter::getInstance()->getObjectDictEntries(builtinModName);

  for (i = 0 ; i < builtinDictContent.size() ; ++i) {
    if (ret == builtinDictContent[i]) {
      ret += "_";
      break;
    }
  }

  ret.replace("\"", "");

  return ret;
}

QString getDefaultScriptCode(const QString &pythonVersion, Graph *graph) {
  QString scriptCode;
  QTextStream oss(&scriptCode);

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
    QString cleanPropName(tlp::tlpStringToQString(prop->getName()));
    cleanPropName.replace("\"", "\\\"");

    if (dynamic_cast<DoubleProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(tlp::tlpStringToQString(prop->getName())) << " = graph.getDoubleProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<LayoutProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(tlp::tlpStringToQString(prop->getName())) << " = graph.getLayoutProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<IntegerProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(tlp::tlpStringToQString(prop->getName())) << " = graph.getIntegerProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<StringProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(tlp::tlpStringToQString(prop->getName())) << " = graph.getStringProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<SizeProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(tlp::tlpStringToQString(prop->getName())) << " = graph.getSizeProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<BooleanProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(tlp::tlpStringToQString(prop->getName())) << " = graph.getBooleanProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<ColorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(tlp::tlpStringToQString(prop->getName())) << " = graph.getColorProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<GraphProperty *>(prop)) {
#ifdef NDEBUG

      if(cleanPropName!="viewMetaGraph")
#endif
        oss << "\t"<< cleanPropertyName(tlp::tlpStringToQString(prop->getName())) << " = graph.getGraphProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<DoubleVectorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(tlp::tlpStringToQString(prop->getName())) << " = graph.getDoubleVectorProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<CoordVectorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(tlp::tlpStringToQString(prop->getName())) << " = graph.getCoordVectorProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<IntegerVectorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(tlp::tlpStringToQString(prop->getName())) << " = graph.getIntegerVectorProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<SizeVectorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(tlp::tlpStringToQString(prop->getName())) << " = graph.getSizeVectorProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<BooleanVectorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(tlp::tlpStringToQString(prop->getName())) << " = graph.getBooleanVectorProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<ColorVectorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(tlp::tlpStringToQString(prop->getName())) << " = graph.getColorVectorProperty(\"" << cleanPropName << "\")" << endl;
    }

    if (dynamic_cast<StringVectorProperty *>(prop)) {
      oss << "\t"<< cleanPropertyName(tlp::tlpStringToQString(prop->getName())) << " = graph.getStringVectorProperty(\"" << cleanPropName << "\")" << endl;
    }
  }

  oss << "\n\tfor n in graph.getNodes():" << endl;

  if (PythonInterpreter::getInstance()->getPythonVersion() >= 3.0)
    oss << "\t\tprint(n)" << endl;
  else
    oss << "\t\tprint n" << endl;

  return scriptCode;
}


PLUGIN(PythonScriptView)

PythonScriptView::PythonScriptView(tlp::PluginContext *) : _viewWidget(NULL), _pythonInterpreter(PythonInterpreter::getInstance()),
  _graph(NULL), _scriptStopped(false), _runningScript(false) {
}

PythonScriptView::~PythonScriptView() {
  //  delete _viewWidget;
}

void PythonScriptView::setupWidget() {

  _viewWidget = new PythonScriptViewWidget(this);
  setCentralWidget(_viewWidget);
  _pythonInterpreter->runString(updateVisualizationFunc);
  _pythonInterpreter->runString(pauseScriptFunc);
  _pythonInterpreter->runString(runGraphScriptFunc);
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

void PythonScriptView::graphChanged(Graph *graph) {
  _graph = graph;
  _viewWidget->setGraph(graph);
}

void PythonScriptView::setState(const tlp::DataSet &dataSet) {
  if (_viewWidget->numberOfScriptEditors() == 0) {

    _viewWidget->setMainTabWidgetIndex(1);
    _pythonInterpreter->setConsoleWidget(_viewWidget->consoleWidget());
    _pythonInterpreter->clearOutputBuffers();
    DataSet modulesDataSet;

    if (dataSet.get("modules", modulesDataSet)) {
      int i = 0;
      string module;
      ostringstream oss;
      oss << "module" << i;

      while (modulesDataSet.get(oss.str(), module)) {
        bool moduleLoaded = loadModule(QString::fromUtf8(module.c_str()), false);

        if (!moduleLoaded) {
          string moduleSrc;
          oss.str("");
          oss << "module_src" << i;

          if (modulesDataSet.get(oss.str(), moduleSrc)) {
            replaceAll(moduleSrc, "    ", "\t");
            QFileInfo fileInfo(QString::fromUtf8(module.c_str()));
            loadModuleFromSrcCode(fileInfo.fileName(), moduleSrc.c_str());
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

    _viewWidget->setMainTabWidgetIndex(0);

    if (dataSet.get("main_scripts", mainScriptsDataSet)) {
      int i = 0;
      string mainScript;
      ostringstream oss;
      oss << "main_script" << i;

      while (mainScriptsDataSet.get(oss.str(), mainScript)) {
        mainScriptLoaded = loadScript(QString::fromUtf8(mainScript.c_str()), false);

        if (!mainScriptLoaded) {
          string mainScriptSrc;
          oss.str("");
          oss << "main_script_src" << i;
          QFileInfo fileInfo(QString::fromUtf8(mainScript.c_str()));

          if (mainScriptsDataSet.get(oss.str(), mainScriptSrc)) {
            int mainScriptId = _viewWidget->addMainScriptEditor();
            PythonCodeEditor *codeEditor = _viewWidget->getMainScriptEditor(mainScriptId);
            // TLPParser seems to replace the tab character with four white spaces when reading the content of the TLP file, don't know why
            // Anyway, replace the original tab character in order to have a correct indentation when setting the script text to the code editor
            replaceAll(mainScriptSrc, "    ", "\t");
            codeEditor->setPlainText(QString::fromUtf8(mainScriptSrc.c_str()));

            if (mainScript != "")
              _viewWidget->setScriptEditorTabText(mainScriptId, fileInfo.fileName());
            else
              _viewWidget->setScriptEditorTabText(mainScriptId, "[no file]");

            _viewWidget->setScriptEditorTabToolTip(mainScriptId, "string main script, don't forget to save the current graph or\n save the script to a file to not lose your source code modifications.");
          }
        }

        oss.str("");
        oss << "main_script" << ++i;
      }

      int mainScriptId = 0;
      mainScriptsDataSet.get("main_script_id", mainScriptId);

      _viewWidget->setCurrentScriptEditor(mainScriptId);

    }
    else {

      if (dataSet.get("main script file", filename)) {
        mainScriptLoaded = loadScript(QString::fromUtf8(filename.c_str()), false);
      }

      if (!mainScriptLoaded) {
        int editorId = _viewWidget->addMainScriptEditor();
        PythonCodeEditor *codeEditor = _viewWidget->getMainScriptEditor(editorId);
        codeEditor->setFileName("");
        QFileInfo fileInfo(QString::fromUtf8(filename.c_str()));

        if (dataSet.get("script code", scriptCode)) {
          replaceAll(scriptCode, "    ", "\t");
          codeEditor->setPlainText(QString::fromStdString(scriptCode));

          if (filename != "")
            _viewWidget->setScriptEditorTabText(0, fileInfo.fileName());
          else
            _viewWidget->setScriptEditorTabText(0, "[no file]");

          _viewWidget->setScriptEditorTabToolTip(0, "string main script, don't forget to save the current graph or\n save the script to a file to not lose your source code modifications.");
        }
        else {
          codeEditor->setPlainText(getDefaultScriptCode(_pythonInterpreter->getPythonVersionStr(), _graph));
          _viewWidget->setScriptEditorTabText(0, "[no file]");
          _viewWidget->setScriptEditorTabToolTip(0, "string main script, don't forget to save the current graph or\n save the script to a file to not lose your source code modifications.");
        }
      }

    }
  }
}

DataSet PythonScriptView::state() const {
  DataSet ret;
  DataSet *dataSet = &ret;

  PythonCodeEditor *codeEditor = _viewWidget->getCurrentMainScriptEditor();

  if (codeEditor) {
    dataSet->set("main script file",
                 codeEditor->getFileName().toUtf8().data());
    string scriptCode = _viewWidget->getCurrentMainScriptEditor()->getCleanCode().toStdString();
    dataSet->set("script code", scriptCode);
  }

  DataSet mainScriptsDataSet;

  for (int i = 0 ; i < _viewWidget->numberOfScriptEditors() ; ++i) {
    string scriptFile =
      _viewWidget->getMainScriptEditor(i)->getFileName().toUtf8().data();

    if (scriptFile != "")
      const_cast<PythonScriptView*>(this)->saveScript(i);

    ostringstream oss;
    oss << "main_script" << i;
    mainScriptsDataSet.set(oss.str(), scriptFile);
    oss.str("");
    oss << "main_script_src" << i;
    mainScriptsDataSet.set(oss.str(), _viewWidget->getMainScriptEditor(i)->getCleanCode().toStdString());
  }

  mainScriptsDataSet.set("main_script_id", _viewWidget->getCurrentMainScriptEditorIndex());
  dataSet->set("main_scripts", mainScriptsDataSet);


  DataSet modulesDataSet;

  for (int i = 0 ; i < _viewWidget->numberOfModulesEditors() ; ++i) {
    string moduleFile =
      _viewWidget->getModuleEditor(i)->getFileName().toUtf8().data();

    if (moduleFile != "")
      const_cast<PythonScriptView*>(this)->saveModule(i);

    ostringstream oss;
    oss << "module" << i;
    modulesDataSet.set(oss.str(), moduleFile);
    oss.str("");
    oss << "module_src" << i;
    modulesDataSet.set(oss.str(), _viewWidget->getModuleEditor(i)->getCleanCode().toStdString());
  }

  dataSet->set("modules", modulesDataSet);

  return ret;
}



void PythonScriptView::executeCurrentScript() {

  if (_pythonInterpreter->isScriptPaused()) {
    Observable::holdObservers();
    _pythonInterpreter->pauseCurrentScript(false);
    _viewWidget->runScriptButton()->setEnabled(false);
    _viewWidget->pauseScriptButton()->setEnabled(true);
    _viewWidget->scriptStatusLabel()->setText("Executing script ...");
    _viewWidget->progressBar()->setRange(0,0);
    return;
  }

  if (!_pythonInterpreter->isRunningScript() && _viewWidget->numberOfScriptEditors() > 0) {

    _runningScript = true;

    _pythonInterpreter->setConsoleWidget(_viewWidget->consoleWidget());
    _viewWidget->consoleWidget()->clear();
    _pythonInterpreter->clearOutputBuffers();
    clearErrorIndicators();

    QString scriptFileName = _viewWidget->getCurrentMainScriptEditor()->getFileName();

    if (scriptFileName == "") {
      scriptFileName = "<unnamed script>";
    }


    saveImportAllScripts();

    saveAllModules();

    _pythonInterpreter->setConsoleWidget(_viewWidget->consoleWidget());

    if (!reloadAllModules() || !_pythonInterpreter->runString(_viewWidget->getCurrentMainScriptEditor()->getCleanCode(), scriptFileName)) {
      indicateErrors();
      return;
    }

    _graph->push();
    Observable::holdObservers();

    _pythonInterpreter->setProcessQtEventsDuringScriptExecution(true);

    _viewWidget->scriptStatusLabel()->setText("Executing script ...");
    _viewWidget->progressBar()->setRange(0,0);

    _viewWidget->runScriptButton()->setEnabled(false);
    _viewWidget->stopScriptButton()->setEnabled(true);
    _viewWidget->pauseScriptButton()->setEnabled(true);

    QApplication::processEvents();

    bool scriptExecOk = _pythonInterpreter->runGraphScript("__main__", "main", _graph, scriptFileName);

    _pythonInterpreter->setProcessQtEventsDuringScriptExecution(false);
    _viewWidget->stopScriptButton()->setEnabled(false);
    _viewWidget->runScriptButton()->setEnabled(true);
    _viewWidget->pauseScriptButton()->setEnabled(false);

    if (scriptExecOk) {
      _viewWidget->scriptStatusLabel()->setText("Script execution has succeed");
      _pythonInterpreter->runString("del main");
    }
    else {

      _viewWidget->scriptStatusLabel()->setText("Script execution has failed");

      if (!_scriptStopped) {
        indicateErrors();
      }

      _graph->pop(false);
    }

    _viewWidget->progressBar()->setRange(0,100);
    _viewWidget->progressBar()->reset();

    _pythonInterpreter->resetConsoleWidget();

    if (Observable::observersHoldCounter() > 0)
      Observable::unholdObservers();

    // some external modules (like numpy) overrides the SIGINT handler at import
    // reinstall the default one, otherwise Tulip can not be interrupted by hitting Ctrl-C in a console
    _pythonInterpreter->setDefaultSIGINTHandler();

    _scriptStopped = false;
    _runningScript = false;

  }
  else {
    QMessageBox::information(_viewWidget->getCurrentMainScriptEditor(), "Script execution not allowed", "The Python interpreter already execute a script. You must wait for its termination or stop its execution before running a new script.");
  }
}

void PythonScriptView::indicateErrors() const {
  QRegExp rx("^.*File.*\"(.*)\".*line.*(\\d+).*$");
  QRegExp rx2("^.*File.*\"(.*)\".*line.*(\\d+).*in (.*)$");

  QMap<QString, QVector<int> > errorLines;
  QString consoleOutput = _pythonInterpreter->getStandardErrorOutput();
  QStringList outputLines = consoleOutput.split("\n");

  for (int i = 0 ; i < outputLines.count() - 1 ; ++i) {
    int pos = 0;

    while ((pos = rx.indexIn(outputLines[i], pos)) != -1) {
      rx2.indexIn(outputLines[i], pos);

      if (rx2.cap(3) != "tlpimporthook") {
        QString file = rx.cap(1);
#ifdef WIN32
        file.replace("\\", "/");
#endif
        int line = rx.cap(2).toInt();
        errorLines[file].push_back(line);
      }

      pos += rx.matchedLength();
    }

  }

  if (errorLines.find("<unnamed script>") != errorLines.end()) {
    for (int i = 0 ; i < errorLines["<unnamed script>"].size() ; ++i) {
      if (errorLines["<unnamed script>"][i] > 1) {
        _viewWidget->getCurrentMainScriptEditor()->indicateScriptCurrentError(errorLines["<unnamed script>"][i]-1);
      }
    }
  }

  _viewWidget->indicateErrors(errorLines);
}

void PythonScriptView::clearErrorIndicators() const {
  _viewWidget->clearErrorIndicators();
}

void PythonScriptView::stopCurrentScript() {
  _scriptStopped = true;
  _pythonInterpreter->stopCurrentScript();
}

void PythonScriptView::pauseCurrentScript() {
  Observable::unholdObservers();
  _pythonInterpreter->pauseCurrentScript();
  _viewWidget->pauseScriptButton()->setEnabled(false);
  _viewWidget->runScriptButton()->setEnabled(true);
  _viewWidget->progressBar()->setRange(0,100);
  _viewWidget->progressBar()->reset();
  _viewWidget->scriptStatusLabel()->setText("Script execution is paused.");
}

void PythonScriptView::newScript() {
  int editorId = _viewWidget->addMainScriptEditor();
  _viewWidget->getMainScriptEditor(editorId)->setPlainText(getDefaultScriptCode(_pythonInterpreter->getPythonVersionStr(), _graph));
  _viewWidget->setScriptEditorTabText(editorId, "[no file]");
  _viewWidget->setScriptEditorTabToolTip(editorId, "string main script, don't forget to save the current graph or\n save the script to a file to not lose your source code modifications.");
}

void PythonScriptView::loadScript() {
  QString fileName = QFileDialog::getOpenFileName(NULL, "Open Main Script","", "Python script (*.py)");
  loadScript(fileName);
}

bool PythonScriptView::loadScript(const QString &fileName, bool clear) {
  QFile file(findFile(fileName));

  if (!file.exists()) {
    return false;
  }

  QFileInfo fileInfo(file);

  _viewWidget->addMainScriptEditor(fileInfo.absoluteFilePath());

  QString modulePath = fileInfo.absolutePath();
  _pythonInterpreter->addModuleSearchPath(modulePath);
  _pythonInterpreter->setConsoleWidget(_viewWidget->consoleWidget());

  if (clear) {
    _viewWidget->consoleWidget()->clear();
    _pythonInterpreter->clearOutputBuffers();
  }

  clearErrorIndicators();
  _pythonInterpreter->reloadModule(fileInfo.fileName().replace(".py", ""));
  indicateErrors();
  _pythonInterpreter->resetConsoleWidget();

  return true;
}

void PythonScriptView::saveScript() {
  saveScript(_viewWidget->getCurrentMainScriptEditorIndex());
}

void PythonScriptView::saveScript(int tabIdx, bool clear) {
  if (tabIdx >=0 && tabIdx < _viewWidget->numberOfScriptEditors()) {
    QString fileName;
    QString mainScriptFileName = _viewWidget->getMainScriptEditor(tabIdx)->getFileName();


    if (mainScriptFileName == "") {
      QString dir = "";
      QString tabText = _viewWidget->getScriptEditorTabText(tabIdx);

      if (!tabText.startsWith("[")) {
        dir = tabText;
      }

      fileName = QFileDialog::getSaveFileName(NULL, tr("Save Main Script"),dir,"Python script (*.py)");
    }
    else
      fileName = mainScriptFileName;

    if (fileName != "") {
      if (!fileName.endsWith(".py"))
        fileName += ".py";

      QFile file(fileName);
      QFileInfo fileInfo(file);

      _viewWidget->getMainScriptEditor(tabIdx)->setFileName(fileInfo.absoluteFilePath());
      _viewWidget->getMainScriptEditor(tabIdx)->saveCodeToFile();
      _viewWidget->setScriptEditorTabText(tabIdx, fileInfo.fileName());
      _viewWidget->setScriptEditorTabToolTip(tabIdx, fileInfo.absoluteFilePath());

      QString modulePath = fileInfo.absolutePath();
      _pythonInterpreter->addModuleSearchPath(modulePath);
      _pythonInterpreter->setConsoleWidget(_viewWidget->consoleWidget());

      if (clear) {
        _viewWidget->consoleWidget()->clear();
        _pythonInterpreter->clearOutputBuffers();
      }

      clearErrorIndicators();
      _pythonInterpreter->reloadModule(fileInfo.fileName().replace(".py", ""));
      indicateErrors();
      _pythonInterpreter->resetConsoleWidget();

    }
  }
}

void PythonScriptView::saveImportAllScripts() {
  for (int i = 0 ; i < _viewWidget->numberOfScriptEditors() ; ++i) {
    PythonCodeEditor *codeEditor = _viewWidget->getMainScriptEditor(i);

    if (codeEditor->getFileName() != "") {
      saveScript(i, false);
    }
    else {
      QString tabText = _viewWidget->getScriptEditorTabText(i);

      if (tabText.contains(".py")) {
        tabText.replace(".py", "");
        _pythonInterpreter->registerNewModuleFromString(tabText, _viewWidget->getMainScriptEditor(i)->getCleanCode());
        _pythonInterpreter->importModule(tabText);
      }
    }
  }
}

QString PythonScriptView::findFile(const QString &filePath) {
  // We first check if filePath is empty to avoid a warning
  // output in the call of FileInfo::absolutePath()
  if (filePath.isEmpty())
    return "";

  QFileInfo fileInfo(filePath);
  QString filepath = fileInfo.absolutePath();
  QString filename = fileInfo.fileName();

  if (fileInfo.exists()) {
    return filePath;
  }
  else if (filename != "" && _graph) {
    std::string tlpFile;

    if (_graph->getRoot()->getAttribute("file", tlpFile)) {
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


void PythonScriptView::loadModule() {
  QString fileName = QFileDialog::getOpenFileName(NULL, "Open Module","", "Python script (*.py)");
  loadModule(fileName);
}


bool PythonScriptView::loadModule(const QString &fileName, bool clear) {
  QFile file(findFile(fileName));

  if (!file.exists())
    return false;

  QFileInfo fileInfo(file);
  QString moduleName(fileInfo.fileName());
  QString modulePath(fileInfo.absolutePath());

  _viewWidget->addModuleEditor(fileInfo.absoluteFilePath());

  _pythonInterpreter->addModuleSearchPath(modulePath);

  checkErrors(clear);

  return true;
}

bool PythonScriptView::loadModuleFromSrcCode(const QString &moduleName, const QString &moduleSrcCode) {
  int editorId = _viewWidget->addModuleEditor();
  PythonCodeEditor *codeEditor = _viewWidget->getModuleEditor(editorId);
  codeEditor->setFileName(moduleName);
  codeEditor->setPlainText(moduleSrcCode);

  _viewWidget->setModuleEditorTabText(editorId, moduleName);
  _viewWidget->setModuleEditorTabToolTip(editorId, "string module, don't forget to save the current graph or\n save the module to a file to not lose your source code modifications.");

  bool ret = _pythonInterpreter->registerNewModuleFromString(QString(moduleName).replace(".py", ""),  moduleSrcCode);

  if (ret) {
    codeEditor->analyseScriptCode(true);
  }

  return ret;
}

void PythonScriptView::newFileModule() {
  QString fileName = QFileDialog::getSaveFileName(NULL, tr("Set Module filename"),"","Python script (*.py)");

  if (fileName.isEmpty())
    return;

  if (!fileName.endsWith(".py"))
    fileName += ".py";

  QFile file(fileName);

  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  file.close();

  QFileInfo fileInfo(file);
  QString moduleName(fileInfo.fileName());
  QString modulePath(fileInfo.absolutePath());

  int editorId = _viewWidget->addModuleEditor(fileInfo.absoluteFilePath());
  _viewWidget->getModuleEditor(editorId)->saveCodeToFile();

  _pythonInterpreter->addModuleSearchPath(modulePath);

}

void PythonScriptView::newStringModule() {
  bool ok;
  QString moduleName = QInputDialog::getText(NULL, "New string module  ", "module name :", QLineEdit::Normal,"", &ok);

  if (ok && !moduleName.isEmpty()) {

    if (!moduleName.endsWith(".py"))
      moduleName += ".py";

    int editorId = _viewWidget->addModuleEditor(moduleName);
    _viewWidget->getModuleEditor(editorId)->setFileName(moduleName);
    _viewWidget->setModuleEditorTabToolTip(editorId, "string module, don't forget to save the current graph or\n save module to file to not lose your source code modifications.");
  }
}

void PythonScriptView::saveModule() {
  int curModule = _viewWidget->getCurrentModuleEditorIndex();

  if (curModule == -1)
    return;

  saveModule(curModule, true);
}

void PythonScriptView::checkErrors(bool clear) {
  _pythonInterpreter->setConsoleWidget(_viewWidget->consoleWidget());

  if (clear) {
    _viewWidget->consoleWidget()->clear();
    _pythonInterpreter->clearOutputBuffers();
  }

  clearErrorIndicators();
  reloadAllModules();
  saveImportAllScripts();
  indicateErrors();
  _pythonInterpreter->resetConsoleWidget();
}

void PythonScriptView::saveModule(int tabIdx, const bool reload) {
  if (tabIdx >= 0 && tabIdx < _viewWidget->numberOfModulesEditors()) {
    QString moduleNameExt = _viewWidget->getModuleEditorTabText(tabIdx);
    QString moduleName;

    if (moduleNameExt[moduleNameExt.size() - 1] == '*')
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 4);
    else
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 3);

    _pythonInterpreter->deleteModule(moduleName);
    _viewWidget->setModuleEditorTabText(tabIdx, moduleName+".py");
    QFile file(_viewWidget->getModuleEditor(tabIdx)->getFileName());
    QFileInfo fileInfo(file);

    if (_viewWidget->getModuleEditor(tabIdx)->saveCodeToFile()) {
      _viewWidget->setModuleEditorTabToolTip(tabIdx, fileInfo.absoluteFilePath());
    }
  }

  if (reload && !_pythonInterpreter->isRunningScript()) {
    checkErrors();
  }
}

void PythonScriptView::saveAllModules() {
  for (int i = 0 ; i < _viewWidget->numberOfModulesEditors() ; ++i) {
    saveModule(i);
  }
}

bool PythonScriptView::reloadAllModules() const {

  bool ret = true;

  for (int i = 0 ; i < _viewWidget->numberOfModulesEditors() ; ++i) {
    QString moduleNameExt = _viewWidget->getModuleEditorTabText(i);
    QString moduleName;

    if (moduleNameExt[moduleNameExt.size() - 1] == '*')
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 4);
    else
      moduleName = moduleNameExt.mid(0, moduleNameExt.size() - 3);

    _pythonInterpreter->deleteModule(moduleName);
    QFileInfo fileInfo(_viewWidget->getModuleEditor(i)->getFileName());

    if (fileInfo.fileName() == _viewWidget->getModuleEditor(i)->getFileName()) {
      ret = ret && _pythonInterpreter->registerNewModuleFromString(moduleName, _viewWidget->getModuleEditor(i)->getCleanCode());
    }
    else {
      _pythonInterpreter->addModuleSearchPath(fileInfo.absolutePath());
      ret = ret && _pythonInterpreter->reloadModule(moduleName);
    }
  }

  return ret;

}

bool PythonScriptView::eventFilter(QObject *obj, QEvent *event) {
#ifdef __APPLE__
  Qt::KeyboardModifiers modifier = Qt::MetaModifier;
#else
  Qt::KeyboardModifiers modifier = Qt::ControlModifier;
#endif

  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvt = static_cast<QKeyEvent *>(event);

    if (obj == _viewWidget->getCurrentMainScriptEditor() && keyEvt->modifiers() == modifier && keyEvt->key() == Qt::Key_Return) {
      executeCurrentScript();
      return true;
    }
  }

  return false;
}
