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

  GlSimpleEntity():visible(true),stencil(0xFFFF) {}

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
  bool isVisible() const {
    return visible;
  }
  /**
   * Set stencil number of the entity
   * Stencil is an OpenGl system to ensure that other entity can't be displayed above this entity; it's a "guaranted visibility" system.
   * A small number causes a guaranted visibility
   * Default value in Tulip is 0xFFFF (greater integer)
   * And when we have stencil on entity value is 0x2
   */
  virtual void setStencil(int stencil) {
    this->stencil=stencil;
  }
  /**
   * Return stencil number of entity
   */
  int getStencil() {
    return stencil;
  }

  /**
   * Return the entity boundingbox
   */
  virtual BoundingBox getBoundingBox() {
    return boundingBox;
  }

  /**
   * Add a parent to this entity
   */
  void addParent(GlComposite *composite);

  /**
   * remove a parent to this entity
   */
  void removeParent(GlComposite *composite);

  /**
   * @brief Return properties names for this entity
   * These properties names are used to dynamically configure the entity
   * for example these function can be used by Mouse information interactor
   * If you create a class that inherits of GlSimpleEntity : you can reimplement this function to return your properties names
   * for example : return QStringList() << "fillColor" << "outlineColor";
   * @return QList of properties names
   */
  virtual QStringList propertiesNames() const;

  /**
   * @brief Return properties (in  QVariant format) for this entity
   * These properties QVariant are used to dynamically configure the entity
   * for example these function can be used by Mouse information interactor
   * If you create a class that inherits of GlSimpleEntity : you can reimplement this function to return your properties
   * for example : return QVariantList() << QVariant::fromValue<Color>(getFillColor()) << QVariant::fromValue<Color>(getOutlineColor());
   * @return QList of properties (in QVariant format)
   */
  virtual QVariantList propertiesQVariant() const;

  /**
   * @brief Set value for a property previously returned by propertiesNames() and properties() functions
   * This function is call when we want to set value of a property
   * this parameter is returned in list by propertiesNames() and properties funtions
   * If you create a class that inherits of GlSimpleEntity : you can reimplement this function to set your properties
   * For example :
   * if(name=="fillColor")
   *   setFillColor(value.value<Color>());
   * else if(name=="outlineColor")
   *   setOutlineColor(value.value<Color>());
   */
  virtual void setProperty(const QString &name, const QVariant &value);

  /**
   * virtual fucntion : Translate entity of vector translation
   */
  virtual void translate(const Coord &) {}

  /**
   * Save the entity in outString (in XML format)
   */
  virtual void getXML(std::string &outString) =0;

  /**
   * Load entity with inString (in XML format)
   */
  virtual void setWithXML(const std::string &inString, unsigned int &currentPosition) =0;

  GlComposite* getParent() const {
    if (parents.size()==0)
      return NULL;

    return parents[0];
  }

protected:

  bool visible;
  int stencil;

  BoundingBox boundingBox;

  std::vector<GlComposite*> parents;

};

}

#endif // Tulip_GLSIMPLEENTITY_H
