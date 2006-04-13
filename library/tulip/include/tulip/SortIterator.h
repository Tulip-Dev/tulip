//-*-c++-*-
/**
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 10/12/2002
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef TULIP_SORTITERATOR_H
#define TULIP_SORTITERATOR_H
#include <vector>
#include <tulip/Iterator.h>
#include <tulip/StableIterator.h>
#include <tulip/Metric.h>
#include <tulip/SuperGraph.h>

struct LessThan {
  Metric* metric;
  bool operator() (node n1,node n2) {
    return (metric->getNodeValue(n1) < metric->getNodeValue(n2));
  } 
};

struct LessThanEdgeTargetMetric {
  LessThanEdgeTargetMetric(SuperGraph *graph, Metric* metric):
    graph(graph),
    metric(metric) {
  }
  bool operator() (edge e1,edge e2){
    return (metric->getNodeValue(graph->target(e1)) < metric->getNodeValue(graph->target(e2)));
  }
private:
  Metric* metric;
  SuperGraph *graph;
};


///Interface of Sortiterator,
struct SortNodeIterator : public StableIterator<node> {
  ///
  SortNodeIterator(Iterator<node> *itIn, Metric* metric):StableIterator<node>(itIn) {
    LessThan tmp;
    tmp.metric=metric;
    sort(cloneIt.begin(),cloneIt.end(),tmp);
    itStl=cloneIt.begin();
  }
  ///
  ~SortNodeIterator(){};
};

///Interface of Sortiterator,
struct SortTargetEdgeIterator : public StableIterator<edge> {
  ///
  SortTargetEdgeIterator(Iterator<edge> *itIn, SuperGraph* graph, Metric* metric):StableIterator<edge>(itIn) {
    LessThanEdgeTargetMetric tmp(graph,metric);
    sort(cloneIt.begin(),cloneIt.end(),tmp);
    itStl=cloneIt.begin();
  }
  ///
  ~SortTargetEdgeIterator(){};
};


#endif
