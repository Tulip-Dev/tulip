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

#include <GL/glew.h>

#include "FishEyeInteractor.h"
#include "FishEyeConfigWidget.h"
#include "../../utils/PluginNames.h"

#include <QMouseEvent>

#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/GlBuffer.h>
#include <tulip/GlFrameBufferObject.h>
#include <tulip/GlGraph.h>
#include <tulip/GlLayer.h>
#include <tulip/GlMainView.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlShaderProgram.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlUtils.h>
#include <tulip/MouseInteractors.h>
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/OpenGlConfigManager.h>
#include <tulip/ShaderManager.h>
#include <tulip/ZoomAndPanAnimation.h>
#include <tulip/ShaderManager.h>
#include <tulip/FontIconManager.h>

using namespace std;
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
    uniform bool u_useFXAA;
    varying vec2 v_texCoord;

    void main(void) {
      vec2 pos = v_texCoord * u_resolution;
      vec2 center = u_mouse;
      float radius = u_fisheyeRadius;
      float height = u_fisheyeHeight;
      float dist = distance(center, pos);
      if (dist < radius && u_fisheyeHeight < -0.05) {
        float coeff = (height + 1.0) * dist / (height * dist/  radius + 1.0);
        vec2 dir = normalize(pos - center) * coeff;
        pos = center + dir;
        vec2 fisheyePos = vec2(pos.x - (u_mouse.x - u_fisheyeRadius), pos.y - (u_mouse.y - u_fisheyeRadius)) / vec2(2.0*u_fisheyeRadius);
        if (!u_useFXAA) {
          gl_FragColor = texture2D(u_fisheyeTexture, fisheyePos);
        } else {
          vec2 fisheyeTexRes = vec2(u_fisheyeTextureSize);
          vec2 fisheyeFragCoord = fisheyePos * fisheyeTexRes;
          gl_FragColor = applyFXAA(u_fisheyeTexture, fisheyeFragCoord, fisheyeTexRes);
        }
      } else {
        gl_FragColor = texture2D(u_texture, pos / u_resolution);
      }
    }
  )";
}

FishEyeInteractor::FishEyeInteractor(const PluginContext *)
    : GLInteractorComposite(FontIconManager::instance()->getMaterialDesignIcon(md::web, Qt::white), "Fisheye"), _fisheyeConfigWidget(nullptr) {
}

void FishEyeInteractor::construct() {
  _fisheyeConfigWidget = new FishEyeConfigWidget();
  push_back(new MousePanNZoomNavigator());
  FishEyeInteractorComponent *fisheyeInteractorComponent = new FishEyeInteractorComponent(_fisheyeConfigWidget);
  push_back(fisheyeInteractorComponent);
}

bool FishEyeInteractor::isCompatible(const std::string &viewName) const {
  return ((viewName == NodeLinkDiagramComponent::viewName) || (viewName == ViewName::HistogramViewName) || (viewName == ViewName::MatrixViewName) ||
          (viewName == ViewName::ParallelCoordinatesViewName) || (viewName == ViewName::PixelOrientedViewName) ||
          (viewName == ViewName::ScatterPlot2DViewName));
}

void FishEyeInteractor::uninstall() {
  InteractorComposite::uninstall();
}

FishEyeInteractor::~FishEyeInteractor() {
  delete _fisheyeConfigWidget;
}

QWidget *FishEyeInteractor::configurationWidget() const {
  return _fisheyeConfigWidget;
}

PLUGIN(FishEyeInteractor)

GlShaderProgram *FishEyeInteractorComponent::_fisheyeShader(nullptr);
GlBuffer *FishEyeInteractorComponent::_buffer(nullptr);
GlBuffer *FishEyeInteractorComponent::_indicesBuffer(nullptr);
int FishEyeInteractorComponent::_maxTextureSize(0);

FishEyeInteractorComponent::FishEyeInteractorComponent(FishEyeConfigWidget *configWidget)
    : _configWidget(configWidget), _curX(-1), _curY(-1), _fbo(nullptr), _fbo2(nullptr) {
  _fboTextureId = "fisheyeTexture" + toString(reinterpret_cast<unsigned long long>(this));
}

FishEyeInteractorComponent::FishEyeInteractorComponent(FishEyeConfigWidget *configWidget) : configWidget(configWidget), activateFishEye(false) {
}

FishEyeInteractorComponent::FishEyeInteractorComponent(const FishEyeInteractorComponent &fisheyeInteractorComponent) {
  fisheyeCenter = fisheyeInteractorComponent.fisheyeCenter;
  configWidget = fisheyeInteractorComponent.configWidget;
  activateFishEye = false;
}

void FishEyeInteractorComponent::viewChanged(View *view) {
  if (view == nullptr) {
    return;
  }

  GlMainView *glView = (GlMainView *)view;
  GlMainWidget *glWidget = glView->getGlMainWidget();

  if (!glWidget->hasMouseTracking()) {
    glWidget->setMouseTracking(true);
  }

  if (_configWidget->getFishEyeRadius() == 0.) {
    _configWidget->setFishEyeRadius(200);
    _configWidget->setFishEyeHeight(0.5);
  }
}

bool FishEyeInteractorComponent::eventFilter(QObject *obj, QEvent *e) {

  GlMainWidget *glWidget = (GlMainWidget *)obj;

  _activateFishEye = false;

  if (e->type() == QEvent::MouseMove || e->type() == QEvent::MouseButtonPress || e->type() == QEvent::MouseButtonRelease) {
    _activateFishEye = true;
    QMouseEvent *me = (QMouseEvent *)e;
    _curX = me->x();
    _curY = me->y();
    glWidget->redraw();
    return true;
  } else if (e->type() == QEvent::Wheel) {
    _activateFishEye = true;
    QWheelEvent *wheelEvent = (QWheelEvent *)e;
    int numSteps = wheelEvent->delta() / 120;

    if (wheelEvent->orientation() == Qt::Vertical && (wheelEvent->modifiers() == Qt::ControlModifier)) {
      _activateFishEye = true;
      double newRadius = _configWidget->getFishEyeRadius() + _configWidget->getFishEyeRadiusIncrementStep() * numSteps * 10;
      if (newRadius < 0) {
        newRadius = 0;
      }
      _configWidget->setFishEyeRadius(newRadius);
      glWidget->redraw();
      return true;
    } else if (wheelEvent->orientation() == Qt::Vertical && (wheelEvent->modifiers() == Qt::ShiftModifier)) {
      _activateFishEye = true;
      double newHeight = _configWidget->getFishEyeHeight() + _configWidget->getFishEyeHeightIncrementStep() * numSteps * 0.05;
      newHeight = clamp(newHeight, 0.2, 0.9);
      _configWidget->setFishEyeHeight(newHeight);
      glWidget->redraw();
      return true;
    }

    return false;
  }

  return false;
}

bool FishEyeInteractorComponent::draw(GlMainWidget *glWidget) {

  if (!_fisheyeShader) {
    _fisheyeShader = new GlShaderProgram();
    _fisheyeShader->addShaderFromSourceCode(GlShader::Vertex, fisheyeVertexShaderSrc());
    _fisheyeShader->addShaderFromSourceCode(GlShader::Fragment, fisheyeFragmentShaderSrc());
    _fisheyeShader->link();
    _fisheyeShader->printInfoLog();
    _buffer = new GlBuffer(GlBuffer::VertexBuffer);
    _indicesBuffer = new GlBuffer(GlBuffer::IndexBuffer);

    if (_maxTextureSize == 0) {
      glGetIntegerv(GL_MAX_TEXTURE_SIZE, &_maxTextureSize);
      if (_maxTextureSize > 4096) {
        _maxTextureSize = 4096;
      }
    }
  }

  if (_activateFishEye && _fisheyeShader && _fisheyeShader->isLinked()) {
    GlScene *glScene = glWidget->getScene();
    Vec4i viewport = glScene->getViewport();

    float factor = 1;
    float fisheyeHeight = _configWidget->getFishEyeHeight();
    float fisheyeRadius = _configWidget->getFishEyeRadius();
    if (fisheyeHeight > 0.1) {
      factor = fisheyeHeight * 10;
    }
    int fboSize = factor * 2 * fisheyeRadius;
    fboSize = std::min(fboSize, _maxTextureSize);

    if (!_fbo || _fbo->width() != fboSize) {
      delete _fbo;
      delete _fbo2;
      if (GlFrameBufferObject::hasOpenGLFramebufferBlit()) {
        _fbo = new GlFrameBufferObject(fboSize, fboSize, GlFrameBufferObject::CombinedDepthStencil,
                                       OpenGlConfigManager::instance().maxNumberOfSamples());
        _fbo2 = new GlFrameBufferObject(fboSize, fboSize, GlFrameBufferObject::NoAttachment, 0, GL_LINEAR, GL_LINEAR);
        GlTextureManager::instance()->addExternalTexture(_fboTextureId, _fbo2->texture());
      } else {
        _fbo = new GlFrameBufferObject(fboSize, fboSize, GlFrameBufferObject::CombinedDepthStencil, 0, GL_LINEAR, GL_LINEAR);
        GlTextureManager::instance()->addExternalTexture(_fboTextureId, _fbo->texture());
      }
    }

    Camera *camera = glScene->getMainLayer()->getCamera();
    Camera camBackup = *camera;

    Coord bbMin = camera->viewportTo3DWorld(glWidget->screenToViewport(Coord(viewport[2] - (_curX - fisheyeRadius), _curY - fisheyeRadius)));
    Coord bbMax = camera->viewportTo3DWorld(glWidget->screenToViewport(Coord(viewport[2] - (_curX + fisheyeRadius), _curY + fisheyeRadius)));
    BoundingBox bb;
    bb.expand(bbMin);
    bb.expand(bbMax);

    std::set<GlEntity *> glEntities = glScene->getEntities();
    for (std::set<GlEntity *>::iterator it = glEntities.begin(); it != glEntities.end(); ++it) {
      GlGraph *glGraph = dynamic_cast<GlGraph *>(*it);
      if (glGraph) {
        glGraph->getRenderingParameters().setMinSizeOfLabels(glGraph->getRenderingParameters().minSizeOfLabels() + factor * 4);
      }
    }

    _fbo->bind();
    glScene->setViewport(0, 0, fboSize, fboSize);
    adjustViewToBoundingBox(camera, bb);
    glDisable(GL_SCISSOR_TEST);
    glScene->draw();
    _fbo->release();
    if (GlFrameBufferObject::hasOpenGLFramebufferBlit()) {
      GlFrameBufferObject::blitFramebuffer(_fbo2, Vec2i(0, 0), Vec2i(fboSize, fboSize), _fbo, Vec2i(0, 0), Vec2i(fboSize, fboSize),
                                           GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }

    glEntities = glScene->getEntities();
    for (std::set<GlEntity *>::iterator it = glEntities.begin(); it != glEntities.end(); ++it) {
      GlGraph *glGraph = dynamic_cast<GlGraph *>(*it);
      if (glGraph) {
        glGraph->getRenderingParameters().setMinSizeOfLabels(glGraph->getRenderingParameters().minSizeOfLabels() - factor * 4);
      }
    }

    glScene->setViewport(viewport);
    *camera = camBackup;
    glScene->initGlParameters();
    glScene->setSceneNeedRedraw(false);

    camera->initGl2D();

    std::vector<float> quadData;
    addTlpVecToVecFloat(Vec3f(0, 0, 0), quadData);
    addTlpVecToVecFloat(Vec2f(0, 0), quadData);
    addTlpVecToVecFloat(Vec3f(0, viewport[3], 0), quadData);
    addTlpVecToVecFloat(Vec2f(0, 1), quadData);
    addTlpVecToVecFloat(Vec3f(viewport[2], viewport[3], 0), quadData);
    addTlpVecToVecFloat(Vec2f(1, 1), quadData);
    addTlpVecToVecFloat(Vec3f(viewport[2], 0, 0), quadData);
    addTlpVecToVecFloat(Vec2f(1, 0), quadData);
    std::vector<unsigned short> indices = {0, 1, 2, 0, 2, 3};

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

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
    _fisheyeShader->setUniformBool("u_useFXAA", !GlFrameBufferObject::hasOpenGLFramebufferBlit());
    _fisheyeShader->setUniformFloat("u_fisheyeRadius", fisheyeRadius);
    _fisheyeShader->setUniformFloat("u_fisheyeHeight", -fisheyeHeight);
    _fisheyeShader->setUniformFloat("u_fisheyeTextureSize", fboSize);
    _indicesBuffer->bind();
    _indicesBuffer->allocate(indices);
    GlTextureManager::instance()->bindTexture(glScene->getBackBufferTextureName());
    GlTextureManager::instance()->bindTexture(_fboTextureId, 1);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
    GlTextureManager::instance()->unbindTexture(glScene->getBackBufferTextureName());
    GlTextureManager::instance()->unbindTexture(_fboTextureId);
    _fisheyeShader->desactivate();
    _indicesBuffer->release();
    _buffer->release();

    camera->initGl();

    return true;
  }

  return false;
}
