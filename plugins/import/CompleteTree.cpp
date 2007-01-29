#include <tulip/TulipPlugin.h>

using namespace std;
using namespace tlp;



namespace
{

	const char * paramHelp[] =
	{
		// depth
		HTML_HELP_OPEN() \
		HTML_HELP_DEF( "type", "int" ) \
		HTML_HELP_DEF( "default", "5" ) \
		HTML_HELP_BODY() \
		"This parameter defines the tree's depth to build." \
		HTML_HELP_CLOSE(),

		// degree
		HTML_HELP_OPEN() \
		HTML_HELP_DEF( "type", "int" ) \
		HTML_HELP_DEF( "default", "2" ) \
		HTML_HELP_BODY() \
		"This parameter defines the tree's degree to build." \
		HTML_HELP_CLOSE(),
	};

}



struct CompleteTree:public ImportModule {
  CompleteTree(AlgorithmContext context):ImportModule(context) {
    addParameter<int>("depth",paramHelp[0],"5");
    addParameter<int>("degree",paramHelp[1],"2");
  }
  ~CompleteTree(){
  }
  void buildNode(node n,unsigned int degree,int depth) {
    if (depth<1) return;
    for (unsigned int i=0;i<degree;++i) {
      node n1=graph->addNode();
      graph->addEdge(n,n1);
      buildNode(n1,degree,depth - 1);
    }
  }
  bool import(const string &name) {
    int degree  = 2;
    int depth   = 5;
    if (dataSet!=0) {
      dataSet->get("depth", depth);
      dataSet->get("degree", degree);
    }
    node n=graph->addNode();
    buildNode(n,degree,depth);
    return true;
  }
};
IMPORTPLUGINOFGROUP(CompleteTree,"Complete Tree","Auber","08/09/2002","0","0","1","Graphs")
