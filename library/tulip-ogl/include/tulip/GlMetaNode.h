//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 16/11/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
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

  };

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLMETANODE_H
