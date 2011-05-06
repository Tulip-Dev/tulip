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

const unsigned int nbReservedCharacters = 22;
const char pythonReservedCharacters[nbReservedCharacters] = {'#', '%', '/', '+', '-', '&', '*', '<', '>',
		'|', '~', '^', '=', '!', '\'', '\"', '{', '}',
		'(', ')', '[', ']'};

const std::string cleanPropertyName(const std::string &propertyName) {
	std::string ret(propertyName);
	std::replace(ret.begin(), ret.end(), ' ', '_');
	for (unsigned int i = 0 ; i < nbReservedCharacters ; ++i) {
		ret.erase(std::remove(ret.begin(), ret.end(), pythonReservedCharacters[i]), ret.end());
	}
	return ret;
}

const std::string getDefaultScriptCode(const string &pythonVersion, Graph *graph) {
	ostringstream oss;

	oss << "# Powered by Python " << pythonVersion << endl << endl;
	oss << "# To cancel the modifications performed by the script" << endl;
	oss << "# on the current graph, click on the undo button." << endl << endl;

	oss << "# Some usefull keyboards shortcuts : " << endl;
	oss << "#   * Ctrl + D : comment selected lines." << endl;
	oss << "#   * Ctrl + Shift + D  : uncomment selected lines." << endl;
	oss << "#   * Ctrl + Space  : run script." << endl << endl;

	oss << "from tulip import *" << endl << endl;;

	oss << "# the updateVisualization(centerViews = True) function can be called" << endl;
	oss << "# during script execution to update the opened views" << endl << endl;
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

VIEWPLUGIN(PythonScriptView, "Python Script view", "Antoine Lambert", "04/2010", "Python Script View", "0.7");

PythonScriptView::PythonScriptView() : pythonInterpreter(NULL),graph(NULL), scriptStopped(false) {}

PythonScriptView::~PythonScriptView() {}

QWidget *PythonScriptView::construct(QWidget *parent) {
	QWidget *widget = AbstractView::construct(parent);
	viewWidget = new PythonScriptViewWidget(this, widget);
	widget->resize(550,700);
	browserDialog = new QDialog(viewWidget);
	browserDialog->setWindowTitle("Tulip Python API");
	browser = new QWebView(browserDialog);
	browser->load(QUrl("qrc:/apidoc/index.html"));
	QVBoxLayout *vLayout = new QVBoxLayout;
	vLayout->addWidget(browser);
	browserDialog->setLayout(vLayout);
	connect(viewWidget->runScriptButton, SIGNAL(clicked()), this, SLOT(executeCurrentScript()));
	connect(viewWidget->stopScriptButton, SIGNAL(clicked()), this, SLOT(stopCurrentScript()));
	connect(viewWidget->newMainScriptAction, SIGNAL(triggered()), this, SLOT(newScript()));
	connect(viewWidget->loadMainScriptAction, SIGNAL(triggered()), this, SLOT(loadScript()));
	connect(viewWidget->saveMainScriptAction, SIGNAL(triggered()), this, SLOT(saveScript()));
	connect(viewWidget->showApiDocButton, SIGNAL(clicked()), this, SLOT(showApiDocumentation()));
	connect(viewWidget->newStringModuleAction, SIGNAL(triggered()), this, SLOT(newStringModule()));
	connect(viewWidget->newFileModuleAction, SIGNAL(triggered()), this, SLOT(newFileModule()));
	connect(viewWidget->loadModuleAction, SIGNAL(triggered()), this, SLOT(loadModule()));
	connect(viewWidget->saveModuleAction, SIGNAL(triggered()), this, SLOT(saveModuleToFile()));
	connect(viewWidget->mainScriptsTabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeMainScriptTabRequested(int)));
	connect(viewWidget->modulesTabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeModuleTabRequested(int)));

	setCentralWidget(viewWidget);
	pythonInterpreter = PythonInterpreter::getInstance();
	pythonInterpreter->runString(updateVisualizationFunc);

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
			} else {
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
	this->graph = graph;

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

		} else {

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
				} else {
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
		if (timer.elapsed() >= 50) {
			if (QApplication::hasPendingEvents())
				QApplication::processEvents();
			timer.start();
		}
	}
	return 0;
}

void PythonScriptView::executeCurrentScript() {

	if (!pythonInterpreter->isRunningScript() && viewWidget->mainScriptsTabWidget->count() > 0) {

		pythonInterpreter->setConsoleWidget(viewWidget->consoleOutputWidget);
		viewWidget->consoleOutputWidget->clear();

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

		timer.start();

		bool scriptExecOk = pythonInterpreter->runGraphScript("__main__", "main", graph);


		pythonInterpreter->setTraceFunction(NULL);
		viewWidget->stopScriptButton->setEnabled(false);
		viewWidget->runScriptButton->setEnabled(true);

		if (scriptExecOk) {
			viewWidget->scriptStatusLabel->setText("Script execution has succeed");
			pythonInterpreter->runString("del main");

			if (clusterTreeWidget)
				clusterTreeWidget->update();

		} else {

			viewWidget->scriptStatusLabel->setText("Script execution has failed");

			if (!scriptStopped) {
				QRegExp rx("^.*File.*\"(.*)\".*line.*(\\d+).*$");

				map<string, vector<int> > errorLines;
				QString consoleOutput = viewWidget->consoleOutputWidget->toPlainText();
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

		Observable::unholdObservers();

// some external modules (like numpy) overrides the SIGINT handler at import
// reinstall the default one, otherwise Tulip can not be interrupted by hitting Ctrl-C in a console
		pythonInterpreter->setDefaultSIGINTHandler();

		scriptStopped = false;

	} else {
		QMessageBox::information(viewWidget->getCurrentMainScriptEditor(), "Script execution not allowed", "The Python interpreter already execute a script. You must wait for its termination or stop its execution before running a new script.");
	}
}


void PythonScriptView::stopCurrentScript() {
	scriptStopped = true;
	pythonInterpreter->stopCurrentScript();
}

void PythonScriptView::newScript() {
	int editorId = viewWidget->addMainScriptEditor();
	editedMainScripts[editorId] = "";
	viewWidget->getMainScriptEditor(editorId)->setText(getDefaultScriptCode(pythonInterpreter->getPythonVersion(), graph).c_str());
	viewWidget->mainScriptsTabWidget->setTabText(editorId, "[no file]");
	viewWidget->mainScriptsTabWidget->setTabToolTip(editorId, "string main script, don't forget to save the current graph or\n save script to file to not lose modifications to source code.");
}

void PythonScriptView::loadScript() {
	QString fileName = QFileDialog::getOpenFileName(viewWidget, "Open Main Script","./", "Python script (*.py)");
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
		fileName = QFileDialog::getSaveFileName(viewWidget, tr("Save Main Script"),"./","Python script (*.py)");
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

void PythonScriptView::showApiDocumentation() {
	browserDialog->show();
}


void PythonScriptView::loadModule() {
	QString fileName = QFileDialog::getOpenFileName(viewWidget, "Open Module","./", "Python script (*.py)");
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
	QString fileName = QFileDialog::getSaveFileName(viewWidget, tr("Set Module filename"),"./","Python script (*.py)");
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
	QString fileName = QFileDialog::getSaveFileName(viewWidget, tr("Set Module filename"),"./","Python script (*.py)");
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
		} else {
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
			} else {
				saveModule();
			}
			return true;
		} else if (keyEvt->modifiers() == Qt::ControlModifier && keyEvt->key() == Qt::Key_Space) {
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
