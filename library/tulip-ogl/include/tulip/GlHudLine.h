//-*-c++-*-
/*
 Author: Delorme Maxime
 Email : Maxime.Delorme@gmail.com

 Last modification : 05/08/2005 (fr) 

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef GL_HUD_LINE_H
#define GL_HUD_LINE_H

#include <tulip/GlHud.h>

namespace tlp {

class GlHudLine : public GlHud
{
 protected:
  Coord *startPos; /**< Starting position of the line */
  Coord *endPos; /**< Ending position of the line */
  Color *startCol; /**< Starting color of the line */
  Color *endCol; /**< Ending color of the line */
  unsigned int thickness; /**< Thickness of the line */


  /**
   * Default constructor (forbidden to use)
   */
  GlHudLine(); 
 public:
  
  /**
   * Constructor
   */
  GlHudLine(const Coord &startPos, const Coord &endPos, const Color &startCol, const Color &endCol, unsigned int thickness = 1);

  /**
   * Destructor
   */
  virtual ~GlHudLine();

  /**
   * Function used to draw ... This is a pure virtual function
   * \attention Must absolutely be called with unsetup in every derivated class
   * \attention You call setup(), you render, you call unsetup()
   * \attention This must not be called withing glBegin/glEnd statements.
   */
  virtual void draw(GlGraph*);

  /**
   * Accessor in reading to the start position of the line
   */
  virtual Coord getStartPosition() const;

  /**
   * Accessor in reading to the end position of the line
   */
  virtual Coord getEndPosition() const;

  /**
   * Accessor in reading to the start color of the line
   */
  virtual Color getStartColor() const;

  /**
   * Accessor in reading to the end color of the line
   */
  virtual Color getEndColor() const;

  /**
   * Accessor in reading to the thickness of the line.
   */
  virtual unsigned int getThickness() const;

  /**
   * Accessor in writing to the start position of the line
   */
  virtual void setStartPosition(const Coord &startPos);

  /**
   * Accessor in writing to the end position of the line
   */
  virtual void setEndPosition(const Coord &endPos);

  /**
   * Accessor in writing to the start color of the line
   */
  virtual void setStartColor(const Color &startColor);

  /**
   * Accessor in writing to the end color of the line.
   */
  virtual void setEndColor(const Color &endColor);

  /**
   * Accessor in writing to the thickness of the line.
   */
  virtual void setThickness(unsigned int thickness);
};

}


#endif
