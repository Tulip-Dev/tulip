/**
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

/**
 *
 * tulip-ogles library : rewrite of old tulip-ogl library using OpenGL ES API
 * Copyright (c) 2016 Antoine Lambert, Thales Services
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#ifndef GLBOUNDINGBOXSCENEVISITOR_H
#define GLBOUNDINGBOXSCENEVISITOR_H

#include <tulip/BoundingBox.h>
#include <tulip/GlSceneVisitor.h>

namespace tlp {

class GlEntity;

class TLP_GLES_SCOPE GlBoundingBoxSceneVisitor : public GlSceneVisitor {

public:

  GlBoundingBoxSceneVisitor() {}

  virtual void visit(GlEntity *entity);

  tlp::BoundingBox getBoundingBox() {
    return _boundingBox;
  }

private:

  tlp::BoundingBox _boundingBox;

};

}

#endif // GLBOUNDINGBOXSCENEVISITOR_H
