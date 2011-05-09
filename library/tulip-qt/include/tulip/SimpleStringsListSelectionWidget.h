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

#ifndef SIMPLESTRINGSLISTSELECTIONWIDGET_H_
#define SIMPLESTRINGSLISTSELECTIONWIDGET_H_

#include <QtGui/QWidget>

#include "StringsListSelectionWidgetInterface.h"
#include "ui_SimpleStringsListSelectionWidget.h"

namespace tlp {

class TLP_QT_SCOPE SimpleStringsListSelectionWidget : public QWidget, public StringsListSelectionWidgetInterface, public Ui::SimpleStringsListSelectionData {

	Q_OBJECT

public :

	SimpleStringsListSelectionWidget(QWidget *parent = 0, const unsigned int maxSelectedStringsListSize = 0);

	void setUnselectedStringsList(const std::vector<std::string> &unselectedStringsList);

	void setSelectedStringsList(const std::vector<std::string> &selectedStringsList);

	void clearUnselectedStringsList();

	void clearSelectedStringsList();

	void setMaxSelectedStringsListSize(const unsigned int maxSelectedStringsListSize);

	std::vector<std::string> getSelectedStringsList() const;

	std::vector<std::string> getUnselectedStringsList() const;

	void selectAllStrings();

	void unselectAllStrings();

private slots :

	void pressButtonSelectAll();
	void pressButtonUnselectAll();
	void listItemClicked(QListWidgetItem *item);
	void pressButtonUp();
	void pressButtonDown();

private :

	void qtWidgetsConnection();

	unsigned int maxSelectedStringsListSize;
};

}

#endif /* SIMPLESTRINGSLISTSELECTIONWIDGET_H_ */
