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

#ifndef PYTHONCODEEDITOR_H_
#define PYTHONCODEEDITOR_H_

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerpython.h>

#include <QtCore/QEvent>

static std::string kw;

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

class PythonCodeEditor : public QsciScintilla {

public :

	PythonCodeEditor(QWidget *parent=0, const int foontZoom=0);

	void indicateScriptCurrentError(int lineNumber);
	void clearErrorIndicator();

protected:

	void showEvent(QShowEvent * event);

private :

	int errorIndicator;

	static QsciLexerPython *pythonLexer;
};


#endif /* PYTHONCODEEDITOR_H_ */
