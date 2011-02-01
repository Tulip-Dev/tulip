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
#include <limits.h>
#include "tulip/IntegerProperty.h"
#include "tulip/PluginContext.h"
#include "tulip/Observable.h"
#include "tulip/IntegerAlgorithm.h"
#include "tulip/AbstractProperty.h"

using namespace std;
using namespace tlp;

//==============================
///Constructeur d'un IntegerProperty
IntegerProperty::IntegerProperty (Graph *sg, std::string n):AbstractProperty<IntegerType,IntegerType, IntegerAlgorithm>(sg, n),
  minMaxOkNode(false),minMaxOkEdge(false) {
  // the property observes the graph
  sg->addGraphObserver(this);
}
//====================================================================
///Renvoie le minimum de la m�trique associ� aux noeuds du IntegerProperty
int IntegerProperty::getNodeMin(Graph *sg) {
  if (sg==0) sg=graph;
  unsigned int sgi = sg->getId();
  if (minMaxOkNode.find(sgi)==minMaxOkNode.end()) minMaxOkNode[sgi]=false;
  if (!minMaxOkNode[sgi]) computeMinMaxNode(sg);
  return minN[sgi];
}
//====================================================================
///Renvoie le maximum de la m�trique associ� aux noeuds du IntegerProperty
int IntegerProperty::getNodeMax(Graph *sg) {
  if (sg==0) sg=graph;
  unsigned int sgi = sg->getId();
  if (minMaxOkNode.find(sgi)==minMaxOkNode.end()) minMaxOkNode[sgi]=false;
  if (!minMaxOkNode[sgi]) computeMinMaxNode(sg);
  return maxN[sgi];
}
//====================================================================
///Renvoie le Minimum de la m�trique associ� aux ar�tes du IntegerProperty
int IntegerProperty::getEdgeMin(Graph *sg) {
  if (sg==0) sg=graph;
  unsigned int sgi = sg->getId();
  if (minMaxOkEdge.find(sgi)==minMaxOkEdge.end()) minMaxOkEdge[sgi]=false;
  if (!minMaxOkEdge[sgi]) computeMinMaxEdge(sg);
  return minE[sgi];
}
//====================================================================
///Renvoie le Maximum de la m�trique associ� aux ar�tes du IntegerProperty
int IntegerProperty::getEdgeMax(Graph *sg) {
  if (sg==0) sg=graph;
  unsigned int sgi = sg->getId();
  if (minMaxOkEdge.find(sgi)==minMaxOkEdge.end()) minMaxOkEdge[sgi]=false;
  if (!minMaxOkEdge[sgi]) computeMinMaxEdge(sg);
  return maxE[sgi];
}
//========================================================================
///Calcul le min et le Max de la m�trique associ� au proxy
void IntegerProperty::computeMinMaxNode(Graph *sg) {
  int maxN2 = INT_MIN, minN2 = INT_MAX;
  if (sg==0) sg=graph;
  Iterator<node> *itN=sg->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    int tmp=getNodeValue(itn);
    if (tmp>maxN2) maxN2=tmp;
    if (tmp<minN2) minN2=tmp;
  } delete itN;

  unsigned int sgi = sg->getId();

  minMaxOkNode[sgi]=true;  
  minN[sgi]=minN2;
  maxN[sgi]=maxN2;
}
//=========================================================
void IntegerProperty::computeMinMaxEdge(Graph *sg) {
  int maxE2 = INT_MIN, minE2 = INT_MAX;
  if (sg==0) sg=graph;
  Iterator<edge> *itE=sg->getEdges();
  while (itE->hasNext()) {
    edge ite=itE->next();
    int tmp=getEdgeValue(ite);
    if (tmp>maxE2) maxE2=tmp;
    if (tmp<minE2) minE2=tmp;
  } delete itE;

  unsigned int sgi = sg->getId();

  minMaxOkEdge[sgi]=true;
  minE[sgi]=minE2;
  maxE[sgi]=maxE2;
}
//=================================================================================
void IntegerProperty::clone_handler(AbstractProperty<IntegerType,IntegerType> &proxyC) {
  if (typeid(this)==typeid(&proxyC)) {
    IntegerProperty *proxy=(IntegerProperty *)&proxyC;
    minMaxOkNode=proxy->minMaxOkNode;
    minMaxOkEdge=proxy->minMaxOkEdge;
    minN=proxy->minN;
    maxN=proxy->maxN;
    minE=proxy->minE;
    maxE=proxy->maxE;
  }
}
//=================================================================================
PropertyInterface* IntegerProperty::clonePrototype(Graph * g, const std::string& n)
{
  if( !g )
    return 0;
  IntegerProperty * p = g->getLocalProperty<IntegerProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
 }
//=================================================================================
void IntegerProperty::setNodeValue(const node n, const int &v) {
  TLP_HASH_MAP<unsigned int, bool>::const_iterator it = minMaxOkNode.begin();
  if (it != minMaxOkNode.end()) {
    int oldV = getNodeValue(n);
    if (v != oldV) {
      // loop on subgraph min/max
      for(; it != minMaxOkNode.end(); ++it) {
	unsigned int gid = (*it).first;
	int minV = minN[gid];
	int maxV = maxN[gid];
	// check if min or max has to be updated
	if ((v < minV) || (v > maxV) || (oldV == minV) || (oldV == maxV)) {
	  minMaxOkNode.clear();
	  break;
	}
      }
    }
  }
  AbstractIntegerProperty::setNodeValue(n, v);
}
//=================================================================================
void IntegerProperty::setEdgeValue(const edge e, const int &v) {
  TLP_HASH_MAP<unsigned int, bool>::const_iterator it = minMaxOkEdge.begin();
  if (it != minMaxOkEdge.end()) {
    int oldV = getEdgeValue(e);
    if (v != oldV) {
      // loop on subgraph min/max
      for(; it != minMaxOkEdge.end(); ++it) {
	unsigned int gid = (*it).first;
	int minV = minE[gid];
	int maxV = maxE[gid];
	// check if min or max has to be updated
	if ((v < minV) || (v > maxV) || (oldV == minV) || (oldV == maxV)) {
	  minMaxOkEdge.clear();
	  break;
	}
      }
    }
  }
  AbstractIntegerProperty::setEdgeValue(e, v);
}
//=================================================================================
void IntegerProperty::setAllNodeValue(const int &v) {
  minMaxOkNode.clear();
  AbstractIntegerProperty::setAllNodeValue(v);
}
//=================================================================================
void IntegerProperty::setAllEdgeValue(const int &v) {
  minMaxOkEdge.clear();
  AbstractIntegerProperty::setAllEdgeValue(v);
}
//=================================================================================
void IntegerProperty::addNode(Graph*, const node) {
  minMaxOkNode.clear();
}
//=================================================================================
void IntegerProperty::addEdge(Graph*, const edge) {
  minMaxOkEdge.clear();
}
//=================================================================================
PropertyInterface* IntegerVectorProperty::clonePrototype(Graph * g, const std::string& n) {
  if( !g )
    return 0;
  IntegerVectorProperty * p = g->getLocalProperty<IntegerVectorProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}
