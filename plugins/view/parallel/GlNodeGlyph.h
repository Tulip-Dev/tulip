//-*-c++-*-
/*
 Author: Antoine Lambert

 Email : antoine.lambert@labri.fr

 Last modification : 12/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef GLNODEGLYPH_H
#define GLNODEGLYPH_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <tulip/GlSimpleEntity.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/Coord.h>
#include <tulip/Size.h>

namespace tlp {

  // Class which allows to draw the glyph associated to a particular node
  // at a specific position with a specific size
  class GlNodeGlyph : public GlSimpleEntity {

  public :

    GlNodeGlyph(const Coord &position, const Size &size, GlGraphInputData *data, node n);

    void draw(float lod,Camera *camera);

    void getXML(xmlNodePtr rootNode) {}

    void setWithXML(xmlNodePtr rootNode) {}


  private :

    GlGraphInputData *data;
    Coord glyphPosition;
    Size glyphSize;
    node nodeId;

  };

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // GLNODEGLYPH_H
