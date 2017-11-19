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

#ifndef GRAPHELEMENTMODEL_H
#define GRAPHELEMENTMODEL_H

#include <tulip/TulipModel.h>
#include <tulip/GraphModel.h>

namespace tlp {

class TLP_QT_SCOPE GraphElementModel : public TulipModel {

public:
  GraphElementModel(Graph *graph, unsigned int id, QObject *parent = nullptr,
                    bool displayvisual = true);

  void setShowVisualProp(bool show);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &child) const override;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  virtual QString headerText(unsigned int id) const = 0;
  virtual QVariant value(unsigned int id, PropertyInterface *prop) const = 0;

  Qt::ItemFlags flags(const QModelIndex &index) const override;

protected:
  Graph *_graph;
  unsigned int _id;
  bool _displayvisualprop;
};

class TLP_QT_SCOPE GraphNodeElementModel : public GraphElementModel {

public:
  GraphNodeElementModel(Graph *graph, unsigned int id, QObject *parent = nullptr,
                        bool displayvisual = true)
      : GraphElementModel(graph, id, parent, displayvisual) {}

  QString headerText(unsigned int id) const override {
    return QString("node: ") + QString::number(id);
  }

  QVariant value(unsigned int id, PropertyInterface *prop) const override {
    return GraphModel::nodeValue(id, prop);
  }

  bool setData(const QModelIndex &index, const QVariant &value, int role) override;
};

class TLP_QT_SCOPE GraphEdgeElementModel : public GraphElementModel {

public:
  GraphEdgeElementModel(Graph *graph, unsigned int id, QObject *parent = nullptr,
                        bool displayvisual = true)
      : GraphElementModel(graph, id, parent, displayvisual) {}

  QString headerText(unsigned int id) const override {
    return QString("edge: ") + QString::number(id);
  }

  QVariant value(unsigned int id, PropertyInterface *prop) const override {
    return GraphModel::edgeValue(id, prop);
  }

  bool setData(const QModelIndex &index, const QVariant &value, int role) override;
};
}

#endif // GRAPHELEMENTMODEL_H
///@endcond
