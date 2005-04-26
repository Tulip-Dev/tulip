#include <time.h>
#include <math.h>
#include <tulip/TulipPlugin.h>

using namespace std;
static const int WIDTH = 1024;
static const int HEIGHT = 1024;

namespace {
  const char * paramHelp[] = {
    // nodes
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "int" ) \
    HTML_HELP_DEF( "default", "100" ) \
    HTML_HELP_BODY() \
    "This parameter defines the amount of node used to build the small-world graph." \
    HTML_HELP_CLOSE(),

    // degree
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "int" ) \
    HTML_HELP_DEF( "default", "10" ) \
    HTML_HELP_BODY() \
    "This parameter defines the average degree of node used to build the small-world graph." \
    HTML_HELP_CLOSE(),
  };
}

/* 
 * TODO :
 * Use a quadtree in order to change the complexity from n*n to n*log(n)
 */

struct SmallWorldGraph:public ImportModule {
  SmallWorldGraph(ClusterContext context):ImportModule(context) {
    addParameter<int>("nodes",paramHelp[0],"100");
    addParameter<int>("degree",paramHelp[1],"10");
  }
  ~SmallWorldGraph(){}
  
  bool import(const string &name) {
    int nbNodes  = 100;
    int avgDegree = 10;
    if (dataSet!=0) {
      dataSet->get("nodes", nbNodes);
      dataSet->get("degree", avgDegree);
    }
    double maxDistance = sqrt(double(avgDegree)*double(WIDTH)*double(HEIGHT)
			      / (double (nbNodes) * M_PI));
    srand(clock()); 
    LayoutProxy *newLayout=superGraph->getLocalProperty<LayoutProxy>("viewLayout");
    vector<node> graph(nbNodes);
    for (int i=0; i<nbNodes;++i) {
      graph[i]=superGraph->addNode();
      newLayout->setNodeValue(graph[i],Coord(rand()%WIDTH,rand()%HEIGHT,0));
    }
    for (int i=0;i<nbNodes-1;++i) 
      for (int j=i+1;j<nbNodes;++j) {
	if (i!=j) {
	  if (newLayout->getNodeValue(graph[i]).dist(newLayout->getNodeValue(graph[j]))<(double)maxDistance)
	  superGraph->addEdge(graph[i],graph[j]);
	}
      }
    return true;
  }
};

IMPORTPLUGIN(SmallWorldGraph,"Grid approximation","Auber","25/06/2002","0","0","1")
