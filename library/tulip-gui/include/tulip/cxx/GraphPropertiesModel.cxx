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

#include <tulip/Graph.h>

namespace tlp {

template<typename PROPTYPE>
void tlp::GraphPropertiesModel<PROPTYPE>::rebuildCache() {
  _properties.clear();

  if (_graph == NULL)
    return;

  std::string propName;
  forEach(propName,_graph->getInheritedProperties()) {
#ifdef NDEBUG

    if (propName == "viewMetaGraph")
      continue;

#endif
    PROPTYPE* prop = dynamic_cast<PROPTYPE*>(_graph->getProperty(propName));

    if (prop != NULL) {
      _properties+=prop;
    }
  }
  forEach(propName,_graph->getLocalProperties()) {
#ifdef NDEBUG

    if (propName == "viewMetaGraph")
      continue;

#endif
    PROPTYPE* prop = dynamic_cast<PROPTYPE*>(_graph->getProperty(propName));

    if (prop != NULL) {
      _properties+=prop;
    }
  }
}

template<typename PROPTYPE>
GraphPropertiesModel<PROPTYPE>::GraphPropertiesModel(tlp::Graph* graph, bool checkable, QObject *parent): tlp::TulipModel(parent), _graph(graph), _placeholder(QString::null), _checkable(checkable), _removingRows(false), forcingRedraw(false) {
  if (_graph != NULL) {
    _graph->addListener(this);
    rebuildCache();
  }
}

template<typename PROPTYPE>
GraphPropertiesModel<PROPTYPE>::GraphPropertiesModel(QString placeholder, tlp::Graph* graph, bool checkable, QObject *parent): tlp::TulipModel(parent), _graph(graph), _placeholder(placeholder), _checkable(checkable), _removingRows(false), forcingRedraw(false) {
  if (_graph != NULL) {
    _graph->addListener(this);
    rebuildCache();
  }
}

template<typename PROPTYPE>
QModelIndex GraphPropertiesModel<PROPTYPE>::index(int row, int column,const QModelIndex &parent) const {
  if (_graph == NULL || !hasIndex(row,column,parent))
    return QModelIndex();

  int vectorIndex = row;

  if (!_placeholder.isNull()) {
    if (row == 0)
      return createIndex(row,column);

    vectorIndex--;
  }

  return createIndex(row,column,_properties[vectorIndex]);
}

template<typename PROPTYPE>
QModelIndex GraphPropertiesModel<PROPTYPE>::parent(const QModelIndex &) const {
  return QModelIndex();
}

template<typename PROPTYPE>
int GraphPropertiesModel<PROPTYPE>::rowCount(const QModelIndex &parent) const {
  if (parent.isValid() || _graph == NULL || forcingRedraw)
    return 0;

  int result = _properties.size();

  if (!_placeholder.isNull())
    result++;

  return result;
}

template<typename PROPTYPE>
int GraphPropertiesModel<PROPTYPE>::columnCount(const QModelIndex &) const {
  return 3;
}

template<typename PROPTYPE>
QVariant GraphPropertiesModel<PROPTYPE>::data(const QModelIndex &index, int role) const {
  if (_graph == NULL || (index.internalPointer() == NULL && index.row() != 0))
    return QVariant();

  PropertyInterface* pi = (PropertyInterface*)(index.internalPointer());

  if (role == Qt::DisplayRole || role == Qt::ToolTipRole) {
    if (!_placeholder.isNull() && index.row() == 0)
      return _placeholder;

    if (pi == NULL)
      return QString();

    if (index.column() == 0)
      return QString::fromUtf8(pi->getName().c_str());
    else if (index.column() == 1)
      return pi->getTypename().c_str();
    else if (index.column() == 2)
      return (_graph->existLocalProperty(pi->getName()) ? trUtf8("Local") : tr("Inherited from graph ") + QString::number(pi->getGraph()->getId()) + " (" + QString::fromUtf8(pi->getGraph()->getName().c_str()) + ')');
  }

  else if (role == Qt::DecorationRole && index.column() == 0 && pi != NULL && !_graph->existLocalProperty(pi->getName()))
    return QIcon(":/tulip/gui/ui/inherited_properties.png");

  else if (role == Qt::FontRole) {
    QFont f;

    if (!_placeholder.isNull() && index.row() == 0)
      f.setItalic(true);

    return f;
  }
  else if (role == PropertyRole) {
    return QVariant::fromValue<PropertyInterface*>(pi);
  }
  else if (_checkable && role == Qt::CheckStateRole && index.column() == 0) {
    return (_checkedProperties.contains((PROPTYPE*) pi) ? Qt::Checked : Qt::Unchecked);
  }

  return QVariant();
}

template<typename PROPTYPE>
int GraphPropertiesModel<PROPTYPE>::rowOf(PROPTYPE* pi) const {
  int result = _properties.indexOf(pi);

  if (!_placeholder.isNull())
    ++result;

  return result;
}

template<typename PROPTYPE>
int GraphPropertiesModel<PROPTYPE>::rowOf(const QString& pName) const {
  for(int i = 0; i < _properties.size(); ++i) {
    if (pName == QString::fromUtf8(_properties[i]->getName().c_str()))
      return i;
  }

  return -1;
}

template<typename PROPTYPE>
QVariant tlp::GraphPropertiesModel<PROPTYPE>::headerData(int section, Qt::Orientation orientation, int role) const {
  if (orientation == Qt::Horizontal) {
    if (role == Qt::DisplayRole) {
      if (section == 0)
        return trUtf8("Name");
      else if (section == 1)
        return trUtf8("Type");
      else if (section == 2)
        return trUtf8("Scope");
    }
  }

  return TulipModel::headerData(section,orientation,role);
}

template<typename PROPTYPE>
bool tlp::GraphPropertiesModel<PROPTYPE>::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (_graph == NULL)
    return false;

  if (_checkable && role == Qt::CheckStateRole && index.column() == 0) {
    if (value.value<int>() == (int)Qt::Checked)
      _checkedProperties.insert((PROPTYPE*)index.internalPointer());
    else
      _checkedProperties.remove((PROPTYPE*)index.internalPointer());

    emit checkStateChanged(index,(Qt::CheckState)(value.value<int>()));
    return true;
  }

  return false;
}

}
