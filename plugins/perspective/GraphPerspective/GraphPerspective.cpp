#include "GraphPerspective.h"

using namespace tlp;

GraphPerspective::GraphPerspective(PerspectiveContext &c): Perspective(c) {
}

void GraphPerspective::construct() {

}

PERSPECTIVEPLUGIN(GraphPerspective,"Graph hierarchy analysis", "Ludwig Fiolka", "2011/07/11", "Analyze several graphs/subgraphs hierarchies", "1.0")
