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

#include <tulip/PluginModel.h>

using namespace tlp;
using namespace std;

SimplePluginListModel::SimplePluginListModel(const QList<std::string>& plugins,QObject *parent):TulipModel(parent), _list(plugins) {
}

SimplePluginListModel::~SimplePluginListModel() {
}

string SimplePluginListModel::pluginName(const QModelIndex& idx) const {
  int index = idx.row();

  if(index > -1 && index < _list.size()) {
    return _list.at(index);
  }
  else {
    return string();
  }
}

QList<std::string> SimplePluginListModel::plugins()const {
  return _list;
}

int SimplePluginListModel::columnCount ( const QModelIndex& ) const {
  return 1;
}

int SimplePluginListModel::rowCount(const QModelIndex &parent) const {
  if(parent.isValid())
    return 0;

  return _list.size();
}

QModelIndex SimplePluginListModel::parent(const QModelIndex &) const {
  return QModelIndex();
}

QModelIndex SimplePluginListModel::index(int row, int column,const QModelIndex &parent) const {
  if(parent.isValid())
    return QModelIndex();

  return createIndex(row, column);
}

QVariant SimplePluginListModel::data(const QModelIndex &index, int role) const {
  if(index.row() < _list.size()) {
    std::string name(_list[index.row()]);

    if(role == Qt::DisplayRole) {
      return tlp::tlpStringToQString(name);
    }
    else if (role == Qt::DecorationRole) {
      const Plugin& p = PluginLister::pluginInformation(name);
      QPixmap pix(tlp::tlpStringToQString(p.icon()));
      return pix;
    }
  }

  return QVariant();
}
