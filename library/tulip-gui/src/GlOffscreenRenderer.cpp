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
#include <Windows.h>
#endif

#include <GL/glew.h>

#include <QGLFramebufferObject>

#include <tulip/Camera.h>
#include <tulip/GlOffscreenRenderer.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/OpenGlConfigManager.h>

#include <sstream>

using namespace std;

namespace tlp {

GlOffscreenRenderer *GlOffscreenRenderer::instance(NULL);

GlOffscreenRenderer *GlOffscreenRenderer::getInstance() {
  if (instance == NULL) {
    instance = new GlOffscreenRenderer();
  }

  return instance;
}

GlOffscreenRenderer::GlOffscreenRenderer()
  : vPWidth(512), vPHeight(512), glFrameBuf(NULL), glFrameBuf2(NULL), mainLayer(new GlLayer("Main")),
    entitiesCpt(0), zoomFactor(DBL_MAX), cameraCenter(FLT_MAX, FLT_MAX, FLT_MAX) {
  GlLayer *backgroundLayer=new GlLayer("Background");
  backgroundLayer->setVisible(true);
  GlLayer *foregroundLayer=new GlLayer("Foreground");
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
  clearScene();
  delete mainLayer;
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

void GlOffscreenRenderer::addGlEntityToScene(GlSimpleEntity *entity) {
  ostringstream oss;
  oss << "entity " << ++entitiesCpt;
  mainLayer->addGlEntity(entity, oss.str());
}

void GlOffscreenRenderer::addGraphToScene(Graph* graph) {
  addGraphCompositeToScene(new GlGraphComposite(graph));
}

void GlOffscreenRenderer::addGraphCompositeToScene(GlGraphComposite *graphComposite) {
  //Delete old composite if it exist
  GlSimpleEntity *oldComposite = mainLayer->findGlEntity("graph");

  if(oldComposite!=NULL) {
    mainLayer->deleteGlEntity(oldComposite);
  }

  GlVertexArrayManager *vertexArrayManager=graphComposite->getInputData()->getGlVertexArrayManager();
  vertexArrayManager->setHaveToComputeAll(true);
  mainLayer->addGlEntity(graphComposite, "graph");
}

void GlOffscreenRenderer::clearScene() {
  mainLayer->getComposite()->reset(false);
  const vector<pair<string, GlLayer *> > &layersList = scene.getLayersList();

  for (unsigned int i = 0 ; i < layersList.size() ; ++i) {
    if (layersList[i].second != mainLayer) {
      layersList[i].second->getComposite()->reset(true);
    }
  }

  entitiesCpt = 0;
  zoomFactor = DBL_MAX;
}

void GlOffscreenRenderer::initFrameBuffers(const bool antialiased) {

#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0)) && (!defined(__APPLE__) || (defined(__APPLE__) && defined(QT_MAC_USE_COCOA)))
  antialiasedFbo = antialiased && QGLFramebufferObject::hasOpenGLFramebufferBlit();
#endif

  if (glFrameBuf != NULL && (vPWidth != static_cast<unsigned int>(glFrameBuf->width()) || vPHeight != static_cast<unsigned int>(glFrameBuf->height()))) {
    delete glFrameBuf;
    glFrameBuf = NULL;
    delete glFrameBuf2;
    glFrameBuf2 = NULL;
  }


  if (glFrameBuf == NULL) {
#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0)) && (!defined(__APPLE__) || (defined(__APPLE__) && defined(QT_MAC_USE_COCOA)))
    QGLFramebufferObjectFormat fboFmt;
    fboFmt.setAttachment(QGLFramebufferObject::CombinedDepthStencil);

    if (antialiasedFbo)
      fboFmt.setSamples(8);

    glFrameBuf = new QGLFramebufferObject(vPWidth, vPHeight, fboFmt);
  }


  if (antialiasedFbo && glFrameBuf2 == NULL) {
    glFrameBuf2 = new QGLFramebufferObject(vPWidth, vPHeight);
  }


#else
    glFrameBuf = new QGLFramebufferObject(vPWidth, vPHeight, QGLFramebufferObject::CombinedDepthStencil);
  }
#endif
}

void GlOffscreenRenderer::renderScene(const bool centerScene, const bool antialiased) {

  //If no OpenGL context, activate the default one in order to avoid segfault when trying to render an OpenGL scene
  if (!QGLContext::currentContext()) {
    QGLWidget *firstWidget = GlMainWidget::getFirstQGLWidget();
    firstWidget->makeCurrent();
  }

  initFrameBuffers(antialiased);

  scene.setViewport(0,0,vPWidth, vPHeight);

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();


  Camera &camera = mainLayer->getCamera();


  glFrameBuf->bind();

  if (centerScene) {
    scene.centerScene();
  }

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

#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0)) && (!defined(__APPLE__) || (defined(__APPLE__) && defined(QT_MAC_USE_COCOA)))

  if (antialiasedFbo)
    QGLFramebufferObject::blitFramebuffer(glFrameBuf2, QRect(0,0,glFrameBuf2->width(), glFrameBuf2->height()), glFrameBuf, QRect(0,0,glFrameBuf->width(), glFrameBuf->height()));

#endif

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glPopAttrib();
}

void GlOffscreenRenderer::renderExternalScene(GlScene *scene, const bool antialiased) {
  //If no OpenGL context, activate the default one in order to avoid segfault when trying to render an OpenGL scene
  if (!QGLContext::currentContext()) {
    QGLWidget *firstWidget = GlMainWidget::getFirstQGLWidget();
    firstWidget->makeCurrent();
  }

  initFrameBuffers(antialiased);

  Vector<int,4> backupViewport=scene->getViewport();

  scene->setViewport(0,0,vPWidth, vPHeight);

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glFrameBuf->bind();
  scene->draw();
  glFrameBuf->release();

#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0)) && (!defined(__APPLE__) || (defined(__APPLE__) && defined(QT_MAC_USE_COCOA)))

  if (antialiasedFbo)
    QGLFramebufferObject::blitFramebuffer(glFrameBuf2, QRect(0,0,glFrameBuf2->width(), glFrameBuf2->height()), glFrameBuf, QRect(0,0,glFrameBuf->width(), glFrameBuf->height()));

#endif

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glPopAttrib();

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

  bool canUseMipmaps = OpenGlConfigManager::getInst().isExtensionSupported("GL_ARB_framebuffer_object") ||
                       OpenGlConfigManager::getInst().isExtensionSupported("GL_EXT_framebuffer_object");

  GLuint textureId = 0;
  glGenTextures(1, &textureId);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureId);

  if (generateMipMaps && canUseMipmaps) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  }
  else {
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
