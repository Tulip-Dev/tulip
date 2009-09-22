/*
 * ListPropertyWidget.h
 *
 *  Created on: 16 sept. 2009
 *      Author: Jonathan Dubois
 *       Email: jonathan.dubois@labri.fr
 *
 *	This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 */

#ifndef LISTPROPERTYWIDGET_H_
#define LISTPROPERTYWIDGET_H_
#include "tulip/ListPropertyWidgetData.h"
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
