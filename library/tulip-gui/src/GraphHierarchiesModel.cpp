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
#include "tulip/GraphHierarchiesModel.h"

#include <QtGui/QFont>
#include <QtCore/QSize>
#include <QtCore/QDebug>
#include <QtCore/QMimeData>
#include <fstream>

#include <tulip/TlpTools.h>
#include <tulip/TulipMetaTypes.h>
#include <tulip/Graph.h>
#include <tulip/TulipProject.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/TulipSettings.h>
#include <tulip/TulipMimes.h>

using namespace std;
using namespace tlp;

#define NAME_SECTION 0
#define ID_SECTION 1
#define NODES_SECTION 2
#define EDGES_SECTION 3

void GraphHierarchiesModel::setApplicationDefaults(tlp::Graph *g) {
  const std::string shapes = "viewShape", colors = "viewColor", sizes = "viewSize", metrics = "viewMetric", fonts = "viewFont", fontSizes = "viewFontSize";

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

  if (!g->existProperty(fonts)) {
    g->getProperty<StringProperty>(fonts)->setAllNodeValue(tlp::TulipBitmapDir + "font.ttf");
    g->getProperty<StringProperty>(fonts)->setAllEdgeValue(tlp::TulipBitmapDir + "font.ttf");
  }

  if (!g->existProperty(fontSizes)) {
    g->getProperty<IntegerProperty>(fontSizes)->setAllNodeValue(18);
    g->getProperty<IntegerProperty>(fontSizes)->setAllEdgeValue(18);
  }
}

GraphHierarchiesModel::GraphHierarchiesModel(QObject *parent): TulipModel(parent), _currentGraph(NULL) {
}

GraphHierarchiesModel::GraphHierarchiesModel(const GraphHierarchiesModel &copy): TulipModel(copy.QObject::parent()), tlp::Observable() {
  for (int i=0; i < copy.size(); ++i)
    addGraph(copy[i]);

  _currentGraph = NULL;
}

GraphHierarchiesModel::~GraphHierarchiesModel() {
}

QModelIndex GraphHierarchiesModel::index(int row, int column, const QModelIndex &parent) const {
  if (!hasIndex(row,column,parent))
    return QModelIndex();

  Graph *g = NULL;

  if (parent.isValid())
    g = ((Graph *)(parent.internalPointer()))->getNthSubGraph(row);
  else
    g = _graphs[row];

  if (g == NULL) {
    return QModelIndex();
  }

  return createIndex(row,column,g);
}

QModelIndex GraphHierarchiesModel::parent(const QModelIndex &child) const {
  if (!child.isValid())
    return QModelIndex();

  Graph* childGraph = (Graph*)(child.internalPointer());

  if (childGraph == NULL || _graphs.contains(childGraph) || childGraph->getSuperGraph() == childGraph) {
    return QModelIndex();
  }

  int row=0;
  Graph *parent = childGraph->getSuperGraph();

  if (_graphs.contains(parent))
    row=_graphs.indexOf(parent);
  else {
    Graph *ancestor = parent->getSuperGraph();

    for (unsigned int i=0; i<ancestor->numberOfSubGraphs(); i++) {
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

  if (parent.column() != 0)
    return 0;

  Graph* parentGraph = ((Graph *)(parent.internalPointer()));
  return parentGraph->numberOfSubGraphs();
}

int GraphHierarchiesModel::columnCount(const QModelIndex&) const {
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

  if (index.column() == 0)
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

  else if (role == GraphRole) {
    return QVariant::fromValue<Graph*>((Graph*)(index.internalPointer()));
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
        return trUtf8("Nodes");
      else if (section == EDGES_SECTION)
        return trUtf8("Edges");
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

bool GraphHierarchiesModel::needsSaving() {
  bool saveNeeded = false;
  foreach(GraphNeedsSavingObserver* observer, _saveNeeded) {
    saveNeeded = saveNeeded || observer->needsSaving();
  }
  return saveNeeded;
}

void GraphHierarchiesModel::addGraph(tlp::Graph *g) {
  if (_graphs.contains(g) || g == NULL)
    return;

  _saveNeeded[g] = new GraphNeedsSavingObserver(g);

  beginInsertRows(QModelIndex(),rowCount(),rowCount());
  Graph *i;
  foreach(i,_graphs) {
    if (i->isDescendantGraph(g))
      return;
  }
  _graphs.push_back(g);

  setApplicationDefaults(g);
  g->addListener(this);

  if (_graphs.size() == 1)
    setCurrentGraph(g);

  endInsertRows();
}

void GraphHierarchiesModel::removeGraph(tlp::Graph *g) {
  if (_graphs.contains(g)) {
    int pos = _graphs.indexOf(g);
    beginRemoveRows(QModelIndex(),pos,pos);
    _graphs.removeAll(g);
    endRemoveRows();

    _saveNeeded.remove(g);

    if (_currentGraph == g) {
      if (_graphs.size() == 0)
        setCurrentGraph(NULL);
      else
        setCurrentGraph(_graphs[0]);
    }
  }
}

void GraphHierarchiesModel::treatEvent(const Event &e) {
  Graph *g = dynamic_cast<tlp::Graph *>(e.sender());
  assert(g);

  if (e.type() == Event::TLP_DELETE && _graphs.contains(g)) { // A root graph has been deleted
    int pos = _graphs.indexOf(g);
    beginRemoveRows(QModelIndex(),pos,pos);
    _graphs.removeAll(g);

    if (_currentGraph == g) {
      if (_graphs.size() == 0)
        _currentGraph = NULL;
      else
        _currentGraph = _graphs[0];

      emit currentGraphChanged(_currentGraph);
    }

    endRemoveRows();
  }
  else if (e.type() == Event::TLP_MODIFICATION) {
    const GraphEvent *ge = dynamic_cast<const tlp::GraphEvent *>(&e);

    if (!ge)
      return;

    if (_graphs.contains(ge->getGraph()->getRoot())) {
      if (ge->getType() == GraphEvent::TLP_BEFORE_ADD_DESCENDANTGRAPH) {
        string pName;
        Graph* parentGraph = ge->getSubGraph()->getSuperGraph();
        parentGraph->getAttribute<string>("name",pName);
        QModelIndex parentIndex = _indexCache[parentGraph];
        assert(parentIndex != QModelIndex());
        beginInsertRows(parentIndex,parentGraph->numberOfSubGraphs(),parentGraph->numberOfSubGraphs());
      }
      else if (ge->getType() == GraphEvent::TLP_AFTER_ADD_DESCENDANTGRAPH) {
        endInsertRows();
      }
      else if (ge->getType() == GraphEvent::TLP_BEFORE_DEL_DESCENDANTGRAPH) {
        const Graph* sg = ge->getSubGraph();

        if (_indexCache.contains(sg)) {
          QModelIndex index = _indexCache[sg];
          QModelIndex parentIndex = index.parent();
          int row = index.row();
          beginRemoveRows(parentIndex,row,row);
        }
      }
      else if (ge->getType() == GraphEvent::TLP_AFTER_DEL_DESCENDANTGRAPH) {
        const Graph* sg = ge->getSubGraph();

        if (_indexCache.contains(sg)) {
          _indexCache.remove(sg);
          endRemoveRows();
        }
      }
    }
  }
}

static QString GRAPHS_PATH("/graphs/");
static QString GRAPH_FILE("graph.json");

QMap<QString,tlp::Graph*> GraphHierarchiesModel::readProject(tlp::TulipProject *project, tlp::PluginProgress *progress) {
  QMap<QString,tlp::Graph*> rootIds;
  foreach(QString entry, project->entryList(GRAPHS_PATH,QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name)) {
    QString file = GRAPHS_PATH + entry + "/graph.json";

    if (!project->exists(file))
      continue;

    QString absolutePath = project->toAbsolutePath(file);
    DataSet data;
    data.set<std::string>("file::filename",absolutePath.toStdString());
    Graph* g = tlp::importGraph("JSON Import",data,progress);
    rootIds[entry] = g;

    addGraph(g);
  }
  return rootIds;
}

QMap<tlp::Graph*,QString> GraphHierarchiesModel::writeProject(tlp::TulipProject *project, tlp::PluginProgress *progress) {
  QMap<tlp::Graph*,QString> rootIds;

  project->removeAllDir(GRAPHS_PATH);
  project->mkpath(GRAPHS_PATH);
  int i=0;
  foreach(tlp::Graph* g, _graphs) {
    rootIds[g] = QString::number(i);
    QString folder = GRAPHS_PATH + "/" + QString::number(i++) + "/";
    project->mkpath(folder);
    DataSet data;
    std::fstream *stream = project->stdFileStream(folder + "graph.json");
    tlp::exportGraph(g,*stream,"JSON Export",data,progress);
  }
  return rootIds;
}

QString GraphHierarchiesModel::generateName(tlp::Graph *graph) const {
  std::string name;
  graph->getAttribute<std::string>("name",name);

  if (name == "") {
    name = (trUtf8("graph_") + QString::number(graph->getId())).toStdString();
    graph->setAttribute<std::string>("name",name);
  }

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

  QModelIndex oldRow1 = _indexCache[_currentGraph];
  QModelIndex oldRow2 = createIndex(oldRow1.row(),columnCount()-1);
  _currentGraph = g;
  QModelIndex newRow1 = _indexCache[_currentGraph];
  QModelIndex newRow2 = createIndex(newRow1.row(),columnCount()-1);
  emit currentGraphChanged(g);
  emit dataChanged(oldRow1,oldRow2);
  emit dataChanged(newRow1,newRow2);
}

Graph *GraphHierarchiesModel::currentGraph() const {
  return _currentGraph;
}

Qt::ItemFlags GraphHierarchiesModel::flags(const QModelIndex &index) const {
  Qt::ItemFlags result = QAbstractItemModel::flags(index);

  if (index.column() == 0)
    result |= Qt::ItemIsEditable | Qt::ItemIsDragEnabled;

  return result;
}

QModelIndex GraphHierarchiesModel::indexOf(tlp::Graph* g) const {
  return _indexCache[g];
}

QMimeData* GraphHierarchiesModel::mimeData(const QModelIndexList &indexes) const {
  QSet<Graph*> graphs;

  foreach(QModelIndex index, indexes) {
    Graph *g = data(index,GraphRole).value<Graph*>();

    if (g != NULL)
      graphs.insert(g);
  }

  GraphMimeType* result = new GraphMimeType();

  //every current implementation uses a single graph, so we do not have a graphmim with multiple graphs.
  foreach(Graph* g,graphs) {
    result->setGraph(g);
  }

  return result;
}
