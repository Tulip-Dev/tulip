/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

  void treatEvent(const tlp::Event &);

signals:
  void drawNeeded(tlp::GlScene*);
};
}

#endif // SCENELAYERSMODEL_H
///@endcond
