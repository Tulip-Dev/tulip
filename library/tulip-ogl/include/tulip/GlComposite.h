#ifndef COMPOSITE_H
#define COMPOSITE_H

#include <tulip/tulipconf.h>

#include <map>
#include <list>
#include <string>

#include "tulip/GlSimpleEntity.h"

namespace tlp {
  class TLP_GL_SCOPE GlComposite : public GlSimpleEntity {

  
  public:
    GlComposite();
    ~GlComposite();

    void reset(bool deleteElems);
    void addGlEntity(GlSimpleEntity *entity, const std::string &key);
    void deleteGlEntity(const std::string &key);
    void deleteGlEntity(GlSimpleEntity *entity);
    std::string findKey(GlSimpleEntity *entity);
    GlSimpleEntity* findGlEntity(const std::string &key);
    inline std::map<std::string, GlSimpleEntity*> *
      getDisplays () {
      return &elements;
    }
    inline void clear() {elements.clear();_sortedElements.clear();}

    void draw(float lod);

    virtual void setStencil(int stencil) {
      this->stencil=stencil;
      for(std::list<GlSimpleEntity*>::iterator it=_sortedElements.begin();it!=_sortedElements.end();++it) {
	(*it)->setStencil(stencil);
      }
    }

    virtual BoundingBox getBoundingBox() {return BoundingBox();} 

    virtual void acceptVisitor(GlSceneVisitor *visitor) {
      for(std::list<GlSimpleEntity*>::iterator it=_sortedElements.begin();it!=_sortedElements.end();++it) {
	if((*it)->isVisible())
	  (*it)->acceptVisitor(visitor);
      }
    }

  protected:

    std::map<std::string, GlSimpleEntity*> elements;
    std::list<GlSimpleEntity *> _sortedElements; //necessary to enable ordering of elements (for alpha blending)
  };
}
#endif
