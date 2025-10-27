/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
GlComposite::GlComposite(bool deleteComponentsInDestructor)
    : deleteComponentsInDestructor(deleteComponentsInDestructor) {}
//============================================================
GlComposite::~GlComposite() {
  reset(deleteComponentsInDestructor);
}
//============================================================
void GlComposite::addLayerParent(GlLayer *layer) {
  layerParents.push_back(layer);

  for (auto elt : _sortedElements) {
    GlComposite *composite = dynamic_cast<GlComposite *>(elt);

    if (composite)
      composite->addLayerParent(layer);
  }
}
//============================================================
void GlComposite::removeLayerParent(GlLayer *layer) {
  for (vector<GlLayer *>::iterator it = layerParents.begin(); it != layerParents.end(); ++it) {
    if ((*it) == layer) {
      layerParents.erase(it);
      break;
    }
  }

  for (auto elt : _sortedElements) {
    GlComposite *composite = dynamic_cast<GlComposite *>(elt);

    if (composite)
      composite->removeLayerParent(layer);
  }
}
//============================================================
void GlComposite::reset(bool deleteElems) {
  vector<GlSimpleEntity *> toTreat;

  for (ITM i = elements.begin(); i != elements.end(); ++i) {
    // Push elements to treat in a vector as deleting elements in the loop invalidate the current
    // iterator.
    toTreat.push_back(i->second);
  }

  for (auto entity : toTreat) {
    for (auto parent : layerParents) {
      if (parent->getScene())
        parent->getScene()->notifyDeletedEntity(entity);
    }

    entity->removeParent(this);

    for (auto parent : layerParents) {
      GlComposite *composite = dynamic_cast<GlComposite *>(entity);

      if (composite)
        composite->removeLayerParent(parent);
    }

    if (deleteElems)
      delete entity;
  }

  elements.clear();
  _sortedElements.clear();

  for (auto parent : layerParents) {
    if (parent->getScene())
      parent->getScene()->notifyModifyLayer(parent->getName(), parent);
  }
}
//============================================================
void GlComposite::addGlEntity(GlSimpleEntity *entity, const string &key) {
  assert(entity != nullptr);

  bool doSceneTreatment = false;

  if (elements.find(key) == elements.end()) {
    elements[key] = entity;
    _sortedElements.push_back(entity);
    doSceneTreatment = true;
  } else {
    if (elements[key] != entity) {
      _sortedElements.remove(elements[key]);
      _sortedElements.push_back(entity);
      elements[key] = entity;
      doSceneTreatment = true;
    }
  }

  if (doSceneTreatment) {
    entity->addParent(this);

    GlComposite *composite;
    composite = dynamic_cast<GlComposite *>(entity);

    for (auto parent : layerParents) {
      if (composite)
        composite->addLayerParent(parent);

      if (parent->getScene())
        parent->getScene()->notifyModifyLayer(parent->getName(), parent);
    }
  }

  GlGraphComposite *graphComposite = dynamic_cast<GlGraphComposite *>(entity);

  if (graphComposite) {
    for (auto parent : layerParents) {
      parent->glGraphCompositeAdded(graphComposite);
    }
  }
}
//============================================================
void GlComposite::deleteGlEntity(const string &key, bool informTheEntity) {
  if (elements.count(key) == 0)
    return;

  GlSimpleEntity *entity = elements[key];

  if (informTheEntity)
    entity->removeParent(this);

  if (informTheEntity) {
    GlComposite *composite = dynamic_cast<GlComposite *>(entity);

    if (composite) {
      for (auto parent : layerParents) {
        composite->removeLayerParent(parent);
      }
    }
  }

  GlGraphComposite *glGraphComposite = dynamic_cast<GlGraphComposite *>(entity);

  if (glGraphComposite) {
    for (auto parent : layerParents) {
      parent->glGraphCompositeRemoved(glGraphComposite);
    }
  }

  _sortedElements.remove(elements[key]);
  elements.erase(key);

  for (auto parent : layerParents) {
    if (parent->getScene()) {
      parent->getScene()->notifyModifyLayer(parent->getName(), parent);
      parent->getScene()->notifyDeletedEntity(entity);
    }
  }
}
//============================================================
void GlComposite::deleteGlEntity(GlSimpleEntity *entity, bool informTheEntity) {
  for (ITM i = elements.begin(); i != elements.end(); ++i) {
    if (entity == (*i).second) {

      if (informTheEntity) {
        entity->removeParent(this);

        GlComposite *composite = dynamic_cast<GlComposite *>(entity);

        if (composite) {
          for (auto parent : layerParents) {
            composite->removeLayerParent(parent);
          }
        }
      }

      _sortedElements.remove((*i).second);
      elements.erase(i->first);

      for (auto parent : layerParents) {
        if (parent->getScene()) {
          parent->getScene()->notifyModifyLayer(parent->getName(), parent);
          parent->getScene()->notifyDeletedEntity(entity);
        }
      }

      return;
    }
  }
}
//============================================================
string GlComposite::findKey(GlSimpleEntity *entity) {
  for (ITM it = elements.begin(); it != elements.end(); ++it) {
    if (entity == (*it).second) {
      return it->first;
    }
  }

  return string("");
}
//============================================================
GlSimpleEntity *GlComposite::findGlEntity(const string &key) {
  ITM ite = elements.find(key);

  if (ite == elements.end())
    return nullptr;

  return (*ite).second;
}
//============================================================
void GlComposite::translate(const Coord &mouvement) {
  for (ITM it = elements.begin(); it != elements.end(); ++it) {
    (*it).second->translate(mouvement);
  }
}
//============================================================
void GlComposite::notifyModified(GlSimpleEntity *entity) {
  if (!layerParents.empty())
    layerParents[0]->getScene()->notifyModifyEntity(entity);
}
//============================================================
void GlComposite::getXML(string &outString) {
  string name;

  GlXMLTools::createProperty(outString, "type", "GlComposite", "GlEntity");

  GlXMLTools::beginChildNode(outString);

  for (auto elt : _sortedElements) {
    name = findKey(elt);
    GlXMLTools::beginChildNode(outString, "GlEntity");
    GlXMLTools::createProperty(outString, "name", name);
    GlXMLTools::beginDataNode(outString);
    GlXMLTools::getXML(outString, "visible", elt->isVisible());
    GlXMLTools::getXML(outString, "stencil", elt->getStencil());
    elt->getXML(outString);
    GlXMLTools::endDataNode(outString);
    GlXMLTools::endChildNode(outString, "GlEntity");
  }

  GlXMLTools::endChildNode(outString);
}
//============================================================
void GlComposite::setWithXML(const string &inString, unsigned int &currentPosition) {

  string childName = GlXMLTools::enterChildNode(inString, currentPosition);
  assert(childName == "children");

  childName = GlXMLTools::enterChildNode(inString, currentPosition);

  while (!childName.empty()) {

    map<string, string> properties = GlXMLTools::getProperties(inString, currentPosition);

    assert(properties.count("name") != 0);
    assert(properties.count("type") != 0);

    GlSimpleEntity *entity = GlXMLTools::createEntity(properties["type"]);

    if (entity) {
      bool visible;
      int stencil;

      GlXMLTools::enterDataNode(inString, currentPosition);

      GlXMLTools::setWithXML(inString, currentPosition, "visible", visible);
      GlXMLTools::setWithXML(inString, currentPosition, "stencil", stencil);
      entity->setWithXML(inString, currentPosition);
      entity->setVisible(visible);
      entity->setStencil(stencil);
      addGlEntity(entity, properties["name"]);

      GlXMLTools::leaveDataNode(inString, currentPosition);
    }

    GlXMLTools::leaveChildNode(inString, currentPosition, childName);
    childName = GlXMLTools::enterChildNode(inString, currentPosition);
  }

  GlXMLTools::leaveChildNode(inString, currentPosition, "children");
}
} // namespace tlp
