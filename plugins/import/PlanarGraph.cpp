#include <time.h>
#include <math.h>
#include <climits>
#include <tulip/TulipPlugin.h>
#include <tulip/Delaunay.h>


using namespace std;
using namespace stdext;
static const int WIDTH = 1024;
static const int HEIGHT = 1024;

namespace {
  const char * paramHelp[] = {
    // nodes
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "int" ) \
    HTML_HELP_DEF( "default", "100" ) \
    HTML_HELP_BODY() \
    "This parameter defines the amount of node used to build the planr graph graph." \
    HTML_HELP_CLOSE(),

    // degree
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "int" ) \
    HTML_HELP_DEF( "default", "10" ) \
    HTML_HELP_BODY() \
    "This parameter defines the number of edges used to build the planar graph." \
    HTML_HELP_CLOSE(),

    // degree
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "bool" ) \
    HTML_HELP_DEF( "default", "false" ) \
    HTML_HELP_BODY() \
    "If true the graph will be connected." \
    HTML_HELP_CLOSE(),
  };
}

//=============================================================
struct PlanarGraph:public ImportModule {
  PlanarGraph(ClusterContext context):ImportModule(context) {
    addParameter<int>("nodes",paramHelp[0],"200");
    addParameter<int>("edges",paramHelp[1],"10");
    addParameter<bool>("connected",paramHelp[2],"false");
  }
  ~PlanarGraph(){}
  
  bool import(const string &name) {
    int nbNodes  = 200;
    int nbEdges  = 10;
    bool enableLongEdge = false;
    if (dataSet!=0) {
      dataSet->get("nodes", nbNodes);
      dataSet->get("edges", nbEdges);
      dataSet->get("connected", enableLongEdge);
    }
    
    srand(clock()); 
    LayoutProxy *newLayout = superGraph->getLocalProperty<LayoutProxy>("viewLayout");
    SizesProxy  *newSize   = superGraph->getLocalProperty<SizesProxy>("viewSize");
    newSize->setAllNodeValue(Size(1.0,1.0,1.0));

    vector<node> graph(nbNodes);
    hash_map < unsigned int, hash_map<unsigned int, bool> > matrix;
    
    vector<Coord> coords(nbNodes);
    for (int i=0; i<nbNodes; ++i) {
      unsigned int x;
      unsigned int y;
      do {
	x = rand()%nbNodes;
	y = rand()%nbNodes;
      } while ( !(matrix.find(x) == matrix.end() || matrix[x].find(y)==matrix[x].end()) ); 
      matrix[x][y]=true;
      graph[i] = superGraph->addNode();
      coords[i] = Coord(x + 1, y + 1, 0);
      cerr << coords[i] << endl;
      newLayout->setNodeValue(graph[i], coords[i]);
    }
    vector<pair<unsigned int, unsigned int> > edges;
    cerr << "*****************" << endl;
    tlp::delaunayTriangulation(coords, edges);
    cerr << "*****************" << endl;
    vector<pair<unsigned int, unsigned int> >::const_iterator it;
    for (it = edges.begin(); it != edges.end(); ++it) {
      superGraph->addEdge(graph[it->first], graph[it->second]);
    }
    return  pluginProgress->state()!=TLP_CANCEL;
  }
};

IMPORTPLUGIN(PlanarGraph,"Planar Graph","Auber","25/06/2005","0","0","1")
