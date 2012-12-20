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
#include <tulip/DrawingTools.h>
#include <tulip/EdgeExtremityGlyphManager.h>

using namespace std;
using namespace tlp;

#define NAME_SECTION 0
#define ID_SECTION 1
#define NODES_SECTION 2
#define EDGES_SECTION 3

// define some specific MetaValueCalculator classes
// viewColor
class ViewColorCalculator :public AbstractColorProperty::MetaValueCalculator {
public:
  virtual void computeMetaValue(AbstractColorProperty* color, node mN,
                                Graph*, Graph*) {
    // meta node color is half opaque white
    color->setNodeValue(mN, Color(255, 255, 255, 127));
  }

  virtual void computeMetaValue(AbstractColorProperty* color, edge mE,
                                Iterator<edge>*itE, Graph*) {
    // meta edge color is the color of the first underlying edge
    color->setEdgeValue(mE, color->getEdgeValue(itE->next()));
  }
};

// viewLabel
class ViewLabelCalculator :public AbstractStringProperty::MetaValueCalculator {
public:
  // set the meta node label to label of viewMetric max corresponding node
  void computeMetaValue(AbstractStringProperty* label,
                        node mN, Graph* sg, Graph*) {
    // nothing to do if viewMetric does not exist
    if (!sg->existProperty("viewMetric"))
      return;

    node viewMetricMaxNode;
    double vMax = -DBL_MAX;
    DoubleProperty *metric = sg->getProperty<DoubleProperty>("viewMetric");
    Iterator<node> *itN= sg->getNodes();

    while (itN->hasNext()) {
      node itn = itN->next();
      const double& value = metric->getNodeValue(itn);

      if (value > vMax) {
        vMax = value;
        viewMetricMaxNode = itn;
      }
    }

    delete itN;
    label->setNodeValue(mN, label->getNodeValue(viewMetricMaxNode));
  }
};

// viewLayout
class ViewLayoutCalculator :public AbstractLayoutProperty::MetaValueCalculator {
public:
  void computeMetaValue(AbstractLayoutProperty* layout,
                        node mN, Graph* sg, Graph* mg) {
    SizeProperty* size = mg->getProperty<SizeProperty>("viewSize");
    DoubleProperty* rot = mg->getProperty<DoubleProperty>("viewRotation");
    BoundingBox box =
      tlp::computeBoundingBox(sg, (LayoutProperty *) layout, size, rot);
    Coord maxL(box[1]);
    Coord minL(box[0]);
    layout->setNodeValue(mN, (maxL + minL) / 2.f );
    Coord v(maxL - minL);

    if (v[2] < 0.0001) v[2] = 0.1f;

    mg->getProperty<SizeProperty>("viewSize")->
    setNodeValue(mN, Size(v[0],v[1],v[2]));
  }
};

class ViewSizeCalculator
    :public AbstractSizeProperty::MetaValueCalculator {
public:
  void computeMetaValue(AbstractSizeProperty*, node, Graph*, Graph*) {
    // do nothing
  }
};

// corresponding static instances
static ViewColorCalculator vColorCalc;
static ViewLabelCalculator vLabelCalc;
static ViewLayoutCalculator vLayoutCalc;
static ViewSizeCalculator vSizeCalc;

void GraphHierarchiesModel::setApplicationDefaults(tlp::Graph *g) {
  const std::string shapes = "viewShape", colors = "viewColor", sizes = "viewSize", metrics = "viewMetric", fonts = "viewFont", fontSizes = "viewFontSize",
                    borderWidth = "viewBorderWidth", borderColor = "viewBorderColor", tgtShape = "viewTgtAnchorShape", srcShape = "viewSrcAnchorShape",
                    labelColor = "viewLabelColor", labelPosition = "viewLabelPosition", rotation = "viewRotation", labelRotation = "viewLabelRotation",
                    srcAnchorSize = "viewSrcAnchorSize", texture = "viewTexture", tgtAnchorSize = "viewTgtAnchorSize";

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

  if (!g->existProperty(borderWidth)) {
    g->getProperty<DoubleProperty>(borderWidth)->setAllNodeValue(0);
    g->getProperty<DoubleProperty>(borderWidth)->setAllEdgeValue(1);
  }

  if (!g->existProperty(borderColor)) {
    g->getProperty<ColorProperty>(borderColor)->setAllNodeValue(TulipSettings::instance().defaultColor(tlp::NODE));
    g->getProperty<ColorProperty>(borderColor)->setAllEdgeValue(TulipSettings::instance().defaultColor(tlp::EDGE));
  }

  if (!g->existProperty(tgtShape)) {
    g->getProperty<IntegerProperty>(tgtShape)->setAllEdgeValue(50);
  }

  if (!g->existProperty(srcShape)) {
    g->getProperty<IntegerProperty>(srcShape)->setAllEdgeValue(EdgeExtremityGlyphManager::NoEdgeExtremetiesId);
  }

  if (!g->existProperty(labelColor)) {
    g->getProperty<ColorProperty>(labelColor)->setAllNodeValue(TulipSettings::instance().defaultLabelColor());
    g->getProperty<ColorProperty>(labelColor)->setAllEdgeValue(TulipSettings::instance().defaultLabelColor());
  }

  if (!g->existProperty(labelPosition)) {
    g->getProperty<IntegerProperty>(labelPosition)->setAllNodeValue(ON_CENTER);
    g->getProperty<IntegerProperty>(labelPosition)->setAllEdgeValue(ON_CENTER);
  }

  if (!g->existProperty(rotation)) {
    g->getProperty<DoubleProperty>(rotation)->setAllNodeValue(0);
    g->getProperty<DoubleProperty>(rotation)->setAllEdgeValue(0);
  }

  if (!g->existProperty(labelRotation)) {
    g->getProperty<DoubleProperty>(labelRotation)->setAllNodeValue(0);
    g->getProperty<DoubleProperty>(labelRotation)->setAllEdgeValue(0);
  }

  if (!g->existProperty(srcAnchorSize)) {
    g->getProperty<SizeProperty>(srcAnchorSize)->setAllNodeValue(Size(1,1,0));
    g->getProperty<SizeProperty>(srcAnchorSize)->setAllEdgeValue(Size(1,1,0));
  }

  if (!g->existProperty(tgtAnchorSize)) {
    g->getProperty<SizeProperty>(tgtAnchorSize)->setAllNodeValue(Size(1,1,0));
    g->getProperty<SizeProperty>(tgtAnchorSize)->setAllEdgeValue(Size(1,1,0));
  }

  if (!g->existProperty(texture)) {
    g->getProperty<StringProperty>(texture)->setAllNodeValue("");
    g->getProperty<StringProperty>(texture)->setAllEdgeValue("");
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

// Cache related methods
QModelIndex GraphHierarchiesModel::indexOf(const tlp::Graph* g) {
  if (g == NULL)
    return QModelIndex();

  QModelIndex result = _indexCache[g];

  if (!result.isValid())
    result = forceGraphIndex(const_cast<Graph*>(g));

  return result;
}
QModelIndex GraphHierarchiesModel::forceGraphIndex(Graph* g) {
  if (g == NULL)
    return QModelIndex();

  QModelIndex result;

  if (g->getRoot() == g) { // Peculiar case for root graphs
    result = createIndex(_graphs.indexOf(g),0,g);
    _indexCache[g] = result;
  }
  else {
    QVector<Graph*> hierarchy;
    Graph* child = g;

    do {
      hierarchy.push_front(g);
      child = child->getSuperGraph();
    }
    while (child != child->getRoot());


    foreach(Graph* child, hierarchy) {
      Graph* parent = child->getSuperGraph();
      unsigned int n = 0;

      for (n=0; n<parent->numberOfSubGraphs(); ++n) {
        if (parent->getNthSubGraph(n) == child)
          break;
      }

      result = createIndex(n,0,child);
      _indexCache[child] = result;
    }
  }

  return result;
}

// Serialization
static QString GRAPHS_PATH("/graphs/");
static QString GRAPH_FILE("graph.json");

bool GraphHierarchiesModel::needsSaving() {
  bool saveNeeded = false;
  foreach(GraphNeedsSavingObserver* observer, _saveNeeded) {
    saveNeeded = saveNeeded || observer->needsSaving();
  }
  return saveNeeded;
}

QMap<QString,tlp::Graph*> GraphHierarchiesModel::readProject(tlp::TulipProject *project, tlp::PluginProgress *progress) {
  QMap<QString,tlp::Graph*> rootIds;
  foreach(QString entry, project->entryList(GRAPHS_PATH,QDir::Dirs | QDir::NoDot | QDir::NoDotDot, QDir::Name)) {
    QString file = GRAPHS_PATH + entry + "/graph.tlp";

    if (!project->exists(file)) {
      file = GRAPHS_PATH + entry + "/graph.json";

      if (!project->exists(file))
        continue;
    }

    QString absolutePath = project->toAbsolutePath(file);
    Graph* g = loadGraph(std::string(absolutePath.toUtf8().data()),progress);
    rootIds[entry] = g;

    addGraph(g);
  }

  QDir::setCurrent(QFileInfo(project->projectFile()).absolutePath());

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
    tlp::saveGraph(g,project->toAbsolutePath(folder + "graph.tlp").toStdString(),progress);
  }
  foreach(GraphNeedsSavingObserver* observer, _saveNeeded)
  observer->saved();
  return rootIds;
}

// Model related
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
    graph->setAttribute("name", std::string(value.toString().toUtf8().data()));
    return true;
  }

  return QAbstractItemModel::setData(index,value,role);
}

QVariant GraphHierarchiesModel::data(const QModelIndex &index, int role) const {
  Graph *graph = (Graph *)(index.internalPointer());

  if (index.column() == 0)
    const_cast<GraphHierarchiesModel*>(this)->_indexCache[graph] = index;

  if (role == Qt::DisplayRole || role == Qt::EditRole) {
    if (index.column() == NAME_SECTION)
      return generateName(graph);
    else if (index.column() == ID_SECTION)
      return graph->getId();
    else if (index.column() == NODES_SECTION)
      return graph->numberOfNodes();
    else if (index.column() == EDGES_SECTION)
      return graph->numberOfEdges();
  }

  else if (role == Qt::ToolTipRole) {
    return QString("<table><tr><td>%1</td></tr><tr><td>Id = %2, Nodes = %3, Edges= %4</tr></td></table>").arg(generateName(graph)).arg(graph->getId()).arg(graph->numberOfNodes()).arg(graph->numberOfEdges());
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
  }

  return TulipModel::headerData(section,orientation,role);
}

Qt::ItemFlags GraphHierarchiesModel::flags(const QModelIndex &index) const {
  Qt::ItemFlags result = QAbstractItemModel::flags(index);

  if (index.column() == 0)
    result |= Qt::ItemIsEditable | Qt::ItemIsDragEnabled;

  return result;
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

// Graphs collection

QString GraphHierarchiesModel::generateName(tlp::Graph *graph) const {
  std::string name;
  graph->getAttribute("name",name);

  if (name == "") {
    name = (trUtf8("graph_") + QString::number(graph->getId())).toStdString();
    graph->setAttribute("name",name);
  }

  return QString::fromUtf8(name.c_str());
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

  Graph* oldGraph = _currentGraph;
  _currentGraph = g;

  if (oldGraph != NULL) {
    QModelIndex oldRow1 = indexOf(_currentGraph);
    QModelIndex oldRow2 = createIndex(oldRow1.row(),columnCount()-1);
    emit dataChanged(oldRow1,oldRow2);
  }

  if (_currentGraph != NULL) {
    QModelIndex newRow1 = indexOf(_currentGraph);
    QModelIndex newRow2 = createIndex(newRow1.row(),columnCount()-1);
    emit dataChanged(newRow1,newRow2);
  }

  emit currentGraphChanged(g);
}

Graph *GraphHierarchiesModel::currentGraph() const {
  return _currentGraph;
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
  g->getProperty<ColorProperty>("viewColor")->setMetaValueCalculator(&vColorCalc);
  g->getProperty<StringProperty>("viewLabel")->setMetaValueCalculator(&vLabelCalc);
  g->getProperty<LayoutProperty>("viewLayout")->setMetaValueCalculator(&vLayoutCalc);
  g->getProperty<SizeProperty>("viewSize")->setMetaValueCalculator(&vSizeCalc);

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
    _saveNeeded.remove(g);
    endRemoveRows();

    _saveNeeded.remove(g);

    if (_currentGraph == g) {
      if (_graphs.size() == 0) {
        _currentGraph = NULL;
        emit currentGraphChanged(_currentGraph);
      }
      else
        setCurrentGraph(_graphs[0]);
    }
  }
}

// Observation

void GraphHierarchiesModel::treatEvent(const Event &e) {
  Graph *g = dynamic_cast<tlp::Graph *>(e.sender());
  assert(g);

  if (e.type() == Event::TLP_DELETE && _graphs.contains(g)) { // A root graph has been deleted
    int pos = _graphs.indexOf(g);
    beginRemoveRows(QModelIndex(),pos,pos);
    _graphs.removeAll(g);
    _saveNeeded.remove(g);

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
        QModelIndex parentIndex = indexOf(parentGraph);

        if (!parentIndex.isValid())
          parentIndex = forceGraphIndex(parentGraph);

        assert(parentIndex != QModelIndex());
        beginInsertRows(parentIndex,parentGraph->numberOfSubGraphs(),parentGraph->numberOfSubGraphs());
      }
      else if (ge->getType() == GraphEvent::TLP_AFTER_ADD_DESCENDANTGRAPH) {
        endInsertRows();
      }
      else if (ge->getType() == GraphEvent::TLP_BEFORE_DEL_DESCENDANTGRAPH) {
        const Graph* sg = ge->getSubGraph();

        QModelIndex index = indexOf(sg);
        assert(index.isValid());
        QModelIndex parentIndex = index.parent();
        int row = index.row();
        beginRemoveRows(parentIndex,row,row);
      }
      else if (ge->getType() == GraphEvent::TLP_AFTER_DEL_DESCENDANTGRAPH) {
        const Graph* sg = ge->getSubGraph();
        assert(_indexCache.contains(sg));
        _indexCache.remove(sg);
        endRemoveRows();
      }
    }
  }
}
