/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
///@cond DOXYGEN_HIDDEN


#ifndef GLSIMPLEENTITYITEMMODEL_H
#define GLSIMPLEENTITYITEMMODEL_H

#include <QAbstractItemModel>

#include <tulip/GlSimpleEntity.h>

namespace tlp {

class TLP_QT_SCOPE GlSimpleEntityItemModel : public QAbstractItemModel {
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
///@endcond
