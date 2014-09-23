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

#include <tulip/GraphElementModel.h>

#include <QFont>

#include <tulip/ForEach.h>
#include <tulip/Graph.h>

using namespace tlp;
using namespace std;

GraphElementModel::GraphElementModel(Graph *graph,unsigned int id,QObject *parent):TulipModel(parent),_graph(graph),_id(id) {

}

int GraphElementModel::rowCount(const QModelIndex &parent) const {
  if (_graph == NULL || parent.isValid())
    return 0;

  PropertyInterface* prop;
  int result=0;
  forEach(prop,_graph->getObjectProperties()) {
#ifdef NDEBUG

    if (prop->getName() == "viewMetaGraph")
      continue;

#endif
    ++result;
  }
  return result;
}

int GraphElementModel::columnCount(const QModelIndex &parent) const {
  if (_graph == NULL || parent.isValid())
    return 0;

  return 1;
}

QModelIndex GraphElementModel::parent(const QModelIndex &/*child*/) const {
  return QModelIndex();
}

QVariant GraphElementModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (orientation == Qt::Horizontal) {
    if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
      return headerText(_id);
    else if (role == Qt::TextAlignmentRole)
      return Qt::AlignCenter;
  }
  else if(role==Qt::DisplayRole) {
    string propertyName;
    int result=0;
    forEach(propertyName,_graph->getProperties()) {
#ifdef NDEBUG

      if (propertyName == "viewMetaGraph")
        continue;

#endif

      if(section==result)
        return propertyName.c_str();

      ++result;
    }
  }

  return TulipModel::headerData(section,orientation,role);
}

QModelIndex GraphElementModel::index(int row, int column,const QModelIndex &parent) const {
  if (!hasIndex(row,column,parent))
    return QModelIndex();

  PropertyInterface* prop=NULL;
  int result=0;
  forEach(prop, _graph->getObjectProperties()) {
#ifdef NDEBUG

    if (prop->getName() == "viewMetaGraph")
      continue;

#endif

    if(result==row)
      break;

    ++result;
  }
  return createIndex(row,column,prop);
}

QVariant GraphElementModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole)
    return value(_id,(PropertyInterface*)(index.internalPointer()));

  return QVariant();
}

Qt::ItemFlags GraphElementModel::flags(const QModelIndex &index) const {
#ifdef NDEBUG
  return TulipModel::flags(index) | Qt::ItemIsEditable;
#else

  if (((PropertyInterface*)(index.internalPointer()))->getName() == "viewMetaGraph")
    return TulipModel::flags(index);

  return TulipModel::flags(index) | Qt::ItemIsEditable;
#endif
}

bool GraphNodeElementModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if(role==Qt::EditRole) {
    PropertyInterface* prop=NULL;
    int result=0;
    forEach(prop,_graph->getObjectProperties()) {
#ifdef NDEBUG

      if (prop->getName() == "viewMetaGraph")
        continue;

#endif

      if(result==index.row())
        break;

      ++result;
    }

    _graph->push();
    return GraphModel::setNodeValue(_id,prop,value);
  }

  return false;
}

bool GraphEdgeElementModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if(role==Qt::EditRole) {
    PropertyInterface* prop=NULL;
    int result=0;
    forEach(prop,_graph->getObjectProperties()) {
#ifdef NDEBUG

      if (prop->getName() == "viewMetaGraph")
        continue;

#endif

      if(result==index.row())
        break;

      ++result;
    }

    _graph->push();
    return GraphModel::setEdgeValue(_id,prop,value);
  }

  return false;
}

