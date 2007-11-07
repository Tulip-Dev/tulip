#ifndef _TulipTypeConverter_H
#define _TulipTypeConverter_H

#include <string>
#include <list>
#include "tulip/Color.h"
#include "tulip/Size.h"
#include "tulip/Coord.h"
#include "tulip/Node.h"
#include "tulip/Edge.h"

namespace tlp {

struct PropertyInterface;


/**
 * \addtogroup types
 */ 
/*@{*/

bool stringToColor(std::string &str,Color &col);
bool stringToSize(std::string &str,Size &siz);
bool stringToCoord(std::string &str,Coord &coo);
bool stringToLCoord(std::string &str,std::list<Coord> &lcoo);
bool stringToNodeProperty(PropertyInterface *propertyName,node n,std::string &str);
bool stringToEdgeProperty(PropertyInterface *propertyName,edge e,std::string &str);
bool stringToAllNodeProperty(PropertyInterface *propertyName,std::string &str);
bool stringToAllEdgeProperty(PropertyInterface *propertyName,std::string &str);
std::string nodePropertyToString(PropertyInterface *propertyName,node n);
std::string edgePropertyToString(PropertyInterface *propertyName,edge e);
std::string propertyType(PropertyInterface *propertyName);
std::string propertyNodeDefaultValue(PropertyInterface *propertyName);
std::string propertyEdgeDefaultValue(PropertyInterface *propertyName);

/*@}*/

}
#endif
