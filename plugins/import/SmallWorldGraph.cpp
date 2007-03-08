#include <time.h>
#include <math.h>
#include <climits>
#include <tulip/TulipPlugin.h>

using namespace std;
using namespace tlp;
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

    // degree
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "bool" ) \
    HTML_HELP_DEF( "default", "false" ) \
    HTML_HELP_BODY() \
    "If true long distance edges will be added in the grid approximation." \
    HTML_HELP_CLOSE(),
  };
}

/* 
 * TODO :
 * Use a quadtree in order to change the complexity from n*n to n*log(n)
 */

struct SmallWorldGraph:public ImportModule {
  SmallWorldGraph(AlgorithmContext context):ImportModule(context) {
    addParameter<int>("nodes",paramHelp[0],"200");
    addParameter<int>("degree",paramHelp[1],"10");
    addParameter<bool>("long edge",paramHelp[2],"false");
  }
  ~SmallWorldGraph(){}
  
  bool import(const string &name) {
    int nbNodes  = 200;
    int avgDegree = 10;
    bool enableLongEdge = false;
    if (dataSet!=0) {
      dataSet->get("nodes", nbNodes);
      dataSet->get("degree", avgDegree);
      dataSet->get("long edge", enableLongEdge);
    }
    double maxDistance = sqrt(double(avgDegree)*double(WIDTH)*double(HEIGHT)
			      / (double (nbNodes) * M_PI));
    srand(clock()); 
    LayoutProperty *newLayout=graph->getLocalProperty<LayoutProperty>("viewLayout");
    SizeProperty *newSize=graph->getLocalProperty<SizeProperty>("viewSize");

    vector<node> sg(nbNodes);
    for (int i=0; i<nbNodes;++i) {
      sg[i]=graph->addNode();
      newLayout->setNodeValue(sg[i],Coord(rand()%WIDTH, rand()%HEIGHT, 0));
    }
    unsigned int count = 0;
    unsigned int iterations = nbNodes*(nbNodes-1)/2;
    double minSize = DBL_MAX;
    for (int i=0;i<nbNodes-1;++i) {
      bool longEdge =false;
      if (pluginProgress->progress(count,iterations)!=TLP_CONTINUE) break;
      for (int j=i+1;j<nbNodes;++j) {
	++count;
	if (i!=j) {
	  double distance = newLayout->getNodeValue(sg[i]).dist(newLayout->getNodeValue(sg[j]));
	  minSize = std::min(distance, minSize);
	  newSize->setAllNodeValue(Size(minSize/2.0, minSize/2.0, 1));
	  if ( distance  < (double)maxDistance)
	    graph->addEdge(sg[i],sg[j]);
	  else 
	    if (!longEdge && enableLongEdge) {
	      double distrand = (double)rand()/(double)RAND_MAX;
	      if (distrand < 1.0/(2.0+double(nbNodes-i-1))) {
		longEdge = true;
		graph->addEdge(sg[i],sg[j]);
	      }
	    }
	}
      }
    }
    return  pluginProgress->state()!=TLP_CANCEL;
  }
};

IMPORTPLUGINOFGROUP(SmallWorldGraph,"Grid approximation","Auber","25/06/2002","","1.0","Graphs")
