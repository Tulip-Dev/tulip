#ifndef COMPOSITE_H
#define COMPOSITE_H

#include <tulip/tulipconf.h>

#include <map>
#include <list>
#include <string>

#include "tulip/GlSimpleEntity.h"

namespace tlp {

  /** \brief GlSimpleEntity used to agregate other GlEntity 
   *
   * GlSimpleEntity used to agregate other GlEntity
   * This class provide basic function to manage other entity
   */
  class TLP_GL_SCOPE GlComposite : public GlSimpleEntity {

  
  public:
    GlComposite();
    ~GlComposite();

    /**
     * Clear the composite, if deleteElems is true, composite's entities are delete
     */
    void reset(bool deleteElems);
    /**
     * Add new entity with name : key
     */
    void addGlEntity(GlSimpleEntity *entity, const std::string &key);
    /**
     * Delete entity with name : key
     */
    void deleteGlEntity(const std::string &key);
    /**
     * Detele given entity 
     */
    void deleteGlEntity(GlSimpleEntity *entity);
    /**
     * Find name of given entity
     */
    std::string findKey(GlSimpleEntity *entity);
    /**
     * Find entity with name : key
     */
    GlSimpleEntity* findGlEntity(const std::string &key);
    /**
     * Return map of entities in composite
     */
    inline std::map<std::string, GlSimpleEntity*> *
      getDisplays () {
      return &elements;
    }

    /**
     * \attention This function do nothing, GlComposite is a GlSimpleEntity so draw function must be define
     */
    void draw(float lod,Camera *camera) {}

    /**
     * Set stencil number for all composite's children
     */
    virtual void setStencil(int stencil) {
      this->stencil=stencil;
      for(std::list<GlSimpleEntity*>::iterator it=_sortedElements.begin();it!=_sortedElements.end();++it) {
	(*it)->setStencil(stencil);
      }
    }

    /**
     * Function used to visit composite's children
     */
    virtual void acceptVisitor(GlSceneVisitor *visitor) {
      for(std::list<GlSimpleEntity*>::iterator it=_sortedElements.begin();it!=_sortedElements.end();++it) {
	if((*it)->isVisible())
	  (*it)->acceptVisitor(visitor);
      }
    }

    /**
     * translate the composite with children
     */
    virtual void translate(const Coord &mouvement); 

    /**
     * Function to export data in XML
     */
    virtual void getXML(xmlNodePtr rootNode);

    /**
     * Function to set data with XML
     */
    virtual void setWithXML(xmlNodePtr rootNode);

  protected:

    std::map<std::string, GlSimpleEntity*> elements;
    std::list<GlSimpleEntity *> _sortedElements; //necessary to enable ordering of elements (for alpha blending)
  };
}
#endif
