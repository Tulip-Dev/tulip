#include "tulip/GlSimpleEntity.h"

#include "tulip/GlComposite.h"

using namespace std;

namespace tlp {
  void GlSimpleEntity::setVisible(bool visible) {
    this->visible=visible;
    for(vector<GlComposite*>::iterator it=parents.begin();it!=parents.end();++it) {
      (*it)->notifyModified(this);
    }
  }
}
