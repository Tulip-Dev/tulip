#ifndef SCENELAYERSMODEL_H
#define SCENELAYERSMODEL_H

#include <QtCore/QAbstractItemModel>
#include <tulip/tulipconf.h>
#include <tulip/Observable.h>

/**
  @brief Oh, you know, just a model for a GlScene entities
  */
namespace tlp {
class GlScene;

class TLP_QT_SCOPE SceneLayersModel : public QAbstractItemModel, tlp::Observable {
  tlp::GlScene* _scene;

public:
  explicit SceneLayersModel(tlp::GlScene* scene, QObject *parent = 0);
  
  QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
//  bool setData(const QModelIndex &index, const QVariant &value, int role);
//  Qt::ItemFlags flags(const QModelIndex &index) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};
}

#endif // SCENELAYERSMODEL_H
