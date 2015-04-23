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
#include "../../utils/ViewNames.h"

#include <QMouseEvent>

#include <tulip/TulipPluginHeaders.h>
#include <tulip/MouseInteractors.h>
#include <tulip/GlMainView.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/GlShaderProgram.h>
#include <tulip/GlMainWidget.h>
#include <tulip/NodeLinkDiagramComponent.h>

using namespace std;
using namespace tlp;

string fisheyeVertexProgram =
  "#version 120\n"
  "uniform vec4 center;"
  "uniform float radius;"
  "uniform float height;"
  "uniform int fisheyeType;"
  ""
  "void main() {"
  "gl_Position = gl_ModelViewMatrix * gl_Vertex;"
  "float dist = distance(center, gl_Position);"
  ""
  "if (fisheyeType == 1) {"
  "	if (dist < radius) {"
  "		float coeff = (height + 1.) * dist / (height * dist/ radius + 1.);"
  "		vec4 dir = normalize(gl_Position - center) * coeff;"
  "		gl_Position = gl_ProjectionMatrix * (center + dir);"
  "	} else {"
  "		gl_Position = ftransform();"
  "	}"
  "} else if (fisheyeType == 2) {"
  "	float coeff = dist+dist*radius/(dist*dist+1.+radius/height);"
  "	vec4 dir = normalize(gl_Position - center) * coeff;"
  "	gl_Position = gl_ProjectionMatrix * (center + dir);"
  "} else {"
  "	if (dist < radius) {"
  "		gl_Position = gl_ProjectionMatrix * (center + height * (gl_Position - center));"
  "	} else {"
  "		gl_Position = gl_ProjectionMatrix * (center + (1. + radius * (height - 1.) / dist) * (gl_Position - center));"
  "	}"
  "}"
  ""
  "gl_FrontColor =  gl_Color;"
  "gl_TexCoord[0] = gl_MultiTexCoord0;"
  "}";

FishEyeInteractor::FishEyeInteractor(const PluginContext *) : GLInteractorComposite(QIcon(":/i_fisheye.png"), "Fisheye"),fisheyeConfigWidget(NULL) {}

void FishEyeInteractor::construct() {
  fisheyeConfigWidget = new FishEyeConfigWidget();
  push_back(new MousePanNZoomNavigator());
  FishEyeInteractorComponent *fisheyeInteractorComponent = new FishEyeInteractorComponent();
  fisheyeInteractorComponent->setConfigWidget(fisheyeConfigWidget);
  push_back(fisheyeInteractorComponent);
}

bool FishEyeInteractor::isCompatible(const std::string &viewName) const {
  return ((viewName==NodeLinkDiagramComponent::viewName)
          ||(viewName==ViewName::HistogramViewName)
          ||(viewName==ViewName::MatrixViewName)
          ||(viewName==ViewName::ParallelCoordinatesViewName)
          ||(viewName==ViewName::PixelOrientedViewName)
          ||(viewName==ViewName::ScatterPlot2DViewName)
         );
}

void FishEyeInteractor::uninstall() {
  InteractorComposite::uninstall();
}

FishEyeInteractor::~FishEyeInteractor() {
  delete fisheyeConfigWidget;
}

QWidget *FishEyeInteractor::configurationWidget() const {
  return fisheyeConfigWidget;
}

PLUGIN(FishEyeInteractor)

FishEyeInteractorComponent::FishEyeInteractorComponent() : fisheyeShader(NULL) {}

FishEyeInteractorComponent::FishEyeInteractorComponent(const FishEyeInteractorComponent &fisheyeInteractorComponent) : fisheyeShader(NULL) {
  fisheyeCenter = fisheyeInteractorComponent.fisheyeCenter;
  configWidget = fisheyeInteractorComponent.configWidget;
}

FishEyeInteractorComponent::~FishEyeInteractorComponent() {
  delete fisheyeShader;
}

void FishEyeInteractorComponent::viewChanged(View *view) {
  if (view == NULL) {
    return;
  }

  GlMainView *glView = (GlMainView *) view;
  GlMainWidget *glWidget = glView->getGlMainWidget();

  if (!glWidget->hasMouseTracking()) {
    glWidget->setMouseTracking(true);
  }

  if (configWidget->getFishEyeRadius() == 0.) {
    configWidget->setFishEyeRadius(glWidget->getScene()->getGraphCamera().getSceneRadius()/4.);
    configWidget->setFishEyeHeight(4.);
  }
}

bool FishEyeInteractorComponent::eventFilter(QObject *obj, QEvent *e) {

  GlMainWidget *glWidget = (GlMainWidget*)obj;
  Camera *camera=&glWidget->getScene()->getGraphCamera();

  activateFishEye = false;

  if (e->type() == QEvent::MouseMove ||
      e->type() == QEvent::MouseButtonPress ||
      e->type() == QEvent::MouseButtonRelease) {
    activateFishEye = true;
    QMouseEvent *me = (QMouseEvent *) e;
    float x = glWidget->width() - me->x();
    float y = me->y();
    Coord screenCoords(x, y, 0);
    fisheyeCenter = camera->screenTo3DWorld(screenCoords);
    glWidget->redraw();
    return true;
  }
  else if (e->type() == QEvent::Wheel) {
    activateFishEye = true;
    QWheelEvent *wheelEvent = (QWheelEvent *) e;
    int numDegrees = wheelEvent->delta() / 8;
    int numSteps = numDegrees / 15;

    if (wheelEvent->orientation() == Qt::Vertical && (wheelEvent->modifiers() == Qt::ControlModifier)) {
      activateFishEye = true;
      configWidget->setFishEyeRadius(configWidget->getFishEyeRadius() + configWidget->getFishEyeRadiusIncrementStep() * numSteps);
      glWidget->redraw();
      return true;
    }
    else if (wheelEvent->orientation() == Qt::Vertical && (wheelEvent->modifiers() == Qt::ShiftModifier)) {
      activateFishEye = true;
      float newHeight = configWidget->getFishEyeHeight() + configWidget->getFishEyeHeightIncrementStep() * numSteps;

      if (newHeight < 0) {
        newHeight = 0;
      }

      configWidget->setFishEyeHeight(newHeight);
      glWidget->redraw();
      return true;
    }

    return false;
  }

  return false;

}

bool FishEyeInteractorComponent::draw(GlMainWidget *glWidget) {
  Camera *camera=&glWidget->getScene()->getGraphCamera();

  if (GlShaderProgram::shaderProgramsSupported()) {
    fisheyeShader = new GlShaderProgram("fisheye");
    fisheyeShader->addShaderFromSourceCode(Vertex, fisheyeVertexProgram);
    fisheyeShader->link();
  }

  if (activateFishEye && fisheyeShader != NULL && fisheyeShader->isLinked()) {
    fisheyeShader->activate();

    Matrix<float, 4> modelViewMatrix;
    camera->getModelviewMatrix(modelViewMatrix);
    Vector<float, 4> centerCoord;
    centerCoord[0] = fisheyeCenter[0];
    centerCoord[1] = fisheyeCenter[1];
    centerCoord[2] = 0;
    centerCoord[3] = 1;
    centerCoord = centerCoord * modelViewMatrix;
    float radius = configWidget->getFishEyeRadius();
    int fishEyeType = configWidget->getFishEyeType();

    if (fishEyeType == 2) {
      radius = (radius * radius) / 2.0;
    }
    else if (fishEyeType == 3) {
      radius = radius / 4.0;
    }

    float height = configWidget->getFishEyeHeight();

    fisheyeShader->setUniformVec4Float("center", centerCoord);
    fisheyeShader->setUniformFloat("radius", radius);
    fisheyeShader->setUniformFloat("height", height);
    fisheyeShader->setUniformInt("fisheyeType", fishEyeType);

    bool clearBufferAtDraw =  glWidget->getScene()->getClearBufferAtDraw();

    glWidget->getScene()->setClearBufferAtDraw(true);
    glWidget->getScene()->draw();
    glWidget->getScene()->setClearBufferAtDraw(clearBufferAtDraw);

    fisheyeShader->desactivate();


    return true;
  }

  return false;
}
