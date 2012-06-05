#ifndef GRAPHPROPERTIESMODEL_H
#define GRAPHPROPERTIESMODEL_H

#include <tulip/TulipModel.h>
#include <tulip/Observable.h>
#include <tulip/Graph.h>
#include <tulip/PropertyInterface.h>
#include <QtGui/QFont>
#include <QtGui/QIcon>
#include <QtCore/QSet>

namespace tlp {

template<typename PROPTYPE>
class GraphPropertiesModel : public tlp::TulipModel, public tlp::Observable {
  tlp::Graph* _graph;
  QString _placeholder;
  bool _checkable;
  QSet<int> _checkedIndexes;
  QVector<PropertyInterface*> _properties;

  void rebuildCache();

public:
  explicit GraphPropertiesModel(tlp::Graph* graph, bool checkable=false, QObject *parent = NULL);
  explicit GraphPropertiesModel(QString placeholder, tlp::Graph* graph, bool checkable=false, QObject *parent = NULL);

  tlp::Graph* graph() const {
    return _graph;
  }

  QSet<int> checkedIndexes() const {
    return _checkedIndexes;
  }

  // Methods re-implemented from QAbstractItemModel
  QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role);

  // Methods inherited from the observable system
  void treatEvent(const tlp::Event& evt) {
    const GraphEvent* graphEvent = dynamic_cast<const GraphEvent*>(&evt);
    if (graphEvent == NULL)
      return;
    if (graphEvent->getType() == GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY || graphEvent->getType() == GraphEvent::TLP_BEFORE_DEL_INHERITED_PROPERTY) {
      PropertyInterface* pi = _graph->getProperty(graphEvent->getPropertyName());
      int row = rowOf(pi);
      beginRemoveRows(QModelIndex(),row,row);
      _properties.remove(row);
    }
    else if (graphEvent->getType() == GraphEvent::TLP_AFTER_DEL_LOCAL_PROPERTY || graphEvent->getType() == GraphEvent::TLP_AFTER_DEL_INHERITED_PROPERTY) {
      endRemoveRows();
    }
    else if (graphEvent->getType() == GraphEvent::TLP_ADD_LOCAL_PROPERTY || graphEvent->getType() == GraphEvent::TLP_ADD_INHERITED_PROPERTY) {
      PropertyInterface* pi = _graph->getProperty(graphEvent->getPropertyName());
      rebuildCache();
      int row = rowOf(pi);
      beginInsertRows(QModelIndex(),row,row);
      endInsertRows();
    }
  }

  int rowOf(tlp::PropertyInterface*) const;

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
