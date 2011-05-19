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

#ifndef LISTPROPERTYWIDGET_H_
#define LISTPROPERTYWIDGET_H_
#include "ui_ListPropertyWidget.h"
#include <tulip/TulipTableWidget.h>
#include <QtGui/QTableWidgetItem>
class ListPropertyWidget: public QWidget, public Ui::Form {
Q_OBJECT
	;
public:
	ListPropertyWidget(tlp::DynamicTypeHandlerInterface *handler, tlp::TulipTableWidgetItem *orig,
			QWidget* parent = NULL);
	virtual ~ListPropertyWidget();
	void updateData();
private slots:
	void addRow();
	void removeRows();
	void setAll();
private:

	void fillList();

	tlp::DynamicTypeHandlerInterface *handler;
	tlp::TulipTableWidgetItem *orig;
};

#endif /* LISTPROPERTYWIDGET_H_ */
