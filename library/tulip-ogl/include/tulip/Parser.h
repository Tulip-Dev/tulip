/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#ifndef _PARSER_H_
#define _PARSER_H_
#ifndef DOXYGEN_NOTFOR_DEVEL

#include <libxml/parser.h>
#include <libxml/tree.h>

namespace tlp {

class Parser{

 private:
  xmlDocPtr doc;
  xmlNodePtr head;

 public:
  Parser(){doc = 0; head = 0;}
  Parser(const char* str);
  ~Parser();
  xmlNodePtr Child(xmlNodePtr parent);
  xmlNodePtr Next(xmlNodePtr element);
  xmlNodePtr Prev(xmlNodePtr element);
  xmlNodePtr Parent(xmlNodePtr element);
  unsigned char* getName(xmlNodePtr element);
  int getType(xmlNodePtr element);  
  unsigned char* getContent(xmlNodePtr element);
  xmlAttr* Attribut(xmlNodePtr element);
  xmlNodePtr getHead(){return head;}

  xmlNodePtr Value(xmlAttr* attribut);
  xmlAttr* Next(xmlAttr* attribut);
  unsigned char* getName(xmlAttr* attribut);
};

}

#endif //DOXYGEN_NOTFOR_DEVEL
#endif
