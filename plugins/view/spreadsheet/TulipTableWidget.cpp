#include "TulipTableWidget.h"
//#include "headerselectiondialog.h"
#include <tulip/PropertyCreationDialog.h>
#include <tulip/CopyPropertyDialog.h>
#include <QtGui/QHeaderView>
#include <QtGui/QMenu>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
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
    connect(verticalHeader(),SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showElementsContextMenu(QPoint)));
}

void TulipTableWidget::setGraph(Graph* graph,ElementType element){
    _graph = graph;
    _type = element;
    _tulipTableModel = new GraphTableModel(graph,element,this);
    QTableView::setModel(_tulipTableModel);
    setItemDelegate(new TulipItemDelegate(this));    
}

void TulipTableWidget::showElementsContextMenu(const QPoint& position){
    QMenu contextMenu(this);
    QHeaderView *headers=verticalHeader();
    int clickedColumn = headers->logicalIndexAt(position);
    QAction* deleteElements = contextMenu.addAction(tr("Remove element(s) from graph"),this,SLOT(deleteElements()));
    deleteElements->setData(clickedColumn);
    QAction* delAllElements = contextMenu.addAction(tr("Delete element(s)"),this,SLOT(deleteAllElements()));
    delAllElements->setData(clickedColumn);
    contextMenu.exec(mapToGlobal(position));
}

void TulipTableWidget::showPropertiesContextMenu(const QPoint& position){
    QMenu contextMenu(this);
    QHeaderView *headers=horizontalHeader();
    int clickedColumn = headers->logicalIndexAt(position);
    //Properties operations
    contextMenu.addAction(tr("Create new column"),this,SLOT(createNewProperties()));
    contextMenu.addSeparator();
    QAction *hideColumnAction = contextMenu.addAction(tr("Hide column(s)"),this,SLOT(hideColumn()));
    hideColumnAction->setData(QVariant(clickedColumn));
    QAction *setAllValuesAction = contextMenu.addAction(tr("Set all values"),this,SLOT(setAllColumnValues()));
    setAllValuesAction->setData(QVariant(clickedColumn));
    QAction *copyToColumnAction = contextMenu.addAction(tr("Copy to"),this,SLOT(copyColumnToOther()));
    copyToColumnAction->setData(QVariant(clickedColumn));
    QAction *clearColumnAction =contextMenu.addAction(tr("Reset column"),this,SLOT(resetColumn()));
    clearColumnAction->setData(QVariant(clickedColumn));
    QAction *deleteColumnAction =contextMenu.addAction(tr("Delete column(s)"),this,SLOT(deleteColumn()));
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
        QModelIndexList indexes = selectedIndexes();
        QHeaderView *headers=horizontalHeader();
        if(indexes.empty()){
            int index = action->data().toInt();
            headers->setSectionHidden(index,true);
        }else{
            set<int> columns;
            for(QModelIndexList::iterator it = indexes.begin() ; it != indexes.end() ; ++it){
                columns.insert((*it).column());
            }
            for(set<int>::iterator it = columns.begin() ; it != columns.end(); ++it){
                headers->setSectionHidden(*it,true);
            }
        }
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
        QModelIndexList indexes = selectedIndexes();
        if(indexes.empty()){
            int index = action->data().toInt();
            _tulipTableModel->removeColumn(index);
        }else{
            _tulipTableModel->removeColumns(indexes);
        }
    }
}

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
