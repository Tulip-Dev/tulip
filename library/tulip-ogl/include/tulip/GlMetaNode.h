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
#ifndef Tulip_GLMETANODE_H
#define Tulip_GLMETANODE_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include "tulip/GlNode.h"

namespace tlp {

  /**
   * Class who represent a meta-node
   */
  class TLP_GL_SCOPE GlMetaNode : public GlNode{

  public:

    /**
     * Create a meta-node with the given id
     */
    GlMetaNode(unsigned int id):GlNode(id) {}

    /**
     * Draw the meta-node
     */
    virtual void draw(float lod,GlGraphInputData *data,Camera* camera);

    /**
     * Draw label of the meta-node
     */
    virtual void drawLabel(OcclusionTest* test,TextRenderer* renderer,GlGraphInputData* data);

    /**
     * Draw label of the meta-node with lod
     */
    virtual void drawLabel(OcclusionTest* test,TextRenderer* renderer,GlGraphInputData* data,float lod,Camera *camera=NULL);

  };

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLMETANODE_H
