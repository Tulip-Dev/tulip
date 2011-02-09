/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#include <float.h>
#include "tulip/SizeProperty.h"
#include "tulip/PluginContext.h"
#include "tulip/Observable.h"
#include "tulip/SizeAlgorithm.h"

using namespace std;
using namespace tlp;

class SizeMetaValueCalculator
  :public AbstractSizeProperty::MetaValueCalculator {
public:
  void computeMetaValue(AbstractSizeProperty* prop,
			node mN, Graph* sg, Graph*) {
    if (sg->numberOfNodes() == 0) {
      prop->setNodeValue(mN, Size(0, 0, 0));
      return;
    }

    // between the min and max computed values
    prop->setNodeValue(mN,
			 (((SizeProperty *)prop)->getMax(sg) +
			  ((SizeProperty *)prop)->getMin(sg)) / 2.0);
  }
};

static SizeMetaValueCalculator mvSizeCalculator;

//==============================
SizeProperty::SizeProperty (Graph *sg, std::string n):
  AbstractProperty<SizeType,SizeType, SizeAlgorithm>(sg, n) {
  // the computed meta value will be the average value
  setMetaValueCalculator(&mvSizeCalculator);
}
//====================================================================
void SizeProperty::scale(const tlp::Vector<float,3>& v, Iterator<node> *itN, Iterator<edge> *itE) {
  Observable::holdObservers();
  while (itN->hasNext()) {
    node itn = itN->next();
    Size tmpSize(getNodeValue(itn));
    tmpSize *= v;
    setNodeValue(itn,tmpSize);
  }
  while (itE->hasNext()) {
    edge ite=itE->next();
    Size tmpSize(getEdgeValue(ite));
    tmpSize *= v;
    setEdgeValue(ite,tmpSize);
  }
  resetMinMax();
  Observable::unholdObservers();
}
//=============================================================================
void SizeProperty::scale( const tlp::Vector<float,3>& v, Graph *sg ) {
  if (sg==0) sg = graph;
  if (sg->numberOfNodes()==0) return;
  Iterator<node> *itN = sg->getNodes();
  Iterator<edge> *itE = sg->getEdges();
  scale(v, itN, itE);
  delete itN;
  delete itE;
}
//=============================================================================
Size SizeProperty::getMax(Graph *sg) {
  if (sg==0) sg=graph;
  unsigned int sgi = sg->getId();
  if (minMaxOk.find(sgi)==minMaxOk.end()) minMaxOk[sgi]=false;
  if (!minMaxOk[sgi]) computeMinMax(sg);
  return max[sgi];
}
//=============================================================================
Size  SizeProperty::getMin(Graph *sg) {
  if (sg==0) sg=graph;
  unsigned int sgi = sg->getId();
  if (minMaxOk.find(sgi)==minMaxOk.end()) minMaxOk[sgi]=false;
  if (!minMaxOk[sgi]) computeMinMax(sg);
  return min[sgi];
}
//=============================================================================
void SizeProperty::computeMinMax(Graph *sg) {
  Size maxS,minS;
  Iterator<node> *itN=sg->getNodes();
  if  (itN->hasNext()) {
    node itn=itN->next();
    const Size& tmpSize=getNodeValue(itn);
    for (int i=0;i<3;++i) {
      maxS[i]=tmpSize[i];
      minS[i]=tmpSize[i];
    }
  }
  while (itN->hasNext()) {
    node itn=itN->next();
    const Size& tmpSize=getNodeValue(itn);
    for (int i=0;i<3;++i) {
      maxS[i] = std::max(maxS[i], tmpSize[i]);
      minS[i] = std::min(minS[i], tmpSize[i]);
    }
  }delete itN;
  unsigned int sgi = sg->getId();
  minMaxOk[sgi]=true;  
  min[sgi]=minS;
  max[sgi]=maxS;
}
//=============================================================================
void SizeProperty::resetMinMax() {
  minMaxOk.clear();
  min.clear();
  max.clear();
}
//=================================================================================
void SizeProperty::setNodeValue(const node n, const Size &v) {
  TLP_HASH_MAP<unsigned int, bool>::const_iterator it = minMaxOk.begin();
  if (it != minMaxOk.end()) {
    const Size& oldV = getNodeValue(n);
    if (v != oldV) {
      // loop on subgraph min/max
      for(; it != minMaxOk.end(); ++it) {
	unsigned int gid = (*it).first;
	const Size& minV = min[gid];
	const Size& maxV = max[gid];
	// check if min or max has to be updated
	if ((v < minV) || (v > maxV) || (oldV == minV) || (oldV == maxV)) {
	  resetMinMax();
	  break;
	}
      }
    }
  }
  AbstractSizeProperty::setNodeValue(n, v);
}
//=================================================================================
void SizeProperty::setAllNodeValue(const Size &v) {
  resetMinMax();
  AbstractSizeProperty::setAllNodeValue(v);
}
//=============================================================================
PropertyInterface* SizeProperty::clonePrototype(Graph * g, const std::string& n) {
  if( !g )
    return 0;
  SizeProperty * p = g->getLocalProperty<SizeProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}
//=============================================================================
PropertyInterface* SizeVectorProperty::clonePrototype(Graph * g, const std::string& n) {
  if( !g )
    return 0;
  SizeVectorProperty * p = g->getLocalProperty<SizeVectorProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}
