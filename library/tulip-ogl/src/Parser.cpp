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
#include "tulip/Parser.h"
#include <cassert>
#include <string.h>

using namespace std;
using namespace tlp;

Parser::Parser(const char* str){
  doc = xmlParseMemory(str,strlen(str));
  assert(doc);
  head = doc->children;
}

Parser::~Parser(){
  if(doc){
    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();
  }
}

xmlNodePtr Parser::Child(xmlNodePtr parent){
  if(parent)  return parent->children;
  else  return 0;
}

xmlNodePtr Parser::Next(xmlNodePtr element){
  if(element) return element->next;
  else return 0;
}

xmlNodePtr Parser::Prev(xmlNodePtr element){
  if(element) return element->prev;
  else return 0;
}

xmlNodePtr Parser::Parent(xmlNodePtr element){
  if(element) return element->parent;
  else return 0;
}
unsigned char* Parser::getName(xmlNodePtr element){
  if(element) return (unsigned char*)(element->name);
  else return 0;
}

int Parser::getType(xmlNodePtr element){
  if(element) return (int)(element->type);
  else return 0;
}

unsigned char* Parser::getContent(xmlNodePtr element){
  if(element) return (unsigned char*)(element->content);
  else return 0;
}

xmlAttr* Parser::Attribut(xmlNodePtr element){
  if(element) return element->properties;
  else return 0;
}
xmlNodePtr Parser::Value(xmlAttr* attribut){
  if(attribut) return attribut->children;
  else return 0;
}
xmlAttr* Parser::Next(xmlAttr* attribut){
  if(attribut) return attribut->next;
  else return 0;
}

unsigned char* Parser::getName(xmlAttr* attribut){
  if(attribut) return (unsigned char*)attribut->name;
  else return 0;
}
