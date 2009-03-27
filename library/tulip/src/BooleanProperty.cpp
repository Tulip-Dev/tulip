//-*-c++-*-

#include "tulip/BooleanProperty.h"
#include "tulip/PluginContext.h"
#include "tulip/Observable.h"
#include "tulip/BooleanAlgorithm.h"
#include "tulip/AbstractProperty.h"
#include "tulip/GraphIterator.h"

using namespace std;
using namespace tlp;

//=================================================================================
BooleanProperty::BooleanProperty (Graph *sg):AbstractProperty<BooleanType,BooleanType, BooleanAlgorithm>(sg) {
  //  propertyProxy=this;
}
//=================================================================================
Iterator<node>* BooleanProperty::getNodesEqualTo(const bool val, Graph *sg) {
  if (sg == 0) sg = graph;
  Iterator<unsigned int> *it = 0;
  if (sg == graph) {
    try {
      it = nodeProperties.findAll(val);
    } catch (ImpossibleOperation &e) {
      it=0;
    }
  }
  if (it==0)
    return (new SGraphNodeIterator(sg, nodeProperties));
  else
    return (new UINTIterator<node>(it));
}
//=================================================================================
Iterator<edge>* BooleanProperty::getEdgesEqualTo(const bool val, Graph *sg) {
  if (sg == 0) sg = graph;
  Iterator<unsigned int> *it=0;
  if (sg == graph) {
    try {
      it = edgeProperties.findAll(val);
    } catch (ImpossibleOperation &e) {
      it=0;
    }
  }
  if (it==0)
    return (new SGraphEdgeIterator(sg, edgeProperties));
  else
    return (new UINTIterator<edge>(it));
}
//=================================================================================
//Fonctionnalit� suppl�mentaire ajout� au seletion 
void BooleanProperty::reverse() {
  Observable::holdObservers();
  Iterator<node> *itN=graph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    if (getNodeValue(itn)==true)
      setNodeValue(itn,false);
    else
      setNodeValue(itn,true);
  } delete itN;
  Iterator<edge> *itE=graph->getEdges();
  while (itE->hasNext()) {
    edge ite=itE->next();
    if (getEdgeValue(ite)==true)
      setEdgeValue(ite,false);
    else
      setEdgeValue(ite,true);
  } delete itE;
  notifyObservers();
  Observable::unholdObservers();
}
//=================================================================================
void BooleanProperty::reverseEdgeDirection() {
  Iterator<edge> *itE=graph->getEdges();
  while (itE->hasNext()) {
    edge ite=itE->next();
    if (getEdgeValue(ite))
      graph->reverse(ite);
  } delete itE;
}
//=================================================================================
PropertyInterface* BooleanProperty::clonePrototype(Graph * g, std::string n)
{
	if( !g )
		return 0;
	BooleanProperty * p = g->getLocalProperty<BooleanProperty>( n );
	p->setAllNodeValue( getNodeDefaultValue() );
	p->setAllEdgeValue( getEdgeDefaultValue() );
	return p;
}
//=============================================================
void BooleanProperty::copy( const node n0, const node n1, PropertyInterface * p )
{
	if( !p )
		return;
	BooleanProperty * tp = dynamic_cast<BooleanProperty*>(p);
	assert( tp );
	setNodeValue( n0, tp->getNodeValue(n1) );
}
//=============================================================
void BooleanProperty::copy( const edge e0, const edge e1, PropertyInterface * p )
{
	if( !p )
		return;
	BooleanProperty * tp = dynamic_cast<BooleanProperty*>(p);
	assert( tp );
	setEdgeValue( e0, tp->getEdgeValue(e1) );
}
//=================================================================================
PropertyInterface* BooleanVectorProperty::clonePrototype(Graph * g, std::string n) {
  if( !g )
    return 0;
  BooleanVectorProperty * p = g->getLocalProperty<BooleanVectorProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}
//=============================================================
void BooleanVectorProperty::copy( const node n0, const node n1, PropertyInterface * p ) {
  if( !p )
    return;
  BooleanVectorProperty * tp = dynamic_cast<BooleanVectorProperty*>(p);
  assert( tp );
  setNodeValue( n0, tp->getNodeValue(n1) );
}
//=============================================================
void BooleanVectorProperty::copy( const edge e0, const edge e1, PropertyInterface * p ) {
  if( !p )
    return;
  BooleanVectorProperty * tp = dynamic_cast<BooleanVectorProperty*>(p);
  assert( tp );
  setEdgeValue( e0, tp->getEdgeValue(e1) );
}



