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

    virtual void setInputData(GlGraphInputData *data){
      inputData=data;
    }
    virtual GlGraphInputData *getInputData(){
      return inputData;
    }


  private :

    GlGraphInputData *inputData;

    static unsigned int depth;

  };

}

#endif // Tulip_GLMETANODETRUERENDERER_H
