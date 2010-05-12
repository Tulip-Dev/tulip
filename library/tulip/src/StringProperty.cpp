//-*-c++-*-

#include "tulip/StringProperty.h"
#include "tulip/PluginContext.h"
#include "tulip/Observable.h"
#include "tulip/StringAlgorithm.h"
#include "tulip/AbstractProperty.h"

using namespace std;
using namespace tlp;

//=================================================================================
PropertyInterface* StringProperty::clonePrototype(Graph * g, const std::string& n)
{
	if( !g )
		return 0;
	StringProperty * p = g->getLocalProperty<StringProperty>( n );
	p->setAllNodeValue( getNodeDefaultValue() );
	p->setAllEdgeValue( getEdgeDefaultValue() );
	return p;
}
//=================================================================================
PropertyInterface* StringVectorProperty::clonePrototype(Graph * g, const std::string& n) {
  if( !g )
    return 0;
  StringVectorProperty * p = g->getLocalProperty<StringVectorProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}

