#include <fstream>
#include <tulip/TulipPlugin.h>
#include <tulip/AbstractProperty.h>
#include "GMLParser.h"

#define NODE "node"
#define EDGE "edge"
#define SOURCE "source"
#define TARGET "target"
#define GRAPH "graph"
#define LABEL "label"
#define ID "id"
#define GRAPHICS "graphics"
#define POINT "point"
#define LINE "Line"
#define DEFAULTVALUE "default"

using namespace std;
using namespace tlp;

void nodeAttributeError() {
  cerr << "Error reading node attribute: The attributes of nodes must be defined after the node id (data ignored)" << endl;
}
void edgeAttributeError() {
  cerr << "Error reading edge attribute: The attributes of edges must be defined after source and target (data ignored)" << endl;
}

//=================================================================================
struct GMLGraphBuilder:public GMLTrue {
  Graph *_graph;
  map<int,node> nodeIndex;
  virtual ~GMLGraphBuilder(){}
  GMLGraphBuilder(Graph *graph):_graph(graph) {
    //cer << "buil GraphBuilder" << endl;
  }
  bool addNode(int id) {
    if (nodeIndex.find(id)==nodeIndex.end()) {nodeIndex[id]=_graph->addNode();}
    return true;
  }
  edge addEdge(int idSource,int idTarget) {
    // return and invalid edge if one of the two nodes does not exits
    if (_graph->isElement(nodeIndex[idSource]) && _graph->isElement(nodeIndex[idTarget]))
      return _graph->addEdge(nodeIndex[idSource],nodeIndex[idTarget]);
    return edge();
  }
  bool setNodeValue(int nodeId, const string propertyName, string value) {
    if (_graph->isElement(nodeIndex[nodeId])) {
      _graph->getLocalProperty<StringProperty>(propertyName)->setNodeValue(nodeIndex[nodeId],value);
      return true;
    }
    return false;
  }
  bool setNodeValue(int nodeId, const string propertyName, double value) { 
    if (_graph->isElement(nodeIndex[nodeId])) {
      _graph->getLocalProperty<DoubleProperty>(propertyName)->setNodeValue(nodeIndex[nodeId],value);
      return true;
    }
    return false;
  }
  bool setNodeValue(int nodeId, const string propertyName, int value) { 
    if (_graph->isElement(nodeIndex[nodeId])) {
      _graph->getLocalProperty<IntegerProperty>(propertyName)->setNodeValue(nodeIndex[nodeId],value);
      return true;
    }
    return false;
  }
  bool setNodeValue(int nodeId, const string propertyName, bool value) { 
    if (_graph->isElement(nodeIndex[nodeId])) {
      _graph->getLocalProperty<BooleanProperty>(propertyName)->setNodeValue(nodeIndex[nodeId],value);
      return true;
    }
    return false;
  }
  bool setNodeValue(int nodeId, const string propertyName, Coord value) { 
    if (_graph->isElement(nodeIndex[nodeId])) {
      _graph->getLocalProperty<LayoutProperty>(propertyName)->setNodeValue(nodeIndex[nodeId],value);
      return true;
    }
    return false;
  }
  bool setNodeValue(int nodeId, const string propertyName, Size value) { 
    if (_graph->isElement(nodeIndex[nodeId])) {
      _graph->getLocalProperty<SizeProperty>(propertyName)->setNodeValue(nodeIndex[nodeId],value);
      return true;
    }
    return false;
  }
  bool setNodeValue(int nodeId, const string propertyName, Color value) { 
    if (_graph->isElement(nodeIndex[nodeId])) {
      _graph->getLocalProperty<ColorProperty>(propertyName)->setNodeValue(nodeIndex[nodeId],value);
      return true;
    }
    return false;
  }

  bool setEdgeValue(edge e, const string &propertyName, string value) {
    return true;
  }
  bool setEdgeValue(edge e, const string &propertyName, int value) {
    return true;
  }
  bool setEdgeValue(edge e, const string &propertyName, bool value) {
    return true;
  }
  bool setEdgeValue(edge e, const string &propertyName, double value) {
    return true;
  }
  void setEdgeValue(edge e,const LineType::RealType & lCoord) {
    _graph->getLocalProperty<LayoutProperty>("viewLayout")->setEdgeValue(e,lCoord);
  }
  bool setAllNodeValue(const string propertyType, const string propertyName, string value) {
    return true;
  }
  bool setAllEdgeValue(const string &propertyType, const string &propertyName, string value) {
    return true;
  }
  bool addStruct(const string& structName,GMLBuilder*&newBuilder) ;
};
//=================================================================================
struct GMLNodeBuilder:public GMLBuilder {
  GMLGraphBuilder *graphBuilder;
  int idSet;

  GMLNodeBuilder(GMLGraphBuilder *graphBuilder):
    graphBuilder(graphBuilder),
    idSet(-1)
  {}

  bool addInt(const string &st,const int id) {
    if (st==ID) {
      bool result=graphBuilder->addNode(id);
      if (result) idSet=id;
      else return false;
    }
    else {
      if (idSet!=-1)
	graphBuilder->setNodeValue(idSet, st, id);
      else
	nodeAttributeError();
    }
    return true;
  }
  bool addDouble(const string &st,const double real) {
    if (idSet!=-1)
      graphBuilder->setNodeValue(idSet, st, real);
    else
      nodeAttributeError();
    return true;
  }
  bool addString(const string &st,const string &str) {
    if (idSet!=-1) {
      if (st==LABEL)
	graphBuilder->setNodeValue(idSet,"viewLabel",str);
      else
	graphBuilder->setNodeValue(idSet, st, str);
    }
    else
      nodeAttributeError();
    return true;
  }
  bool addBool(const string &st,const bool boolean) {    
    if (idSet!=-1)
      graphBuilder->setNodeValue(idSet,st,boolean);
    else
      nodeAttributeError();
    return true;
  }
  void setColor(const Color &color) {
    graphBuilder->setNodeValue(idSet,"viewColor",color);
  }
  void setSize(const Size &size) {
    graphBuilder->setNodeValue(idSet,"viewSize",size);
  }
  void setCoord(const Coord &coord) {
    graphBuilder->setNodeValue(idSet,"viewLayout",coord);
  }
  bool addStruct(const string& structName,GMLBuilder*&newBuilder);
  bool close(){return true;}
};

//=================================================================================
struct GMLNodeGraphicsBuilder:public GMLTrue {
  GMLNodeBuilder *nodeBuilder;
  Coord coord;
  Size size;
  Color color;

  GMLNodeGraphicsBuilder(GMLNodeBuilder *nodeBuilder):
    nodeBuilder(nodeBuilder),
    coord(Coord(0,0,0)),
    size(Size(1,1,1)),
    color(Color(0,0,0,0))
  {}

  bool addInt(const string &st,const int integer) {
    if (st=="x") coord.setX(integer);
    if (st=="y") coord.setY(integer);
    if (st=="z") coord.setZ(integer);
    if (st=="w") size.setH(integer);
    if (st=="h") size.setW(integer);
    if (st=="d") size.setD(integer);
    return true;
  }
  bool addDouble(const string &st,const double real) {
    if (st=="x") coord.setX(real);
    if (st=="y") coord.setY(real);
    if (st=="z") coord.setZ(real);
    if (st=="w") size.setH(real);
    if (st=="h") size.setW(real);
    if (st=="d") size.setD(real);
    return true;
  }
  bool addString(const string &st,const string &str) {return true;}
  bool close() {
    nodeBuilder->setCoord(coord);
    nodeBuilder->setColor(color);
    nodeBuilder->setSize(size);
    return true;
  }
};
//=================================================================================
bool GMLNodeBuilder::addStruct(const string& structName,GMLBuilder*&newBuilder) {
  if (idSet==-1) {
    newBuilder=new GMLTrue();
    nodeAttributeError();
    return true;
  }
  if (structName==GRAPHICS)
    newBuilder=new GMLNodeGraphicsBuilder(this);
  else
    newBuilder=new GMLTrue();
  return true;
}
//=================================================================================
struct GMLEdgeBuilder:public GMLTrue {
  GMLGraphBuilder *graphBuilder;
  int source,target;
  bool edgeOk;
  edge curEdge;

  GMLEdgeBuilder(GMLGraphBuilder *graphBuilder):
    graphBuilder(graphBuilder),
    source(-1),
    target(-1),
    edgeOk(false) {
  }
  bool addInt(const string &st,const int id) {
    bool result=true;
    if (st==SOURCE) source=id;
    if (st==TARGET) target=id;
    if ((!edgeOk) && (source!=-1) && (target!=-1)) {edgeOk=true;curEdge=graphBuilder->addEdge(source,target);}
    if ((st!=SOURCE) && (st!=TARGET))
      if (edgeOk && curEdge.isValid())
	result=graphBuilder->setEdgeValue(curEdge, st, id);
      else
	edgeAttributeError();
    return result;
  }
  bool addDouble(const string &st,const double real) {
    if (edgeOk)
      graphBuilder->setEdgeValue(curEdge, st, real);
    else
      edgeAttributeError();
    return true;
  }
  bool addString(const string &st,const string &str) {
    if (edgeOk)
      graphBuilder->setEdgeValue(curEdge, st, str);
    else
      edgeAttributeError();
    return true;
  }
  bool addBool(const string &st,const bool boolean) {
    if (edgeOk)
      graphBuilder->setEdgeValue(curEdge, st, boolean);
    else
      edgeAttributeError();
    return true;
  }
  void setEdgeValue(const LineType::RealType &lCoord) {
    graphBuilder->setEdgeValue(curEdge,lCoord);
  }
  bool addStruct(const string& structName,GMLBuilder*&newBuilder);
  bool close()  {
    return true;
  }
};
//=================================================================================
struct GMLEdgeGraphicsBuilder:public GMLTrue {
  GMLEdgeBuilder *edgeBuilder;
  Size size;
  Color color;

  GMLEdgeGraphicsBuilder(GMLEdgeBuilder *edgeBuilder):
    edgeBuilder(edgeBuilder),
    size(Size(0,0,0)),
    color(Color(0,0,0,0))
  {}
  bool addString(const string &st,const string &str) {
    return true;
  }
  void setLine(const LineType::RealType &lCoord) {
    edgeBuilder->setEdgeValue(lCoord);
  }
  bool addStruct(const string& structName,GMLBuilder*&newBuilder);
  bool close() {
    return true;
  }
};
//=================================================================================
struct GMLEdgeGraphicsLineBuilder:public GMLTrue {
  GMLEdgeGraphicsBuilder *edgeGraphicsBuilder;
  LineType::RealType lCoord;
  GMLEdgeGraphicsLineBuilder(GMLEdgeGraphicsBuilder *edgeGraphicsBuilder):
    edgeGraphicsBuilder(edgeGraphicsBuilder)
  {}
  virtual ~GMLEdgeGraphicsLineBuilder(){}
  bool addStruct(const string& structName,GMLBuilder*&newBuilder);
  void addPoint(const Coord &coord) {
    lCoord.push_back(coord);
  }
  bool close() {
    edgeGraphicsBuilder->setLine(lCoord);
    return true;
  }
};
//=================================================================================
struct GMLEdgeGraphicsLinePointBuilder:public GMLTrue {
  GMLEdgeGraphicsLineBuilder *edgeGraphicsLineBuilder;
  Coord coord;
  GMLEdgeGraphicsLinePointBuilder(GMLEdgeGraphicsLineBuilder *edgeGraphicsLineBuilder):
    edgeGraphicsLineBuilder(edgeGraphicsLineBuilder),
    coord(0,0,0)
  {}
  bool addInt(const string &st,const int integer) {
    if (st=="x") coord.setX(integer);
    if (st=="y") coord.setY(integer);
    if (st=="z") coord.setZ(integer);
    return true;
  }
  bool addDouble(const string &st,const double real) {
    if (st=="x") coord.setX(real);
    if (st=="y") coord.setY(real);
    if (st=="z") coord.setZ(real);
    return true;
  }
  bool close() {
    edgeGraphicsLineBuilder->addPoint(coord);
    return true;
  }
};
//=================================================================================
bool GMLEdgeGraphicsLineBuilder::addStruct(const string& structName,GMLBuilder*&newBuilder) {
  if (structName==POINT)
    newBuilder=new GMLEdgeGraphicsLinePointBuilder(this);
  else
    newBuilder=new GMLTrue();
  return true;
} 
//=================================================================================
bool GMLEdgeGraphicsBuilder::addStruct(const string& structName,GMLBuilder*&newBuilder) {
  if (structName==LINE)
    newBuilder=new GMLEdgeGraphicsLineBuilder(this);
  else
    newBuilder=new GMLTrue();
  return true;
}
//=================================================================================
bool GMLEdgeBuilder::addStruct(const string& structName,GMLBuilder*&newBuilder) {
  if (!edgeOk) {
    newBuilder=new GMLTrue();
    edgeAttributeError();
    return true;
  }
  if (structName==GRAPHICS)
    newBuilder=new GMLEdgeGraphicsBuilder(this);
  else
    newBuilder=new GMLTrue();
  return true;
}
//=================================================================================
bool GMLGraphBuilder::addStruct(const string& structName,GMLBuilder*&newBuilder) {
  if (structName==GRAPH) {
    newBuilder=new GMLGraphBuilder(_graph);
  }
  else if (structName==NODE) {
    newBuilder=new GMLNodeBuilder(this);
  }
  else if (structName==EDGE) {
    newBuilder=new GMLEdgeBuilder(this);
  }
  else
    newBuilder=new GMLTrue();
  return true;
}
//=================================================================================


namespace {
  const char * paramHelp[] = {
    // filename
    HTML_HELP_OPEN()				    \
    HTML_HELP_DEF( "type", "pathname" )		    \
    HTML_HELP_BODY()						      \
    "This parameter defines the file pathname to import."	      \
    HTML_HELP_CLOSE(),
  };
}


/** \addtogroup import */
/*@{*/
struct GMLImport:public ImportModule {
  GMLImport(ClusterContext context):ImportModule(context) {
    addParameter<string>("file::filename",paramHelp[0]);
  }
  ~GMLImport(){}
  bool import(const string &dummy) {
    string filename;
    dataSet->get<string>("file::filename", filename);
    ifstream myFile(filename.c_str());
    GMLParser<true> myParser(myFile,new GMLGraphBuilder(graph));
    myParser.parse();
    return true;
  }
};
/*@}*/

IMPORTPLUGINOFGROUP(GMLImport,"GML","Auber","04/07/2001","0","0","1","File")
