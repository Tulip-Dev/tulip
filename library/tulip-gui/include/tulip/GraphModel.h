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

#ifndef GRAPHMODEL_H
#define GRAPHMODEL_H

#include <QVector>
#include <QSet>
#include <QSortFilterProxyModel>

#include <tulip/Edge.h>
#include <tulip/Observable.h>
#include <tulip/TulipModel.h>

namespace tlp {
class Graph;
class PropertyInterface;
class BooleanProperty;

class TLP_QT_SCOPE GraphModel : public tlp::TulipModel, public Observable {

public:
  static QVariant nodeValue(unsigned int, tlp::PropertyInterface *);
  static QVariant nodeDefaultValue(tlp::PropertyInterface *);
  static bool setNodeValue(unsigned int, tlp::PropertyInterface *, QVariant);
  static bool setNodeDefaultValue(tlp::PropertyInterface *, QVariant);
  static bool setAllNodeValue(tlp::PropertyInterface *, QVariant, Graph *graph = nullptr);
  static QVariant edgeValue(unsigned int, tlp::PropertyInterface *);
  static QVariant edgeDefaultValue(tlp::PropertyInterface *);
  static bool setEdgeValue(unsigned int, tlp::PropertyInterface *, QVariant);
  static bool setEdgeDefaultValue(tlp::PropertyInterface *, QVariant);
  static bool setAllEdgeValue(tlp::PropertyInterface *, QVariant, Graph *graph = nullptr);

  explicit GraphModel(QObject *parent = nullptr);

  virtual void setGraph(tlp::Graph *);
  tlp::Graph *graph() const;

  // Methods re-implemented from QAbstractItemModel
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &child) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role) override;

  void treatEvent(const tlp::Event &) override;
  void treatEvents(const std::vector<tlp::Event> &) override;

  unsigned int elementAt(int) const;
  virtual bool lessThan(unsigned int, unsigned int, tlp::PropertyInterface *) const = 0;
  virtual QString stringValue(unsigned int, tlp::PropertyInterface *) const = 0;
  virtual QVariant value(unsigned int, tlp::PropertyInterface *) const = 0;
  virtual bool isNode() const = 0;

protected:
  tlp::Graph *_graph;
  QVector<unsigned int> _elements;
  QVector<QPair<unsigned int, bool>> _elementsToModify;
  QVector<PropertyInterface *> _properties;
  QSet<PropertyInterface *> _propertiesModified;

  virtual bool setValue(unsigned int, tlp::PropertyInterface *, QVariant) const = 0;
  void addRemoveRowsSequence(QVector<unsigned int> &rowsSequence, bool add);
};

class TLP_QT_SCOPE GraphSortFilterProxyModel : public QSortFilterProxyModel, public Observable {
  QVector<PropertyInterface *> _properties;
  BooleanProperty *_filterProperty;

public:
  GraphSortFilterProxyModel(QObject *parent = nullptr);

  void setFilterProperty(tlp::BooleanProperty *);
  void setSelectedOnly(bool);
  void setProperties(QVector<PropertyInterface *>);

  bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
  bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

  void treatEvent(const tlp::Event &) override;

  tlp::BooleanProperty *filterProperty() const;
};

class TLP_QT_SCOPE NodesGraphModel : public GraphModel {
public:
  NodesGraphModel(QObject *parent = nullptr);
  void setGraph(Graph *) override;

  void treatEvent(const tlp::Event &) override;
  void treatEvents(const std::vector<tlp::Event> &) override;
  bool lessThan(unsigned int, unsigned int, tlp::PropertyInterface *) const override;
  QString stringValue(unsigned int, tlp::PropertyInterface *) const override;
  QVariant value(unsigned int, tlp::PropertyInterface *) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

  bool isNode() const override {
    return true;
  }

  static QString getNodeTooltip(Graph *graph, node n);

protected:
  bool setValue(unsigned int, tlp::PropertyInterface *, QVariant) const override;

private:
  bool _nodesAdded;
  bool _nodesRemoved;
};

class TLP_QT_SCOPE EdgesGraphModel : public GraphModel {
public:
  EdgesGraphModel(QObject *parent = nullptr);
  void setGraph(Graph *) override;
  void treatEvent(const tlp::Event &) override;
  void treatEvents(const std::vector<tlp::Event> &) override;
  bool lessThan(unsigned int, unsigned int, tlp::PropertyInterface *) const override;
  QString stringValue(unsigned int, tlp::PropertyInterface *) const override;
  QVariant value(unsigned int, tlp::PropertyInterface *) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

  bool isNode() const override {
    return false;
  }

  static QString getEdgeTooltip(Graph *graph, edge e);

protected:
  bool setValue(unsigned int, tlp::PropertyInterface *, QVariant) const override;

private:
  bool _edgesAdded;
  bool _edgesRemoved;
};
} // namespace tlp

#endif // GRAPHMODEL_H
///@endcond
