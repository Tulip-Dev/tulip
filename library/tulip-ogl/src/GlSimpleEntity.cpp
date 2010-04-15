#include "tulip/GlSimpleEntity.h"

#include "tulip/GlComposite.h"

using namespace std;

namespace tlp {
  GlSimpleEntity::~GlSimpleEntity(){
    for(std::vector<GlComposite*>::iterator it=parents.begin();it!=parents.end();++it){
      (*it)->deleteGlEntity(this,false);
    }
  }

  void GlSimpleEntity::addParent(GlComposite *composite){
    parents.push_back(composite);
  }

  void GlSimpleEntity::setVisible(bool visible) {
    this->visible=visible;
    for(vector<GlComposite*>::iterator it=parents.begin();it!=parents.end();++it) {
      (*it)->notifyModified(this);
    }
  }
  void GlSimpleEntity::removeParent(GlComposite *composite) {
    for(std::vector<GlComposite*>::iterator it=parents.begin();it!=parents.end();++it){
      if((*it)==composite){
        parents.erase(it);
        return;
      }
    }
  }
}
