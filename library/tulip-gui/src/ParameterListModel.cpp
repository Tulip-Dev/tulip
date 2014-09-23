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
#include <tulip/ParameterListModel.h>
#include <tulip/ForEach.h>
#include <tulip/TulipMetaTypes.h>
#include <tulip/TlpQtTools.h>

#include <QColor>
#include <QModelIndex>

namespace tlp {

ParameterListModel::ParameterListModel(const tlp::ParameterDescriptionList &params, tlp::Graph *graph, QObject *parent)
  : TulipModel(parent), _graph(graph) {
  ParameterDescription param;
  QVector<ParameterDescription> outParams;
  // first add in parameters
  forEach(param,params.getParameters()) {
    if (param.getDirection() != OUT_PARAM)
      _params.push_back(param);
    else
      outParams.push_back(param);
  }

  // then add out parameters
  for(int i = 0; i < outParams.size(); ++i) {
    _params.push_back(outParams[i]);
  }

  // no sort, keep the predefined ordering
  params.buildDefaultDataSet(_data,graph);
}

QModelIndex ParameterListModel::index(int row, int column,const QModelIndex&) const {
  return createIndex(row,column);
}

QModelIndex ParameterListModel::parent(const QModelIndex&) const {
  return QModelIndex();
}

int ParameterListModel::rowCount(const QModelIndex&) const {
  return _params.size();
}

int ParameterListModel::columnCount(const QModelIndex&) const {
  return 1;
}

QVariant ParameterListModel::data(const QModelIndex &index, int role) const {
  if (role == GraphRole)
    return QVariant::fromValue<tlp::Graph*>(_graph);

  const ParameterDescription& infos = _params[index.row()];

  if (role == Qt::ToolTipRole)
    return tlp::tlpStringToQString(infos.getHelp());
  else if (role == Qt::WhatsThisRole)
    return tlp::tlpStringToQString(infos.getHelp());
  else if (role == Qt::BackgroundRole) {
    if (infos.isMandatory())
      return QColor(255, 255, 222);
    else
      return QColor(222, 255, 222);
  }
  else if (role == Qt::DisplayRole) {
    tlp::DataType *dataType = _data.getData(infos.getName());

    if (!dataType)
      return infos.getTypeName().c_str();

    QVariant result = TulipMetaTypes::dataTypeToQvariant(dataType, infos.getName());
    delete dataType;
    return result;
  }
  else if (role == MandatoryRole) {
    return infos.isMandatory();
  }

  return QVariant();
}

QVariant ParameterListModel::headerData(int section, Qt::Orientation orientation, int role) const {

  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    if (section == 0)
      return QObject::trUtf8("Name");
    else
      return QObject::trUtf8("Value");
  }

  if (orientation == Qt::Vertical) {
    const ParameterDescription& infos = _params[section];

    if (role == Qt::DisplayRole) {
      // remove prefix if any
      size_t pos = infos.getName().find("::");

      if (pos != std::string::npos)
        return infos.getName().c_str() + pos + 2;

      return infos.getName().c_str();
    }
    else if (role == Qt::BackgroundRole) {
      if (infos.isMandatory())
        return QColor(255, 255, 222);
      else
        return QColor(222, 255, 222);
    }
    else if (role == Qt::ToolTipRole)
      return tlp::tlpStringToQString(infos.getHelp());
  }

  return TulipModel::headerData(section,orientation,role);
}

Qt::ItemFlags ParameterListModel::flags(const QModelIndex &index) const {
  Qt::ItemFlags result = QAbstractItemModel::flags(index);

  if (index.column() == 0)
    result |= Qt::ItemIsEditable;

  return result;
}

bool ParameterListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (role == Qt::EditRole) {
    const ParameterDescription& infos = _params[index.row()];

    DataType *dataType = TulipMetaTypes::qVariantToDataType(value);

    if (dataType)
      _data.setData(infos.getName(), dataType);

    return (dataType != NULL);
  }

  return QAbstractItemModel::setData(index,value,role);
}

DataSet ParameterListModel::parametersValues() const {
  return _data;
}

void ParameterListModel::setParametersValues(const DataSet &data) {
  _data = data;
}

}
