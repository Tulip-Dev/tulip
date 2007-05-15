#include <time.h>
#include <tulip/TulipPlugin.h>
#include <tulip/StableIterator.h>

using namespace std;
using namespace tlp;



namespace {

  const char * paramHelp[] =
    {
      // minsize
      HTML_HELP_OPEN() \
      HTML_HELP_DEF( "type", "int" ) \
      HTML_HELP_DEF( "default", "100" ) \
      HTML_HELP_BODY() \
      "This parameter defines the minimal amount of node used to build the randomized tree." \
      HTML_HELP_CLOSE(),

      // maxsize
      HTML_HELP_OPEN() \
      HTML_HELP_DEF( "type", "int" ) \
      HTML_HELP_DEF( "default", "1000" ) \
      HTML_HELP_BODY() \
      "This parameter defines the maximal amount of node used to build the randomized tree." \
      HTML_HELP_CLOSE(),
    };
}

struct RandomTree:public ImportModule {
  RandomTree(AlgorithmContext context):ImportModule(context) {
    addParameter<int>("minsize",paramHelp[0],"100");
    addParameter<int>("maxsize",paramHelp[1],"1000");
  }
  ~RandomTree() {
  }

  bool buildNode(node n, unsigned int sizeM) {
    if (graph->numberOfNodes()>sizeM+2) return false;
    bool result=true;
    int randNumber=rand();
    if (randNumber>RAND_MAX/2) {
      node n1,n2;
      n1=graph->addNode();
      n2=graph->addNode();
      graph->addEdge(n,n1);
      graph->addEdge(n,n2);
      result= result && buildNode(n1,sizeM);
      result= result && buildNode(n2,sizeM);
    }
    return result;
  }

  bool import(const string &name) {
    srand(clock()); 
    unsigned int minSize  = 100;
    unsigned int maxSize  = 1000;
    if (dataSet!=0) {
      dataSet->get("minsize", minSize);
      dataSet->get("maxsize", maxSize);
    }

    if (pluginProgress)
      pluginProgress->showPreview(false);

    bool ok=true;
    int i=0;
    while (ok) {
      if (pluginProgress->progress(i%100,100)!=TLP_CONTINUE) break;
      i++;
      graph->clear();
      node n=graph->addNode();
      ok = !buildNode(n,maxSize);
      if (graph->numberOfNodes()<minSize-2) ok=true;
    }
    return pluginProgress->progress(100,100)!=TLP_CANCEL;
  }
};
IMPORTPLUGINOFGROUP(RandomTree,"Uniform Random Binary Tree","Auber","16/02/2001","","1.0","Graphs")
