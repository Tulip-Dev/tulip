#ifndef _KRUSKAL_H
#define _KRUSKAL_H

#include <tulip/TulipPlugin.h>
#include <tulip/MetricProxy.h>
#include <tulip/MethodFactory.h>
#include <tulip/SelectionProxy.h>
#include <map>
#include <iostream>

class Kruskal:public Selection { 
  
public:
  Kruskal(const PropertyContext &);
  ~Kruskal();
  bool run();

private:

  struct ltEdge {
    MetricProxy *m;
    ltEdge(MetricProxy *m) : m(m) {}
    bool operator()(const edge &e1, const edge &e2) const
    {
      return (m->getEdgeValue(e1) < m->getEdgeValue(e2));
    } 
  };

  std::map<int,int> *classes;
  int numClasses;
  int getClass(const int i);
  int makeUnion(const int p, const int q);
  bool edgeOk(const edge &e);
};

#endif
