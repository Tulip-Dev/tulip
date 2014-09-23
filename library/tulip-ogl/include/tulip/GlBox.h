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

#ifndef Tulip_GLBOX_H
#define Tulip_GLBOX_H

#if defined(_MSC_VER)
#include <Windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <tulip/Color.h>
#include <tulip/Size.h>
#include <tulip/GlSimpleEntity.h>

namespace tlp {
/**
 * @ingroup OpenGL
 * @brief General class used to render boxes as GlSimpleEntity.
 */
class TLP_GL_SCOPE GlBox : public GlSimpleEntity {

public:

  /**
   * @brief Don't use this constructor
   */
  GlBox();

  /**
   * @brief Constructor
   *
   * @param position The center of the box.
   * @param size The length of each dimension of the box.
   * @param fillColor The fill color of the box.
   * @param outlineColor The outline color of the box
   * @param filled Fill the box ?
   * @param outlined outline the box ?
   * @param outlineSize The size of the outline
   */
  GlBox(const Coord& position, const Size &size, const Color& fillColor, const Color &outlineColor,bool filled=true, bool outlined=true, const std::string &textureName="",float outlineSize=1.);

  /**
   * @brief Destructor.
   */
  virtual ~GlBox();

  virtual void draw(float lod,Camera *camera);

  /**
   * @brief Accessor in reading to the size.
   */
  Size getSize() const;

  /**
   * @brief Accessor in writing to the size of the box
   */
  void setSize(const Size& size);

  /**
   * @brief Accessor in reading to the position.
   */
  Coord* getPosition() const;

  /**
   * @brief Accessor in writing to the position.
   */
  void setPosition(const Coord& position);

  /**
   * @brief Accessor in reading to the fill color.
   */
  Color getFillColor() const;

  /**
   * @brief Accessor in writing to the fill color of the box
   */
  void setFillColor(const Color& color);

  /**
   * @brief Accessor in reading to the outline color.
   */
  Color getOutlineColor() const;

  /**
   * @brief Accessor in writing to the outline color of the box
   */
  void setOutlineColor(const Color& color);

  /**
   * @brief Accessor in reading to the outline size.
   */
  float getOutlineSize() const;

  /**
   * @brief Accessor in writing to the outline size of the box
   */
  void setOutlineSize(float size);

  /**
   * @brief Accessor in reading to the texture name.
   */
  std::string getTextureName() const;

  /**
   * @brief Accessor in writing to the texture name of the box
   */
  void setTextureName(const std::string& textureName);

  /**
   * @brief Translate entity
   */
  virtual void translate(const Coord& mouvement);

  /**
   * @brief Function to export data in outString (in XML format)
   */
  virtual void getXML(std::string &outString);

  /**
   * @brief Function to set data with inString (in XML format)
   */
  virtual void setWithXML(const std::string &inString, unsigned int &currentPosition);

protected:

  virtual void clearGenerated();

  Coord position; /**< The position of the center of the box*/
  Size size; /**< size is the "radius" of the box */
  std::vector<Color> fillColors; /**< fillColor of the box */
  std::vector<Color> outlineColors; /**< outlineColor of the box */
  bool filled; /**< the box is filled ? */
  bool outlined; /**< the box is outlined ? */
  std::string textureName;
  float outlineSize; /**< size of the ouline */

  float *newCubeCoordArrays;
  bool generated;
  GLuint buffers[5];
};

}
#endif
