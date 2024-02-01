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
#include <tulip/ParameterListModel.h>
#include <tulip/TulipMetaTypes.h>
#include <tulip/TulipItemDelegate.h>
#include <tulip/TlpQtTools.h>

#include <QFont>
#include <QFontMetrics>
#include <QHeaderView>
#include <QIcon>

namespace tlp {

ParameterListModel::ParameterListModel(const tlp::ParameterDescriptionList &params,
                                       tlp::Graph *graph, QObject *parent, bool showIcons,
                                       bool darkBackground)
    : TulipModel(parent), _graph(graph), _showIcons(showIcons), _darkBackground(darkBackground) {
  std::vector<ParameterDescription> outParams;
  // first add in parameters
  for (const ParameterDescription &param : params.getParameters()) {
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

// The method below is used to centralized the configuration of TableView
// displaying a ParameterDescriptionList.
// With Qt5 the verticalHeader is used to display the parameters names
// and there is only one column table to display/edit the parameters values.
// With Qt6 the vertical header cannot be used because there is a bug
// which make the parameters names hidden.
// So with Qt6 the vertical header must be hidden and then the first column
// is used to display the parameters names and the second on to display/edit
// the parameters values
ParameterListModel *
ParameterListModel::configureTableView(QTableView *tableView,
                                       const tlp::ParameterDescriptionList &params,
                                       tlp::Graph *graph, QObject *parent, bool showIcons) {
  ParameterListModel *model = new ParameterListModel(
      params, graph, parent, showIcons,
      tableView->palette().color(tableView->backgroundRole()) != QColor("white"));
  tableView->setModel(model);

  auto hHeader = tableView->horizontalHeader();
  auto vHeader = tableView->verticalHeader();
  hHeader->setVisible(false);
  hHeader->setStretchLastSection(true);
  vHeader->setSectionResizeMode(QHeaderView::Fixed);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  // parameters names are displayed in the vertical header
  vHeader->setVisible(true);
  tableView->setItemDelegate(new TulipItemDelegate(tableView));
#else
  // parameters are displayed in column 0
  vHeader->setVisible(false);
  hHeader->resizeSection(0, model->parameterColumnMinWidth());
  // only values column needs edition
  tableView->setItemDelegateForColumn(1, new TulipItemDelegate(tableView));
  // avoid to show focus on parameters names
  tableView->setFocusPolicy(Qt::NoFocus);
#endif

  return model;
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  return 1;
#else
  return 2;
#endif
}

QVariant ParameterListModel::data(const QModelIndex &index, int role) const {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
  if (index.column() == 0) // parameters names
    return headerData(index.row(), Qt::Vertical, role);
#endif

  if (role == GraphRole)
    return QVariant::fromValue<tlp::Graph *>(_graph);

  const ParameterDescription &info = _params[index.row()];

  if (role == Qt::ToolTipRole) {
    QString displayName;
    // remove prefix if any
    size_t pos = info.getName().find("::");

    if (pos == std::string::npos)
      displayName = tlpStringToQString(info.getName().c_str());
    else
      displayName = tlpStringToQString(info.getName().c_str() + pos + 2);
    return QString("<b>&quot;%1&quot;</b>%2%3")
        .arg(displayName)
        .arg(info.isMandatory() ? "&nbsp;<i>[mandatory]</i>" : "")
        .arg(info.getHelp().c_str());
  } else if (role == Qt::WhatsThisRole)
    return tlp::tlpStringToQString(info.getHelp());
  else if (role == Qt::BackgroundRole) {
    if (info.isMandatory() || !_showIcons)
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
  // use the standard font for a QHeaderView
  if (role == Qt::FontRole) {
    QFont font = QAbstractItemModel::headerData(section, orientation, role).value<QFont>();
    font.setBold(true);
    return QVariant::fromValue<QFont>(font);
  }
#endif
  if (orientation == Qt::Vertical) {
    const ParameterDescription &info = _params[section];

    if (role == Qt::DisplayRole) {
      // remove prefix if any
      size_t pos = info.getName().find("::");

      if (pos != std::string::npos)
        return tlp::tlpStringToQString(info.getName().c_str() + pos + 2);

      return tlp::tlpStringToQString(info.getName().c_str());
    } else if (role == Qt::BackgroundRole) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
      if (info.isMandatory())
        return QColor(255, 255, 222);
      else
        return QColor(222, 255, 222);
#else
      return QAbstractItemModel::headerData(section, orientation, role);
    } else if (role == Qt::ForegroundRole) {
      return _darkBackground ? QColor("white") : QColor("black");
#endif
    } else if (role == Qt::ToolTipRole) {
      return data(createIndex(section, -1), role);
    } else if (role == Qt::DecorationRole && _showIcons) {
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
  if (index.column() == 0) // parameters names
    return Qt::ItemIsEnabled;
#endif
  Qt::ItemFlags result = QAbstractItemModel::flags(index);
  const ParameterDescription &info = _params[index.row()];
  bool editable = info.isEditable();

  return editable ? (result | Qt::ItemIsEditable) : (result & ~Qt::ItemIsEditable);
}

bool ParameterListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (role == Qt::EditRole) {
    const ParameterDescription &info = _params[index.row()];

    DataType *dataType = TulipMetaTypes::qVariantToDataType(value);

    if (dataType)
      _data.setData(info.getName(), dataType);

    return (dataType != nullptr);
  }

  return QAbstractItemModel::setData(index, value, role);
}

DataSet ParameterListModel::parametersValues() const {
  return _data;
}

void ParameterListModel::setParametersValues(const DataSet &data) {
  // update internal data
  // and ensure display refresh
  beginResetModel();
  _data = data;
  endResetModel();
}

QString ParameterListModel::getParameterName(int section) {
  return tlp::tlpStringToQString(_params[section].getName());
}

QString ParameterListModel::getParameterHelp(int section) {
  return tlp::tlpStringToQString(_params[section].getHelp());
}

bool ParameterListModel::isMandatory(int section) {
  return _params[section].isMandatory();
}

int ParameterListModel::parameterColumnMinWidth() {
  int minWidth = 0;

  // we compute the minimum width needed to display the first column
  // when it displays the parameters names
  for (unsigned int i = 0; i < _params.size(); ++i) {
    // the used font
    QFont font = QAbstractItemModel::headerData(i, Qt::Vertical, Qt::FontRole).value<QFont>();
    font.setBold(true);
    QFontMetrics metrics(font);
    // add the width of param name
    int width = metrics.boundingRect(tlpStringToQString(_params[i].getName())).width();
    if (width > minWidth)
      minWidth = width;
  }
  // add icon width + margin spaces
  return minWidth + (_showIcons ? 28 : 5);
}

} // namespace tlp
