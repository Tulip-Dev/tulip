/*
 * SpreadConfigurationWidget.h
 *
 *  Created on: 1 juil. 2009
 *      Author: Jonathan Dubois
 *       Email: jonathan.dubois@labri.fr
 *
 *	This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 */

#ifndef SPREADCONFIGURATIONWIDGET_H_
#define SPREADCONFIGURATIONWIDGET_H_

#include <QtGui/QWidget>
#include "SpreadConfigurationWidgetData.h"
namespace tlp {
class Graph;

class SpreadConfigurationWidget: public QWidget, public Ui::SpreadConfigurationWidget {
Q_OBJECT
	;
public:
	SpreadConfigurationWidget(QWidget* parent = NULL);
	virtual ~SpreadConfigurationWidget();
	void setGraph(tlp::Graph *graph);
	std::vector<std::string> getSelectedProperties();
	void setSelectedProperties(const std::vector<std::string>& properties);
	bool allPropertiesSelected();

signals:
	void updated();
};
}
#endif /* SPREADCONFIGURATIONWIDGET_H_ */
