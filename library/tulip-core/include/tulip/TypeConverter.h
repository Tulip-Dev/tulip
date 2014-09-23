/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
///@cond DOXYGEN_HIDDEN

#ifndef _TulipTypeConverter_H
#define _TulipTypeConverter_H

#include <string>
#include <list>
#include <tulip/Color.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Node.h>
#include <tulip/Edge.h>

namespace tlp {

struct PropertyInterface;

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



}
#endif
///@endcond
