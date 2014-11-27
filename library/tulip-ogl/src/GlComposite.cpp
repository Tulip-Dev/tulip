/**
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
#include <tulip/GlComposite.h>

#include <tulip/GlLayer.h>
#include <tulip/GlScene.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlXMLTools.h>

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
void GlComposite::addLayerParent(GlLayer *layer) {
  layerParents.push_back(layer);

  for(list<GlSimpleEntity *>::iterator it = _sortedElements.begin(); it != _sortedElements.end(); ++it) {
    GlComposite* composite=dynamic_cast<GlComposite*>(*it);

    if(composite)
      composite->addLayerParent(layer);
  }
}
//============================================================
void GlComposite::removeLayerParent(GlLayer *layer) {
  for(vector<GlLayer*>::iterator it=layerParents.begin(); it!=layerParents.end(); ++it) {
    if((*it)==layer) {
      layerParents.erase(it);
      break;
    }
  }

  for(list<GlSimpleEntity *>::iterator it = _sortedElements.begin(); it != _sortedElements.end(); ++it) {
    GlComposite* composite=dynamic_cast<GlComposite*>(*it);

    if(composite)
      composite->removeLayerParent(layer);
  }
}
//============================================================
void GlComposite::reset(bool deleteElems) {
  vector<GlSimpleEntity*> toTreat;

  for(ITM i = elements.begin(); i != elements.end(); ++i) {
    //Push elements to treat in a vector as deleting elements in the loop invalidate the current iterator.
    toTreat.push_back((*i).second);
  }

  for(vector<GlSimpleEntity*>::iterator it = toTreat.begin() ; it != toTreat.end() ; ++it) {
    for(vector<GlLayer*>::iterator it2=layerParents.begin(); it2!=layerParents.end(); ++it2) {
      if((*it2)->getScene())
        (*it2)->getScene()->notifyDeletedEntity(*it);
    }

    if (deleteElems) {
      delete (*it);
    }
    else {
      (*it)->removeParent(this);

      for(vector<GlLayer*>::iterator itLayers=layerParents.begin(); itLayers!=layerParents.end(); ++itLayers) {
        GlComposite *composite=dynamic_cast<GlComposite*>(*it);

        if(composite)
          composite->removeLayerParent(*itLayers);
      }
    }
  }

  elements.clear();
  _sortedElements.clear();

  for(vector<GlLayer*>::iterator it=layerParents.begin(); it!=layerParents.end(); ++it) {
    if((*it)->getScene())
      (*it)->getScene()->notifyModifyLayer((*it)->getName(),(*it));
  }
}
//============================================================
void GlComposite::addGlEntity(GlSimpleEntity *entity, const string &key) {
  assert(entity!=NULL);

  bool doSceneTreatment=false;

  if(elements.find(key)==elements.end()) {
    elements[key] = entity;
    _sortedElements.push_back(entity);
    doSceneTreatment=true;
  }
  else {
    if(elements[key]!=entity) {
      _sortedElements.remove(elements[key]);
      _sortedElements.push_back(entity);
      elements[key] = entity;
      doSceneTreatment=true;
    }
  }

  if(doSceneTreatment) {
    entity->addParent(this);

    GlComposite *composite;
    composite=dynamic_cast<GlComposite*>(entity);

    for(vector<GlLayer*>::iterator it=layerParents.begin(); it!=layerParents.end(); ++it) {
      if(composite)
        composite->addLayerParent(*it);

      if((*it)->getScene())
        (*it)->getScene()->notifyModifyLayer((*it)->getName(),(*it));
    }
  }

  GlGraphComposite *graphComposite=dynamic_cast<GlGraphComposite *>(entity);

  if(graphComposite) {
    for(std::vector<GlLayer *>::iterator it=layerParents.begin(); it!=layerParents.end(); ++it) {
      (*it)->glGraphCompositeAdded(graphComposite);
    }
  }

}
//============================================================
void GlComposite::deleteGlEntity(const string &key,bool informTheEntity) {
  if(elements.count(key)==0)
    return;

  GlSimpleEntity *entity=elements[key];

  if(informTheEntity)
    entity->removeParent(this);

  if(informTheEntity) {
    GlComposite *composite=dynamic_cast<GlComposite *>(entity);

    if(composite) {
      for(vector<GlLayer*>::iterator it=layerParents.begin(); it!=layerParents.end(); ++it) {
        composite->removeLayerParent(*it);
      }
    }
  }

  GlGraphComposite *glGraphComposite=dynamic_cast<GlGraphComposite *>(entity);

  if(glGraphComposite) {
    for(vector<GlLayer*>::iterator it=layerParents.begin(); it!=layerParents.end(); ++it) {
      (*it)->glGraphCompositeRemoved(glGraphComposite);
    }
  }

  _sortedElements.remove(elements[key]);
  elements.erase(key);

  for(vector<GlLayer*>::iterator it=layerParents.begin(); it!=layerParents.end(); ++it) {
    if((*it)->getScene()) {
      (*it)->getScene()->notifyModifyLayer((*it)->getName(),(*it));
      (*it)->getScene()->notifyDeletedEntity(entity);
    }
  }

}
//============================================================
void GlComposite::deleteGlEntity(GlSimpleEntity *entity,bool informTheEntity) {
  for(ITM i = elements.begin(); i != elements.end(); ++i) {
    if(entity == (*i).second) {

      if(informTheEntity) {
        entity->removeParent(this);

        GlComposite *composite=dynamic_cast<GlComposite *>(entity);

        if(composite) {
          for(vector<GlLayer*>::iterator it=layerParents.begin(); it!=layerParents.end(); ++it) {
            composite->removeLayerParent(*it);
          }
        }
      }

      _sortedElements.remove((*i).second);
      elements.erase(i->first);

      for(vector<GlLayer*>::iterator it=layerParents.begin(); it!=layerParents.end(); ++it) {
        if((*it)->getScene())  {
          (*it)->getScene()->notifyModifyLayer((*it)->getName(),(*it));
          (*it)->getScene()->notifyDeletedEntity(entity);
        }
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
  if(!layerParents.empty())
    layerParents[0]->getScene()->notifyModifyEntity(entity);
}
//============================================================
void GlComposite::getXML(string &outString) {
  string name;

  GlXMLTools::createProperty(outString,"type","GlComposite","GlEntity");

  GlXMLTools::beginChildNode(outString);

  for(list<GlSimpleEntity*>::iterator it=_sortedElements.begin(); it!=_sortedElements.end(); ++it) {
    name=findKey(*it);
    GlXMLTools::beginChildNode(outString,"GlEntity");
    GlXMLTools::createProperty(outString,"name",name);
    GlXMLTools::beginDataNode(outString);
    GlXMLTools::getXML(outString,"visible",(*it)->isVisible());
    GlXMLTools::getXML(outString,"stencil",(*it)->getStencil());
    (*it)->getXML(outString);
    GlXMLTools::endDataNode(outString);
    GlXMLTools::endChildNode(outString,"GlEntity");
  }

  GlXMLTools::endChildNode(outString);
}
//============================================================
void GlComposite::setWithXML(const string &inString, unsigned int &currentPosition) {

  string childName=GlXMLTools::enterChildNode(inString,currentPosition);
  assert(childName=="children");

  childName=GlXMLTools::enterChildNode(inString,currentPosition);

  while(childName!="") {

    map<string,string> properties=GlXMLTools::getProperties(inString,currentPosition);

    assert(properties.count("name")!=0);
    assert(properties.count("type")!=0);

    GlSimpleEntity *entity=GlXMLTools::createEntity(properties["type"]);

    if(entity) {
      bool visible;
      int stencil;

      GlXMLTools::enterDataNode(inString,currentPosition);

      GlXMLTools::setWithXML(inString,currentPosition, "visible", visible);
      GlXMLTools::setWithXML(inString,currentPosition, "stencil", stencil);
      entity->setWithXML(inString,currentPosition);
      entity->setVisible(visible);
      entity->setStencil(stencil );
      addGlEntity(entity,properties["name"]);

      GlXMLTools::leaveDataNode(inString,currentPosition);
    }

    GlXMLTools::leaveChildNode(inString,currentPosition,childName);
    childName=GlXMLTools::enterChildNode(inString,currentPosition);
  }

  GlXMLTools::leaveChildNode(inString,currentPosition,"children");
}
}
