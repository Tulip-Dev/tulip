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

#include <fstream>

#include <tulip/Graph.h>
#include <tulip/TlpTools.h>
#include <tulip/GraphImpl.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/GraphProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/Reflect.h>
#include <tulip/ExportModule.h>
#include <tulip/Algorithm.h>
#include <tulip/ImportModule.h>
#include <tulip/SimplePluginProgress.h>

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

//=========================================================
Graph * tlp::newGraph() {
  return new GraphImpl();
}
//=========================================================
Graph * tlp::newSubGraph(Graph *graph, std::string name) {
  return graph->addSubGraph(NULL, 0, name);
}
//=========================================================
Graph * tlp::newCloneSubGraph(Graph *graph, std::string name) {
  return graph->addCloneSubGraph(name);
}
//=========================================================
Graph * tlp::loadGraph(const std::string &filename) {
  DataSet dataSet;
  dataSet.set("file::filename", filename);
  Graph *sg = tlp::importGraph("tlp", dataSet);
  return sg;
}
//=========================================================
bool tlp::saveGraph(Graph *sg, const std::string &filename) {
  ostream *os;

  if (filename.rfind(".gz") == (filename.length() - 3))
    os = tlp::getOgzstream(filename.c_str());
  else
    os = new ofstream(filename.c_str());

  bool result;
  DataSet data;
  data.set("file", filename);
  result=tlp::exportGraph(sg, *os, "tlp", data, 0);
  delete os;
  return result;
}
//=========================================================
// this method is temporary added here
// while the deprecated method exists.
// Will be declared as pure virtual after.
bool ImportModule::importGraph() {
  // by default calls the deprecated method
  std::string str;
  return import(str);
}

Graph * tlp::importGraph(const std::string &alg, DataSet &dataSet, PluginProgress *plugProgress, Graph *newGraph) {

  if (!ImportModuleFactory::factory->pluginExists(alg)) {
    cerr << "libtulip: " << __FUNCTION__ << ": import plugin \"" << alg
         << "\" doesn't exists (or is not loaded)" << endl;
    return NULL;
  }

  bool newGraphP=false;

  if (newGraph==NULL) {
    newGraph=new GraphImpl();
    newGraphP=true;
  }

  AlgorithmContext tmp;
  tmp.graph=newGraph;
  tmp.dataSet = &dataSet;
  PluginProgress *tmpProgress;
  bool deletePluginProgress=false;

  if (plugProgress==NULL) {
    tmpProgress=new SimplePluginProgress();
    deletePluginProgress=true;
  }
  else tmpProgress=plugProgress;

  tmp.pluginProgress=tmpProgress;
  ImportModule *newImportModule=ImportModuleFactory::factory->getPluginObject(alg, tmp);
  assert(newImportModule!=NULL);
  bool result;

  if (!(result=newImportModule->importGraph())) {
    if (newGraphP) delete newGraph;
  }
  else {
    std::string filename;

    if (dataSet.get("file::filename", filename)) {
      newGraph->setAttribute("file", filename);
    }
  }

  if (deletePluginProgress) delete tmpProgress;

  delete newImportModule;
  dataSet = *tmp.dataSet;

  if (!result)
    return 0;
  else
    return newGraph;
}
//=========================================================
// this method is temporary added here
// while the deprecated method exists.
// Will be declared as pure virtual after.
bool ExportModule::exportGraph(std::ostream &os) {
  // by default calls the deprecated method
  return exportGraph(os, graph);
}

bool tlp::exportGraph(Graph *sg,ostream &os, const std::string &alg,
                      DataSet &dataSet, PluginProgress *plugProgress) {
  if (!ExportModuleFactory::factory->pluginExists(alg)) {
    cerr << "libtulip: " << __FUNCTION__ << ": export plugin \"" << alg
         << "\" does not exist (or is not loaded)" << endl;
    return false;
  }

  bool result;
  bool deletePluginProgress=false;
  AlgorithmContext tmp;
  tmp.graph=sg;
  tmp.dataSet=&dataSet;
  PluginProgress *tmpProgress=NULL;

  if (plugProgress==NULL) {
    tmpProgress=new SimplePluginProgress();
    deletePluginProgress=true;
  }
  else tmpProgress=plugProgress;

  tmp.pluginProgress=tmpProgress;
  ExportModule *newExportModule=ExportModuleFactory::factory->getPluginObject(alg, tmp);
  assert(newExportModule!=NULL);
  std::string filename;

  if (dataSet.get("file", filename)) {
    sg->setAttribute("file", filename);
  }

  result=newExportModule->exportGraph(os);

  if (deletePluginProgress) delete tmpProgress;

  delete newExportModule;
  return result;
}

//=========================================================
bool tlp::applyAlgorithm(Graph *graph, std::string &errorMsg,DataSet *dataSet,
                         const std::string &alg, PluginProgress *progress) {
  return graph->applyAlgorithm(alg, errorMsg, dataSet, progress);
}
//=========================================================
bool Graph::applyAlgorithm(const std::string &alg, std::string &errorMsg,
                           DataSet *dataSet, PluginProgress *progress) {
  if (!AlgorithmPlugin::factory->pluginExists(alg)) {
    cerr << "libtulip: " << __FUNCTION__ << ": algorithm plugin \"" << alg
         << "\" does not exist (or is not loaded)" << endl;
    return false;
  }

  bool result;
  bool deletePluginProgress=false;
  AlgorithmContext tmp;
  tmp.graph=this;
  tmp.dataSet=dataSet;
  PluginProgress *tmpProgress;

  if (progress==NULL) {
    tmpProgress=new SimplePluginProgress();
    deletePluginProgress=true;
  }
  else tmpProgress=progress;

  tmp.pluginProgress=tmpProgress;
  Algorithm *newAlgo=AlgorithmPlugin::factory->getPluginObject(alg, tmp);

  if ((result=newAlgo->check(errorMsg)))
    result = newAlgo->run();

  delete newAlgo;

  if (deletePluginProgress) delete tmpProgress;

  return result;
}
//=========================================================
bool tlp::getSource(const Graph *graph, node &n) {
  return (n = graph->getSource()).isValid();
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

tlp::node Graph::getSource() const {
  node source(UINT_MAX);

  Iterator<node> *it = getNodes();

  while (it->hasNext()) {
    source=it->next();

    if (indeg(source) == 0) {
      break;
    }
  }

  delete it;

  return source;
}

DataType* Graph::getAttribute(const std::string& name) const {
  return getAttributes().getData(name);
}

void Graph::setAttribute(const std::string &name, const DataType* value) {
  notifyBeforeSetAttribute(name);
  getNonConstAttributes().setData(name, value);
  notifyAfterSetAttribute(name);
}

void Graph::addGraphObserver(GraphObserver *gObs) const {
  addListener(&gObs->hiddenObserver);
}

void Graph::removeGraphObserver(GraphObserver *gObs) const {
  removeListener(&gObs->hiddenObserver);
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

void Graph::notifyAddSubGraph(const Graph* sg) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_SUBGRAPH, sg));
}

void Graph::notifyDelSubGraph(const Graph* sg) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_DEL_SUBGRAPH, sg));
}

void Graph::notifyAddLocalProperty(const std::string& propName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_ADD_LOCAL_PROPERTY, propName));
}

void Graph::notifyBeforeDelLocalProperty(const std::string& propName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY,
                         propName, Event::TLP_INFORMATION));
}

void Graph::notifyAfterDelLocalProperty(const std::string& propName) {
  if (hasOnlookers())
    sendEvent(GraphEvent(*this, GraphEvent::TLP_AFTER_DEL_LOCAL_PROPERTY,
                         propName));
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
  else {
    return NULL;
  }
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

//====================================================================================
bool tlp::Graph::applyPropertyAlgorithm(const std::string &algorithm,
                                        PropertyInterface* prop,
                                        std::string &msg,
                                        tlp::PluginProgress *progress,
                                        tlp::DataSet *data) {
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
      msg = "The passed property does not belong to the graph";
      return false;
    }
  }

#ifndef NDEBUG
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
#endif

  TLP_HASH_MAP<std::string, PropertyInterface *>::const_iterator it =
    circularCalls.find(algorithm);

  if (it != circularCalls.end() && (*it).second == prop) {
#ifndef NDEBUG
    std::cerr << "Circular call of " << __PRETTY_FUNCTION__ << " " << msg << std::endl;
#endif
    return false;
  }

  // nothing to do if the graph is empty
  if (numberOfNodes() == 0) {
    msg= "The graph is empty";
    return false;
  }

  tlp::PluginProgress *tmpProgress;

  if (progress==NULL)
    tmpProgress=new tlp::SimplePluginProgress();
  else
    tmpProgress=progress;

  bool hasData = data != NULL;

  if (!hasData)
    data = new tlp::DataSet();

  // add prop as result in dataset
  data->set<PropertyInterface *>("result", prop);

  context.pluginProgress = tmpProgress;
  context.graph = this;
  context.dataSet = data;

  tlp::Observable::holdObservers();
  circularCalls[algorithm] = prop;
  tlp::AlgorithmContext tmpContext(context);
  Algorithm *tmpAlgo =
    AlgorithmPlugin::factory->getPluginObject(algorithm, tmpContext);

  if (tmpAlgo != NULL) {
    result = tmpAlgo->check(msg);

    if (result) {
      tmpAlgo->run();
    }

    delete tmpAlgo;
  }
  else {
    msg = "No algorithm available with this name";
    result=false;
  }

  circularCalls.erase(algorithm);
  tlp::Observable::unholdObservers();

  if (progress==NULL) delete tmpProgress;

  if (hasData)
    // remove result from dataset
    data->remove("result");
  else
    delete data;

  return result;
}
//=========================================================
Graph* Graph::addSubGraph(std::string name) {
  return addSubGraph(NULL, 0, name);
}
//=========================================================
Graph* Graph::addCloneSubGraph(std::string name) {
  BooleanProperty selection(this);
  selection.setAllNodeValue(true);
  selection.setAllEdgeValue(true);
  return addSubGraph(&selection, 0, name);
}


