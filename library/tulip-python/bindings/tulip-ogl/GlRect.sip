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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (http://booggie.org/)                                                   |
// +-------------------------------------------------------------------------+


namespace tlp {

class GlRect : tlp::GlPolygon {
%TypeHeaderCode
#include <tulip/GlRect.h>
%End

 public:
  
  GlRect(const tlp::Coord &topLeftPos, const tlp::Coord &bottomRightPos, const tlp::Color &topLeftCol, const tlp::Color &bottomRightCol, bool filled, bool outlined );

  GlRect(const tlp::Coord &center, const tlp::Size &size, const tlp::Color &fillColor, const tlp::Color &outlineColor);

  GlRect(bool filled=true, bool outlined=false);

  virtual ~GlRect();

  virtual tlp::Coord getCenter();

  void setCenterAndSize(const tlp::Coord &center, const tlp::Size &size);

  virtual tlp::Coord getTopLeftPos();

  virtual tlp::Coord getBottomRightPos();

  virtual tlp::Color getTopLeftColor();

  virtual tlp::Color getBottomRightColor();

  virtual void setTopLeftPos(const tlp::Coord &topLeftPos);

  virtual void setBottomRightPos(const tlp::Coord &bottomRightPos);

  virtual void setTopLeftColor(const tlp::Color &topLeftCol);

  virtual void setBottomRightColor(const tlp::Color &bottomRightCol);

  bool inRect(double x, double y);

};

};

