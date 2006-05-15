#ifndef _PARSER_H_
#define _PARSER_H_

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

#endif
