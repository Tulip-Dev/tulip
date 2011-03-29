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
#include "PythonCodeEditor.h"

#include <iostream>
#include <sstream>

#include <QtCore/QEvent>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <Qsci/qscilexerpython.h>
#include <Qsci/qsciapis.h>

using namespace std;

static string kw;

class CustomLexerPython : public QsciLexerPython {
public:
	CustomLexerPython(QObject *parent=0) : QsciLexerPython(parent) {
		kw = QsciLexerPython::keywords(1);
		kw += " True False";
	}

	const char *keywords (int) const {
		return kw.c_str();
	}
};

class GragKeyboardFocusEventFilter : public QObject{

public :

	bool eventFilter(QObject *, QEvent *event) {
		if (event->type() == QEvent::ShortcutOverride) {
			event->accept();
			return true;
		}

		return false;
	}

};

void loadApiFile(const QString &path, QsciAPIs *api) {
	QFile apiFile(path);
	apiFile.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream in(&apiFile);
	while (!in.atEnd()) {
		QString line = in.readLine();
		api->add(line);
		if (line.startsWith("tulip.tlp.Graph.")) {
			api->add(line.replace("tulip.tlp.Graph", "graph"));
		}
		if (line.startsWith("tulip.tlp.LayoutProperty.")) {
			api->add(line.replace("tulip.tlp.LayoutProperty", "viewLayout"));
		}
		if (line.startsWith("tulip.tlp.DoubleProperty.")) {
			api->add(line.replace("tulip.tlp.DoubleProperty", "viewMetric"));
			api->add(line.replace("tulip.tlp.DoubleProperty", "viewBorderWidth"));
			api->add(line.replace("tulip.tlp.DoubleProperty", "viewRotation"));
		}
		if (line.startsWith("tulip.tlp.ColorProperty.")) {
			api->add(line.replace("tulip.tlp.ColorProperty", "viewColor"));
			api->add(line.replace("tulip.tlp.ColorProperty", "viewBorderColor"));
			api->add(line.replace("tulip.tlp.ColorProperty", "viewLabelColor"));
		}
		if (line.startsWith("tulip.tlp.StringProperty.")) {
			api->add(line.replace("tulip.tlp.StringProperty", "viewLabel"));
			api->add(line.replace("tulip.tlp.StringProperty", "viewTexture"));
			api->add(line.replace("tulip.tlp.StringProperty", "viewFont"));
		}
		if (line.startsWith("tulip.tlp.BooleanProperty.")) {
			api->add(line.replace("tulip.tlp.BooleanProperty", "viewSelection"));
		}
		if (line.startsWith("tulip.tlp.GraphProperty.")) {
			api->add(line.replace("tulip.tlp.GraphProperty", "viewMetaGraph"));
		}
		if (line.startsWith("tulip.tlp.IntegerProperty.")) {
			api->add(line.replace("tulip.tlp.IntegerProperty", "viewShape"));
			api->add(line.replace("tulip.tlp.IntegerProperty", "viewFontSize"));
			api->add(line.replace("tulip.tlp.IntegerProperty", "viewLabelPosition"));
			api->add(line.replace("tulip.tlp.IntegerProperty", "viewSrcAnchorShape"));
			api->add(line.replace("tulip.tlp.IntegerProperty", "viewTgtAnchorShape"));
		}
		if (line.startsWith("tulip.tlp.SizeProperty.")) {
			api->add(line.replace("tulip.tlp.SizeProperty", "viewSize"));
			api->add(line.replace("tulip.tlp.SizeProperty", "viewSrcAnchorSize"));
			api->add(line.replace("tulip.tlp.SizeProperty", "viewTgtAnchorSize"));
		}
	}
	apiFile.close();
}

PythonCodeEditor::PythonCodeEditor(QWidget *parent, int fontZoom) : QsciScintilla(parent) {
	installEventFilter(new GragKeyboardFocusEventFilter());
	QsciLexerPython *pythonLexer = new CustomLexerPython;
	pythonLexer->setIndentationWarning(QsciLexerPython::Inconsistent);
	QsciAPIs *api = new QsciAPIs(pythonLexer);
	loadApiFile(":/tulip.api", api);
	loadApiFile(":/Python-" + QString(PythonInterpreter::getInstance()->getPythonVersion().c_str()) + ".api", api);
	api->add("updateVisualization()");
	api->prepare();
	setUtf8(true);
	setLexer(pythonLexer);
	setBraceMatching(QsciScintilla::SloppyBraceMatch);
	setAutoIndent(true);
	setAutoCompletionSource(QsciScintilla::AcsAll);
	setAutoCompletionThreshold(1);
	setMarginLineNumbers(1, true);
	setMarginWidth(1, "---------");
	setFolding(QsciScintilla::CircledTreeFoldStyle);

	errorIndicator = indicatorDefine(QsciScintilla::SquiggleIndicator);
	setIndicatorForegroundColor(QColor(255,0,0), errorIndicator);

	if (fontZoom > 0) {
		for (int i = 0 ; i < fontZoom ; ++i) {
			zoomIn();
		}
	} else if (fontZoom < 0) {
		for (int i = 0 ; i > fontZoom ; --i) {
			zoomOut();
		}
	}
}

void PythonCodeEditor::indicateScriptCurrentError(int lineNumber) {
	fillIndicatorRange(lineNumber, 0, lineNumber, text(lineNumber).length(), errorIndicator);
}

void PythonCodeEditor::clearErrorIndicator() {
	clearIndicatorRange(0, 0, lines(), text(lines()).length(), errorIndicator);
}
