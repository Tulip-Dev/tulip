#include "TableColumnSelectionWidget.h"
#include "ui_TableColumnSelectionWidget.h"
#include "TableViewColumnModel.h"
#include <QtGui/QMenu>
#include <cassert>

TableColumnSelectionWidget::TableColumnSelectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableColumnSelectionWidget),_tableColumnModel(NULL)
{
    ui->setupUi(this);    
    connect(ui->checkUncheckAllCheckBox,SIGNAL(stateChanged(int)),this,SLOT(checkUncheckAllStateChanged(int)));
    //Predefined filters
    connect(ui->dataPropertiesPushButton,SIGNAL(clicked()),this,SLOT(showDataProperties()));
    connect(ui->visualPropertiesPushButton,SIGNAL(clicked()),this,SLOT(showDataProperties()));
    connect(ui->visualPropertiesPushButton,SIGNAL(clicked()),this,SLOT(showDataProperties()));
    connect(ui->inheritedPropertiesPushButton,SIGNAL(clicked()),this,SLOT(showInheritedProperties()));
    connect(ui->localPropertiesPushButton,SIGNAL(clicked()),this,SLOT(showLocalProperties()));

    QMenu* menu = new QMenu(ui->typePropertiesPushButton);
    menu->addAction(tr("Color"));
    menu->addAction(tr("Integer"));
    ui->typePropertiesPushButton->setMenu(menu);
    //User pattern
    connect(ui->customPatternLineEdit,SIGNAL(textChanged( QString)),this,SLOT(showCustomPatternProperties(QString)));

}

TableColumnSelectionWidget::~TableColumnSelectionWidget()
{
    delete ui;
}

void TableColumnSelectionWidget::setTableView(QTableView* tableView){
    assert(tableView != NULL);
    QAbstractItemModel* oldModel = ui->listView->model();
    if(oldModel){
    disconnect(oldModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(updateCheckUncheckAllButtonState()));
    }
    _tableColumnModel = new TableViewColumnModel(tableView,this);
    ui->listView->setModel(_tableColumnModel);
    updateCheckUncheckAllButtonState();
    connect(_tableColumnModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(updateCheckUncheckAllButtonState()));
    if(oldModel != NULL){
        oldModel->deleteLater();
    }
}

void TableColumnSelectionWidget::showVisualProperties(){
    selectColumns(QRegExp("^([\\*]?view)"),true);
}

void TableColumnSelectionWidget::showDataProperties(){
    selectColumns(QRegExp("^([\\*]?view)"),false);
}
void TableColumnSelectionWidget::showLocalProperties(){
    selectColumns(QRegExp("^\\*"),false);
}

void TableColumnSelectionWidget::showInheritedProperties(){
    selectColumns(QRegExp("^\\*"),true);
}

void TableColumnSelectionWidget::showCustomPatternProperties(const QString& pattern){
    selectColumns(QRegExp(pattern),true);
}

void TableColumnSelectionWidget::selectColumns(const QRegExp& regExp,bool match){
    assert(_tableColumnModel != NULL);
    for(int i = 0 ; i < _tableColumnModel->rowCount() ; ++i){
        _tableColumnModel->setColumnVisible(i,match == (regExp.indexIn(_tableColumnModel->data(_tableColumnModel->index(i)).toString())!= -1));
    }
}

void TableColumnSelectionWidget::checkUncheckAllStateChanged(int state){
    bool showColumn = state == Qt::Unchecked?false:true;
    for(int i = 0 ; i < _tableColumnModel->rowCount() ; ++i){
        _tableColumnModel->setColumnVisible(i,showColumn);
    }
}

void TableColumnSelectionWidget::updateCheckUncheckAllButtonState(){
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

void TableColumnSelectionWidget::showPropertiesWithType(){
    QAction* action = qobject_cast<QAction*>(sender());
    if(action){

    }
}
