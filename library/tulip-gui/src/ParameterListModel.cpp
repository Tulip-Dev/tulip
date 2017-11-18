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
#include <tulip/ParameterListModel.h>
#include <tulip/ForEach.h>
#include <tulip/TulipMetaTypes.h>
#include <tulip/TlpQtTools.h>

#include <QColor>
#include <QModelIndex>
#include <QIcon>

namespace tlp {

ParameterListModel::ParameterListModel(const tlp::ParameterDescriptionList &params,
                                       tlp::Graph *graph, QObject *parent)
    : TulipModel(parent), _graph(graph) {
  ParameterDescription param;
  std::vector<ParameterDescription> outParams;
  // first add in parameters
  forEach(param, params.getParameters()) {
    if (param.getDirection() != OUT_PARAM)
      _params.push_back(param);
    else
      outParams.push_back(param);
  }

  // then add out parameters
  for (unsigned int i = 0; i < outParams.size(); ++i) {
    _params.push_back(outParams[i]);
  }

  // no sort, keep the predefined ordering
  params.buildDefaultDataSet(_data, graph);
}

QModelIndex ParameterListModel::index(int row, int column, const QModelIndex &) const {
  return createIndex(row, column);
}

QModelIndex ParameterListModel::parent(const QModelIndex &) const {
  return QModelIndex();
}

int ParameterListModel::rowCount(const QModelIndex &) const {
  return _params.size();
}

int ParameterListModel::columnCount(const QModelIndex &) const {
  return 1;
}

QVariant ParameterListModel::data(const QModelIndex &index, int role) const {
  if (role == GraphRole)
    return QVariant::fromValue<tlp::Graph *>(_graph);

  const ParameterDescription &info = _params[index.row()];

  if (role == Qt::ToolTipRole)
    return tlp::tlpStringToQString(info.getHelp());
  else if (role == Qt::WhatsThisRole)
    return tlp::tlpStringToQString(info.getHelp());
  else if (role == Qt::BackgroundRole) {
    if (info.isMandatory())
      return QColor(255, 255, 222);
    else
      return QColor(222, 255, 222);
  } else if (role == Qt::DisplayRole) {
    tlp::DataType *dataType = _data.getData(info.getName());

    if (!dataType)
      return info.getTypeName().c_str();

    QVariant result = TulipMetaTypes::dataTypeToQvariant(dataType, info.getName());
    delete dataType;
    return result;
  } else if (role == MandatoryRole) {
    return info.isMandatory();
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
    const ParameterDescription &info = _params[section];

    if (role == Qt::DisplayRole) {
      // remove prefix if any
      size_t pos = info.getName().find("::");

      if (pos != std::string::npos)
        return tlp::tlpStringToQString(info.getName().c_str() + pos + 2);

      return tlp::tlpStringToQString(info.getName().c_str());
    } else if (role == Qt::BackgroundRole) {
      if (info.isMandatory())
        return QColor(255, 255, 222);
      else
        return QColor(222, 255, 222);
    } else if (role == Qt::ToolTipRole) {
      return tlp::tlpStringToQString(info.getHelp());
    } else if (role == Qt::DecorationRole) {
      if (info.getDirection() == IN_PARAM) {
        return QIcon(":/tulip/gui/icons/32/input.png");
      } else if (info.getDirection() == OUT_PARAM) {
        return QIcon(":/tulip/gui/icons/32/output.png");
      } else {
        return QIcon(":/tulip/gui/icons/32/input-output.png");
      }
    }
  }

  return TulipModel::headerData(section, orientation, role);
}

Qt::ItemFlags ParameterListModel::flags(const QModelIndex &index) const {
  Qt::ItemFlags result = QAbstractItemModel::flags(index);
  const ParameterDescription &info = _params[index.row()];
  bool editable = info.isEditable();

  if (index.column() == 0) {
    if (editable)
      result |= Qt::ItemIsEditable;
  } else if (!editable)
    result ^= Qt::ItemIsEditable;

  return result;
}

bool ParameterListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (role == Qt::EditRole) {
    const ParameterDescription &info = _params[index.row()];

    DataType *dataType = TulipMetaTypes::qVariantToDataType(value);

    if (dataType)
      _data.setData(info.getName(), dataType);

    return (dataType != NULL);
  }

  return QAbstractItemModel::setData(index, value, role);
}

DataSet ParameterListModel::parametersValues() const {
  return _data;
}

void ParameterListModel::setParametersValues(const DataSet &data) {
  _data = data;
}
}
