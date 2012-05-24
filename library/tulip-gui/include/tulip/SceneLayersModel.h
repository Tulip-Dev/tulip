#ifndef SCENELAYERSMODEL_H
#define SCENELAYERSMODEL_H

#include <tulip/tulipconf.h>
#include <tulip/Observable.h>
#include <tulip/TulipModel.h>

/**
  @brief Oh, you know, just a model for a GlScene entities
  */
namespace tlp {
class GlScene;

class TLP_QT_SCOPE SceneLayersModel : public TulipModel, tlp::Observable {
  Q_OBJECT

  tlp::GlScene* _scene;

  QModelIndex graphCompositeIndex() const;

public:
  explicit SceneLayersModel(tlp::GlScene* scene, QObject *parent = NULL);

  QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role);
  Qt::ItemFlags flags(const QModelIndex &index) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

signals:
  void drawNeeded(tlp::GlScene*);
};
}

#endif // SCENELAYERSMODEL_H
