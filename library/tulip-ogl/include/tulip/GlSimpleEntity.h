/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#include <tulip/Coord.h>
#include <tulip/GlSceneVisitor.h>
#include <tulip/BoundingBox.h>

namespace tlp {

class GlComposite;
class Camera;

/**
 * @ingroup OpenGL
 * @brief Base class for all Tulip OpenGL entities
 *
 * Other Tulip entities inherit for this class.
 *
 * You don't have to create a GlSimpleEntity, you have to use GlLine, GlRect or GlSphere for example
 * @see Gl2DRect
 * @see GlPolygon
 * @see GlAxis
 * @see GlBezierCurve
 * @see GlBox
 * @see GlCatmullRomCurve
 * @see GlCircle
 * @see GlComplexPolygon
 * @see GlGrid
 * @see GlHexagon
 * @see GlLabel
 * @see GlSphere
 * @see GlPentagon
 * @see GlTriangle
 * @see GlOpenUniformCubicBSpline
 *
 * To GlSimpleEntity manipulation :
 * @see GlLayer
 * @see GlScene
 */
class TLP_GL_SCOPE GlSimpleEntity : public GlEntity {

public:

  /**
   * @brief Constructor
   */
  GlSimpleEntity():visible(true),stencil(0xFFFF) {}

  /**
   * @brief Destructor
   */
  virtual ~GlSimpleEntity();

  /**
   * @brief Set if entity is visible
   */
  virtual void setVisible(bool visible);
  /**
   * @brief Return if entity is visible
   */
  bool isVisible() const {
    return visible;
  }
  /**
   * @brief Set stencil number of the entity
   *
   * Stencil is an OpenGl system to ensure that other entity can't be displayed above this entity; it's a "guaranted visibility" system.
   * A small number causes a guaranted visibility
   * Default value in Tulip is 0xFFFF (greater integer)
   * And when we have stencil on entity value is 0x2
   */
  virtual void setStencil(int stencil) {
    this->stencil=stencil;
  }
  /**
   * @brief Return stencil number of entity
   *
   * @see setStencil()
   */
  int getStencil() {
    return stencil;
  }

  /**
   * @brief Draw function
   *
   * @warning You don't have to call this function, the Tulip OpenGL engine call it.
   */
  virtual void draw(float lod,Camera* camera) = 0;

  /**
   * @brief Return the entity boundingbox
   *
   * @warning You don't have to call this function, the Tulip OpenGL engine call it.
   */
  virtual BoundingBox getBoundingBox() {
    return boundingBox;
  }

  /**
   * @brief Save the entity in outString (in XML format)
   *
   * @warning You don't have to call this function, the Tulip OpenGL engine call it.
   */
  virtual void getXML(std::string &outString) =0;

  /**
   * @brief Load entity with inString (in XML format)
   *
   * @warning You don't have to call this function, the Tulip OpenGL engine call it.
   */
  virtual void setWithXML(const std::string &inString, unsigned int &currentPosition) =0;

///@cond DOXYGEN_HIDDEN

  /**
   * @brief Accept visitor function
   */
  virtual void acceptVisitor(GlSceneVisitor *visitor) {
    visitor->visit(this);
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
   * virtual fucntion : Translate entity of vector translation
   */
  virtual void translate(const Coord &) {}

  GlComposite* getParent() const {
    if (parents.empty())
      return NULL;

    return parents[0];
  }

///@endcond

protected:

  bool visible;
  int stencil;

  BoundingBox boundingBox;

  std::vector<GlComposite*> parents;

};

}

#endif // Tulip_GLSIMPLEENTITY_H

