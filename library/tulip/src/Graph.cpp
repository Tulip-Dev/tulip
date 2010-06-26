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

#include "thirdparty/gzstream/gzstream.h"
#include "tulip/Graph.h"
#include "tulip/IdManager.h"
#include "tulip/TlpTools.h"
#include "tulip/GraphImpl.h"
#include "tulip/BooleanProperty.h"
#include "tulip/GraphProperty.h"
#include "tulip/Reflect.h"
#include "tulip/ExportModule.h"
#include "tulip/Algorithm.h"
#include "tulip/ImportModule.h"

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
    } else {
      if (current.id == previousNode.id + 1) {
	previousNode = current;
	if (!itn->hasNext())
	  os << ".." << current.id;
      } else {
	if (previousNode != beginNode) {
	  os << ".." << previousNode.id;
	}
	os  << " " << current.id;
	beginNode = previousNode = current;
      }
    }
  } delete itn;
  os << ")" << endl;
  os << ";(edge <edge_id> <source_id> <target_id>)" << endl;
  Iterator<edge> *ite=sp->getEdges();
  for (;ite->hasNext();) {
    edge e=ite->next();
    os << "(edge " << e.id << " " << sp->source(e).id << " " << sp->target(e).id << ")";
    if (ite->hasNext()) os << endl;
  } delete ite;
  os << endl;
  return os;
}

//=========================================================
Graph * tlp::newGraph(){
  return new GraphImpl();
}
//=========================================================
Graph * tlp::newSubGraph(Graph *sg,string name) {
  Graph *newGraph = sg->addSubGraph();
  newGraph->setAttribute("name", name);
  return newGraph;
}
//=========================================================
Graph * tlp::newCloneSubGraph(Graph *sg, string name) {
  BooleanProperty sel1(sg);
  sel1.setAllNodeValue(true);
  sel1.setAllEdgeValue(true);
  Graph *newGraph = sg->addSubGraph(&sel1);
  newGraph->setAttribute("name", name);
  return newGraph;
}
//=========================================================
Graph * tlp::loadGraph(const string &filename) {
  DataSet dataSet;
  dataSet.set("file::filename", filename);
  Graph *sg = tlp::importGraph("tlp", dataSet);
  return sg;
}
//=========================================================
bool tlp::saveGraph(Graph *sg, const string &filename) {
  ostream *os;
  if (filename.rfind(".gz") == (filename.length() - 3))
    os = tlp::getOgzstream(filename.c_str());
  else
    os = new ofstream(filename.c_str());
  bool result;
  DataSet data;
  result=tlp::exportGraph(sg, *os, "tlp", data, 0);
  delete os;
  return result;
}
//=========================================================
Graph * tlp::importGraph(const string &alg, DataSet &dataSet, PluginProgress *plugProgress, Graph *newGraph){

  if (!ImportModuleFactory::factory->pluginExists(alg)) {
    cerr << "libtulip: " << __FUNCTION__ << ": import plugin \"" << alg
         << "\" doesn't exists (or is not loaded)" << endl;
    return NULL;
  }
  bool newGraphP=false;
  if (newGraph==0) {
    newGraph=new GraphImpl();
    newGraphP=true;
  }

  AlgorithmContext tmp;
  tmp.graph=newGraph;
  tmp.dataSet = &dataSet;
  PluginProgress *tmpProgress;
  bool deletePluginProgress=false;
  if (plugProgress==0) {
    tmpProgress=new PluginProgress();
    deletePluginProgress=true;
  }
  else tmpProgress=plugProgress;
  tmp.pluginProgress=tmpProgress;
  ImportModule *newImportModule=ImportModuleFactory::factory->getPluginObject(alg, tmp);
  assert(newImportModule!=0);
  bool result;
  if (!(result=newImportModule->import(""))) {
    if (newGraphP) delete newGraph;
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
bool tlp::exportGraph(Graph *sg,ostream &os, const string &alg,
		      DataSet &dataSet, PluginProgress *plugProgress) {
  if (!ExportModuleFactory::factory->pluginExists(alg)) {
    cerr << "libtulip: " << __FUNCTION__ << ": export plugin \"" << alg
         << "\" doesn't exists (or is not loaded)" << endl;
    return false;
  }

  bool result;
  bool deletePluginProgress=false;
  AlgorithmContext tmp;
  tmp.graph=sg;
  tmp.dataSet=&dataSet;
  PluginProgress *tmpProgress=NULL;
  if (plugProgress==0) {
    tmpProgress=new PluginProgress();
    deletePluginProgress=true;
  }
  else tmpProgress=plugProgress;
  tmp.pluginProgress=tmpProgress;
  ExportModule *newExportModule=ExportModuleFactory::factory->getPluginObject(alg, tmp);
  assert(newExportModule!=0);
  result=newExportModule->exportGraph(os,sg);

  if (deletePluginProgress) delete tmpProgress;
  delete newExportModule;
  return result;
}
//=========================================================
bool tlp::applyAlgorithm(Graph *sg,string &errorMsg,DataSet *dataSet,
			  const string &alg, PluginProgress *plugProgress) {
  if (!AlgorithmFactory::factory->pluginExists(alg)) {
    cerr << "libtulip: " << __FUNCTION__ << ": algorithm plugin \"" << alg
         << "\" doesn't exists (or is not loaded)" << endl;
    return false;
  }

  bool result;
  bool deletePluginProgress=false;
  AlgorithmContext tmp;
  tmp.graph=sg;
  tmp.dataSet=dataSet;
  PluginProgress *tmpProgress;
  if (plugProgress==0) {
    tmpProgress=new PluginProgress();
    deletePluginProgress=true;
  }
  else tmpProgress=plugProgress;
  tmp.pluginProgress=tmpProgress; 
  Algorithm *newAlgo=AlgorithmFactory::factory->getPluginObject(alg, tmp);
  if ((result=newAlgo->check(errorMsg)))
    newAlgo->run();
  delete newAlgo;
  if (deletePluginProgress) delete tmpProgress;
  return result;
}
//=========================================================
bool tlp::getSource(Graph *graph, node &n) {
  Iterator<node> *it=graph->getNodes();
  while (it->hasNext()) {
    n=it->next();
    if (graph->indeg(n)==0){
      delete it;
      return true;
    }
  }delete it;
  return false;
}
//=========================================================
void tlp::removeFromGraph(Graph *ioG, BooleanProperty *inSel) {
  if( !ioG )
    return;

  vector<node>	nodeA;
  vector<edge>	edgeA;

  // Get edges
  Iterator<edge> * edgeIt = ioG->getEdges();
  while( edgeIt->hasNext() ) {
    edge e = edgeIt->next();
    if( !inSel || inSel->getEdgeValue(e) ) {
      // selected edge -> remove it !
      edgeA.push_back( e );
    } else {
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

void tlp::copyToGraph (Graph *outG, Graph* inG,
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
    } delete itE;
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
      if (dynamic_cast<GraphProperty *>(src) == 0) {
	const std::string& pName = src->getName();
	PropertyInterface *dst =
	  outG->existProperty(pName) ? outG->getProperty(pName)
	  : src->clonePrototype(outG, pName);
	dst->copy( nOut, nIn, src );
      }
    } delete propIt;
  } delete nodeIt;

  // loop on selected edges
  Iterator<edge> * edgeIt =
    inSel ? inSel->getNonDefaultValuatedEdges(inG) : inG->getEdges();
	
  edgeIt = inSel ? inSel->getNonDefaultValuatedEdges(inG) : inG->getEdges();
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
      if (dynamic_cast<GraphProperty *>(src) == 0) {
	const std::string& pName = src->getName();
	PropertyInterface *dst =
	  outG->existProperty(pName) ? outG->getProperty(pName)
	  : src->clonePrototype(outG, pName);
	dst->copy( eOut, eIn, src );
      }
    } delete propIt;
  } delete edgeIt;
}

void Graph::notifyDestroy() {
  ObservableGraph::notifyDestroy(this);
  Observable::notifyDestroy();
}

void Graph::notifyAddSubGraph(Graph* sg) {
  ObservableGraph::notifyAddSubGraph(this, sg);
  Observable::notifyObservers();
}

DataType* Graph::getAttribute(const std::string& name) const {
    return getAttributes().getData(name);
}

void Graph::setAttribute(const std::string &name, const DataType* value) {
  notifyBeforeSetAttribute(this, name);
  getNonConstAttributes().setData(name, value);
  notifyAfterSetAttribute(this, name);
}

