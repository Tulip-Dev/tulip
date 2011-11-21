/**
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
#ifndef FLATTENEDTREEMODELDECORATOR_H
#define FLATTENEDTREEMODELDECORATOR_H

#include <tulip/tulipconf.h>

#include <QtCore/QVector>
#include <QtCore/QAbstractItemModel>

class TLP_QT_SCOPE FlattenedTreeModelDecorator : public QAbstractItemModel {
  struct ItemInfos {
    ItemInfos(int d, const QModelIndex& idx): depth(d), originalIndex(idx) {}
    ItemInfos() {}
    ItemInfos(const ItemInfos& cpy) {
      depth = cpy.depth;
      originalIndex = cpy.originalIndex;
    }
    int depth;
    QModelIndex originalIndex;
  };

  QAbstractItemModel* _component;
  QVector<ItemInfos> _flattenedItemsCache;

  void buildCache(const QModelIndex& parent, int depth);

public:
  explicit FlattenedTreeModelDecorator(QAbstractItemModel* component, QObject *parent = 0);

  int mapToRow(const QModelIndex& originalIndex) const;

  QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role);
  Qt::ItemFlags flags(const QModelIndex &index) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // FLATTENEDTREEMODELDECORATOR_H
