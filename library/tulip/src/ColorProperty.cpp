//-*-c++-*-
#include <float.h>
#include "tulip/ColorProperty.h"
#include "tulip/PluginContext.h"
#include "tulip/Observable.h"
#include "tulip/ColorAlgorithm.h"
#include "tulip/AbstractProperty.h"

using namespace std;
using namespace tlp;

//=================================================================================
PropertyInterface* ColorProperty::clonePrototype(Graph * g, const std::string& n) {
  if( !g )
    return 0;
  ColorProperty * p = g->getLocalProperty<ColorProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}
//=================================================================================
PropertyInterface* ColorVectorProperty::clonePrototype(Graph * g, const std::string& n) {
  if( !g )
    return 0;
  ColorVectorProperty * p = g->getLocalProperty<ColorVectorProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}
