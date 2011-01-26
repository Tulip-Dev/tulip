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
#ifndef COMPOSITE_H
#define COMPOSITE_H

#include <tulip/tulipconf.h>

#include <map>
#include <list>
#include <string>

#include "tulip/GlSimpleEntity.h"

namespace tlp {

  /**
   * \addtogroup GlEntities
   */
  /*@{*/
  /**
   * \brief GlSimpleEntity used to agregate other GlEntity
   *
   * GlSimpleEntity used to agregate other GlEntity
   * This class provide basic function to manage other entity
   */
  class TLP_GL_SCOPE GlComposite : public GlSimpleEntity {


  public:
    GlComposite(bool deleteComponentsInDestructor=true);
    ~GlComposite();

    /**
     * Add a layer parent to this entity
     */
    virtual void addLayerParent(GlLayer *layer);

    /**
     * Remove a layer parent to this entity
     */
    virtual void removeLayerParent(GlLayer *layer);

    /**
     * Clear the composite, if deleteElems is true, composite's entities are delete
     */
    void reset(bool deleteElems);
    /**
     * Add new entity with name : key.
		 * The composite does not takes the entity's ownership, i.e. it is not its responsibility to delete it.
     */
    void addGlEntity(GlSimpleEntity *entity, const std::string &key);
    /**
     * Remove entity with name : key
     * The entity is not deleted
     */
    void deleteGlEntity(const std::string &key,bool informTheEntity=true);
    /**
     * Remove given entity
     * The entity is not deleted
     */
    void deleteGlEntity(GlSimpleEntity *entity,bool informTheEntity=true);
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
     * Call when a child of the composite is modified
     */
    void notifyModified(GlSimpleEntity *entity);

    /**
     * \attention This function do nothing, GlComposite is a GlSimpleEntity so draw function must be define
     */
    virtual void draw(float,Camera *) {}

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
      //visitor->visit(this);
      for(std::list<GlSimpleEntity*>::iterator it=_sortedElements.begin();it!=_sortedElements.end();++it) {
        if((*it)->isVisible()){

#ifndef NDEBUG
          GlComposite *composite=dynamic_cast<GlComposite*>(*it);
          if(!composite && !(*it)->getBoundingBox().isValid()){
            for(std::map<std::string, GlSimpleEntity*>::iterator itE=elements.begin();itE!=elements.end();++itE){
              if((*itE).second==(*it)){
                std::cerr << "Invalid bounding box for entity : " << (*itE).first << std::endl;
                assert(false);
              }
            }
          }
#endif

          (*it)->acceptVisitor(visitor);
        }
      }
    }

    /**
     * Set if at the destruction of composite, components well be deleted
     */
    void setDeleteComponentsInDestructor(bool deleteComponentsInDestructor){
      this->deleteComponentsInDestructor=deleteComponentsInDestructor;
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
    std::vector<GlLayer *> layerParents;
    bool deleteComponentsInDestructor;
  };
  /*@}*/
}
#endif
