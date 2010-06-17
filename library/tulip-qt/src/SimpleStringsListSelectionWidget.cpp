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

#ifndef SIMPLESTRINGSLISTSELECTIONWIDGET_CPP_
#define SIMPLESTRINGSLISTSELECTIONWIDGET_CPP_

#include "tulip/SimpleStringsListSelectionWidget.h"

using namespace std;

namespace tlp {

SimpleStringsListSelectionWidget::SimpleStringsListSelectionWidget(QWidget *parent, const unsigned int maxSelectedStringsListSize) :
		QWidget(parent), maxSelectedStringsListSize(maxSelectedStringsListSize) {
	setupUi(this);
	if (maxSelectedStringsListSize != 0) {
		selectButton->setEnabled(false);
	} else {
		selectButton->setEnabled(true);
	}
	qtWidgetsConnection();
}

void SimpleStringsListSelectionWidget::qtWidgetsConnection() {
	connect(listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(listItemClicked(QListWidgetItem *)));
	connect(upButton, SIGNAL(clicked()), this, SLOT(pressButtonUp()));
	connect(downButton, SIGNAL(clicked()), this, SLOT(pressButtonDown()));
	connect(selectButton, SIGNAL(clicked()), this, SLOT(pressButtonSelectAll()));
	connect(unselectButton, SIGNAL(clicked()), this, SLOT(pressButtonUnselectAll()));

}

void SimpleStringsListSelectionWidget::setUnselectedStringsList(const std::vector<std::string> &unselectedStringsList) {
	for (unsigned int i = 0 ; i < unselectedStringsList.size() ; ++i) {
	  QList<QListWidgetItem *> items = listWidget->findItems(QString::fromUtf8(unselectedStringsList[i].c_str()), Qt::MatchExactly);
		if (items.size() > 0) {
			items[0]->setFlags(items[0]->flags() | Qt::ItemIsUserCheckable);
			items[0]->setCheckState(Qt::Unchecked);
		} else {
		  QListWidgetItem *item = new QListWidgetItem(QString::fromUtf8(unselectedStringsList[i].c_str()));
			item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
			item->setCheckState(Qt::Unchecked);
			listWidget->addItem(item);
		}
	}
}

void SimpleStringsListSelectionWidget::setSelectedStringsList(const std::vector<std::string> &selectedStringsList) {
	for (unsigned int i = 0 ; i < selectedStringsList.size() ; ++i) {
		if (maxSelectedStringsListSize != 0 && getSelectedStringsList().size() == maxSelectedStringsListSize) {
			break;
		}
		QList<QListWidgetItem *> items = listWidget->findItems(QString::fromUtf8(selectedStringsList[i].c_str()), Qt::MatchExactly);
		if (items.size() > 0) {
			items[0]->setFlags(items[0]->flags() | Qt::ItemIsUserCheckable);
			items[0]->setCheckState(Qt::Checked);
		} else {
		  QListWidgetItem *item = new QListWidgetItem(QString::fromUtf8(selectedStringsList[i].c_str()));
			item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
			item->setCheckState(Qt::Checked);
			listWidget->addItem(item);
		}
	}
}

void SimpleStringsListSelectionWidget::clearUnselectedStringsList() {
	vector<QListWidgetItem *> itemsToDelete;
	for (int i = 0 ; i < listWidget->count() ; ++i) {
		QListWidgetItem *item = listWidget->item(i);
		if (item->checkState() == Qt::Unchecked) {
			itemsToDelete.push_back(item);
		}
	}
	for (unsigned int i = 0 ; i < itemsToDelete.size() ; ++i) {
		delete itemsToDelete[i];
	}
}

void SimpleStringsListSelectionWidget::clearSelectedStringsList() {
	vector<QListWidgetItem *> itemsToDelete;
	for (int i = 0 ; i < listWidget->count() ; ++i) {
		QListWidgetItem *item = listWidget->item(i);
		if (item->checkState() == Qt::Checked) {
			itemsToDelete.push_back(item);
		}
	}
	for (unsigned int i = 0 ; i < itemsToDelete.size() ; ++i) {
		delete itemsToDelete[i];
	}
}

void SimpleStringsListSelectionWidget::setMaxSelectedStringsListSize(const unsigned int maxSelectedStringsListSize) {
	this->maxSelectedStringsListSize = maxSelectedStringsListSize;
	if (maxSelectedStringsListSize != 0) {
		selectButton->setEnabled(false);
	} else {
		selectButton->setEnabled(true);
	}
}

vector<string> SimpleStringsListSelectionWidget::getSelectedStringsList() const {
	vector<string> ret;
	for (int i = 0 ; i < listWidget->count() ; ++i) {
		QListWidgetItem *item = listWidget->item(i);
		if (item->checkState() == Qt::Checked) {
		  ret.push_back(string(item->text().toUtf8().data()));
		}
	}
	return ret;
}

vector<string> SimpleStringsListSelectionWidget::getUnselectedStringsList() const {
	vector<string> ret;
	for (int i = 0 ; i < listWidget->count() ; ++i) {
		QListWidgetItem *item = listWidget->item(i);
		if (item->checkState() == Qt::Unchecked) {
		  ret.push_back(string(item->text().toUtf8().data()));
		}
	}
	return ret;
}

void SimpleStringsListSelectionWidget::selectAllStrings() {
	for (int i = 0 ; i < listWidget->count() ; ++i) {
		QListWidgetItem *item = listWidget->item(i);
		item->setCheckState(Qt::Checked);
	}
}

void SimpleStringsListSelectionWidget::unselectAllStrings() {
	for (int i = 0 ; i < listWidget->count() ; ++i) {
		QListWidgetItem *item = listWidget->item(i);
		item->setCheckState(Qt::Unchecked);
	}
}

void SimpleStringsListSelectionWidget::pressButtonUp() {
	if (listWidget->count() > 0) {
		int row = listWidget->currentRow();
		if (row > 0) {
			QListWidgetItem *item1 = new QListWidgetItem(*(listWidget->currentItem()));
			QListWidgetItem *item2 = new QListWidgetItem(*(listWidget->item(row - 1)));
			delete listWidget->item(row - 1);
			delete listWidget->item(row - 1);
			listWidget->insertItem(row - 1, item2);
			listWidget->insertItem(row - 1, item1);
			listWidget->setCurrentRow(row - 1);
		}
	}
}

void SimpleStringsListSelectionWidget::pressButtonDown() {
	if (listWidget->count() > 0) {
		int row = listWidget->currentRow();
		if (row < (listWidget->count() - 1)) {
			QListWidgetItem *item1 = new QListWidgetItem(*(listWidget->currentItem()));
			QListWidgetItem *item2 = new QListWidgetItem(*(listWidget->item(row + 1)));
			delete listWidget->item(row);
			delete listWidget->item(row);
			listWidget->insertItem(row, item1);
			listWidget->insertItem(row, item2);
			listWidget->setCurrentRow(row + 1);
		}
	}
}

void SimpleStringsListSelectionWidget::pressButtonSelectAll() {
	selectAllStrings();
}

void SimpleStringsListSelectionWidget::pressButtonUnselectAll() {
	unselectAllStrings();
}

void SimpleStringsListSelectionWidget::listItemClicked(QListWidgetItem *item) {
	if (maxSelectedStringsListSize != 0 && getSelectedStringsList().size() > maxSelectedStringsListSize) {
		if (item->checkState() == Qt::Checked) {
			item->setCheckState(Qt::Unchecked);
		}
	}
}

}

#endif /* SIMPLESTRINGSLISTSELECTIONWIDGET_CPP_ */
