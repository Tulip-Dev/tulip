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
#include "SpreadView.h"

#include <iostream>
#include <sstream>
#include "ui_SpreadViewWidget.h"
#include <tulip/Graph.h>
#include <tulip/PropertyCreationDialog.h>
#include <tulip/BooleanProperty.h>
#include <QtGui/QPixmap>
#include <QtGui/QMenu>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include "TableViewColumnModel.h"
#include "GraphTableModel.h"
#include "HeaderSelectionDialog.h"
using namespace std;

namespace tlp {



SpreadView::SpreadView() :
    AbstractView(),ui(new Ui::SpreadViewWidget),_graph(NULL),_nodesColumnsModel(NULL),_edgesColumnsModel(NULL){
}
SpreadView::~SpreadView() {
    delete ui;
}

QWidget *SpreadView::construct(QWidget *parent) {
    QWidget *widget = AbstractView::construct(parent);
    QWidget* mainWidget = new QWidget(widget);
    ui->setupUi(mainWidget);
    setCentralWidget(mainWidget);
    installEventFilter(mainWidget);
    installEventFilter(ui->nodesTableView);

    //Nodes table
    ui->nodesTableView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->nodesTableView->horizontalHeader(),SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showPropertiesContextMenu(QPoint)));
    ui->nodesTableView->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->nodesTableView->verticalHeader(),SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showElementsContextMenu(QPoint)));
    ui->nodesTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->nodesTableView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showTableContextMenu(QPoint)));

    //Edges table
    ui->edgesTableView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->edgesTableView->horizontalHeader(),SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showPropertiesContextMenu(QPoint)));
    ui->edgesTableView->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->edgesTableView->verticalHeader(),SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showElementsContextMenu(QPoint)));
    ui->edgesTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->edgesTableView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showTableContextMenu(QPoint)));

    //Column selection widget
    connect(ui->editColumnPushButton,SIGNAL(clicked( bool )),this,SLOT(showEditColumnDialog()));

    return widget;
}

void SpreadView::setData(Graph *graph, DataSet) {
    if(graph!= _graph){
        _graph = graph;
        ui->nodesTableView->setGraph(graph,NODE);
        ui->edgesTableView->setGraph(graph,EDGE);

        TableViewColumnModel *oldNodeColumnModel = _nodesColumnsModel;
        TableViewColumnModel *oldEdgeColumnModel = _edgesColumnsModel;

        _nodesColumnsModel = new TableViewColumnModel(ui->nodesTableView,this);
        _edgesColumnsModel = new TableViewColumnModel(ui->edgesTableView,this);

        if(oldNodeColumnModel){
            oldNodeColumnModel->deleteLater();
        }
        if(oldEdgeColumnModel){
            oldEdgeColumnModel->deleteLater();
        }
    }
}

void SpreadView::getData(Graph **graph, DataSet *) {
    *graph = _graph;
}

Graph* SpreadView::getGraph() {
    return _graph;
}

void SpreadView::setGraph(Graph *graph) {
    setData(graph,DataSet());
}

void SpreadView::draw() {
}

void SpreadView::refresh() {
}

void SpreadView::init() {
}


QImage SpreadView::createPicture(int width, int height, bool , int  , int xOffset , int yOffset ){
    return QPixmap::grabWidget(getCentralWidget(),xOffset,yOffset,width,height).toImage();
}

TulipTableWidget* SpreadView::currentTable()const{
    return ui->tabWidget->currentWidget()==ui->nodeTab?ui->nodesTableView:ui->edgesTableView;
}

void SpreadView::showElementsContextMenu(TulipTableWidget* tableWidget,int clickedRowIndex,const QPoint& position){
    QMenu contextMenu(tableWidget);
    fillElementsContextMenu(contextMenu,tableWidget,clickedRowIndex);
    contextMenu.exec(tableWidget->mapToGlobal(position));
}
void SpreadView::fillElementsContextMenu(QMenu& menu,TulipTableWidget* tableWidget,int clickedRowIndex){
    QAbstractItemModel* tulipTableModel = tableWidget->model();
    //Ensure the clicked index is always selected.
    if(!tableWidget->selectionModel()->isRowSelected(clickedRowIndex,QModelIndex())){
        tableWidget->selectionModel()->select(tulipTableModel->index(clickedRowIndex,0),QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    }
    QModelIndexList rows = tableWidget->selectionModel()->selectedRows(0);
    set<unsigned int> elements = tableWidget->indexListToIds(rows);
    menu.addAction(tableWidget->areAllElementsSelected(rows)?tr("Deselect on graph"):tr("Select on graph"),this,SLOT(selectElements()));
    menu.addAction(tr("Highlight selected element(s)"),this,SLOT(highlightElements()));

    if(tableWidget->elementType() == NODE){

        menu.addAction(tr("Copy"),this,SLOT(copyNodes()));

        //Group only available if there is more than one node selected.
        QAction *group = menu.addAction(tr("Group "),this,SLOT(group()));
        group->setEnabled(rows.size()>1);

        QAction *ungroup = menu.addAction(tr("Ungroup "),this,SLOT(ungroup()));
        //If only one node is not a meta node cannot ungroup.
        for(set<unsigned int>::iterator it = elements.begin(); it != elements.end();++it){
            if(!_graph->isMetaNode(node(*it))){
                ungroup->setEnabled(false);
                break;
            }
        }
    }

    menu.addAction(QIcon(":/i_del.png"),tr("Delete"),this,SLOT(deleteElements()));
}


void SpreadView::showPropertiesContextMenu(TulipTableWidget* tableWidget,int clickedColumn,const QPoint& position){
    QMenu contextMenu(tableWidget);
    fillPropertiesContextMenu(contextMenu,tableWidget,clickedColumn);
    contextMenu.exec(tableWidget->mapToGlobal(position));
}
void SpreadView::fillPropertiesContextMenu(QMenu& menu,TulipTableWidget*,int clickedColumn){
    //Properties operations
    QAction *hideColumnAction = menu.addAction(tr("Hide column"),this,SLOT(hideColumn()));    
    hideColumnAction->setData(QVariant(clickedColumn));

    menu.addAction(tr("Create new column"),this,SLOT(createNewProperties()));

    QAction *copyToColumnAction = menu.addAction(tr("Copy to"),this,SLOT(copyColumnToOther()));    
    copyToColumnAction->setData(QVariant(clickedColumn));

    QAction *setAllValuesAction = menu.addAction(tr("Set all values"),this,SLOT(setAllColumnValues()));
    setAllValuesAction->setData(QVariant(clickedColumn));

    QAction *clearColumnAction =menu.addAction(tr("Reset column"),this,SLOT(resetColumn()));
    clearColumnAction->setData(QVariant(clickedColumn));

    QAction *deleteColumnAction =menu.addAction(tr("Delete column"),this,SLOT(deleteColumn()));
    deleteColumnAction->setData(QVariant(clickedColumn));
}

void SpreadView::showElementsContextMenu(const QPoint& position){
    QHeaderView* headerView = qobject_cast<QHeaderView*>(sender());
    if(ui->nodesTableView->verticalHeader() == headerView){
        showElementsContextMenu(ui->nodesTableView,headerView->logicalIndexAt(position),position);
    }else{
        showElementsContextMenu(ui->edgesTableView,headerView->logicalIndexAt(position),position);
    }

}

void SpreadView::showPropertiesContextMenu(const QPoint& position){
    QHeaderView* headerView = qobject_cast<QHeaderView*>(sender());
    if(ui->nodesTableView->horizontalHeader() == headerView){
        showPropertiesContextMenu(ui->nodesTableView,headerView->logicalIndexAt(position),position);
    }else{
        showPropertiesContextMenu(ui->edgesTableView,headerView->logicalIndexAt(position),position);
    }
}

void SpreadView::showTableContextMenu(const QPoint& position){
    TulipTableWidget* tableWidget = qobject_cast<TulipTableWidget*>(sender());
    QMenu contextMenu(tableWidget);
    fillElementsContextMenu(contextMenu,tableWidget,tableWidget->indexAt(position).row());
    contextMenu.exec(tableWidget->mapToGlobal(position));
}


void SpreadView::showEditColumnDialog(){
    QTableView *tableView = (ui->tabWidget->currentWidget() == ui->nodeTab )? ui->nodesTableView : ui->edgesTableView;
    QString title = (ui->tabWidget->currentWidget() == ui->nodeTab )? tr("Nodes table visible columns") : tr("Edges table visible columns");
    HeaderSelectionDialog::updateHeaders(tableView,title,getCentralWidget());
}


void SpreadView::hideColumn(){
    QAction* action = qobject_cast<QAction*>(sender());
    if(action!=NULL){
        QHeaderView *headers=currentTable()->horizontalHeader();
        int index = action->data().toInt();
        headers->setSectionHidden(index,true);
    }
}

void SpreadView::createNewProperties(){
    PropertyCreationDialog::createNewProperty(_graph,currentTable());
}

void SpreadView::copyColumnToOther(){

}

void SpreadView::setAllColumnValues(){
    QAction* action = qobject_cast<QAction*>(sender());
    if(action!=NULL){
        int index = action->data().toInt();
        TulipTableWidget* tableWidget = currentTable();
        GraphTableModel* model = tableWidget->graphModel();
        QDialog dialog(tableWidget);
        dialog.setLayout(new QVBoxLayout(&dialog));
        //Take the value of the first element.
        QWidget *editorWidget = tableWidget->itemDelegate()->createEditor(tableWidget,QStyleOptionViewItem(),model->index(0,index,QModelIndex()));
        dialog.layout()->addWidget(editorWidget);
        QDialogButtonBox *buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
        connect(buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
        dialog.layout()->addWidget(buttonBox);
        if(dialog.exec()== QDialog::Accepted){
            Observable::holdObservers();
            for(int j = 0 ; j < model->rowCount() ; ++j){
                tableWidget->itemDelegate()->setModelData(editorWidget,model,model->index(j,index));
            }
            Observable::unholdObservers();
        }
    }
}

void SpreadView::resetColumn(){
    QAction* action = qobject_cast<QAction*>(sender());
    if(action!=NULL){
        int index = action->data().toInt();
        TulipTableWidget* tableWidget = currentTable();
        GraphTableModel* model = tableWidget->graphModel();
        PropertyInterface* property = model->propertyForIndex(index);
        if(model->elementType() == NODE){
            property->setAllNodeStringValue(property->getNodeDefaultStringValue());
        }else{
            property->setAllEdgeStringValue(property->getEdgeDefaultStringValue());
        }
    }
}

void SpreadView::deleteColumn(){
    QAction* action = qobject_cast<QAction*>(sender());
    if(action!=NULL){
        int index = action->data().toInt();
        GraphTableModel* model = currentTable()->graphModel();
        PropertyInterface* property = model->propertyForIndex(index);
        string propertyName = property->getName();
        property->getGraph()->delLocalProperty(propertyName);
    }
}

void SpreadView::selectElements(){
    TulipTableWidget *tableWidget = currentTable();
    QModelIndexList rows = tableWidget->selectionModel()->selectedRows(0);
    BooleanProperty* selectionProperty = _graph->getProperty<BooleanProperty>("viewSelection");
    set<unsigned int> elements = tableWidget->indexListToIds(rows);
    //If all elements are selected deselect them.
    bool select = !tableWidget->areAllElementsSelected(rows);
    Observable::holdObservers();
    for(set<unsigned int>::iterator it = elements.begin(); it != elements.end();++it){
        if(tableWidget->elementType() == NODE){
            selectionProperty->setNodeValue(node(*it),select);
        }else{
            selectionProperty->setEdgeValue(edge(*it),select);
        }
    }
    Observable::unholdObservers();
}

void SpreadView::highlightElements(){
    TulipTableWidget *tableWidget = currentTable();
    BooleanProperty* selectionProperty = _graph->getProperty<BooleanProperty>("viewSelection");
    set<unsigned int> ids;
    if(tableWidget->elementType()==NODE){
        node n;
        forEach(n,selectionProperty->getNodesEqualTo(true,_graph)){
            ids.insert(n.id);
        }
    }else{
        edge e;
        forEach(e,selectionProperty->getEdgesEqualTo(true,_graph)){
            ids.insert(e.id);
        }
    }
    tableWidget->highlightAndDisplayElements(ids);
}

void SpreadView::deleteElements(){
    QAction* action = qobject_cast<QAction*>(sender());
    if(action!=NULL){
        TulipTableWidget *tableWidget = currentTable();
        QModelIndexList indexes = tableWidget->selectionModel()->selectedRows(0);
        deleteElements(indexes,tableWidget,false);
    }
}

void SpreadView::deleteElements(const QModelIndexList& elements,TulipTableWidget *tableWidget ,bool delAll){
    //Get the elements list
    set<unsigned int> elementsList = tableWidget->indexListToIds(elements);
    Observable::holdObservers();
    for(set<unsigned int>::iterator it = elementsList.begin(); it != elementsList.end(); ++it){
        if(*it != UINT_MAX){
            if(tableWidget->elementType() == NODE){
                _graph->delNode(node(*it),delAll);
            }else{
                _graph->delEdge(edge(*it),delAll);
            }
        }
    }
    Observable::unholdObservers();
}

void SpreadView::copyNodes(){
    TulipTableWidget *tableWidget = currentTable();
    QModelIndexList rows = tableWidget->selectionModel()->selectedRows(0);
    set<unsigned int> elements = tableWidget->indexListToIds(rows);
    set<unsigned int> createdElements;
    Observable::holdObservers();
    for(set<unsigned int>::iterator it = elements.begin(); it != elements.end();++it){
        node orig = node(*it);
        node dest = _graph->addNode();
        createdElements.insert(dest.id);
        PropertyInterface* property;
        forEach(property,_graph->getObjectProperties()){
            property->setNodeStringValue(dest,property->getNodeStringValue(orig));
        }
    }
    Observable::unholdObservers();
    tableWidget->highlightAndDisplayElements(createdElements);
}

void SpreadView::group(){
    TulipTableWidget *tableWidget = currentTable();
    QModelIndexList rows = tableWidget->selectionModel()->selectedRows(0);
    set<unsigned int> elements = tableWidget->indexListToIds(rows);
    set<node> nodes;
    for(set<unsigned int>::iterator it = elements.begin(); it != elements.end();++it){
        nodes.insert(node(*it));
    }
    Observable::holdObservers();
    node createdMetanode = _graph->createMetaNode(nodes);
    Observable::unholdObservers();
    set<unsigned int> metanodeId;
    metanodeId.insert(createdMetanode.id);
    tableWidget->highlightAndDisplayElements(metanodeId);
}

void SpreadView::ungroup(){
    TulipTableWidget *tableWidget = currentTable();
    QModelIndexList rows = tableWidget->selectionModel()->selectedRows(0);
    set<unsigned int> elements = tableWidget->indexListToIds(rows);
    set<unsigned int> metanodeNodes;
    set<unsigned int> metanodeEdges;
    Observable::holdObservers();
    for(set<unsigned int>::iterator it = elements.begin(); it != elements.end();++it){
        Graph* metanodeGraph = _graph->getNodeMetaInfo(node(*it));
        node n;
        forEach(n,metanodeGraph->getNodes()){
            metanodeNodes.insert(n.id);
        }
        edge e;
        forEach(e,metanodeGraph->getEdges()){
            metanodeEdges.insert(e.id);
        }
        _graph->openMetaNode(node(*it));
    }
    Observable::unholdObservers();
    ui->nodesTableView->highlightAndDisplayElements(metanodeNodes);
    ui->edgesTableView->highlightAndDisplayElements(metanodeEdges);
}




VIEWPLUGIN(SpreadView, "Table view", "Tulip Team", "16/04/2008", "Spreadsheet view", "2.0")

}


