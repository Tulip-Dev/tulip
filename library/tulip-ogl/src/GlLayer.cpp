#include "tulip/GlLayer.h"

#include <iostream>

#include "tulip/Camera.h"
#include "tulip/GlEntity.h"
#include "tulip/GlSceneVisitor.h"
#include "tulip/GlScene.h"

using namespace std;

namespace tlp {

  GlLayer::GlLayer(const std::string& name)
    :name(name),visible(true),scene(0),camera(0){
    composite.addParent(this);
  }

  void GlLayer::acceptVisitor(GlSceneVisitor *visitor) {
    if(composite.isVisible()) {
      visitor->visit(this);
      composite.acceptVisitor(visitor);
    }
  }

  void GlLayer::addGlEntity(GlSimpleEntity *entity,const std::string& name){
    composite.addGlEntity(entity,name);
    if(scene)
      scene->notifyModifyLayer(scene,this->name,this);
  }

  void GlLayer::deleteGlEntity(const std::string &key) {
    composite.deleteGlEntity(key);
    if(scene)
      scene->notifyModifyLayer(scene,this->name,this);
  }

  void GlLayer::deleteGlEntity(GlSimpleEntity *entity) {
    composite.deleteGlEntity(entity);
    if(scene)
      scene->notifyModifyLayer(scene,this->name,this);
  }

  GlSimpleEntity* GlLayer::findGlEntity(const std::string &key) {
    return composite.findGlEntity(key);
  }

  std::map<std::string, GlSimpleEntity*> *GlLayer::getDisplays() {
    return composite.getDisplays();
  }

  void GlLayer::getXML(xmlNodePtr rootNode){
    xmlNodePtr dataNode= NULL;
    xmlNodePtr childrenNode= NULL;
    xmlNodePtr node=NULL;

    GlXMLTools::createDataAndChildrenNodes(rootNode, dataNode, childrenNode);

    GlXMLTools::createChild(dataNode,"camera",node);
    camera.getXML(node);
    GlXMLTools::getXML(dataNode,"visible",visible);

    composite.getXML(childrenNode);

  }

  void GlLayer::setWithXML(xmlNodePtr rootNode){
    xmlNodePtr dataNode= NULL;
    xmlNodePtr childrenNode= NULL;
    xmlNodePtr node= NULL;

    GlXMLTools::getDataAndChildrenNodes(rootNode, dataNode, childrenNode);

    // Parse data
    if(dataNode) {
      GlXMLTools::getData("camera", dataNode, node);
      if(node) {
	camera.setWithXML(node);
      }
      GlXMLTools::setWithXML(dataNode,"visible",visible);
    }

    // Parse children
    if(childrenNode){
      composite.setWithXML(childrenNode);
    }

  }


}
