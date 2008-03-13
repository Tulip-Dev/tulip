#include "tulip/GlXMLTools.h"

#include <string>

#include "tulip/GlSimpleEntity.h"
#include "tulip/GlRectTextured.h"
#include "tulip/GlComposite.h"
#include "tulip/GlBox.h"
#include "tulip/GlCircle.h"
#include "tulip/GlGrid.h"
#include "tulip/GlLabel.h"
#include "tulip/GlLine.h"
#include "tulip/GlMultiPolygon.h"
#include "tulip/GlPolygon.h"
#include "tulip/GlRect.h"
#include "tulip/GlSphere.h"
#include "tulip/GlCurve.h"

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
    if(name=="GlRectTextured"){
      return new GlRectTextured();
    }else if(name=="GlBox"){
      return new GlBox();
    }else if(name=="GlCircle"){
      return new GlCircle();
    }else if(name=="GlGrid"){
      return new GlGrid();
    }else if(name=="GlLabel"){
      return new GlLabel();
    }else if(name=="GlLine"){
      return new GlLine();
    }else if(name=="GlCurve") {
      return new GlCurve();
    }else if(name=="GlMultiPolygon"){
      return new GlMultiPolygon();
    }else if(name=="GlPolygon"){
      return new GlPolygon();
    }else if(name=="GlRect"){
      return new GlRect();
    }else if(name=="GlSphere"){
      return new GlSphere();
    }else if(name=="GlComposite"){
      return new GlComposite();
    }else if(name=="GlGraphComposite"){
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
