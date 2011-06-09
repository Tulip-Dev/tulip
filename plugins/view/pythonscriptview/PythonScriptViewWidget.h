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

#ifndef PYTHONSCRIPTVIEWWIDGET_H_
#define PYTHONSCRIPTVIEWWIDGET_H_

#include "PythonScriptViewWidgetData.h"
#include <QtGui/QAction>

#include "PythonCodeEditor.h"

class QToolBar;

class PythonScriptView;

class PythonScriptViewWidget : public QWidget, public Ui::PythonScriptViewWidgetData {

	Q_OBJECT

public :

	PythonScriptViewWidget(PythonScriptView *view, QWidget *parent);

	void showEvent(QShowEvent *);
	void resizeEvent(QResizeEvent *);

	int addMainScriptEditor();
	int addModuleEditor();

	PythonCodeEditor *getMainScriptEditor(int idx) const;
	PythonCodeEditor *getCurrentMainScriptEditor() const;
	PythonCodeEditor *getModuleEditor(int idx) const;
	PythonCodeEditor *getCurrentModuleEditor() const;

	std::string getCurrentMainScriptCode() const;
	std::string getMainScriptCode(int idx) const;
	std::string getModuleCode(int idx) const;

	bool eventFilter(QObject *obj, QEvent *event);

public slots :

	void decreaseFontSize();
	void increaseFontSize();
	void mainScriptTextChanged();
	void moduleScriptTextChanged();
	void resizeToolBars();

	void currentTabChanged(int index);

public :

	QAction *newMainScriptAction;
	QAction *loadMainScriptAction;
	QAction *saveMainScriptAction;
	QAction *newStringModuleAction;
	QAction *newFileModuleAction;
	QAction *loadModuleAction;
	QAction *saveModuleAction;

	QToolBar *mainScriptToolBar;
	QToolBar *modulesToolBar;

private :

	int fontZoom;


	void commentSelectedCode(QObject *obj);
	void uncommentSelectedCode(QObject *obj);

	PythonScriptView *pythonScriptView;
};

#endif /* PYTHONSCRIPTVIEWWIDGET_H_ */
