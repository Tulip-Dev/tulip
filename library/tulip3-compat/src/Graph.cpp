#include <tulip3/Graph.h>
#include <tulip/Graph.h>

bool getSource(tlp::Graph *g, tlp::node &n) {
  n = g->getSource();
  return n.isValid();
}
