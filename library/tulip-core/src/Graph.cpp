/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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
#include <fstream>
#include <stack>

#include <tulip/ForEach.h>
#include <tulip/StlIterator.h>
#include <tulip/TlpTools.h>
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

using namespace std;
using namespace tlp;

ostream & operator << (ostream &os,const Graph *sp) {
  os << ";(nodes <node_id> <node_id> ...)" << endl;
  os << "(nodes ";
  node beginNode, previousNode;
  Iterator<node> *itn=sp->getNodes();

  while (itn->hasNext()) {
    node current = itn->next();

    if (!beginNode.isValid()) {
      beginNode = previousNode = current;
      os << beginNode.id;
    }
    else {
      if (current.id == previousNode.id + 1) {
        previousNode = current;

        if (!itn->hasNext())
          os << ".." << current.id;
      }
      else {
        if (previousNode != beginNode) {
          os << ".." << previousNode.id;
        }

        os  << " " << current.id;
        beginNode = previousNode = current;
      }
    }
  }

  delete itn;
  os << ")" << endl;
  os << ";(edge <edge_id> <source_id> <target_id>)" << endl;
  Iterator<edge> *ite=sp->getEdges();

  for (; ite->hasNext();) {
    edge e=ite->next();
    os << "(edge " << e.id << " " << sp->source(e).id << " " << sp->target(e).id << ")";

    if (ite->hasNext()) os << endl;
  }

  delete ite;
  os << endl;
  return os;
}

static void setViewPropertiesDefaults(Graph *g) {

  const std::string shapes = "viewShape", colors = "viewColor", sizes = "viewSize", metrics = "viewMetric", fonts = "viewFont", fontSizes = "viewFontSize",
                    borderWidth = "viewBorderWidth", borderColor = "viewBorderColor", tgtShape = "viewTgtAnchorShape", srcShape = "viewSrcAnchorShape",
                    labelColor = "viewLabelColor", labelBorderColor = "viewLabelBorderColor", labelBorderWidth = "viewLabelBorderWidth", labelPosition = "viewLabelPosition", label="viewLabel",
                    layout = "viewLayout", rotation = "viewRotation", srcAnchorSize = "viewSrcAnchorSize", selection = "viewSelection", texture = "viewTexture", tgtAnchorSize = "viewTgtAnchorSize";

  if (!g->existProperty(shapes)) {
    g->getProperty<IntegerProperty>(shapes)->setAllNodeValue(TulipViewSettings::instance().defaultShape(NODE));
    g->getProperty<IntegerProperty>(shapes)->setAllEdgeValue(TulipViewSettings::instance().defaultShape(EDGE));
  }

  if (!g->existProperty(colors)) {
    g->getProperty<ColorProperty>(colors)->setAllNodeValue(TulipViewSettings::instance().defaultColor(NODE));
    g->getProperty<ColorProperty>(colors)->setAllEdgeValue(TulipViewSettings::instance().defaultColor(EDGE));
  }

  if (!g->existProperty(sizes)) {
    g->getProperty<SizeProperty>(sizes)->setAllNodeValue(TulipViewSettings::instance().defaultSize(NODE));
    g->getProperty<SizeProperty>(sizes)->setAllEdgeValue(TulipViewSettings::instance().defaultSize(EDGE));
  }

  if (!g->existProperty(metrics)) {
    g->getProperty<DoubleProperty>(metrics)->setAllNodeValue(0);
    g->getProperty<DoubleProperty>(metrics)->setAllEdgeValue(0);
  }

  if (!g->existProperty(fonts)) {
    g->getProperty<StringProperty>(fonts)->setAllNodeValue(TulipViewSettings::instance().defaultFontFile());
    g->getProperty<StringProperty>(fonts)->setAllEdgeValue(TulipViewSettings::instance().defaultFontFile());
  }

  if (!g->existProperty(fontSizes)) {
    g->getProperty<IntegerProperty>(fontSizes)->setAllNodeValue(TulipViewSettings::instance().defaultFontSize());
    g->getProperty<IntegerProperty>(fontSizes)->setAllEdgeValue(TulipViewSettings::instance().defaultFontSize());
  }

  if (!g->existProperty(borderWidth)) {
    g->getProperty<DoubleProperty>(borderWidth)->setAllNodeValue(TulipViewSettings::instance().defaultBorderWidth(NODE));
    g->getProperty<DoubleProperty>(borderWidth)->setAllEdgeValue(TulipViewSettings::instance().defaultBorderWidth(EDGE));
  }

  if (!g->existProperty(borderColor)) {
    g->getProperty<ColorProperty>(borderColor)->setAllNodeValue(TulipViewSettings::instance().defaultBorderColor(NODE));
    g->getProperty<ColorProperty>(borderColor)->setAllEdgeValue(TulipViewSettings::instance().defaultBorderColor(EDGE));
  }

  if (!g->existProperty(tgtShape)) {
    g->getProperty<IntegerProperty>(tgtShape)->setAllEdgeValue(TulipViewSettings::instance().defaultEdgeExtremityTgtShape());
  }

  if (!g->existProperty(srcShape)) {
    g->getProperty<IntegerProperty>(srcShape)->setAllEdgeValue(TulipViewSettings::instance().defaultEdgeExtremitySrcShape());
  }

  if (!g->existProperty(labelColor)) {
    g->getProperty<ColorProperty>(labelColor)->setAllNodeValue(TulipViewSettings::instance().defaultLabelColor());
    g->getProperty<ColorProperty>(labelColor)->setAllEdgeValue(TulipViewSettings::instance().defaultLabelColor());
  }

  if (!g->existProperty(labelBorderColor)) {
    g->getProperty<ColorProperty>(labelBorderColor)->setAllNodeValue(TulipViewSettings::instance().defaultLabelBorderColor());
    g->getProperty<ColorProperty>(labelBorderColor)->setAllEdgeValue(TulipViewSettings::instance().defaultLabelBorderColor());
  }

  if (!g->existProperty(labelBorderWidth)) {
    g->getProperty<DoubleProperty>(labelBorderWidth)->setAllNodeValue(TulipViewSettings::instance().defaultLabelBorderWidth());
    g->getProperty<DoubleProperty>(labelBorderWidth)->setAllEdgeValue(TulipViewSettings::instance().defaultLabelBorderWidth());
  }

  if (!g->existProperty(labelPosition)) {
    g->getProperty<IntegerProperty>(labelPosition)->setAllNodeValue(TulipViewSettings::instance().defaultLabelPosition());
    g->getProperty<IntegerProperty>(labelPosition)->setAllEdgeValue(TulipViewSettings::instance().defaultLabelPosition());
  }

  if (!g->existProperty(layout)) {
    g->getProperty<LayoutProperty>(layout)->setAllNodeValue(Coord(0,0,0));
    g->getProperty<LayoutProperty>(layout)->setAllEdgeValue(std::vector<Coord>());
  }

  if (!g->existProperty(rotation)) {
    g->getProperty<DoubleProperty>(rotation)->setAllNodeValue(0);
    g->getProperty<DoubleProperty>(rotation)->setAllEdgeValue(0);
  }

  if (!g->existProperty(srcAnchorSize)) {
    g->getProperty<SizeProperty>(srcAnchorSize)->setAllEdgeValue(TulipViewSettings::instance().defaultEdgeExtremitySrcSize());
  }

  if (!g->existProperty(tgtAnchorSize)) {
    g->getProperty<SizeProperty>(tgtAnchorSize)->setAllEdgeValue(TulipViewSettings::instance().defaultEdgeExtremityTgtSize());
  }

  if (!g->existProperty(texture)) {
    g->getProperty<StringProperty>(texture)->setAllNodeValue("");
    g->getProperty<StringProperty>(texture)->setAllEdgeValue("");
  }

  if (!g->existProperty(label)) {
    g->getProperty<StringProperty>(label)->setAllNodeValue("");
    g->getProperty<StringProperty>(label)->setAllEdgeValue("");
  }

  if (!g->existProperty(selection)) {
    g->getProperty<BooleanProperty>(selection)->setAllNodeValue(false);
    g->getProperty<BooleanProperty>(selection)->setAllEdgeValue(false);
  }

}

//=========================================================
Graph * tlp::newGraph() {
  Graph *g = new GraphImpl();
  setViewPropertiesDefaults(g);
  return g;
}
//=========================================================
Graph * tlp::loadGraph(const std::string &filename, PluginProgress *progress) {
  DataSet dataSet;
  std::string algName = "TLP Import";

  /*if (filename.rfind(".json") == (filename.length() - 5))
    algName = "JSON Import";*/

  dataSet.set("file::filename", filename);
  Graph *graph = tlp::importGraph(algName, dataSet,progress);
  return graph;
}
//=========================================================
bool tlp::saveGraph(Graph* graph, const std::string& filename, PluginProgress *progress) {
  ostream *os;

  if (filename.rfind(".gz") == (filename.length() - 3))
    os =  tlp::getOgzstream(filename);
  else
    os = tlp::getOutputFileStream(filename);

  bool result;
  DataSet data;
  data.set("file", filename);
  result=tlp::exportGraph(graph, *os, "TLP Export", data, progress);
  delete os;
  return result;
}
//=========================================================
Graph * tlp::importGraph(const std::string &format, DataSet &dataSet, PluginProgress *progress, Graph *graph) {

  if (!PluginLister::pluginExists(format)) {
    tlp::warning() << "libtulip: " << __FUNCTION__ << ": import plugin \"" << format
                   << "\" does not exist (or is not loaded)" << endl;
    return NULL;
  }

  bool newGraphP=false;

  if (graph==NULL) {
    graph=tlp::newGraph();
    newGraphP=true;
  }

  PluginProgress *tmpProgress;
  bool deletePluginProgress=false;

  if (progress==NULL) {
    tmpProgress=new SimplePluginProgress();
    deletePluginProgress=true;
  }
  else tmpProgress=progress;

  AlgorithmContext* tmp = new AlgorithmContext(graph, &dataSet, tmpProgress);
  ImportModule *newImportModule = PluginLister::instance()->getPluginObject<ImportModule>(format, tmp);
  assert(newImportModule!=NULL);

  //If the import failed and we created the graph then delete the graph
  if (!newImportModule->importGraph()) {
    graph = NULL;

    if (newGraphP)
      delete graph;
  }
  else {
    std::string filename;

    if (dataSet.get("file::filename", filename)) {
      graph->setAttribute("file", filename);
    }
  }

  if (deletePluginProgress) delete tmpProgress;

  delete newImportModule;
  dataSet = *tmp->dataSet;

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
  bool deletePluginProgress=false;
  PluginProgress *tmpProgress=NULL;

  if (progress==NULL) {
    tmpProgress=new SimplePluginProgress();
    deletePluginProgress=true;
  }
  else tmpProgress=progress;

  AlgorithmContext* context = new AlgorithmContext(graph, &dataSet, tmpProgress);
  ExportModule *newExportModule=PluginLister::instance()->getPluginObject<ExportModule>(format, context);
  assert(newExportModule!=NULL);
  std::string filename;

  if (dataSet.get("file", filename)) {
    graph->setAttribute("file", filename);
  }

  result=newExportModule->exportGraph(outputStream);

  if (deletePluginProgress) delete tmpProgress;

  delete newExportModule;
  return result;
}
//=========================================================
void tlp::removeFromGraph(Graph *ioG, BooleanProperty *inSel) {
  if( !ioG )
    return;

  vector<node>  nodeA;
  vector<edge>  edgeA;

  // Get edges
  Iterator<edge> * edgeIt = ioG->getEdges();

  while( edgeIt->hasNext() ) {
    edge e = edgeIt->next();

    if( !inSel || inSel->getEdgeValue(e) ) {
      // selected edge -> remove it !
      edgeA.push_back( e );
    }
    else {
      // unselected edge -> don't remove node ends !
      node n0 = ioG->source( e );
      node n1 = ioG->target( e );
      assert( inSel );
      inSel->setNodeValue( n0, false );
      inSel->setNodeValue( n1, false );
    }
  }

  delete edgeIt;

  // Get nodes
  Iterator<node> * nodeIt = ioG->getNodes();

  while( nodeIt->hasNext() ) {
    node n = nodeIt->next();

    if( !inSel || inSel->getNodeValue(n) )
      nodeA.push_back( n );
  }

  delete nodeIt;

  // Clean properties
  Iterator<std::string> * propIt = ioG->getProperties();

  while( propIt->hasNext() ) {
    std::string n = propIt->next();
    PropertyInterface * p = ioG->getProperty( n );

    for( unsigned int in = 0 ; in < nodeA.size() ; in++ )
      p->erase( nodeA[in] );

    for( unsigned int ie = 0 ; ie < edgeA.size() ; ie++ )
      p->erase( edgeA[ie] );
  }

  delete propIt;

  // Remove edges
  for( unsigned int ie = 0 ; ie < edgeA.size() ; ie++ )
    ioG->delEdge( edgeA[ie] );

  // Remove nodes
  for( unsigned int in = 0 ; in < nodeA.size() ; in++ )
    ioG->delNode( nodeA[in] );
}

void tlp::copyToGraph (Graph *outG, const Graph* inG,
                       BooleanProperty *inSel, BooleanProperty* outSel) {
  if( outSel ) {
    outSel->setAllNodeValue( false );
    outSel->setAllEdgeValue( false );
  }

  if( !outG || !inG )
    return;

  // extend the selection to edge ends
  if( inSel ) {
    Iterator<edge> *itE = inSel->getNonDefaultValuatedEdges(inG);

    while (itE->hasNext()) {
      edge e = itE->next();
      const pair<node, node>& eEnds = inG->ends(e);
      inSel->setNodeValue(eEnds.first, true);
      inSel->setNodeValue(eEnds.second, true);
    }

    delete itE;
  }

  MutableContainer<node> nodeTrl;
  nodeTrl.setAll(node());
  // loop on selected nodes
  Iterator<node> * nodeIt =
    inSel ? inSel->getNonDefaultValuatedNodes(inG) : inG->getNodes();

  while( nodeIt->hasNext() ) {
    node nIn = nodeIt->next();
    // add outG corresponding node
    node nOut = outG->addNode();

    // select added node
    if( outSel )
      outSel->setNodeValue( nOut, true );

    // add to translation tab
    nodeTrl.set(nIn.id, nOut);
    // copy node properties
    Iterator<PropertyInterface *>* propIt = inG->getObjectProperties();

    while (propIt->hasNext()) {
      PropertyInterface *src = propIt->next();

      if (dynamic_cast<GraphProperty *>(src) == NULL) {
        const std::string& pName = src->getName();
        PropertyInterface *dst =
          outG->existProperty(pName) ? outG->getProperty(pName)
          : src->clonePrototype(outG, pName);
        dst->copy( nOut, nIn, src );
      }
    }

    delete propIt;
  }

  delete nodeIt;

  // loop on selected edges
  Iterator<edge> * edgeIt =
    inSel ? inSel->getNonDefaultValuatedEdges(inG) : inG->getEdges();

  while( edgeIt->hasNext() ) {
    edge eIn = edgeIt->next();
    const pair<node, node>& eEnds = inG->ends(eIn);
    // add outG correponding edge
    edge eOut = outG->addEdge(nodeTrl.get(eEnds.first.id),
                              nodeTrl.get(eEnds.second.id));

    // select added edge
    if( outSel )
      outSel->setEdgeValue( eOut, true );

    // copy edge properties
    Iterator<PropertyInterface *>* propIt = inG->getObjectProperties();

    while (propIt->hasNext()) {
      PropertyInterface *src = propIt->next();

      if (dynamic_cast<GraphProperty *>(src) == NULL) {
        const std::string& pName = src->getName();
        PropertyInterface *dst =
          outG->existProperty(pName) ? outG->getProperty(pName)
          : src->clonePrototype(outG, pName);
        dst->copy( eOut, eIn, src );
      }
    }

    delete propIt;
  }

  delete edgeIt;
}

// define a class of Iterator to iterate the root graphs
class RootGraphsIterator :public tlp::Iterator<tlp::Graph *> {
  std::vector<Graph *> roots;
  Iterator<Graph *>* rootsIterator;

public:
  RootGraphsIterator() {
    const VectorGraph& ovg = tlp::Observable::getObservableGraph();

    // we iterate the observable graph nodes
    node n;
    forEach(n, ovg.getNodes()) {
      Graph* g;

      if (tlp::Observable::isAlive(n) &&
          (g = dynamic_cast<Graph*>(tlp::Observable::getObject(n)))) {
        // check if g is a root graph
        if (g->getRoot() == g)
          roots.push_back(g);
      }
    }

    rootsIterator =
      new StlIterator<Graph*, std::vector<tlp::Graph*>::iterator>(roots.begin(),
          roots.end());
  }

  ~RootGraphsIterator() {
    delete rootsIterator;
  }

  Graph* next() {
    return rootsIterator->next();
  }

  bool hasNext() {
    return rootsIterator->hasNext();
  }
};

Iterator<Graph*>* tlp::getRootGraphs() {
  return new RootGraphsIterator();
}

bool Graph::applyAlgorithm(const std::string &algorithm,
                           std::string &errorMessage,
                           DataSet *dataSet, PluginProgress *progress) {
  if (!PluginLister::pluginExists(algorithm)) {
    tlp::warning() << "libtulip: " << __FUNCTION__ << ": algorithm plugin \"" << algorithm
                   << "\" does not exist (or is not loaded)" << endl;
    return false;
  }

  bool result;
  bool deletePluginProgress=false;
  PluginProgress *tmpProgress;

  if (progress == NULL) {
    tmpProgress = new SimplePluginProgress();
    deletePluginProgress = true;
  }
  else tmpProgress = progress;

  AlgorithmContext* context = new AlgorithmContext(this, dataSet, tmpProgress);
  Algorithm *newAlgo = PluginLister::instance()->getPluginObject<Algorithm>(algorithm, context);

  if ((result=newAlgo->check(errorMessage))) {
    result = newAlgo->run();

    if (!result)
      errorMessage = tmpProgress->getError();
  }

  delete newAlgo;
  delete context;

  if (deletePluginProgress)
    delete tmpProgress;

  return result;
}

//=========================================================
bool tlp::Graph::applyPropertyAlgorithm(const std::string &algorithm,
                                        PropertyInterface* prop,
                                        std::string &errorMessage,
                                        tlp::PluginProgress *progress,
                                        tlp::DataSet *parameters) {
  bool result;
  tlp::AlgorithmContext context;

  // check if this is a subgraph of prop->graph
  if (getRoot() != prop->getGraph()) {
    tlp::Graph *currentGraph = this;

    while(currentGraph->getSuperGraph() != currentGraph) {
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

  TLP_HASH_MAP<std::string, PropertyInterface *>::const_iterator it =
    circularCalls.find(algorithm);

  if (it != circularCalls.end() && (*it).second == prop) {
    errorMessage = std::string("Circular call of ") + __PRETTY_FUNCTION__;
#ifndef NDEBUG
    tlp::error() << errorMessage << std::endl;
#endif
    return false;
  }

  // nothing to do if the graph is empty
  if (numberOfNodes() == 0) {
    errorMessage= "The graph is empty";
    return false;
  }

  tlp::PluginProgress *tmpProgress;

  if (progress==NULL)
    tmpProgress=new tlp::SimplePluginProgress();
  else
    tmpProgress=progress;

  bool hasData = parameters != NULL;

  if (!hasData)
    parameters = new tlp::DataSet();

  // add prop as result in dataset
  parameters->set("result", prop);

  context.pluginProgress = tmpProgress;
  context.graph = this;
  context.dataSet = parameters;

  tlp::Observable::holdObservers();
  circularCalls[algorithm] = prop;
  Algorithm *tmpAlgo =
    tlp::PluginLister::instance()->getPluginObject<PropertyAlgorithm>(algorithm, &context);

  if (tmpAlgo != NULL) {
    result = tmpAlgo->check(errorMessage);

    if (result) {
      result = tmpAlgo->run();

      if (!result)
        errorMessage = tmpProgress->getError();
    }

    delete tmpAlgo;
  }
  else {
    errorMessage = algorithm + " - No algorithm available with this name";
#ifndef NDEBUG
    tlp::error() << __PRETTY_FUNCTION__ << ": " << errorMessage << std::endl;
#endif
    result=false;
  }

  circularCalls.erase(algorithm);
  tlp::Observable::unholdObservers();

  if (progress==NULL) delete tmpProgress;

  if (hasData)
    // remove result from dataset
    parameters->remove("result");
  else
    delete parameters;

  return result;
}

tlp::node Graph::getSource() const {
  node source;
  forEach(source, getNodes()) {
    if (indeg(source) == 0)
      return source;
  }
  return node();
}

DataType* Graph::getAttribute(const std::string& name) const {
  return getAttributes().getData(name);
}

void Graph::setAttribute(const std::string &name, const DataType* value) {
  notifyBeforeSetAttribute(name);
  getNonConstAttributes().setData(name, value);
  notifyAfterSetAttribute(name);
}

void Graph::notifyAddNode(const node n) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_NODE, n));
}

void Graph::notifyDelNode(const node n) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_DEL_NODE, n));
}

void Graph::notifyAddEdge(const edge e) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_EDGE, e));
}

void Graph::notifyDelEdge(const edge e) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_DEL_EDGE, e));
}

void Graph::notifyReverseEdge(const edge e) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_REVERSE_EDGE, e));
}

void Graph::notifyBeforeSetEnds(const edge e) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_SET_ENDS, e,
                         Event::TLP_INFORMATION));
}

void Graph::notifyAfterSetEnds(const edge e) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_SET_ENDS, e));
}

void Graph::notifyBeforeAddSubGraph(const Graph* graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_ADD_SUBGRAPH, graph));

  Graph *g = this;

  while (g != getRoot()) {
    g->notifyBeforeAddDescendantGraph(graph);
    g = g->getSuperGraph();
  }

  getRoot()->notifyBeforeAddDescendantGraph(graph);
}

void Graph::notifyAfterAddSubGraph(const Graph* graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_ADD_SUBGRAPH, graph));

  Graph *g = this;

  while (g != getRoot()) {
    g->notifyAfterAddDescendantGraph(graph);
    g = g->getSuperGraph();
  }

  getRoot()->notifyAfterAddDescendantGraph(graph);
}

void Graph::notifyBeforeDelSubGraph(const Graph* graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_DEL_SUBGRAPH, graph));

  Graph *g = this;

  while (g != getRoot()) {
    g->notifyBeforeDelDescendantGraph(graph);
    g = g->getSuperGraph();
  }

  getRoot()->notifyBeforeDelDescendantGraph(graph);
}
void Graph::notifyAfterDelSubGraph(const Graph* graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_DEL_SUBGRAPH, graph));

  Graph *g = this;

  while (g != getRoot()) {
    g->notifyAfterDelDescendantGraph(graph);
    g = g->getSuperGraph();
  }

  getRoot()->notifyAfterDelDescendantGraph(graph);
}

void Graph::notifyBeforeAddDescendantGraph(const Graph* graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_ADD_DESCENDANTGRAPH, graph));
}
void Graph::notifyAfterAddDescendantGraph(const Graph* graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_ADD_DESCENDANTGRAPH, graph));
}

void Graph::notifyBeforeDelDescendantGraph(const Graph* graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_DEL_DESCENDANTGRAPH, graph));
}
void Graph::notifyAfterDelDescendantGraph(const Graph* graph) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_DEL_DESCENDANTGRAPH, graph));
}

void Graph::notifyBeforeAddLocalProperty(const std::string& propName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_ADD_LOCAL_PROPERTY, propName));
}
void Graph::notifyAddLocalProperty(const std::string& propName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_LOCAL_PROPERTY, propName));
}

void Graph::notifyBeforeDelLocalProperty(const std::string& propName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY, propName,Event::TLP_INFORMATION));
}

void Graph::notifyAfterDelLocalProperty(const std::string& propName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this,GraphEvent::TLP_AFTER_DEL_LOCAL_PROPERTY,propName));
}

void Graph::notifyBeforeSetAttribute(const std::string& attName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_SET_ATTRIBUTE,
                         attName, Event::TLP_INFORMATION));
}

void Graph::notifyAfterSetAttribute(const std::string& attName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_SET_ATTRIBUTE, attName,
                         Event::TLP_INFORMATION));
}

void Graph::notifyRemoveAttribute(const std::string& attName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_REMOVE_ATTRIBUTE, attName,
                         Event::TLP_INFORMATION));
}

void Graph::notifyDestroy() {
  if (hasOnlookers()) {
    // the undo/redo mechanism has to simulate graph destruction
    Event evt(*this, Event::TLP_MODIFICATION);
    evt._type = Event::TLP_DELETE;
    sendEvent(evt);
  }
}
PropertyInterface *Graph::getLocalProperty(const std::string& propertyName, const std::string& propertyType) {
  if (propertyType.compare(DoubleProperty::propertyTypename) == 0) {
    return getLocalProperty<DoubleProperty> (propertyName);
  }
  else if (propertyType.compare(LayoutProperty::propertyTypename)==0) {
    return getLocalProperty<LayoutProperty> (propertyName);
  }
  else if (propertyType.compare(StringProperty::propertyTypename)==0) {
    return getLocalProperty<StringProperty> (propertyName);
  }
  else if (propertyType.compare(IntegerProperty::propertyTypename)==0) {
    return getLocalProperty<IntegerProperty> (propertyName);
  }
  else if (propertyType.compare(ColorProperty::propertyTypename)==0) {
    return getLocalProperty<ColorProperty> (propertyName);
  }
  else if (propertyType.compare(SizeProperty::propertyTypename)==0) {
    return getLocalProperty<SizeProperty> (propertyName);
  }
  else if (propertyType.compare(BooleanProperty::propertyTypename)==0) {
    return getLocalProperty<BooleanProperty> (propertyName);
  }
  else if (propertyType.compare(DoubleVectorProperty::propertyTypename)==0) {
    return getLocalProperty<DoubleVectorProperty> (propertyName);
  }
  else if (propertyType.compare(StringVectorProperty::propertyTypename)==0) {
    return getLocalProperty<StringVectorProperty> (propertyName);
  }
  else if (propertyType.compare(IntegerVectorProperty::propertyTypename)==0) {
    return getLocalProperty<IntegerVectorProperty> (propertyName);
  }
  else if (propertyType.compare(CoordVectorProperty::propertyTypename)==0) {
    return getLocalProperty<CoordVectorProperty> (propertyName);
  }
  else if (propertyType.compare(ColorVectorProperty::propertyTypename)==0) {
    return getLocalProperty<ColorVectorProperty> (propertyName);
  }
  else if (propertyType.compare(BooleanVectorProperty::propertyTypename)==0) {
    return getLocalProperty<BooleanVectorProperty> (propertyName);
  }
  else if (propertyType.compare(SizeVectorProperty::propertyTypename)==0) {
    return getLocalProperty<SizeVectorProperty> (propertyName);
  }
  else if (propertyType.compare(GraphProperty::propertyTypename)==0) {
    return getLocalProperty<GraphProperty> (propertyName);
  }
  else {
    return NULL;
  }
}

PropertyInterface *Graph::getProperty(const std::string& propertyName, const std::string& propertyType) {
  if (propertyType.compare(DoubleProperty::propertyTypename) == 0) {
    return getProperty<DoubleProperty> (propertyName);
  }
  else if (propertyType.compare(LayoutProperty::propertyTypename)==0) {
    return getProperty<LayoutProperty> (propertyName);
  }
  else if (propertyType.compare(StringProperty::propertyTypename)==0) {
    return getProperty<StringProperty> (propertyName);
  }
  else if (propertyType.compare(IntegerProperty::propertyTypename)==0) {
    return getProperty<IntegerProperty> (propertyName);
  }
  else if (propertyType.compare(ColorProperty::propertyTypename)==0) {
    return getProperty<ColorProperty> (propertyName);
  }
  else if (propertyType.compare(SizeProperty::propertyTypename)==0) {
    return getProperty<SizeProperty> (propertyName);
  }
  else if (propertyType.compare(BooleanProperty::propertyTypename)==0) {
    return getProperty<BooleanProperty> (propertyName);
  }
  else if (propertyType.compare(DoubleVectorProperty::propertyTypename)==0) {
    return getProperty<DoubleVectorProperty> (propertyName);
  }
  else if (propertyType.compare(StringVectorProperty::propertyTypename)==0) {
    return getProperty<StringVectorProperty> (propertyName);
  }
  else if (propertyType.compare(IntegerVectorProperty::propertyTypename)==0) {
    return getProperty<IntegerVectorProperty> (propertyName);
  }
  else if (propertyType.compare(CoordVectorProperty::propertyTypename)==0) {
    return getProperty<CoordVectorProperty> (propertyName);
  }
  else if (propertyType.compare(ColorVectorProperty::propertyTypename)==0) {
    return getProperty<ColorVectorProperty> (propertyName);
  }
  else if (propertyType.compare(BooleanVectorProperty::propertyTypename)==0) {
    return getProperty<BooleanVectorProperty> (propertyName);
  }
  else if (propertyType.compare(SizeVectorProperty::propertyTypename)==0) {
    return getProperty<SizeVectorProperty> (propertyName);
  }
  else if(propertyType.compare(GraphProperty::propertyTypename)==0) {
    return getProperty<GraphProperty> (propertyName);
  }
  else {
    return NULL;
  }
}

static const string layoutProperty = "viewLayout";
static const string sizeProperty = "viewSize";
static const string rotationProperty = "viewRotation";
static const string colorProperty = "viewColor";

static void buildMapping(Iterator<node> *it, MutableContainer<node> &mapping, GraphProperty * metaInfo, const node from = node()) {
  while(it->hasNext()) {
    node n = it->next();

    if (!from.isValid())
      mapping.set(n.id, n);
    else
      mapping.set(n.id, from);

    Graph *meta = metaInfo->getNodeValue(n);

    if ( meta != NULL)
      buildMapping(meta->getNodes(), mapping, metaInfo, mapping.get(n.id));
  }

  delete it;
}
//====================================================================================
void updatePropertiesUngroup(Graph *graph, node metanode,
                             GraphProperty *clusterInfo) {
  if (clusterInfo->getNodeValue(metanode)==NULL) return; //The metanode is not a metanode.

  LayoutProperty *graphLayout = graph->getProperty<LayoutProperty>(layoutProperty);
  SizeProperty *graphSize = graph->getProperty<SizeProperty>(sizeProperty);
  DoubleProperty *graphRot = graph->getProperty<DoubleProperty>(rotationProperty);
  const Size& size = graphSize->getNodeValue(metanode);
  const Coord& pos = graphLayout->getNodeValue(metanode);
  const double& rot = graphRot->getNodeValue(metanode);
  Graph  *cluster = clusterInfo->getNodeValue(metanode);
  LayoutProperty *clusterLayout = cluster->getProperty<LayoutProperty>(layoutProperty);
  SizeProperty  *clusterSize   = cluster->getProperty<SizeProperty>(sizeProperty);
  DoubleProperty *clusterRot = cluster->getProperty<DoubleProperty>(rotationProperty);
  BoundingBox box = tlp::computeBoundingBox(cluster, clusterLayout, clusterSize, clusterRot);
  Coord maxL(box[1]);
  Coord minL(box[0]);
  double width  = maxL[0] - minL[0];
  double height = maxL[1] - minL[1];
  double depth =  maxL[2] - minL[2];

  if (width<0.0001) width=1.0;

  if (height<0.0001) height=1.0;

  if (depth<0.0001) depth=1.0;

  Coord center = (maxL + minL) / -2.0f;
  clusterLayout->translate(center, cluster);
  clusterLayout->rotateZ(graphRot->getNodeValue(metanode), cluster);
  clusterLayout->scale(Coord(size[0]/width, size[1]/height, size[2]/depth), cluster);
  clusterLayout->translate(pos, cluster);
  clusterSize->scale(Size(size[0]/width, size[1]/height, size[2]/depth), cluster);

  Iterator<node> *itN = cluster->getNodes();

  while(itN->hasNext()) {
    node itn = itN->next();
    graphLayout->setNodeValue(itn, clusterLayout->getNodeValue(itn));
    graphSize->setNodeValue(itn, clusterSize->getNodeValue(itn));
    graphRot->setNodeValue(itn, clusterRot->getNodeValue(itn) + rot);
  }

  delete itN;
  Iterator<edge> *itE= cluster->getEdges();

  while (itE->hasNext()) {
    edge ite = itE->next();
    graphLayout->setEdgeValue(ite, clusterLayout->getEdgeValue(ite));
    graphSize->setEdgeValue(ite, clusterSize->getEdgeValue(ite));
  }

  delete itE;



  // propagate all cluster local properties
  PropertyInterface* property;
  forEach(property, cluster->getLocalObjectProperties()) {
    if (property == graphLayout ||
        property == graphSize ||
        property == graphRot)
      continue;

    PropertyInterface *graphProp = NULL;

    if (graph->existProperty(property->getName()) &&
        graph->getProperty(property->getName())->getTypename() == property->getTypename())
      graphProp = graph->getProperty(property->getName());
    else
      graphProp = property->clonePrototype(graph, property->getName());

    itN = cluster->getNodes();

    while(itN->hasNext()) {
      node itn = itN->next();
      graphProp->setNodeStringValue(itn, property->getNodeStringValue(itn));
    }

    delete itN;
    itE= cluster->getEdges();

    while (itE->hasNext()) {
      edge ite = itE->next();
      graphProp->setEdgeStringValue(ite, property->getEdgeStringValue(ite));
    }

    delete itE;
  }
}
//=========================================================
Graph* Graph::addSubGraph(const std::string& name) {
  Graph* g = addSubGraph(NULL, name);
  return g;
}
//=========================================================
Graph* Graph::addCloneSubGraph(const std::string& name, bool addSibling) {
  BooleanProperty selection(this);
  selection.setAllNodeValue(true);
  selection.setAllEdgeValue(true);
  Graph* parentSubGraph = this;

  if (addSibling) {
    parentSubGraph = getSuperGraph();

    if (this == parentSubGraph)
      // cannot add sibling of root graph
      return NULL;
  }

  return parentSubGraph->addSubGraph(&selection, name);
}
//=========================================================
Graph * Graph::inducedSubGraph(const std::set<node> &nodes,
                               Graph* parentSubGraph) {
  if (parentSubGraph == NULL)
    parentSubGraph = this;

  // create subgraph and add nodes
  Graph *result = parentSubGraph->addSubGraph();
  StlIterator<node, std::set<node>::const_iterator> it(nodes.begin(), nodes.end());
  result->addNodes(&it);

  Iterator<node> *itN=result->getNodes();

  while (itN->hasNext()) {
    node itn=itN->next();
    Iterator<edge> *itE=getOutEdges(itn);

    while (itE->hasNext()) {
      edge ite = itE->next();

      if (result->isElement(target(ite)))
        result->addEdge(ite);
    }

    delete itE;
  }

  delete itN;
  return result;
}
//====================================================================================
node Graph::createMetaNode (const std::set<node> &nodeSet, bool multiEdges, bool delAllEdge) {
  if (getRoot() == this) {
    tlp::warning() << __PRETTY_FUNCTION__ << std::endl;
    tlp::warning() << "\t Error: Could not group a set of nodes in the root graph" << std::endl;
    return node();
  }

  if (nodeSet.empty()) {
    tlp::warning() << __PRETTY_FUNCTION__ << std::endl;
    tlp::warning() << '\t' << "Warning: Creation of an empty metagraph" << std::endl;
  }

  // create an induced brother sub graph
  Graph *subGraph = inducedSubGraph(nodeSet, getSuperGraph());
  // all local properties
  // must be cloned in subgraph
  PropertyInterface *prop;
  forEach(prop, getLocalObjectProperties()) {
    PropertyInterface* sgProp =
      prop->clonePrototype(subGraph, prop->getName());
    set<node>::const_iterator itNodeSet = nodeSet.begin();

    for(; itNodeSet!=nodeSet.end(); ++itNodeSet) {
      node n = *itNodeSet;
      DataMem* val =  prop->getNodeDataMemValue(n);
      sgProp->setNodeDataMemValue(n, val);
      delete val;
    }
  }

  stringstream st;
  st << "grp_" << setfill('0') << setw(5) << subGraph->getId();
  subGraph->setAttribute("name", st.str());
  return createMetaNode(subGraph, multiEdges, delAllEdge);
}

//====================================================================================
node Graph::createMetaNode(Graph *subGraph, bool multiEdges, bool edgeDelAll) {
  if (getRoot() == this) {
    tlp::warning() << __PRETTY_FUNCTION__ << std::endl;
    tlp::warning() << "\t Error: Could not create a meta node in the root graph" << std::endl;
    return node();
  }

  GraphProperty* metaInfo =
    ((GraphAbstract *)getRoot())->getMetaGraphProperty();
  node metaNode = addNode();
  metaInfo->setNodeValue(metaNode, subGraph);
  Observable::holdObservers();
  //updateGroupLayout(this, subGraph, metaNode);
  // compute meta node values
  PropertyInterface *property;
  forEach(property, getObjectProperties()) {
    property->computeMetaValue(metaNode, subGraph, this);
  }

  // keep track of graph existing edges
  MutableContainer<bool> graphEdges;
  graphEdges.setAll(false);
  Iterator<edge>* itE = getEdges();

  while(itE->hasNext())
    graphEdges.set(itE->next().id, true);

  delete itE;

  //we can now Remove nodes from graph
  StableIterator<node> itN(subGraph->getNodes());
  delNodes(&itN);

  //create new meta edges from nodes to metanode
  Graph* super = getSuperGraph();
  //colors = super->getProperty<ColorProperty> (colorProperty);
  TLP_HASH_MAP<node, TLP_HASH_SET<node> > edges;
  TLP_HASH_MAP<node, edge> metaEdges;
  TLP_HASH_MAP<edge, set<edge> > subEdges;
  Iterator<node> *subGraphNodes = subGraph->getNodes();

  while (subGraphNodes->hasNext()) {
    node n = subGraphNodes->next();
    StableIterator<edge> it(getSuperGraph()->getInOutEdges(n));

    while (it.hasNext()) {
      edge e = it.next();
      pair<node, node> eEnds  = ends(e);
      node src = eEnds.first;
      node tgt = eEnds.second;
      bool toDelete =
        ((metaInfo->getNodeValue(src)!=NULL) ||
         (metaInfo->getNodeValue(tgt)!=NULL)) &&
        isElement (src) && isElement (tgt) &&
        existEdge (src, tgt).isValid();

      if (isElement(src) && subGraph->isElement(tgt)) {
        if (multiEdges || edges[src].empty()) {
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
        }
        else if (!multiEdges)
          // e is a sub-edge of an already created meta edge
          subEdges[metaEdges[src]].insert(e);

        edges[src].insert(tgt);

        if (toDelete) {
          delEdge(e, edgeDelAll);
        }
      }

      if (isElement(tgt) && subGraph->isElement(src)) {
        if (multiEdges || edges[tgt].empty()) {
          // add new meta edge
          edge metaEdge = addEdge(metaNode, tgt);

          if (!graphEdges.get(e.id))
            delEdge(metaEdge);

          // e is a sub-edge of metaEdge
          subEdges[metaEdge].insert(e);

          if (!multiEdges)
            // record metaEdge
            metaEdges[tgt] = metaEdge;

          if (!super->isElement(metaEdge))
            super->addEdge(metaEdge);
        }
        else if (!multiEdges)
          // e is a sub-edge of an already created meta edge
          subEdges[metaEdges[tgt]].insert(e);

        edges[tgt].insert(src);

        if (toDelete) {
          delEdge(e, edgeDelAll);
        }
      }
    }
  }

  delete subGraphNodes;
  // update metaInfo of new meta edges
  TLP_HASH_MAP<edge, set<edge> >::const_iterator it;

  for(it = subEdges.begin(); it != subEdges.end(); ++it) {
    edge mE = (*it).first;
    metaInfo->setEdgeValue(mE, (*it).second);
    // compute meta edge values
    forEach(property, getObjectProperties()) {
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
static void mapSubGraphNodes(Graph* sg, node metaNode,
                             MutableContainer<node>& mappingM,
                             GraphProperty* metaInfo) {
  node n;
  forEach(n, sg->getNodes()) {
    mappingM.set(n.id, metaNode);
    Graph* ssg = metaInfo->getNodeValue(n);

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

  GraphProperty* metaInfo =
    ((GraphAbstract *) getRoot())->getMetaGraphProperty();
  Graph *metaGraph = metaInfo->getNodeValue(metaNode);

  if (metaGraph == NULL) return;

  Observable::holdObservers();
  MutableContainer<node> mappingM;
  mappingM.setAll(node());
  //add node from meta to graph
  {
    node n;
    //stable in case of fractal graph
    stableForEach(n, metaGraph->getNodes()) {
      addNode(n);
      mappingM.set(n.id, n);
      Graph *sg = metaInfo->getNodeValue(n);

      if (sg)
        // map all nodes or embedded nodes
        // of this subgraph to n
        mapSubGraphNodes(sg, n, mappingM, metaInfo);
    }
    StableIterator<edge> ite(metaGraph->getEdges());
    addEdges(&ite);
  }

  if (updateProperties)
    updatePropertiesUngroup(this, metaNode, metaInfo);

  // check for edges from or to the meta node
  Graph* super = getSuperGraph();
  Iterator<edge>* metaEdges = super->getInOutEdges(metaNode);

  if (!metaEdges->hasNext()) {
    delete metaEdges;
    // no edge so just remove the meta node
    getRoot()->delNode(metaNode, true);
    Observable::unholdObservers();
    return;
  }

  bool hasSubEdges = super->isMetaEdge(metaEdges->next());
  delete metaEdges;
  metaEdges = new StableIterator<edge>(super->getInOutEdges(metaNode));
  ColorProperty *graphColors =
    getProperty<ColorProperty>(colorProperty);

  if (hasSubEdges) {
    node mn;
    // compute mapping for neighbour nodes
    // and their sub nodes
    forEach(mn, super->getInOutNodes(metaNode)) {
      mappingM.set(mn.id, mn);
      Graph *mnGraph = metaInfo->getNodeValue(mn);

      if (mnGraph != NULL) {
        Iterator<node> *it = mnGraph->getNodes();

        while(it->hasNext()) {
          mappingM.set(it->next().id, mn);
        }

        delete it;
      }
    }

    while (metaEdges->hasNext()) {
      edge metaEdge = metaEdges->next();
      Color metaColor = graphColors->getEdgeValue(metaEdge);
      Iterator<edge>* subEdges = getEdgeMetaInfo(metaEdge);
      TLP_HASH_MAP<node, TLP_HASH_MAP<node, set<edge> > > newMetaEdges;

      while(subEdges->hasNext()) {
        edge e = subEdges->next();
        const std::pair<node, node>& eEnds = super->ends(e);

        if (isElement(eEnds.first)) {
          if (isElement(eEnds.second)) {
            addEdge(e);

            if (!isElement(metaEdge))
              delEdge(e);

            graphColors->setEdgeValue(e, metaColor);
          }
          else {
            node tgt = mappingM.get(eEnds.second.id);

            // tgt may not be valid because at this time
            // when deleting a node from a subgraph pointed
            // by a metanode we are not able to correctly
            // update the meta edges embedding the inout edges
            // of this node
            if (tgt.isValid())
              newMetaEdges[eEnds.first][tgt].insert(e);
          }
        }
        else {
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

      delete subEdges;
      // iterate on newMetaEdges
      TLP_HASH_MAP<node, TLP_HASH_MAP<node, set<edge> > >::iterator itme =
        newMetaEdges.begin();

      while(itme != newMetaEdges.end()) {
        node src = (*itme).first;
        TLP_HASH_MAP<node, set<edge> >::iterator itnme = (*itme).second.begin();
        TLP_HASH_MAP<node, set<edge> >::iterator itnmeEnd = (*itme).second.end();

        while(itnme != itnmeEnd) {
          Graph* graph = this;
          node tgt(itnme->first);

          // add edge in the right graph
          if (!isElement(src) || !isElement(tgt))
            graph = super;

          edge mE = graph->addEdge(src, tgt);
          metaInfo->setEdgeValue(mE, itnme->second);
          // compute meta edge values
          PropertyInterface *property;
          forEach(property, graph->getObjectProperties()) {
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
  }
  else {
    MutableContainer<node> mappingC;
    MutableContainer<node> mappingN;
    mappingC.setAll(node());
    mappingN.setAll(node());
    Graph *root = getRoot();
    buildMapping(root->getInOutNodes(metaNode), mappingC, metaInfo, node() );
    buildMapping(metaGraph->getNodes() , mappingN, metaInfo, node() );

    TLP_HASH_MAP<node, Color> metaEdgeToColor;

    while (metaEdges->hasNext()) {
      edge metaEdge = metaEdges->next();
      metaEdgeToColor[opposite (metaEdge, metaNode)] =
        graphColors->getEdgeValue(metaEdge);
    }

    //Remove the metagraph from the hierarchy and remove the metanode
    root->delNode(metaNode, true);
    TLP_HASH_MAP<node, TLP_HASH_SET<node> > edges;
    //=================================
    StableIterator<edge> it(root->getEdges());

    while(it.hasNext()) {
      edge e = it.next();

      if (isElement(e)) continue;

      pair<node, node> eEnds = root->ends(e);
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
      }
      else {
        if (sourceN.isValid() && targetC.isValid()) {
          src = sourceN;
          tgt = targetC;
          edgeColor = metaEdgeToColor[tgt];
        }
        else continue;
      }

      if (metaInfo->getNodeValue(src) == NULL &&
          metaInfo->getNodeValue(tgt) == NULL) {
        addEdge(e);
        continue;
      }

      if ( (edges.find(src) == edges.end()) ||
           (edges[src].find(tgt) == edges[src].end()) ) {
        edges[src].insert(tgt);

        if (!existEdge(src,tgt).isValid()) {
          edge addedEdge = addEdge(src,tgt);
          graphColors->setEdgeValue (addedEdge, edgeColor);
        }
        else
          tlp::error() << __PRETTY_FUNCTION__<< ": bug exist edge 1" << std::endl;
      }

      // }
    }
  }

  delete metaEdges;
  Observable::unholdObservers();
}
//====================================================================================
struct MetaEdge {
  unsigned int source,target;
  edge mE;
};

namespace std {
template<>
struct less<MetaEdge> {
  bool operator()(const MetaEdge &c,const MetaEdge &d) const {
    /*if (c.source<d.source) return true;
    if (c.source>d.source) return false;
    if (c.target<d.target) return true;
    if (c.target>d.target) return false;
    return false;*/
    return (c.source > d.source) ||
           ((c.source == d.source) && (c.target < d.target));
  }
};
}

void Graph::createMetaNodes(Iterator<Graph *> *itS, Graph *quotientGraph,
                            vector<node>& metaNodes) {
  GraphProperty *metaInfo =
    ((GraphAbstract*)getRoot())->getMetaGraphProperty();
  map <edge, set<edge> > eMapping;
  Observable::holdObservers();
  {
    map<node, set<node> > nMapping;

    while (itS->hasNext()) {
      Graph *its=itS->next();

      if (its!=quotientGraph) {
        // Create one metanode for each subgraph(cluster)
        node metaN = quotientGraph->addNode();
        metaNodes.push_back(metaN);
        metaInfo->setNodeValue(metaN, its);
        // compute meta node values
        PropertyInterface *property;
        forEach(property, quotientGraph->getObjectProperties()) {
          property->computeMetaValue(metaN, its, quotientGraph);
        }
        node n;
        forEach(n, its->getNodes()) {
          // map each subgraph's node to a set of meta nodes
          // in order to deal consistently with overlapping clusters
          if (nMapping.find(n) == nMapping.end())
            nMapping[n] = set<node>();

          nMapping[n].insert(metaN);
        }
      }
    }

    {
      set<MetaEdge> myQuotientGraph;
      edge e;
      // for each existing edge in the current graph
      // add a meta edge for the corresponding couple
      // (meta source, meta target) if it does not already exists
      // and register the edge as associated to this meta edge
      stableForEach(e, getEdges()) {
        pair<node, node> eEnds = ends(e);
        set<node>& metaSources = nMapping[eEnds.first];
        set<node>& metaTargets = nMapping[eEnds.second];

        for(set<node>::const_iterator itms = metaSources.begin();
            itms != metaSources.end(); ++itms) {
          node mSource = *itms;

          for(set<node>::const_iterator itmt = metaTargets.begin();
              itmt != metaTargets.end(); ++itmt) {
            node mTarget = *itmt;

            if (mSource != mTarget) {
              MetaEdge tmp;
              tmp.source = mSource.id, tmp.target = mTarget.id;
              set<MetaEdge>::const_iterator itm = myQuotientGraph.find(tmp);

              if (itm == myQuotientGraph.end()) {
                edge mE = quotientGraph->addEdge(mSource, mTarget);
                tmp.mE = mE;
                myQuotientGraph.insert(tmp);
                eMapping[mE].insert(e);
              }
              else {
                // add edge
                eMapping[(*itm).mE].insert(e);
              }
            }
          }
        }
      }
    }
  }
  // set viewMetaGraph for added meta edges
  map<edge, set<edge> >::const_iterator itm = eMapping.begin();

  while(itm != eMapping.end()) {
    edge mE = (*itm).first;
    metaInfo->setEdgeValue(mE, (*itm).second);
    // compute meta edge values
    string pName;
    forEach(pName, quotientGraph->getProperties()) {
      Iterator<edge> *itE = getRoot()->getEdgeMetaInfo(mE);
      PropertyInterface *property = quotientGraph->getProperty(pName);
      property->computeMetaValue(mE, itE, quotientGraph);
      delete itE;
    }
    ++itm;
  }

  Observable::unholdObservers();
}

Graph *Graph::getNthSubGraph(unsigned int n) const {
  unsigned int i=0;
  Iterator<Graph *> *it = getSubGraphs();

  while (it->hasNext()) {
    Graph *result = it->next();

    if (i++ == n) {
      delete it;
      return result;
    }
  }

  delete it;
  return NULL;
}

const std::string& GraphEvent::getPropertyName() const {
  assert(evtType > TLP_AFTER_DEL_SUBGRAPH && evtType < TLP_BEFORE_SET_ATTRIBUTE);

  if (evtType == TLP_BEFORE_RENAME_LOCAL_PROPERTY ||
      evtType == TLP_AFTER_RENAME_LOCAL_PROPERTY)
    return info.renamedProp->first->getName();

  return *(info.name);
}

void Graph::addNodes(const std::vector<node>& nodes) {
  StlIterator<node, vector<node>::const_iterator> vIterator(nodes.begin(), nodes.end());
  addNodes(&vIterator);
}

void Graph::delNodes(const std::vector<node>& nodes, bool deleteInAllGraphs) {
  StlIterator<node, vector<node>::const_iterator> vIterator(nodes.begin(), nodes.end());
  delNodes(&vIterator, deleteInAllGraphs);
}

void Graph::addEdges(const std::vector<edge>& edges) {
  StlIterator<edge, vector<edge>::const_iterator> vIterator(edges.begin(), edges.end());
  addEdges(&vIterator);
}

void Graph::delEdges(const std::vector<edge>& edges, bool deleteInAllGraphs) {
  StlIterator<edge, vector<edge>::const_iterator> vIterator(edges.begin(), edges.end());
  delEdges(&vIterator, deleteInAllGraphs);
}

struct DescendantGraphsIterator :public Iterator<Graph*> {
  // use a stack to store non empty iterators
  stack<Iterator<Graph *>*> iterators;
  // the current one
  Iterator<Graph *>* current;

  DescendantGraphsIterator(const Graph *g) {
    current = g->getSubGraphs();

    if (!current->hasNext()) {
      delete current;
      current = NULL;
    }
  }

  ~DescendantGraphsIterator() {
    if (current)
      delete current;

    while(!iterators.empty()) {
      delete iterators.top();
      iterators.pop();
    }
  }

  bool hasNext() {
    return current != NULL;
  }

  Graph* next() {
    if (current) {
      Graph* g = current->next();
      Iterator<Graph *>* itg = g->getSubGraphs();

      if (itg->hasNext()) {
        if (current->hasNext())
          // pushed iterators are always non empty
          iterators.push(current);
        else
          delete current;

        current = itg;
      }
      else {
        delete itg;

        if (!current->hasNext()) {
          delete current;

          if (!iterators.empty()) {
            current = iterators.top();
            iterators.pop();
          }
          else
            current = NULL;
        }
      }

      return g;
    }

    return NULL;
  }
};

Iterator<Graph*>* Graph::getDescendantGraphs() const {
  return new DescendantGraphsIterator(this);
}
