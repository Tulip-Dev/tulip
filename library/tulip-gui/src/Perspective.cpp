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
#include "tulip/Perspective.h"

namespace tlp {

tlp::Perspective* Perspective::_instance = NULL;

Perspective* Perspective::instance() {
  return _instance;
}

Perspective::Perspective(PerspectiveContext &c) {
  _instance = this;
  _mainWindow = c.mainWindow;
  _project = c.project;
  _externalFile = c.externalFile;
  _parameters = c.parameters;
}

Perspective::~Perspective() {
  delete _project;
//  delete _mainWindow;
}

bool Perspective::isCompatible(tlp::TulipProject *) {
  return false;
}

}
