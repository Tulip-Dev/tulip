#include <time.h>
#include <math.h>
#include <climits>
#include <tulip/TulipPlugin.h>
#include <tulip/Delaunay.h>


using namespace std;
using namespace stdext;

namespace {
  const char * paramHelp[] = {
    // nodes
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "int" ) \
    HTML_HELP_DEF( "default", "30" ) \
    HTML_HELP_BODY() \
    "This parameter defines the number of nodes used to build the planr graph graph." \
    HTML_HELP_CLOSE(),
  };
}

//=============================================================
struct PlanarGraph:public ImportModule {
  PlanarGraph(ClusterContext context):ImportModule(context) {
    addParameter<int>("nodes", paramHelp[0], "30");
  }
  ~PlanarGraph(){}
  
  bool import(const string &name) {
    int nbNodes  = 20;
    if (dataSet!=0) {
      dataSet->get("nodes", nbNodes);
    }
    if (nbNodes < 3) nbNodes = 3;
    srand(clock()); 
    LayoutProperty *newLayout = graph->getLocalProperty<LayoutProperty>("viewLayout");
    SizeProperty  *newSize   = graph->getLocalProperty<SizeProperty>("viewSize");
    newSize->setAllNodeValue(Size(1.0,1.0,1.0));

    vector<node> sg(nbNodes);
    hash_map < unsigned int, hash_map<unsigned int, bool> > matrix;
    
    vector<Coord> coords(nbNodes);
    for (int i=0; i<nbNodes; ++i) {
      unsigned int x;
      unsigned int y;
      bool ok = false;
      do {
	x = rand()%nbNodes;
	y = rand()%nbNodes;
	if (matrix.find(x) == matrix.end())
	  ok = true;
	else 
	  if (matrix[x].find(y)==matrix[x].end())
	    ok = true;
	
      } while ( !ok);
      matrix[x][y]=true;
      sg[i] = graph->addNode();
      coords[i] = Coord(x + 1, y + 1, 0);
      newLayout->setNodeValue(sg[i], coords[i]);
    }
    vector<pair<unsigned int, unsigned int> > edges;
    tlp::delaunayTriangulation(coords, edges);
    vector<pair<unsigned int, unsigned int> >::const_iterator it;
    for (it = edges.begin(); it != edges.end(); ++it) {
      graph->addEdge(sg[it->first], sg[it->second]);
    }
    return  pluginProgress->state()!=TLP_CANCEL;
  }
};

IMPORTPLUGIN(PlanarGraph,"Planar Graph","Auber","25/06/2005","0","0","1")
