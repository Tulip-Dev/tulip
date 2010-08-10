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
#include "tulip/AbstractView.h"

#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QFrame>
#include <QtOpenGL/QGLWidget>
#include <QtGui/QMenu>
#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMenuBar>
#include <QtGui/QImageWriter>
#include <QtGui/QFileDialog>

#include "tulip/Interactor.h"

using namespace std;

namespace tlp {

AbstractView::AbstractView() :
	View(), centralWidget(NULL), activeInteractor(NULL) {

}

AbstractView::~AbstractView() {
	for (list<Interactor *>::iterator it = interactors.begin(); it != interactors.end(); ++it) {
		delete (*it);
	}
}

QWidget *AbstractView::construct(QWidget *parent) {
	widget = new QWidget(parent);
	QGridLayout *gridLayout = new QGridLayout(widget);
	gridLayout->setSpacing(0);
	gridLayout->setMargin(0);

	mainLayout = new QVBoxLayout;

	gridLayout->addLayout(mainLayout, 0, 0, 1, 1);

	// Add this to by-pass a bug in Qt 4.4.1
	// In the QWorkspace if the widget doesn't have a QGLWidget this widget pass below others widget
	QFrame *frame = new QFrame(widget);
	frame->setGeometry(QRect(0, 0, 0, 0));
	QGridLayout *gridLayout_2 = new QGridLayout(frame);
	QWidget *widget_2 = new QGLWidget(frame);

	//Build output image list
	exportImageMenu = new QMenu("&Save Picture as ");

	set<string> imgFormats;
	buildOutputImagesFormatsList(imgFormats);

	for (set<string>::iterator it = imgFormats.begin(); it != imgFormats.end(); ++it) {
		exportImageMenu->addAction(QString::fromStdString(*it));
	}

	connect(exportImageMenu, SIGNAL(triggered(QAction*)), SLOT(exportImage(QAction*)));

	widget->installEventFilter(this);

	return widget;
}

void AbstractView::setInteractors(const std::list<Interactor *> &interactorsList) {
	interactors = interactorsList;
	for (list<Interactor *>::iterator it = interactors.begin(); it != interactors.end(); ++it) {
		(*it)->setView(this);
	}
}

list<Interactor *> AbstractView::getInteractors() {
	return interactors;
}

void AbstractView::setActiveInteractor(Interactor *interactor) {
	if (activeInteractor)
		activeInteractor->remove();
	interactor->install(centralWidget);
	activeInteractor = interactor;
}

void AbstractView::setCentralWidget(QWidget *widget) {
	widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	mainLayout->addWidget(widget);
	centralWidget = widget;
}

bool AbstractView::eventFilter(QObject *object, QEvent *event) {
	specificEventFilter(object, event);

	if (event->type() == QEvent::MouseButtonPress) {
		QMouseEvent *me = (QMouseEvent *) event;
		if (me->button() == Qt::RightButton) {
			QMenu contextMenu(getWidget());
			buildContextMenu(object, me, &contextMenu);
			if (!contextMenu.actions().isEmpty()) {
				QAction* menuAction = contextMenu.exec(me->globalPos());
				if (menuAction)
					computeContextMenuAction(menuAction);
			}
		}
	}
	return false;
}

void AbstractView::buildOutputImagesFormatsList(set<string>& outputFormats) {
	// Tulip known formats (see GlGraph)
	// formats are sorted, "~" is just an end marker
	//Image PopuMenu
	// int Qt 4, output formats are not yet sorted and uppercased
	// first add Tulip known formats
	outputFormats.insert("EPS");
	outputFormats.insert("SVG");
	// uppercase and insert all Qt formats
	foreach (QByteArray format, QImageWriter::supportedImageFormats())
		{
			char* tmp = format.data();
			for (int i = strlen(tmp) - 1; i >= 0; --i)
				tmp[i] = toupper(tmp[i]);
			outputFormats.insert(tmp);
		}
	// sort before inserting in exportImageMenu
}

void AbstractView::exportImage(QAction* action) {
        QString extension = action->text().toLower();
	QString s(QFileDialog::getSaveFileName(NULL, QString("Save Picture as ") + extension + " file", QString(), QString("Images (*.") + extension + ")"));

	if (s.isNull())
		return;

	//If no extension found automatically add the selected format extension.
	if(!s.contains(QChar('.')) ) {
	  s.append('.');
	  s.append(extension);
	}
	createPicture(s.toStdString());
}

void AbstractView::buildContextMenu(QObject *, QMouseEvent *, QMenu *contextMenu) {
	if (!exportImageMenu->isEmpty())
		contextMenu->addMenu(exportImageMenu);
}

}
