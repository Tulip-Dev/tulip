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
