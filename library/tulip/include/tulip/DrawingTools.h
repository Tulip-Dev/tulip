//-*-c++-*-
#ifndef _DRAWINGTOOLS_H
#define _DRAWINGTOOLS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/Coord.h>

class SuperGraph;
class Layout;
class Sizes;
class Metric;
class Selection;

namespace tlp {
  TLP_SCOPE   std::pair<Coord, Coord> computeBoundingBox(SuperGraph *graph, Layout *layout, Sizes *size, Metric *rotation, Selection *selection = 0);
};


#endif
