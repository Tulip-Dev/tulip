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
#include "GraphTableModel.h"
#include <tulip/TlpQtTools.h>
using namespace std;

namespace tlp {



SpreadView::SpreadView() :
    AbstractView(),ui(new Ui::SpreadViewWidget),_graph(NULL),_selectionProperty(NULL),_reloadSelectionProperty(false){
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

    //Column selection widgets
    ui->nodesTableColumnEditionWidget->setVisible(false);
    ui->nodesTableColumnEditionWidget->setEnabled(false);
    ui->edgesTableColumnEditionWidget->setVisible(false);
    ui->edgesTableColumnEditionWidget->setEnabled(false);

    connect(ui->showOnlySelectedElementsCheckBox,SIGNAL(stateChanged(int)),this,SLOT(updateElementVisibility(int)));
    connect(ui->filterPatternLineEdit,SIGNAL(returnPressed()),this,SLOT(filterElements()));
    connect(ui->filterPushButton,SIGNAL(clicked()),this,SLOT(filterElements()));

    return widget;
}

void SpreadView::setData(Graph *graph, DataSet) {
    if(graph!= _graph){
        _graph = graph;
        ui->nodesTableView->setGraph(graph,NODE);
        ui->edgesTableView->setGraph(graph,EDGE);

        ui->nodesTableColumnEditionWidget->setTableView(ui->nodesTableView);
        ui->nodesTableColumnEditionWidget->setEnabled(true);
        ui->edgesTableColumnEditionWidget->setTableView(ui->edgesTableView);
        ui->edgesTableColumnEditionWidget->setEnabled(true);

        //Connect data modification events to ensure filtering il always up to date.
        connect(ui->nodesTableView->graphModel(),SIGNAL(columnsAboutToBeInserted ( QModelIndex , int , int)),this,SLOT(columnsInserted(QModelIndex,int,int)));
        connect(ui->nodesTableView->graphModel(),SIGNAL(columnsAboutToBeRemoved(QModelIndex,int,int)),this,SLOT(rowsInserted(QModelIndex,int,int)));
        connect(ui->nodesTableView->graphModel(),SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)),this,SLOT(rowsInserted(QModelIndex,int,int)));
        connect(ui->nodesTableView->graphModel(),SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(dataChanged(QModelIndex,QModelIndex)));
        connect(ui->nodesTableView->graphModel(),SIGNAL(modelReset()),this,SLOT(modelReset()));

        connect(ui->edgesTableView->graphModel(),SIGNAL(columnsAboutToBeInserted ( QModelIndex , int , int)),this,SLOT(columnsInserted(QModelIndex,int,int)));
        connect(ui->edgesTableView->graphModel(),SIGNAL(columnsAboutToBeRemoved(QModelIndex,int,int)),this,SLOT(rowsInserted(QModelIndex,int,int)));
        connect(ui->edgesTableView->graphModel(),SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)),this,SLOT(rowsInserted(QModelIndex,int,int)));
        connect(ui->edgesTableView->graphModel(),SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(dataChanged(QModelIndex,QModelIndex)));
        connect(ui->edgesTableView->graphModel(),SIGNAL(modelReset()),this,SLOT(modelReset()));

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

GraphTableWidget* SpreadView::currentTable()const{
    return ui->tabWidget->currentWidget()==ui->nodeTab?ui->nodesTableView:ui->edgesTableView;
}

void SpreadView::showElementsContextMenu(GraphTableWidget* tableWidget,int clickedRowIndex,const QPoint& position){
    QMenu contextMenu(tableWidget);
    fillElementsContextMenu(contextMenu,tableWidget,clickedRowIndex);
    contextMenu.exec(tableWidget->mapToGlobal(position));
}
void SpreadView::fillElementsContextMenu(QMenu& menu,GraphTableWidget* tableWidget,int clickedRowIndex){
    QAbstractItemModel* tulipTableModel = tableWidget->model();
    //Ensure the clicked index is always selected.
    if(!tableWidget->selectionModel()->isRowSelected(clickedRowIndex,QModelIndex())){
        tableWidget->selectionModel()->select(tulipTableModel->index(clickedRowIndex,0),QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    }
    QModelIndexList rows = tableWidget->selectionModel()->selectedRows(0);
    set<unsigned int> elements = tableWidget->indexListToIds(rows);    
    QAction* selectOnGraph = menu.addAction(tr("Select on graph"),this,SLOT(selectElements()));
    selectOnGraph->setToolTip(tr("Replace the current graph selection by the elements highlighted in the table."));
    GraphTableWidget::SelectionStatus status = tableWidget->selectionStatus(rows);
    QAction* addToSelection = menu.addAction(tr("Add to graph selection"),this,SLOT(addToSelection()));
    addToSelection->setToolTip(tr("Add the elements highlighted in the table to the graph selection."));
    QAction* removeFromSelection = menu.addAction(tr("Remove from graph selection"),this,SLOT(removeFromSelection()));
    removeFromSelection->setToolTip(tr("Remove the elements highlighted in the table from the graph selection."));
    if(status == GraphTableWidget::Selected){
        addToSelection->setEnabled(false);
    }else if(status == GraphTableWidget::Unselected){
        removeFromSelection->setEnabled(false);
    }
    menu.addAction(tr("Highlight graph selection in the table"),this,SLOT(highlightElements()));

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


void SpreadView::showPropertiesContextMenu(GraphTableWidget* tableWidget,int clickedColumn,const QPoint& position){
    QMenu contextMenu(tableWidget);
    fillPropertiesContextMenu(contextMenu,tableWidget,clickedColumn);
    contextMenu.exec(tableWidget->mapToGlobal(position));
}
void SpreadView::fillPropertiesContextMenu(QMenu& menu,GraphTableWidget* tulipTableWidget,int clickedColumn){
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
    //Avoid to delete inherited properties
    if(tulipTableWidget->graphModel()->propertyForIndex(clickedColumn)->getGraph() != _graph){
        deleteColumnAction->setEnabled(false);
    }
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
    GraphTableWidget* tableWidget = qobject_cast<GraphTableWidget*>(sender());
    QMenu contextMenu(tableWidget);
    fillElementsContextMenu(contextMenu,tableWidget,tableWidget->indexAt(position).row());
    contextMenu.exec(tableWidget->mapToGlobal(position));
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
        GraphTableWidget* tableWidget = currentTable();
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
                if(!tableWidget->isRowHidden(j)){
                    tableWidget->itemDelegate()->setModelData(editorWidget,model,model->index(j,index));
                }
            }
            Observable::unholdObservers();
        }
    }
}

void SpreadView::resetColumn(){
    QAction* action = qobject_cast<QAction*>(sender());
    if(action!=NULL){
        int index = action->data().toInt();
        GraphTableWidget* tableWidget = currentTable();
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
        Observable::holdObservers();
        property->getGraph()->delLocalProperty(propertyName);
        Observable::unholdObservers();
    }
}

void SpreadView::selectElements(){
    GraphTableWidget *tableWidget = currentTable();    
    updateSelectionForHighlightedElements(tableWidget,true,true);
}

void SpreadView::addToSelection(){
    GraphTableWidget *tableWidget = currentTable();
    updateSelectionForHighlightedElements(tableWidget,true);
}

void SpreadView::removeFromSelection(){
    GraphTableWidget *tableWidget = currentTable();
    updateSelectionForHighlightedElements(tableWidget,false);
}

void SpreadView::updateSelectionForHighlightedElements(GraphTableWidget* tableWidget,bool select,bool clearOldSelection){
    BooleanProperty* selectionProperty = _graph->getProperty<BooleanProperty>("viewSelection");
    set<unsigned int> elements = tableWidget->indexListToIds(tableWidget->selectionModel()->selectedRows(0));
    Observable::holdObservers();
    if(clearOldSelection){
        if(tableWidget->elementType() == NODE){
            selectionProperty->setAllNodeValue(false);
        }else{
            selectionProperty->setAllEdgeValue(false);
        }
    }
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
    GraphTableWidget *tableWidget = currentTable();
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
        GraphTableWidget *tableWidget = currentTable();
        QModelIndexList indexes = tableWidget->selectionModel()->selectedRows(0);
        deleteElements(indexes,tableWidget,false);
    }
}

void SpreadView::deleteElements(const QModelIndexList& elements,GraphTableWidget *tableWidget ,bool delAll){
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
    GraphTableWidget *tableWidget = currentTable();
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
    GraphTableWidget *tableWidget = currentTable();
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
    GraphTableWidget *tableWidget = currentTable();
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

void SpreadView::updateElementVisibility(int state){
    showOnlySelectedElements(state == Qt::Checked);
}

void SpreadView::showOnlySelectedElements(bool show){
    assert(_graph != NULL);    
    if(show){
        //Listen for property creation destruction.
        _graph->addListener(this);
        _graph->addObserver(this);
        if(_graph->existProperty("viewSelection")){
            _selectionProperty = _graph->getProperty<BooleanProperty>("viewSelection");
            _selectionProperty->addObserver(this);
            _selectionProperty->addListener(this);
        }
    }else{
        //No need to listen graph anymore.
        _graph->removeListener(this);
        _graph->removeObserver(this);
        if(_selectionProperty != NULL){
            _selectionProperty->removeListener(this);
            _selectionProperty->removeObserver(this);
            _selectionProperty = NULL;
        }
    }
    //Need to update filter.
    _updatedNodes.setAll(true);
    _updatedEdges.setAll(true);
    updateFilters();
}

void SpreadView::updateFilters(){    
    GraphTableModel* nodesGraphModel = ui->nodesTableView->graphModel();
    bool displayOnlySelected = displayOnlySelectedElements();
    QRegExp regExp = elementValuesFilter();
    for(int i= 0; i < nodesGraphModel->rowCount(); ++i){
        node n(nodesGraphModel->idForIndex(i));
        if(_updatedNodes.get(n.id)){
            bool display = true;
            if(displayOnlySelected){
                display = _selectionProperty->getNodeValue(n);
            }
            bool match = false;
            if(regExp.isEmpty() ){
                match = true;
            }else{
                for(int j=0;j< nodesGraphModel->columnCount() ; ++j){
                    //If the column is visible.
                    if(!ui->nodesTableView->isColumnHidden(i)){
                        match |= regExp.exactMatch(nodesGraphModel->data(nodesGraphModel->index(i,j)).toString());
                    }
                }
            }
            ui->nodesTableView->setRowHidden(i,!(display && match));
        }
    }
    GraphTableModel* edgesGraphModel = ui->edgesTableView->graphModel();
    for(int i= 0; i < edgesGraphModel->rowCount(); ++i){
        edge e(edgesGraphModel->idForIndex(i));
        if(_updatedEdges.get(e.id)){
            bool display = true;
            if(displayOnlySelected){
                display = _selectionProperty->getEdgeValue(e);
            }
            bool match = false;
            if(regExp.isEmpty() ){
                match = true;
            }else{
                for(int j=0;j< edgesGraphModel->columnCount() ; ++j){
                    //If the column is visible.
                    if(!ui->edgesTableView->isColumnHidden(i)){
                        match |= regExp.exactMatch(edgesGraphModel->data(edgesGraphModel->index(i,j)).toString());
                    }
                }
            }
            ui->edgesTableView->setRowHidden(i,!(display && match));
        }
    }
    _updatedNodes.setAll(false);
    _updatedEdges.setAll(false);
}

void SpreadView::treatEvent(const Event &ev){
    const PropertyEvent* propEvt = dynamic_cast<const PropertyEvent*>(&ev);
    if (propEvt) {
        switch(propEvt->getType()) {
        case PropertyEvent::TLP_AFTER_SET_NODE_VALUE:
            _updatedNodes.set(propEvt->getNode(),true);
            break;
        case PropertyEvent::TLP_AFTER_SET_ALL_NODE_VALUE:
            _updatedNodes.setAll(true);
            break;
        case PropertyEvent::TLP_AFTER_SET_ALL_EDGE_VALUE:
            _updatedEdges.setAll(true);
            break;
        case PropertyEvent::TLP_AFTER_SET_EDGE_VALUE:
            _updatedEdges.set(propEvt->getEdge(),true);
            break;
        default:
            break;
        }
    } else {
        const GraphEvent* graphEvt = dynamic_cast<const GraphEvent*>(&ev);
        if(graphEvt){
            switch(graphEvt->getType()) {
            case GraphEvent::TLP_ADD_LOCAL_PROPERTY:
            case GraphEvent::TLP_ADD_INHERITED_PROPERTY:
            case GraphEvent::TLP_BEFORE_DEL_INHERITED_PROPERTY:
            case GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY:{
                string propertyName = graphEvt->getPropertyName();
                //A new selection property was created or the current one will be destructed.
                if(propertyName.compare("viewSelection")==0){
                    if(_selectionProperty != NULL){
                        _selectionProperty->removeListener(this);
                        _selectionProperty->removeObserver(this);
                        _selectionProperty = NULL;
                    }
                    _reloadSelectionProperty = true;
                }
            }
                break;
            default:
                break;
            }
        }
    }
}

void SpreadView::treatEvents(const  std::vector<Event> &){
    //Need to reload selection property
    if (_reloadSelectionProperty) {
        assert(_selectionProperty == NULL);
        if(_graph->existProperty("viewSelection")){
            _selectionProperty = _graph->getProperty<BooleanProperty>("viewSelection");
            _selectionProperty->addObserver(this);
            _selectionProperty->addPropertyObserver(this);
            _updatedNodes.setAll(true);
            _updatedEdges.setAll(true);
        }
        _reloadSelectionProperty = false;
    }
    updateFilters();
}
void SpreadView::filterElements(){
    filterElements(ui->filterPatternLineEdit->text());
}

void SpreadView::filterElements(const QString&){
    _updatedNodes.setAll(true);
    _updatedEdges.setAll(true);
    updateFilters();
}

bool SpreadView::displayOnlySelectedElements()const{
    return ui->showOnlySelectedElementsCheckBox->checkState()==Qt::Checked && _selectionProperty != NULL;
}

QRegExp SpreadView::elementValuesFilter()const{
    return QRegExp(ui->filterPatternLineEdit->text());
}

void SpreadView::columnsInserted(const QModelIndex & , int , int ){
    //When adding a column we need to sort all the elements
    modelReset();
}

void SpreadView::columnsDeleted(const QModelIndex &, int , int ){
    //When deleting a column we need to sort all the elements
    modelReset();
}

void SpreadView::rowsInserted(const QModelIndex &, int first, int last){
    GraphTableModel* model = static_cast<GraphTableModel*>(sender());
    if(model == ui->nodesTableView->graphModel()){
        for(int i = first ; i <= last ; ++i){
            _updatedNodes.set(model->idForIndex(i),true);
        }
    }else if(model == ui->edgesTableView->graphModel()){
        for(int i = first ; i <= last ; ++i){
            _updatedEdges.set(model->idForIndex(i),true);
        }
    }
    updateFilters();
}

void SpreadView::modelReset(){
    GraphTableModel* model = static_cast<GraphTableModel*>(sender());
    if(model == ui->nodesTableView->graphModel()){
        _updatedNodes.setAll(true);
    }else if(model == ui->edgesTableView->graphModel()){
        _updatedEdges.setAll(true);
    }
    updateFilters();
}
void SpreadView::dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight){
    GraphTableModel* model = static_cast<GraphTableModel*>(sender());    
    if(model == ui->nodesTableView->graphModel()){
        for(int i = topLeft.row() ; i <= bottomRight.row() ; ++i){
            _updatedNodes.set(model->idForIndex(i),true);
        }
    }else if(model == ui->edgesTableView->graphModel()){
        for(int i = topLeft.row() ; i <= bottomRight.row() ; ++i){
            _updatedEdges.set(model->idForIndex(i),true);
        }
    }
    updateFilters();
}

VIEWPLUGIN(SpreadView, "Table view", "Tulip Team", "07/06/2011", "Spreadsheet view", "2.0")
}


