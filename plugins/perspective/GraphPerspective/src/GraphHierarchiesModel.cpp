#include "GraphHierarchiesModel.h"

#include <tulip/Graph.h>
#include <QtGui/QFont>
#include <QtCore/QSize>

//FIXME: remove me
#include <QtCore/QDebug>

using namespace std;
using namespace tlp;

#define NAME_SECTION 0
#define ID_SECTION 1
#define NODES_SECTION 2
#define EDGES_SECTION 3

GraphHierarchiesModel::GraphHierarchiesModel(QObject *parent): QAbstractItemModel(parent) {
}

GraphHierarchiesModel::GraphHierarchiesModel(const GraphHierarchiesModel &copy): QAbstractItemModel(copy.QObject::parent()), tlp::Observable() {
  for (int i=0;i < copy.size();++i)
    addGraph(copy[i]);
}

GraphHierarchiesModel::~GraphHierarchiesModel() {
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

    for (int i=0; i<ancestor->numberOfSubGraphs(); i++) {
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
  emit layoutAboutToBeChanged();
  if (_graphs.contains(g))
    return;
  Graph *i;
  foreach(i,_graphs) {
    if (i->isDescendantGraph(g))
      return;
  }
  _graphs.push_back(g);
  g->addListener(this);
  emit layoutChanged();
}

void GraphHierarchiesModel::removeGraph(tlp::Graph *g) {
  emit layoutAboutToBeChanged();
  if (_graphs.contains(g))
    _graphs.removeAll(g);
  emit layoutChanged();
}

void GraphHierarchiesModel::treatEvent(const Event &e) {
  Graph *g = dynamic_cast<tlp::Graph *>(e.sender());
  assert(g);
  if (e.type() == Event::TLP_DELETE) {
    emit layoutAboutToBeChanged();
    _graphs.removeAll(g);
    emit layoutChanged();
  }
  else if (e.type() == Event::TLP_MODIFICATION) {
    const GraphEvent *ge = dynamic_cast<const tlp::GraphEvent *>(&e);
    if (!ge)
      return;
    if (ge->getType() == GraphEvent::TLP_ADD_DESCENDANTGRAPH || ge->getType() == GraphEvent::TLP_DEL_DESCENDANTGRAPH)
      emit layoutChanged();
  }
}
