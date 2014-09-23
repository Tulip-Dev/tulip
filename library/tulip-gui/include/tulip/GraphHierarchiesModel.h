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

#ifndef GRAPHHIERARCHIESMODEL_H
#define GRAPHHIERARCHIESMODEL_H

#include <tulip/tulipconf.h>
#include <tulip/TulipModel.h>
#include <tulip/Observable.h>

#include <QList>

namespace tlp {
class Graph;
class GraphNeedsSavingObserver;
class TulipProject;
class PluginProgress;

class TLP_QT_SCOPE GraphHierarchiesModel : public tlp::TulipModel, public tlp::Observable {
  Q_OBJECT

  QList<tlp::Graph *> _graphs;
  QString generateName(tlp::Graph *) const;

  tlp::Graph *_currentGraph;
  QMap<const tlp::Graph*,QModelIndex> _indexCache;
  QMap<const tlp::Graph*, GraphNeedsSavingObserver*> _saveNeeded;
  void initIndexCache(tlp::Graph *root);

public:
  bool needsSaving();

  explicit GraphHierarchiesModel(QObject *parent=0);
  GraphHierarchiesModel(const GraphHierarchiesModel &);
  virtual ~GraphHierarchiesModel();

  // Allows the model to behave like a list and to be iterable
  typedef QList<tlp::Graph *>::iterator iterator;
  typedef QList<tlp::Graph *>::const_iterator const_iterator;
  tlp::Graph *operator[](int i) const {
    return _graphs[i];
  }
  tlp::Graph *operator[](int i) {
    return _graphs[i];
  }
  int size() const {
    return _graphs.size();
  }

  iterator begin() {
    return _graphs.begin();
  }
  iterator end() {
    return _graphs.end();
  }
  const_iterator begin() const {
    return _graphs.begin();
  }
  const_iterator end() const {
    return _graphs.end();
  }

  QList<tlp::Graph *> graphs() const {
    return _graphs;
  }
  bool empty() const {
    return _graphs.isEmpty();
  }

  // Methods re-implemented from QAbstractItemModel
  QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role);
  Qt::ItemFlags flags(const QModelIndex &index) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  QMimeData* mimeData(const QModelIndexList &indexes) const;

  QModelIndex indexOf(const Graph *);
  QModelIndex forceGraphIndex(Graph *);

  // Methods inherited from the observable system
  void treatEvent(const tlp::Event &);

  // active graph handling
  void setCurrentGraph(tlp::Graph *);
  tlp::Graph *currentGraph() const;

signals:
  void currentGraphChanged(tlp::Graph *);

public slots:
  void addGraph(tlp::Graph *);
  void removeGraph(tlp::Graph *);

  QMap<QString,tlp::Graph*> readProject(tlp::TulipProject *,tlp::PluginProgress *);
  QMap<tlp::Graph*,QString> writeProject(tlp::TulipProject *, tlp::PluginProgress *);

};

}

#endif // GRAPHHIERARCHIESMODEL_H
///@endcond
