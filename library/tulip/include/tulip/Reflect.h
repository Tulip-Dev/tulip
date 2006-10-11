#ifndef _TULIPREFLECT
#define _TULIPREFLECT

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <tulip/tulipconf.h>
#include <iostream>
#include <string>
#include <cassert>
#include <typeinfo>
#include <list>
#include "StlIterator.h"

namespace tlp {

struct Graph;


struct TLP_SIMPLE_SCOPE DataType {
  DataType(){}
  DataType(void *value,const std::string typeName):value(value),typeName(typeName){}
  void * value;
  std::string typeName;
};


/*!  A container which allows insertion of different type.
     The inserted data must have a copy-constructor well done */
struct TLP_SIMPLE_SCOPE DataSet {
  /** Return a copy of the value of the variable with name str.
     Type are checked in Debug Mode.
     If the variable str doesn't exist return false else true. */
  template<typename T> bool get(const std::string &str, T& value) const;
  /** Return a copy of the value of the variable with name str.
     Type are checked in Debug Mode.
     If the variable str doesn't exist return false else true.
     The data is removed after the call. */
  template<typename T> bool getAndFree(const std::string &str, T& value);
  /** Set the value of the variable str.*/
  template<typename T> void set(const std::string &str,const T& value);
  /** return true if str exists else false.*/
  bool exist(const std::string &str) const;
  /**Return an iterator on all values*/
  Iterator< std::pair<std::string, DataType> > *getValues() const;
private:
  std::list< std::pair<std::string, DataType> > data;
};



///This class enables to define a structure
struct TLP_SIMPLE_SCOPE StructDef {
  ///Add the variable of type T and name str in the structure.
  template<typename T> void add(const char* str,
				const char* inHelp = 0,
				const char* inDefValue = 0);

  ///Get iterator on structure fields
  Iterator< std::pair<std::string,std::string> >* getField() const;

  ///Get field help & default string-value (see also XXXType in Types.h)
  std::string									  getHelp( std::string str ) const;
  std::string									  getDefValue( std::string str ) const;

  ///Remove the variable which have str has name in the structure.
  void		erase(std::string str);

  ///Build a default dataSet according to fields
  ///The optional Graph is needed to create properties (PropertyInterface*)
  void		buildDefaultDataSet( DataSet & ioDataSet, Graph * inG = 0 );
private:
  std::list< std::pair<std::string, std::string> > data;
  std::map<std::string,std::string> help;
  std::map<std::string,std::string> defValue;
};




//
// Standard structDef's HTML Help

#define	HTML_HELP_OPEN()	"<table><tr><td>" \
							"<table CELLSPACING=1 bgcolor=\"#EEEEEE\">"
#define	HTML_HELP_DEF(A,B)	"<tr><td><b>" A "</b></td><td><FONT COLOR=\"red\">" B "</td></tr>"
#define	HTML_HELP_BODY()	"</table></td><td>"
#define	HTML_HELP_CLOSE()	"</td></tr></table>"


/*
 * Usage :

  	const char * paramHelp[] = {
		// property
		HTML_HELP_OPEN() \
		HTML_HELP_DEF( "type", "DoubleProperty" ) \
		HTML_HELP_BODY() \
		"This metricProxy is used to affect scalar values to graph items." \
		"The meaning of theses values depends of the choosen color model." \
		HTML_HELP_CLOSE(),

		// colormodel
		HTML_HELP_OPEN() \
		HTML_HELP_DEF( "type", "int" ) \
		HTML_HELP_DEF( "values", "[0,1]" ) \
		HTML_HELP_DEF( "default", "0" ) \
		HTML_HELP_BODY() \
		"This value defines the type of color interpolation. Following values are valid :" \
		"<ul><li>0: linear mapping on hue ;</li><li>1: RGB interpolation</li></ul>" \
		HTML_HELP_CLOSE(),
	};
	
	...	
	addParameter<DoubleProperty>("property",paramHelp[0]);
    addParameter<int>("colormodel",paramHelp[1]);
	...

 *
 */
}

#include "cxx/Reflect.cxx"

#endif


