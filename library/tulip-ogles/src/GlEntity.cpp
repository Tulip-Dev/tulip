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

#include <tulip/GlEntity.h>
#include <tulip/GlUtils.h>

using namespace std;
using namespace tlp;

unsigned int GlEntity::nextId(0);
map<unsigned int, GlEntity*> GlEntity::idToEntity;

GlEntity::GlEntity() : _visible(true), _stencil(0xFF), _selected(false), _layer(nullptr), _parent(nullptr) {
  _id = ++nextId;
  idToEntity[_id] = this;
  _pickingColor = uintToColor(_id);
  _boundingBox[0].fill(0);
  _boundingBox[1].fill(0);
}

void GlEntity::setVisible(bool visible) {
  if (_visible != visible) {
    _visible = visible;
    notifyModified();
  }
}

void GlEntity::setStencil(int stencil) {
  if (_stencil != stencil) {
    _stencil = stencil;
    notifyModified();
  }
}

void GlEntity::notifyModified() {
  sendEvent(Event(*this, Event::TLP_MODIFICATION));
}
