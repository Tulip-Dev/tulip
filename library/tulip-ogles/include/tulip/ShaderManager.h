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

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <string>
#include <cfloat>
#include <map>

#include <tulip/tulipconf.h>

namespace tlp {

class GlShaderProgram;

class TLP_GLES_SCOPE ShaderManager {

public:

  static ShaderManager *getInstance(const std::string &canvasId);

  static ShaderManager *getInstance();

  static void setCurrentCanvasId(const std::string &canvasId) {
    _currentCanvasId = canvasId;
  }

  static std::string getShaderSrcPrefix();

  static std::string getFXAAFunctionsSource();

  GlShaderProgram *getFlatRenderingShader() const {
    return _flatRenderingShader;
  }

  GlShaderProgram *getBlinnPhongRenderingShader() const {
    return _blinnPhongRenderingShader;
  }

private:

  ShaderManager();

  static std::map<std::string, ShaderManager *> _instances;
  static std::string _currentCanvasId;

  GlShaderProgram *_flatRenderingShader;

  GlShaderProgram *_blinnPhongRenderingShader;

};

}

#endif // SHADERMANAGER_H
