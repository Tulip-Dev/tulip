/*
 * ListPropertyWidget.cpp
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
#include <tulip/ListPropertyWidget.h>

using namespace std;
using namespace tlp;
#include <iostream>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
ListPropertyWidget::ListPropertyWidget(tlp::DynamicTypeHandlerInterface *handler,
		tlp::TulipTableWidgetItem *orig, QWidget* parent) :
	QWidget(parent), handler(handler), orig(orig) {
	setupUi(this);
	tableWidget->setItemPrototype(orig->clone());
	tableWidget->setColumnCount(1);

	fillList();
}

ListPropertyWidget::~ListPropertyWidget() {
	delete orig;
}

void ListPropertyWidget::fillList() {
	tableWidget->clear();
	tableWidget->setRowCount(handler->size());
	for (unsigned int i = 0; i < handler->size(); ++i) {
		if (tableWidget->item(i, 0) != NULL) {
		}
		TulipTableWidgetItem *item = (TulipTableWidgetItem*) orig->clone();
		item->setTextFromTulip(handler->get(i));
		tableWidget->setItem(i, 0, item);
	}
}

void ListPropertyWidget::addRow() {
	tableWidget->setRowCount(tableWidget->rowCount() + 1);
	tableWidget->setItem(tableWidget->rowCount() - 1, 0, orig->clone());
}
void ListPropertyWidget::removeRows() {
	QList<QTableWidgetItem*> selection = tableWidget->selectedItems();
	for (QList<QTableWidgetItem*>::iterator it = selection.begin(); it != selection.end(); ++it) {
		tableWidget->removeRow((*it)->row());
	}
}

void ListPropertyWidget::updateData() {
	handler->clear();
	for (int i = 0; i < tableWidget->rowCount(); ++i) {
		handler->set(i,
			     string(((TulipTableWidgetItem*) tableWidget->item(i, 0))->textForTulip().toUtf8().data()));
	}
}

void ListPropertyWidget::setAll() {
	if (tableWidget->rowCount() != 0) {
		QDialog* dialog = new QDialog(this);
		dialog->setWindowTitle("Set all...");
		dialog->setLayout(new QVBoxLayout());
		QWidget *editWidget = tableWidget->itemDelegate()->createEditor(tableWidget,
				QStyleOptionViewItem(), tableWidget->model()->index(0, 0));
		editWidget->setParent(dialog);
		dialog->layout()->addWidget(editWidget);
		QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
				| QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
		dialog->layout()->addWidget(buttonBox);
		connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
		connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

		if (dialog->exec() == QDialog::Accepted) {
			tableWidget->itemDelegate()->setModelData(editWidget, tableWidget->model(),
					tableWidget->model()->index(0, 0));
			TulipTableWidgetItem *item = (TulipTableWidgetItem *)tableWidget->item(0, 0);
			for (int i = 0; i < tableWidget->rowCount(); ++i) {
				((TulipTableWidgetItem*) tableWidget->item(i, 0))->setTextFromTulip(
												    string(item->textForTulip().toUtf8().data()));
			}
		}
	}
}
