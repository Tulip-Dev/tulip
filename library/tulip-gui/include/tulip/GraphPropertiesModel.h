#ifndef GRAPHPROPERTIESMODEL_H
#define GRAPHPROPERTIESMODEL_H

#include <tulip/TulipModel.h>
#include <tulip/Observable.h>
#include <tulip/Graph.h>
#include <tulip/PropertyInterface.h>
#include <QtGui/QFont>
#include <QtCore/QSet>

namespace tlp {

template<typename PROPTYPE>
class GraphPropertiesModel : public tlp::TulipModel, public tlp::Observable {
  tlp::Graph* _graph;

public:
  explicit GraphPropertiesModel(tlp::Graph* graph, QObject *parent = 0);

  // Methods re-implemented from QAbstractItemModel
  QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  // Methods inherited from the observable system
  void treatEvent(const tlp::Event& evt) {
    const GraphEvent* graphEvent = dynamic_cast<const GraphEvent*>(&evt);

    if (graphEvent == NULL)
      return;

    if (graphEvent->getType() == GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY || graphEvent->getType() == GraphEvent::TLP_BEFORE_DEL_INHERITED_PROPERTY) {
      PropertyInterface* pi = _graph->getProperty(graphEvent->getPropertyName());
      int row = rowOf(pi);
      beginRemoveRows(QModelIndex(),row,row);
    }
    else if (graphEvent->getType() == GraphEvent::TLP_AFTER_DEL_LOCAL_PROPERTY || graphEvent->getType() == GraphEvent::TLP_AFTER_DEL_INHERITED_PROPERTY) {
      endRemoveRows();
    }
    else if (graphEvent->getType() == GraphEvent::TLP_ADD_LOCAL_PROPERTY || graphEvent->getType() == GraphEvent::TLP_ADD_INHERITED_PROPERTY) {
      PropertyInterface* pi = _graph->getProperty(graphEvent->getPropertyName());
      int row = rowOf(pi);
      beginInsertRows(QModelIndex(),row,row);
      endInsertRows();
    }
  }

  int rowOf(tlp::PropertyInterface*) const;
};

}

#include "cxx/GraphPropertiesModel.cxx"


#endif // GRAPHPROPERTIESMODEL_H
