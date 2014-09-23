/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
///@cond DOXYGEN_HIDDEN

#ifndef Tulip_GLENTITY_H
#define Tulip_GLENTITY_H

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
///@endcond
