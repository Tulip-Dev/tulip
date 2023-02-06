/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#include <tulip/GraphElementModel.h>
#include <tulip/TulipMetaTypes.h>
#include <tulip/Graph.h>

using namespace tlp;
using namespace std;

GraphElementModel::GraphElementModel(Graph *graph, unsigned int id, QObject *parent)
    : TulipModel(parent), _graph(graph), _id(id) {}

int GraphElementModel::rowCount(const QModelIndex &parent) const {
  if (_graph == nullptr || parent.isValid())
    return 0;
  return getGraphProperties().size();
}

int GraphElementModel::columnCount(const QModelIndex &parent) const {
  if (_graph == nullptr || parent.isValid())
    return 0;

  return 1;
}

QModelIndex GraphElementModel::parent(const QModelIndex & /*child*/) const {
  return QModelIndex();
}

QVariant GraphElementModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (orientation == Qt::Horizontal) {
    if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
      return headerText(_id);
    else if (role == Qt::TextAlignmentRole)
      return Qt::AlignCenter;
  } else if (role == Qt::DisplayRole) {
    return getGraphProperties()[section]->getName().c_str();
  }

  return TulipModel::headerData(section, orientation, role);
}

QModelIndex GraphElementModel::index(int row, int column, const QModelIndex &parent) const {
  if (!hasIndex(row, column, parent))
    return QModelIndex();
  return createIndex(row, column, getGraphProperties()[row]);
}

QVariant GraphElementModel::data(const QModelIndex &index, int role) const {
  if (role == PropertyNameRole) {
    return static_cast<PropertyInterface *>(index.internalPointer())->getName().c_str();
  } else if (role == Qt::DisplayRole) {
    return value(_id, static_cast<PropertyInterface *>(index.internalPointer()));
  } else if (role == TulipModel::PropertyRole) {
    return QVariant::fromValue<PropertyInterface *>(
        static_cast<PropertyInterface *>(index.internalPointer()));
  }

  return QVariant();
}

Qt::ItemFlags GraphElementModel::flags(const QModelIndex &index) const {
#ifdef NDEBUG
  return TulipModel::flags(index) | Qt::ItemIsEditable;
#else

  if (static_cast<PropertyInterface *>(index.internalPointer())->getName() == "viewMetaGraph")
    return TulipModel::flags(index);

  return TulipModel::flags(index) | Qt::ItemIsEditable;
#endif
}

std::vector<PropertyInterface *> GraphElementModel::getGraphProperties() const {
  std::vector<PropertyInterface *> properties;
  for (PropertyInterface *prop : _graph->getObjectProperties()) {
    auto propName = prop->getName();
    // an empty set indicates that all the properties are visible
    if (!_visibleProps.empty() && (_visibleProps.find(propName) == _visibleProps.end()))
      continue;
#ifdef NDEBUG
    if (propName == "viewMetaGraph")
      continue;
#endif
    properties.push_back(prop);
  }
  return properties;
}

bool GraphNodeElementModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (role == Qt::EditRole) {
    PropertyInterface *prop = static_cast<PropertyInterface *>(index.internalPointer());
    _graph->push();
    bool result = GraphModel::setNodeValue(_id, prop, value);
    _graph->popIfNoUpdates();
    return result;
  }

  return false;
}

bool GraphEdgeElementModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (role == Qt::EditRole) {
    PropertyInterface *prop = static_cast<PropertyInterface *>(index.internalPointer());
    _graph->push();
    bool result = GraphModel::setEdgeValue(_id, prop, value);
    _graph->popIfNoUpdates();
    return result;
  }

  return false;
}
