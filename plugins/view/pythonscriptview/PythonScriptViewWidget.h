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

class QToolBar;

class PythonScriptViewWidget : public QWidget, public Ui::PythonScriptViewWidgetData {

	Q_OBJECT

public :

	PythonScriptViewWidget(QWidget *parent=0);

	void showEvent(QShowEvent *);
	void resizeEvent(QResizeEvent *);

	std::string getPythonCode() const;

public slots :

	void resizeToolBars();

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

};

#endif /* PYTHONSCRIPTVIEWWIDGET_H_ */
