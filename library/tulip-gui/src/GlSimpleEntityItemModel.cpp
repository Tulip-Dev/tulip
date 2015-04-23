/**
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

#include <tulip/GlSimpleEntityItemModel.h>
#include <tulip/GlSimpleEntity.h>

#include <QStringList>
#include <QFont>

namespace tlp {

QStringList GlSimpleEntityItemEditor::propertiesNames() const {
  return QStringList();
}

QVariantList GlSimpleEntityItemEditor::propertiesQVariant() const {
  return QVariantList();
}

void GlSimpleEntityItemEditor::setProperty(const QString &, const QVariant &) {
}

GlSimpleEntityItemModel::GlSimpleEntityItemModel(GlSimpleEntityItemEditor *itemEditor, QObject *parent): QAbstractItemModel(parent),editor(itemEditor) {
}

GlSimpleEntityItemModel::~GlSimpleEntityItemModel() {
}

int GlSimpleEntityItemModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;

  return editor->propertiesNames().size();
}

int GlSimpleEntityItemModel::columnCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;

  return 1;
}

QModelIndex GlSimpleEntityItemModel::parent(const QModelIndex &) const {
  return QModelIndex();
}

QVariant GlSimpleEntityItemModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (orientation == Qt::Horizontal) {
    if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
      return headerText();
    else if (role == Qt::TextAlignmentRole)
      return Qt::AlignCenter;
  }
  else if(role==Qt::DisplayRole) {
    return editor->propertiesNames()[section];
  }

  if (orientation == Qt::Horizontal && role == Qt::FontRole) {
    QFont f;
    f.setBold(true);
    f.setPointSize(f.pointSize() - 1);
    return f;
  }

  return QVariant();
}

QModelIndex GlSimpleEntityItemModel::index(int row, int column,const QModelIndex &parent) const {
  if (!hasIndex(row,column,parent))
    return QModelIndex();

  return QAbstractItemModel::createIndex(row,column,(void*)(NULL));
}

QVariant GlSimpleEntityItemModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole) {
    return editor->propertiesQVariant()[index.row()];
  }

  return QVariant();
}

bool GlSimpleEntityItemModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if(role==Qt::EditRole) {
    editor->setProperty(editor->propertiesNames()[index.row()],value);
    return true;
  }

  return false;
}

}
