#ifndef _TulipTypeConverter_H
#define _TulipTypeConverter_H

#include <string>
#include <list>
#include "Color.h"
#include "Size.h"
#include "Coord.h"
#include "Node.h"
#include "Edge.h"

struct PProxy;


/**
 * \defgroup types
 */ 
/*@{*/

bool stringToColor(std::string &str,Color &col);
bool stringToSize(std::string &str,Size &siz);
bool stringToCoord(std::string &str,Coord &coo);
bool stringToLCoord(std::string &str,std::list<Coord> &lcoo);
bool stringToNodeProperty(PProxy *propertyName,node n,std::string &str);
bool stringToEdgeProperty(PProxy *propertyName,edge e,std::string &str);
bool stringToAllNodeProperty(PProxy *propertyName,std::string &str);
bool stringToAllEdgeProperty(PProxy *propertyName,std::string &str);
std::string nodePropertyToString(PProxy *propertyName,node n);
std::string edgePropertyToString(PProxy *propertyName,edge e);
std::string propertyType(PProxy *propertyName);
std::string propertyNodeDefaultValue(PProxy *propertyName);
std::string propertyEdgeDefaultValue(PProxy *propertyName);

/*@}*/
#endif
