#include <typeinfo>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include "tulip/GlComposite.h"

#include <tulip/GlLayer.h>
#include <tulip/GlScene.h>

using namespace std;

namespace tlp {
  typedef std::map<string, GlSimpleEntity *>::const_iterator ITM;
  //============================================================
  GlComposite::GlComposite() {
  }
  //============================================================
  GlComposite::~GlComposite() {
    reset(false);
  }
  //============================================================
  void GlComposite::reset(bool deleteElems) {
    if (deleteElems)
      for(ITM i = elements.begin(); i != elements.end(); ++i)
	delete (*i).second;
    elements.clear();
    _sortedElements.clear();
  }
  //============================================================
  void GlComposite::addGlEntity(GlSimpleEntity *entity, const string &key) {
    if(elements.find(key)==elements.end()) {
      elements[key] = entity;
      _sortedElements.push_back(entity);

      for(vector<GlLayer*>::iterator it=parents.begin();it!=parents.end();++it) {
	entity->addParent(*it);
	if((*it)->getScene())
	  (*it)->getScene()->notifyModifyLayer((*it)->getScene(),(*it)->getName(),(*it));
      }
    }else{
      if(elements[key]!=entity) {
	_sortedElements.remove(elements[key]);
	_sortedElements.push_back(entity);
	elements[key] = entity;
      }
    }
  }
  //============================================================
  void GlComposite::deleteGlEntity(const string &key) {
    _sortedElements.remove(elements[key]);
    elements.erase(key);
  }
  //============================================================
  void GlComposite::deleteGlEntity(GlSimpleEntity *entity) {
    for(ITM i = elements.begin(); i != elements.end(); ++i) {
      if(entity == (*i).second) {
	_sortedElements.remove((*i).second);
	elements.erase(i->first);
	return;
      }
    }
  }
  //============================================================
  string GlComposite::findKey(GlSimpleEntity *entity) {
    for(ITM it = elements.begin(); it != elements.end(); ++it) {
      if(entity == (*it).second) {
	return it->first;
      }
    }
    return string("");
  }
  //============================================================
  GlSimpleEntity* GlComposite::findGlEntity(const string &key) {
    ITM ite = elements.find(key);
    if (ite == elements.end())
      return NULL;
    return (*ite).second;
  }
  //============================================================
  void GlComposite::translate(const Coord &mouvement) {
    for(ITM it = elements.begin(); it != elements.end(); ++it) {
      (*it).second->translate(mouvement);
    }
  }
  //============================================================
  void GlComposite::getXML(xmlNodePtr rootNode) {
    xmlNodePtr node=NULL;
    xmlNodePtr dataNode=NULL;
    xmlNodePtr childrenNode=NULL;

    string name;

    GlXMLTools::createProperty(rootNode,"type","GlComposite");

    GlXMLTools::createDataAndChildrenNodes(rootNode, dataNode, childrenNode);

    for(list<GlSimpleEntity*>::iterator it=_sortedElements.begin();it!=_sortedElements.end();++it) {
      name=findKey(*it);
      GlXMLTools::createChild(childrenNode,"GlEntity",node);
      GlXMLTools::createProperty(node,"name",name);
      GlXMLTools::createDataNode(node,dataNode);
      GlXMLTools::getXML(dataNode,"visible",(*it)->isVisible());
      GlXMLTools::getXML(dataNode,"stencil",(*it)->getStencil());
      (*it)->getXML(node);
    }
  }
  //============================================================
  void GlComposite::setWithXML(xmlNodePtr rootNode) {
    xmlNodePtr node=NULL;
    xmlNodePtr dataNode=NULL;
    xmlNodePtr childrenNode=NULL;

    GlXMLTools::getDataAndChildrenNodes(rootNode,dataNode,childrenNode);

    // Parse Children
    for (node = childrenNode->children; node; node = node->next) {
      if(node->type == XML_ELEMENT_NODE) {
	string type = GlXMLTools::getProperty("type",node);
	string name;

	if(string((char*)node->name)=="GlEntity") {
	  //New version
	  name = GlXMLTools::getProperty("name",node);
	}else{
	  //previous version comglStencilFunc(GL_LEQUAL,0x0002,0xFFFF);patibility
	  name=(char*)node->name;
	}
	if(type!="") {
	  GlSimpleEntity *entity=GlXMLTools::createEntity(type);
	  if(entity) {
	    bool visible;
	    int stencil;

	    entity->setWithXML(node);
	    GlXMLTools::getDataNode(node,dataNode);

	    GlXMLTools::setWithXML(dataNode, "visible", visible);
	    GlXMLTools::setWithXML(dataNode, "stencil", stencil);
	    entity->setVisible(visible);
	    entity->setStencil(stencil );
	    addGlEntity(entity,name);
	    }
	  }
      }
    }
  }
}
