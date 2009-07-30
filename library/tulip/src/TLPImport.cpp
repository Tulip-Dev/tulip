#include <fstream>
#include <string>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>

#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif

#include <tulip/TulipPlugin.h>
#include <tulip/GraphProperty.h>
#include <tulip/Types.h>
#include <tulip/Coord.h>
#include <tulip/TlpTools.h>
#include <tulip/AbstractProperty.h>
#include <tulip/TLPParser.h>

#define TLP "tlp"
#define AUTHOR "author"
#define COMMENTS "comments"
#define TLP_VERSION 2.0
#define NODES "nodes"
#define EDGE "edge"
#define CLUSTER "cluster"
#define CLUSTERNODES "nodes"
#define CLUSTEREDGES "edges"
#define PROPERTY "property"
#define METRIC "metric"
#define METAGRAPH "metagraph"
#define GRAPH "graph"
#define LAYOUT "layout"
#define SIZE "size"
#define COLOR "color"
#define COORD "coord"
#define INT "int"
#define UINT "uint"
#define DOUBLE "double"
#define FLOAT "float"
#define STRING "string"
#define BOOL "bool"
#define SIZE_VECTOR "vector<size>"
#define COLOR_VECTOR "vector<color>"
#define COORD_VECTOR "vector<coord>"
#define INT_VECTOR "vector<int>"
#define DOUBLE_VECTOR "vector<double>"
#define STRING_VECTOR "vector<string>"
#define BOOL_VECTOR "vector<bool>"
#define DATASET "DataSet"
#define NODESVALUE "node"
#define EDGESVALUE "edge"
#define DEFAULTVALUE "default"

#define DISPLAYING "displaying"
#define GLYPH "glyph"
#define PLUGIN "plugin"
#define ATTRIBUTES "attributes"
#define SCENE "scene"
#define VIEWS "views"
#define CONTROLLER "controller"

using namespace std;
using namespace tlp;

#ifndef DOXYGEN_NOTFOR_DEVEL

namespace tlp {
//=================================================================================
struct TLPGraphBuilder:public TLPTrue {
  Graph *_graph;
  map<int,node> nodeIndex;
  map<int,edge> edgeIndex;
  map<int,Graph *> clusterIndex;
  DataSet *dataSet;
  bool inTLP;
  float version;

  TLPGraphBuilder(Graph *graph, DataSet *dataSet): _graph(graph),
						   dataSet(dataSet) {
    clusterIndex[0]=graph;
    inTLP = false;
    version = 0;
  }

  virtual ~TLPGraphBuilder(){
  }

  bool addDouble(const double val) {
    // only used to handle the version of tlp file format
    if (!version) {
      version = (float) val;
      return version == TLP_VERSION;
    }
    return false;
  }

  bool addNode(int id) {
    nodeIndex[id]=_graph->addNode();
    return true;
  }
  bool addClusterNode(int id, int nodeId) {
    if (_graph->isElement(nodeIndex[nodeId]) && clusterIndex[id]) {
      clusterIndex[id]->addNode(nodeIndex[nodeId]);
      return true;
    }
    return false;
  }
  bool addClusterEdge(int id, int edgeId)  {
    if (_graph->isElement(edgeIndex[edgeId]) && clusterIndex[id]) {
      clusterIndex[id]->addEdge(edgeIndex[edgeId]);
      return true;
    }
    return true;
  }
  bool addEdge(int id, int idSource,int idTarget) {
    if (_graph->isElement(nodeIndex[idSource]) && _graph->isElement(nodeIndex[idTarget])) {
      edgeIndex[id]=_graph->addEdge(nodeIndex[idSource],nodeIndex[idTarget]);
      return true;
    }
    return false;
  }
  bool setNodeValue(int nodeId, int clusterId, const string& propertyType, const string& propertyName, string& value) {
    if (_graph->isElement(nodeIndex[nodeId]) && clusterIndex[clusterId]) {
      //cerr << "set node value ....." ;
      if (propertyType==GRAPH || propertyType==METAGRAPH) { // METAGRAPH was used in Tulip 2
	//      cerr << ".";
	char *endPtr=0;
	const char *startPtr=value.c_str();
	int result=strtol(startPtr,&endPtr,10);
	if (endPtr==startPtr) return false;
	if (clusterIndex.find(result)==clusterIndex.end()) return false;
	if (result==0)
	  clusterIndex[clusterId]->getLocalProperty<GraphProperty>(propertyName)->setNodeValue(nodeIndex[nodeId],0);
	else
	  clusterIndex[clusterId]->getLocalProperty<GraphProperty>(propertyName)->setNodeValue(nodeIndex[nodeId],clusterIndex[result]);
	return true;
      }
      if (propertyType==DOUBLE || propertyType==METRIC) // METRIC was used in Tulip 2
	return clusterIndex[clusterId]->getLocalProperty<DoubleProperty>(propertyName)->setNodeStringValue( nodeIndex[nodeId], value );
      if (propertyType==LAYOUT)
	return clusterIndex[clusterId]->getLocalProperty<LayoutProperty>(propertyName)->setNodeStringValue( nodeIndex[nodeId], value );
      if (propertyType==SIZE)
	return clusterIndex[clusterId]->getLocalProperty<SizeProperty>(propertyName)->setNodeStringValue( nodeIndex[nodeId], value );
      if (propertyType==COLOR)
	return clusterIndex[clusterId]->getLocalProperty<ColorProperty>(propertyName)->setNodeStringValue( nodeIndex[nodeId], value );
      if (propertyType==INT)
	return clusterIndex[clusterId]->getLocalProperty<IntegerProperty>(propertyName)->setNodeStringValue( nodeIndex[nodeId], value );
      if (propertyType==BOOL)
	return clusterIndex[clusterId]->getLocalProperty<BooleanProperty>(propertyName)->setNodeStringValue( nodeIndex[nodeId], value );
      if (propertyType==STRING)
	return clusterIndex[clusterId]->getLocalProperty<StringProperty>(propertyName)->setNodeStringValue( nodeIndex[nodeId], value );
      if (propertyType==SIZE_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<SizeVectorProperty>(propertyName)->setNodeStringValue( nodeIndex[nodeId], value );
      if (propertyType==COLOR_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<ColorVectorProperty>(propertyName)->setNodeStringValue( nodeIndex[nodeId], value );
      if (propertyType==COORD_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<CoordVectorProperty>(propertyName)->setNodeStringValue( nodeIndex[nodeId], value );
      if (propertyType==DOUBLE_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<DoubleVectorProperty>(propertyName)->setNodeStringValue( nodeIndex[nodeId], value );
      if (propertyType==INT_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<IntegerVectorProperty>(propertyName)->setNodeStringValue( nodeIndex[nodeId], value );
      if (propertyType==BOOL_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<BooleanVectorProperty>(propertyName)->setNodeStringValue( nodeIndex[nodeId], value );
      if (propertyType==STRING_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<StringVectorProperty>(propertyName)->setNodeStringValue( nodeIndex[nodeId], value );
    }
    return false;
    //cer << "..ok" << endl;
  }

  bool setEdgeValue(int edgeId, int clusterId, const string& propertyType, const string& propertyName, string& value) {
    if (_graph->isElement(edgeIndex[edgeId]) && clusterIndex[clusterId]) {
      //cerr << "setEdgeValue...." << "edge:" << edgeId << " cluster " << clusterId << " " << propertyName << " " << propertyType << " value=\""<< value<<"\"  ";
      bool result=false;

      if (propertyType==GRAPH || propertyType==METAGRAPH)  { // METAGRAPH was used in Tulip 2
	set<edge> v;
	result = EdgeSetType::fromString(v, value);
	if (result)
	  clusterIndex[clusterId]->getLocalProperty<GraphProperty>(propertyName)->setEdgeValue( edgeIndex[edgeId], v);
	return result;
      }
      if (propertyType==DOUBLE || propertyType==METRIC) // METRIC was used in Tulip 2
	return clusterIndex[clusterId]->getLocalProperty<DoubleProperty>(propertyName)->setEdgeStringValue( edgeIndex[edgeId], value );
      if (propertyType==LAYOUT)
	return clusterIndex[clusterId]->getLocalProperty<LayoutProperty>(propertyName)->setEdgeStringValue( edgeIndex[edgeId], value );
      if (propertyType==SIZE)
	return clusterIndex[clusterId]->getLocalProperty<SizeProperty>(propertyName)->setEdgeStringValue( edgeIndex[edgeId], value );
      if (propertyType==COLOR)
	return clusterIndex[clusterId]->getLocalProperty<ColorProperty>(propertyName)->setEdgeStringValue( edgeIndex[edgeId], value );
      if (propertyType==INT)
	return clusterIndex[clusterId]->getLocalProperty<IntegerProperty>(propertyName)->setEdgeStringValue( edgeIndex[edgeId], value );
      if (propertyType==BOOL)
	return clusterIndex[clusterId]->getLocalProperty<BooleanProperty>(propertyName)->setEdgeStringValue( edgeIndex[edgeId], value );
      if (propertyType==STRING)
	return clusterIndex[clusterId]->getLocalProperty<StringProperty>(propertyName)->setEdgeStringValue( edgeIndex[edgeId], value );
      if (propertyType==SIZE_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<SizeVectorProperty>(propertyName)->setEdgeStringValue( edgeIndex[edgeId], value );
      if (propertyType==COLOR_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<ColorVectorProperty>(propertyName)->setEdgeStringValue( edgeIndex[edgeId], value );
      if (propertyType==COORD_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<CoordVectorProperty>(propertyName)->setEdgeStringValue( edgeIndex[edgeId], value );
      if (propertyType==DOUBLE_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<DoubleVectorProperty>(propertyName)->setEdgeStringValue( edgeIndex[edgeId], value );
      if (propertyType==INT_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<IntegerVectorProperty>(propertyName)->setEdgeStringValue( edgeIndex[edgeId], value );
      if (propertyType==BOOL_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<BooleanVectorProperty>(propertyName)->setEdgeStringValue( edgeIndex[edgeId], value );
      if (propertyType==STRING_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<StringVectorProperty>(propertyName)->setEdgeStringValue( edgeIndex[edgeId], value );
    }
    return false;
  }

  bool setAllNodeValue(int clusterId, const string& propertyType, const string& propertyName, string& value) {
    if (clusterIndex[clusterId]) {
      //cerr << "set all node value ....." ;
      if (propertyType==GRAPH || propertyType==METAGRAPH) { // METAGRAPH was used in Tulip 2
	char *endPtr=0;
	const char *startPtr=value.c_str();
	int result=strtol(startPtr,&endPtr,10);
	if (endPtr==startPtr) result =0; //return false;
	if (clusterIndex.find(result)==clusterIndex.end()) return false;
	if (result==0)
	  clusterIndex[clusterId]->getLocalProperty<GraphProperty>(propertyName)->setAllNodeValue(0);
	else
	  clusterIndex[clusterId]->getLocalProperty<GraphProperty>(propertyName)->setAllNodeValue(clusterIndex[result]);
	return true;
      }
      if (propertyType==DOUBLE || propertyType==METRIC) // METRIC was used in Tulip 2
	return clusterIndex[clusterId]->getLocalProperty<DoubleProperty>(propertyName)->setAllNodeStringValue( value );
      if (propertyType==LAYOUT)
	return clusterIndex[clusterId]->getLocalProperty<LayoutProperty>(propertyName)->setAllNodeStringValue( value );
      if (propertyType==SIZE)
	return clusterIndex[clusterId]->getLocalProperty<SizeProperty>(propertyName)->setAllNodeStringValue( value );
      if (propertyType==COLOR)
	return clusterIndex[clusterId]->getLocalProperty<ColorProperty>(propertyName)->setAllNodeStringValue( value );
      if (propertyType==INT)
	return clusterIndex[clusterId]->getLocalProperty<IntegerProperty>(propertyName)->setAllNodeStringValue( value );
      if (propertyType==BOOL)
	return clusterIndex[clusterId]->getLocalProperty<BooleanProperty>(propertyName)->setAllNodeStringValue( value );
      if (propertyType==STRING)
	return clusterIndex[clusterId]->getLocalProperty<StringProperty>(propertyName)->setAllNodeStringValue( value );
      if (propertyType==SIZE_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<SizeVectorProperty>(propertyName)->setAllNodeStringValue( value );
      if (propertyType==COLOR_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<ColorVectorProperty>(propertyName)->setAllNodeStringValue( value );
      if (propertyType==COORD_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<CoordVectorProperty>(propertyName)->setAllNodeStringValue( value );
      if (propertyType==DOUBLE_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<DoubleVectorProperty>(propertyName)->setAllNodeStringValue( value );
      if (propertyType==INT_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<IntegerVectorProperty>(propertyName)->setAllNodeStringValue( value );
      if (propertyType==BOOL_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<BooleanVectorProperty>(propertyName)->setAllNodeStringValue( value );
      if (propertyType==STRING_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<StringVectorProperty>(propertyName)->setAllNodeStringValue( value );
    }
    return false;
    //cer << "..ok" << endl;
  }
  bool setAllEdgeValue(int clusterId, const string& propertyType, const string& propertyName, string& value) {
    if (clusterIndex[clusterId]) {
      //cerr << "setAllEdgeValue.." << endl;
      bool result=false;

      if (propertyType==GRAPH|| propertyType==METAGRAPH)  { // METAGRAPH was used in Tulip 2
	set<edge> v;
	result = EdgeSetType::fromString(v, value);
	if (result)
	  clusterIndex[clusterId]->getLocalProperty<GraphProperty>(propertyName)->setAllEdgeValue(v);
	return result;
      }
      if (propertyType==DOUBLE || propertyType==METRIC) // METRIC was used in Tulip 2
	return clusterIndex[clusterId]->getLocalProperty<DoubleProperty>(propertyName)->setAllEdgeStringValue( value );
      if (propertyType==LAYOUT)
	return clusterIndex[clusterId]->getLocalProperty<LayoutProperty>(propertyName)->setAllEdgeStringValue( value );
      if (propertyType==SIZE)
	return clusterIndex[clusterId]->getLocalProperty<SizeProperty>(propertyName)->setAllEdgeStringValue( value );
      if (propertyType==COLOR)
	return clusterIndex[clusterId]->getLocalProperty<ColorProperty>(propertyName)->setAllEdgeStringValue( value );
      if (propertyType==INT)
	return clusterIndex[clusterId]->getLocalProperty<IntegerProperty>(propertyName)->setAllEdgeStringValue( value );
      if (propertyType==BOOL)
	return clusterIndex[clusterId]->getLocalProperty<BooleanProperty>(propertyName)->setAllEdgeStringValue( value );
      if (propertyType==STRING)
	return clusterIndex[clusterId]->getLocalProperty<StringProperty>(propertyName)->setAllEdgeStringValue( value );
      if (propertyType==SIZE_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<SizeVectorProperty>(propertyName)->setAllEdgeStringValue( value );
      if (propertyType==COLOR_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<ColorVectorProperty>(propertyName)->setAllEdgeStringValue( value );
      if (propertyType==COORD_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<CoordVectorProperty>(propertyName)->setAllEdgeStringValue( value );
      if (propertyType==DOUBLE_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<DoubleVectorProperty>(propertyName)->setAllEdgeStringValue( value );
      if (propertyType==INT_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<IntegerVectorProperty>(propertyName)->setAllEdgeStringValue( value );
      if (propertyType==BOOL_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<BooleanVectorProperty>(propertyName)->setAllEdgeStringValue( value );
      if (propertyType==STRING_VECTOR)
	return clusterIndex[clusterId]->getLocalProperty<StringVectorProperty>(propertyName)->setAllEdgeStringValue( value );
    }
    return false;
  }
  bool addCluster(int id, const string& name, int supergraphId=0) {
    if (clusterIndex[supergraphId]) {
      BooleanProperty sel(clusterIndex[supergraphId]);
      sel.setAllNodeValue(false);
      sel.setAllEdgeValue(false);
      clusterIndex[id] = clusterIndex[supergraphId]->addSubGraph(&sel, id);
      clusterIndex[id]->setAttribute("name", name);
      return true;
    }
    return false;
  }
  bool addStruct(const string& structName,TLPBuilder*&newBuilder) ;
};
//=================================================================================
struct TLPNodeBuilder:public TLPFalse {
  TLPGraphBuilder *graphBuilder;
  TLPNodeBuilder(TLPGraphBuilder *graphBuilder):graphBuilder(graphBuilder){}
  bool addInt(const int id) {return graphBuilder->addNode(id);}
  bool close(){return true;}
};
//=================================================================================
struct TLPEdgeBuilder:public TLPFalse {
  TLPGraphBuilder *graphBuilder;
  int parameter[3];
  int nbParameter;
  TLPEdgeBuilder(TLPGraphBuilder *graphBuilder):graphBuilder(graphBuilder),nbParameter(0){}
  bool addInt(const int id)  {
    if (nbParameter<3) {
      parameter[nbParameter]=id;
      nbParameter++;
      return true;
    }
    else
      return false;
  }
  bool close()  {
    if (nbParameter==3) {
      return graphBuilder->addEdge(parameter[0],parameter[1],parameter[2]);
    }
    else
      return false;
  }
};
//=================================================================================
struct TLPClusterBuilder:public TLPFalse {
  TLPGraphBuilder *graphBuilder;
  int clusterId, supergraphId;
  TLPClusterBuilder(TLPGraphBuilder *graphBuilder, int supergraph=0):graphBuilder(graphBuilder), supergraphId(supergraph){}
  bool addInt(const int id) {
    clusterId = id;
    return true;
  }
  bool addString(const string& str) {
    return graphBuilder->addCluster(clusterId , str, supergraphId);
  }
  bool addStruct(const string& structName, TLPBuilder*&newBuilder);
  bool addNode (int nodeId) {
    return graphBuilder->addClusterNode(clusterId,nodeId);
  }
  bool addEdge (int edgeId) {
    return graphBuilder->addClusterEdge(clusterId,edgeId);
  }
  bool close(){
    return true;
  }
};
//=================================================================================
struct TLPClusterNodeBuilder:public TLPFalse {
  TLPClusterBuilder *clusterBuilder;
  TLPClusterNodeBuilder(TLPClusterBuilder *clusterBuilder):clusterBuilder(clusterBuilder){}
  bool addInt(const int id) {return clusterBuilder->addNode(id);}
  bool close(){return true;}
};
//=================================================================================
struct TLPClusterEdgeBuilder:public TLPFalse {
  TLPClusterBuilder *clusterBuilder;
  TLPClusterEdgeBuilder(TLPClusterBuilder *clusterBuilder):clusterBuilder(clusterBuilder){}
  bool addInt(const int id) {return clusterBuilder->addEdge(id);}
  bool close(){return true;}
};
//================================================================================
struct TLPDataSetBuilder: public TLPFalse {
  TLPGraphBuilder *graphBuilder;
  DataSet dataSet;
  DataSet *currentDataSet;
  char* dataSetName;
  string dataSetNameStr;

  TLPDataSetBuilder(TLPGraphBuilder *graphBuilder):graphBuilder(graphBuilder),currentDataSet(graphBuilder->dataSet){
    dataSetName = (char *) 0;
  }
  TLPDataSetBuilder(TLPGraphBuilder *graphBuilder, char* name):graphBuilder(graphBuilder),currentDataSet(graphBuilder->dataSet){
    dataSetName = name;
    graphBuilder->dataSet->get(dataSetName, dataSet);
  }
  TLPDataSetBuilder(TLPGraphBuilder *graphBuilder,DataSet *currentDataSet):graphBuilder(graphBuilder),currentDataSet(currentDataSet){
  }
  virtual ~TLPDataSetBuilder(){
  }
  bool addStruct(const string& structName, TLPBuilder*&newBuilder);
  bool addString(const string &str) {
    dataSetName = (char *) 0;
    dataSetNameStr = str;
    return true;
  }
  bool close(){
    if (dataSetName) {
      // handle old displaying properties
      // to ensure compatibility
      bool b;
      if (dataSet.get<bool>("_viewArrow", b))
	dataSet.set("arrow", b);
      if (dataSet.get<bool>("_viewLabel", b)) {
	dataSet.set("nodeLabel", b);
	dataSet.set("edgeLabel", b);
      }
      if (dataSet.get<bool>("_viewNodeLabel", b))
	dataSet.set("nodeLabel", b);
      if (dataSet.get<bool>("_viewEdgeLabel", b))
	dataSet.set("edgeLabel", b);
      if (dataSet.get<bool>("_viewMetaLabel", b))
	dataSet.set("metaLabel", b);
      if (dataSet.get<bool>("_viewElementOrdered", b))
	dataSet.set("elementOrdered", b);
      if (dataSet.get<bool>("_viewStrahler", b))
	dataSet.set("elementOrdered", b);
      if (dataSet.get<bool>("_viewAutoScale", b))
	dataSet.set("autoScale", b);
      if (dataSet.get<bool>("_incrementalRendering", b))
	dataSet.set("incrementalRendering", b);
      if (dataSet.get<bool>("_edgeColorInterpolate", b))
	dataSet.set("edgeColorInterpolation", b);
      if (dataSet.get<bool>("_edgeSizeInterpolate", b))
	dataSet.set("edgeSizeInterpolation", b);
      if (dataSet.get<bool>("_edge3D", b))
	dataSet.set("edge3D", b);
      unsigned int ui;
      if (dataSet.get<unsigned int>("_viewOrtho", ui))
	dataSet.set("orthogonalProjection", ui ? true : false);
      if (dataSet.get<unsigned int>("_FontsType", ui))
	dataSet.set("fontType", ui);
      graphBuilder->dataSet->set<DataSet>(dataSetName, dataSet);
    }
    if(dataSetNameStr!="") {
      currentDataSet->set<DataSet>(dataSetNameStr.c_str(), dataSet);
      Iterator< std::pair<std::string, DataType*> > *it=dataSet.getValues();
      while(it->hasNext()) {
	pair<string, DataType*> p;
	p = it->next();
      }
    }
    return true;
  }
};
//================================================================================
struct TLPFileInfoBuilder: public TLPFalse {
  TLPGraphBuilder *graphBuilder;
  string name;

  TLPFileInfoBuilder(TLPGraphBuilder *graphBuilder, string infoName):
    graphBuilder(graphBuilder), name(infoName){
  }
  virtual ~TLPFileInfoBuilder(){
  }

  bool addString(const string &str) {
    if (name == AUTHOR)
      graphBuilder->dataSet->set<string>(AUTHOR, str);
    else if (name == COMMENTS)
      graphBuilder->dataSet->set<string>("text::comments", str);
    return true;
  }
  bool close(){
    return true;
  }
};
//================================================================================
struct TLPSceneBuilder: public TLPFalse {
  TLPGraphBuilder *graphBuilder;

  TLPSceneBuilder(TLPGraphBuilder *graphBuilder):
    graphBuilder(graphBuilder){
  }
  virtual ~TLPSceneBuilder(){
  }

  bool addString(const string &str) {
    graphBuilder->dataSet->set<string>(SCENE, str);
    return true;
  }
  bool close(){
    return true;
  }
};
//================================================================================
struct TLPDataBuilder : public TLPFalse
{
  TLPDataSetBuilder *dataSetBuilder;
  DataSet *dataSet;
  string type, prop;
  int token;
  TLPDataBuilder(TLPDataSetBuilder *dataSetBuilder, const string& type): dataSetBuilder(dataSetBuilder),
											     type(type), prop(""),
											     token(0) {
    if (dataSetBuilder->dataSetName || dataSetBuilder->dataSetNameStr!="")
      dataSet = &(dataSetBuilder->dataSet);
    else
      dataSet = (DataSet *) &(dataSetBuilder->graphBuilder->_graph->getAttributes());
  }

  ~TLPDataBuilder(){}
  bool addString(const string &val) {
    switch(token++) {
    case 0: prop = val; break;
    case 1: {
      if (type==COLOR) {
	tlp::Color c;
	if ( ColorType::fromString(c,val) )
		dataSet->set(prop, c);
	else cerr << __PRETTY_FUNCTION__ << ": COLOR failed" << endl;
	//else return false;
      }
      else if (type==COORD) {
	Coord c;
	if ( PointType::fromString(c,val) )
		dataSet->set(prop, c);
	else cerr << __PRETTY_FUNCTION__ << ": COORD failed" << endl;
	//else return false;
      }
      else if (type==STRING)
	dataSet->set(prop, val);
      else {
	cerr << __PRETTY_FUNCTION__ << ": TYPE = " << type << ", " << prop << " = " << val << endl;
	return false;
      }
      break;
    }
    default: return false;
    }
    return true;
  }
  bool addBool(const bool val) {
    if ((type==BOOL) && (token++ == 1)) dataSet->set(prop, val);
    return true;
  }

  bool addInt(const int val) {
    if ((type==INT) && (token == 1)) {
      if (prop == "SupergraphId") {
	int v;
	if (dataSetBuilder->graphBuilder->clusterIndex.find(val) != dataSetBuilder->graphBuilder->clusterIndex.end()) {
	  v=dataSetBuilder->graphBuilder->clusterIndex[val]->getId();
	  dataSet->set<int>(prop, v);
	}
	else {
	  //	  cerr << "Import TLP: Warning: no cluster #" << val << " defined." << endl;
	}
      }
      else {
	dataSet->set<int>(prop, val);
      }
    }
    else if ((type==UINT) && (token == 1)) {
      dataSet->set<unsigned int>(prop, val);
    }
    token++;
    return true;
  }

  bool addDouble(const double val) {
    if ((type==DOUBLE) && (token == 1)) {
      dataSet->set<double>(prop, val);
      token++;
      return true;
    }
    else if ((type==FLOAT) && (token == 1)) {
      dataSet->set<float>(prop, (float) val);
      token++;
      return true;
    }
    else return false;
  }
  bool close() {return (token==2);}
};
//=================================================================================
bool TLPClusterBuilder::addStruct(const string& structName, TLPBuilder*&newBuilder)   {
    if (structName==CLUSTERNODES) {
      newBuilder=new TLPClusterNodeBuilder(this);
    }
    else
      if (structName==CLUSTEREDGES) {
	newBuilder=new TLPClusterEdgeBuilder(this);
      }
      else
	if (structName==CLUSTER) {
	  newBuilder = new TLPClusterBuilder(graphBuilder, clusterId);
	}
	else {
	  newBuilder=new TLPFalse();
	  return false;
	}
    return true;
}
//=================================================================================
struct TLPPropertyBuilder:public TLPFalse {
  TLPGraphBuilder *graphBuilder;
  int clusterId;
  string propertyType,propertyName;
  bool typeOk,nameOk;
  virtual ~TLPPropertyBuilder() {}
  TLPPropertyBuilder(TLPGraphBuilder *graphBuilder):graphBuilder(graphBuilder),typeOk(false),nameOk(false){}
  bool addInt(const int id)  {
    clusterId = id;
    return true;
  }
  bool addString(const string &str)
  {
    if (!typeOk)
      {propertyType=str;typeOk=true;}
    else
      if (!nameOk)
	{propertyName=str;nameOk=true;}
      else
	return false;
    return true;
  }
  bool setNodeValue(int nodeId, const string& value)  {
    return graphBuilder->setNodeValue(nodeId,clusterId,propertyType,propertyName, (string&) value);
  }
  bool setEdgeValue(int edgeId, const string& value)  {
    return graphBuilder->setEdgeValue(edgeId,clusterId,propertyType,propertyName, (string&) value);
  }
  bool setAllNodeValue(const string& value)  {
    return graphBuilder->setAllNodeValue(clusterId,propertyType,propertyName, (string&) value);
  }
  bool setAllEdgeValue(const string value)  {
    return graphBuilder->setAllEdgeValue(clusterId,propertyType,propertyName, (string&) value);
  }
  bool addStruct(const string& structName,TLPBuilder*&newBuilder);
  bool close(){return true;}
};
//=================================================================================
struct TLPNodePropertyBuilder:public TLPFalse {
  TLPPropertyBuilder *propertyBuilder;
  int nodeId;
  TLPNodePropertyBuilder(TLPPropertyBuilder *propertyBuilder):propertyBuilder(propertyBuilder){}
  bool addInt(const int id)
  {nodeId=id;return true;}
  bool addString(const string&val)
  {return propertyBuilder->setNodeValue(nodeId,val);}
  bool close(){return true;}
};
//=================================================================================
struct TLPEdgePropertyBuilder:public TLPFalse {
  TLPPropertyBuilder *propertyBuilder;
  int edgeId;
  string nodeValue;
  TLPEdgePropertyBuilder(TLPPropertyBuilder *propertyBuilder):propertyBuilder(propertyBuilder){}
  virtual ~TLPEdgePropertyBuilder(){}
  bool addInt(const int id)
  {edgeId=id;return true;}
  bool addString(const string &val)
  {return propertyBuilder->setEdgeValue(edgeId,val);}
  bool close(){return true;}
};
struct TLPDefaultPropertyBuilder:public TLPFalse {
  TLPPropertyBuilder *propertyBuilder;
  int edgeId;
  string nodeValue;
  int i;
  TLPDefaultPropertyBuilder(TLPPropertyBuilder *propertyBuilder):propertyBuilder(propertyBuilder),i(0){}
  virtual ~TLPDefaultPropertyBuilder(){}
  bool addString(const string &val)
  {
    if (i==0) {i++;return propertyBuilder->setAllNodeValue(val);}
    if (i==1) {i++;return propertyBuilder->setAllEdgeValue(val);}
    return false;
  }
  bool close(){return true;}
};

} // namespace tlp
//=================================================================================
bool TLPPropertyBuilder::addStruct(const string& structName,TLPBuilder*&newBuilder) {
  if (structName==DEFAULTVALUE) {
    newBuilder= new TLPDefaultPropertyBuilder(this);
    return true;
  }
  else
    if (structName==NODESVALUE) {
      newBuilder=new TLPNodePropertyBuilder(this);
      return true;
    }
    else
      if (structName==EDGESVALUE) {
	newBuilder=new TLPEdgePropertyBuilder(this);
	return true;
      }
  return false;
}
//=================================================================================
bool TLPGraphBuilder::addStruct(const string& structName,TLPBuilder*&newBuilder) {
  if (structName==TLP) {
    inTLP = true;
    newBuilder=this;
  }
  else if (structName==NODES) {
    newBuilder=new TLPNodeBuilder(this);
  }
  else if (structName==EDGE) {
    newBuilder=new TLPEdgeBuilder(this);
  }
  else if (structName==CLUSTER) {
    newBuilder=new TLPClusterBuilder(this);
  }
  else if (structName==PROPERTY) {
    newBuilder=new TLPPropertyBuilder(this);
  }
  else if (structName==DISPLAYING) {
    newBuilder=new TLPDataSetBuilder(this, (char *) DISPLAYING);
  } else if (structName==ATTRIBUTES) {
    newBuilder=new TLPDataSetBuilder(this);
  } else if (structName==SCENE) {
    newBuilder=new TLPSceneBuilder(this);
  } else if (structName==VIEWS) {
    newBuilder=new TLPDataSetBuilder(this, (char *) VIEWS);
  } else if (structName==CONTROLLER) {
    newBuilder=new TLPDataSetBuilder(this, (char *) CONTROLLER);
  }
  else
    newBuilder=new TLPFileInfoBuilder(this, structName);
  return true;
}
//================================================================================
bool TLPDataSetBuilder::addStruct(const string& structName,TLPBuilder*&newBuilder) {
  if (structName==COORD || structName==COLOR || structName==BOOL ||
      structName==INT || structName==UINT || structName==FLOAT ||
      structName==DOUBLE || structName==STRING) {
    newBuilder= new TLPDataBuilder(this, structName);
  }else if(structName==DATASET) {
    newBuilder = new TLPDataSetBuilder(this->graphBuilder,&this->dataSet);
  }else
    newBuilder= new TLPTrue();
  return true;
}
//================================================================================

namespace {
  const char * paramHelp[] = {
    // filename
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "pathname" ) \
    HTML_HELP_BODY() \
    "This parameter defines the file pathname to import." \
    HTML_HELP_CLOSE(),
  };
}

namespace tlp {
#endif //DOXYGEN_NOTFOR_DEVEL

/** \addtogroup import */
/*@{*/
/// Import plugin for TLP format.
/**
 * This plugin imports a Tulip graph structure recorded in a file
 * using the TLP format.
 * TLP is the Tulip Software Graph Format.
 * See 'Tulip-Software.org->Docs->TLP File Format' for description.
 * Note: When using the Tulip graphical user interface,
 * choosing "File->Import->TLP" menu item is the same that using
 * "File->Open" menu item.
 */
class TLPImport:public ImportModule {
public:
  TLPImport(AlgorithmContext context):ImportModule(context) {
    addParameter<string>("file::filename", paramHelp[0]);
    addParameter<DataSet>(DISPLAYING);
  }
  ~TLPImport(){}
  bool import(const string &dummy) {
    string filename;
    string data;
    stringstream *tmpss=NULL;
    int size;
    istream *input;
    bool result;

    if(dataSet->exist("file::filename")){
      dataSet->get<string>("file::filename", filename);
      struct stat infoEntry;
      result = (stat(filename.c_str(),&infoEntry) == 0);
      if (!result) {
        std::stringstream ess;
        ess << filename.c_str() << ": " << strerror(errno);
        pluginProgress->setError(ess.str());
        std::cerr << pluginProgress->getError() << std::endl;
        return false;
      }
      size=infoEntry.st_size;
      if (filename.rfind(".gz") == (filename.length() - 3)) {
        input = tlp::getIgzstream(filename.c_str());
        size *= 4;
      }
      else
        input = new ifstream(filename.c_str());
    }else{
      dataSet->get<string>("file::data", data);
      size=data.size();
      stringstream *tmpss=new stringstream;
      (*tmpss) << data;
      input = tmpss;
    }

    pluginProgress->showPreview(false);
    pluginProgress->setComment(string("Loading ") + filename + "...");
    TLPParser<false> myParser(*input, new TLPGraphBuilder(graph, dataSet), pluginProgress, size);
    result = myParser.parse();
    if (!result) {
      pluginProgress->setError(filename + ": " + pluginProgress->getError());
      std::cerr << pluginProgress->getError() << std::endl;
    }
    if(tmpss)
      delete tmpss;
    delete input;
    return result;
  }
};
IMPORTPLUGINOFGROUP(TLPImport,"tlp","Auber","16/02/2001","Tlp Import plugin","1.0","File");
/*@}*/
}
