/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include "GraphHierarchiesModel.h"

#include <tulip/Graph.h>
#include <QtGui/QFont>
#include <QtCore/QSize>
#include <QtCore/QDebug>
#include <fstream>

#include <tulip/TulipProject.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/TulipSettings.h>

using namespace std;
using namespace tlp;

#define NAME_SECTION 0
#define ID_SECTION 1
#define NODES_SECTION 2
#define EDGES_SECTION 3

void GraphHierarchiesModel::setApplicationDefaults(tlp::Graph *g) {
  const std::string shapes = "viewShape", colors = "viewColor", sizes = "viewSize", metrics = "viewMetric";

  if (!g->existProperty(shapes)) {
    g->getProperty<IntegerProperty>(shapes)->setAllNodeValue(TulipSettings::instance().defaultShape(tlp::NODE));
    g->getProperty<IntegerProperty>(shapes)->setAllEdgeValue(TulipSettings::instance().defaultShape(tlp::EDGE));
  }

  if (!g->existProperty(colors)) {
    g->getProperty<ColorProperty>(colors)->setAllNodeValue(TulipSettings::instance().defaultColor(tlp::NODE));
    g->getProperty<ColorProperty>(colors)->setAllEdgeValue(TulipSettings::instance().defaultColor(tlp::EDGE));
  }

  if (!g->existProperty(sizes)) {
    g->getProperty<SizeProperty>(sizes)->setAllNodeValue(TulipSettings::instance().defaultSize(tlp::NODE));
    g->getProperty<SizeProperty>(sizes)->setAllEdgeValue(TulipSettings::instance().defaultSize(tlp::EDGE));
  }

  if (!g->existProperty(metrics)) {
    g->getProperty<DoubleProperty>(metrics)->setAllNodeValue(0);
    g->getProperty<DoubleProperty>(metrics)->setAllEdgeValue(0);
  }
}

GraphHierarchiesModel::GraphHierarchiesModel(QObject *parent): QAbstractItemModel(parent) {
}

GraphHierarchiesModel::GraphHierarchiesModel(const GraphHierarchiesModel &copy): QAbstractItemModel(copy.QObject::parent()), tlp::Observable() {
  for (int i=0; i < copy.size(); ++i)
    addGraph(copy[i]);
}

GraphHierarchiesModel::~GraphHierarchiesModel() {
}

QModelIndex GraphHierarchiesModel::index(int row, int column, const QModelIndex &parent) const {
  Graph *g = NULL;
  if (parent.isValid())
    g = ((Graph *)(parent.internalPointer()))->getNthSubGraph(row);
  else if (row < _graphs.size())
    g = _graphs[row];

  if (g == NULL)
    return QModelIndex();

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

  Graph* parentGraph = ((Graph *)(parent.internalPointer()));
  return parentGraph->numberOfSubGraphs();
}

int GraphHierarchiesModel::columnCount(const QModelIndex &parent) const {
  return 4;
}

bool GraphHierarchiesModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (index.column() == NAME_SECTION) {
    Graph *graph = (Graph *)(index.internalPointer());
    graph->setAttribute("name",value.toString().toStdString());
    return true;
  }
  return QAbstractItemModel::setData(index,value,role);
}

QVariant GraphHierarchiesModel::data(const QModelIndex &index, int role) const {
  Graph *graph = (Graph *)(index.internalPointer());
  const_cast<GraphHierarchiesModel*>(this)->_indexCache[graph] = index;

  if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::ToolTipRole) {
    if (index.column() == NAME_SECTION)
      return generateName(graph);
    else if (index.column() == ID_SECTION)
      return graph->getId();
    else if (index.column() == NODES_SECTION)
      return graph->numberOfNodes();
    else if (index.column() == EDGES_SECTION)
      return graph->numberOfEdges();
  }

  else if (role == Qt::TextAlignmentRole && index.column() != NAME_SECTION)
    return Qt::AlignCenter;

  else if (role == Qt::FontRole) {
    QFont f;

    if (graph == _currentGraph)
      f.setBold(true);

    return f;
  }

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
        return trUtf8("N");
      else if (section == EDGES_SECTION)
        return trUtf8("E");
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

  beginInsertRows(QModelIndex(),rowCount(),rowCount());
  Graph *i;
  foreach(i,_graphs) {
    if (i->isDescendantGraph(g))
      return;
  }
  _graphs.push_back(g);
  if (_graphs.size() == 1)
    setCurrentGraph(g);

  setApplicationDefaults(g);
  g->addListener(this);
  endInsertRows();
}

void GraphHierarchiesModel::removeGraph(tlp::Graph *g) {
  if (_graphs.contains(g)) {
    int pos = _graphs.indexOf(g);
    beginRemoveRows(QModelIndex(),pos,pos);
    _graphs.removeAll(g);
    endRemoveRows();
  }
}

void GraphHierarchiesModel::treatEvent(const Event &e) {
  Graph *g = dynamic_cast<tlp::Graph *>(e.sender());
  assert(g);

  if (e.type() == Event::TLP_DELETE && _graphs.contains(g)) { // A root graph has been deleted
    int pos = _graphs.indexOf(g);
    beginRemoveRows(QModelIndex(),pos,pos);
    _graphs.removeAll(g);
    endRemoveRows();
  }
  else if (e.type() == Event::TLP_MODIFICATION) {
    const GraphEvent *ge = dynamic_cast<const tlp::GraphEvent *>(&e);
    if (!ge)
      return;
    if (ge->getType() == GraphEvent::TLP_DEL_DESCENDANTGRAPH && _graphs.contains(ge->getGraph()->getRoot())) {
      const Graph* sg = ge->getSubGraph();
      if (_indexCache.contains(sg)) {
        QModelIndex index = _indexCache[sg];
        QModelIndex parentIndex = index.parent();
        int row = index.row();
        beginRemoveRows(parentIndex,row,row);
        _indexCache.remove(sg);
        endRemoveRows();
      }
    }
  }
}

static QString GRAPHS_PATH("/graphs/");
static QString GRAPH_FOLDER_PREFIX("graph_");
static QString GRAPH_FILE("graph.tlp");

void GraphHierarchiesModel::readProject(tlp::TulipProject *project, tlp::PluginProgress *progress) {
  if (!project->exists(GRAPHS_PATH) || !project->isDir(GRAPHS_PATH))
    return;

  QString e;
  foreach (e,project->entryList(GRAPHS_PATH,QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs)) {
    DataSet dataSet;
    dataSet.set<std::string>("file::filename", project->toAbsolutePath(GRAPHS_PATH + "/" + e + "/" + GRAPH_FILE).toStdString());
    Graph *g = tlp::importGraph("tlp",dataSet,progress);

    if (g)
      addGraph(g);
  }
}

bool GraphHierarchiesModel::writeProject(tlp::TulipProject *project, tlp::PluginProgress *progress) {
  if (project->exists(GRAPHS_PATH) && !project->isDir(GRAPHS_PATH))
    return false;

  project->removeAllDir(GRAPHS_PATH);

  int i=0;
  Graph *g;
  foreach (g,_graphs) {
    QString path = GRAPHS_PATH + "graph_" + QString::number(i++);
    project->mkpath(path);
    std::fstream *file = project->stdFileStream(path + "/" + GRAPH_FILE);
    DataSet dataSet;
    tlp::exportGraph(g, *file, "tlp", dataSet, progress);
    delete file;
  }

  return true;
}

QString GraphHierarchiesModel::generateName(tlp::Graph *graph) const {
  std::string name;
  graph->getAttribute<std::string>("name",name);

  if (name == "")
    return trUtf8("graph_") + QString::number(graph->getId());

  return name.c_str();
}

void GraphHierarchiesModel::setCurrentGraph(tlp::Graph *g) {
  bool inHierarchy = false;
  foreach(Graph *i,_graphs) {
    if (i->isDescendantGraph(g) || g == i) {
      inHierarchy = true;
      break;
    }
  }

  if (!inHierarchy)
    return;

  _currentGraph = g;
  emit currentGraphChanged(g);
  emit layoutChanged();
}

Graph *GraphHierarchiesModel::currentGraph() const {
  return _currentGraph;
}

Qt::ItemFlags GraphHierarchiesModel::flags(const QModelIndex &index) const {
  Qt::ItemFlags result = QAbstractItemModel::flags(index);
  if (index.column() == 0)
    result |= Qt::ItemIsEditable;
  return result;
}
