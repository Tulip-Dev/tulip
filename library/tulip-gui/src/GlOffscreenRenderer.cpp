/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

// remove warnings about qt5/glew incompatibility
// as we do not rely on QOpenGLFunctions for rendering
#undef __GLEW_H__
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#define __GLEW_H__

#include <QOffscreenSurface>

#include <tulip/Camera.h>
#include <tulip/GlOffscreenRenderer.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/OpenGlConfigManager.h>
#include <tulip/GlTools.h>

#include <sstream>

using namespace std;

namespace tlp {

GlOffscreenRenderer *GlOffscreenRenderer::instance(new GlOffscreenRenderer);

GlOffscreenRenderer::GlOffscreenRenderer()
    : glContext(nullptr), offscreenSurface(nullptr), vPWidth(512), vPHeight(512),
      glFrameBuf(nullptr), glFrameBuf2(nullptr), mainLayer(new GlLayer("Main")), entitiesCpt(0),
      zoomFactor(DBL_MAX), cameraCenter(FLT_MAX, FLT_MAX, FLT_MAX) {
  GlLayer *backgroundLayer = new GlLayer("Background");
  backgroundLayer->setVisible(true);
  GlLayer *foregroundLayer = new GlLayer("Foreground");
  foregroundLayer->setVisible(true);
  backgroundLayer->set2DMode();
  foregroundLayer->set2DMode();
  scene.addExistingLayer(backgroundLayer);
  scene.addExistingLayer(mainLayer);
  scene.addExistingLayer(foregroundLayer);
  antialiasedFbo = false;
}

GlOffscreenRenderer::~GlOffscreenRenderer() {
  delete glFrameBuf;
  delete glFrameBuf2;
  delete offscreenSurface;
  delete glContext;
}

void GlOffscreenRenderer::setViewPortSize(const unsigned int viewPortWidth,
                                          const unsigned int viewPortHeight) {
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

void GlOffscreenRenderer::addGlEntityToScene(GlSimpleEntity *entity) {
  ostringstream oss;
  oss << "entity " << ++entitiesCpt;
  mainLayer->addGlEntity(entity, oss.str());
}

void GlOffscreenRenderer::addGraphToScene(Graph *graph) {
  addGraphCompositeToScene(new GlGraphComposite(graph));
}

void GlOffscreenRenderer::addGraphCompositeToScene(GlGraphComposite *graphComposite) {
  // Delete old composite if it exist
  GlSimpleEntity *oldComposite = mainLayer->findGlEntity("graph");

  if (oldComposite != nullptr) {
    mainLayer->deleteGlEntity(oldComposite);
  }

  GlVertexArrayManager *vertexArrayManager =
      graphComposite->getInputData()->getGlVertexArrayManager();
  vertexArrayManager->setHaveToComputeAll(true);
  mainLayer->addGlEntity(graphComposite, "graph");
}

void GlOffscreenRenderer::clearScene(bool deleteGlEntities) {
  mainLayer->getComposite()->reset(deleteGlEntities);
  const vector<pair<string, GlLayer *>> &layersList = scene.getLayersList();

  for (unsigned int i = 0; i < layersList.size(); ++i) {
    if (layersList[i].second != mainLayer) {
      layersList[i].second->getComposite()->reset(true);
    }
  }

  entitiesCpt = 0;
  zoomFactor = DBL_MAX;
}

void GlOffscreenRenderer::initFrameBuffers(const bool antialiased) {

  antialiasedFbo = antialiased && QOpenGLFramebufferObject::hasOpenGLFramebufferBlit();

  if (glFrameBuf != nullptr &&
      (vPWidth != uint(glFrameBuf->width()) || vPHeight != uint(glFrameBuf->height()))) {
    delete glFrameBuf;
    glFrameBuf = nullptr;
    delete glFrameBuf2;
    glFrameBuf2 = nullptr;
  }

  if (glFrameBuf == nullptr) {
    QOpenGLFramebufferObjectFormat fboFmt;
    fboFmt.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);

    if (antialiasedFbo)
      fboFmt.setSamples(OpenGlConfigManager::maxNumberOfSamples());

    glFrameBuf = new QOpenGLFramebufferObject(vPWidth, vPHeight, fboFmt);
  }

  if (antialiasedFbo && glFrameBuf2 == nullptr) {
    glFrameBuf2 = new QOpenGLFramebufferObject(vPWidth, vPHeight);
  }
}

void GlOffscreenRenderer::renderScene(const bool centerScene, const bool antialiased) {

  makeOpenGLContextCurrent();

  initFrameBuffers(antialiased);

  scene.setViewport(0, 0, vPWidth, vPHeight);

  glFrameBuf->bind();

  if (centerScene) {
    scene.centerScene();
  }

  Camera &camera = mainLayer->getCamera();

  if (cameraCenter != Coord(FLT_MAX, FLT_MAX, FLT_MAX)) {
    camera.setCenter(cameraCenter);
    camera.setEyes(Coord(0, 0, camera.getSceneRadius()));
    camera.setEyes(camera.getEyes() + camera.getCenter());
    camera.setUp(Coord(0, 1., 0));
  }

  if (zoomFactor != DBL_MAX) {
    camera.setZoomFactor(zoomFactor);
  }

  scene.draw();
  glFrameBuf->release();

  if (antialiasedFbo)
    QOpenGLFramebufferObject::blitFramebuffer(
        glFrameBuf2, QRect(0, 0, glFrameBuf2->width(), glFrameBuf2->height()), glFrameBuf,
        QRect(0, 0, glFrameBuf->width(), glFrameBuf->height()));
}

void GlOffscreenRenderer::renderExternalScene(GlScene *scene, const bool antialiased) {

  makeOpenGLContextCurrent();

  initFrameBuffers(antialiased);

  Vector<int, 4> backupViewport = scene->getViewport();

  scene->setViewport(0, 0, vPWidth, vPHeight);

  glFrameBuf->bind();
  scene->draw();
  glFrameBuf->release();

  if (antialiasedFbo)
    QOpenGLFramebufferObject::blitFramebuffer(
        glFrameBuf2, QRect(0, 0, glFrameBuf2->width(), glFrameBuf2->height()), glFrameBuf,
        QRect(0, 0, glFrameBuf->width(), glFrameBuf->height()));

  scene->setViewport(backupViewport);
}

static inline QImage convertImage(const QImage &image, bool alpha) {
  QImage qimg(image.constBits(), image.width(), image.height(), QImage::Format_ARGB32);
  return alpha ? qimg : qimg.convertToFormat(QImage::Format_RGB32);
}

QImage GlOffscreenRenderer::getImage(bool alpha) {
  makeOpenGLContextCurrent();
  if (!antialiasedFbo)
    return convertImage(glFrameBuf->toImage(), alpha);
  else
    return convertImage(glFrameBuf2->toImage(), alpha);
}

GLuint GlOffscreenRenderer::getGLTexture(const bool generateMipMaps) {

  makeOpenGLContextCurrent();

  bool canUseMipmaps = OpenGlConfigManager::isExtensionSupported("GL_ARB_framebuffer_object") ||
                       OpenGlConfigManager::isExtensionSupported("GL_EXT_framebuffer_object");

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

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, getViewportWidth(), getViewportHeight(), 0, GL_BGRA,
               GL_UNSIGNED_BYTE, buff);

  if (generateMipMaps && canUseMipmaps) {
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  GL_TEST_ERROR();
  glDisable(GL_TEXTURE_2D);
  GL_TEST_ERROR();

  return textureId;
}

QOpenGLContext *GlOffscreenRenderer::getOpenGLContext() {
  if (!glContext) {
    glContext = new QOpenGLContext();
    offscreenSurface = new QOffscreenSurface();
    glContext->create();
    offscreenSurface->create();
    assert(glContext->isValid());
    makeOpenGLContextCurrent();
    OpenGlConfigManager::initExtensions();
    doneOpenGLContextCurrent();
  }
  return glContext;
}

void GlOffscreenRenderer::makeOpenGLContextCurrent() {
  getOpenGLContext()->makeCurrent(offscreenSurface);
}

void GlOffscreenRenderer::doneOpenGLContextCurrent() {
  getOpenGLContext()->doneCurrent();
}

bool GlOffscreenRenderer::isValid() {
  return getOpenGLContext()->isValid();
}

QImage GlOffscreenRenderer::renderGlMainWidget(GlMainWidget *glWidget, bool redrawNeeded) {
  setViewPortSize(glWidget->screenToViewport(glWidget->width()),
                  glWidget->screenToViewport(glWidget->height()));
  makeOpenGLContextCurrent();
  initFrameBuffers(true);
  glFrameBuf2->bind();
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glWidget->render(redrawNeeded ? GlMainWidget::RenderingOptions(GlMainWidget::RenderScene)
                                : GlMainWidget::RenderingOptions(),
                   false);
  glPopAttrib();
  glFrameBuf2->release();

  QImage img = getImage();
  img.setDevicePixelRatio(glWidget->devicePixelRatio());

  return img;
}

} // namespace tlp
