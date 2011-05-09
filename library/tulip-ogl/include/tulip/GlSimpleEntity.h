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
#ifndef Tulip_GLSIMPLEENTITY_H
#define Tulip_GLSIMPLEENTITY_H

#include <vector>

#include <tulip/GlEntity.h>
#include <tulip/GlSceneVisitor.h>
#include <tulip/Camera.h>
#include <tulip/GlXMLTools.h>
#include <tulip/BoundingBox.h>

namespace tlp {

  class GlComposite;

  /**
   * Base class for all simple entity (entity who not need GraphInputData
   */
  class TLP_GL_SCOPE GlSimpleEntity : public GlEntity {

  public:

    GlSimpleEntity():visible(true),stencil(0xFFFF),checkByBoundingBoxVisitor(true) {}

    virtual ~GlSimpleEntity();

    /**
     * Draw function
     */
    virtual void draw(float lod,Camera* camera) = 0;

    /**
     * Accept visitor function
     */
    virtual void acceptVisitor(GlSceneVisitor *visitor) {
      visitor->visit(this);
    }

    /**
     * Set if entity is visible
     */
    virtual void setVisible(bool visible);
    /**
     * Return if entity is visible
     */
    bool isVisible() {return visible;}
    /**
     * Set stencil number of the entity
     */
    virtual void setStencil(int stencil) {this->stencil=stencil;}
    /**
     * Return stencil number of entity
     */
    int getStencil() {return stencil;}
    /**
     * Set if the entity is check by boundingbox visitor
     */
    void setCheckByBoundingBoxVisitor(bool check) {checkByBoundingBoxVisitor=check;}
    /**
     * Return if entity is check by boudingbox visitor
     */
    bool isCheckByBoundingBoxVisitor() {return checkByBoundingBoxVisitor;}

    /**
     * Return the entity boundingbox
     */
    virtual BoundingBox getBoundingBox() {return boundingBox;}

    /**
     * Add a layer parent to this entity
     */
    virtual void addLayerParent(GlLayer *) {}

    /**
     * remove a layer parent to this entity
     */
    virtual void removeLayerParent(GlLayer *) {}

    /**
     * Add a parent to this entity
     */
    void addParent(GlComposite *composite);

    /**
     * remove a parent to this entity
     */
    void removeParent(GlComposite *composite);

    /**
     * virtual fucntion : Translate entity of vector translation
     */
    virtual void translate(const Coord &){};

    /**
     * Save the entity in Xml
     */
    virtual void getXML(xmlNodePtr rootNode) =0;

    /**
     * Load entity with Xml
     */
    virtual void setWithXML(xmlNodePtr rootNode) =0;

  protected:

    bool visible;
    int stencil;
    bool checkByBoundingBoxVisitor;

    BoundingBox boundingBox;

    std::vector<GlComposite*> parents;

  };

}

#endif // Tulip_GLSIMPLEENTITY_H
