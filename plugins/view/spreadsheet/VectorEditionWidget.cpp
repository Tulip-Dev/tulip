#include "VectorEditionWidget.h"
#include "ui_VectorEditionWidget.h"
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include "TulipItemDelegate.h"

using namespace tlp;

VectorEditionWidget::VectorEditionWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::VectorEditionWidget)
{
    ui->setupUi(this);
    connect(ui->addPushButton,SIGNAL(clicked(bool)),this,SLOT(addRow()));
    connect(ui->delPushButton,SIGNAL(clicked(bool)),this,SLOT(removeRows()));
    connect(ui->setAllPushButton,SIGNAL(clicked(bool)),this,SLOT(setAll()));
}

VectorEditionWidget::~VectorEditionWidget()
{
    delete ui;
}

ListPropertyWidgetModel::ListPropertyWidgetModel(TulipPropertyType elementsType,ListPropertyWidgetTypeMangerInterface *typeManager, QWidget* parent): QAbstractListModel( parent),elements(typeManager),elementsType(elementsType)
{

}

ListPropertyWidgetModel::~ListPropertyWidgetModel()
{
    delete elements;
}

QVariant ListPropertyWidgetModel::data(const QModelIndex& index, int role) const
{
    if(index.isValid() && index.row() < elements->getElementNumber()){
        if(role == Qt::DisplayRole){
            return elements->getStringValue(index.row());
        }
        if(role == Qt::EditRole){
            return elements->getValue(index.row());
        }
    }
    return QVariant();
}

int ListPropertyWidgetModel::rowCount(const QModelIndex&) const
{
    return elements->getElementNumber();
}

Qt::ItemFlags ListPropertyWidgetModel::flags(const QModelIndex& index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool ListPropertyWidgetModel::setData(const QModelIndex& index, const QVariant& value, int)
{
    if(index.isValid() && index.row() < elements->getElementNumber()){
        if(elements->setValue(index.row(),value)){
            emit dataChanged(index,index);
            return true;
        }
    }else{
        return false;
    }
    return false;
}
bool ListPropertyWidgetModel::insertRows(int, int, const QModelIndex& parent)
{
    beginInsertRows(parent,elements->getElementNumber(),elements->getElementNumber());
    elements->insertRow();
    endInsertRows();
    return true;
}

bool ListPropertyWidgetModel::removeRows(int row, int count, const QModelIndex& parent)
{
    if(row < elements->getElementNumber() && row + count <= elements->getElementNumber()){
        beginRemoveRows(parent,row,row+count-1);
        for(int i = 0 ; i < count ; ++i){
            elements->deleteRow(row+i);
        }
        endRemoveRows();
        return true;
    }
    return false;
}

ListPropertyWidgetTypeMangerInterface* VectorEditionWidget::getInterface()
{
    return ((ListPropertyWidgetModel*)ui->listView->model())->getInterface();
}

void VectorEditionWidget::setInterface(TulipPropertyType elementsType,ListPropertyWidgetTypeMangerInterface *interf){
    ui->listView->setModel(new ListPropertyWidgetModel(elementsType,interf,this));
    ui->listView->setItemDelegate(new TulipItemDelegate());    
}

void VectorEditionWidget::addRow(){
    ui->listView->model()->insertRow(ui->listView->model()->rowCount());
    ui->listView->selectionModel()->select(ui->listView->model()->index(ui->listView->model()->rowCount()-1,0),QItemSelectionModel::ClearAndSelect);
}
void VectorEditionWidget::removeRows(){
    QModelIndexList selection = ui->listView->selectionModel()->selectedRows();
    if(!selection.empty()){
        ui->listView->model()->removeRows(selection.first().row(),selection.count());
    }
}
void VectorEditionWidget::setAll(){
    if(ui->listView->model()->rowCount() > 0){
        QDialog dialog(this);
        dialog.setLayout(new QVBoxLayout(this));        
        QWidget *editorWidget = ui->listView->itemDelegate()->createEditor(this,QStyleOptionViewItem(),ui->listView->model()->index(0,0));
        dialog.layout()->addWidget(editorWidget);
        QDialogButtonBox *buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        dialog.layout()->addWidget(buttonBox);
        connect(buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
        connect(buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
        if(dialog.exec()== QDialog::Accepted){
            for(int i = 0 ; i < ui->listView->model()->rowCount() ; ++i){
                ui->listView->itemDelegate()->setModelData(editorWidget,ui->listView->model(),ui->listView->model()->index(i,0));
            }
        }
    }
}
