#include <dirent.h>
#include <string.h>

#include "thirdparty/gzstream/gzstream.h"

#include "tulip/TlpTools.h"
#include "tulip/GraphImpl.h"
#include "tulip/Reflect.h"

#include "tulip/PluginsCreation.h"

using namespace std;

#ifndef _TULIP_LIB_DIR
#ifdef _WIN32
#define _TULIP_LIB_DIR "c:\\Program Files\\Tulip\\lib\\"
#else
#define _TULIP_LIB_DIR "/usr/local/lib/"
#endif
#endif

static const char *TULIP_PLUGINS_PATH_VARIABLE="TLP_PLUGINS_PATH";

string tlp::TulipLibDir;
string tlp::TulipPluginsPath;
string tlp::TulipDocProfile;
string tlp::TulipUserHandBookIndex;
#ifdef _WIN32
const char tlp::PATH_DELIMITER = ';';
#else
const char tlp::PATH_DELIMITER = ':';
#endif
//=========================================================
void tlp::initTulipLib(char* appDirPath) {
  char *getEnvTlp;
  std::string tulipDocDir;
  string::size_type pos;

  getEnvTlp=getenv("TLP_DIR");
  if (getEnvTlp==0) {
    if (appDirPath) {
      // one dir up to initialize the lib dir
      char *last = strrchr(appDirPath, '/');
      last[1] = 0;
      TulipLibDir = std::string(appDirPath) + "lib";
    } else
      TulipLibDir=string(_TULIP_LIB_DIR);
  }
  else
    TulipLibDir=string(getEnvTlp);
#ifdef _WIN32
  // ensure it is a unix-style path
  pos = TulipLibDir.find('\\', 0);
  while(pos != string::npos) {
    TulipLibDir[pos] = '/';
    pos = TulipLibDir.find('\\', pos);
  }
 #endif
  // ensure it is '/' terminated
  if (TulipLibDir[TulipLibDir.length() - 1] != '/')
    TulipLibDir+='/';
  
  getEnvTlp=getenv(TULIP_PLUGINS_PATH_VARIABLE);
  if (getEnvTlp!=0) {
    TulipPluginsPath=string(getEnvTlp);
#ifdef _WIN32
    // ensure it is a unix-style path
    pos = TulipPluginsPath.find('\\', 0);
    while(pos != string::npos) {
      TulipPluginsPath[pos] = '/';
      pos = TulipPluginsPath.find('\\', pos);
    }
#endif
    TulipPluginsPath= TulipLibDir + "tlp/plugins" + PATH_DELIMITER + TulipPluginsPath;
  } else
    TulipPluginsPath= TulipLibDir + "tlp/plugins";
    

  // one dir up to initialize the doc dir
  pos = TulipLibDir.length() - 2;
  pos = TulipLibDir.rfind("/", pos);
  tulipDocDir=TulipLibDir.substr(0, pos + 1)+"share/tulip/";
  TulipDocProfile=tulipDocDir+"profile.adp";
  TulipUserHandBookIndex=tulipDocDir+"userHandbook/html/index.html";
}
//=========================================================
istream *tlp::getIgzstream(const char *name, int open_mode) {
  return new igzstream(name, open_mode);
}

ostream *tlp::getOgzstream(const char *name, int open_mode) {
  return new ogzstream(name, open_mode);
}
//=========================================================
TemplateFactory<ClusteringFactory,Clustering,ClusterContext > *ClusteringFactory::factory = 0;
TemplateFactory<ImportModuleFactory,ImportModule,ClusterContext > *ImportModuleFactory::factory = 0;
TemplateFactory<ExportModuleFactory,ExportModule,ClusterContext > *ExportModuleFactory::factory = 0;
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
  dataSet.set("filename", filename);
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

  if (!ImportModuleFactory::factory->exists(alg)) {
    cerr << "libtulip: " << __FUNCTION__ << ": import plugin \"" << alg
         << "\" doesn't exists (or is not loaded)" << endl;
    return NULL;
  }
  bool newGraphP=false;
  if (newGraph==0) {
    newGraph=new GraphImpl();
    newGraphP=true;
  }

  ClusterContext tmp;
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
  ImportModule *newImportModule=ImportModuleFactory::factory->getObject(alg, tmp);
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
  if (!ExportModuleFactory::factory->exists(alg)) {
    cerr << "libtulip: " << __FUNCTION__ << ": export plugin \"" << alg
         << "\" doesn't exists (or is not loaded)" << endl;
    return false;
  }

  bool result;
  bool deletePluginProgress=false;
  ClusterContext tmp;
  tmp.graph=sg;
  tmp.dataSet=&dataSet;
  PluginProgress *tmpProgress=NULL;
  if (plugProgress==0) {
    tmpProgress=new PluginProgress();
    deletePluginProgress=true;
  }
  else tmpProgress=plugProgress;
  tmp.pluginProgress=tmpProgress;
  ExportModule *newExportModule=ExportModuleFactory::factory->getObject(alg, tmp);
  assert(newExportModule!=0);
  result=newExportModule->exportGraph(os,sg);

  if (deletePluginProgress) delete tmpProgress;
  delete newExportModule;
  return result;
}
//=========================================================
bool tlp::clusterizeGraph(Graph *sg,string &errorMsg,DataSet *dataSet,
			  const string &alg, PluginProgress *plugProgress) {
  if (!ClusteringFactory::factory->exists(alg)) {
    cerr << "libtulip: " << __FUNCTION__ << ": cluster plugin \"" << alg
         << "\" doesn't exists (or is not loaded)" << endl;
    return false;
  }

  bool result;
  bool deletePluginProgress=false;
  ClusterContext tmp;
  tmp.graph=sg;
  tmp.dataSet=dataSet;
  PluginProgress *tmpProgress;
  if (plugProgress==0) {
    tmpProgress=new PluginProgress();
    deletePluginProgress=true;
  }
  else tmpProgress=plugProgress;
  tmp.pluginProgress=tmpProgress; 
  Clustering *newClustering=ClusteringFactory::factory->getObject(alg, tmp);
  if ((result=newClustering->check(errorMsg)))
    newClustering->run();
  delete newClustering;
  if (deletePluginProgress) delete tmpProgress;
  return result;
}

// initialize and export the factories needed to manage
// our different kinds of plugins
#if !defined( __APPLE__)
template <class Tnode, class Tedge, class TPROPERTY>
  TemplateFactory<PropertyFactory<TPROPERTY >, TPROPERTY, PropertyContext > *AbstractProperty<Tnode,Tedge,TPROPERTY>::factory = 0;
 #else
TemplateFactory<PropertyFactory<ColorAlgorithm>, ColorAlgorithm, PropertyContext> *AbstractProperty<ColorType, ColorType, ColorAlgorithm>::factory = 0;
TemplateFactory<PropertyFactory<IntegerAlgorithm>, IntegerAlgorithm, PropertyContext> *AbstractProperty<IntegerType, IntegerType, IntegerAlgorithm>::factory = 0;
TemplateFactory<PropertyFactory<LayoutAlgorithm>, LayoutAlgorithm, PropertyContext> *AbstractProperty<PointType, LineType, LayoutAlgorithm>::factory = 0;
TemplateFactory<PropertyFactory<DoubleAlgorithm>, DoubleAlgorithm, PropertyContext> *AbstractProperty<DoubleType, DoubleType, DoubleAlgorithm>::factory = 0;
TemplateFactory<PropertyFactory<BooleanAlgorithm>, BooleanAlgorithm, PropertyContext> *AbstractProperty<BooleanType, BooleanType, BooleanAlgorithm>::factory = 0;
TemplateFactory<PropertyFactory<SizeAlgorithm>, SizeAlgorithm, PropertyContext> *AbstractProperty<SizeType,SizeType, SizeAlgorithm>::factory = 0;
TemplateFactory<PropertyFactory<StringAlgorithm>, StringAlgorithm, PropertyContext> *AbstractProperty<StringType, StringType, StringAlgorithm>::factory = 0;
#endif
//=========================================================
void loadPlugins(string dir,PluginLoader *plug) {
  SizeProperty::initFactory();
  SizeProperty::factory->load(dir + "sizes", "Size",plug);
  IntegerProperty::initFactory();
  IntegerProperty::factory->load(dir + "int", "Integer",plug);
  LayoutProperty::initFactory();
  LayoutProperty::factory->load(dir + "layout" , "Layout",plug);
  ColorProperty::initFactory();
  ColorProperty::factory->load(dir + "colors" , "Color",plug);
  DoubleProperty::initFactory();
  DoubleProperty::factory->load(dir + "metric" , "Double",plug);
  StringProperty::initFactory();
  StringProperty::factory->load(dir + "string" , "String",plug);
  BooleanProperty::initFactory();
  BooleanProperty::factory->load(dir + "selection" , "Boolean",plug);
  ClusteringFactory::initFactory();
  ClusteringFactory::factory->load(dir + "clustering" , "Cluster",plug);
  ImportModuleFactory::initFactory();
  ImportModuleFactory::factory->load(dir + "import" , "Import Module",plug);
  ExportModuleFactory::initFactory();
  ExportModuleFactory::factory->load(dir + "export" , "Export Module",plug);
}
//=========================================================
void tlp::loadPlugins(PluginLoader *plug) {
  string::const_iterator begin=TulipPluginsPath.begin();
  string::const_iterator end=begin;
  while (end!=TulipPluginsPath.end())
    if ((*end)==PATH_DELIMITER) {
      if (begin!=end) 
	loadPlugins(string(begin,end)+'/',plug);
      ++end;
      begin=end;
    } else
      ++end;
  if (begin!=end) 
    loadPlugins(string(begin,end)+'/',plug);
}
//=========================================================
bool tlp::loadPlugin(const std::string & filename, PluginLoader *plug) {
    PluginIterator::loadPlugin(filename, plug);
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

void tlp::copyToGraph (	Graph *outG, Graph *	inG, BooleanProperty *inSel, BooleanProperty* outSel) {
  if( outSel ) {
    outSel->setAllNodeValue( false );
    outSel->setAllEdgeValue( false );
  }

  if( !outG || !inG )
    return;

  // extend the selection to edge ends
  if( inSel ) {
    Iterator<edge> * edgeIt = inG->getEdges();
    while( edgeIt->hasNext() ) {
      edge e = edgeIt->next();
      if( inSel->getEdgeValue(e) ) {
	node n0 = inG->source( e );
	node n1 = inG->target( e );
	inSel->setNodeValue( n0, true );
	inSel->setNodeValue( n1, true );
      }
    }
    delete edgeIt;
  }
	
  // copy selected nodes
  MutableContainer<node> nodeTrl;
  {
    Iterator<node> * nodeIt = inG->getNodes();
    while( nodeIt->hasNext() ) {
      node n0 = nodeIt->next();
      if( !inSel || inSel->getNodeValue(n0) ) {
	node n1 = outG->addNode();
	      
	// select appended node
	if( outSel )
	  outSel->setNodeValue( n1, true );
	      
	// add to translation tab
	nodeTrl.set( n0.id, n1 );
	      
	// copy node properties
	Iterator<std::string> * propIt = inG->getProperties();
	while( propIt->hasNext() ) {
	  std::string n = propIt->next();
	  PropertyInterface * src = inG->getProperty( n );
	  if(dynamic_cast<GraphProperty *>(src) == 0) {
	    PropertyInterface *dst = outG->existProperty(n) ? outG->getProperty(n) : src->clonePrototype(outG,n);
	    dst->copy( n1, n0, src );
	  }
	}
	delete propIt;
      }
    }
    delete nodeIt;
  }
	
  // copy selected edges
  {
    Iterator<edge> * edgeIt = inG->getEdges();
    while( edgeIt->hasNext() ) {
      edge e0    = edgeIt->next();
      if( !inSel || inSel->getEdgeValue(e0) ) {
	node e0_n0 = inG->source(e0);
	node e0_n1 = inG->target(e0);
	edge e1 = outG->addEdge( nodeTrl.get(e0_n0.id),
				 nodeTrl.get(e0_n1.id) );
	      
	// select appended edge
	if( outSel )
	  outSel->setEdgeValue( e1, true );
	      
	// copy edge properties
	Iterator<std::string> * propIt = inG->getProperties();
	while( propIt->hasNext() ) {
	  std::string n = propIt->next();
	  PropertyInterface * src = inG->getProperty( n ),
	    * dst = outG->existProperty(n) ? outG->getProperty(n) : src->clonePrototype(outG,n);
	  dst->copy( e1, e0, src );
	}
	delete propIt;
      }
    }
    delete edgeIt;
  }
}
