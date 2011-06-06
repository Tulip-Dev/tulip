#include "TableViewColumnModel.h"
#include <QtGui/QTableView>
TableViewColumnModel::TableViewColumnModel(QTableView* tableView ,QObject* parent):QAbstractListModel(parent),_tableView(tableView),_tableModel(tableView->model())
{
    connect(_tableModel,SIGNAL(columnsAboutToBeInserted(QModelIndex,int,int)),this,SLOT(columnsAboutToBeInserted(QModelIndex,int,int)));
    connect(_tableModel,SIGNAL(columnsAboutToBeRemoved(QModelIndex,int,int)),this,SLOT(columnsAboutToBeRemoved(QModelIndex,int,int)));
    connect(_tableModel,SIGNAL(columnsMoved(QModelIndex,int,int,QModelIndex,int)),this,SLOT(columnsMoved(QModelIndex,int,int,QModelIndex,int)));
    connect(_tableModel,SIGNAL(columnsInserted (QModelIndex,int,int)),this,SLOT(columnsInserted(QModelIndex,int,int)));
    connect(_tableModel,SIGNAL(columnsMoved(QModelIndex,int,int,QModelIndex,int)),this,SLOT(columnsMoved(QModelIndex,int,int,QModelIndex,int)));
    connect(_tableModel,SIGNAL(columnsRemoved(QModelIndex,int,int)),this,SLOT(columnsRemoved(QModelIndex,int,int)));
    connect(_tableModel,SIGNAL(headerDataChanged(Qt::Orientation,int,int)),this,SLOT(headerDataChanged(Qt::Orientation,int,int)));
}

int TableViewColumnModel::rowCount( const QModelIndex &) const{
    return _tableModel->columnCount();
}

QVariant TableViewColumnModel::data( const QModelIndex & index, int role) const{
    if(hasIndex(index.row(),index.column(),index.parent())){
        switch(role){
        case Qt::DisplayRole:
            return _tableModel->headerData(index.row(),Qt::Horizontal,Qt::DisplayRole);
            break;
        case Qt::CheckStateRole:
            {
                QVariant v = _tableView->isColumnHidden(index.row())?QVariant(Qt::Unchecked):QVariant(Qt::Checked);
                return v;
            }
            break;
        default:
            return QVariant();
            break;
        }
    }
    return QVariant();
}

bool TableViewColumnModel::setData( const QModelIndex & index, const QVariant & value, int role){
    if(hasIndex(index.row(),index.column(),index.parent())){
        switch(role){
        case Qt::CheckStateRole:
            {
                Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
                _tableView->setColumnHidden(index.row(),state==Qt::Unchecked);
                emit dataChanged(index,index);
                return true;
            }
            break;
        default:
            return false;
        }
    }
    return false;
}

Qt::ItemFlags TableViewColumnModel::flags( const QModelIndex & index ) const{
    return QAbstractListModel::flags(index) | Qt::ItemIsUserCheckable;
}

void TableViewColumnModel::columnsInserted( const QModelIndex &, int , int  ){
    endInsertColumns();
    reset();
}

void TableViewColumnModel::columnsMoved( const QModelIndex & , int , int , const QModelIndex & , int  ){
    reset();
}

void TableViewColumnModel::columnsRemoved( const QModelIndex & , int , int  ){

    endRemoveColumns();
    reset();
}

void TableViewColumnModel::headerDataChanged( Qt::Orientation , int , int  ){
    reset();
}

void TableViewColumnModel::columnsAboutToBeInserted( const QModelIndex & , int first, int last ){
    beginInsertColumns(QModelIndex(),first,last);
}


void TableViewColumnModel::columnsAboutToBeRemoved( const QModelIndex & , int first, int last ){
    beginRemoveColumns(QModelIndex(),first,last);
}

void TableViewColumnModel::setColumnVisible(int columnIndex,bool visible){
    if(hasIndex(columnIndex,0)){
        _tableView->setColumnHidden(columnIndex,!visible);
        emit dataChanged(index(columnIndex,0),index(columnIndex,0));
    }
}
