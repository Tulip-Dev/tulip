#ifndef GRAPHHIERARCHIESMODEL_H
#define GRAPHHIERARCHIESMODEL_H

#include <QtCore/QAbstractItemModel>

namespace tlp {
class Graph;
}

class GraphHierarchiesModel : public QAbstractItemModel {
  Q_OBJECT

  QList<tlp::Graph *> _graphs;

public:
  explicit GraphHierarchiesModel(QObject *parent=0);

  // Methods re-implemented from QAbstractItemModel
  QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:

public slots:
  void addGraph(tlp::Graph *);
  void removeGraph(tlp::Graph *);
};

#endif // GRAPHHIERARCHIESMODEL_H
