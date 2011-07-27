#include "GraphHierarchiesModel.h"

#include <tulip/Graph.h>
#include <QtGui/QFont>
#include <QtCore/QSize>
using namespace tlp;

#define NAME_SECTION 0
#define ID_SECTION 1
#define NODES_SECTION 2
#define EDGES_SECTION 3

GraphHierarchiesModel::GraphHierarchiesModel(QObject *parent): QAbstractItemModel(parent) {
}

QModelIndex GraphHierarchiesModel::index(int row, int column, const QModelIndex &parent) const {
  Graph *g;
  if (parent.isValid())
    g = ((Graph *)(parent.internalPointer()))->getNthSubGraph(row);
  else
    g = _graphs[row];
  return createIndex(row,column,g);
}

QModelIndex GraphHierarchiesModel::parent(const QModelIndex &child) const {
  Graph *graph = (Graph *)(child.internalPointer());

  if (_graphs.contains(graph))
    return QModelIndex();

  int row=0;
  Graph *parent = graph->getSuperGraph();
  if (_graphs.contains(parent))
    row=_graphs.indexOf(parent);

  else {
    Graph *ancestor = parent->getSuperGraph();
    for (int i=0;i<ancestor->numberOfSubGraphs();i++) {
      if (ancestor->getNthSubGraph(i) == parent)
        break;
      row++;
    }
  }


  return createIndex(row,0,parent);
}

int GraphHierarchiesModel::rowCount(const QModelIndex &parent) const {
  if (!parent.isValid())
    return _graphs.size();
  return ((Graph *)(parent.internalPointer()))->numberOfSubGraphs();
}

int GraphHierarchiesModel::columnCount(const QModelIndex &parent) const {
  return 4;
}

QVariant GraphHierarchiesModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole || role == Qt::EditRole) {
    Graph *graph = (Graph *)(index.internalPointer());
    if (index.column() == NAME_SECTION) {
      std::string name;
      graph->getAttribute<std::string>("name",name);
      if (name == "")
        return trUtf8("untitled_") + QString::number(graph->getId());
      return name.c_str();
    }
    else if (index.column() == ID_SECTION)
      return graph->getId();
    else if (index.column() == NODES_SECTION)
      return graph->numberOfNodes();
    else if (index.column() == EDGES_SECTION)
      return graph->numberOfEdges();
  }

  else if (role == Qt::TextAlignmentRole && index.column() != NAME_SECTION)
    return Qt::AlignCenter;

  return QVariant();
}

QVariant GraphHierarchiesModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (orientation == Qt::Horizontal) {
    if (role == Qt::DisplayRole) {
      if (section == NAME_SECTION)
        return trUtf8("Name");
      else if (section == ID_SECTION)
        return trUtf8("Id");
      else if (section == NODES_SECTION)
        return trUtf8("#N");
      else if (section == EDGES_SECTION)
        return trUtf8("#E");
    }

    else if (role == Qt::TextAlignmentRole && section != NAME_SECTION)
      return Qt::AlignCenter;

    else if (role == Qt::FontRole) {
      QFont f;
      f.setBold(true);
      f.setPointSize(f.pointSize() - 1);
      return f;
    }
  }
  return QAbstractItemModel::headerData(section,orientation,role);
}

void GraphHierarchiesModel::addGraph(tlp::Graph *g) {
  if (_graphs.contains(g))
    return;
  Graph *i;
  foreach(i,_graphs)
    if (i->isDescendantGraph(g))
      return;
  _graphs.push_back(g);
  emit dataChanged(createIndex(_graphs.size()-1,0,g),createIndex(_graphs.size()-1,3,g));
}

void GraphHierarchiesModel::removeGraph(tlp::Graph *g) {
  if (_graphs.contains(g))
    _graphs.removeAll(g);
  emit dataChanged(createIndex(0,0,_graphs[0]),createIndex(_graphs.size()-1,3,_graphs[_graphs.size()-1]));
}
