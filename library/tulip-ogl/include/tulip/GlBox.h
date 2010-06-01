//-*-c++-*-
/*
 Author: Delorme Maxime and Mathiaut Morgan
 Email : Maxime.Delorme@gmail.com and mathiaut@labri.fr

 Last modification : 05/11/07

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_GLBOX_H
#define Tulip_GLBOX_H

#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/Size.h>

#include "tulip/GlSimpleEntity.h"
#include "tulip/GlTextureManager.h"

namespace tlp {
/** \brief General class used to render boxes as GlEntity.
 *
 * This class is a generic class to render boxes as GlEntity.
 */
class TLP_GL_SCOPE GlBox : public GlSimpleEntity
{

 public:  

  /**
   * The default constructor
   *
   * \attention It's usage is forbidden.
   */
  GlBox();

  /**
   * Constructor from size
   *
   * \param position The center of the box.
   * \param size The length of each dimension of the box.
   * \param fillColor The fill color of the box.
   * \param outlineColor The outline color of the box
   * \param filled Fill the box ?
   * \param outlined outline the box ?
   * \param outlineSize The size of the outline
   */
  GlBox(const Coord& position, const Size &size, const Color& fillColor, const Color &outlineColor,bool filled=true, bool outlined=true, const std::string &textureName="",float outlineSize=1.);

  /**
   * Destructor.
   */
  virtual ~GlBox();

  /**
   * Virtual function used to draw the box.
   */
  virtual void draw(float lod,Camera *camera);

  /**
   * Accessor in reading to the size.
   */
  Size getSize() const;

  /**
   * Accessor in writing to the size of the box
   */
  void setSize(const Size& size);

  /**
   * Accessor in reading to the position.
   */
  Coord* getPosition() const;

  /**
   * Accessor in writing to the position.
   */
  void setPosition(const Coord& position);

  /**
   * Accessor in reading to the fill color.
   */
  Color getFillColor() const;

  /**
   * Accessor in writing to the fill color of the box
   */
  void setFillColor(const Color& color);

  /**
   * Accessor in reading to the outline color.
   */
  Color getOutlineColor() const;

  /**
   * Accessor in writing to the outline color of the box
   */
  void setOutlineColor(const Color& color);

  /**
   * Accessor in reading to the outline size.
   */
  float getOutlineSize() const;

  /**
   * Accessor in writing to the outline size of the box
   */
  void setOutlineSize(float size);

  /**
   * Accessor in reading to the texture name.
   */
  std::string getTextureName() const;

  /**
   * Accessor in writing to the texture name of the box
   */
  void setTextureName(const std::string& textureName);

  /**
   * Translate entity
   */
  virtual void translate(const Coord& mouvement);

  /**
   * Function to export data in XML
   */
  virtual void getXML(xmlNodePtr rootNode);
  
  /**
   * Function to set data with XML
   */
  virtual void setWithXML(xmlNodePtr rootNode);

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
