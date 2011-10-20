#include "TulipTableWidgetColumnSelectionModel.h"
#include "GraphTableWidget.h"
#include "GraphTableModel.h"
#include <tulip/PropertyInterface.h>

using namespace tlp;
TulipTableWidgetColumnSelectionModel::TulipTableWidgetColumnSelectionModel(GraphTableWidget* tableView ,QObject* parent):QAbstractListModel(parent),_tableView(tableView),_tableModel(tableView->graphModel()) {
  connect(_tableModel,SIGNAL(columnsInserted (QModelIndex,int,int)),this,SLOT(columnsInserted(QModelIndex,int,int)));
  connect(_tableModel,SIGNAL(columnsMoved(QModelIndex,int,int,QModelIndex,int)),this,SLOT(columnsMoved(QModelIndex,int,int,QModelIndex,int)));
  connect(_tableModel,SIGNAL(columnsRemoved(QModelIndex,int,int)),this,SLOT(columnsRemoved(QModelIndex,int,int)));
  connect(_tableModel,SIGNAL(headerDataChanged(Qt::Orientation,int,int)),this,SLOT(headerDataChanged(Qt::Orientation,int,int)));
}

int TulipTableWidgetColumnSelectionModel::rowCount( const QModelIndex &) const {
  if(_tableModel!=NULL) {
    return _tableModel->columnCount();
  }
  else {
    return 0;
  }
}

QVariant TulipTableWidgetColumnSelectionModel::data( const QModelIndex & index, int role) const {
  if(hasIndex(index.row(),index.column(),index.parent())) {
    switch(role) {
    case Qt::CheckStateRole: {
      return _tableView->isColumnHidden(index.row())?QVariant(Qt::Unchecked):QVariant(Qt::Checked);
    }
    break;

    default:
      return _tableModel->headerData(index.row(),Qt::Horizontal,role);
      break;
    }
  }

  return QVariant();
}

bool TulipTableWidgetColumnSelectionModel::setData( const QModelIndex & index, const QVariant & value, int role) {
  if(hasIndex(index.row(),index.column(),index.parent())) {
    switch(role) {
    case Qt::CheckStateRole: {
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

Qt::ItemFlags TulipTableWidgetColumnSelectionModel::flags( const QModelIndex & index ) const {
  return QAbstractListModel::flags(index) | Qt::ItemIsUserCheckable;
}

void TulipTableWidgetColumnSelectionModel::columnsInserted( const QModelIndex &, int first, int  last) {
  beginInsertRows(QModelIndex(),first,last);
  endInsertRows();
}

void TulipTableWidgetColumnSelectionModel::columnsMoved( const QModelIndex & , int sourceStart, int sourceEnd, const QModelIndex & , int  destinationRow ) {
  beginMoveRows(QModelIndex(),sourceStart,sourceEnd,QModelIndex(),destinationRow);
  endMoveRows();
}

void TulipTableWidgetColumnSelectionModel::columnsRemoved( const QModelIndex & , int first, int  last) {
  beginRemoveRows(QModelIndex(),first,last);
  endRemoveRows();
}

void TulipTableWidgetColumnSelectionModel::headerDataChanged( Qt::Orientation, int firt, int last) {
  emit dataChanged(index(firt),index(last));
}

void TulipTableWidgetColumnSelectionModel::setColumnVisible(int columnIndex,bool visible) {
  if(hasIndex(columnIndex,0)) {
    _tableView->setColumnHidden(columnIndex,!visible);
    emit dataChanged(index(columnIndex,0),index(columnIndex,0));
  }
}

PropertyInterface* TulipTableWidgetColumnSelectionModel::propertyForIndex(const QModelIndex& index) {
  return _tableModel->propertyForIndex(index);
}

bool TulipTableWidgetColumnSelectionModel::isColumnVisible(int columnIndex)const {
  return !_tableView->isColumnHidden(columnIndex);

}
