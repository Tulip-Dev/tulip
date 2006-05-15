//-*-c++-*-
#include <float.h>
#include "tulip/ColorProperty.h"
#include "tulip/PluginContext.h"
#include "tulip/Observable.h"
#include "tulip/ColorAlgorithm.h"
#include "tulip/AbstractProperty.h"

using namespace std;
using namespace tlp;

//==============================
ColorProperty::ColorProperty (Graph *sg):AbstractProperty<ColorType,ColorType, ColorAlgorithm>(sg) {
  //  propertyProxy=this;
}
//==============================
ColorProperty::~ColorProperty() {
  notifyDestroy();
}    
//=================================================================================
PropertyInterface* ColorProperty::clonePrototype(Graph * g, std::string n) {
  if( !g )
    return 0;
  ColorProperty * p = g->getLocalProperty<ColorProperty>( n );
  p->setAllNodeValue( getNodeDefaultValue() );
  p->setAllEdgeValue( getEdgeDefaultValue() );
  return p;
}
//=============================================================
void ColorProperty::copy( const node n0, const node n1, PropertyInterface * p ) {
  if( !p )
    return;
  ColorProperty * tp = dynamic_cast<ColorProperty*>(p);
  assert( tp );
  setNodeValue( n0, tp->getNodeValue(n1) );
}
//=============================================================
void ColorProperty::copy( const edge e0, const edge e1, PropertyInterface * p ) {
  if( !p )
    return;
  ColorProperty * tp = dynamic_cast<ColorProperty*>(p);
  assert( tp );
  setEdgeValue( e0, tp->getEdgeValue(e1) );
}
