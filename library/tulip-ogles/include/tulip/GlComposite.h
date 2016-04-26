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

#ifndef GLCOMPOSITE_H
#define GLCOMPOSITE_H

#include <map>
#include <string>

#include <tulip/GlEntity.h>

namespace tlp {

class TLP_GLES_SCOPE GlComposite : public GlEntity {

public:


  GlComposite(bool deleteEntitiesInDestructor = true);

  ~GlComposite();

  void setDeleteEntitiesInDestructor(bool deleteEntitiesInDestructor) {
    _deleteEntitiesInDestructor=deleteEntitiesInDestructor;
  }

  void reset(bool deleteElements);

  void addGlEntity(GlEntity *entity, const std::string &key);

  void deleteGlEntity(const std::string &key);

  void deleteGlEntity(GlEntity *entity);

  std::string findKey(GlEntity *entity);

  GlEntity* findGlEntity(const std::string &key);

  const std::map<std::string, GlEntity*> &getGlEntities () const {
    return _entities;
  }

  void setVisible(bool visible);

  void setStencil(int stencil);

  void acceptVisitor(GlSceneVisitor *visitor);

  virtual void draw(const Camera &, const Light &, bool) {}

protected:

  std::map<std::string, GlEntity*> _entities;
  bool _deleteEntitiesInDestructor;

};

}

#endif // GLCOMPOSITE_H
