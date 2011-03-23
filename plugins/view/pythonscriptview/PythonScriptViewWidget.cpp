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

#include <QtGui/QToolBar>
#include <QtGui/QIcon>

#include "PythonScriptViewWidget.h"

PythonScriptViewWidget::PythonScriptViewWidget(QWidget *parent) : QWidget(parent) {
	setupUi(this);
	mainScriptToolBar = new QToolBar(mainScriptToolBarWidget);
	newMainScriptAction = mainScriptToolBar->addAction(QIcon(":/icons/doc_new.png"), "New main script");
	loadMainScriptAction = mainScriptToolBar->addAction(QIcon(":/icons/doc_import.png"), "Load main script from file");
	saveMainScriptAction = mainScriptToolBar->addAction(QIcon(":/icons/doc_export.png"), "Save main script to file");

	modulesToolBar = new QToolBar(modulesToolBarWidget);
	modulesToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
	newStringModuleAction = modulesToolBar->addAction(QIcon(":/icons/doc_plus.png"), "New string module");
	newFileModuleAction = modulesToolBar->addAction(QIcon(":/icons/doc_new.png"), "New file module");
	loadModuleAction = modulesToolBar->addAction(QIcon(":/icons/doc_import.png"), "Import module from file");
	saveModuleAction = modulesToolBar->addAction(QIcon(":/icons/doc_export.png"), "Save module to file");


	modulesTabWidget->clear();
	QList<int> sizes;
	sizes.push_back(500);
	sizes.push_back(200);
	splitter->setSizes(sizes);

	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(resizeToolBars()));
}

std::string PythonScriptViewWidget::getPythonCode() const {
	return pythonCodeWidget->text().replace("\r\n", "\n").toStdString();
}

void PythonScriptViewWidget::resizeEvent(QResizeEvent *e) {
	QWidget::resizeEvent(e);
	resizeToolBars();
}

void PythonScriptViewWidget::showEvent(QShowEvent *e) {
	QWidget::showEvent(e);
	resizeToolBars();
}

void PythonScriptViewWidget::resizeToolBars() {
	modulesToolBar->resize(modulesToolBarWidget->size());
	mainScriptToolBar->resize(mainScriptToolBarWidget->size());
}
