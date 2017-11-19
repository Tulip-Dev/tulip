/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include "MouseMagnifyingGlass.h"
#include "../../utils/PluginNames.h"

#include <tulip/GlRect.h>
#include <tulip/GlMainView.h>
#include <tulip/GlTextureManager.h>
#include <tulip/MouseInteractors.h>
#include <tulip/GlTools.h>
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/GlCircle.h>
#include <tulip/OpenGlConfigManager.h>

#include <QGLFramebufferObject>
#include <QEvent>
#include <QMouseEvent>

#include <sstream>

#include <cstdint>

using namespace std;
using namespace tlp;

MouseMagnifyingGlassInteractor::MouseMagnifyingGlassInteractor(const tlp::PluginContext *)
    : GLInteractorComposite(QIcon(":/i_magnifying_glass.png"), "Magnifying glass") {}

void MouseMagnifyingGlassInteractor::construct() {
  push_back(new MousePanNZoomNavigator());
  push_back(new MouseMagnifyingGlassInteractorComponent());
}

bool MouseMagnifyingGlassInteractor::isCompatible(const std::string &viewName) const {
  return ((viewName == NodeLinkDiagramComponent::viewName) ||
          (viewName == ViewName::HistogramViewName) || (viewName == ViewName::MatrixViewName) ||
          (viewName == ViewName::ParallelCoordinatesViewName) ||
          (viewName == ViewName::PixelOrientedViewName) ||
          (viewName == ViewName::ScatterPlot2DViewName));
}

PLUGIN(MouseMagnifyingGlassInteractor)

MouseMagnifyingGlassInteractorComponent::MouseMagnifyingGlassInteractorComponent()
    : fbo(nullptr), fbo2(nullptr), glWidget(nullptr), camera(nullptr), drawInteractor(false),
      radius(200), magnifyPower(2) {}

MouseMagnifyingGlassInteractorComponent::MouseMagnifyingGlassInteractorComponent(
    const MouseMagnifyingGlassInteractorComponent &mouseMagnifyingGlassInteractorComponent)
    : fbo(nullptr), fbo2(nullptr), glWidget(nullptr), camera(nullptr), drawInteractor(false) {
  boxCenter = mouseMagnifyingGlassInteractorComponent.boxCenter;
  radius = mouseMagnifyingGlassInteractorComponent.radius;
  magnifyPower = mouseMagnifyingGlassInteractorComponent.magnifyPower;
}

MouseMagnifyingGlassInteractorComponent::~MouseMagnifyingGlassInteractorComponent() {
  delete fbo;
  delete fbo2;
  GlTextureManager::getInst().deleteTexture(textureName);
}

bool MouseMagnifyingGlassInteractorComponent::eventFilter(QObject *, QEvent *e) {

  bool updateMagnifyingGlass = false;
  Coord screenCoords;

  if (e->type() == QEvent::MouseMove) {
    QMouseEvent *me = static_cast<QMouseEvent *>(e);
    float x = glWidget->width() - me->x();
    float y = me->y();
    screenCoords = Coord(x, y, 0);
    boxCenter = camera->viewportTo3DWorld(glWidget->screenToViewport(screenCoords));

    updateMagnifyingGlass = true;
  } else if (e->type() == QEvent::Wheel) {
    QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(e);
    float x = glWidget->width() - wheelEvent->x();
    float y = wheelEvent->y();
    screenCoords = Coord(x, y, 0);
    boxCenter = camera->viewportTo3DWorld(glWidget->screenToViewport(screenCoords));
    int numDegrees = wheelEvent->delta() / 8;
    int numSteps = numDegrees / 15;

    if (wheelEvent->orientation() == Qt::Vertical &&
        (wheelEvent->modifiers() == Qt::ControlModifier)) {
      updateMagnifyingGlass = true;
      radius += numSteps;

      if (radius < 1)
        radius = 1;

      delete fbo;
      delete fbo2;
      fbo = nullptr;
      fbo2 = nullptr;
    } else if (wheelEvent->orientation() == Qt::Vertical &&
               (wheelEvent->modifiers() == Qt::ShiftModifier)) {
      updateMagnifyingGlass = true;
      magnifyPower += numSteps;

      if (magnifyPower < 1)
        magnifyPower = 1;
    }
  }

  static bool canUseFbo = QGLFramebufferObject::hasOpenGLFramebufferObjects();

  if (canUseFbo && updateMagnifyingGlass) {
    generateMagnifyingGlassTexture(screenCoords);
    drawInteractor = true;
    glWidget->redraw();
    return true;
  }

  return false;
}

void MouseMagnifyingGlassInteractorComponent::generateMagnifyingGlassTexture(
    const Coord &magnifyingGlassCenterScr) {

  bool antialiased = false;

  bool canUseMultisampleFbo =
      OpenGlConfigManager::getInst().isExtensionSupported("GL_EXT_framebuffer_multisample");

  if (QGLFramebufferObject::hasOpenGLFramebufferBlit() && canUseMultisampleFbo) {
    antialiased = true;
  }

  int fboSize = int(radius * 2);

  // instantiate fbo if needed
  if (fbo == nullptr) {
    QGLFramebufferObjectFormat fboFormat;
    fboFormat.setAttachment(QGLFramebufferObject::CombinedDepthStencil);

    if (antialiased) {
      fboFormat.setSamples(OpenGlConfigManager::getInst().maxNumberOfSamples());
    }

    fbo = new QGLFramebufferObject(fboSize, fboSize, fboFormat);

    if (antialiased) {
      fbo2 = new QGLFramebufferObject(fboSize, fboSize);
    }

    if (!antialiased) {
      GlTextureManager::getInst().registerExternalTexture(textureName, fbo->texture());
    } else {
      GlTextureManager::getInst().registerExternalTexture(textureName, fbo2->texture());
    }
  }

  Vector<int, 4> viewport = glWidget->getScene()->getViewport();

  // get the magnifying glass bounding box in screen space
  BoundingBox boundingBox;
  boundingBox[0] =
      Coord(magnifyingGlassCenterScr.getX() - radius, magnifyingGlassCenterScr.getY() - radius);
  boundingBox[1] =
      Coord(magnifyingGlassCenterScr.getX() + radius, magnifyingGlassCenterScr.getY() + radius);

  // compute the zoom factor to apply to scene's camera to get the area under the magnifying glass
  // displayed entirely in the viewport
  float bbWidthScreen = boundingBox[1][0] - boundingBox[0][0];
  float bbHeightScreen = boundingBox[1][1] - boundingBox[0][1];
  float startSize = glWidget->screenToViewport(min(glWidget->width(), glWidget->height()));
  float endSize = max(bbHeightScreen, bbWidthScreen);
  float zoomFactor = startSize / endSize;

  // backup current camera parameters
  float sceneRadiusBak = camera->getSceneRadius();
  float zoomFactorBak = camera->getZoomFactor();
  Coord eyesBak = camera->getEyes();
  Coord centerBak = camera->getCenter();
  Coord upBak = camera->getUp();

  Coord move = boxCenter - centerBak;
  camera->setCenter(camera->getCenter() + move);
  camera->setEyes(camera->getEyes() + move);
  camera->setZoomFactor(magnifyPower * zoomFactor * zoomFactorBak);

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  // resize the viewport to the size of fbo and render the scene into this last one
  GlScene *scene = glWidget->getScene();
  scene->setViewport(0, 0, fboSize, fboSize);
  fbo->bind();
  Color color = scene->getBackgroundColor();
  glClearColor(color.getRGL(), color.getGGL(), color.getBGL(), color.getAGL());
  glClear(GL_COLOR_BUFFER_BIT);
  scene->draw();
  fbo->release();

  if (antialiased) {
    QGLFramebufferObject::blitFramebuffer(fbo2, QRect(0, 0, fboSize, fboSize), fbo,
                                          QRect(0, 0, fboSize, fboSize));
  }

  // restore original camera parameters
  scene->setViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
  camera->setSceneRadius(sceneRadiusBak);
  camera->setZoomFactor(zoomFactorBak);
  camera->setEyes(eyesBak);
  camera->setCenter(centerBak);
  camera->setUp(upBak);

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  glPopAttrib();

  // need to call this explicitely otherwise we have to redraw the scene
  glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

bool MouseMagnifyingGlassInteractorComponent::draw(GlMainWidget *glWidget) {
  if (!drawInteractor) {
    return false;
  }

  camera->initGl();
  Coord boxCenterScr = camera->worldTo2DViewport(boxCenter);
  Camera camera2D(camera->getScene(), false);
  camera2D.setScene(camera->getScene());
  camera2D.initGl();
  glDisable(GL_LIGHTING);
  glDisable(GL_BLEND);
  glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glPushMatrix();
  glTranslatef(boxCenterScr.getX(), boxCenterScr.getY(), 0);

  Color outlineColor;
  int bgV = glWidget->getScene()->getBackgroundColor().getV();

  if (bgV < 128) {
    outlineColor = Color(255, 255, 255);
  } else {
    outlineColor = Color(0, 0, 0);
  }

  GlCircle circle(Coord(0, 0, 0), radius, outlineColor, Color::White, true, true, 0.0, 60);
  circle.setOutlineSize(3);
  circle.setTextureName(textureName);
  circle.draw(0, nullptr);

  glPopMatrix();
  drawInteractor = false;
  return true;
}

void MouseMagnifyingGlassInteractorComponent::viewChanged(View *view) {
  if (view == nullptr) {
    glWidget = nullptr;
    return;
  }

  GlMainView *glView = dynamic_cast<GlMainView *>(view);
  glWidget = glView->getGlMainWidget();
  radius = glWidget->screenToViewport(glWidget->width()) / 4;
  camera = &glWidget->getScene()->getLayer("Main")->getCamera();

  if (!glWidget->hasMouseTracking()) {
    glWidget->setMouseTracking(true);
  }

  ostringstream oss;
  oss << "magnifyingglass" << reinterpret_cast<uintptr_t>(glWidget);
  textureName = oss.str();
}
