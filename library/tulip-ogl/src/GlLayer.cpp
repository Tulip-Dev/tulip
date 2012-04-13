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
#include <tulip/GlLayer.h>

#include <tulip/Camera.h>
#include <tulip/GlSceneVisitor.h>
#include <tulip/GlScene.h>

using namespace std;

namespace tlp {

GlLayer::GlLayer(const std::string& name,bool workingLayer)
  :name(name),scene(0),camera(new Camera(0)),sharedCamera(false),workingLayer(workingLayer) {
  composite.addLayerParent(this);
}

GlLayer::GlLayer(const std::string& name,Camera *camera,bool workingLayer)
  :name(name),scene(0),camera(camera),sharedCamera(true),workingLayer(workingLayer) {
  composite.addLayerParent(this);
}

GlLayer::~GlLayer() {
  if(!sharedCamera)
    delete camera;
}

void GlLayer::setCamera(Camera* camera) {
  Camera *oldCamera=this->camera;
  this->camera = camera;

  if (!sharedCamera)
    delete oldCamera;

  sharedCamera = false;
}

void GlLayer::setSharedCamera(Camera *camera) {
  Camera *oldCamera=this->camera;
  this->camera = camera;

  if (!sharedCamera)
    delete oldCamera;

  sharedCamera = true;
}

void GlLayer::set2DMode() {
  Camera *oldCamera=this->camera;
  this->camera = new Camera(oldCamera->getScene(),false);

  if (!sharedCamera)
    delete oldCamera;

  sharedCamera = false;
}

void GlLayer::setVisible(bool visible) {
  if(composite.isVisible()==visible)
    return;

  composite.setVisible(visible);

  if(scene)
    scene->notifyModifyLayer(this->name,this);
}

void GlLayer::acceptVisitor(GlSceneVisitor *visitor) {
  if(composite.isVisible()) {
    visitor->visit(this);
    composite.acceptVisitor(visitor);
  }
}

void GlLayer::addGlEntity(GlSimpleEntity *entity,const std::string& name) {
  composite.addGlEntity(entity,name);

  if(scene)
    scene->notifyModifyLayer(this->name,this);
}

void GlLayer::deleteGlEntity(const std::string &key) {
  composite.deleteGlEntity(key);

  if(scene)
    scene->notifyModifyLayer(this->name,this);
}

void GlLayer::deleteGlEntity(GlSimpleEntity *entity) {
  composite.deleteGlEntity(entity);

  if(scene)
    scene->notifyModifyLayer(this->name,this);
}

GlSimpleEntity* GlLayer::findGlEntity(const std::string &key) {
  return composite.findGlEntity(key);
}

const std::map<std::string, GlSimpleEntity*> &GlLayer::getGlEntities() const {
  return composite.getGlEntities();
}

void GlLayer::getXML(string &outString) {
  GlXMLTools::beginDataNode(outString);

  GlXMLTools::beginChildNode(outString,"camera");
  camera->getXML(outString);
  GlXMLTools::endChildNode(outString,"camera");

  bool visible=composite.isVisible();
  GlXMLTools::getXML(outString,"visible",visible);

  GlXMLTools::endDataNode(outString);

  GlXMLTools::beginChildNode(outString,"GlEntity");
  composite.getXML(outString);
  GlXMLTools::endChildNode(outString);

}

void GlLayer::setWithXML(const string &inString, unsigned int &currentPosition) {

  GlXMLTools::enterDataNode(inString,currentPosition);
  string childName=GlXMLTools::enterChildNode(inString,currentPosition);
  assert(childName=="camera");
  camera->setWithXML(inString,currentPosition);
  GlXMLTools::leaveChildNode(inString,currentPosition,"camera");

  bool visible;
  GlXMLTools::setWithXML(inString,currentPosition,"visible",visible);
  composite.setVisible(visible);

  GlXMLTools::leaveDataNode(inString,currentPosition);

  childName=GlXMLTools::enterChildNode(inString,currentPosition);
  assert(childName=="GlEntity");

  map<string,string> childMap=GlXMLTools::getProperties(inString,currentPosition);
  assert(childMap["type"]=="GlComposite");
  composite.setWithXML(inString,currentPosition);

  GlXMLTools::leaveChildNode(inString,currentPosition,"children");
  /*xmlNodePtr dataNode= NULL;
  xmlNodePtr childrenNode= NULL;
  xmlNodePtr node= NULL;

  GlXMLTools::getDataAndChildrenNodes(rootNode, dataNode, childrenNode);

  // Parse data
  if(dataNode) {
    GlXMLTools::getData("camera", dataNode, node);

    if(node) {
      camera->setWithXML(node);
    }

    bool visible;
    GlXMLTools::setWithXML(dataNode,"visible",visible);
    composite.setVisible(visible);
  }

  // Parse children
  if(childrenNode) {
    composite.setWithXML(childrenNode);
  }*/

}

void GlLayer::glGraphCompositeAdded(GlGraphComposite *composite) {
  assert(scene);
  scene->glGraphCompositeAdded(this,composite);
}

void GlLayer::glGraphCompositeRemoved(GlGraphComposite *composite) {
  assert(scene);
  scene->glGraphCompositeRemoved(this,composite);
}



}
