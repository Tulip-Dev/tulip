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
    void addGlEntity(GlEntity *entity, const std::string &key);
    void deleteGlEntity(const std::string &key);
    void deleteGlEntity(GlEntity *entity);
    std::string findKey(GlEntity *entity);
    GlEntity* findGlEntity(const std::string &key);
    inline std::map<std::string, GlEntity*> *
      getDisplays () {
      return &elements;
    }
    inline void clear() {elements.clear();_sortedElements.clear();}

    void draw(float lod);

    virtual BoundingBox getBoundingBox() {return BoundingBox();} 

    virtual void acceptVisitor(GlSceneVisitor *visitor) {
      for(std::list<GlEntity*>::iterator it=_sortedElements.begin();it!=_sortedElements.end();++it) {
	(*it)->acceptVisitor(visitor);
      }
    }

  protected:
    std::map<std::string, GlEntity*> elements;
    std::list<GlEntity *> _sortedElements; //necessary to enable ordering of elements (for alpha blending)
  };
}
#endif
