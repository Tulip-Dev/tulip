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
 * tulip.js : port of the Tulip framework to JavaScript through emscripten
 * Copyright (c) 2016 Antoine Lambert, Thales Services SAS
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#include "FisheyeInteractor.h"
#include "ZoomAndPanInteractor.h"

#include <tulip/GlShaderProgram.h>
#include <tulip/GlBuffer.h>
#include <tulip/GlUtils.h>
#include <tulip/Camera.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlFrameBufferObject.h>
#include <tulip/GlLayer.h>
#include <tulip/ZoomAndPanAnimation.h>
#include <tulip/ShaderManager.h>
#include <tulip/GlScene.h>
#include <tulip/GlGraph.h>

using namespace tlp;

static std::string fisheyeVertexShaderSrc() {
  return ShaderManager::getShaderSrcPrefix() + R"(
  uniform mat4 u_modelviewMatrix;
  uniform mat4 u_projectionMatrix;

  attribute vec3 a_position;
  attribute vec2 a_texCoord;

  varying vec2 v_texCoord;

  void main() {
    gl_Position = u_projectionMatrix * u_modelviewMatrix * vec4(a_position, 1.0);
    v_texCoord = a_texCoord;
  }
)";
}

static std::string fisheyeFragmentShaderSrc() {
  return ShaderManager::getShaderSrcPrefix() + ShaderManager::getFXAAFunctionsSource() + R"(
    uniform sampler2D u_texture;
    uniform sampler2D u_fisheyeTexture;
    uniform float u_fisheyeRadius;
    uniform float u_fisheyeHeight;
    uniform float u_fisheyeTextureSize;
    uniform vec2 u_mouse;
    uniform vec2 u_resolution;

    varying vec2 v_texCoord;

    void main(void) {
      vec2 pos = v_texCoord * u_resolution;
      vec2 center = u_mouse;
      float radius = u_fisheyeRadius;
      float height = u_fisheyeHeight;
      float dist = distance(center, pos);
      if (dist < radius && u_fisheyeHeight < -0.05) {
        float coeff = (height + 1.0) * dist / (height * dist/ radius + 1.0);
        vec2 dir = normalize(pos - center) * coeff;
        pos = center + dir;
        vec2 fisheyePos = vec2(pos.x - (u_mouse.x - u_fisheyeRadius), pos.y - (u_mouse.y - u_fisheyeRadius)) / vec2(2.0*u_fisheyeRadius);
        vec2 fisheyeTexRes = vec2(u_fisheyeTextureSize);
        vec2 fisheyeFragCoord = fisheyePos * fisheyeTexRes;
        gl_FragColor = applyFXAA(u_fisheyeTexture, fisheyeFragCoord, fisheyeTexRes);
      } else {
        gl_FragColor = texture2D(u_texture, pos / u_resolution);
      }
    }
  )";
}

FisheyeInteractor::FisheyeInteractor(GlScene *scene)
    : _curX(-1), _curY(-1), _znpInteractor(NULL), _fisheyeShader(NULL), _buffer(NULL), _fbo(NULL), _fisheyeRadius(200), _fisheyeHeight(0.5),
      _maxTextureSize(0) {
  _glScene = scene;
  _znpInteractor = new ZoomAndPanInteractor(scene);
}

void FisheyeInteractor::activate() {
  _fisheyeShader = new GlShaderProgram();
  _fisheyeShader->addShaderFromSourceCode(GlShader::Vertex, fisheyeVertexShaderSrc());
  _fisheyeShader->addShaderFromSourceCode(GlShader::Fragment, fisheyeFragmentShaderSrc());
  _fisheyeShader->link();
  _fisheyeShader->printInfoLog();
  _buffer = new GlBuffer(GlBuffer::VertexBuffer);
  _indicesBuffer = new GlBuffer(GlBuffer::IndexBuffer);
  _fbo = NULL;

  if (_maxTextureSize == 0) {
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &_maxTextureSize);
    if (_maxTextureSize > 4096) {
      _maxTextureSize = 4096;
    }
  }
}

void FisheyeInteractor::desactivate() {
  delete _fisheyeShader;
  delete _buffer;
  delete _indicesBuffer;
  delete _fbo;
}

void FisheyeInteractor::setScene(GlScene *glScene) {
  GlSceneInteractor::setScene(glScene);
  _znpInteractor->setScene(glScene);
}

static float clamp(float n, float lower, float upper) {
  return std::max(lower, std::min(n, upper));
}

bool FisheyeInteractor::mouseCallback(const MouseButton &button, const MouseButtonState &state, int x, int y, const int &modifiers) {
  if (!_glScene)
    return false;
  if (button == WHEEL) {
    if (modifiers == ACTIVE_CTRL) {
      if (state == UP) {
        _fisheyeRadius += 10;
      } else {
        _fisheyeRadius -= 10;
      }
      if (_fisheyeRadius < 0)
        _fisheyeRadius = 0;
      _glScene->requestDraw();
      return true;
    } else if (modifiers == ACTIVE_SHIFT) {
      if (state == UP) {
        _fisheyeHeight += 0.05;
      } else {
        _fisheyeHeight -= 0.05;
      }
      _fisheyeHeight = clamp(_fisheyeHeight, 0.2, 0.9);
      _glScene->requestDraw();
      return true;
    } else {
      return _znpInteractor->mouseCallback(button, state, x, y, modifiers);
    }
  } else {
    _curX = x;
    _curY = y;
    _glScene->requestDraw();
    return true;
  }
}

bool FisheyeInteractor::mouseMoveCallback(int x, int y, const int & /*modifiers*/) {
  if (!_glScene)
    return false;
  _curX = x;
  _curY = y;
  _glScene->requestDraw();
  return true;
}

void FisheyeInteractor::draw() {

  tlp::Vec4i viewport = _glScene->getViewport();

  float factor = 1;
  if (_fisheyeHeight > 0.1) {
    factor = _fisheyeHeight * 10;
  }
  int fboSize = factor * 2 * _fisheyeRadius;
  fboSize = std::min(fboSize, _maxTextureSize);

  if (_fbo == NULL || _fbo->width() != fboSize) {
    delete _fbo;
    _fbo = new GlFrameBufferObject(fboSize, fboSize, GlFrameBufferObject::CombinedDepthStencil, GL_LINEAR, GL_LINEAR);
    GlTextureManager::instance()->addExternalTexture("fisheyeTexture", _fbo->texture());
  }

  Camera *camera = _glScene->getMainLayer()->getCamera();
  Camera camBackup = *camera;

  tlp::Coord bbMin = camera->viewportTo3DWorld(tlp::Coord(viewport[2] - (_curX - _fisheyeRadius), _curY - _fisheyeRadius));
  tlp::Coord bbMax = camera->viewportTo3DWorld(tlp::Coord(viewport[2] - (_curX + _fisheyeRadius), _curY + _fisheyeRadius));
  tlp::BoundingBox bb;
  bb.expand(bbMin);
  bb.expand(bbMax);

  std::set<GlEntity *> glEntities = _glScene->getEntities();
  for (std::set<GlEntity *>::iterator it = glEntities.begin(); it != glEntities.end(); ++it) {
    GlGraph *glGraph = dynamic_cast<GlGraph *>(*it);
    if (glGraph) {
      glGraph->getRenderingParameters().setMinSizeOfLabels(glGraph->getRenderingParameters().minSizeOfLabels() + factor * 4);
    }
  }

  _fbo->bind();
  _glScene->setViewport(0, 0, fboSize, fboSize);
  adjustViewToBoundingBox(camera, bb);
  glDisable(GL_SCISSOR_TEST);
  _glScene->draw();
  _fbo->release();

  glEntities = _glScene->getEntities();
  for (std::set<GlEntity *>::iterator it = glEntities.begin(); it != glEntities.end(); ++it) {
    GlGraph *glGraph = dynamic_cast<GlGraph *>(*it);
    if (glGraph) {
      glGraph->getRenderingParameters().setMinSizeOfLabels(glGraph->getRenderingParameters().minSizeOfLabels() - factor * 4);
    }
  }

  _glScene->setViewport(viewport);
  *camera = camBackup;
  _glScene->initGlParameters();
  _glScene->setSceneNeedRedraw(false);

  camera->initGl2D();

  std::vector<float> quadData;
  addTlpVecToVecFloat(tlp::Vec3f(0, 0, 0), quadData);
  addTlpVecToVecFloat(tlp::Vec2f(0, 0), quadData);
  addTlpVecToVecFloat(tlp::Vec3f(0, viewport[3], 0), quadData);
  addTlpVecToVecFloat(tlp::Vec2f(0, 1), quadData);
  addTlpVecToVecFloat(tlp::Vec3f(viewport[2], viewport[3], 0), quadData);
  addTlpVecToVecFloat(tlp::Vec2f(1, 1), quadData);
  addTlpVecToVecFloat(tlp::Vec3f(viewport[2], 0, 0), quadData);
  addTlpVecToVecFloat(tlp::Vec2f(1, 0), quadData);
  std::vector<unsigned short> indices = {0, 1, 2, 0, 2, 3};

  glDisable(GL_BLEND);

  _buffer->bind();
  _buffer->allocate(quadData);
  _fisheyeShader->activate();
  _fisheyeShader->setUniformMat4Float("u_modelviewMatrix", camera->modelviewMatrix());
  _fisheyeShader->setUniformMat4Float("u_projectionMatrix", camera->projectionMatrix());
  _fisheyeShader->setVertexAttribPointer("a_position", 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), BUFFER_OFFSET(0));
  _fisheyeShader->setVertexAttribPointer("a_texCoord", 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
  _fisheyeShader->setUniformTextureSampler("u_texture", 0);
  _fisheyeShader->setUniformTextureSampler("u_fisheyeTexture", 1);
  _fisheyeShader->setUniformVec2Float("u_mouse", _curX, viewport[3] - _curY);
  _fisheyeShader->setUniformVec2Float("u_resolution", viewport[2], viewport[3]);
  _fisheyeShader->setUniformFloat("u_fisheyeRadius", _fisheyeRadius);
  _fisheyeShader->setUniformFloat("u_fisheyeHeight", -_fisheyeHeight);
  _fisheyeShader->setUniformFloat("u_fisheyeTextureSize", fboSize);
  _indicesBuffer->bind();
  _indicesBuffer->allocate(indices);
  GlTextureManager::instance()->bindTexture(_glScene->getBackBufferTextureName());
  GlTextureManager::instance()->bindTexture("fisheyeTexture", 1);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
  GlTextureManager::instance()->unbindTexture(_glScene->getBackBufferTextureName());
  GlTextureManager::instance()->unbindTexture("fisheyeTexture");
  _fisheyeShader->desactivate();
  _indicesBuffer->release();
  _buffer->release();

  camera->initGl();
}
