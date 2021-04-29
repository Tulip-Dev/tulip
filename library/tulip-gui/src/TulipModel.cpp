/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include "tulip/TulipModel.h"

#include <QFont>
#include <QWidget>

using namespace tlp;

TulipModel::TulipModel(QObject *parent) : QAbstractItemModel(parent) {}

TulipModel::~TulipModel() {}

QVariant TulipModel::headerData(int, Qt::Orientation orientation, int role) const {
  if (orientation == Qt::Horizontal && role == Qt::FontRole) {
    QFont f;
    QWidget *p = dynamic_cast<QWidget *>(QAbstractItemModel::parent());
    if (p)
      f = p->font();

    f.setBold(true);
    f.setPointSize(f.pointSize() - 1);
    return f;
  }

  return QVariant();
}
