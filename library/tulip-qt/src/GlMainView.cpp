#include "tulip/GlMainView.h"

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QMenu>
#include <QtGui/QImageWriter>

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
		mainWidget->createPicture(pictureName, mainWidget->width(), mainWidget->height());
	else
		mainWidget->createPicture(pictureName, width, height);
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
}

