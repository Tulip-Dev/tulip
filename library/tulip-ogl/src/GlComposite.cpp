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
  GlComposite::GlComposite(bool deleteComponentsInDestructor):deleteComponentsInDestructor(deleteComponentsInDestructor) {
  }
  //============================================================
  GlComposite::~GlComposite() {
    reset(deleteComponentsInDestructor);
  }
  //============================================================
  void GlComposite::addLayerParent(GlLayer *layer){
    layerParents.push_back(layer);

    for(list<GlSimpleEntity *>::iterator it = _sortedElements.begin(); it != _sortedElements.end(); ++it) {
      (*it)->addLayerParent(layer);
    }
  }
  //============================================================
  void GlComposite::removeLayerParent(GlLayer *layer){
    for(vector<GlLayer*>::iterator it=layerParents.begin();it!=layerParents.end();++it){
      if((*it)==layer){
        layerParents.erase(it);
        break;
      }
    }

    for(list<GlSimpleEntity *>::iterator it = _sortedElements.begin(); it != _sortedElements.end(); ++it) {
      (*it)->removeLayerParent(layer);
    }
  }
  //============================================================
  void GlComposite::reset(bool deleteElems) {
	  vector<GlSimpleEntity*> toTreat;
    for(ITM i = elements.begin(); i != elements.end(); ++i){
    	//Push elements to treat in a vector as deleting elements in the loop invalidate the current iterator.
    	toTreat.push_back((*i).second);
    }
    for(vector<GlSimpleEntity*>::iterator it = toTreat.begin() ; it != toTreat.end() ; ++it){
    	if (deleteElems){
    		delete (*it);
    	}else{
    		(*it)->removeParent(this);
    		for(vector<GlLayer*>::iterator itLayers=layerParents.begin();itLayers!=layerParents.end();++itLayers){
    			(*it)->removeLayerParent(*itLayers);
    		}
    	}
    }
    elements.clear();
    _sortedElements.clear();

    for(vector<GlLayer*>::iterator it=layerParents.begin();it!=layerParents.end();++it) {
      if((*it)->getScene())
        (*it)->getScene()->notifyModifyLayer((*it)->getScene(),(*it)->getName(),(*it));
    }
  }
  //============================================================
  void GlComposite::addGlEntity(GlSimpleEntity *entity, const string &key) {
    assert(entity!=NULL);
    if(elements.find(key)==elements.end()) {
      elements[key] = entity;
      _sortedElements.push_back(entity);

      entity->addParent(this);
      for(vector<GlLayer*>::iterator it=layerParents.begin();it!=layerParents.end();++it) {
        entity->addLayerParent(*it);
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
  void GlComposite::deleteGlEntity(const string &key,bool informTheEntity) {
    if(elements.count(key)==0)
      return;

    if(informTheEntity){
      GlSimpleEntity *entity=elements[key];
      if(informTheEntity)
      for(vector<GlLayer*>::iterator it=layerParents.begin();it!=layerParents.end();++it){
        entity->removeLayerParent(*it);
      }
    }

    _sortedElements.remove(elements[key]);
    elements.erase(key);

    for(vector<GlLayer*>::iterator it=layerParents.begin();it!=layerParents.end();++it) {
      if((*it)->getScene())
        (*it)->getScene()->notifyModifyLayer((*it)->getScene(),(*it)->getName(),(*it));
    }
  }
  //============================================================
  void GlComposite::deleteGlEntity(GlSimpleEntity *entity,bool informTheEntity) {
    for(ITM i = elements.begin(); i != elements.end(); ++i) {
      if(entity == (*i).second) {

        if(informTheEntity){
          entity->removeParent(this);
          for(vector<GlLayer*>::iterator it=layerParents.begin();it!=layerParents.end();++it){
            entity->removeLayerParent(*it);
          }
        }

        _sortedElements.remove((*i).second);
        elements.erase(i->first);

        for(vector<GlLayer*>::iterator it=layerParents.begin();it!=layerParents.end();++it) {
          if((*it)->getScene())
            (*it)->getScene()->notifyModifyLayer((*it)->getScene(),(*it)->getName(),(*it));
        }

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
  void GlComposite::notifyModified(GlSimpleEntity *entity) {
    if(layerParents.size()!=0)
      layerParents[0]->getScene()->notifyModifyEntity(layerParents[0]->getScene(),entity);
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
	  //previous version compatibility
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
