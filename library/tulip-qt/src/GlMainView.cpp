/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include "tulip/GlMainView.h"

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QMenu>
#include <QtGui/QImageWriter>

#include "tulip/GlMainWidget.h"
#include "tulip/FindSelectionWidget.h"
#include "tulip/TabWidget.h"
#include "tulip/GWOverviewWidget.h"

using namespace std;

namespace tlp {

//==================================================
GlMainView::GlMainView() :
	AbstractView() {
}
//==================================================
GlMainView::~GlMainView() {
}
//==================================================
QWidget *GlMainView::construct(QWidget *parent) {
	QWidget *widget = AbstractView::construct(parent);

	mainWidget = new GlMainWidget(widget, this);

	setCentralWidget(mainWidget);

	// Create overview widget
	overviewFrame = new QFrame(mainWidget);
	overviewFrame->setGeometry(QRect(0, 0, 100, 100));
	overviewFrame->setFrameShape(QFrame::StyledPanel);
	overviewFrame->setFrameShadow(QFrame::Raised);
	overviewFrame->setAutoFillBackground(true);
	QGridLayout *gridLayout_2 = new QGridLayout(overviewFrame);
	gridLayout_2->setSpacing(6);
	gridLayout_2->setMargin(0);
	overviewWidget = new GWOverviewWidget(overviewFrame);
	gridLayout_2->addWidget(overviewWidget, 0, 0, 1, 1);
	connect(overviewWidget, SIGNAL(hideOverview(bool)), this, SLOT(hideOverview(bool)));

	dialogMenu=new QMenu("Dialog");
	connect(dialogMenu, SIGNAL(triggered(QAction*)), SLOT(showDialog(QAction*)));
	overviewAction=dialogMenu->addAction("3D &Overview");
	overviewAction->setCheckable(true);
	overviewAction->setChecked(true);

	return widget;
}
//==================================================
void GlMainView::createPicture(const std::string &pictureName, int width, int height) {

	string extension = pictureName.substr(pictureName.find_last_of('.') + 1);

	for (unsigned int i = 0; i < extension.size(); ++i)
		extension[i] = tolower(extension[i]);

	if (extension.compare("eps") == 0) {
		if (!mainWidget->outputEPS(64000000, true, pictureName.c_str()))
			QMessageBox::critical(0, "Save Picture Failed", "The file has not been saved.");
		return;
	} else if (extension.compare("svg") == 0) {
		if (!mainWidget->outputSVG(64000000, pictureName.c_str()))
			QMessageBox::critical(0, "Save Picture Failed", "The file has not been saved.");
		return;
	}

	if (width == 0 && height == 0)
        mainWidget->createPicture(pictureName, mainWidget->width(), mainWidget->height(),false);
	else
        mainWidget->createPicture(pictureName, width, height,false);
}

//==================================================
// Accessor and set
//==================================================
Graph *GlMainView::getGraph() {
	return mainWidget->getGraph();
}
//==================================================
GlMainWidget *GlMainView::getGlMainWidget() {
	return mainWidget;
}
//==================================================
void GlMainView::buildContextMenu(QObject *object, QMouseEvent *event, QMenu *contextMenu) {
	AbstractView::buildContextMenu(object, event, contextMenu);
	contextMenu->addMenu(dialogMenu);
}

//==================================================
// GUI functions
//==================================================
void GlMainView::draw() {
	mainWidget->draw();
}
//==================================================
void GlMainView::refresh() {
	draw();
}
//==================================================
void GlMainView::hideOverview(bool hide) {
	if (hide) {
		overviewFrame->hide();
	} else {
		overviewFrame->show();
	}
	overviewAction->setChecked(!hide);
}
//==================================================
void GlMainView::showDialog(QAction* action){
  string name(action->text().toStdString());

  if (name=="3D &Overview") {
    if(overviewFrame->isVisible())
      overviewFrame->hide();
    else
      overviewFrame->show();
  }
}
}

