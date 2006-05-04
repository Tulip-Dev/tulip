#include <tulip/TulipPlugin.h>

using namespace std;



namespace
{

	const char * paramHelp[] =
	{
		// nodes
		HTML_HELP_OPEN() \
		HTML_HELP_DEF( "type", "int" ) \
		HTML_HELP_DEF( "default", "5" ) \
		HTML_HELP_BODY() \
		"This parameter defines the amount of nodes composing the completed graph." \
		HTML_HELP_CLOSE(),
	};

}



struct CompleteGraph:public ImportModule {

  CompleteGraph(ClusterContext context):ImportModule(context) {
    addParameter<int>("nodes",paramHelp[0],"5");
  }
  ~CompleteGraph(){
  }
  
  bool import(const string &name) {
    int nbNodes  = 5;
    if (dataSet!=0) {
      dataSet->get("nodes", nbNodes);
    }
    
    for (int j=0; j<nbNodes; ++j) 
      graph->addNode();
    Iterator<node> *it1,*it2;
    it1=graph->getNodes();
    int i=0;
    while (it1->hasNext()) {
      if (pluginProgress->progress(i,nbNodes) != TLP_CONTINUE) 
	return false;
      i++;
      node n1=it1->next();
      it2=graph->getNodes();
      while (it2->hasNext()) {
	node n2=it2->next();
	if (n1!=n2) graph->addEdge(n1,n2);
      } delete it2;
    } delete it1;
    return true;
  }
};

IMPORTPLUGIN(CompleteGraph,"Complete General Graph","Auber","16/12/2002","0","0","1")
