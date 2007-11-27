//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 09/11/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_GLEDGE_H
#define Tulip_GLEDGE_H

#include <tulip/Types.h>

#include "tulip/GlComplexeEntity.h"

namespace tlp {
  
  class OcclusionTest;
  class TextRenderer;

  class TLP_GL_SCOPE GlEdge : public GlComplexeEntity{
  
  public:

    GlEdge(unsigned int id):id(id) {}

    virtual void acceptVisitor(GlSceneVisitor *visitor) {
      visitor->visit(this);
    }

    virtual BoundingBox getBoundingBox(GlGraphInputData* data);

    virtual void draw(float lod,GlGraphInputData *data,Camera* camera);

    void drawEdge(const Coord &srcNodePos, const Coord &tgtNodePos,
		  const Coord &startPoint, const Coord &endPoint, const LineType::RealType &bends,
		  const Color &startColor, const Color &endColor, const Size &size, int shape, bool edge3D);

    void drawLabel(bool drawSelect,OcclusionTest* test,TextRenderer* renderer,GlGraphInputData* data);

    unsigned int id;
    
  };
  
}

#endif // Tulip_GLEDGE_H
