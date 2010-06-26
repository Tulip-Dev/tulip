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
#include "SpreadWidget.h"

#include <iostream>

#include <tulip/Graph.h>
#include "SpreadView.h"
using namespace std;

namespace tlp {

SpreadWidget::SpreadWidget(SpreadView* view, QWidget *parent) :
	currentCell(NULL), editingLine(false), nodeTab(true) {
	setupUi(this);
	spreadNodesTable->setView(SpreadTable::NodesView);
	spreadEdgesTable->setView(SpreadTable::EdgesView);
	spreadNodesTable->setLinkedSpreadView(view);
	spreadEdgesTable->setLinkedSpreadView(view);
	/*vboxLayout = new QVBoxLayout(this);
	 setCentralWidget(vboxLayout);
	 vboxLayout->setContentsMargins(0, 0, 0, 0);
	 lineEdit = new SpreadLineEdit();
	 vboxLayout->addWidget(lineEdit);
	 tabWidget = new QTabWidget();
	 spreadNodesTable = new SpreadTable(SpreadTable::NodesView);
	 spreadNodesTable->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
	 spreadEdgesTable = new SpreadTable(SpreadTable::EdgesView);
	 spreadEdgesTable->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
	 vboxLayout->addWidget(tabWidget);
	 tabWidget->addTab(spreadNodesTable,"Nodes");
	 tabWidget->addTab(spreadEdgesTable,"Edges");
	 //connect(spreadTable, SIGNAL(cellPressed(int,int)), this, SLOT(cellPressedSlot(int,int)));*/
	//noModif : connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(returnPressedSlot()));
	//noModif : connect(lineEdit, SIGNAL(widgetActivate()), this, SLOT(beginEditLineSlot()));
	connect(spreadNodesTable, SIGNAL(mouseReleasedSignal(SpreadTable*,QTableWidgetItem *)), this,
			SLOT(tableMouseReleasedSlot(SpreadTable*,QTableWidgetItem *)));
	connect(spreadEdgesTable, SIGNAL(mouseReleasedSignal(SpreadTable*,QTableWidgetItem *)), this,
			SLOT(tableMouseReleasedSlot(SpreadTable*,QTableWidgetItem *)));
	//noModif : connect(lineEdit, SIGNAL(textChanged (const QString &) ), this, SLOT(textChangedSlot(const QString &)));
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
}

void SpreadWidget::setData(Graph *graph, DataSet dataSet) {
	this->graph = graph;
	spreadNodesTable->setGraph(graph);
	spreadEdgesTable->setGraph(graph);
}

DataSet SpreadWidget::getData() {
	return DataSet();
}

Graph* SpreadWidget::getGraph() {
	return graph;
}

void SpreadWidget::redrawView() {
	spreadEdgesTable->redrawView();
	spreadNodesTable->redrawView();
}

void SpreadWidget::tableMouseReleasedSlot(SpreadTable* table, QTableWidgetItem *cell) {
	QString right = currentText.right(1);
	if (editingLine && (right == "(" || right == "+" || right == "-" || right == "*" || right
			== "/" || right == "=")) {
		QString position;
		QList<QTableWidgetItem *> items = table->selectedItems();
		if (items.size() == 1) {
			SpreadTable::encodePosition(position, table->row(items[0]), table->column(items[0]));
			//noModif : lineEdit->setText(currentText+position);
		} else {
			QList<QTableWidgetSelectionRange> ranges = table->selectedRanges();
			assert(ranges.size()==1);
			SpreadTable::encodeRange(position, ranges[0]);
			//noModif : lineEdit->setText(currentText+position);
		}
		//noModif : lineEdit->setFocus(Qt::OtherFocusReason);
	} else {
		if (cell) {
			currentCell = cell;
			//noModif : lineEdit->setText(currentCell->text());
			currentText = currentCell->text();
		} else {
			//noModif : lineEdit->setText("");
			currentText = "";
		}
	}
}

void SpreadWidget::returnPressedSlot() {
	/*noModif : if(currentCell){
	 if(nodeTab)
	 spreadNodesTable->setItemText(spreadNodesTable->row(currentCell),spreadNodesTable->column(currentCell),lineEdit->text());
	 else
	 spreadEdgesTable->setItemText(spreadEdgesTable->row(currentCell),spreadEdgesTable->column(currentCell),lineEdit->text());
	 }
	 editingLine=false;*/
}

void SpreadWidget::beginEditLineSlot() {
	editingLine = true;
}

void SpreadWidget::textChangedSlot(const QString &text) {
	currentText = text;
}

void SpreadWidget::tabChanged(int index) {
	assert(index<=1);
	if (index == 0)
		nodeTab = true;
	else
		nodeTab = false;
}

}
