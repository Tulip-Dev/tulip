//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 23/10/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/
#ifndef Tulip_GLENTITY_H
#define Tulip_GLENTITY_H

#include <utility>

#include <tulip/tulipconf.h>

namespace tlp {

  class GlSceneVisitor;
  class GlLayout;

  /**
   * Abstract class used to represent all entity of a scene
   */
  class TLP_GL_SCOPE GlEntity {

  public:

    virtual ~GlEntity() {}

    /**
     * Accpet visitor function
     */
    virtual void acceptVisitor(GlSceneVisitor *visitor)=0;

  };

}

#endif // Tulip_GLENTITY_H
