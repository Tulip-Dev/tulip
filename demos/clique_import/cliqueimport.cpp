#include <string>
#include <tulip/TulipPluginHeaders.h>

using namespace tlp;
using namespace std;

class CliqueImport: public tlp::ImportModule {
public:
  PLUGININFORMATION("Clique", "Tulip Team", "05/10/2012", "Clique Import Plugin", "1.0", "Graph")
  CliqueImport(tlp::PluginContext* context);
  ~CliqueImport();
  bool importGraph();
};

PLUGIN(CliqueImport)

//a description of the parameter, that will appear in the GUI to explain how the parameter influences the plugin.
const char * nodeCountDescription =
  // number of nodes (equivalent to degree)
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "unsigned int" ) \
  HTML_HELP_BODY() \
  "How many nodes the clique will contain" \
  HTML_HELP_CLOSE();

CliqueImport::CliqueImport(tlp::PluginContext* context):ImportModule(context) {
  //how many nodes we want in our clique
  addInParameter<unsigned int>("nodeCount", nodeCountDescription, "5");

  //depend on the circular layout, as it is the most obvious choice to draw a clique
  addDependency("Circular", "1.1");
}

CliqueImport::~CliqueImport() {
}

bool CliqueImport::importGraph() {
  //retrieve the number of nodes we need to create
  unsigned int nodeCount = 5;
  dataSet->get("nodeCount", nodeCount);

  //create the nodes
  for(unsigned int i = 0; i < nodeCount; ++i) {
    graph->addNode();
  }

  /*
   * This could be done more efficiently by using a vector of nodes and iterating over the indices,
   * but we don't really care about performances in this case, do we ?
   */

  //double iteration over the graph's nodes.
  node current;
  node other;
  forEach(current, graph->getNodes()) {
    forEach(other, graph->getNodes()) {
      if(current != other) {
        graph->addEdge(current, other);
      }
    }
  }

  //apply the circular algorithm on the clique to make it look good (at least better than 'Random')
  LayoutProperty* layout = graph->getProperty<LayoutProperty>("viewLayout");
  string message;
  graph->applyPropertyAlgorithm("Circular", layout, message);

  //no way in hell this can fail.
  return true;
}
