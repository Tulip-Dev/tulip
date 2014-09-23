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

#ifndef TULIPMODEL_H
#define TULIPMODEL_H

#include <QAbstractItemModel>

#include <tulip/tulipconf.h>

namespace tlp {

class TLP_QT_SCOPE TulipModel: public QAbstractItemModel {
  Q_OBJECT
  Q_ENUMS(TulipRole)

public:
  enum TulipRole {
    GraphRole = Qt::UserRole+1,
    PropertyRole = Qt::UserRole+2,
    IsNodeRole = Qt::UserRole+3,
    StringRole = Qt::UserRole+4,
    MandatoryRole = Qt::UserRole+5,
    ElementIdRole = Qt::UserRole+6
  };

  explicit TulipModel(QObject *parent=NULL);
  virtual ~TulipModel();

  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

signals:
  void checkStateChanged(QModelIndex,Qt::CheckState);
};

}

#endif // TULIPMODEL_H
///@endcond
