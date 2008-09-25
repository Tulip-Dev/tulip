#include "SpreadWidget.h"

#include <iostream>

#include <tulip/Graph.h>

using namespace std;

namespace tlp {

  SpreadWidget::SpreadWidget(QWidget *parent) :
    currentCell(NULL),editingLine(false),nodeTab(true) {
    setupUi(this);
    spreadNodesTable->setView(SpreadTable::NodesView);
    spreadEdgesTable->setView(SpreadTable::EdgesView);
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
    connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(returnPressedSlot()));
    connect(lineEdit, SIGNAL(widgetActivate()), this, SLOT(beginEditLineSlot()));
    connect(spreadNodesTable, SIGNAL(mouseReleasedSignal(SpreadTable*)), this, SLOT(tableMouseReleasedSlot(SpreadTable*)));
    connect(spreadEdgesTable, SIGNAL(mouseReleasedSignal(SpreadTable*)), this, SLOT(tableMouseReleasedSlot(SpreadTable*)));
    connect(lineEdit, SIGNAL(textChanged (const QString &) ), this, SLOT(textChangedSlot(const QString &)));
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
  }

  void SpreadWidget::setData(Graph *graph,DataSet dataSet) {
    this->graph=graph;
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

  void SpreadWidget::tableMouseReleasedSlot(SpreadTable* table) {
    QString right=currentText.right(1);
    if(editingLine && (right=="(" || right=="+" || right=="-" || right=="*" || right=="/" || right=="=")) {
      QString position;
      QList<QTableWidgetItem *> items=table->selectedItems();
      if(items.size()==1) {
	SpreadTable::encodePosition(position,table->row(items[0]),table->column(items[0]));
	lineEdit->setText(currentText+position);
      }else{
	QList<QTableWidgetSelectionRange> ranges=table->selectedRanges();
	assert(ranges.size()==1);
	SpreadTable::encodeRange(position,ranges[0]);
	lineEdit->setText(currentText+position);
      }
      lineEdit->setFocus(Qt::OtherFocusReason);
    }else{
      currentCell=table->selectedItems()[0];
      lineEdit->setText(currentCell->text());
      currentText=currentCell->text();
    }
  }

  void SpreadWidget::returnPressedSlot() {
    if(currentCell){ 
      if(nodeTab)
	spreadNodesTable->setItemText(spreadNodesTable->row(currentCell),spreadNodesTable->column(currentCell),lineEdit->text());
      else
	spreadEdgesTable->setItemText(spreadEdgesTable->row(currentCell),spreadEdgesTable->column(currentCell),lineEdit->text());
    }
    editingLine=false;
  }

  void SpreadWidget::beginEditLineSlot() {
    editingLine=true;
  }

  void SpreadWidget::textChangedSlot(const QString &text) {
    currentText=text;
  }

  void SpreadWidget::tabChanged(int index) {
    assert(index<=1);
    if(index==0)
      nodeTab=true;
    else
      nodeTab=false;
  }

  void SpreadWidget::addNode(Graph *graph,const node n) {
    spreadNodesTable->addRow(n.id);
  }
  void SpreadWidget::addEdge(Graph *graph,const edge e) {
    spreadEdgesTable->addRow(e.id);
  }
  void SpreadWidget::delNode(Graph *graph,const node n) {
    spreadNodesTable->delRow(n.id);
  }
  void SpreadWidget::delEdge(Graph *graph,const edge e) {
    spreadEdgesTable->delRow(e.id);
  }

}
