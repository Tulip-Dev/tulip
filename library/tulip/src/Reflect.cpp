#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include "tulip/Types.h"
#include "tulip/AbstractProperty.h"
#include "tulip/Color.h"
#include "tulip/DoubleProperty.h"
#include "tulip/StringProperty.h"
#include "tulip/BooleanProperty.h"
#include "tulip/LayoutProperty.h"
#include "tulip/IntegerProperty.h"
#include "tulip/ColorProperty.h"
#include "tulip/SizeProperty.h"

#include "tulip/Reflect.h"

using namespace std;


bool DataSet::exist(const string &str) const {
  return data.find(str)!=data.end();
}

Iterator< pair<string,DataType> >* DataSet::getValues() const {
  return new StlMapIterator<string,DataType>(data.begin(),data.end());
}

void StructDef::erase(string str) {
  data.erase( data.find(str) );
  help.erase( help.find(str) );
  defValue.erase( defValue.find(str) );
}

Iterator< pair<string,string> >* StructDef::getField() const {
  return new StlMapIterator<string,string>( data.begin(), data.end() );
}

string StructDef::getHelp( string str ) const {
  map<string,string>::const_iterator it = help.find(str);
  if( it == help.end() )
  	return string();
  else
  	return it->second;
}

string StructDef::getDefValue( string str ) const {
  map<string,string>::const_iterator it = defValue.find(str);
  if( it == defValue.end() )
    return string();
  else
    return it->second;
}

#undef TN
#define	TN( T )	typeid(T).name()

void StructDef::buildDefaultDataSet(DataSet &ioDataSet, Graph *inG) {
  Iterator< pair<string,string> > * defIt;
  defIt = getField();

  while( defIt->hasNext() ) {
    pair<string,string> def;
    def = defIt->next();
    const string & name  = def.first;
    const string & tname = def.second;
    // Already defined ?
    if( ioDataSet.exist(name) )
      continue;
    // Has def value ?
    const string & defv = getDefValue( name );
    if( defv.size() == 0 )
      continue;
    // bool
    if( tname == TN(bool) ){
      bool v;
      bool res = BooleanType::fromString( v, defv );
      assert( res );
      if( res )
	ioDataSet.set( name, v );
    }
    // int
    else if( tname == TN(int) ) {
      int v;
      bool res = IntegerType::fromString( v, defv );
      assert( res );
      if( res )
	ioDataSet.set( name, v );
    }
    // unsigned int
    else if( tname == TN(unsigned int) ) {
      unsigned int v = atol( defv.c_str() );
      ioDataSet.set( name, v );
    }
    // fp
    else if( tname == TN(double) ) {
      double v;
      bool res = DoubleType::fromString( v, defv );
      assert( res );
      if( res )
	ioDataSet.set( name, (double)v );
    }
    else if( tname == TN(float) ) {
      double v;
      bool res = DoubleType::fromString( v, defv );
      assert( res );
      if( res )
	ioDataSet.set( name, (float)v );
    }
    // string
    else if( tname == TN(string) ) {
      ioDataSet.set( name, defv );
    }
    // Color
    else if( tname == TN(Color) ) {
      Color v;
      bool res = ColorType::fromString( v, defv );
      assert( res );
      if( res ) {
	ioDataSet.set( name, v );
	cout << v;
      }
    }
    // Size
    else if( tname == TN(Size) ) {
      Size v;
      bool res = SizeType::fromString( v, defv );
      assert( res );
      if( res )
	ioDataSet.set( name, v );
    }
    // Selection
    else if( inG && tname == TN(BooleanProperty) ) {
      ioDataSet.set( name, inG->getProperty<BooleanProperty>(defv) );
    }
    // Metric
    else if( inG && tname == TN(DoubleProperty) ) {
      ioDataSet.set( name, inG->getProperty<DoubleProperty>(defv) );
    }
    // Layout
    else if( inG && tname == TN(LayoutProperty) ) {
      ioDataSet.set( name, inG->getProperty<LayoutProperty>(defv) );
    }
    // String
    else if( inG && tname == TN(StringProperty) ) {
      ioDataSet.set( name, inG->getProperty<StringProperty>(defv) );
    }
    // Integer
    else if( inG && tname == TN(IntegerProperty) ) {
      ioDataSet.set( name, inG->getProperty<IntegerProperty>(defv) );
    }
    // SizeProperty
    else if( inG && tname == TN(SizeProperty) ) {
      ioDataSet.set( name, inG->getProperty<SizeProperty>(defv) );
    }
    // Colors
    else if( inG && tname == TN(ColorProperty) ) {
      ioDataSet.set( name, inG->getProperty<ColorProperty>(defv) );
    }
  }
  delete defIt;
}





