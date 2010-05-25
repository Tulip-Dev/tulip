/*
 * StringListSelectionWidget.cpp
 *
 *  Created on: 3 févr. 2009
 *      Author: antoine
 */

#include "tulip/SimpleStringsListSelectionWidget.h"
#include "tulip/DoubleStringsListSelectionWidget.h"
#include "tulip/StringsListSelectionWidget.h"

using namespace std;

namespace tlp {

StringsListSelectionWidget::StringsListSelectionWidget(QWidget *parent,
		const ListType listType, const unsigned int maxSelectedStringsListSize) :
	QWidget(parent), listType(listType), stringsListSelectionWidget(NULL) {
	setListType(listType);
	stringsListSelectionWidget->setMaxSelectedStringsListSize(
			maxSelectedStringsListSize);
}

StringsListSelectionWidget::StringsListSelectionWidget(
		const vector<string> &unselectedStringsList, QWidget *parent,
		const ListType listType, const unsigned int maxSelectedStringsListSize) :
	QWidget(parent), listType(listType), stringsListSelectionWidget(NULL) {
	setListType(listType);
	stringsListSelectionWidget->setMaxSelectedStringsListSize(
			maxSelectedStringsListSize);
	stringsListSelectionWidget->setUnselectedStringsList(unselectedStringsList);
}

void StringsListSelectionWidget::setListType(const ListType listType) {
	QLayout *currentLayout = layout();
  delete currentLayout;

	if (stringsListSelectionWidget != NULL) {
		QWidget *widget = dynamic_cast<QWidget *> (stringsListSelectionWidget);
		delete widget;
	}

	if (listType == DOUBLE_LIST) {
		stringsListSelectionWidget = new DoubleStringsListSelectionWidget();
	} else {
		stringsListSelectionWidget = new SimpleStringsListSelectionWidget();
	}
	QVBoxLayout *newLayout = new QVBoxLayout;
	newLayout->addWidget(dynamic_cast<QWidget *> (stringsListSelectionWidget));
	setLayout(newLayout);
}

void StringsListSelectionWidget::setUnselectedStringsList(
		const vector<string> &unselectedStringsList) {
	stringsListSelectionWidget->setUnselectedStringsList(unselectedStringsList);
}

void StringsListSelectionWidget::setSelectedStringsList(
		const vector<string> &selectedStringsList) {
	stringsListSelectionWidget->setSelectedStringsList(selectedStringsList);
}

void StringsListSelectionWidget::clearUnselectedStringsList() {
	stringsListSelectionWidget->clearUnselectedStringsList();
}

void StringsListSelectionWidget::clearSelectedStringsList() {
	stringsListSelectionWidget->clearSelectedStringsList();
}

void StringsListSelectionWidget::setUnselectedStringsListLabel(
		const std::string &unselectedStringsListLabel) {
	if (listType == DOUBLE_LIST) {
		((DoubleStringsListSelectionWidget *) stringsListSelectionWidget)->setUnselectedStringsListLabel(
				unselectedStringsListLabel);
	}
}

void StringsListSelectionWidget::setSelectedStringsListLabel(
		const std::string &selectedStringsListLabel) {
	if (listType == DOUBLE_LIST) {
		((DoubleStringsListSelectionWidget *) stringsListSelectionWidget)->setSelectedStringsListLabel(
				selectedStringsListLabel);
	}
}

void StringsListSelectionWidget::setMaxSelectedStringsListSize(
		const unsigned int maxSelectedStringsListSize) {
	stringsListSelectionWidget->setMaxSelectedStringsListSize(
			maxSelectedStringsListSize);
}

vector<string> StringsListSelectionWidget::getSelectedStringsList() const {
	return stringsListSelectionWidget->getSelectedStringsList();
}

vector<string> StringsListSelectionWidget::getUnselectedStringsList() const {
	return stringsListSelectionWidget->getUnselectedStringsList();
}

std::vector<std::string> StringsListSelectionWidget::getCompleteStringsList() const {
	vector<string> completeList =
			stringsListSelectionWidget->getSelectedStringsList();
	vector<string> unselected =
			stringsListSelectionWidget->getUnselectedStringsList();
	completeList.insert(completeList.end(), unselected.begin(),
			unselected.end());
	return completeList;
}

void StringsListSelectionWidget::selectAllStrings() {
	stringsListSelectionWidget->selectAllStrings();
}

void StringsListSelectionWidget::unselectAllStrings() {
	stringsListSelectionWidget->unselectAllStrings();
}

}
