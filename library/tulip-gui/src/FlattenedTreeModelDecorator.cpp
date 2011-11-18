#include "tulip/FlattenedTreeModelDecorator.h"

FlattenedTreeModelDecorator::FlattenedTreeModelDecorator(QAbstractItemModel* component, QObject *parent): QAbstractItemModel(parent), _component(component) {
  buildCache(QModelIndex(),0);
}

QModelIndex FlattenedTreeModelDecorator::index(int row, int column,const QModelIndex &) const {
  if (column != 0 || row < 0 || row >= _flattenedItemsCache.size())
    return QModelIndex();
  return createIndex(row,column);
}

QModelIndex FlattenedTreeModelDecorator::parent(const QModelIndex &child) const {
  return QModelIndex();
}

int FlattenedTreeModelDecorator::rowCount(const QModelIndex &parent) const {
  if (parent != QModelIndex())
    return 0;
  return _flattenedItemsCache.size();
}

int FlattenedTreeModelDecorator::columnCount(const QModelIndex &parent) const {
  return 1;
}

QVariant FlattenedTreeModelDecorator::data(const QModelIndex &index, int role) const {
  if (!hasIndex(index.row(),index.column(),index.parent()))
    return QVariant();
  return _component->data(_flattenedItemsCache[index.row()].originalIndex,role);
}

bool FlattenedTreeModelDecorator::setData(const QModelIndex &index, const QVariant &value, int role) {
  return false;
}

Qt::ItemFlags FlattenedTreeModelDecorator::flags(const QModelIndex &index) const {
  return QAbstractItemModel::flags(index);
}

QVariant FlattenedTreeModelDecorator::headerData(int section, Qt::Orientation orientation, int role) const {
  return QVariant();
}

void FlattenedTreeModelDecorator::buildCache(const QModelIndex &parent, int depth) {
  if (parent.isValid() && _component->hasIndex(parent.row(),parent.column(),parent.parent()))
    _flattenedItemsCache.push_back(ItemInfos(depth,parent));

  depth++;
  int rowCount = _component->rowCount(parent);
  for (int i=0;i<rowCount;++i) {
    QModelIndex index = _component->index(i,0,parent);
    buildCache(index,depth);
  }
}
