//-*-c++-*-

#include "tulip/StringProperty.h"
#include "tulip/PluginContext.h"
#include "tulip/Observable.h"
#include "tulip/StringAlgorithm.h"
#include "tulip/AbstractProperty.h"

using namespace std;
using namespace tlp;

StringProperty::StringProperty (Graph *sg):AbstractProperty<StringType,StringType, StringAlgorithm>(sg) {
  //  propertyProxy=this;
}
  
void StringProperty::reset_handler() {
}

//=============================================================================
void StringProperty::recompute_handler() {
  //  graph->getPropertyManager()->currentAbstractProperty=this;
}

//=================================================================================
PropertyInterface* StringProperty::clonePrototype(Graph * g, std::string n)
{
	if( !g )
		return 0;
	StringProperty * p = g->getLocalProperty<StringProperty>( n );
	p->setAllNodeValue( getNodeDefaultValue() );
	p->setAllEdgeValue( getEdgeDefaultValue() );
	return p;
}
//=============================================================
void StringProperty::copy( const node n0, const node n1, PropertyInterface * p )
{
	if( !p )
		return;
	StringProperty * tp = dynamic_cast<StringProperty*>(p);
	assert( tp );
	setNodeValue( n0, tp->getNodeValue(n1) );
}
//=============================================================
void StringProperty::copy( const edge e0, const edge e1, PropertyInterface * p )
{
	if( !p )
	  return;
	StringProperty * tp = dynamic_cast<StringProperty*>(p);
	assert( tp );
	setEdgeValue( e0, tp->getEdgeValue(e1) );
}


