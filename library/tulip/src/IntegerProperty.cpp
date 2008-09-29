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
IntegerProperty::IntegerProperty (Graph *sg):AbstractProperty<IntegerType,IntegerType, IntegerAlgorithm>(sg) {
  minMaxOk=false;
  //  propertyProxy=this;
}
//====================================================================
///Renvoie le minimum de la m�trique associ� aux noeuds du IntegerProperty
int IntegerProperty::getNodeMin() {
  if (!minMaxOk) 
    computeMinMax();
  return minN;
}
//====================================================================
///Renvoie le maximum de la m�trique associ� aux noeuds du IntegerProperty
int IntegerProperty::getNodeMax() {
  if (!minMaxOk) 
    computeMinMax();
  return maxN;
}
//====================================================================
///Renvoie le Minimum de la m�trique associ� aux ar�tes du IntegerProperty
int IntegerProperty::getEdgeMin() {
  if (!minMaxOk) 
    computeMinMax();
  return minE;
}
//====================================================================
///Renvoie le Maximum de la m�trique associ� aux ar�tes du IntegerProperty
int IntegerProperty::getEdgeMax() {
  if (!minMaxOk) 
    computeMinMax();
  return maxE;
}
//========================================================================
///Calcul le min et le Max de la m�trique associ� au proxy
///Attention, la gestion du mim et max des ar�tes n'est pas 
///assur� ici et doit �tre ajout� ult�rieurement
void IntegerProperty::computeMinMax() {
  //cerr << "Compute Min Max" << endl;
  int tmp;
  Iterator<node> *itN=graph->getNodes();
  if (itN->hasNext()) {
    node itn=itN->next();
    tmp=getNodeValue(itn);
    maxN=tmp;
    minN=tmp;
  }
  for (;itN->hasNext();) {
    node itn=itN->next();
    tmp=getNodeValue(itn);
    if (tmp>maxN) maxN=tmp;
    if (tmp<minN) minN=tmp;
  }
  delete itN;
  Iterator<edge> *itE=graph->getEdges();
  if (itE->hasNext()) {
    edge ite=itE->next();
    tmp=getEdgeValue(ite);
    maxE=tmp;
    minE=tmp;
  }
  for (;itE->hasNext();) {
    edge ite=itE->next();
    tmp=getEdgeValue(ite);
    if (tmp>maxE) maxE=tmp;
    if (tmp<minE) minE=tmp;
  }
  delete itE;
  minMaxOk=true;
}
//=============================================================================
///Poign� permettnet le reset des variables du IntegerProperty par le AbstractProperty
void IntegerProperty::reset_handler() {
  minMaxOk=false;
}
//=============================================================================
///Poign� permettnet le recompute il faut absolument mettre le sgProperties
///currentIntProxy � this sinon le r�sultat est impr�visible
void IntegerProperty::recompute_handler() {
  //  graph->getPropertyManager()->currentAbstractProperty=this;
  minMaxOk=false;
}
//=================================================================================
void IntegerProperty::clone_handler(AbstractProperty<IntegerType,IntegerType> &proxyC) {
  if (typeid(this)==typeid(&proxyC)) {
    IntegerProperty *proxy=(IntegerProperty *)&proxyC;
    minMaxOk=proxy->minMaxOk;
    if (minMaxOk) {
      minE=proxy->minE;
      maxE=proxy->maxE;
      minN=proxy->minN;
      maxN=proxy->maxN;
    }
  }
  else{
    minMaxOk=false;
  }
}

//=================================================================================
PropertyInterface* IntegerProperty::clonePrototype(Graph * g, std::string n)
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






