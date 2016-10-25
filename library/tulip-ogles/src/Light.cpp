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

#include <tulip/Camera.h>
#include <tulip/Light.h>

using namespace tlp;

Light::Light()
    : _camera(nullptr), _directionnalLight(false), _ambientColor(Color(127, 127, 127)),
      _diffuseColor(127, 127, 127), _specularColor(0, 0, 0) {
}

Light::Light(Camera *camera)
    : _camera(camera), _directionnalLight(false), _ambientColor(Color(127, 127, 127)),
      _diffuseColor(127, 127, 127), _specularColor(0, 0, 0) {
}

void Light::setDirectionnalLight(const bool directionnalLight) {
  _directionnalLight = directionnalLight;
}

bool Light::directionnalLight() const {
  return _directionnalLight;
}

Vec4f Light::getPosition() const {
  Vec4f lightPos;
  if (!directionnalLight() && _camera) {
    Vec3f eyes = _camera->getEyes();
    Vec3f center = _camera->getCenter();
    float zoomFactor = _camera->getZoomFactor();
    lightPos = Vec4f(eyes, 0);
    lightPos[0] += (((eyes[0] - center[0]) / zoomFactor) + (eyes[0] - center[0]) * 4);
    lightPos[1] += (((eyes[1] - center[1]) / zoomFactor) + (eyes[1] - center[1]) * 4);
    lightPos[2] += (((eyes[2] - center[2]) / zoomFactor) + (eyes[2] - center[2]) * 4);
    lightPos[3] = 1;
  } else {
    lightPos[0] = 0;
    lightPos[1] = 0;
    lightPos[2] = 100;
    lightPos[3] = 0;
  }
  return lightPos;
}

void Light::setAmbientColor(const Color &ambient) {
  _ambientColor = ambient;
}

Color Light::getAmbientColor() const {
  return _ambientColor;
}

void Light::setDiffuseColor(const Color &diffuse) {
  _diffuseColor = diffuse;
}

Color Light::getDiffuseColor() const {
  return _diffuseColor;
}

void Light::setSpecularColor(const Color &specular) {
  _specularColor = specular;
}

Color Light::getSpecularColor() const {
  return _specularColor;
}
