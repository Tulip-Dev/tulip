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


#ifndef GRAPHPROPERTIESMODEL_H
#define GRAPHPROPERTIESMODEL_H

#include <tulip/TulipModel.h>
#include <tulip/Observable.h>
#include <tulip/Graph.h>

#include <QFont>
#include <QIcon>
#include <QSet>

namespace tlp {

template<typename PROPTYPE>
class GraphPropertiesModel : public tlp::TulipModel, public tlp::Observable {
  tlp::Graph* _graph;
  QString _placeholder;
  bool _checkable;
  QSet<PROPTYPE*> _checkedProperties;
  QVector<PROPTYPE*> _properties;
  bool _removingRows;
  bool forcingRedraw;

  void rebuildCache();

public:
  explicit GraphPropertiesModel(tlp::Graph* graph, bool checkable=false, QObject *parent = NULL);
  explicit GraphPropertiesModel(QString placeholder, tlp::Graph* graph, bool checkable=false, QObject *parent = NULL);
  virtual ~GraphPropertiesModel() {}

  tlp::Graph* graph() const {
    return _graph;
  }

  void setGraph(tlp::Graph* graph) {
    if (_graph == graph)
      return;

    beginResetModel();

    if (_graph != NULL)
      _graph->removeListener(this);

    _graph = graph;

    if (_graph != NULL)
      _graph->addListener(this);

    rebuildCache();
    endResetModel();
  }

  QSet<PROPTYPE*> checkedProperties() const {
    return _checkedProperties;
  }

  // Methods re-implemented from QAbstractItemModel
  QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  bool setData(const QModelIndex &quiindex, const QVariant &value, int role);

  // Methods inherited from the observable system
  void treatEvent(const tlp::Event& evt) {
    if (evt.type() == Event::TLP_DELETE) {
      beginResetModel();
      _graph = NULL;
      _properties.clear();
      endResetModel();
      return;
    }

    const GraphEvent* graphEvent = dynamic_cast<const GraphEvent*>(&evt);

    if (graphEvent == NULL)
      return;

    if (graphEvent->getType() == GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY || graphEvent->getType() == GraphEvent::TLP_BEFORE_DEL_INHERITED_PROPERTY) {

      PROPTYPE* prop = dynamic_cast<PROPTYPE*>(_graph->getProperty(graphEvent->getPropertyName()));

      if (prop != NULL) {
        int row = rowOf(prop);
        beginRemoveRows(QModelIndex(),row,row);
        _properties.remove(_properties.indexOf(prop));
        _removingRows = true;
        _checkedProperties.remove(prop);
      }
    }
    else if (graphEvent->getType() == GraphEvent::TLP_AFTER_DEL_LOCAL_PROPERTY || graphEvent->getType() == GraphEvent::TLP_AFTER_DEL_INHERITED_PROPERTY) {
      if (_removingRows) {
        endRemoveRows();
        _removingRows = false;
      }
    }
    else if (graphEvent->getType() == GraphEvent::TLP_ADD_LOCAL_PROPERTY || graphEvent->getType() == GraphEvent::TLP_ADD_INHERITED_PROPERTY) {
      PROPTYPE* prop = dynamic_cast<PROPTYPE*>(_graph->getProperty(graphEvent->getPropertyName()));

      if (prop != NULL) {
        rebuildCache();
        int row = rowOf(prop);

        if (row >= 0) {
          beginInsertRows(QModelIndex(),row,row);
          endInsertRows();
        }
      }
    }
    else if (graphEvent->getType() == GraphEvent::TLP_AFTER_RENAME_LOCAL_PROPERTY) {
      // force any needed sorting
      emit layoutAboutToBeChanged();
      changePersistentIndex(createIndex(0, 0),
                            createIndex(_properties.size() - 1, 0));
      emit layoutChanged();
    }
  }

  int rowOf(PROPTYPE*) const;

  int rowOf(const QString& pName) const;

  Qt::ItemFlags flags(const QModelIndex &index) const {
    Qt::ItemFlags result = QAbstractItemModel::flags(index);

    if (index.column() == 0 && _checkable)
      result |= Qt::ItemIsUserCheckable;

    return result;
  }
};

}

#include "cxx/GraphPropertiesModel.cxx"


#endif // GRAPHPROPERTIESMODEL_H
///@endcond
