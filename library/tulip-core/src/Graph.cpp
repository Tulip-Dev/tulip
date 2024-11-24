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

#include <iomanip>
#include <mutex>
#include <sstream>
#include <stack>
#include <unordered_set>

#include <tulip/StlIterator.h>
#include <tulip/Graph.h>
#include <tulip/GraphImpl.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/GraphProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/DataSet.h>
#include <tulip/ExportModule.h>
#include <tulip/Algorithm.h>
#include <tulip/ImportModule.h>
#include <tulip/SimplePluginProgress.h>
#include <tulip/BoundingBox.h>
#include <tulip/DrawingTools.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/vectorgraph.h>
#include <tulip/PluginLister.h>

using namespace std;
using namespace tlp;

ostream &operator<<(ostream &os, const Graph *graph) {
  os << ";(nodes <node_id> <node_id> ...)" << endl;
  os << "(nodes ";

  if (graph->numberOfNodes()) {
    node beginNode, previousNode;
    beginNode = previousNode = graph->getOneNode();
    os << beginNode.id;
    unsigned int i = 0;

    for (auto current : graph->nodes()) {

      if (current.id == previousNode.id + 1) {
        previousNode = current;

        if (++i == graph->numberOfNodes())
          os << ".." << current.id;
      } else {
        if (previousNode != beginNode) {
          os << ".." << previousNode.id;
        }

        os << " " << current.id;
        beginNode = previousNode = current;
      }
    }
  }

  os << ")" << endl;
  os << ";(edge <edge_id> <source_id> <target_id>)" << endl;

  for (auto e : graph->edges()) {
    auto ends = graph->ends(e);
    os << "(edge " << e.id << " " << ends.first.id << " " << ends.second.id << ")" << endl;
  }

  return os;
}

//=========================================================
static std::set<ImportGraphObserver *> importGraphObservers;
static std::mutex importGraphObserversMtx;

ImportGraphObserver::ImportGraphObserver() {
  importGraphObserversMtx.lock();
  importGraphObservers.insert(this);
  importGraphObserversMtx.unlock();
}

ImportGraphObserver::~ImportGraphObserver() {
  importGraphObserversMtx.lock();
  importGraphObservers.erase(this);
  importGraphObserversMtx.unlock();
}

//=========================================================
Graph *tlp::newGraph() {
  auto g = GraphImpl::newGraph();

  importGraphObserversMtx.lock();
  for (auto ngo : importGraphObservers)
    ngo->graphImported(g);
  importGraphObserversMtx.unlock();

  return g;
}

//=========================================================
Graph *tlp::loadGraph(const std::string &filename, PluginProgress *progress) {
  DataSet dataSet;
  std::string importPluginName = "TLP Import";

  list<string> &&importPlugins = PluginLister::availablePlugins<ImportModule>();

  for (const string &pluginName : importPlugins) {
    const ImportModule &importPlugin =
        static_cast<const ImportModule &>(PluginLister::pluginInformation(pluginName));
    list<string> extensions(importPlugin.fileExtensions());

    for (const string &ext : extensions)
      if (filename.rfind(ext) == (filename.size() - ext.size())) {
        importPluginName = importPlugin.name();
        break;
      }
  }

  dataSet.set("file::filename", filename);
  return tlp::importGraph(importPluginName, dataSet, progress);
}
//=========================================================
bool tlp::saveGraph(Graph *graph, const std::string &filename, PluginProgress *progress,
                    DataSet *data) {
  ostream *os;

  bool gzip = false;

  string exportPluginName;
  list<string> &&exportPlugins = PluginLister::availablePlugins<ExportModule>();

  for (const string &pluginName : exportPlugins) {
    ExportModule *exportPlugin = PluginLister::getPluginObject<ExportModule>(pluginName);
    list<string> extensions(exportPlugin->fileExtensions());

    for (const string &ext : extensions) {
      if (filename.rfind(ext) == filename.length() - ext.length()) {
        exportPluginName = exportPlugin->name();
        gzip = ext != extensions.front();
        break;
      }
    }
    delete exportPlugin;
    if (!exportPluginName.empty())
      break;
  }

  if (exportPluginName.empty()) {
    string str("No suitable export plugin found. Exporting in TLP format.");

    if (progress)
      progress->setError(str);

    tlp::warning() << str << std::endl;
    exportPluginName = "TLP Export";
  }

  if (gzip) {
    os = tlp::getOgzstream(filename);
  } else {
    std::ios_base::openmode openMode = ios::out;

    if (exportPluginName == "TLPB Export") {
      openMode |= ios::binary;
    }

    os = tlp::getOutputFileStream(filename, openMode);
  }

  bool result = false;
  // check for open stream failure
  if (os->fail()) {
    std::stringstream sstr;
    sstr << "Unable to open " << filename << ": " << tlp::getStrError();
    if (progress)
      progress->setError(sstr.str());

    tlp::error() << exportPluginName << " failed - " << sstr.str() << std::endl;
  } else {
    DataSet ds;

    if (data != nullptr)
      ds = *data;

    ds.set("file", filename);
    result = tlp::exportGraph(graph, *os, exportPluginName, ds, progress);
  }
  delete os;
  return result;
}
//=========================================================
Graph *tlp::importGraph(const std::string &format, DataSet &dataSet, PluginProgress *progress,
                        Graph *graph) {

  if (!PluginLister::pluginExists(format)) {
    tlp::warning() << "libtulip: " << __FUNCTION__ << ": import plugin \"" << format
                   << "\" does not exist (or is not loaded)" << endl;
    return nullptr;
  }

  bool newGraphP = false;

  if (graph == nullptr) {
    graph = GraphImpl::newGraph();
    newGraphP = true;
  }

  bool deletePluginProgress = false;

  if (progress == nullptr) {
    progress = new SimplePluginProgress();
    deletePluginProgress = true;
  }

  AlgorithmContext context(graph, &dataSet, progress);
  ImportModule *importModule = PluginLister::getPluginObject<ImportModule>(format, &context);
  assert(importModule != nullptr);

  // ensure that the parsing of float or double does not depend on locale
  setlocale(LC_NUMERIC, "C");

  // If the import failed and we created the graph then delete the graph
  if (!importModule->importGraph()) {
    if (newGraphP)
      delete graph;

    graph = nullptr;
    if (!progress->getError().empty())
      tlp::error() << progress->getError() << std::endl;
  } else if (newGraphP) {
    importGraphObserversMtx.lock();
    for (auto ngo : importGraphObservers)
      ngo->graphImported(graph);
    importGraphObserversMtx.unlock();
  }

  if (deletePluginProgress)
    delete progress;

  delete importModule;
  dataSet = *context.dataSet;

  return graph;
}
//=========================================================
bool tlp::exportGraph(Graph *graph, std::ostream &outputStream, const std::string &format,
                      DataSet &dataSet, PluginProgress *progress) {
  if (!PluginLister::pluginExists(format)) {
    tlp::warning() << "libtulip: " << __FUNCTION__ << ": export plugin \"" << format
                   << "\" does not exist (or is not loaded)" << endl;
    return false;
  }

  bool result;
  bool deletePluginProgress = false;
  PluginProgress *tmpProgress = nullptr;

  if (progress == nullptr) {
    tmpProgress = new SimplePluginProgress();
    deletePluginProgress = true;
  } else
    tmpProgress = progress;

  AlgorithmContext context(graph, &dataSet, tmpProgress);
  ExportModule *exportModule = PluginLister::getPluginObject<ExportModule>(format, &context);
  assert(exportModule != nullptr);
  std::string filename;

  if (dataSet.get("file", filename)) {
    graph->setAttribute("file", filename);
  }

  result = exportModule->exportGraph(outputStream);

  if (deletePluginProgress)
    delete tmpProgress;

  delete exportModule;
  return result;
}
//=========================================================
static void removeFromGraph(Graph *g, const vector<node> &nodes, const std::vector<edge> &edges) {
  unsigned int nbNodes = nodes.size();
  unsigned int nbEdges = edges.size();

  // Clean properties
  for (auto p : g->getObjectProperties()) {
    for (unsigned int i = 0; i < nbNodes; i++)
      p->erase(nodes[i]);

    for (unsigned int i = 0; i < nbEdges; i++)
      p->erase(edges[i]);
  }

  // Remove edges
  for (unsigned int i = 0; i < nbEdges; i++)
    g->delEdge(edges[i]);

  // Remove nodes
  for (unsigned int i = 0; i < nbNodes; i++)
    g->delNode(nodes[i]);
}

void tlp::removeFromGraph(Graph *ioG, BooleanProperty *inSel) {
  if (!ioG)
    return;

  if (!inSel) {
    ::removeFromGraph(ioG, ioG->nodes(), ioG->edges());
    return;
  }

  vector<edge> edgeA;

  // get edges
  for (auto e : ioG->edges()) {
    if (inSel->getEdgeValue(e)) {
      // selected edge -> remove it !
      edgeA.push_back(e);
    } else {
      // unselected edge -> don't remove node ends !
      auto ends = ioG->ends(e);
      inSel->setNodeValue(ends.first, false);
      inSel->setNodeValue(ends.second, false);
    }
  }

  vector<node> nodeA;
  // get nodes
  for (auto n : ioG->nodes()) {
    if (inSel->getNodeValue(n)) {
      // selected node -> remove it !
      nodeA.push_back(n);
    }
  }

  ::removeFromGraph(ioG, nodeA, edgeA);
}

void tlp::copyToGraph(Graph *outG, const Graph *inG, BooleanProperty *inSel,
                      BooleanProperty *outSel) {
  if (outSel) {
    outSel->setAllNodeValue(false);
    outSel->setAllEdgeValue(false);
  }

  if (!outG || !inG)
    return;

  // extend the selection to edge ends
  if (inSel) {
    for (auto e : inSel->getNonDefaultValuatedEdges(inG)) {
      auto eEnds = inG->ends(e);
      inSel->setNodeValue(eEnds.first, true);
      inSel->setNodeValue(eEnds.second, true);
    }
  }

  // the selected nodes
  Iterator<node> *nodeIt = nullptr;
  // the number of selected nodes
  unsigned int nbSelNodes = 0;

  if (inSel) {
    nodeIt = inSel->getNonDefaultValuatedNodes(inG);
    if (!nodeIt->hasNext()) {
      delete nodeIt;
      return;
    }
    nbSelNodes = inSel->numberOfNonDefaultValuatedNodes(inG);
  } else {
    nodeIt = inG->getNodes();
    if (!nodeIt->hasNext()) {
      delete nodeIt;
      return;
    }
    nbSelNodes = inG->numberOfNodes();
  }

  // reserve space for nodes
  outG->reserveNodes(outG->numberOfNodes() + nbSelNodes);

  // get properties
  std::vector<std::pair<PropertyInterface *, PropertyInterface *>> properties;
  for (PropertyInterface *src : inG->getObjectProperties()) {
    if (dynamic_cast<GraphProperty *>(src) == nullptr) {
      const std::string &pName = src->getName();
      PropertyInterface *dst =
          outG->existProperty(pName) ? outG->getProperty(pName) : src->clonePrototype(outG, pName);
      properties.emplace_back(src, dst);
    }
  }
  unsigned int nbProperties = properties.size();

  MutableContainer<node> nodeTrl;
  nodeTrl.setAll(node());
  // loop on nodes
  for (auto nIn : nodeIt) {
    // add outG corresponding node
    node nOut = outG->addNode();

    // select added node
    if (outSel)
      outSel->setNodeValue(nOut, true);

    // add to translation tab
    nodeTrl.set(nIn.id, nOut);

    // copy node properties
    for (unsigned int i = 0; i < nbProperties; ++i) {
      std::pair<PropertyInterface *, PropertyInterface *> &props = properties[i];
      props.second->copy(nOut, nIn, props.first);
    }
  }

  // get selected edges
  Iterator<edge> *edgeIt = nullptr;

  if (inSel) {
    edgeIt = inSel->getNonDefaultValuatedEdges(inG);
    outG->reserveEdges(outG->numberOfEdges() + inSel->numberOfNonDefaultValuatedEdges(inG));
  } else {
    edgeIt = inG->getEdges();
    outG->reserveEdges(outG->numberOfEdges() + inG->numberOfEdges());
  }

  // loop on edges
  for (auto eIn : edgeIt) {
    const pair<node, node> &eEnds = inG->ends(eIn);
    // add outG corresponding edge
    edge eOut = outG->addEdge(nodeTrl.get(eEnds.first.id), nodeTrl.get(eEnds.second.id));

    // select added edge
    if (outSel)
      outSel->setEdgeValue(eOut, true);

    // copy edge properties
    for (unsigned int i = 0; i < nbProperties; ++i) {
      std::pair<PropertyInterface *, PropertyInterface *> &props = properties[i];
      props.second->copy(eOut, eIn, props.first);
    }
  }
}

// define a class of Iterator to iterate the root graphs
class RootGraphsIterator : public tlp::Iterator<tlp::Graph *> {
  std::vector<Graph *> roots;
  Iterator<Graph *> *rootsIterator;

public:
  RootGraphsIterator() {
    const VectorGraph &ovg = tlp::Observable::getObservableGraph();

    // we iterate the observable graph nodes
    for (auto n : ovg.nodes()) {
      Graph *g = nullptr;

      if (tlp::Observable::isAlive(n) &&
          (g = dynamic_cast<Graph *>(tlp::Observable::getObject(n)))) {
        // check if g is a root graph
        if (g->getRoot() == g)
          roots.push_back(g);
      }
    }

    rootsIterator =
        new StlIterator<Graph *, std::vector<tlp::Graph *>::iterator>(roots.begin(), roots.end());
  }

  ~RootGraphsIterator() override {
    delete rootsIterator;
  }

  Graph *next() override {
    return rootsIterator->next();
  }

  bool hasNext() override {
    return rootsIterator->hasNext();
  }
};

Iterator<Graph *> *tlp::getRootGraphs() {
  return new RootGraphsIterator();
}

bool Graph::applyAlgorithm(const std::string &algorithm, std::string &errorMessage,
                           DataSet *parameters, PluginProgress *progress) {
  if (!PluginLister::pluginExists(algorithm)) {
    tlp::warning() << "libtulip: " << __FUNCTION__ << ": algorithm plugin \"" << algorithm
                   << "\" does not exist (or is not loaded)" << endl;
    return false;
  }

  bool result;
  bool deletePluginProgress = false;
  PluginProgress *tmpProgress;

  if (progress == nullptr) {
    tmpProgress = new SimplePluginProgress();
    deletePluginProgress = true;
  } else
    tmpProgress = progress;

  AlgorithmContext context(this, parameters, tmpProgress);
  Algorithm *algo = PluginLister::getPluginObject<Algorithm>(algorithm, &context);

  if ((result = algo->check(errorMessage))) {
    result = algo->run();

    if (!result)
      errorMessage = tmpProgress->getError();
  }

  delete algo;

  if (deletePluginProgress)
    delete tmpProgress;

  return result;
}

//=========================================================
bool tlp::Graph::applyPropertyAlgorithm(const std::string &algorithm, PropertyInterface *prop,
                                        std::string &errorMessage, tlp::DataSet *parameters,
                                        tlp::PluginProgress *progress) {
  bool result;

  // check if this is a subgraph of prop->graph
  if (getRoot() != prop->getGraph()) {
    tlp::Graph *currentGraph = this;

    while (currentGraph->getSuperGraph() != currentGraph) {
      if (currentGraph == prop->getGraph())
        break;

      currentGraph = currentGraph->getSuperGraph();
    }

    if (currentGraph != prop->getGraph()) {
      errorMessage = "The property parameter does not belong to the graph";
#ifndef NDEBUG
      tlp::error() << __PRETTY_FUNCTION__ << ": " << errorMessage << std::endl;
#endif
      return false;
    }
  }

  const auto it = circularCalls.find(algorithm);

  if (it != circularCalls.end() && it->second == prop) {
    errorMessage = std::string("Circular call of ") + __PRETTY_FUNCTION__;
#ifndef NDEBUG
    tlp::error() << errorMessage << std::endl;
#endif
    return false;
  }

  // nothing to do if the graph is empty
  if (isEmpty()) {
    errorMessage = "The graph is empty";
    return false;
  }

  tlp::PluginProgress *tmpProgress;

  if (progress == nullptr)
    tmpProgress = new tlp::SimplePluginProgress();
  else
    tmpProgress = progress;

  bool hasData = parameters != nullptr;

  if (!hasData)
    parameters = new tlp::DataSet();

  // add prop as result in dataset
  parameters->set("result", prop);

  tlp::AlgorithmContext context(this, parameters, tmpProgress);

  tlp::Observable::holdObservers();
  circularCalls[algorithm] = prop;
  Algorithm *algo = tlp::PluginLister::getPluginObject<PropertyAlgorithm>(algorithm, &context);

  if (algo != nullptr) {
    result = algo->check(errorMessage);

    if (result) {
      result = algo->run();

      if (!result)
        errorMessage = tmpProgress->getError();
    }

    delete algo;
  } else {
    errorMessage = algorithm + " - No algorithm available with this name";
#ifndef NDEBUG
    tlp::error() << __PRETTY_FUNCTION__ << ": " << errorMessage << std::endl;
#endif
    result = false;
  }

  circularCalls.erase(algorithm);
  tlp::Observable::unholdObservers();

  if (progress == nullptr)
    delete tmpProgress;

  if (hasData)
    // remove result from dataset
    parameters->remove("result");
  else
    delete parameters;

  return result;
}

tlp::node Graph::getSource() const {
  for (auto source : this->nodes()) {
    if (indeg(source) == 0)
      return source;
  }

  return node();
}

tlp::node Graph::getSink() const {
  for (auto sink : this->nodes()) {
    if (outdeg(sink) == 0)
      return sink;
  }

  return node();
}

DataType *Graph::getAttribute(const std::string &name) const {
  return getAttributes().getData(name);
}

void Graph::setAttribute(const std::string &name, const DataType *value) {
  notifyBeforeSetAttribute(name);
  getNonConstAttributes().setData(name, value);
  notifyAfterSetAttribute(name);
}

void Graph::notifyAddNode(const node n) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_NODE, n));
}

void Graph::notifyBeforeDelNode(const node n) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_DEL_NODE, n));
}

void Graph::notifyAfterDelNode(const node n) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_DEL_NODE, n));
}

void Graph::notifyAddEdge(const edge e) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_EDGE, e));
}

void Graph::notifyBeforeDelEdge(const edge e) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_DEL_EDGE, e));
}

void Graph::notifyAfterDelEdge(const edge e) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_DEL_EDGE, e));
}

void Graph::notifyReverseEdge(const edge e) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_REVERSE_EDGE, e));
}

void Graph::notifyBeforeSetEnds(const edge e) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_SET_ENDS, e, Event::TLP_INFORMATION));
}

void Graph::notifyAfterSetEnds(const edge e) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_SET_ENDS, e));
}

void Graph::notifyBeforeAddSubGraph(const Graph *graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_ADD_SUBGRAPH, graph));

  Graph *g = this;

  while (g != getRoot()) {
    g->notifyBeforeAddDescendantGraph(graph);
    g = g->getSuperGraph();
  }

  getRoot()->notifyBeforeAddDescendantGraph(graph);
}

void Graph::notifyAfterAddSubGraph(const Graph *graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_ADD_SUBGRAPH, graph));

  Graph *g = this;

  while (g != getRoot()) {
    g->notifyAfterAddDescendantGraph(graph);
    g = g->getSuperGraph();
  }

  getRoot()->notifyAfterAddDescendantGraph(graph);
}

void Graph::notifyBeforeDelSubGraph(const Graph *graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_DEL_SUBGRAPH, graph));

  Graph *g = this;

  while (g != getRoot()) {
    g->notifyBeforeDelDescendantGraph(graph);
    g = g->getSuperGraph();
  }

  getRoot()->notifyBeforeDelDescendantGraph(graph);
}
void Graph::notifyAfterDelSubGraph(const Graph *graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_DEL_SUBGRAPH, graph));

  Graph *g = this;

  while (g != getRoot()) {
    g->notifyAfterDelDescendantGraph(graph);
    g = g->getSuperGraph();
  }

  getRoot()->notifyAfterDelDescendantGraph(graph);
}

void Graph::notifyBeforeAddDescendantGraph(const Graph *graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_ADD_DESCENDANTGRAPH, graph));
}
void Graph::notifyAfterAddDescendantGraph(const Graph *graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_ADD_DESCENDANTGRAPH, graph));
}

void Graph::notifyBeforeDelDescendantGraph(const Graph *graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_DEL_DESCENDANTGRAPH, graph));
}
void Graph::notifyAfterDelDescendantGraph(const Graph *graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_DEL_DESCENDANTGRAPH, graph));
}

void Graph::notifyBeforeAddLocalProperty(const std::string &propName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_ADD_LOCAL_PROPERTY, propName));
}
void Graph::notifyAddLocalProperty(const std::string &propName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_LOCAL_PROPERTY, propName));
}

void Graph::notifyBeforeDelLocalProperty(const std::string &propName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY, propName,
                         Event::TLP_INFORMATION));
}

void Graph::notifyAfterDelLocalProperty(const std::string &propName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_DEL_LOCAL_PROPERTY, propName));
}

void Graph::notifyBeforeSetAttribute(const std::string &attName) {
  if (hasOnlookers())
    sendEvent(
        GraphEvent(*this, GraphEvent::TLP_BEFORE_SET_ATTRIBUTE, attName, Event::TLP_INFORMATION));
}

void Graph::notifyAfterSetAttribute(const std::string &attName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_SET_ATTRIBUTE, attName));
}

void Graph::notifyRemoveAttribute(const std::string &attName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_REMOVE_ATTRIBUTE, attName, Event::TLP_INFORMATION));
}

void Graph::notifyDestroy() {
  if (hasOnlookers()) {
    // the undo/redo mechanism has to simulate graph destruction
    Event evt(*this, Event::TLP_MODIFICATION);
    evt._type = Event::TLP_DELETE;
    sendEvent(evt);
  }
}

PropertyInterface *Graph::getLocalProperty(const std::string &propertyName,
                                           const std::string &propertyType) {
  if (propertyType.compare(DoubleProperty::propertyTypename) == 0) {
    return getLocalProperty<DoubleProperty>(propertyName);
  } else if (propertyType.compare(LayoutProperty::propertyTypename) == 0) {
    return getLocalProperty<LayoutProperty>(propertyName);
  } else if (propertyType.compare(StringProperty::propertyTypename) == 0) {
    return getLocalProperty<StringProperty>(propertyName);
  } else if (propertyType.compare(IntegerProperty::propertyTypename) == 0) {
    return getLocalProperty<IntegerProperty>(propertyName);
  } else if (propertyType.compare(ColorProperty::propertyTypename) == 0) {
    return getLocalProperty<ColorProperty>(propertyName);
  } else if (propertyType.compare(SizeProperty::propertyTypename) == 0) {
    return getLocalProperty<SizeProperty>(propertyName);
  } else if (propertyType.compare(BooleanProperty::propertyTypename) == 0) {
    return getLocalProperty<BooleanProperty>(propertyName);
  } else if (propertyType.compare(DoubleVectorProperty::propertyTypename) == 0) {
    return getLocalProperty<DoubleVectorProperty>(propertyName);
  } else if (propertyType.compare(StringVectorProperty::propertyTypename) == 0) {
    return getLocalProperty<StringVectorProperty>(propertyName);
  } else if (propertyType.compare(IntegerVectorProperty::propertyTypename) == 0) {
    return getLocalProperty<IntegerVectorProperty>(propertyName);
  } else if (propertyType.compare(CoordVectorProperty::propertyTypename) == 0) {
    return getLocalProperty<CoordVectorProperty>(propertyName);
  } else if (propertyType.compare(ColorVectorProperty::propertyTypename) == 0) {
    return getLocalProperty<ColorVectorProperty>(propertyName);
  } else if (propertyType.compare(BooleanVectorProperty::propertyTypename) == 0) {
    return getLocalProperty<BooleanVectorProperty>(propertyName);
  } else if (propertyType.compare(SizeVectorProperty::propertyTypename) == 0) {
    return getLocalProperty<SizeVectorProperty>(propertyName);
  } else if (propertyType.compare(GraphProperty::propertyTypename) == 0) {
    return getLocalProperty<GraphProperty>(propertyName);
  } else {
    return nullptr;
  }
}

PropertyInterface *Graph::getProperty(const std::string &propertyName,
                                      const std::string &propertyType) {
  if (propertyType.compare(DoubleProperty::propertyTypename) == 0) {
    return getProperty<DoubleProperty>(propertyName);
  } else if (propertyType.compare(LayoutProperty::propertyTypename) == 0) {
    return getProperty<LayoutProperty>(propertyName);
  } else if (propertyType.compare(StringProperty::propertyTypename) == 0) {
    return getProperty<StringProperty>(propertyName);
  } else if (propertyType.compare(IntegerProperty::propertyTypename) == 0) {
    return getProperty<IntegerProperty>(propertyName);
  } else if (propertyType.compare(ColorProperty::propertyTypename) == 0) {
    return getProperty<ColorProperty>(propertyName);
  } else if (propertyType.compare(SizeProperty::propertyTypename) == 0) {
    return getProperty<SizeProperty>(propertyName);
  } else if (propertyType.compare(BooleanProperty::propertyTypename) == 0) {
    return getProperty<BooleanProperty>(propertyName);
  } else if (propertyType.compare(DoubleVectorProperty::propertyTypename) == 0) {
    return getProperty<DoubleVectorProperty>(propertyName);
  } else if (propertyType.compare(StringVectorProperty::propertyTypename) == 0) {
    return getProperty<StringVectorProperty>(propertyName);
  } else if (propertyType.compare(IntegerVectorProperty::propertyTypename) == 0) {
    return getProperty<IntegerVectorProperty>(propertyName);
  } else if (propertyType.compare(CoordVectorProperty::propertyTypename) == 0) {
    return getProperty<CoordVectorProperty>(propertyName);
  } else if (propertyType.compare(ColorVectorProperty::propertyTypename) == 0) {
    return getProperty<ColorVectorProperty>(propertyName);
  } else if (propertyType.compare(BooleanVectorProperty::propertyTypename) == 0) {
    return getProperty<BooleanVectorProperty>(propertyName);
  } else if (propertyType.compare(SizeVectorProperty::propertyTypename) == 0) {
    return getProperty<SizeVectorProperty>(propertyName);
  } else if (propertyType.compare(GraphProperty::propertyTypename) == 0) {
    return getProperty<GraphProperty>(propertyName);
  } else {
    return nullptr;
  }
}

static const string layoutProperty = "viewLayout";
static const string sizeProperty = "viewSize";
static const string rotationProperty = "viewRotation";
static const string colorProperty = "viewColor";

static void buildMapping(Iterator<node> *it, MutableContainer<node> &mapping,
                         GraphProperty *metaInfo, const node from = node()) {
  for (auto n : it) {
    if (!from.isValid())
      mapping.set(n.id, n);
    else
      mapping.set(n.id, from);

    Graph *meta = metaInfo->getNodeValue(n);

    if (meta != nullptr)
      buildMapping(meta->getNodes(), mapping, metaInfo, mapping.get(n.id));
  }
}
//====================================================================================
void updatePropertiesUngroup(Graph *graph, node metanode, GraphProperty *clusterInfo) {
  if (clusterInfo->getNodeValue(metanode) == nullptr)
    return; // The metanode is not a metanode.

  LayoutProperty *graphLayout = graph->getProperty<LayoutProperty>(layoutProperty);
  SizeProperty *graphSize = graph->getProperty<SizeProperty>(sizeProperty);
  DoubleProperty *graphRot = graph->getProperty<DoubleProperty>(rotationProperty);
  const Size &size = graphSize->getNodeValue(metanode);
  const Coord &pos = graphLayout->getNodeValue(metanode);
  const double &rot = graphRot->getNodeValue(metanode);
  Graph *cluster = clusterInfo->getNodeValue(metanode);
  LayoutProperty *clusterLayout = cluster->getProperty<LayoutProperty>(layoutProperty);
  SizeProperty *clusterSize = cluster->getProperty<SizeProperty>(sizeProperty);
  DoubleProperty *clusterRot = cluster->getProperty<DoubleProperty>(rotationProperty);
  BoundingBox &&box = tlp::computeBoundingBox(cluster, clusterLayout, clusterSize, clusterRot);
  double width = box.width();
  double height = box.height();
  double depth = box.depth();

  if (width < 0.0001)
    width = 1.0;

  if (height < 0.0001)
    height = 1.0;

  if (depth < 0.0001)
    depth = 1.0;

  // keep aspect ratio of content layout when opening a meta-node
  double divW = size[0] / width;
  double divH = size[1] / height;
  double scale = 1.0;

  if (divH * width > size[0]) {
    scale = divW;
  } else {
    scale = divH;
  }

  clusterLayout->translate(-box.center(), cluster);
  clusterLayout->rotateZ(graphRot->getNodeValue(metanode), cluster);
  clusterLayout->scale(Coord(scale, scale, size[2] / depth), cluster);
  clusterLayout->translate(pos, cluster);
  clusterSize->scale(Size(scale, scale, size[2] / depth), cluster);

  for (auto n : cluster->nodes()) {
    graphLayout->setNodeValue(n, clusterLayout->getNodeValue(n));
    graphSize->setNodeValue(n, clusterSize->getNodeValue(n));
    graphRot->setNodeValue(n, clusterRot->getNodeValue(n) + rot);
  }

  for (auto e : cluster->edges()) {
    graphLayout->setEdgeValue(e, clusterLayout->getEdgeValue(e));
    graphSize->setEdgeValue(e, clusterSize->getEdgeValue(e));
  }

  // propagate all cluster local properties
  for (PropertyInterface *property : cluster->getLocalObjectProperties()) {
    if (property == graphLayout || property == graphSize || property == graphRot)
      continue;

    PropertyInterface *graphProp = nullptr;

    if (graph->existProperty(property->getName()) &&
        graph->getProperty(property->getName())->getTypename() == property->getTypename())
      graphProp = graph->getProperty(property->getName());
    else
      graphProp = property->clonePrototype(graph, property->getName());

    for (auto n : cluster->nodes()) {
      graphProp->setNodeStringValue(n, property->getNodeStringValue(n));
    }

    for (auto e : cluster->edges()) {
      graphProp->setEdgeStringValue(e, property->getEdgeStringValue(e));
    }
  }
}
//=========================================================
Graph *Graph::addSubGraph(const std::string &name) {
  Graph *g = addSubGraph(nullptr, name);
  return g;
}
//=========================================================
Graph *Graph::addCloneSubGraph(const std::string &name, bool addSibling,
                               bool addSiblingProperties) {
  BooleanProperty selection(this);
  selection.setAllNodeValue(true);
  selection.setAllEdgeValue(true);
  Graph *parentSubGraph = this;

  if (addSibling) {
    parentSubGraph = getSuperGraph();

    if (this == parentSubGraph)
      // cannot add sibling of root graph
      return nullptr;
  }

  Graph *clone = parentSubGraph->addSubGraph(&selection, name);

  if (addSibling && addSiblingProperties) {
    for (PropertyInterface *prop : getLocalObjectProperties()) {
      PropertyInterface *cloneProp = prop->clonePrototype(clone, prop->getName());
      cloneProp->copy(prop);
    }
  }

  return clone;
}
//=========================================================
Graph *Graph::inducedSubGraph(const std::vector<node> &nodes, Graph *parentSubGraph,
                              const string &name) {
  if (parentSubGraph == nullptr)
    parentSubGraph = this;

  // create subgraph and add nodes
  Graph *result = parentSubGraph->addSubGraph(name);
  result->addNodes(nodes);

  for (auto n : nodes) {
    for (auto e : getOutEdges(n)) {
      if (result->isElement(target(e)))
        result->addEdge(e);
    }
  }

  return result;
}
//=========================================================
Graph *Graph::inducedSubGraph(BooleanProperty *selection, Graph *parentSubGraph,
                              const string &name) {
  vector<node> nodes;
  for (auto n : selection->getNodesEqualTo(true, parentSubGraph)) {
    nodes.push_back(n);
  }
  for (auto e : selection->getEdgesEqualTo(true, parentSubGraph)) {
    auto ext = ends(e);
    nodes.push_back(ext.first);
    nodes.push_back(ext.second);
  }
  return inducedSubGraph(nodes, parentSubGraph, name);
}
//====================================================================================
node Graph::createMetaNode(const std::vector<node> &nodes, bool multiEdges, bool delAllEdge,
                           bool allGrouped) {
  if (getRoot() == this) {
    tlp::warning() << __PRETTY_FUNCTION__ << std::endl;
    tlp::warning() << "\t Error: Could not group a set of nodes in the root graph" << std::endl;
    return node();
  }

  if (nodes.empty()) {
    tlp::warning() << __PRETTY_FUNCTION__ << std::endl;
    tlp::warning() << '\t' << "Warning: Creation of an empty metagraph" << std::endl;
  }

  // create an induced brother sub graph
  Graph *subGraph = inducedSubGraph(nodes, getSuperGraph());
  // all local properties
  // must be cloned in subgraph
  for (PropertyInterface *prop : getLocalObjectProperties()) {
    PropertyInterface *sgProp = prop->clonePrototype(subGraph, prop->getName());
    for (auto n : nodes) {
      DataMem *val = prop->getNodeDataMemValue(n);
      sgProp->setNodeDataMemValue(n, val);
      delete val;
    }
  }

  stringstream st;
  st << "grp_" << setfill('0') << setw(5) << subGraph->getId();
  subGraph->setAttribute("name", st.str());
  return createMetaNode(subGraph, multiEdges, delAllEdge, allGrouped);
}
//====================================================================================
#define NEED_TODEL 2
#define NO_NEED_TODEL 0
#define CHECK_TODEL 1
node Graph::createMetaNode(Graph *subGraph, bool multiEdges, bool edgeDelAll, bool allGrouped) {
  if (getRoot() == this) {
    tlp::warning() << __PRETTY_FUNCTION__ << std::endl;
    tlp::warning() << "\t Error: Could not create a meta node in the root graph" << std::endl;
    return node();
  }

  GraphProperty *metaInfo = static_cast<GraphAbstract *>(getRoot())->getMetaGraphProperty();
  node metaNode = addNode();
  metaInfo->setNodeValue(metaNode, subGraph);
  Observable::holdObservers();

  // updateGroupLayout(this, subGraph, metaNode);
  // compute meta node values
  for (PropertyInterface *property : getObjectProperties()) {
    property->computeMetaValue(metaNode, subGraph, this);
  }

  // keep track of graph existing edges
  MutableContainer<bool> graphEdges;
  graphEdges.setAll(false);

  for (auto e : edges())
    graphEdges.set(e.id, true);

  // we can now remove nodes from graph
  delNodes(subGraph->nodes());

  // create new meta edges from nodes to metanode
  Graph *super = getSuperGraph();
  tlp_hash_map<node, bool> hasEdges;
  tlp_hash_map<node, edge> metaEdges;
  tlp_hash_map<edge, set<edge>> subEdges;
  // needed for the !multiEdges and !allGrouped case
  tlp_hash_map<node, bool> hasInvEdges;
  tlp_hash_map<node, edge> metaInvEdges;

  for (auto n : subGraph->nodes()) {
    for (auto e : getSuperGraph()->getInOutEdges(n)) {
      auto eEnds = ends(e);
      node src = eEnds.first;
      node tgt = eEnds.second;
      unsigned int toDelete = isElement(src);

      if (toDelete && subGraph->isElement(tgt)) {
        if (multiEdges || !hasEdges[src]) {
          // add new meta edge
          edge metaEdge = addEdge(src, metaNode);

          if (!graphEdges.get(e.id))
            delEdge(metaEdge);

          // e is a sub-edge of metaEdge
          subEdges[metaEdge].insert(e);

          if (!multiEdges)
            // record metaEdge
            metaEdges[src] = metaEdge;

          if (!super->isElement(metaEdge))
            super->addEdge(metaEdge);
        } else if (!multiEdges)
          // e is a sub-edge of an already created meta edge
          subEdges[metaEdges[src]].insert(e);

        hasEdges[src] = true;

        if (((metaInfo->getNodeValue(src) != nullptr) ||
             (metaInfo->getNodeValue(tgt) != nullptr)) &&
            existEdge(src, tgt).isValid()) {
          toDelete = NEED_TODEL;
          delEdge(e, edgeDelAll);
        }
      }

      if (isElement(tgt) && subGraph->isElement(src)) {
        tlp_hash_map<node, bool> *hasLinks;
        tlp_hash_map<node, edge> *metaLinks;
        if (multiEdges || allGrouped) {
          hasLinks = &hasEdges;
          metaLinks = &metaEdges;
        } else {
          hasLinks = &hasInvEdges;
          metaLinks = &metaInvEdges;
        }
        if (multiEdges || !(*hasLinks)[tgt]) {
          // add new meta edge
          edge metaEdge = addEdge(metaNode, tgt);

          if (!graphEdges.get(e.id))
            delEdge(metaEdge);

          // e is a sub-edge of metaEdge
          subEdges[metaEdge].insert(e);

          if (!multiEdges)
            // record metaEdge
            (*metaLinks)[tgt] = metaEdge;

          if (!super->isElement(metaEdge))
            super->addEdge(metaEdge);
        } else if (!multiEdges)
          // e is a sub-edge of an already created meta edge
          subEdges[(*metaLinks)[tgt]].insert(e);

        (*hasLinks)[tgt] = true;

        if (toDelete == CHECK_TODEL)
          toDelete = ((metaInfo->getNodeValue(src) != nullptr) ||
                      (metaInfo->getNodeValue(tgt) != nullptr)) &&
                     existEdge(src, tgt).isValid();

        if (toDelete) {
          delEdge(e, edgeDelAll);
        }
      }
    }
  }

  // update metaInfo of new meta edges
  for (auto it = subEdges.begin(); it != subEdges.end(); ++it) {
    edge mE = it->first;
    metaInfo->setEdgeValue(mE, it->second);
    // compute meta edge values
    for (PropertyInterface *property : getObjectProperties()) {
      Iterator<edge> *itE = getEdgeMetaInfo(mE);
      assert(itE->hasNext());
      property->computeMetaValue(mE, itE, this);
      delete itE;
    }
  }

  Observable::unholdObservers();
  return metaNode;
}

// map all nodes or embedded nodes (nodes of meta node subgraph)
// of a subgraph to metaNode
static void mapSubGraphNodes(Graph *sg, node metaNode, MutableContainer<node> &mappingM,
                             GraphProperty *metaInfo) {
  for (auto n : sg->nodes()) {
    mappingM.set(n.id, metaNode);
    Graph *ssg = metaInfo->getNodeValue(n);

    if (ssg)
      mapSubGraphNodes(ssg, metaNode, mappingM, metaInfo);
  }
}
//====================================================================================
void Graph::openMetaNode(node metaNode, bool updateProperties) {
  if (getRoot() == this) {
    tlp::warning() << __PRETTY_FUNCTION__ << std::endl;
    tlp::warning() << "\t Error: Could not ungroup a meta node in the root graph" << std::endl;
    return;
  }

  GraphProperty *metaInfo = static_cast<GraphAbstract *>(getRoot())->getMetaGraphProperty();
  Graph *metaGraph = metaInfo->getNodeValue(metaNode);

  if (metaGraph == nullptr)
    return;

  Observable::holdObservers();
  MutableContainer<node> mappingM;
  mappingM.setAll(node());
  // add node from meta to graph
  {
    const vector<node> &nodes = metaGraph->nodes();
    unsigned int nbNodes = nodes.size();
    // stable in case of fractal graph
    for (unsigned int i = 0; i < nbNodes; ++i) {
      node n = nodes[i];
      addNode(n);
      mappingM.set(n.id, n);
      Graph *sg = metaInfo->getNodeValue(n);

      if (sg)
        // map all nodes or embedded nodes
        // of this subgraph to n
        mapSubGraphNodes(sg, n, mappingM, metaInfo);
    }
    addEdges(metaGraph->edges());
  }

  if (updateProperties)
    updatePropertiesUngroup(this, metaNode, metaInfo);

  // check for edges from or to the meta node
  Graph *super = getSuperGraph();
  Iterator<edge> *metaEdges = super->getInOutEdges(metaNode);

  if (!metaEdges->hasNext()) {
    delete metaEdges;
    // no edge so just remove the meta node
    getRoot()->delNode(metaNode, true);
    Observable::unholdObservers();
    return;
  }

  bool hasSubEdges = super->isMetaEdge(metaEdges->next());
  delete metaEdges;

  ColorProperty *graphColors = getProperty<ColorProperty>(colorProperty);

  if (hasSubEdges) {
    // compute mapping for neighbour nodes
    // and their sub nodes
    for (auto mn : super->getInOutNodes(metaNode)) {
      mappingM.set(mn.id, mn);
      Graph *mnGraph = metaInfo->getNodeValue(mn);

      if (mnGraph != nullptr) {
        for (auto mnn : mnGraph->nodes()) {
          mappingM.set(mnn.id, mn);
        }
      }
    }

    for (auto metaEdge : super->allEdges(metaNode)) {
      if (!super->isElement(metaEdge))
        continue;
      Color metaColor = graphColors->getEdgeValue(metaEdge);
      tlp_hash_map<node, tlp_hash_map<node, set<edge>>> newMetaEdges;

      for (auto e : getEdgeMetaInfo(metaEdge)) {
        auto eEnds = super->ends(e);

        if (isElement(eEnds.first)) {
          if (isElement(eEnds.second) && isElement(metaEdge)) {
            addEdge(e);
            graphColors->setEdgeValue(e, metaColor);
          } else if (eEnds.first != metaNode) {
            node tgt = mappingM.get(eEnds.second.id);

            // tgt may not be valid because at this time
            // when deleting a node from a subgraph pointed
            // by a metanode we are not able to correctly
            // update the meta edges embedding the inout edges
            // of this node
            if (tgt.isValid())
              newMetaEdges[eEnds.first][tgt].insert(e);
          }
        } else if (eEnds.second != metaNode) {
          node src = mappingM.get(eEnds.first.id);

          // src may not be valid because at this time
          // when deleting a node from a subgraph pointed
          // by a metanode we are not able to correctly
          // update the meta edges embedding the inout edges
          // of this node
          if (src.isValid())
            newMetaEdges[src][eEnds.second].insert(e);
        }
      }

      // iterate on newMetaEdges
      auto itme = newMetaEdges.begin();

      while (itme != newMetaEdges.end()) {
        node src = itme->first;
        auto itnme = itme->second.begin();
        auto itnmeEnd = itme->second.end();

        while (itnme != itnmeEnd) {
          Graph *graph = this;
          node tgt(itnme->first);

          // add an edge in the relevant graph
          if (!isElement(src) || !isElement(tgt))
            graph = super;

          edge mE = graph->addEdge(src, tgt);
          metaInfo->setEdgeValue(mE, itnme->second);
          // compute meta edge values
          for (PropertyInterface *property : graph->getObjectProperties()) {
            Iterator<edge> *itE = getEdgeMetaInfo(mE);
            assert(itE->hasNext());
            property->computeMetaValue(mE, itE, graph);
            delete itE;
          }
          ++itnme;
        }

        ++itme;
      }
    }

    getRoot()->delNode(metaNode, true);
  } else {
    MutableContainer<node> mappingC;
    MutableContainer<node> mappingN;
    mappingC.setAll(node());
    mappingN.setAll(node());
    Graph *root = getRoot();
    buildMapping(root->getInOutNodes(metaNode), mappingC, metaInfo, node());
    buildMapping(metaGraph->getNodes(), mappingN, metaInfo, node());

    tlp_hash_map<node, Color> metaEdgeToColor;

    for (auto metaEdge : super->getInOutEdges(metaNode)) {
      metaEdgeToColor[opposite(metaEdge, metaNode)] = graphColors->getEdgeValue(metaEdge);
    }

    // Remove the metagraph from the hierarchy and remove the metanode
    root->delNode(metaNode, true);
    tlp_hash_map<node, std::unordered_set<node>> edges;
    //=================================
    for (auto e : root->edges()) {

      if (isElement(e))
        continue;

      auto eEnds = root->ends(e);
      unsigned int srcId = eEnds.first.id;
      unsigned int tgtId = eEnds.second.id;
      node sourceC = mappingC.get(srcId);
      node targetN = mappingN.get(tgtId);
      node sourceN = mappingN.get(srcId);
      node targetC = mappingC.get(tgtId);
      node src, tgt;
      Color edgeColor;

      if (sourceC.isValid() && targetN.isValid()) {
        src = sourceC;
        tgt = targetN;
        edgeColor = metaEdgeToColor[src];
      } else {
        if (sourceN.isValid() && targetC.isValid()) {
          src = sourceN;
          tgt = targetC;
          edgeColor = metaEdgeToColor[tgt];
        } else
          continue;
      }

      if (metaInfo->getNodeValue(src) == nullptr && metaInfo->getNodeValue(tgt) == nullptr) {
        addEdge(e);
        continue;
      }

      if ((edges.find(src) == edges.end()) || (edges[src].find(tgt) == edges[src].end())) {
        edges[src].insert(tgt);

        if (!existEdge(src, tgt).isValid()) {
          edge addedEdge = addEdge(src, tgt);
          graphColors->setEdgeValue(addedEdge, edgeColor);
        } else
          tlp::error() << __PRETTY_FUNCTION__ << ": bug exist edge 1" << std::endl;
      }
    }
  }

  Observable::unholdObservers();
}
//====================================================================================
struct MetaEdge {
  unsigned int source, target;
  edge mE;
};

void Graph::createMetaNodes(Iterator<Graph *> *itS, Graph *quotientGraph, vector<node> &metaNodes,
                            bool inoutGrouped) {
  GraphProperty *metaInfo = static_cast<GraphAbstract *>(getRoot())->getMetaGraphProperty();
  tlp_hash_map<edge, set<edge>> eMapping;
  Observable::holdObservers();
  {
    tlp_hash_map<node, set<node>> nMapping;

    while (itS->hasNext()) {
      Graph *its = itS->next();

      if (its != quotientGraph) {
        // Create one metanode for each subgraph(cluster)
        node metaN = quotientGraph->addNode();
        metaNodes.push_back(metaN);
        metaInfo->setNodeValue(metaN, its);
        // compute meta node values
        for (PropertyInterface *property : quotientGraph->getObjectProperties()) {
          property->computeMetaValue(metaN, its, quotientGraph);
        }
        for (auto n : its->nodes()) {
          // map each subgraph's node to a set of meta nodes
          // in order to deal consistently with overlapping clusters
          nMapping[n].insert(metaN);
        }
      }
    }

    {
      // define a contextual comparator
      // to manage comparison according inoutGrouped value
      // inoutGrouped = true implies 2 meta-edges
      // corresponding to in/out direction of underlying edges
      auto cmp = [inoutGrouped](const MetaEdge &m1, const MetaEdge &m2) {
        if (!inoutGrouped) {
          if (m1.source == m2.target)
            return m1.target < m2.source;
          if (m1.target == m2.source)
            return m1.source < m2.target;
        }
        return (m1.source < m2.source) || ((m1.source == m2.source) && (m1.target < m2.target));
      };

      set<MetaEdge, decltype(cmp)> myQuotientGraph(cmp);

      // for each existing edge in the current graph
      // add a meta edge for the corresponding couple
      // (meta source, meta target) if it does not already exists
      // and register the edge as associated to this meta edge
      const vector<edge> &edges = this->edges();
      unsigned int nbEdges = edges.size();
      for (unsigned int i = 0; i < nbEdges; ++i) {
        edge e = edges[i];
        auto eEnds = ends(e);
        set<node> &metaSources = nMapping[eEnds.first];
        set<node> &metaTargets = nMapping[eEnds.second];

        for (auto mSource : metaSources) {
          for (auto mTarget : metaTargets) {
            if (mSource != mTarget) {
              MetaEdge tmp;
              tmp.source = mSource.id, tmp.target = mTarget.id;
              const auto itm = myQuotientGraph.find(tmp);

              if (itm == myQuotientGraph.end()) {
                edge mE = quotientGraph->addEdge(mSource, mTarget);
                tmp.mE = mE;
                myQuotientGraph.insert(tmp);
                eMapping[mE].insert(e);
              } else {
                // add edge
                eMapping[itm->mE].insert(e);
              }
            }
          }
        }
      }
    }
  }
  // set viewMetaGraph for added meta edges
  auto itm = eMapping.begin();

  while (itm != eMapping.end()) {
    edge mE = itm->first;
    metaInfo->setEdgeValue(mE, itm->second);
    // compute meta edge values
    for (auto prop : quotientGraph->getObjectProperties()) {
      Iterator<edge> *itE = getRoot()->getEdgeMetaInfo(mE);
      prop->computeMetaValue(mE, itE, quotientGraph);
      delete itE;
    }
    ++itm;
  }

  Observable::unholdObservers();
}

Graph *Graph::getNthSubGraph(unsigned int n) const {
  unsigned int i = 0;
  for (tlp::Graph *sg : subGraphs()) {
    if (i++ == n) {
      return sg;
    }
  }
  return nullptr;
}

const std::string &GraphEvent::getPropertyName() const {
  assert((evtType > TLP_AFTER_DEL_SUBGRAPH && evtType < TLP_BEFORE_SET_ATTRIBUTE) ||
         evtType > TLP_REMOVE_ATTRIBUTE);

  if (evtType == TLP_BEFORE_RENAME_LOCAL_PROPERTY || evtType == TLP_AFTER_RENAME_LOCAL_PROPERTY)
    return info.renamedProp->first->getName();

  return *(info.name);
}

void Graph::addNodes(const std::vector<node> &nodes) {
  StlIterator<node, vector<node>::const_iterator> vIterator(nodes.begin(), nodes.end());
  addNodes(&vIterator);
}

void Graph::delNodes(const std::vector<node> &nodes, bool deleteInAllGraphs) {
  StlIterator<node, vector<node>::const_iterator> vIterator(nodes.begin(), nodes.end());
  delNodes(&vIterator, deleteInAllGraphs);
}

void Graph::addEdges(const std::vector<edge> &edges) {
  StlIterator<edge, vector<edge>::const_iterator> vIterator(edges.begin(), edges.end());
  addEdges(&vIterator);
}

void Graph::delEdges(const std::vector<edge> &edges, bool deleteInAllGraphs) {
  StlIterator<edge, vector<edge>::const_iterator> vIterator(edges.begin(), edges.end());
  delEdges(&vIterator, deleteInAllGraphs);
}

struct DescendantGraphsIterator : public Iterator<Graph *> {
  // use a stack to store non empty iterators
  stack<Iterator<Graph *> *> iterators;
  // the current one
  Iterator<Graph *> *current;

  DescendantGraphsIterator(const Graph *g) {
    current = g->getSubGraphs();

    if (!current->hasNext()) {
      delete current;
      current = nullptr;
    }
  }

  ~DescendantGraphsIterator() override {
    if (current)
      delete current;

    while (!iterators.empty()) {
      delete iterators.top();
      iterators.pop();
    }
  }

  bool hasNext() override {
    return current != nullptr;
  }

  Graph *next() override {
    if (current) {
      Graph *g = current->next();
      Iterator<Graph *> *itg = g->getSubGraphs();

      if (itg->hasNext()) {
        if (current->hasNext())
          // pushed iterators are always non empty
          iterators.push(current);
        else
          delete current;

        current = itg;
      } else {
        delete itg;

        if (!current->hasNext()) {
          delete current;

          if (!iterators.empty()) {
            current = iterators.top();
            iterators.pop();
          } else
            current = nullptr;
        }
      }

      return g;
    }

    return nullptr;
  }
};

Iterator<Graph *> *Graph::getDescendantGraphs() const {
  return new DescendantGraphsIterator(this);
}

// destructor
GraphEvent::~GraphEvent() {
  if (evtType > TLP_AFTER_DEL_SUBGRAPH) {
    // need to cleanup name if any
    if (evtType == TLP_BEFORE_RENAME_LOCAL_PROPERTY || evtType == TLP_AFTER_RENAME_LOCAL_PROPERTY)
      delete info.renamedProp;
    else
      delete info.name;
  } else {
    //  need to cleanup vectInfos if not null
    if (evtType == TLP_ADD_NODES && vectInfos.addedNodes)
      delete vectInfos.addedNodes;
    else if (evtType == TLP_ADD_EDGES && vectInfos.addedEdges)
      delete vectInfos.addedEdges;
  }
}

const std::vector<node> &GraphEvent::getNodes() const {
  assert(evtType == TLP_ADD_NODES);

  if (vectInfos.addedNodes == nullptr) {
    unsigned int nbElts = info.nbElts;
    std::vector<node> *addedNodes = new std::vector<node>();
    addedNodes->reserve(nbElts);
    const std::vector<node> &nodes = getGraph()->nodes();
    // copy new graph nodes in addedNodes reserved memory
    memcpy(addedNodes->data(), &nodes[nodes.size() - nbElts], nbElts * sizeof(node));
    // set addedNodes size using underlying vector pointers
    // to avoid unnecessary multiple constructeur calls
    reinterpret_cast<node **>(addedNodes)[1] = reinterpret_cast<node **>(addedNodes)[0] + nbElts;
    // record allocated vector in vectInfos
    const_cast<GraphEvent *>(this)->vectInfos.addedNodes = addedNodes;
  }

  return *vectInfos.addedNodes;
}

const std::vector<edge> &GraphEvent::getEdges() const {
  assert(evtType == TLP_ADD_EDGES);

  if (vectInfos.addedEdges == nullptr) {
    unsigned int nbElts = info.nbElts;
    std::vector<edge> *addedEdges = new std::vector<edge>();
    addedEdges->reserve(nbElts);
    const std::vector<edge> &edges = getGraph()->edges();
    // copy new graph edges in addedEdges reserved memory
    memcpy(addedEdges->data(), &edges[edges.size() - nbElts], nbElts * sizeof(edge));
    // set addedEdges size using underlying vector pointers
    // to avoid unnecessary multiple constructeur calls
    reinterpret_cast<edge **>(addedEdges)[1] = reinterpret_cast<edge **>(addedEdges)[0] + nbElts;
    // record allocated vector in vectInfos
    const_cast<GraphEvent *>(this)->vectInfos.addedEdges = addedEdges;
  }

  return *vectInfos.addedEdges;
}
