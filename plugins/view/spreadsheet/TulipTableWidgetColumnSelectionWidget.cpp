#include "TulipTableWidgetColumnSelectionWidget.h"
#include "ui_TulipTableWidgetColumnSelectionWidget.h"
#include "TulipTableWidgetColumnSelectionModel.h"
#include "GraphTableWidget.h"
#include <QtGui/QMenu>
#include <QtGui/QStyledItemDelegate>
#include <cassert>


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

