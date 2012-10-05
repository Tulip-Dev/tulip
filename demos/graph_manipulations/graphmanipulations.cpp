#include <tulip/Graph.h>
#include <tulip/Interactor.h>
#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>
#include <tulip/PluginLoaderTxt.h>
#include <tulip/ColorProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/DoubleProperty.h>
#include <QtCore/QCoreApplication>

using namespace std;
using namespace tlp;

int main(int argc,char ** argv ){
  //instanciate the QApplication
  QCoreApplication app(argc, argv);
  
  /*
   * Let's create the following graph
   *
   *      A
   *    /   \
   *  B       C
   *   \     /
   *    D - E
   */

  //initialize Tulip (load plugins, etc)
  tlp::initTulipSoftware();

  //create a new graph
  Graph* myGraph = tlp::newGraph();

  node a = myGraph->addNode();
  node b = myGraph->addNode();
  node c = myGraph->addNode();
  node d = myGraph->addNode();
  node e = myGraph->addNode();

  myGraph->addEdge(a, b);
  myGraph->addEdge(a, c);
  myGraph->addEdge(b, d);
  myGraph->addEdge(c, e);
  myGraph->addEdge(d, e);


  //now in color. 'viewColor' is the Tulip GUI's default color property, so when we load it we will see the color immediately
  ColorProperty* color = myGraph->getProperty<ColorProperty>("viewColor");
  color->setNodeValue(a, Color(255, 0, 0));
  color->setNodeValue(b, Color(0, 255, 0));
  color->setNodeValue(c, Color(0, 0, 255));
  color->setNodeValue(d, Color(255, 0, 0));
  color->setNodeValue(e, Color(0, 255, 0));
  //hey look, this is a 3-coloration :)

  //set the label of the nodes (again, with Tulip's default label property)
  StringProperty* label = myGraph->getProperty<StringProperty>("viewLabel");
  label->setNodeValue(a, "A");
  label->setNodeValue(b, "B");
  label->setNodeValue(c, "C");
  label->setNodeValue(d, "D");
  label->setNodeValue(e, "E");

  DoubleProperty* metric = myGraph->getProperty<DoubleProperty>("degree");
  //if the degree plugin is available, let's call it.
  if(tlp::PluginLister::instance()->pluginExists("Degree")) {
    //now compute the degree of the nodes.
    string errorMessage;
    //this calls the Tulip plugin 'Degree'.
    bool success = myGraph->applyPropertyAlgorithm("Degree", metric, errorMessage);
    if(!success) {
      std::cout << errorMessage << std::endl;
    }
  }
  else {
    std::cout << "could not find the plugin, computing" << std::endl;
    node n;
    forEach(n, myGraph->getNodes()) {
      metric->setNodeValue(n, myGraph->deg(n));
    }
  }

  //output the degree of node a;
  std::cout << metric->getNodeValue(a) <<std::endl;

  //saveGraph is a shortcut ofr exportGraph that uses the TLP export.
  tlp::saveGraph(myGraph, "mygraph.tlp");
  
  return EXIT_SUCCESS;
}

