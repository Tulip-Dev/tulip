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

// +-------------------------------------------------------------------------+
// | Tulip Python View                                                       |
// | Author:  Antoine Lambert                                                |
// | Contact:  antoine.lambert@labri.fr                                      |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | Tulip Python View is free software; you can redistribute it             |
// | and/or modify  it under the terms of the GNU General Public License     |
// | as published by the Free Software Foundation; either version 2 of the   |
// | License, or (at your option) any later version.                         |
// |                                                                         |
// | Tulip Python View is distributed in the hope that it will be            |
// | useful, but WITHOUT ANY WARRANTY; without even the implied warranty of  |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with this program.  If not, see <http://www.gnu.org/licenses/>.   |
// |                                                                         |
// +-------------------------------------------------------------------------+

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

PythonScriptView::PythonScriptView() : pythonInterpreter(NULL), fontZoom(0), scriptStopped(false) {}

PythonScriptView::~PythonScriptView() {}

QWidget *PythonScriptView::construct(QWidget *parent) {
	QWidget *widget = AbstractView::construct(parent);
	viewWidget = new PythonScriptViewWidget(widget);
	viewWidget->pythonCodeWidget->installEventFilter(this);
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
	connect(viewWidget->pythonCodeWidget, SIGNAL(textChanged()), this, SLOT(mainScriptTextChanged()));
	connect(viewWidget->modulesTabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeModuleTabRequested(int)));
	connect(viewWidget->decreaseFontSizeButton, SIGNAL(clicked()), this, SLOT(decreaseFontSize()));
	connect(viewWidget->increaseFontSizeButton, SIGNAL(clicked()), this, SLOT(increaseFontSize()));
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
	string scriptCode;
	string filename;
	bool mainScriptLoaded = false;
	if (dataSet.get("main script file", filename)) {
		mainScriptLoaded = loadScript(filename.c_str());
	}
	if (!mainScriptLoaded && dataSet.get("script code", scriptCode)) {
		// TLPParser seems to replace the tab character with four white spaces when reading the content of the TLP file, don't know why
		// Anyway, replace the original tab character in order to have a correct indentation when setting the script text to the code editor
		replaceAll(scriptCode, "    ", "\t");
		viewWidget->pythonCodeWidget->setText(QString::fromStdString(scriptCode));
		viewWidget->tabWidget->setTabText(0, "Main script [no file]");
		viewWidget->tabWidget->setTabToolTip(0, "string main script, don't forget to save the current graph or\n save script to file to not lose modifications to source code.");
	} else if (viewWidget->pythonCodeWidget->text() == "") {
		viewWidget->pythonCodeWidget->setText(getDefaultScriptCode(pythonInterpreter->getPythonVersion(), graph).c_str());
		viewWidget->tabWidget->setTabText(0, "Main script [no file]");
		viewWidget->tabWidget->setTabToolTip(0, "string main script, don't forget to save the current graph or\n save script to file to not lose modifications to source code.");
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

void PythonScriptView::getData(Graph **graph,DataSet *dataSet) {
	*graph = this->graph;
	dataSet->set("main script file", mainScriptFileName);
	string scriptCode = viewWidget->getPythonCode();
	dataSet->set("script code", scriptCode);
	map<int, string>::const_iterator it = editedModules.begin();
	DataSet modulesDataSet;
	int i = 0;
	for ( ; it != editedModules.end() ; ++it) {
		ostringstream oss;
		oss << "module" << i;
		modulesDataSet.set(oss.str(), it->second);
		oss.str("");
		oss << "module_src" << i++;
		modulesDataSet.set(oss.str(), static_cast<QsciScintilla *>(viewWidget->modulesTabWidget->widget(it->first))->text().toStdString());
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

	if (!pythonInterpreter->isRunningScript()) {

		pythonInterpreter->setConsoleWidget(viewWidget->consoleOutputWidget);
		viewWidget->consoleOutputWidget->clear();

		if (mainScriptFileName != "") {
			saveScript();
		}

		saveAllModules();
		reloadAllModules();

		viewWidget->pythonCodeWidget->clearErrorIndicator();
		map<int, string>::const_iterator it;
		for (it = editedModules.begin() ; it != editedModules.end() ; ++it) {
			PythonCodeEditor *codeEditor = static_cast<PythonCodeEditor *>(viewWidget->modulesTabWidget->widget(it->first));
			codeEditor->clearErrorIndicator();
		}

		graph->push();
		Observable::holdObservers();

		pythonInterpreter->setTraceFunction(tracefunc);
		pythonInterpreter->runString(viewWidget->getPythonCode().c_str());

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
						viewWidget->pythonCodeWidget->indicateScriptCurrentError(errorLines["<string>"][i]-1);
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

		Observable::unholdObservers(true);

		scriptStopped = false;

	} else {
		QMessageBox::information(viewWidget->pythonCodeWidget, "Script execution not allowed", "The Python interpreter already execute a script. You must wait for its termination or stop its execution before running a new script.");
	}
}


void PythonScriptView::stopCurrentScript() {
	scriptStopped = true;
	pythonInterpreter->stopCurrentScript();
}

void PythonScriptView::newScript() {
	if (QMessageBox::question(viewWidget, "Save current script ?", "Do you want to save the current script ?", QMessageBox::Cancel | QMessageBox::Ok ) == QMessageBox::Ok) {
		saveScript();
	}
	viewWidget->pythonCodeWidget->setText(getDefaultScriptCode(pythonInterpreter->getPythonVersion(), graph).c_str());
	mainScriptFileName = "";
	viewWidget->tabWidget->setTabText(0, "Main script [no file]");
	viewWidget->tabWidget->setTabToolTip(0, "string main script, don't forget to save the current graph or\n save script to file to not lose modifications to source code.");
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

	mainScriptFileName = fileInfo.absoluteFilePath().toStdString();
	QString scriptCode;
	while (!file.atEnd()) {
		scriptCode += file.readLine();
	}

	viewWidget->pythonCodeWidget->setText(scriptCode);
	viewWidget->tabWidget->setTabText(0, "Main script ["+fileInfo.fileName()+"]");
	viewWidget->tabWidget->setTabToolTip(0, fileInfo.absoluteFilePath());

	file.close();

	return true;
}

void PythonScriptView::saveScript() {
	QString fileName;
	if (mainScriptFileName == "")
		fileName = QFileDialog::getSaveFileName(viewWidget, tr("Save Main Script"),"./","Python script (*.py)");
	else
		fileName = mainScriptFileName.c_str();

	if (fileName != "") {
		if (!fileName.endsWith(".py"))
			fileName += ".py";
		QFile file(fileName);
		QFileInfo fileInfo(file);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			return;
		mainScriptFileName = fileInfo.absoluteFilePath().toStdString();
		QTextStream out(&file);
		out << viewWidget->getPythonCode().c_str();
		viewWidget->tabWidget->setTabText(0, "Main script ["+fileInfo.fileName()+"]");
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

	QsciScintilla *codeEditor = new PythonCodeEditor(viewWidget, fontZoom);
	codeEditor->installEventFilter(this);

	pythonInterpreter->addModuleSearchPath(modulePath.toStdString());
	int tab = viewWidget->modulesTabWidget->addTab(codeEditor, moduleName);

	viewWidget->modulesTabWidget->setTabText(tab, fileInfo.fileName());

	editedModules[tab] = fileInfo.absoluteFilePath().toStdString();

	codeEditor->setText(scriptCode);

	viewWidget->modulesTabWidget->setTabToolTip(tab, fileInfo.absoluteFilePath());
	viewWidget->modulesTabWidget->setCurrentIndex(tab);

	pythonInterpreter->reloadModule(moduleName.replace(".py", "").toStdString());

	connect(codeEditor, SIGNAL(textChanged()), this, SLOT(moduleScriptTextChanged()));

	return true;
}

bool PythonScriptView::loadModuleFromSrcCode(const std::string &moduleName, const std::string &moduleSrcCode) {
	QsciScintilla *codeEditor = new PythonCodeEditor(viewWidget, fontZoom);
	codeEditor->installEventFilter(this);

	int tab = viewWidget->modulesTabWidget->addTab(codeEditor, moduleName.c_str());

	editedModules[tab] = moduleName;

	viewWidget->modulesTabWidget->setTabText(tab, moduleName.c_str());

	codeEditor->setText(moduleSrcCode.c_str());

	viewWidget->modulesTabWidget->setTabToolTip(tab, "string module, don't forget to save the current graph or\n save module to file to not lose modifications to source code.");
	viewWidget->modulesTabWidget->setCurrentIndex(tab);

	pythonInterpreter->registerNewModuleFromString(QString(moduleName.c_str()).replace(".py", "").toStdString(),  moduleSrcCode);

	connect(codeEditor, SIGNAL(textChanged()), this, SLOT(moduleScriptTextChanged()));

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

	QsciScintilla *codeEditor = new PythonCodeEditor(viewWidget, fontZoom);
	codeEditor->installEventFilter(this);
	connect(codeEditor, SIGNAL(textChanged()), this, SLOT(moduleScriptTextChanged()));

	pythonInterpreter->addModuleSearchPath(modulePath.toStdString());
	int tab = viewWidget->modulesTabWidget->addTab(codeEditor, moduleName);
	viewWidget->modulesTabWidget->setTabText(tab, fileInfo.fileName());
	viewWidget->modulesTabWidget->setTabToolTip(tab, "string module, don't forget to save the current graph or\n save module to file to not lose modifications to source code.");

	editedModules[tab] = fileInfo.absoluteFilePath().toStdString();

	viewWidget->modulesTabWidget->setTabToolTip(tab, fileInfo.absoluteFilePath());

	file.close();

	viewWidget->modulesTabWidget->setCurrentIndex(tab);
}

void PythonScriptView::newStringModule() {
	bool ok;
	QString moduleName = QInputDialog::getText(this->viewWidget, "New string module  ", "module name :", QLineEdit::Normal,"", &ok);
	if (ok && !moduleName.isEmpty()) {

		if (!moduleName.endsWith(".py"))
			moduleName += ".py";

		QsciScintilla *codeEditor = new PythonCodeEditor(viewWidget, fontZoom);
		codeEditor->installEventFilter(this);
		connect(codeEditor, SIGNAL(textChanged()), this, SLOT(moduleScriptTextChanged()));

		int tab = viewWidget->modulesTabWidget->addTab(codeEditor, moduleName);
		viewWidget->modulesTabWidget->setTabText(tab, moduleName);

		editedModules[tab] = moduleName.toStdString();

		viewWidget->modulesTabWidget->setTabToolTip(tab, "string module, don't forget to save the current graph or\n save module to file to not lose modifications to source code.");
		viewWidget->modulesTabWidget->setCurrentIndex(tab);
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
		QsciScintilla *codeEditor = static_cast<QsciScintilla *>(viewWidget->modulesTabWidget->widget(tabIdx));
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
			out << codeEditor->text().replace("\r\n", "\n");
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
			if (obj == viewWidget->pythonCodeWidget) {
				saveScript();
			} else {
				saveModule();
			}
			return true;
		} else if (keyEvt->modifiers() == Qt::ControlModifier && keyEvt->key() == Qt::Key_Space) {
			executeCurrentScript();
			return true;
		} else if (keyEvt->modifiers() == Qt::ControlModifier && keyEvt->key() == Qt::Key_D) {
			commentSelectedCode(obj);
			return true;
		} else if (keyEvt->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier) && keyEvt->key() == Qt::Key_D) {
			uncommentSelectedCode(obj);
			return true;
		}
	}
	return false;
}

void PythonScriptView::mainScriptTextChanged() {
	if (mainScriptFileName != "") {
		QString curTabText = viewWidget->tabWidget->tabText(0);
		if (curTabText[curTabText.size() -1] != '*') {
			curTabText += "*";
			viewWidget->tabWidget->setTabText(0, curTabText);
		}
	}
}

void PythonScriptView::moduleScriptTextChanged() {
	QString curTabText = viewWidget->modulesTabWidget->tabText(viewWidget->modulesTabWidget->currentIndex());
	if (curTabText[curTabText.size() -1] != '*') {
		curTabText += "*";
		viewWidget->modulesTabWidget->setTabText(viewWidget->modulesTabWidget->currentIndex(), curTabText);
	}
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

void PythonScriptView::commentSelectedCode(QObject *obj) {
	QsciScintilla *codeEditor = dynamic_cast<QsciScintilla *>(obj);
	if (codeEditor && codeEditor->hasSelectedText()) {
		int lineFrom = 0;
		int indexFrom = 0;
		int lineTo = 0;
		int indexTo = 0;
		codeEditor->getSelection(&lineFrom, &indexFrom, &lineTo, &indexTo);
		for (int i = lineFrom ; i <= lineTo ; ++i) {
			codeEditor->insertAt("#", i, 0);
		}
		codeEditor->setSelection(lineFrom, 0, lineTo, codeEditor->text(lineTo).length() - 1);
	}
}
void PythonScriptView::uncommentSelectedCode(QObject *obj) {
	QsciScintilla *codeEditor = dynamic_cast<QsciScintilla *>(obj);
	if (codeEditor && codeEditor->hasSelectedText()) {
		int lineFrom = 0;
		int indexFrom = 0;
		int lineTo = 0;
		int indexTo = 0;
		codeEditor->getSelection(&lineFrom, &indexFrom, &lineTo, &indexTo);
		for (int i = lineFrom ; i <= lineTo ; ++i) {
			codeEditor->setSelection(i, 0, i, 1);
			if (codeEditor->selectedText() == "#") {
				codeEditor->removeSelectedText();
			}
		}
		codeEditor->setSelection(lineFrom, 0, lineTo, codeEditor->text(lineTo).length() - 1);
	}
}

void PythonScriptView::decreaseFontSize() {
	viewWidget->pythonCodeWidget->zoomOut();
	map<int, string>::const_iterator it;
	for (it = editedModules.begin() ; it != editedModules.end() ; ++it) {
		QsciScintilla *codeEditor = static_cast<QsciScintilla *>(viewWidget->modulesTabWidget->widget(it->first));
		codeEditor->zoomOut();
	}
	--fontZoom;
}

void PythonScriptView::increaseFontSize() {
	viewWidget->pythonCodeWidget->zoomIn();
	map<int, string>::const_iterator it;
	for (it = editedModules.begin() ; it != editedModules.end() ; ++it) {
		QsciScintilla *codeEditor = static_cast<QsciScintilla *>(viewWidget->modulesTabWidget->widget(it->first));
		codeEditor->zoomIn();
	}
	++fontZoom;
}
