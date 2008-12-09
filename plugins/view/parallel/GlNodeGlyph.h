//-*-c++-*-
/*
 Author: Didier Bathily, Nicolas Bellino, Jonathan Dubois, Christelle Jolly, Antoine Lambert, Nicolas Sorraing

 Email : didier.bathily@etu.u-bordeaux1.fr, nicolas.bellino@etu.u-bordeaux1.fr, jonathan.dubois@etu.u-bordeaux1.fr, christelle.jolly@etu.u-bordeaux1.fr, antoine.lambert@etu.u-bordeaux1.fr, nicolas.sorraing@etu.u-bordeaux1.fr

 Last modification : 03/08

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
  class TLP_GL_SCOPE GlNodeGlyph : public GlSimpleEntity {

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
