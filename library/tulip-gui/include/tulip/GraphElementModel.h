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
///@cond DOXYGEN_HIDDEN

#ifndef GRAPHELEMENTMODEL_H
#define GRAPHELEMENTMODEL_H

#include <tulip/TulipModel.h>
#include <tulip/GraphModel.h>

namespace tlp {

class TLP_QT_SCOPE GraphElementModel: public TulipModel {

public:

  GraphElementModel(Graph *graph, unsigned int id, QObject *parent=NULL);

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  virtual QString headerText(unsigned int id) const = 0;
  virtual QVariant value(unsigned int id,PropertyInterface *prop) const = 0;

  Qt::ItemFlags flags(const QModelIndex &index) const;

protected :

  Graph *_graph;
  unsigned int _id;

};

class TLP_QT_SCOPE GraphNodeElementModel : public GraphElementModel {
public :

  GraphNodeElementModel(Graph *graph, unsigned int id, QObject *parent=NULL):GraphElementModel(graph,id,parent) {}

  QString headerText(unsigned int id) const {
    return QString("node: ") + QString::number(id);
  }

  QVariant value(unsigned int id, PropertyInterface *prop) const {
    return GraphModel::nodeValue(id,prop);
  }

  bool setData(const QModelIndex &index, const QVariant &value, int role);

};

class TLP_QT_SCOPE GraphEdgeElementModel : public GraphElementModel {
public :

  GraphEdgeElementModel(Graph *graph, unsigned int id, QObject *parent=NULL):GraphElementModel(graph,id,parent) {}

  QString headerText(unsigned int id) const {
    return QString("edge: ") + QString::number(id);
  }

  QVariant value(unsigned int id, PropertyInterface *prop) const {
    return GraphModel::edgeValue(id,prop);
  }

  bool setData(const QModelIndex &index, const QVariant &value, int role);

};

}

#endif // GRAPHELEMENTMODEL_H
///@endcond
