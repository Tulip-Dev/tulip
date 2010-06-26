/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include "tulip/GlXMLTools.h"

#include <string>

#include <libxml/tree.h>
#include <libxml/parser.h>

#include "tulip/GlSimpleEntity.h"
#include "tulip/Gl2DRect.h"
#include "tulip/GlComposite.h"
#include "tulip/GlBox.h"
#include "tulip/GlCircle.h"
#include "tulip/GlGrid.h"
#include "tulip/GlLabel.h"
#include "tulip/GlLine.h"
#include "tulip/GlPolygon.h"
#include "tulip/GlRect.h"
#include "tulip/GlSphere.h"
#include "tulip/GlCurve.h"
#include "tulip/GlComplexPolygon.h"
#include "tulip/GlConvexHull.h"
#include "tulip/GlQuad.h"

using namespace std;

namespace tlp {

  void GlXMLTools::createDataAndChildrenNodes(xmlNodePtr rootNode,xmlNodePtr &dataNode, xmlNodePtr &childrenNode) {
    xmlNodePtr data=NULL;
    xmlNodePtr children=NULL;
    getDataAndChildrenNodes(rootNode,data,children);

    if(!data)
      dataNode=xmlNewChild(rootNode, NULL, BAD_CAST "data",NULL);
    else
      dataNode=data;
    if(!children)
      childrenNode = xmlNewChild(rootNode, NULL, BAD_CAST "children",NULL);
    else
      childrenNode=children;
  }

  void GlXMLTools::getDataAndChildrenNodes(xmlNodePtr rootNode,xmlNodePtr &dataNode, xmlNodePtr &childrenNode) {
    xmlNodePtr node;
    for (node = rootNode->children; node; node = node->next) {
      if(node->type == XML_ELEMENT_NODE) {
	string name=(char *)node->name;
	if(name=="data") {
	  dataNode=node;
	}else if(name=="children") {
	  childrenNode=node;
	}else{
	  assert(false);
	}
      }
    }
  }

  void GlXMLTools::createDataNode(xmlNodePtr rootNode,xmlNodePtr &dataNode) {
    xmlNodePtr node = NULL;
    getDataNode(rootNode,node);

    if(!node)
      dataNode = xmlNewChild(rootNode, NULL, BAD_CAST "data",NULL);
    else
      dataNode=node;
  }

  void GlXMLTools::createChild(xmlNodePtr rootNode, const string &name, xmlNodePtr &childNode) {
    childNode=xmlNewChild(rootNode, NULL, BAD_CAST name.c_str(),NULL);
  }

  void GlXMLTools::createProperty(xmlNodePtr rootNode, const string &name, const string &value) {
    xmlNewProp(rootNode, BAD_CAST name.c_str(), BAD_CAST value.c_str());
  }

  void GlXMLTools::getDataNode(xmlNodePtr rootNode,xmlNodePtr &dataNode) {
    xmlNodePtr node;
    node = rootNode->children;
    getDataNodeDirectly(node,dataNode);
  }

  void GlXMLTools::getDataNodeDirectly(xmlNodePtr rootNode,xmlNodePtr &dataNode) {
    xmlNodePtr node;
    for (node = rootNode; node; node = node->next) {
      if(node->type == XML_ELEMENT_NODE) {
	string name=(char *)node->name;
	if(name=="data") {
	  dataNode=node;
	  return;
	}
      }
    }
    dataNode=NULL;
  }

  void GlXMLTools::getData(const string &name, xmlNodePtr dataNode, xmlNodePtr &outNode) {
    xmlNodePtr node;
    for (node = dataNode->children; node; node = node->next) {
      if(node->type == XML_ELEMENT_NODE && (char*)(node->name)==name) {
	outNode=node->children;
	return;
      }
    }
    outNode=NULL;
  }

  std::string GlXMLTools::getProperty(const std::string &name, xmlNodePtr node) {
    xmlAttrPtr prop;
    for(prop=node->properties;prop;prop=prop->next) {
      if((char*)prop->name==name) {
	return (char*)prop->children->content;
      }
    }
    return "";
  }

  GlSimpleEntity *GlXMLTools::createEntity(const string &name) {
    if(name=="GlBox"){
      return new GlBox();
    }else if(name=="GlCircle"){
      return new GlCircle();
    }else if(name=="GlComplexPolygon"){
      return new GlComplexPolygon();
    }else if(name=="GlComposite"){
      return new GlComposite();
    }else if(name=="GlConvexHull"){
      return new GlConvexHull;
    }else if(name=="GlCurve") {
      return new GlCurve();
    }else if(name=="GlGraphComposite"){
    }else if(name=="GlGrid"){
      return new GlGrid();
    }else if(name=="GlLabel"){
      return new GlLabel();
    }else if(name=="GlLine"){
      return new GlLine();
    }else if(name=="GlPolygon"){
      return new GlPolygon();
    }else if(name=="GlQuad"){
      return new GlQuad();
    }else if(name=="GlRect"){
      return new GlRect();
    }else if(name=="GlRectTextured"){
      return new Gl2DRect();
    }else if(name=="Gl2DRect"){
      return new Gl2DRect();
    }else if(name=="GlSphere"){
      return new GlSphere();
    }else{
      cout << "Unknow entity type : " << name << ". Can't create it !" << endl;
    }
    return NULL;
  }

  void GlXMLTools::addContent(xmlNodePtr rootNode,const std::string &content) {
    xmlNodeAddContent(rootNode,(xmlChar*)(content.c_str()));
  }

  void GlXMLTools::getContent(xmlNodePtr rootNode,std::string &content) {
    content=(char*)rootNode->content;
  }

}
