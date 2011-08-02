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

#include <iostream>

#include <QtCore/QRegExp>
#include <QtGui/QKeyEvent>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDir>

#include <Qsci/qsciapis.h>

#include <tulip/TlpTools.h>
#include <tulip/Observable.h>

#include "PythonShellWidget.h"


using namespace std;

static QRegExp linesepRegExp = QRegExp("\r\n|\n|\r");

static QString ps1 = ">>> ";
static QString ps2 = "....... ";

static QString rtrim(const QString &s) {
	int lastNonSpaceIdx = s.length() -1;
	while (s.at(lastNonSpaceIdx).isSpace()) {
		--lastNonSpaceIdx;
	}
	return s.mid(0, lastNonSpaceIdx+1);
}

static QString ltrim(const QString &s) {
	int firstNonSpaceIdx = 0;
	while (s.at(firstNonSpaceIdx).isSpace()) {
		++firstNonSpaceIdx;
	}
	return s.mid(firstNonSpaceIdx);
}

static void loadApiFile(const QString &path, QsciAPIs *api) {
	QFile apiFile(path);
	apiFile.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream in(&apiFile);
	while (!in.atEnd()) {
		QString line = in.readLine();
		api->add(line);
	}
	apiFile.close();
}

const string setCurrentGraphFunction =
		"graph = None\n"
		"def setCurrentGraph(g):\n"
		"	global graph\n"
		"	graph = g\n"
		;

PythonShellWidget::PythonShellWidget(QWidget *parent) : QsciScintilla(parent) {
	setUtf8(true);
	installEventFilter(new GragKeyboardFocusEventFilter());
	pythonLexer = new CustomLexerPython();
	api = new QsciAPIs(pythonLexer);
	QDir apiDir(QString(tlp::TulipBitmapDir.c_str()) + "../apiFiles");
	QStringList nameFilter;
	loadApiFile(QString(tlp::TulipBitmapDir.c_str()) + "../apiFiles/Python-" + QString(PythonInterpreter::getInstance()->getPythonVersion().c_str()) + ".api", api);
	api->prepare();
	setLexer(pythonLexer);
	setAutoCompletionSource(QsciScintilla::AcsAll);
	setBraceMatching(QsciScintilla::SloppyBraceMatch);
	insert(QString(PythonInterpreter::getInstance()->getPythonShellBanner().c_str())+ QString("\n"));
	insert("# The current graph is bound to the \"graph\" variate\n");
	insert(ps1);
	currentPs = ps1;
	currentHistoryPos = -1;

	PythonInterpreter::getInstance()->runString(setCurrentGraphFunction);

	connect(this, SIGNAL(userListActivated(int, const QString &)), this, SLOT(treatUserListSelection(int, const QString &)));
}

bool PythonShellWidget::isCursorOnLastLine() {
	int cline, ccol;
	getCursorPosition(&cline, &ccol);
	return cline == lines() - 1;
}

void PythonShellWidget::insert(const QString &txt, const bool atEnd) {
	int l = txt.length();
	int line, col;
	if (atEnd) {
		line = lines() - 1;
		col = lineLength(line);
	} else {
		getCursorPosition(&line, &col);
	}
	insertAt(txt, line, col);
	setCursorPosition(line + txt.count(linesepRegExp), col + l);
}

void PythonShellWidget::keyPressEvent(QKeyEvent * e) {

	int key = e->key();
	QString txt = e->text();

	if (isListActive()) {
		if (key ==  Qt::Key_Enter || key ==  Qt::Key_Return || key ==  Qt::Key_Up ||
				key ==  Qt::Key_Down || key ==  Qt::Key_Escape) {
			QsciScintilla::keyPressEvent(e);
		} else {
			cancelList();
			if (key !=  Qt::Key_Home)
				QsciScintilla::keyPressEvent(e);
		}
		return;
	}

	QString lineNotTrimmed = text(lines()-1).mid(currentPs.length());
	QString line = rtrim(text(lines()-1)).mid(currentPs.length());

	int cline, ccol;
	getCursorPosition(&cline, &ccol);

	if (key ==  Qt::Key_Backspace || key ==  Qt::Key_Left || key == Qt::Key_Right) {
		if (isCursorOnLastLine()) {
			if (key ==  Qt::Key_Backspace && selectedText() != "") {
				removeSelectedText();
			} else {
				int line, col;
				getCursorPosition(&line, &col);
				if (col > currentPs.length()) {
					QsciScintilla::keyPressEvent(e);
				}
				if (key == Qt::Key_Right && col == currentPs.length()) {
					QsciScintilla::keyPressEvent(e);
				}

			}
		} else {
			setCursorPosition(lines() - 1, lineLength(lines() - 1));
		}
	} else if (key ==  Qt::Key_Up) {
		if (isCursorOnLastLine()) {
			if (currentHistoryPos >= 0) {

				setSelection(lines()-1, 0, lines()-1, lineLength(lines()-1));
				removeSelectedText();
				insert(currentPs + history[currentHistoryPos]);
				if (currentHistoryPos != 0)
					--currentHistoryPos;
			}
		} else {
			setCursorPosition(lines() - 1, lineLength(lines() - 1));
		}
	} else if (key ==  Qt::Key_Down) {
		if (isCursorOnLastLine()) {
			setSelection(lines()-1, 0, lines()-1, lineLength(lines()-1));
			removeSelectedText();
			if (currentHistoryPos < static_cast<int>(history.size() -  1) && history.size() > 0) {
				++currentHistoryPos;
				insert(currentPs + history[currentHistoryPos]);
			} else {
				insert(currentPs);
			}
		} else {
			setCursorPosition(lines() - 1, lineLength(lines() - 1));
		}
	} else if (key ==  Qt::Key_Home) {
		if (isCursorOnLastLine()) {

			if (e->modifiers() == Qt::ShiftModifier) {
				setSelection(lines()-1, currentPs.length(), lines()-1, ccol);
			} else {
				setCursorPosition(lines()-1, currentPs.length());
			}
		} else {
			setCursorPosition(lines() - 1, lineLength(lines() - 1));
		}
	} else if (key ==  Qt::Key_End) {
		if (isCursorOnLastLine()) {

			if (e->modifiers() == Qt::ShiftModifier) {
				setSelection(lines()-1, ccol, lines()-1, lineLength(lines()-1));
			} else {
				setCursorPosition(lines()-1, lineLength(lines() - 1));
			}
		} else {
			setCursorPosition(lines() - 1, lineLength(lines() - 1));
		}
	} else if (key ==  Qt::Key_A && e->modifiers() == Qt::ControlModifier) {
		if (isCursorOnLastLine()) {
			setSelection(lines()-1, currentPs.length(), lines()-1, lineLength(lines()-1));
		}
	} else if ((key ==  Qt::Key_Enter || key ==  Qt::Key_Return)) {
		if  (isCursorOnLastLine()) {
			if (line != "") {
				currentCodeLines += (line + "\n");
				history.push_back(line);
				currentHistoryPos = history.size() - 1;
			}
			setCursorPosition(lines() - 1, lineLength(lines() - 1));
			QsciScintilla::keyPressEvent(e);
			if (line != "") {
				if(currentPs == ps2 || line.endsWith(":")) {
					currentPs = ps2;
					insert(ps2);
				}  else {
					executeCurrentLines();
					currentPs = ps1;
					insert(ps1);
				}
			} else {
				executeCurrentLines();
				currentPs = ps1;
				insert(ps1);
			}
		}
	} else if (line != "" && key == Qt::Key_Tab) {
		if (isCursorOnLastLine()) {
			if (isListActive()) {
				cancelList();
			} else {
				if (lineNotTrimmed[lineNotTrimmed.length()-1] != ' ' && lineNotTrimmed[lineNotTrimmed.length()-1] != '\t') {
					QStringList context = ltrim(line).split(".");
					for (int i = 0 ; i < context.count() ; ++i) {
						context[i].replace("=", " ");
						context[i].replace("+", " ");
						context[i].replace("-", " ");
						context[i].replace("*", " ");
						context[i].replace("%", " ");
						context[i].replace("/", " ");
						context[i].replace("^", " ");
						context[i].replace("<", " ");
						context[i].replace(">", " ");
						context[i].replace("|", " ");
						context[i].replace("!", " ");
						QStringList words = context[i].split(" ");
						context[i] = words.back();
					}

					QStringList autoCompletionResult;
					if (context.size() == 2) {
						vector<string> dynamicAutoCompletionList = PythonInterpreter::getInstance()->getObjectDictEntries(context[0].toStdString(), context[1].toStdString());
						for (size_t i = 0 ; i < dynamicAutoCompletionList.size() ; ++i) {
							autoCompletionResult.push_back(QString(dynamicAutoCompletionList[i].c_str()));
						}
					}

					if (autoCompletionResult.count() == 0) {
						if (context.size() == 1) {
							vector<string> dynamicAutoCompletionList = PythonInterpreter::getInstance()->getGlobalDictEntries(context[0].toStdString());
							for (size_t i = 0 ; i < dynamicAutoCompletionList.size() ; ++i) {
								autoCompletionResult.push_back(QString(dynamicAutoCompletionList[i].c_str()));
							}
						}
						api->updateAutoCompletionList(context, autoCompletionResult);
					}

					if (autoCompletionResult.count() > 0) {
						showUserList(1, autoCompletionResult);
					}
				}
			}
		} else {
			setCursorPosition(lines() - 1, lineLength(lines() - 1));
		}
	} else if (txt.length() > 0 && txt >= " ") {
		if (isCursorOnLastLine()) {
			insert(txt);
		} else {
			insert(txt, true);
		}
	} else {
		QsciScintilla::keyPressEvent(e);
	}
}

void PythonShellWidget::treatUserListSelection(int, const QString &txt) {
	QString line = text(lines()-1);
	int col = line.length() - 1;
	while (col > 0 && line.at(col) != ' ' && line.at(col) != '\t' && line.at(col) != '.') {
		--col;
	}
	setSelection(lines()-1, col+1, lines()-1, lineLength(lines()-1));
	removeSelectedText();
	QString selectedEntry = txt;
	if (selectedEntry.indexOf("(") != -1) {
		selectedEntry = selectedEntry.mid(0, selectedEntry.indexOf("("));
	}
	if (selectedEntry[selectedEntry.length()-1] == ' ') {
		selectedEntry = selectedEntry.mid(0, selectedEntry.length()-1);
	}
	insert(selectedEntry);
}

void PythonShellWidget::executeCurrentLines() {
	tlp::Observable::holdObservers();
	PythonInterpreter::getInstance()->setPythonShellWidget(this);
	PythonInterpreter::getInstance()->runString(currentCodeLines.toStdString());
	currentCodeLines = "";
	PythonInterpreter::getInstance()->setDefaultConsoleWidget();
	PythonInterpreter::getInstance()->setDefaultSIGINTHandler();
	tlp::Observable::unholdObservers();
}

void PythonShellWidget::setCurrentGraph(tlp::Graph *graph) {
	PythonInterpreter::getInstance()->runGraphScript("__main__", "setCurrentGraph", graph);
}

void PythonShellWidget::showEvent(QShowEvent *) {
	setFocus();
}
