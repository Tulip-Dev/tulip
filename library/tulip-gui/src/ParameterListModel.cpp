#include "tulip/ParameterListModel.h"

#include <tulip/ForEach.h>
#include <algorithm>
#include <QtGui/QFont>
#include <QtGui/QColor>
#include <tulip/Color.h>
#include <tulip/TlpQtTools.h>

namespace tlp {

bool ParameterListModel::ParamInfosSorter::operator()(ParameterListModel::ParamInfos a, ParameterListModel::ParamInfos b) {
  if (a.mandatory && !b.mandatory)
    return true;
  else if (!a.mandatory && b.mandatory)
    return false;
  return a.name.compare(b.name) < 0;
}

ParameterListModel::ParameterListModel(const tlp::ParameterList &params, tlp::Graph *graph, QObject *parent)
  : QAbstractItemModel(parent) {
  std::string name;
  forEach(name,params.getParametersNames())
    _params.push_back(ParamInfos(params.isMandatory(name),name.c_str(),params.getHelp(name).c_str()));
  std::sort(_params.begin(),_params.end(),ParamInfosSorter());
  params.buildDefaultDataSet(_data,graph);

}

QModelIndex ParameterListModel::index(int row, int column,const QModelIndex &parent) const {
  return createIndex(row,column);
}

QModelIndex ParameterListModel::parent(const QModelIndex &child) const {
  return QModelIndex();
}

int ParameterListModel::rowCount(const QModelIndex &parent) const {
  return _params.size();
}

int ParameterListModel::columnCount(const QModelIndex &parent) const {
  return 2;
}

QVariant ParameterListModel::data(const QModelIndex &index, int role) const {
  ParamInfos infos = _params[index.row()];
  if (role == Qt::ToolTipRole)
    return infos.name;
  else if (role == Qt::WhatsThisRole)
    return infos.desc;
  else if (role == Qt::FontRole && index.column() == 0) {
    QFont f;
    f.setBold(true);
    return f;
  }
  else if (role == Qt::BackgroundRole) {
    if (infos.mandatory)
      return QColor(255, 255, 222);
    else
      return QColor(222, 255, 222);
  }
  else if (role == Qt::DisplayRole) {
    if (index.column() == 0)
      return infos.name;

    return TulipMetaTypes::dataMemToQvariant(_data.getData(infos.name.toStdString()));
  }

  return QVariant();
}

QVariant ParameterListModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (orientation == Qt::Horizontal) {
    if (section == 0)
      return QObject::trUtf8("Name");
    else
      return QObject::trUtf8("Value");
  }
  return QAbstractItemModel::headerData(section,orientation,role);
}

Qt::ItemFlags ParameterListModel::flags(const QModelIndex &index) const {
  Qt::ItemFlags result = QAbstractItemModel::flags(index);
  if (index.column() == 1)
    result |= Qt::ItemIsEditable;
  return result;
}

}
