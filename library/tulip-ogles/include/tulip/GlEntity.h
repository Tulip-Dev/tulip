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
 * Copyright (c) 2016 Antoine Lambert, Thales Services SAS
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#ifndef GLENTITY_H
#define GLENTITY_H

#include <vector>
#include <map>

#include <tulip/BoundingBox.h>
#include <tulip/Observable.h>

#include <tulip/GlEntity.h>
#include <tulip/Light.h>
#include <tulip/GlSceneVisitor.h>

namespace tlp {

class GlComposite;

class TLP_GLES_SCOPE GlEntity : public tlp::Observable {

public:

  GlEntity();

  virtual ~GlEntity();

  virtual void setVisible(bool visible);

  virtual bool isVisible() const {
    return _visible;
  }

  virtual void setStencil(int stencil);

  virtual int getStencil() {
    return _stencil;
  }

  virtual void draw(const Camera &camera, bool pickingMode=false) {
    static Light light;
    draw(camera, light, pickingMode);
  }

  virtual void draw(const Camera &camera, const Light &light, bool pickingMode=false) = 0;

  virtual tlp::BoundingBox getBoundingBox() {
    return _boundingBox;
  }

  virtual void acceptVisitor(GlSceneVisitor *visitor) {
    visitor->visit(this);
  }

  void setLayer(GlLayer *layer) {
    _layer = layer;
  }

  GlLayer *getLayer() const {
    return _layer;
  }

  void setSelected(bool selected) {
    _selected = selected;
  }

  bool selected() const {
    return _selected;
  }

  unsigned int getId() const {
    return _id;
  }

  void setParent(GlComposite *composite) {
    _parent = composite;
  }

  GlComposite *getParent() const {
    return _parent;
  }

  static GlEntity *fromId(unsigned int id) {
    return idToEntity[id];
  }


protected:

  void notifyModified();

  bool _visible;
  int _stencil;
  bool _selected;

  tlp::BoundingBox _boundingBox;

  GlLayer *_layer;

  unsigned int _id;
  tlp::Color _pickingColor;

  GlComposite *_parent;

  static unsigned int nextId;
  static std::map<unsigned int, GlEntity*> idToEntity;

};

}

#endif // GLENTITY_H


