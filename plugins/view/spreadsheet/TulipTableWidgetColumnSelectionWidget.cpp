#include "TulipTableWidgetColumnSelectionWidget.h"
#include "ui_TulipTableWidgetColumnSelectionWidget.h"
#include "TulipTableWidgetColumnSelectionModel.h"
#include "GraphTableWidget.h"
#include "GraphTableModel.h"
#include <QtGui/QMenu>
#include <QtGui/QStyledItemDelegate>
#include <tulip/PropertyCreationDialog.h>
#include <cassert>
#include <string>


class TulipTableWidgetColumnSelectionWidgetItemDelegate : public QStyledItemDelegate{
public :
    TulipTableWidgetColumnSelectionWidgetItemDelegate(QObject * parent = 0) : QStyledItemDelegate(parent){
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{
        QStyleOptionViewItemV4 opt = option;
        initStyleOption(&opt, index);
        opt.decorationPosition= QStyleOptionViewItem::Right;
        const QWidget *widget = NULL;
        if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option)){
            widget = v3->widget;
        }
        QStyle *style = widget ? widget->style() : QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
    }
};

TulipTableWidgetColumnSelectionWidget::TulipTableWidgetColumnSelectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableColumnSelectionWidget),_tableColumnModel(NULL)
{
    ui->setupUi(this);    
    connect(ui->checkUncheckAllCheckBox,SIGNAL(stateChanged(int)),this,SLOT(checkUncheckAllStateChanged(int)));
    //Predefined filters
    connect(ui->dataPropertiesPushButton,SIGNAL(clicked()),this,SLOT(showDataProperties()));
    connect(ui->visualPropertiesPushButton,SIGNAL(clicked()),this,SLOT(showVisualProperties()));

    //User pattern
    connect(ui->customPatternLineEdit,SIGNAL(textChanged( QString)),this,SLOT(showCustomPatternProperties(QString)));

    ui->listView->setItemDelegate(new TulipTableWidgetColumnSelectionWidgetItemDelegate(ui->listView));

    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showContextMenu(QPoint)));

}

TulipTableWidgetColumnSelectionWidget::~TulipTableWidgetColumnSelectionWidget()
{
    delete ui;
}

void TulipTableWidgetColumnSelectionWidget::setTableView(GraphTableWidget* tableView){
    assert(tableView != NULL);
    QAbstractItemModel* oldModel = ui->listView->model();
    if(oldModel){
        disconnect(oldModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(updateCheckUncheckAllButtonState()));
    }
    _tableColumnModel = new TulipTableWidgetColumnSelectionModel(tableView,this);
    ui->listView->setModel(_tableColumnModel);
    updateCheckUncheckAllButtonState();
    connect(_tableColumnModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(updateCheckUncheckAllButtonState()));
    if(oldModel != NULL){
        oldModel->deleteLater();
    }
}

void TulipTableWidgetColumnSelectionWidget::showVisualProperties(){
    selectColumns(QRegExp("^view"),true);
}

void TulipTableWidgetColumnSelectionWidget::showDataProperties(){
    selectColumns(QRegExp("^view"),false);
}

void TulipTableWidgetColumnSelectionWidget::showCustomPatternProperties(const QString& pattern){
    selectColumns(QRegExp(pattern),true);
}

void TulipTableWidgetColumnSelectionWidget::selectColumns(const QRegExp& regExp,bool match){
    assert(_tableColumnModel != NULL);
    for(int i = 0 ; i < _tableColumnModel->rowCount() ; ++i){        
        _tableColumnModel->setColumnVisible(i,match == (regExp.indexIn(_tableColumnModel->data(_tableColumnModel->index(i)).toString())!= -1));
    }
}

void TulipTableWidgetColumnSelectionWidget::checkUncheckAllStateChanged(int state){
    bool showColumn = state == Qt::Unchecked?false:true;
    for(int i = 0 ; i < _tableColumnModel->rowCount() ; ++i){
        _tableColumnModel->setColumnVisible(i,showColumn);
    }
}

void TulipTableWidgetColumnSelectionWidget::updateCheckUncheckAllButtonState(){
    Qt::CheckState state=Qt::Checked;
    for(int i = 0 ; i < _tableColumnModel->rowCount() ; ++i){
        Qt::CheckState columnCheckState = static_cast<Qt::CheckState>(_tableColumnModel->data(_tableColumnModel->index(i),Qt::CheckStateRole).toInt());
        if(i==0 ){
            //If the first element is not checked so uncheck it.
            if(columnCheckState == Qt::Unchecked){
                state = Qt::Unchecked;
            }
        }else{
            if(columnCheckState != state){
                state = Qt::PartiallyChecked;
                break;
            }
        }
    }
    ui->checkUncheckAllCheckBox->blockSignals(true);
    ui->checkUncheckAllCheckBox->setCheckState(state);
    ui->checkUncheckAllCheckBox->blockSignals(false);
}


void TulipTableWidgetColumnSelectionWidget::showContextMenu(const QPoint& position){

    QMenu menu(ui->listView);
    QModelIndex index = ui->listView->indexAt(position);

    //Ensure the clicked index is always selected.
    if(!ui->listView->selectionModel()->isRowSelected(index.row(),QModelIndex())){
        ui->listView->selectionModel()->select(_tableColumnModel->index(index.row(),0),QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    }
    QModelIndexList rows = ui->listView->selectionModel()->selectedRows(0);

    //Properties operations
    menu.addAction(tr("Show column(s)"),this,SLOT(showSelectedColumns()));
    menu.addAction(tr("Hide column(s)"),this,SLOT(hideSelectedColumns()));

    menu.addAction(tr("Create new column"),this,SLOT(createNewColumn()));

    QAction *copyToColumnAction = menu.addAction(tr("Copy to"),this,SLOT(copyColumn()));
    if(rows.size()!=1){
        copyToColumnAction->setEnabled(false);
    }

    QAction *deleteColumnAction =menu.addAction(tr("Delete column(s)"));
    connect(deleteColumnAction,SIGNAL(triggered()),this,SLOT(deleteSelectedColumns()),Qt::QueuedConnection);
    //Avoid to delete inherited properties
    for(QModelIndexList::iterator it  = rows.begin() ; it != rows.end(); ++it){
        //If the property is an inherited property don't erase it
        if(_tableColumnModel->propertyForIndex(*it)->getGraph() != _tableColumnModel->graphTableModel()->graph()){
            deleteColumnAction->setEnabled(false);
        }
    }
    menu.exec(ui->listView->mapToGlobal(position));

}
void TulipTableWidgetColumnSelectionWidget::showSelectedColumns(){
    QModelIndexList rows = ui->listView->selectionModel()->selectedRows(0);
    if(!rows.isEmpty()){
        for(QModelIndexList::iterator it  = rows.begin() ; it != rows.end(); ++it){
            _tableColumnModel->setColumnVisible((*it).row(),true);
        }
    }
}

void TulipTableWidgetColumnSelectionWidget::hideSelectedColumns(){
    QModelIndexList rows = ui->listView->selectionModel()->selectedRows(0);
    if(!rows.isEmpty()){
        for(QModelIndexList::iterator it  = rows.begin() ; it != rows.end(); ++it){
            _tableColumnModel->setColumnVisible((*it).row(),false);
        }
    }
}

void TulipTableWidgetColumnSelectionWidget::createNewColumn(){
    tlp::PropertyCreationDialog::createNewProperty(_tableColumnModel->graphTableModel()->graph(),this);
}

void TulipTableWidgetColumnSelectionWidget::copyColumn(){

}


void TulipTableWidgetColumnSelectionWidget::deleteSelectedColumns(){
    QModelIndexList rows = ui->listView->selectionModel()->selectedRows(0);
    ui->listView->selectionModel()->clearSelection();
    tlp::Observable::holdObservers();
    for(QModelIndexList::iterator it  = rows.begin() ; it != rows.end(); ++it){
        tlp::PropertyInterface* property = _tableColumnModel->propertyForIndex(*it);
        property->getGraph()->delLocalProperty(std::string(property->getName()));
    }
    tlp::Observable::unholdObservers();
}
