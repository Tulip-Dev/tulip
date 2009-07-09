//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 06/04/2009
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/
#ifndef Tulip_GLMETANODETRUERENDERER_H
#define Tulip_GLMETANODETRUERENDERER_H

#include <tulip/tulipconf.h>
#include <tulip/Graph.h>

#include "tulip/GlMetaNodeRenderer.h"

namespace tlp {

  class GlGraphInputData;

  /**
   * Class use to render a meta node, this version render meta node with old OpenGl tulip system
   */
  class TLP_GL_SCOPE GlMetaNodeTrueRenderer : public GlMetaNodeRenderer{

  public:

    GlMetaNodeTrueRenderer(GlGraphInputData *inputData);
    virtual ~GlMetaNodeTrueRenderer() {}

    virtual void render(node n,float lod,Camera* camera);

  private :

    GlGraphInputData *inputData;

  };

}

#endif // Tulip_GLMETANODETRUERENDERER_H
