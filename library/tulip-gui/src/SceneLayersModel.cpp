#include "tulip/SceneLayersModel.h"

#include <tulip/GlScene.h>

using namespace tlp;



SceneLayersModel::SceneLayersModel(GlScene* scene, QObject *parent): QAbstractItemModel(parent), _scene(scene) {
}

QModelIndex SceneLayersModel::index(int row, int column,const QModelIndex &parent) const {
  if (!parent.isValid()) { // Top level: layers
    std::vector<std::pair<std::string, GlLayer*> > layers = _scene->getLayersList();
    if (row > layers.size())
      return QModelIndex();
    return createIndex(row,column,layers[row].second);
  }

}
QModelIndex SceneLayersModel::parent(const QModelIndex &child) const {

}
int SceneLayersModel::rowCount(const QModelIndex &parent) const {

}
int SceneLayersModel::columnCount(const QModelIndex &parent) const {
  return 3;
}
QVariant SceneLayersModel::data(const QModelIndex &index, int role) const {

}
QVariant SceneLayersModel::headerData(int section, Qt::Orientation orientation, int role) const {

}
//  bool setData(const QModelIndex &index, const QVariant &value, int role);
//  Qt::ItemFlags flags(const QModelIndex &index) const;
