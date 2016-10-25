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

#ifndef LIGHT_H
#define LIGHT_H

#include <tulip/Vector.h>
#include <tulip/Color.h>

namespace tlp {

class Camera;

class TLP_GLES_SCOPE Light {

public:
  Light();
  Light(Camera *camera);

  void setCamera(Camera *camera) {
    _camera = camera;
  }

  void setDirectionnalLight(const bool directionnalLight);
  bool directionnalLight() const;

  tlp::Vec4f getPosition() const;

  void setAmbientColor(const tlp::Color &ambient);
  tlp::Color getAmbientColor() const;

  void setDiffuseColor(const tlp::Color &diffuse);
  tlp::Color getDiffuseColor() const;

  void setSpecularColor(const tlp::Color &specular);
  tlp::Color getSpecularColor() const;

private:
  Camera *_camera;
  bool _directionnalLight;
  tlp::Color _ambientColor;
  tlp::Color _diffuseColor;
  tlp::Color _specularColor;
};
}

#endif // LIGHT_H
