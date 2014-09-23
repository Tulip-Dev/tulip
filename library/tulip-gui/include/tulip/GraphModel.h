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

#ifndef GRAPHMODEL_H
#define GRAPHMODEL_H

#include <QVector>
#include <QSortFilterProxyModel>


#include <tulip/Observable.h>
#include <tulip/TulipModel.h>

namespace tlp {
class Graph;
class PropertyInterface;
class BooleanProperty;

class TLP_QT_SCOPE GraphModel : public tlp::TulipModel, public Observable {
  tlp::Graph* _graph;

public:
  static QVariant nodeValue(unsigned int, tlp::PropertyInterface*);
  static QVariant nodeDefaultValue(tlp::PropertyInterface*);
  static bool setNodeValue(unsigned int,tlp::PropertyInterface*, QVariant);
  static bool setAllNodeValue(tlp::PropertyInterface*, QVariant);
  static QVariant edgeValue(unsigned int, tlp::PropertyInterface*);
  static QVariant edgeDefaultValue(tlp::PropertyInterface*);
  static bool setEdgeValue(unsigned int,tlp::PropertyInterface*, QVariant);
  static bool setAllEdgeValue(tlp::PropertyInterface*, QVariant);

  explicit GraphModel(QObject *parent = NULL);

  virtual void setGraph(tlp::Graph*);
  tlp::Graph* graph() const;

  // Methods re-implemented from QAbstractItemModel
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role);

  void treatEvent(const tlp::Event&);
  void treatEvents(const std::vector<tlp::Event> &);

  unsigned int elementAt(int) const;
  virtual bool lessThan(unsigned int,unsigned int,tlp::PropertyInterface*) const = 0;
  virtual QString stringValue(unsigned int,tlp::PropertyInterface*) const = 0;
  virtual QVariant value(unsigned int,tlp::PropertyInterface*) const = 0;
  virtual bool isNode() const = 0;

protected:
  QVector<unsigned int> _elements;
  QVector<QPair<unsigned int,bool> > _elementsToModify;
  QVector<PropertyInterface*> _properties;

  virtual bool setValue(unsigned int,tlp::PropertyInterface*,QVariant) const = 0;
};

class TLP_QT_SCOPE GraphSortFilterProxyModel: public QSortFilterProxyModel, public Observable {
  QVector<PropertyInterface*> _properties;
  BooleanProperty* _filterProperty;

public:
  GraphSortFilterProxyModel(QObject* parent = NULL);

  void setFilterProperty(tlp::BooleanProperty*);
  void setSelectedOnly(bool);
  void setProperties(QVector<PropertyInterface*>);

  bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
  bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

  void treatEvent(const tlp::Event&);

  tlp::BooleanProperty* filterProperty() const;
};

class TLP_QT_SCOPE NodesGraphModel: public GraphModel {
public:
  NodesGraphModel(QObject* parent = NULL);
  void setGraph(Graph *);

  void treatEvent(const tlp::Event &);
  virtual bool lessThan(unsigned int,unsigned int,tlp::PropertyInterface*) const;
  virtual QString stringValue(unsigned int,tlp::PropertyInterface*) const;
  virtual QVariant value(unsigned int,tlp::PropertyInterface*) const;
  bool isNode() const {
    return true;
  }

protected:
  virtual bool setValue(unsigned int,tlp::PropertyInterface*,QVariant) const;
};

class TLP_QT_SCOPE EdgesGraphModel: public GraphModel {
public:
  EdgesGraphModel(QObject* parent = NULL);
  void setGraph(Graph *);
  void treatEvent(const tlp::Event &);
  virtual bool lessThan(unsigned int,unsigned int,tlp::PropertyInterface*) const;
  virtual QString stringValue(unsigned int,tlp::PropertyInterface*) const;
  virtual QVariant value(unsigned int,tlp::PropertyInterface*) const;

  bool isNode() const {
    return false;
  }
protected:
  virtual bool setValue(unsigned int,tlp::PropertyInterface*,QVariant) const;
};

}

#endif // GRAPHMODEL_H
///@endcond
