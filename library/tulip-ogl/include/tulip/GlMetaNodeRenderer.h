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
#ifndef Tulip_GLMETANODERENDERER_H
#define Tulip_GLMETANODERENDERER_H

#include <tulip/tulipconf.h>
#include <tulip/Graph.h>

namespace tlp {

  class GlGraphInputData;
  class Camera;

  /**
   * Class use to render a meta node, this version render nothing
   */
  class TLP_GL_SCOPE GlMetaNodeRenderer {

  public:

    virtual ~GlMetaNodeRenderer() {}

    virtual void prerender(node n,float lod,Camera* camera) {}
    virtual void render(node n,float lod,Camera* camera) {}
    virtual bool havePrerender() {return false;}
    virtual bool glMetaNodeHaveToRenderLabels(){return true;}

    virtual void setInputData(GlGraphInputData *data) {}
    virtual GlGraphInputData *getInputData() {return NULL;}

  };

}

#endif // Tulip_GLMETANODERENDERER_H
