#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include "tulip/Types.h"
#include "tulip/PropertyProxy.h"
#include "tulip/Color.h"
#include "tulip/MetricProxy.h"
#include "tulip/StringProxy.h"
#include "tulip/SelectionProxy.h"
#include "tulip/LayoutProxy.h"
#include "tulip/IntProxy.h"
#include "tulip/ColorsProxy.h"
#include "tulip/SizesProxy.h"

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

void StructDef::buildDefaultDataSet(DataSet &ioDataSet, SuperGraph *inG) {
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
      bool res = IntType::fromString( v, defv );
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
	//cout << v;
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
    // SelectionProxy
    else if( inG && tname == TN(SelectionProxy) ) {
      ioDataSet.set( name, inG->getProperty<SelectionProxy>(defv) );
    }
    // MetricProxy
    else if( inG && tname == TN(MetricProxy) ) {
      ioDataSet.set( name, inG->getProperty<MetricProxy>(defv) );
    }
    // LayoutProxy
    else if( inG && tname == TN(LayoutProxy) ) {
      ioDataSet.set( name, inG->getProperty<LayoutProxy>(defv) );
    }
    // StringProxy
    else if( inG && tname == TN(StringProxy) ) {
      ioDataSet.set( name, inG->getProperty<StringProxy>(defv) );
    }
    // IntProxy
    else if( inG && tname == TN(IntProxy) ) {
      ioDataSet.set( name, inG->getProperty<IntProxy>(defv) );
    }
    // SizesProxy
    else if( inG && tname == TN(SizesProxy) ) {
      ioDataSet.set( name, inG->getProperty<SizesProxy>(defv) );
    }
    // ColorsProxy
    else if( inG && tname == TN(ColorsProxy) ) {
      ioDataSet.set( name, inG->getProperty<ColorsProxy>(defv) );
    }
  }
  delete defIt;
}





