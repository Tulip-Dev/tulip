#include <time.h>
#include <math.h>
#include <climits>
#include <tulip/TulipPlugin.h>


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

struct Triangle {
  Triangle(node a, node b, node c):
    a(a),b(b),c(c) {
  }
  node a,b,c;
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
    LayoutProxy *newLayout = superGraph->getLocalProperty<LayoutProxy>("viewLayout");
    SizesProxy  *newSize   = superGraph->getLocalProperty<SizesProxy>("viewSize");
    newSize->setAllNodeValue(Size(1.0,1.0,1.0));

    newSize->setAllNodeValue(Size(1.0,1.0,1.0));    

    vector<Triangle> faces;
    Triangle f(superGraph->addNode(),
	       superGraph->addNode(),
	       superGraph->addNode());
    faces.push_back(f);
    superGraph->addEdge(f.a, f.b);
    superGraph->addEdge(f.b, f.c);
    superGraph->addEdge(f.c, f.a);
    newLayout->setNodeValue(f.a, Coord(-nbNodes,-nbNodes,0));
    newLayout->setNodeValue(f.b, Coord(0,nbNodes,0));
    newLayout->setNodeValue(f.c, Coord(+nbNodes,-nbNodes,0));
    unsigned int nb = 3;
    unsigned int ext_faces = 0;
    while(nb<nbNodes) {
      //choose a Triangle randomly
      unsigned int i = rand()%faces.size(); 
      Triangle f = faces[i];
      node n = superGraph->addNode();
      Coord tmp = newLayout->getNodeValue(f.a) +
	newLayout->getNodeValue(f.b) +
	newLayout->getNodeValue(f.c);
      tmp /= 3.0;
      newLayout->setNodeValue(n, tmp);
	
      //Split the triangle in three part
      superGraph->addEdge(n, f.a);
      superGraph->addEdge(n, f.b);
      superGraph->addEdge(n, f.c);
      //add the three new Triangle, remove the old one(replace)
      Triangle f1(f.a, f.b, n);
      Triangle f2(f.b, f.c, n);
      Triangle f3(f.c, f.a, n);
      faces[i] = f1;
      faces.push_back(f2);
      faces.push_back(f3);
      ++nb;
    }

    return  pluginProgress->state()!=TLP_CANCEL;
  }
};

IMPORTPLUGINOFGROUP(PlanarGraph,"Planar Graph","Auber","25/06/2005","0","0","1","Graphs")
