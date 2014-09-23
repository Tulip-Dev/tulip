/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#include <tulip/SizeProperty.h>
#include <tulip/Observable.h>

using namespace std;
using namespace tlp;

class SizeMetaValueCalculator
    :public AbstractSizeProperty::MetaValueCalculator {
public:
  void computeMetaValue(AbstractSizeProperty* prop,
                        node mN, Graph* sg, Graph*) {
    // nothing to do if the subgraph is not linked to the property graph
    if (sg!=prop->getGraph() && !prop->getGraph()->isDescendantGraph(sg)) {
#ifndef NDEBUG
      tlp::warning() << "Warning : " << __PRETTY_FUNCTION__ << " does not compute any value for a subgraph not linked to the graph of the property " << prop->getName().c_str() << std::endl;
#endif
      return;
    }

    if (sg->numberOfNodes() == 0) {
      prop->setNodeValue(mN, Size(1, 1, 1));
      return;
    }

    // between the min and max computed values
    prop->setNodeValue(mN,
                       (((SizeProperty *)prop)->getMax(sg) +
                        ((SizeProperty *)prop)->getMin(sg)) / 2.0f);
  }
};

static SizeMetaValueCalculator mvSizeCalculator;

const string SizeProperty::propertyTypename="size";
const string SizeVectorProperty::propertyTypename="vector<size>";

//==============================
SizeProperty::SizeProperty (Graph *sg, const std::string& n):
  AbstractProperty<SizeType,SizeType>(sg, n) {
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
  if (sg==NULL) sg = graph;

  if (sg->numberOfNodes()==0) return;

  Iterator<node> *itN = sg->getNodes();
  Iterator<edge> *itE = sg->getEdges();
  scale(v, itN, itE);
  delete itN;
  delete itE;
}
//=============================================================================
Size SizeProperty::getMax(Graph *sg) {
  if (sg==NULL) sg=graph;

  unsigned int sgi = sg->getId();

  if (minMaxOk.find(sgi)==minMaxOk.end()) minMaxOk[sgi]=false;

  if (!minMaxOk[sgi]) computeMinMax(sg);

  return max[sgi];
}
//=============================================================================
Size  SizeProperty::getMin(Graph *sg) {
  if (sg==NULL) sg=graph;

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

    for (int i=0; i<3; ++i) {
      maxS[i]=tmpSize[i];
      minS[i]=tmpSize[i];
    }
  }

  while (itN->hasNext()) {
    node itn=itN->next();
    const Size& tmpSize=getNodeValue(itn);

    for (int i=0; i<3; ++i) {
      maxS[i] = std::max(maxS[i], tmpSize[i]);
      minS[i] = std::min(minS[i], tmpSize[i]);
    }
  }

  delete itN;
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

  // allow to get an unregistered property (empty name)
  SizeProperty * p = n.empty()
                     ? new SizeProperty(g) : g->getLocalProperty<SizeProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}

int SizeProperty::compare(const node n1, const node n2) const {
  const Size& s1 = getNodeValue(n1);
  const Size& s2 = getNodeValue(n2);
  float v1 = fabs(s1[0])* fabs(s1[1]) * fabs(s1[2]);
  float v2 = fabs(s2[0])* fabs(s2[1])* fabs(s2[2]);
  return v1 == v2 ?0:(v1>v2 ? 1 : -1);
}

//=============================================================================
PropertyInterface* SizeVectorProperty::clonePrototype(Graph * g, const std::string& n) {
  if( !g )
    return 0;

  // allow to get an unregistered property (empty name)
  SizeVectorProperty * p = n.empty()
                           ? new SizeVectorProperty(g) : g->getLocalProperty<SizeVectorProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}
