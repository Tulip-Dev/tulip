//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 23/01/2008
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_GLXMLTOOLS_H
#define Tulip_GLXMLTOOLS_H

#include <tulip/tulipconf.h>

#include <sstream>
#include <vector>

/*#include <libxml/tree.h>
  #include <libxml/parser.h>*/

#include <tulip/Array.h>
#include <tulip/Color.h>
#include <tulip/Coord.h>

typedef struct _xmlNode xmlNode;
typedef xmlNode * xmlNodePtr;

namespace tlp {

  class GlSimpleEntity;

  class TLP_GL_SCOPE GlXMLTools {

  public :

    static void createDataAndChildrenNodes(xmlNodePtr rootNode,xmlNodePtr &dataNode, xmlNodePtr &childrenNode);
    static void createDataNode(xmlNodePtr rootNode,xmlNodePtr &dataNode);
    static void createChild(xmlNodePtr rootNode, const std::string &name, xmlNodePtr &childNode);
    static void createProperty(xmlNodePtr rootNode, const std::string &name, const std::string &value);
    static void addContent(xmlNodePtr rootNode,const std::string &content);
    static void getDataAndChildrenNodes(xmlNodePtr rootNode,xmlNodePtr &dataNode, xmlNodePtr &childrenNode);
    static void getDataNode(xmlNodePtr rootNode,xmlNodePtr &dataNode);
    static void getDataNodeDirectly(xmlNodePtr rootNode,xmlNodePtr &dataNode);
    static void getData(const std::string &name, xmlNodePtr dataNode, xmlNodePtr &outNode);
    static std::string getProperty(const std::string &name, xmlNodePtr node);
    static void getContent(xmlNodePtr rootNode,std::string &content);
    static GlSimpleEntity *createEntity(const std::string &name);

    template <typename Obj>
      static void getXML(xmlNodePtr rootNode,const std::string &name,const typename std::vector<Obj> &vect)
    {
      xmlNodePtr node;
      createChild(rootNode,name,node);
    
      std::stringstream str;
      str << "(" ;
      typename std::vector<Obj>::const_iterator it=vect.begin();
      str << *it ;
      for(;it!=vect.end();++it) {
	str << "," << *it ;
      }
      str << ")" ;
      addContent(node,str.str());
    }
    
    template <typename Obj>
      static void setWithXML(xmlNodePtr rootNode,const std::string &name,std::vector<Obj> &vect)
    {
      xmlNodePtr node;
      GlXMLTools::getData(name, rootNode, node);
      std::string tmp;
      getContent(node,tmp);
      std::istringstream is(tmp);
      Obj data;
      char c=is.get();
      while(c!=')'){
	is >> data ;
	vect.push_back(data);
	c=is.get();
      }
    }
    
    template <typename Obj>
      static void getXML(xmlNodePtr rootNode, const std::string &name, const Obj &value)
    {
      xmlNodePtr node;
      createChild(rootNode,name,node);
      std::stringstream str;
      str << value ;
      addContent(node,str.str());
    }
    
    template <typename Obj>
      static void setWithXML(xmlNodePtr rootNode, const std::string &name, Obj &value) {
      xmlNodePtr node;
      GlXMLTools::getData(name, rootNode, node);
      std::string tmp;
      getContent(node,tmp);
      std::stringstream str(tmp);
      str >> value;
    }

  };

}

#endif
