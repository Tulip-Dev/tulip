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

#ifndef GRAPHPROPERTIESMODEL_H
#define GRAPHPROPERTIESMODEL_H

#include <tulip/TulipModel.h>
#include <tulip/Observable.h>
#include <tulip/Graph.h>

#include <QFont>
#include <QIcon>
#include <QSet>

namespace tlp {

template <typename PROPTYPE>
class GraphPropertiesModel : public tlp::TulipModel, public tlp::Observable {
  tlp::Graph *_graph;
  QString _placeholder;
  bool _checkable;
  QSet<PROPTYPE *> _checkedProperties;
  QVector<PROPTYPE *> _properties;
  bool _removingRows;
  bool forcingRedraw;

  void rebuildCache();

public:
  explicit GraphPropertiesModel(tlp::Graph *graph, bool checkable = false,
                                QObject *parent = nullptr);
  explicit GraphPropertiesModel(QString placeholder, tlp::Graph *graph, bool checkable = false,
                                QObject *parent = nullptr);
  ~GraphPropertiesModel() override {
    if (_graph != nullptr)
      _graph->removeListener(this);
  }

  tlp::Graph *graph() const {
    return _graph;
  }

  void setGraph(tlp::Graph *graph) {
    if (_graph == graph)
      return;

    beginResetModel();

    if (_graph != nullptr)
      _graph->removeListener(this);

    _graph = graph;

    if (_graph != nullptr)
      _graph->addListener(this);

    rebuildCache();
    endResetModel();
  }

  QSet<PROPTYPE *> checkedProperties() const {
    return _checkedProperties;
  }

  // Methods re-implemented from QAbstractItemModel
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &child) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  bool setData(const QModelIndex &quiindex, const QVariant &value, int role) override;

  // Methods inherited from the observable system
  void treatEvent(const tlp::Event &evt) override {
    if (evt.type() == Event::TLP_DELETE) {
      // calls to *ResetModel() functions below
      // are not needed because they may cause a Free Memory Read.
      // However the current model will be soon deleted
      // beginResetModel();
      _graph = nullptr;
      _properties.clear();
      // endResetModel();
      return;
    }

    const GraphEvent *graphEvent = dynamic_cast<const GraphEvent *>(&evt);

    if (graphEvent == nullptr)
      return;

    if (graphEvent->getType() == GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY ||
        graphEvent->getType() == GraphEvent::TLP_BEFORE_DEL_INHERITED_PROPERTY) {

      PROPTYPE *prop = dynamic_cast<PROPTYPE *>(_graph->getProperty(graphEvent->getPropertyName()));

      if (prop != nullptr) {
        int row = rowOf(prop);
        if (row > -1) {
          beginRemoveRows(QModelIndex(), row, row);
          _properties.remove(_properties.indexOf(prop));
          _removingRows = true;
          _checkedProperties.remove(prop);
        }
      }
    } else if (graphEvent->getType() == GraphEvent::TLP_AFTER_DEL_LOCAL_PROPERTY ||
               graphEvent->getType() == GraphEvent::TLP_AFTER_DEL_INHERITED_PROPERTY) {
      if (_removingRows) {
        endRemoveRows();
        _removingRows = false;
      }
    } else if (graphEvent->getType() == GraphEvent::TLP_ADD_LOCAL_PROPERTY ||
               graphEvent->getType() == GraphEvent::TLP_ADD_INHERITED_PROPERTY) {
      PROPTYPE *prop = dynamic_cast<PROPTYPE *>(_graph->getProperty(graphEvent->getPropertyName()));

      if (prop != nullptr) {
        rebuildCache();
        int row = rowOf(prop);

        if (row > -1) {
          beginInsertRows(QModelIndex(), row, row);
          endInsertRows();
        }
      }
    } else if (graphEvent->getType() == GraphEvent::TLP_AFTER_RENAME_LOCAL_PROPERTY) {
      // force any needed sorting
      emit layoutAboutToBeChanged();
      changePersistentIndex(createIndex(0, 0), createIndex(_properties.size() - 1, 0));
      emit layoutChanged();
    }
  }

  int rowOf(PROPTYPE *) const;

  int rowOf(const QString &pName) const;

  Qt::ItemFlags flags(const QModelIndex &index) const override {
    Qt::ItemFlags result = QAbstractItemModel::flags(index);

    if (index.column() == 0 && _checkable)
      result |= Qt::ItemIsUserCheckable;

    return result;
  }
};
} // namespace tlp

#include "cxx/GraphPropertiesModel.cxx"

#endif // GRAPHPROPERTIESMODEL_H
///@endcond
