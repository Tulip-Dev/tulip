#include <tulip3/Graph.h>
#include <tulip/Graph.h>

bool getSource(tlp::Graph *g, tlp::node &n) {
  n = g->getSource();
  return n.isValid();
}

bool applyAlgorithm(tlp::Graph *graph, std::string &errorMessage, tlp::DataSet *dataSet,const std::string &algorithm, tlp::PluginProgress *progress) {
  return graph->applyAlgorithm(errorMessage, dataSet, algorithm, progress);
}
