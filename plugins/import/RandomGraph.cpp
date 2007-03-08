#include <time.h>
#include <tulip/TulipPlugin.h>

using namespace std;
using namespace tlp;

struct edgeS {
  unsigned  source,target;
};

namespace std {
  template<>
  struct less<edgeS> {
    bool operator()(const edgeS &c,const edgeS &d) const {
      if (c.source<d.source) return true;
      if (c.source>d.source) return false;
      if (c.target<d.target) return true;
      if (c.target>d.target) return false;
      return false;
    }
  };
};


namespace {
  const char * paramHelp[] = {
    // nodes
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "int" ) \
    HTML_HELP_DEF( "default", "5" ) \
    HTML_HELP_BODY() \
    "This parameter defines the amount of node used to build the randomized graph." \
    HTML_HELP_CLOSE(),

    // edges
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "int" ) \
    HTML_HELP_DEF( "default", "9" ) \
    HTML_HELP_BODY() \
    "This parameter defines the amount of edge used to build the randomized graph." \
    HTML_HELP_CLOSE(),
  };
}


struct RandomGraph:public ImportModule {
  RandomGraph(AlgorithmContext context):ImportModule(context) {
    addParameter<int>("nodes",paramHelp[0],"5");
    addParameter<int>("edges",paramHelp[1],"9");
  }
  ~RandomGraph(){
  }
  
  bool import(const string &name) {
    srand(clock());
    unsigned int nbNodes  = 5;
    unsigned int nbEdges  = 9;
    if (dataSet!=0) {
      dataSet->get("nodes", nbNodes);
      dataSet->get("edges", nbEdges);
    }
    int ite = nbNodes*nbEdges;
    int nbIteration = ite;
    set<edgeS> myGraph;
    while (ite>0) {
      if (ite%nbNodes==1) if (pluginProgress->progress(nbIteration-ite,nbIteration)!=TLP_CONTINUE) 
	return pluginProgress->state()!=TLP_CANCEL;
      edgeS tmp;
      tmp.source=rand()%nbNodes;
      tmp.target=rand()%nbNodes;
      if (myGraph.find(tmp)!=myGraph.end())
	myGraph.erase(tmp);
      else 
	if (myGraph.size()<nbEdges) myGraph.insert(tmp);
      ite--;
    }
    
    vector<node> tmpVect(nbNodes);
    for (unsigned int i=0; i<nbNodes; ++i) {
      tmpVect[i]=graph->addNode();
    }
    
    set<edgeS>::const_iterator it;
    for (it=myGraph.begin(); it!=myGraph.end(); ++it)   {
      graph->addEdge(tmpVect[(*it).source],tmpVect[(*it).target]);
    }
    return true;
  }
};
IMPORTPLUGINOFGROUP(RandomGraph,"Random General Graph","Auber","16/06/2002","","1.0","Graphs")
