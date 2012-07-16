#ifndef GLSIMPLEENTITYITEMMODEL_H
#define GLSIMPLEENTITYITEMMODEL_H

#include <QtCore/QAbstractItemModel>

#include <tulip/GlSimpleEntity.h>

namespace tlp {

class GlSimpleEntityItemModel : public QAbstractItemModel {
  Q_OBJECT
  Q_ENUMS(SimpleEntityRole)

public:
  enum SimpleEntityRole {
    SimpleEntityRole = Qt::UserRole+1
  };

  GlSimpleEntityItemModel(GlSimpleEntity *entity,QObject *parent=NULL);
  virtual ~GlSimpleEntityItemModel();

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  QString headerText() const {
    return QString("toto");
  }

  bool setData(const QModelIndex &index, const QVariant &value, int role);

  Qt::ItemFlags flags(const QModelIndex &index) const {
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
  }

protected :

  GlSimpleEntity *_entity;
};

}

#endif // GLSIMPLEENTITYITEMMODEL_H
