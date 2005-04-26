/**
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#include <assert.h>
#include "MultipleEdgeSelection.h"
#include <tulip/SelectionProxy.h>
#include <tulip/MethodFactory.h>

SELECTIONPLUGIN(MultipleEdgeSelection,"Multiple edge","David Auber","20/01/2003","Alpha","0","1");

using namespace std;

struct edgeS {
  edgeS(SuperGraph *sg,edge e) {
    source=sg->source(e);
    target=sg->target(e);
    if (source.id>target.id) {
      target=source;
      source=sg->target(e);
    }
  }
  node source,target;
};

namespace std {
  template<>
  struct less<edgeS> {
    bool operator()(const edgeS &c,const edgeS &d) const {
      if (c.source.id<d.source.id) return true;
      if (c.source.id>d.source.id) return false;
      if (c.target.id<d.target.id) return true;
      if (c.target.id>d.target.id) return false;
      return false;
    }
  };
};

MultipleEdgeSelection::MultipleEdgeSelection(const PropertyContext &context):Selection(context) {}

MultipleEdgeSelection::~MultipleEdgeSelection() {
}

bool MultipleEdgeSelection::run() {
  set<edgeS> tmp;
  Iterator<edge> *itE=superGraph->getEdges();
  for(;itE->hasNext();) {
    edge ite=itE->next();
    edgeS tmpE(superGraph,ite);
    if (tmp.find(tmpE)==tmp.end())
      tmp.insert(tmpE);
    else
      selectionProxy->setEdgeValue(ite,true);
  }
  return true;
}
