/*
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
///@cond DOXYGEN_HIDDEN

#ifndef PARAMETERLISTMODEL_H
#define PARAMETERLISTMODEL_H

#include <vector>

#include <tulip/TulipModel.h>
#include <tulip/WithParameter.h>
#include <tulip/DataSet.h>

namespace tlp {

class TLP_QT_SCOPE ParameterListModel : public TulipModel {
  std::vector<ParameterDescription> _params;
  tlp::DataSet _data;
  tlp::Graph *_graph;

public:
  explicit ParameterListModel(const tlp::ParameterDescriptionList &params,
                              tlp::Graph *graph = nullptr, QObject *parent = nullptr);
  tlp::DataSet parametersValues() const;
  void setParametersValues(const tlp::DataSet &data);

  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &child) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role) override;
};
}

#endif // PARAMETERLISTMODEL_H
///@endcond
