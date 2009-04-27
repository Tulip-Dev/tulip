/*
	  SimpleStringsListSelectionWidget.h

   Created on: 3 avr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#ifndef SIMPLESTRINGSLISTSELECTIONWIDGET_H_
#define SIMPLESTRINGSLISTSELECTIONWIDGET_H_

#include <QtGui/QWidget>

#include "StringsListSelectionWidgetInterface.h"
#include "SimpleStringsListSelectionWidgetData.h"

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
