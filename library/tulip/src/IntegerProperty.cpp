//-*-c++-*-
#include <float.h>
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
  // the property observes itself; see afterSet... methods
  addPropertyObserver(this);
}
//====================================================================
///Renvoie le minimum de la m�trique associ� aux noeuds du IntegerProperty
int IntegerProperty::getNodeMin(Graph *sg) {
  if (sg==0) sg=graph;
  unsigned long sgi=(unsigned long)sg;
  if (minMaxOkNode.find(sgi)==minMaxOkNode.end()) minMaxOkNode[sgi]=false;
  if (!minMaxOkNode[sgi]) computeMinMaxNode(sg);
  return minN[sgi];
}
//====================================================================
///Renvoie le maximum de la m�trique associ� aux noeuds du IntegerProperty
int IntegerProperty::getNodeMax(Graph *sg) {
  if (sg==0) sg=graph;
  unsigned long sgi=(unsigned long)sg;
  if (minMaxOkNode.find(sgi)==minMaxOkNode.end()) minMaxOkNode[sgi]=false;
  if (!minMaxOkNode[sgi]) computeMinMaxNode(sg);
  return maxN[sgi];
}
//====================================================================
///Renvoie le Minimum de la m�trique associ� aux ar�tes du IntegerProperty
int IntegerProperty::getEdgeMin(Graph *sg) {
  if (sg==0) sg=graph;
  unsigned long sgi=(unsigned long)sg;
  if (minMaxOkEdge.find(sgi)==minMaxOkEdge.end()) minMaxOkEdge[sgi]=false;
  if (!minMaxOkEdge[sgi]) computeMinMaxEdge(sg);
  return minE[sgi];
}
//====================================================================
///Renvoie le Maximum de la m�trique associ� aux ar�tes du IntegerProperty
int IntegerProperty::getEdgeMax(Graph *sg) {
  if (sg==0) sg=graph;
  unsigned long sgi=(unsigned long)sg;
  if (minMaxOkEdge.find(sgi)==minMaxOkEdge.end()) minMaxOkEdge[sgi]=false;
  if (!minMaxOkEdge[sgi]) computeMinMaxEdge(sg);
  return maxE[sgi];
}
//========================================================================
///Calcul le min et le Max de la m�trique associ� au proxy
void IntegerProperty::computeMinMaxNode(Graph *sg) {
  int maxN2,minN2;
  if (sg==0) sg=graph;
  Iterator<node> *itN=sg->getNodes();
  if (itN->hasNext()) {
    node itn=itN->next();
    int tmp=getNodeValue(itn);
    maxN2=tmp;
    minN2=tmp;
  }
  while (itN->hasNext()) {
    node itn=itN->next();
    int tmp=getNodeValue(itn);
    if (tmp>maxN2) maxN2=tmp;
    if (tmp<minN2) minN2=tmp;
  } delete itN;

  unsigned long sgi=(unsigned long)sg;

  minMaxOkNode[sgi]=true;  
  minN[sgi]=minN2;
  maxN[sgi]=maxN2;
}
//=========================================================
void IntegerProperty::computeMinMaxEdge(Graph *sg) {
  int maxE2,minE2;
  if (sg==0) sg=graph;
  Iterator<edge> *itE=sg->getEdges();
  if (itE->hasNext()) {
    edge ite=itE->next();
    int tmp=getEdgeValue(ite);
    maxE2=tmp;
    minE2=tmp;
  }
  while (itE->hasNext()) {
    edge ite=itE->next();
    int tmp=getEdgeValue(ite);
    if (tmp>maxE2) maxE2=tmp;
    if (tmp<minE2) minE2=tmp;
  } delete itE;

  unsigned long sgi=(unsigned long)sg;

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
//=============================================================
void IntegerProperty::copy( const node n0, const node n1, PropertyInterface * p )
{
  if( !p )
    return;
  IntegerProperty * tp = dynamic_cast<IntegerProperty*>(p);
  assert( tp );
  setNodeValue( n0, tp->getNodeValue(n1) );
}
//=============================================================
void IntegerProperty::copy( const edge e0, const edge e1, PropertyInterface * p )
{
  if( !p )
    return;
  IntegerProperty * tp = dynamic_cast<IntegerProperty*>(p);
  assert( tp );
  setEdgeValue( e0, tp->getEdgeValue(e1) );
}
//===============================================================
void IntegerProperty::beforeSetNodeValue(PropertyInterface*, const node) {
  minMaxOkNode.clear();
}
void IntegerProperty::beforeSetEdgeValue(PropertyInterface*, const edge) {
  minMaxOkEdge.clear();
}
void IntegerProperty::beforeSetAllNodeValue(PropertyInterface*) {
  minMaxOkNode.clear();
}
void IntegerProperty::beforeSetAllEdgeValue(PropertyInterface*) {
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
//=============================================================
void IntegerVectorProperty::copy( const node n0, const node n1, PropertyInterface * p ) {
  if( !p )
    return;
  IntegerVectorProperty * tp = dynamic_cast<IntegerVectorProperty*>(p);
  assert( tp );
  setNodeValue( n0, tp->getNodeValue(n1) );
}
//=============================================================
void IntegerVectorProperty::copy( const edge e0, const edge e1, PropertyInterface * p ) {
  if( !p )
    return;
  IntegerVectorProperty * tp = dynamic_cast<IntegerVectorProperty*>(p);
  assert( tp );
  setEdgeValue( e0, tp->getEdgeValue(e1) );
}
