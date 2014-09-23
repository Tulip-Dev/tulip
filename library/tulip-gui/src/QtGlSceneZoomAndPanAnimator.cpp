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

#include <QApplication>
#include <QTimeLine>

#include <tulip/QtGlSceneZoomAndPanAnimator.h>
#include <tulip/GlMainWidget.h>

namespace tlp {

QtGlSceneZoomAndPanAnimator::QtGlSceneZoomAndPanAnimator(GlMainWidget *glWidget, const BoundingBox &boundingBox, const double duration, const std::string &layerName, const bool optimalPath, const double velocity, const double p) :
  GlSceneZoomAndPan(glWidget->getScene(), boundingBox, layerName, 0, optimalPath, p), glWidget(glWidget), animationDurationMsec(duration) {
  if (doZoomAndPan) {
    animationDurationMsec *= S/velocity;
  }

  nbAnimationSteps = animationDurationMsec / 40 + 1;
}

void QtGlSceneZoomAndPanAnimator::animateZoomAndPan() {
  QTimeLine timeLine(animationDurationMsec);
  timeLine.setFrameRange(0, nbAnimationSteps);
  connect(&timeLine, SIGNAL(frameChanged(int)), this, SLOT(zoomAndPanAnimStepSlot(int)));

  if (doZoomAndPan || (!doZoomAndPan && additionalAnimation != NULL)) {
    timeLine.start();

    while (timeLine.state() != QTimeLine::NotRunning) {
      QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }
  }
}



void QtGlSceneZoomAndPanAnimator::zoomAndPanAnimStepSlot(int animationStep) {
  zoomAndPanAnimationStep(animationStep);
  glWidget->draw();
}

}
