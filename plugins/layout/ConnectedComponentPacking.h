#ifndef CONNECTEDCOMPONNENTPACKING_H
#define CONNECTEDCOMPONNENTPACKING_H

#include <vector>
#include <tulip/TulipPlugin.h>

class ConnectedComponentPacking: public tlp::LayoutAlgorithm {
public:
    ConnectedComponentPacking(const tlp::PropertyContext&);
    bool run();

private: 
    tlp::Graph *workingGraph;
};

#endif



