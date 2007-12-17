#include <typeinfo>
#include "tulip/GlComposite.h"

#include <tulip/GlLayer.h>
#include <tulip/GlScene.h>

using namespace tlp;
using namespace std;

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
void GlComposite::draw(float lod) {
  list<GlSimpleEntity *>::iterator it;
  for(it = _sortedElements.begin(); it!=_sortedElements.end(); ++it) {
    (*it)->draw(lod);
  }
}
