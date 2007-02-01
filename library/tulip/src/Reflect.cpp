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
using namespace tlp;


bool DataSet::exist(const string &str) const {
  for (std::list< std::pair<std::string, tlp::DataType> >::const_iterator it =
	 data.begin(); it != data.end(); ++it) {
    if ((*it).first == str)
      return true;
  }
  return false;
}

Iterator< pair<string, DataType> >* DataSet::getValues() const {
  list< pair<string, DataType> >::const_iterator begin = data.begin();
  list< pair<string, DataType> >::const_iterator end = data.end();
  
  return new StlIterator<pair<string, DataType>, list< pair<string, DataType> >::const_iterator>(begin, end);
}

void StructDef::erase(string str) {
  for (std::list< std::pair<std::string, std::string> >::iterator it =
	 data.begin(); it != data.end(); ++it) {
    if ((*it).first == str) {
      data.erase(it);
      break;
    }
  }
  help.erase( help.find(str) );
  defValue.erase( defValue.find(str) );
}

Iterator< pair<string, string> >* StructDef::getField() const {
  list< pair<string, string> >::const_iterator begin = data.begin();
  list< pair<string, string> >::const_iterator end = data.end();
  
  return new StlIterator<pair<string, string>, list< pair<string, string> >::const_iterator>(begin, end);
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
    // BooleanProperty
    else if( inG && tname == TN(BooleanProperty) ) {
      ioDataSet.set( name, inG->getProperty<BooleanProperty>(defv) );
    }
    // DoubleProperty
    else if( inG && tname == TN(DoubleProperty) ) {
      ioDataSet.set( name, inG->getProperty<DoubleProperty>(defv) );
    }
    // LayoutProperty
    else if( inG && tname == TN(LayoutProperty) ) {
      ioDataSet.set( name, inG->getProperty<LayoutProperty>(defv) );
    }
    // StringProperty
    else if( inG && tname == TN(StringProperty) ) {
      ioDataSet.set( name, inG->getProperty<StringProperty>(defv) );
    }
    // IntegerProperty
    else if( inG && tname == TN(IntegerProperty) ) {
      ioDataSet.set( name, inG->getProperty<IntegerProperty>(defv) );
    }
    // SizeProperty
    else if( inG && tname == TN(SizeProperty) ) {
      ioDataSet.set( name, inG->getProperty<SizeProperty>(defv) );
    }
    // ColorProperty
    else if( inG && tname == TN(ColorProperty) ) {
      ioDataSet.set( name, inG->getProperty<ColorProperty>(defv) );
    }
    //
    else if ( inG && tname == TN(PropertyInterface*) ) {
      // look for an already existing property
      if (inG->existProperty(defv))
	ioDataSet.set( name, inG->getProperty(defv) );
    }
  }
  delete defIt;
}
