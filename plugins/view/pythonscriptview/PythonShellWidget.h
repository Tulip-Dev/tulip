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

#ifndef PYTHONSHELLWIDGET_H_
#define PYTHONSHELLWIDGET_H_

#include <vector>

#include <QtGui/QPlainTextEdit>

#include "PythonCodeEditor.h"

#include <tulip/Graph.h>

class QKeyEvent;
class QsciAPIs;
class QsciLexer;

class PythonShellWidget : public QsciScintilla {

	Q_OBJECT

public :

	PythonShellWidget(QWidget *parent = 0);

	void keyPressEvent(QKeyEvent * e);

	bool isCursorOnLastLine();

	void setConsoleWidget(QPlainTextEdit *consoleWidget) {this->consoleWidget = consoleWidget;}

	void executeCurrentLines();

	void setCurrentGraph(tlp::Graph *graph);

protected:

	void showEvent(QShowEvent * event);

public slots:

	void insert(const QString &txt, const bool atEnd = false);
	void treatUserListSelection(int id, const QString &text);

private :

	QString currentPs;
	QString currentCodeLines;

	std::vector<QString> history;
	int currentHistoryPos;

	QsciLexer *pythonLexer;
	QsciAPIs *api;

	QPlainTextEdit *consoleWidget;

};

#endif /* PYTHONSHELLWIDGET_H_ */
