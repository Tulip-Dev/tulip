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

#if defined(_MSC_VER)
#include <windows.h>
#endif

#include <GL/glew.h>

#include <QGLFramebufferObject>

#include <tulip/Camera.h>
#include <tulip/GlOffscreenRenderer.h>
#include <tulip/GlMainWidget.h>
#include <tulip/OpenGlConfigManager.h>
#include <tulip/GlLayer.h>
#include <tulip/GlGraph.h>

#include <sstream>
#include <cfloat>

using namespace std;

namespace tlp {

GlOffscreenRenderer *GlOffscreenRenderer::instance(nullptr);

GlOffscreenRenderer *GlOffscreenRenderer::getInstance() {
  if (instance == nullptr) {
    instance = new GlOffscreenRenderer();
  }

  return instance;
}

GlOffscreenRenderer::GlOffscreenRenderer()
    : vPWidth(512), vPHeight(512), glFrameBuf(nullptr), glFrameBuf2(nullptr), entitiesCpt(0), zoomFactor(DBL_MAX),
      cameraCenter(FLT_MAX, FLT_MAX, FLT_MAX) {
  mainLayer = scene.getMainLayer();
  GlLayer *backgroundLayer = new GlLayer("Background", false);
  backgroundLayer->setVisible(true);
  GlLayer *foregroundLayer = new GlLayer("Foreground", false);
  foregroundLayer->setVisible(true);
  scene.addExistingLayerBefore(backgroundLayer, mainLayer);
  scene.addExistingLayerAfter(foregroundLayer, mainLayer);
  antialiasedFbo = false;
}

GlOffscreenRenderer::~GlOffscreenRenderer() {
  delete glFrameBuf;
  delete glFrameBuf2;
  clearScene();
}

void GlOffscreenRenderer::setViewPortSize(const unsigned int viewPortWidth, const unsigned int viewPortHeight) {
  vPWidth = viewPortWidth;
  vPHeight = viewPortHeight;
}

void GlOffscreenRenderer::setSceneBackgroundColor(const Color &color) {
  scene.setBackgroundColor(color);
}

unsigned int GlOffscreenRenderer::getViewportWidth() {
  return glFrameBuf->width();
}

unsigned int GlOffscreenRenderer::getViewportHeight() {
  return glFrameBuf->height();
}

void GlOffscreenRenderer::addGlEntityToScene(GlEntity *entity) {
  ostringstream oss;
  oss << "entity " << ++entitiesCpt;
  mainLayer->addGlEntity(entity, oss.str());
}

void GlOffscreenRenderer::addGraphToScene(Graph *graph) {
  scene.addMainGlGraph(graph);
}

void GlOffscreenRenderer::clearScene() {
  mainLayer->clear(false);

  for (GlLayer *layer : scene.getLayersList()) {
    if (layer != mainLayer) {
      layer->clear(true);
    }
  }

  entitiesCpt = 0;
  zoomFactor = DBL_MAX;
}

void GlOffscreenRenderer::initFrameBuffers(const bool antialiased) {

#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0)) &&                                                                                                     \
    (!defined(__APPLE__) || (defined(__APPLE__) && ((QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)) || defined(QT_MAC_USE_COCOA))))
  antialiasedFbo = antialiased && QGLFramebufferObject::hasOpenGLFramebufferBlit();
#endif

  if (glFrameBuf != nullptr &&
      (vPWidth != static_cast<unsigned int>(glFrameBuf->width()) || vPHeight != static_cast<unsigned int>(glFrameBuf->height()))) {
    delete glFrameBuf;
    glFrameBuf = nullptr;
    delete glFrameBuf2;
    glFrameBuf2 = nullptr;
  }

  if (glFrameBuf == nullptr) {
#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0)) &&                                                                                                     \
    (!defined(__APPLE__) || (defined(__APPLE__) && ((QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)) || defined(QT_MAC_USE_COCOA))))
    QGLFramebufferObjectFormat fboFmt;
    fboFmt.setAttachment(QGLFramebufferObject::CombinedDepthStencil);

    if (antialiasedFbo)
      fboFmt.setSamples(OpenGlConfigManager::instance().maxNumberOfSamples());

    glFrameBuf = new QGLFramebufferObject(vPWidth, vPHeight, fboFmt);
  }

  if (antialiasedFbo && glFrameBuf2 == nullptr) {
    glFrameBuf2 = new QGLFramebufferObject(vPWidth, vPHeight);
  }

#else
    glFrameBuf = new QGLFramebufferObject(vPWidth, vPHeight, QGLFramebufferObject::CombinedDepthStencil);
  }
#endif
}

void GlOffscreenRenderer::renderScene(const bool centerScene, const bool antialiased) {

  // If no OpenGL context, activate the default one in order to avoid segfault when trying to render an OpenGL scene
  if (!QGLContext::currentContext()) {
    QGLWidget *firstWidget = GlMainWidget::getFirstQGLWidget();
    firstWidget->makeCurrent();
  }

  initFrameBuffers(antialiased);

  scene.setViewport(0, 0, vPWidth, vPHeight);

  Camera *camera = mainLayer->getCamera();

  glFrameBuf->bind();

  if (centerScene) {
    scene.centerScene();
  }

  if (cameraCenter != Coord(FLT_MAX, FLT_MAX, FLT_MAX)) {
    camera->setCenter(cameraCenter);
    camera->setEyes(Coord(0, 0, camera->getSceneRadius()));
    camera->setEyes(camera->getEyes() + camera->getCenter());
    camera->setUp(Coord(0, 1., 0));
  }

  if (zoomFactor != DBL_MAX) {
    camera->setZoomFactor(zoomFactor);
  }

  scene.draw();
  glFrameBuf->release();

#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0)) &&                                                                                                     \
    (!defined(__APPLE__) || (defined(__APPLE__) && ((QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)) || defined(QT_MAC_USE_COCOA))))

  if (antialiasedFbo)
    QGLFramebufferObject::blitFramebuffer(glFrameBuf2, QRect(0, 0, glFrameBuf2->width(), glFrameBuf2->height()), glFrameBuf,
                                          QRect(0, 0, glFrameBuf->width(), glFrameBuf->height()));

#endif
}

void GlOffscreenRenderer::renderExternalScene(GlScene *scene, const bool antialiased) {
  // If no OpenGL context, activate the default one in order to avoid segfault when trying to render an OpenGL scene
  if (!QGLContext::currentContext()) {
    QGLWidget *firstWidget = GlMainWidget::getFirstQGLWidget();
    firstWidget->makeCurrent();
  }

  initFrameBuffers(antialiased);

  Vector<int, 4> backupViewport = scene->getViewport();

  scene->setViewport(0, 0, vPWidth, vPHeight);

  glFrameBuf->bind();
  scene->draw();
  glFrameBuf->release();

#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0)) &&                                                                                                     \
    (!defined(__APPLE__) || (defined(__APPLE__) && ((QT_VERSION > QT_VERSION_CHECK(5, 0, 0)) || defined(QT_MAC_USE_COCOA))))

  if (antialiasedFbo)
    QGLFramebufferObject::blitFramebuffer(glFrameBuf2, QRect(0, 0, glFrameBuf2->width(), glFrameBuf2->height()), glFrameBuf,
                                          QRect(0, 0, glFrameBuf->width(), glFrameBuf->height()));

#endif

  scene->setViewport(backupViewport);
}

bool GlOffscreenRenderer::frameBufferOk() const {
  return glFrameBuf->isValid();
}

static QImage convertImage(const QImage &image) {
  return QImage(image.bits(), image.width(), image.height(), QImage::Format_ARGB32).convertToFormat(QImage::Format_RGB32);
}

QImage GlOffscreenRenderer::getImage() {
  if (!antialiasedFbo)
    return convertImage(glFrameBuf->toImage());
  else
    return convertImage(glFrameBuf2->toImage());
}

GLuint GlOffscreenRenderer::getGLTexture(const bool generateMipMaps) {

  bool canUseMipmaps = OpenGlConfigManager::instance().isExtensionSupported("GL_ARB_framebuffer_object") ||
                       OpenGlConfigManager::instance().isExtensionSupported("GL_EXT_framebuffer_object");

  GLuint textureId = 0;
  glGenTextures(1, &textureId);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureId);

  if (generateMipMaps && canUseMipmaps) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  } else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  QImage image = getImage().mirrored();
  unsigned char *buff = image.bits();

  glBindTexture(GL_TEXTURE_2D, textureId);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, getViewportWidth(), getViewportHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, buff);

  if (generateMipMaps && canUseMipmaps) {
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  glDisable(GL_TEXTURE_2D);

  return textureId;
}
}
