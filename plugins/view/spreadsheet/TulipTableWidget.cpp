#include "TulipTableWidget.h"
#include <tulip/PropertyCreationDialog.h>
#include <tulip/CopyPropertyDialog.h>
#include <tulip/BooleanProperty.h>

#include <QtGui/QHeaderView>
#include <QtGui/QMenu>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QKeyEvent>

#include "GraphTableModel.h"
#include "TulipItemDelegate.h"
#include "HeaderSelectionDialog.h"

using namespace tlp;
using namespace std;
TulipTableWidget::TulipTableWidget(QWidget* parent):QTableView(parent),_graph(NULL),_type(NODE),_tulipTableModel(NULL)
{
    horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(horizontalHeader(),SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showPropertiesContextMenu(QPoint)));
    horizontalHeader()->setMovable(true);
    verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(verticalHeader(),SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showHorizontalHeaderViewContextMenu(QPoint)));
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showTableContextMenu(QPoint)));
}

void TulipTableWidget::setGraph(Graph* graph,ElementType element){
    _graph = graph;
    _type = element;
    _tulipTableModel = new GraphTableModel(graph,element,this);
    QTableView::setModel(_tulipTableModel);
    setItemDelegate(new TulipItemDelegate(this));    
}

void TulipTableWidget::showTableContextMenu(const QPoint& position){
    showElementContextMenu(rowAt(position.y()),position);
}

void TulipTableWidget::showHorizontalHeaderViewContextMenu(const QPoint& position){
    showElementContextMenu(verticalHeader()->logicalIndexAt(position),position);
}

void TulipTableWidget::showElementContextMenu(int clickedRowIndex,const QPoint& position){
    QMenu contextMenu(this);
    //Ensure the clicked index is always selected.
    if(!selectionModel()->isRowSelected(clickedRowIndex,QModelIndex())){
        selectionModel()->select(_tulipTableModel->index(clickedRowIndex,0),QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    }
    QModelIndexList rows = selectionModel()->selectedRows(0);
    set<unsigned int> elements = indexListToIds(rows);
    contextMenu.addAction(areAllElementsSelected(rows)?tr("Deselect on graph"):tr("Select on graph"),this,SLOT(selectElements()));
    contextMenu.addAction(tr("Highlight selected element(s)"),this,SLOT(highlightElements()));
    contextMenu.addAction(QIcon(":/i_del.png"),tr("Delete"),this,SLOT(deleteElements()));
    if(_type == NODE){
        contextMenu.addAction(tr("Copy"),this,SLOT(copyNodes()));
        //Group only available if there is more than one node selected.
        QAction *group = contextMenu.addAction(tr("Group "),this,SLOT(group()));
        group->setEnabled(rows.size()>1);
        QAction *ungroup = contextMenu.addAction(tr("Ungroup "),this,SLOT(ungroup()));
        //If only one node is not a meta node cannot ungroup.
        for(set<unsigned int>::iterator it = elements.begin(); it != elements.end();++it){
            if(!_graph->isMetaNode(node(*it))){
                ungroup->setEnabled(false);
                break;
            }
        }
    }
    contextMenu.exec(mapToGlobal(position));
}

void TulipTableWidget::showPropertiesContextMenu(const QPoint& position){
    QMenu contextMenu(this);
    QHeaderView *headers=horizontalHeader();
    int clickedColumn = headers->logicalIndexAt(position);
    //Properties operations
    contextMenu.addAction(tr("Create new column"),this,SLOT(createNewProperties()));
    contextMenu.addSeparator();
    QAction *hideColumnAction = contextMenu.addAction(tr("Hide column"),this,SLOT(hideColumn()));
    hideColumnAction->setData(QVariant(clickedColumn));
    QAction *setAllValuesAction = contextMenu.addAction(tr("Set all values"),this,SLOT(setAllColumnValues()));
    setAllValuesAction->setData(QVariant(clickedColumn));
    QAction *copyToColumnAction = contextMenu.addAction(tr("Copy to"),this,SLOT(copyColumnToOther()));
    copyToColumnAction->setData(QVariant(clickedColumn));
    QAction *clearColumnAction =contextMenu.addAction(tr("Reset column"),this,SLOT(resetColumn()));
    clearColumnAction->setData(QVariant(clickedColumn));
    QAction *deleteColumnAction =contextMenu.addAction(tr("Delete column"),this,SLOT(deleteColumn()));
    deleteColumnAction->setData(QVariant(clickedColumn));    

    contextMenu.addSeparator();
    //Column visibility and size property
    QMenu * columnsMenu = contextMenu.addMenu(tr("Columns"));


    int count = model()->columnCount();
    for(int i = 0 ; i< count ; ++i){
        QVariant headerName = model()->headerData(i,Qt::Horizontal);
        if( !headers->isSectionHidden(i) || headerName.toString().contains("view")){
            QAction *action = columnsMenu->addAction(headerName.toString());
            action->setData(QVariant(i));
            action->setCheckable(true);
            action->setChecked(!headers->isSectionHidden(i));
            connect(action,SIGNAL(triggered(bool)),this,SLOT(updateHorizontalHeaderVisibility(bool)));
        }
    }
    columnsMenu->addSeparator();
    QAction* othersAction = columnsMenu->addAction(tr("Others..."));
    connect(othersAction,SIGNAL(triggered()),this,SLOT(showPropertiesListDialog()));
    contextMenu.exec(mapToGlobal(position));
}

void TulipTableWidget::showTableContextMenu(){
    QMenu contextMenu(this);

}

void TulipTableWidget::updateHorizontalHeaderVisibility(bool checked){
    QAction* action = qobject_cast<QAction*>(sender());
    if(action!=NULL){
        int index = action->data().toInt();
        horizontalHeader()->setSectionHidden(index,!checked);
    }
}
void TulipTableWidget::showPropertiesListDialog(){
    HeaderSelectionDialog::updateHeaders(horizontalHeader(),this);
}


void TulipTableWidget::createNewProperties(){
    PropertyCreationDialog::createNewProperty(_graph,this);
}

void TulipTableWidget::hideColumn(){
    QAction* action = qobject_cast<QAction*>(sender());
    if(action!=NULL){        
        QHeaderView *headers=horizontalHeader();
        int index = action->data().toInt();
        headers->setSectionHidden(index,true);
    }
}

void TulipTableWidget::setAllColumnValues(){
    QAction* action = qobject_cast<QAction*>(sender());
    if(action!=NULL){
        int index = action->data().toInt();
        QDialog dialog(this);
        dialog.setLayout(new QVBoxLayout(this));
        //Take the value of the first element.
        QWidget *editorWidget = itemDelegate()->createEditor(this,QStyleOptionViewItem(),model()->index(0,index));
        dialog.layout()->addWidget(editorWidget);
        QDialogButtonBox *buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
        connect(buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
        dialog.layout()->addWidget(buttonBox);
        if(dialog.exec()== QDialog::Accepted){
            Observable::holdObservers();
            for(int j = 0 ; j < model()->rowCount() ; ++j){
                itemDelegate()->setModelData(editorWidget,model(),model()->index(j,index));
            }
            Observable::unholdObservers();
        }
    }
}

void TulipTableWidget::copyColumnToOther(){
    QAction* action = qobject_cast<QAction*>(sender());
    if(action!=NULL){
        int index = action->data().toInt();
        PropertyInterface* property = _tulipTableModel->propertyForIndex(index);
        CopyPropertyDialog::copyProperty(_graph,property,true,this);
    }
}

void TulipTableWidget::resetColumn(){
    QAction* action = qobject_cast<QAction*>(sender());
    if(action!=NULL){
        int index = action->data().toInt();
        PropertyInterface* property = _tulipTableModel->propertyForIndex(index);
        if(_type == NODE){
            property->setAllNodeStringValue(property->getNodeDefaultStringValue());
        }else{
            property->setAllEdgeStringValue(property->getEdgeDefaultStringValue());
        }
    }
}

void TulipTableWidget::deleteColumn(){
    QAction* action = qobject_cast<QAction*>(sender());
    if(action!=NULL){        
        int index = action->data().toInt();
        _tulipTableModel->removeColumn(index);
    }
}

//////////////////////////////////////NODES OPERATIONS/////////////////////////////////////////////////


void TulipTableWidget::deleteElements(){
    QAction* action = qobject_cast<QAction*>(sender());
    if(action!=NULL){
        QModelIndexList indexes = selectedIndexes();
        if(indexes.empty()){
            int index = action->data().toInt();
            indexes.push_back(model()->index(index,0));
        }
        doDeleteElements(indexes,false);
    }
}

void TulipTableWidget::deleteAllElements(){
    QAction* action = qobject_cast<QAction*>(sender());
    if(action!=NULL){
        QModelIndexList indexes = selectedIndexes();
        if(indexes.empty()){
            int index = action->data().toInt();
            indexes.push_back(model()->index(index,0));
        }
        doDeleteElements(indexes,true);
    }
}

void TulipTableWidget::doDeleteElements(const QModelIndexList& elements,bool delAll){
    //Get the elements list
    set<unsigned int> elementsList;
    for(QModelIndexList::const_iterator it = elements.begin();it != elements.end();++it){
        elementsList.insert(_tulipTableModel->idForIndex(*it));
    }
    Observable::holdObservers();
    for(set<unsigned int>::iterator it = elementsList.begin(); it != elementsList.end(); ++it){
        if(*it != UINT_MAX){
            if(_type == NODE){
                _graph->delNode(node(*it),delAll);
            }else{
                _graph->delEdge(edge(*it),delAll);
            }
        }
    }
    Observable::unholdObservers();
}

void TulipTableWidget::selectElements(){
    QModelIndexList rows = selectionModel()->selectedRows(0);
    BooleanProperty* selectionProperty = _graph->getProperty<BooleanProperty>("viewSelection");
    set<unsigned int> elements = indexListToIds(rows);
    //If all elements are selected deselect them.
    bool select = !areAllElementsSelected(rows);
    Observable::holdObservers();
    for(set<unsigned int>::iterator it = elements.begin(); it != elements.end();++it){
        if(_type == NODE){
            selectionProperty->setNodeValue(node(*it),select);
        }else{
            selectionProperty->setEdgeValue(edge(*it),select);
        }
    }
    Observable::unholdObservers();
}

void TulipTableWidget::highlightElements(){
    BooleanProperty* selectionProperty = _graph->getProperty<BooleanProperty>("viewSelection");
    QItemSelectionModel *itemSelectionModel = new QItemSelectionModel(_tulipTableModel);
    for(int i = 0 ; i < _tulipTableModel->rowCount() ; ++i){
        if(_type == NODE){
            if(selectionProperty->getNodeValue(node(_tulipTableModel->idForIndex(i)))){
                itemSelectionModel->select(_tulipTableModel->index(i,0),QItemSelectionModel::Select| QItemSelectionModel::Rows);
            }
        }else{
            if(selectionProperty->getEdgeValue(edge(_tulipTableModel->idForIndex(i)))){
                itemSelectionModel->select(_tulipTableModel->index(i,0),QItemSelectionModel::Select| QItemSelectionModel::Rows);
            }
        }
    }
    QItemSelectionModel *oldSelectionModel = selectionModel();
    setSelectionModel(itemSelectionModel);
    oldSelectionModel->deleteLater();
}

bool TulipTableWidget::areAllElementsSelected(const QModelIndexList& elementsIndexes)const{
    BooleanProperty* selectionProperty = _graph->getProperty<BooleanProperty>("viewSelection");
    set<unsigned int> elements = indexListToIds(elementsIndexes);
    bool allSelected = true;
    for(set<unsigned int>::iterator it = elements.begin(); it != elements.end();++it){
        if(_type == NODE){
            if(!selectionProperty->getNodeValue(node(*it))){
                allSelected = false;
                break;
            }
        }else{
            if(!selectionProperty->getEdgeValue(edge(*it))){
                allSelected = false;
                break;
            }
        }
    }
    return allSelected;
}

set<unsigned int> TulipTableWidget::indexListToIds(const QModelIndexList& elementsIndexes)const{
    set<unsigned int> elements;
    for(QModelIndexList::const_iterator it = elementsIndexes.begin();it != elementsIndexes.end();++it){
        elements.insert(_tulipTableModel->idForIndex(*it));
    }
    return elements;
}

void TulipTableWidget::group(){
    QModelIndexList rows = selectionModel()->selectedRows(0);
    set<unsigned int> elements = indexListToIds(rows);
    set<node> nodes;
    for(set<unsigned int>::iterator it = elements.begin(); it != elements.end();++it){
        nodes.insert(node(*it));
    }
    Observable::holdObservers();
    _graph->createMetaNode(nodes);
    Observable::unholdObservers();
}

void TulipTableWidget::ungroup(){
    QModelIndexList rows = selectionModel()->selectedRows(0);
    set<unsigned int> elements = indexListToIds(rows);
    Observable::holdObservers();
    for(set<unsigned int>::iterator it = elements.begin(); it != elements.end();++it){
        _graph->openMetaNode(node(*it));
    }
    Observable::unholdObservers();
}

void TulipTableWidget::copyNodes(){
    QModelIndexList rows = selectionModel()->selectedRows(0);
    set<unsigned int> elements = indexListToIds(rows);
    Observable::holdObservers();
    for(set<unsigned int>::iterator it = elements.begin(); it != elements.end();++it){
        node orig = node(*it);
        node dest = _graph->addNode();
        PropertyInterface* property;
        forEach(property,_graph->getObjectProperties()){
            property->setNodeStringValue(dest,property->getNodeStringValue(orig));
        }
    }
    Observable::unholdObservers();
}



void TulipTableWidget::keyPressedEvent(QKeyEvent * event){
    if(event->modifiers()== Qt::ControlModifier && event->key() == Qt::Key_A){
        QItemSelection selection(_tulipTableModel->index(0,0),_tulipTableModel->index(_tulipTableModel->rowCount(),_tulipTableModel->columnCount()));
        selectionModel()->select(selection,QItemSelectionModel::ClearAndSelect);
        event->accept();
    }
}
