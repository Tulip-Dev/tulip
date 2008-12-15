//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 08/11/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/
#ifndef Tulip_GLNODE_H
#define Tulip_GLNODE_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <tulip/Color.h>

#include "tulip/GlComplexeEntity.h"

namespace tlp {

  class OcclusionTest;
  class TextRenderer;
  class GlSceneVisitor;

  class TLP_GL_SCOPE GlNode : public GlComplexeEntity{

  public:

    GlNode(unsigned int id):id(id) {}

    virtual void acceptVisitor(GlSceneVisitor *visitor);

    virtual BoundingBox getBoundingBox(GlGraphInputData* data);

    virtual void draw(float lod,GlGraphInputData *data,Camera* camera);
    virtual void drawLabel(bool drawSelect,bool drawNodesLabel,bool drawEdgesLabel,OcclusionTest* test,TextRenderer* renderer,GlGraphInputData* data);

    unsigned int id;

    virtual void drawPixmapFont(OcclusionTest* test,TextRenderer* renderer,GlGraphInputData* data,
        const std::string &str, const Color &col,  const Coord &position, int labelPos, bool selected, float width);

  };

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLNODE_H
