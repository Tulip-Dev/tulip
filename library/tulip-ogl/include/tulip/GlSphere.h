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

#ifndef Tulip_GLSPHERE_H
#define Tulip_GLSPHERE_H

#include <tulip/tulipconf.h>

#include <tulip/Coord.h>
#include <tulip/Color.h>

#include <tulip/GlSimpleEntity.h>

namespace tlp {

/**
 * @ingroup OpenGL
 * @brief Class to create a sphere with GlEntity system
 */
class TLP_GL_SCOPE GlSphere : public GlSimpleEntity {

public:

  /**
   * @brief Constructor
   *
   * @warning Don't use this constructor
   */
  GlSphere() {}

  /**
   * @brief Create a sphere with a position, a radius a fill color and multiple rotation (if you want)
   */
  GlSphere(const Coord &position,float radius,const Color &color=Color(0,0,0,255),float rotX=0,float rotY=0,float rotZ=0);

  /**
   * @brief Create a sphere with a position, a radius, a texture, an alphe and multiple rotation (if you want)
   */
  GlSphere(const Coord &position,float radius,const std::string& textureFile,int alpha=255,float rotX=0,float rotY=0,float rotZ=0);

  /**
   * @brief Draw the sphere
   */
  virtual void draw(float lod,Camera *camera);

  /**
   * @brief Translate entity
   */
  virtual void translate(const Coord& mouvement);

  /**
   * @brief Get absolute position
   */
  const Coord& getPosition() const {
    return position;
  }

  /**
   * @brief Set absolute position
   */
  void setPosition(const Coord& pos) {
    position = pos;
  }

  /**
   * @brief Set the texture name
   */
  virtual void setTexture(const std::string &texture) {
    textureFile=texture;
  }

  /**
   * @brief Get the color
   */
  const Color& getColor() const {
    return color;
  }

  /**
   * @brief Set the color
   */
  void setColor(const Color &newColor) {
    color = newColor;
  }

  /**
   * @brief Function to export data in outString (in XML format)
   */
  virtual void getXML(std::string &outString);

  /**
   * @brief Function to set data with inString (in XML format)
   */
  virtual void setWithXML(const std::string &inString, unsigned int &currentPosition);

private:

  Coord position;
  float radius;
  Color color;
  std::string textureFile;
  Coord rot;

};


}

#endif // Tulip_GLSCENE_H
