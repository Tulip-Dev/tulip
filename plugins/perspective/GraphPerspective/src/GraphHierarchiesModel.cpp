#include "GraphHierarchiesModel.h"

#include <tulip/Graph.h>
#include <QtGui/QFont>
#include <QtCore/QSize>
#include <fstream>

#include <tulip/TulipProject.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/TulipSettings.h>

using namespace std;
using namespace tlp;

#define NAME_SECTION 0
#define ID_SECTION 1
#define NODES_SECTION 2
#define EDGES_SECTION 3

void GraphHierarchiesModel::setApplicationDefaults(tlp::Graph *g) {
  const std::string shapes = "viewShape", colors = "viewColor", sizes = "viewSize";

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
  Graph *graph = (Graph *)(index.internalPointer());

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

  else if (role == Qt::TextAlignmentRole && index.column() != NAME_SECTION)
    return Qt::AlignCenter;

  else if (role == Qt::FontRole) {
    QFont f;

    if (index.column() == NAME_SECTION && graph == graph->getRoot())
      f.setItalic(true);

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
  emit layoutAboutToBeChanged();

  if (_graphs.contains(g))
    return;

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
    return trUtf8("untitled_") + QString::number(graph->getId());

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
