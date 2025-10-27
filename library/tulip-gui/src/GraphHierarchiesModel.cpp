/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include "tulip/GraphHierarchiesModel.h"

#include <QFont>
#include <QCryptographicHash>
#include <QWidget>

#include <tulip/TlpTools.h>
#include <tulip/TulipMetaTypes.h>
#include <tulip/Graph.h>
#include <tulip/TulipProject.h>
#include <tulip/TulipSettings.h>
#include <tulip/TulipMimes.h>
#include <tulip/GraphNeedsSavingObserver.h>
#include <tulip/TlpQtTools.h>
#include <tulip/Perspective.h>
#include <tulip/StableIterator.h>

#include <sstream>

using namespace std;
using namespace tlp;

#define NAME_SECTION 0
#define ID_SECTION 1
#define NODES_SECTION 2
#define EDGES_SECTION 3

// Serialization
static QString GRAPHS_PATH("/graphs/");
static QString TEXTURES_PATH("/textures/");

static void copyTextureFileInProject(const QString &viewTextureValue, tlp::TulipProject *project,
                                     QStringList &projectTexturesFolders,
                                     QStringList &projectTexturesFiles) {
  // We use QCryptographicHash to generate a MD5 sum for each value in the viewTexture property.
  // Each texture is copied to the following project path : /textures/<md5_sum>/<texture filename> .
  // It enables to copy textures with the same filename but located in different folders.
  QCryptographicHash hasher(QCryptographicHash::Md5);
  QFileInfo fileInfo(viewTextureValue);
  // get corresponding project file path if any
  QString projectFilePath = tlp::getRegisteredTextureFile(viewTextureValue);

  // If the texture file exists or a known project file corresponds
  // to it then we copy the texture in the project
  if (fileInfo.exists() || (!projectFilePath.isEmpty() && QFileInfo(projectFilePath).exists())) {
    // Generate a MD5 sum from the absolute texture file path
    hasher.reset();
    hasher.addData(viewTextureValue.toUtf8());
    // Compute texture folder and texture file path in the project
    QString textureProjectFolder = TEXTURES_PATH + hasher.result().toHex() + "/";
    QString textureProjectFile = textureProjectFolder + fileInfo.fileName();

    if (!projectTexturesFiles.contains(textureProjectFile)) {
      // The texture file may have not already been copied in the project,
      // so create its folder if needed and copy the file
      if (!project->exists(textureProjectFolder)) {
        project->mkpath(textureProjectFolder);
      }
      if (fileInfo.exists())
        project->copy(fileInfo.absoluteFilePath(), textureProjectFile);
      else
        project->copy(projectFilePath, textureProjectFile);
    } else {
      // The texture file has already been copied in the project,
      // recopy the file as it may have changed and remove texture folder and project path
      // from the lists of folders and files to remove afterwards
      project->copy(fileInfo.absoluteFilePath(), textureProjectFile);
      projectTexturesFiles.removeAll(textureProjectFile);
      projectTexturesFolders.removeAll(textureProjectFolder);
    }
  }
}

// Method for copying nodes and edges textures in the project to make it portable across computers
static void writeTextureFilesInProject(const QList<tlp::Graph *> &graphs,
                                       tlp::TulipProject *project, tlp::PluginProgress *progress) {
  if (progress) {
    progress->progress(0, 0);
    progress->setComment(
        "Writing texture files into project to ensure its portability across computers ...");
  }

  QStringList projectTexturesFolders;
  QStringList projectTexturesFiles;

  // gather list of texture folders already present in the project
  for (const QString &textureFolder :
       project->entryList(TEXTURES_PATH, QDir::Dirs | QDir::NoDotAndDotDot)) {
    projectTexturesFolders.append(TEXTURES_PATH + textureFolder);
  }

  // gather list of texture file paths already present in the project
  for (const QString &textureFolder : projectTexturesFolders) {
    for (const QString &textureFile : project->entryList(textureFolder, QDir::Files)) {
      projectTexturesFiles.append(textureFolder + "/" + textureFile);
    }
  }

  for (auto g : graphs) {
    // Process the viewTexture default node value
    StringProperty *viewTexture = g->getProperty<StringProperty>("viewTexture");
    copyTextureFileInProject(tlpStringToQString(viewTexture->getNodeDefaultValue()), project,
                             projectTexturesFolders, projectTexturesFiles);

    // Process the non default valuated nodes in the viewTexture property
    for (auto n : viewTexture->getNonDefaultValuatedNodes()) {
      copyTextureFileInProject(tlpStringToQString(viewTexture->getNodeValue(n)), project,
                               projectTexturesFolders, projectTexturesFiles);
    }

    // Process the viewTexture default edge value
    copyTextureFileInProject(tlpStringToQString(viewTexture->getEdgeDefaultValue()), project,
                             projectTexturesFolders, projectTexturesFiles);

    // Process the non default valuated nodes in the viewTexture property
    for (auto e : viewTexture->getNonDefaultValuatedEdges()) {
      copyTextureFileInProject(tlpStringToQString(viewTexture->getEdgeValue(e)), project,
                               projectTexturesFolders, projectTexturesFiles);
    }
  }

  // Previously copied textures are not used anymore in the project,
  // so remove associated files and folders
  for (const QString &textureProjectFileToRemove : projectTexturesFiles) {
    project->removeFile(textureProjectFileToRemove);
  }

  for (const QString &textureProjectFolderToRemove : projectTexturesFolders) {
    project->removeDir(textureProjectFolderToRemove);
  }
}

static void restoreTextureFileFromProject(QString viewTextureValue, tlp::TulipProject *project) {
  // We use QCryptographicHash to generate a MD5 sum for
  // each value in the viewTexture property.
  // It enables to copy textures with the same filename but located
  // in different folders.
  // Each texture may have been copied to the following project path :
  // /textures/<md5_sum>/<texture filename> .
  QCryptographicHash hasher(QCryptographicHash::Md5);

  QFileInfo fileInfo(viewTextureValue);
  // Generate a MD5 sum from the absolute texture file path
  hasher.addData(viewTextureValue.toUtf8());
  // Compute texture file path in the project
  QString textureProjectFile = TEXTURES_PATH + hasher.result().toHex() + "/" + fileInfo.fileName();

  // If the texture file does not exist but is present in the project
  // register the project file as texture file if needed
  if (!fileInfo.exists() && project->exists(textureProjectFile)) {
    textureProjectFile = project->toAbsolutePath(textureProjectFile);
    // register
    tlp::registerTextureFile(viewTextureValue, textureProjectFile);
  }
}

// Method for restoring nodes and edges textures possibly bundled in the project if the original
// texture files
// are not present on the computer loading the project
static void restoreTextureFilesFromProject(tlp::Graph *g, tlp::TulipProject *project,
                                           tlp::PluginProgress *progress) {
  if (progress) {
    progress->progress(0, 0);
    progress->setComment("Checking if texture files can be restored from project if needed ...");
  }

  StringProperty *viewTexture = g->getProperty<StringProperty>("viewTexture");

  // Process the nodes first
  // Get the default node texture file
  QString viewTextureValue(tlpStringToQString(viewTexture->getNodeDefaultValue()));
  if (!viewTextureValue.isEmpty())
    restoreTextureFileFromProject(viewTextureValue, project);

  // Iterate on default valuated nodes
  for (auto n : viewTexture->getNonDefaultValuatedNodes()) {
    // Get the node texture file previously backuped
    viewTextureValue = tlpStringToQString(viewTexture->getNodeValue(n));
    restoreTextureFileFromProject(viewTextureValue, project);
  }

  // process the edges
  viewTextureValue = tlpStringToQString(viewTexture->getEdgeDefaultValue());
  if (!viewTextureValue.isEmpty())
    restoreTextureFileFromProject(viewTextureValue, project);

  // Iterate on the default valuated edges
  for (auto e : viewTexture->getNonDefaultValuatedEdges()) {
    // Get the edge texture file
    viewTextureValue = tlpStringToQString(viewTexture->getEdgeValue(e));
    restoreTextureFileFromProject(viewTextureValue, project);
  }
}

GraphHierarchiesModel::GraphHierarchiesModel(QObject *parent)
    : TulipModel(parent), tlp::ImportGraphObserver(), _currentGraph(nullptr) {}

GraphHierarchiesModel::GraphHierarchiesModel(const GraphHierarchiesModel &copy)
    : TulipModel(copy.QObject::parent()), tlp::Observable(), tlp::ImportGraphObserver() {
  for (int i = 0; i < copy.size(); ++i)
    graphImported(copy[i]);

  _currentGraph = nullptr;
}

GraphHierarchiesModel::~GraphHierarchiesModel() {
  qDeleteAll(_saveNeeded);
}

// Cache related methods
QModelIndex GraphHierarchiesModel::indexOf(const tlp::Graph *g) {
  if (g == nullptr)
    return QModelIndex();

  QModelIndex result = _indexCache[g];

  // ensure result is valid and points on an existing row
  if (!result.isValid() || (result.row() > (size() - 1)))
    result = forceGraphIndex(const_cast<Graph *>(g));

  return result;
}

QModelIndex GraphHierarchiesModel::forceGraphIndex(Graph *g) {
  if (g == nullptr)
    return QModelIndex();

  QModelIndex result;

  if (g->getRoot() == g) { // Peculiar case for root graphs
    result = createIndex(_graphs.indexOf(g), 0, g);
  } else {
    Graph *parent = g->getSuperGraph();
    unsigned int n = 0;

    for (n = 0; n < parent->numberOfSubGraphs(); ++n) {
      if (parent->getNthSubGraph(n) == g)
        break;
    }

    result = createIndex(n, 0, g);
  }
  _indexCache[g] = result;

  return result;
}

bool GraphHierarchiesModel::needsSaving() {
  bool saveNeeded = false;

  for (GraphNeedsSavingObserver *observer : _saveNeeded) {
    saveNeeded = saveNeeded || observer->needsSaving();
  }

  return saveNeeded;
}

QMap<QString, tlp::Graph *> GraphHierarchiesModel::readProject(tlp::TulipProject *project,
                                                               tlp::PluginProgress *progress) {
  QMap<QString, tlp::Graph *> rootIds;

  for (const QString &entry :
       project->entryList(GRAPHS_PATH, QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name)) {
    QString file = GRAPHS_PATH + entry + "/graph.tlp";

    if (!project->exists(file)) {
      file = GRAPHS_PATH + entry + "/graph.tlpb";

      if (!project->exists(file))
        continue;
    }

    QString absolutePath = project->toAbsolutePath(file);
    Graph *g = loadGraph(QStringToTlpString(absolutePath), progress);

    if (g) {
      rootIds[entry] = g;
      restoreTextureFilesFromProject(g, project, progress);
    } else {
      // failure when loading a graph
      // so delete already loaded graphs
      for (QMap<QString, tlp::Graph *>::iterator it = rootIds.begin(); it != rootIds.end(); ++it) {
        delete it.value();
      }

      // and clear rootIds
      rootIds.clear();
      break;
    }
  }

  QDir::setCurrent(QFileInfo(project->projectFile()).absolutePath());

  return rootIds;
}
QMap<tlp::Graph *, QString> GraphHierarchiesModel::writeProject(tlp::TulipProject *project,
                                                                tlp::PluginProgress *progress) {
  QMap<tlp::Graph *, QString> rootIds;

  project->removeAllDir(GRAPHS_PATH);
  project->mkpath(GRAPHS_PATH);

  int i = 0;

  for (auto g : _graphs) {
    rootIds[g] = QString::number(i);
    QString folder = GRAPHS_PATH + "/" + QString::number(i++) + "/";
    project->mkpath(folder);

    if (!TulipSettings::isUseTlpbFileFormat())
      tlp::saveGraph(g, QStringToTlpString(project->toAbsolutePath(folder + "graph.tlp")),
                     progress);
    else
      tlp::saveGraph(g, QStringToTlpString(project->toAbsolutePath(folder + "graph.tlpb")),
                     progress);
  }

  writeTextureFilesInProject(_graphs, project, progress);

  for (GraphNeedsSavingObserver *observer : _saveNeeded)
    observer->saved();

  return rootIds;
}

// Model related
QModelIndex GraphHierarchiesModel::index(int row, int column, const QModelIndex &parent) const {
  if (row < 0)
    return QModelIndex();

  Graph *g = nullptr;

  if (parent.isValid())
    g = static_cast<Graph *>(parent.internalPointer())->getNthSubGraph(row);
  else if (row < _graphs.size())
    g = _graphs[row];

  if (g == nullptr) {
    return QModelIndex();
  }

  return createIndex(row, column, g);
}

QModelIndex GraphHierarchiesModel::parent(const QModelIndex &child) const {
  if (!child.isValid())
    return QModelIndex();

  Graph *childGraph = static_cast<Graph *>(child.internalPointer());

  if (childGraph == nullptr || _graphs.contains(childGraph) ||
      childGraph->getSuperGraph() == childGraph) {
    return QModelIndex();
  }

  int row = 0;
  Graph *parent = childGraph->getSuperGraph();

  if (_graphs.contains(parent))
    row = _graphs.indexOf(parent);
  else {
    Graph *ancestor = parent->getSuperGraph();

    for (unsigned int i = 0; i < ancestor->numberOfSubGraphs(); i++) {
      if (ancestor->getNthSubGraph(i) == parent) {
        break;
      }

      row++;
    }
  }

  return createIndex(row, 0, parent);
}

int GraphHierarchiesModel::rowCount(const QModelIndex &parent) const {
  if (!parent.isValid())
    return _graphs.size();

  if (parent.column() != 0)
    return 0;

  Graph *parentGraph = static_cast<Graph *>(parent.internalPointer());

  return parentGraph->numberOfSubGraphs();
}

int GraphHierarchiesModel::columnCount(const QModelIndex &) const {
  return 4;
}

bool GraphHierarchiesModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (index.column() == NAME_SECTION) {
    Graph *graph = static_cast<Graph *>(index.internalPointer());
    graph->setName(QStringToTlpString(value.toString()));
    return true;
  }

  return QAbstractItemModel::setData(index, value, role);
}

QVariant GraphHierarchiesModel::data(const QModelIndex &index, int role) const {

  if (!index.isValid())
    return QVariant();

  Graph *graph = static_cast<Graph *>(index.internalPointer());

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
    auto selection = graph->getProperty<BooleanProperty>("viewSelection");
    return QString("<table><tr><td><b>%1</b></td></tr><tr><td>Id = %2, Nodes = %3, Edges = "
                   "%4, selected = %5 nodes , %6 edges</tr></td></table>")
        .arg(generateName(graph))
        .arg(graph->getId())
        .arg(graph->numberOfNodes())
        .arg(graph->numberOfEdges())
        .arg(selection->numberOfNonDefaultValuatedNodes(graph))
        .arg(selection->numberOfNonDefaultValuatedEdges(graph));
  }

  else if (role == GraphRole) {
    return QVariant::fromValue<Graph *>(static_cast<Graph *>(index.internalPointer()));
  }

  else if (role == Qt::TextAlignmentRole && index.column() != NAME_SECTION)
    return Qt::AlignCenter;

  else if (role == Qt::FontRole) {
    QFont f;
    QWidget *p = dynamic_cast<QWidget *>(QAbstractItemModel::parent());
    if (p)
      f = p->font();

    if (graph == _currentGraph)
      f.setBold(true);

    return f;
  }

  return QVariant();
}

QVariant GraphHierarchiesModel::headerData(int section, Qt::Orientation orientation,
                                           int role) const {
  if (orientation == Qt::Horizontal) {
    if (role == Qt::DisplayRole) {
      if (section == NAME_SECTION)
        return "Name";
      else if (section == ID_SECTION)
        return "Id";
      else if (section == NODES_SECTION)
        return "Nodes";
      else if (section == EDGES_SECTION)
        return "Edges";
    }

    else if (role == Qt::TextAlignmentRole && section != NAME_SECTION)
      return Qt::AlignCenter;
  }

  return TulipModel::headerData(section, orientation, role);
}

Qt::ItemFlags GraphHierarchiesModel::flags(const QModelIndex &index) const {
  Qt::ItemFlags result = QAbstractItemModel::flags(index);

  if (index.column() == 0)
    result = result | Qt::ItemIsEditable | Qt::ItemIsDragEnabled;

  return result;
}

QMimeData *GraphHierarchiesModel::mimeData(const QModelIndexList &indexes) const {
  QSet<Graph *> graphs;

  for (const QModelIndex &index : indexes) {
    Graph *g = data(index, GraphRole).value<Graph *>();

    if (g != nullptr)
      graphs.insert(g);
  }

  GraphMimeType *result = new GraphMimeType();

  // every current implementation uses a single graph, so we do not have a graphmim with multiple
  // graphs.
  for (auto g : graphs)
    result->setGraph(g);

  return result;
}

// Graphs collection
QString GraphHierarchiesModel::generateName(tlp::Graph *graph) const {
  std::string name = graph->getName();

  if (name.empty()) {
    stringstream ss;
    ss << "graph_" << graph->getId();
    name = ss.str();
    graph->setName(name);
  }

  return tlp::tlpStringToQString(name);
}

void GraphHierarchiesModel::setCurrentGraph(tlp::Graph *g) {

  bool inHierarchy = false;

  for (auto _g : _graphs) {
    if (_g->isDescendantGraph(g) || g == _g) {
      inHierarchy = true;
      break;
    }
  }

  if (!inHierarchy)
    return;

  Graph *oldGraph = _currentGraph;
  _currentGraph = g;

  if (oldGraph != nullptr && oldGraph != _currentGraph) {
    QModelIndex oldRow1 = indexOf(oldGraph);
    QModelIndex oldRow2 = createIndex(oldRow1.row(), columnCount() - 1);
    emit dataChanged(oldRow1, oldRow2);
  }

  if (_currentGraph != nullptr) {
    QModelIndex newRow1 = indexOf(_currentGraph);
    QModelIndex newRow2 = createIndex(newRow1.row(), columnCount() - 1);
    emit dataChanged(newRow1, newRow2);
  }

  emit currentGraphChanged(g);
}

Graph *GraphHierarchiesModel::currentGraph() const {
  return _currentGraph;
}

void GraphHierarchiesModel::initIndexCache(tlp::Graph *root) {
  int i = 0;
  for (auto sg : root->subGraphs()) {
    _indexCache[sg] = createIndex(i++, 0, sg);
    initIndexCache(sg);
  }
}

static void addListenerToWholeGraphHierarchy(Graph *root, Observable *listener) {
  for (auto sg : root->subGraphs()) {
    addListenerToWholeGraphHierarchy(sg, listener);
  }
  root->addListener(listener);
  root->addObserver(listener);
}

void GraphHierarchiesModel::graphImported(tlp::Graph *g) {
  beginInsertRows(QModelIndex(), rowCount(), rowCount());

  _saveNeeded[g] = new GraphNeedsSavingObserver(
      g, Perspective::instance() ? Perspective::instance()->mainWindow() : nullptr);

  _graphs.push_back(g);

  if (_graphs.size() == 1)
    setCurrentGraph(g);

  endInsertRows();
  initIndexCache(g);

  // listen events on the whole hierarchy
  // in order to keep track of subgraphs names, number of nodes and edges
  // must be done after the row is inserted
  // to prevent the use of invalid QModelIndex
  addListenerToWholeGraphHierarchy(g, this);
}

void GraphHierarchiesModel::removeGraph(tlp::Graph *g) {
  if (_graphs.contains(g)) {
    int pos = _graphs.indexOf(g);
    beginRemoveRows(QModelIndex(), pos, pos);
    _graphs.removeAll(g);
    GraphNeedsSavingObserver *s = _saveNeeded.take(g);
    delete s;
    endRemoveRows();

    if (_currentGraph == g) {
      if (_graphs.empty()) {
        _currentGraph = nullptr;
        // registered texture files are no longer needed
        tlp::clearRegisteredTextureFiles();
        emit currentGraphChanged(_currentGraph);
      } else
        setCurrentGraph(_graphs[0]);
    }
  }
}

// Observation
void GraphHierarchiesModel::treatEvent(const Event &e) {
  Graph *g = static_cast<tlp::Graph *>(e.sender());
  static QModelIndex sgIndex;
  static Graph *parentGraph;

  if (e.type() == Event::TLP_DELETE && _graphs.contains(g)) { // A root graph has been deleted
    int pos = _graphs.indexOf(g);
    _graphs.removeAt(pos);
    beginRemoveRows(QModelIndex(), pos, pos);
    GraphNeedsSavingObserver *s = _saveNeeded.take(g);
    delete s;

    if (_currentGraph == g) {
      if (_graphs.empty())
        _currentGraph = nullptr;
      else
        _currentGraph = _graphs[0];

      emit currentGraphChanged(_currentGraph);
    }
    endRemoveRows();
  } else if (e.type() == Event::TLP_MODIFICATION) {
    const GraphEvent *ge = dynamic_cast<const tlp::GraphEvent *>(&e);

    if (!ge)
      return;

    if (_graphs.contains(ge->getGraph()->getRoot())) {

      if (ge->getType() == GraphEvent::TLP_AFTER_ADD_DESCENDANTGRAPH) {
        // that event must only be treated on a root graph
        if (ge->getGraph() != ge->getGraph()->getRoot()) {
          return;
        }

        const Graph *sg = ge->getSubGraph();

        parentGraph = sg->getSuperGraph();

#ifndef NDEBUG
        QModelIndex parentIndex = indexOf(parentGraph);

        assert(parentIndex.isValid());
#endif

        // update index cache for subgraphs of parent graph and added subgraphs
        int i = 0;
        for (auto sg2 : parentGraph->subGraphs()) {
          _indexCache[sg2] = createIndex(i++, 0, sg2);
        }

        i = 0;
        for (auto sg2 : sg->subGraphs()) {
          _indexCache[sg2] = createIndex(i++, 0, sg2);
        }

        sg->addListener(this);
        sg->addObserver(this);

        // insert the parent graph in the graphs changed set
        // in order to update associated tree views, displaying the graphs hierarchies,
        // when the treatEvents method is called
        _graphsChanged.insert(parentGraph);

      } else if (ge->getType() == GraphEvent::TLP_BEFORE_DEL_DESCENDANTGRAPH) {
        // that event must only be treated on a root graph
        if (ge->getGraph() != ge->getGraph()->getRoot()) {
          return;
        }

        // first get needed infos to update model
        // after sg removal
        const Graph *sg = ge->getSubGraph();
        sgIndex = indexOf(sg);
        assert(sgIndex.isValid());
        parentGraph = sg->getSuperGraph();

      } else if (ge->getType() == GraphEvent::TLP_AFTER_DEL_DESCENDANTGRAPH) {
        // that event must only be treated on a root graph
        if (ge->getGraph() != ge->getGraph()->getRoot()) {
          return;
        }

        const Graph *sg = ge->getSubGraph();
        QModelIndex parentIndex = sgIndex.parent();
        assert(parentIndex.isValid());

        // update index cache for subgraphs of parent graph
        int i = 0;
        for (auto sg2 : parentGraph->subGraphs()) {
          if (i >= sgIndex.row())
            _indexCache[sg2] = createIndex(i, 0, sg2);
          ++i;
        }

        // remove sg corresponding row from model
        // and prevent dangling pointer to remain in the persistent indexes
        beginRemoveRows(parentIndex, sgIndex.row(), sgIndex.row());
        changePersistentIndex(sgIndex, QModelIndex());
        _indexCache.remove(sg);
        endRemoveRows();

        // insert the parent graph in the graphs changed set
        // in order to update associated tree views, displaying the graphs hierarchies,
        // when the treatEvents method is called
        _graphsChanged.insert(parentGraph);

        // remove the subgraph from the graphs changed set
        // as no update will be required for it in the associated tree views
        _graphsChanged.remove(sg);

        if (currentGraph() == sg) {
          setCurrentGraph(parentGraph);
          setCurrentGraph(parentGraph);
        }

      } else if (ge->getType() == GraphEvent::TLP_ADD_NODE ||
                 ge->getType() == GraphEvent::TLP_ADD_NODES ||
                 ge->getType() == GraphEvent::TLP_AFTER_DEL_NODE ||
                 ge->getType() == GraphEvent::TLP_ADD_EDGE ||
                 ge->getType() == GraphEvent::TLP_ADD_EDGES ||
                 ge->getType() == GraphEvent::TLP_AFTER_DEL_EDGE ||
                 (ge->getType() == GraphEvent::TLP_AFTER_SET_ATTRIBUTE &&
                  ge->getAttributeName() == "name")) {
        const Graph *graph = ge->getGraph();
        // row representing the graph in the associated tree views has to be updated
        _graphsChanged.insert(graph);
      }
    }
  }
}

void GraphHierarchiesModel::treatEvents(const std::vector<tlp::Event> &) {

  if (_graphsChanged.isEmpty()) {
    return;
  }

  // update the rows associated to modified graphs (number of subgraphs/nodes/edges has changed)
  // in the associated tree views

  emit layoutAboutToBeChanged();

  for (auto graph : _graphsChanged) {
    QModelIndex graphIndex = indexOf(graph);
    QModelIndex graphEdgesIndex = graphIndex.sibling(graphIndex.row(), EDGES_SECTION);
    emit dataChanged(graphIndex, graphEdgesIndex);
  }

  emit layoutChanged();

  _graphsChanged.clear();
}
