/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#include <QtGui/QApplication>
#include <QtCore/QTimeLine>

#include "tulip/QtGlSceneZoomAndPanAnimator.h"

class MouseEventDiscardFilter : public QObject {

public :

  bool eventFilter(QObject*, QEvent *event) {
    if (event->type() == QEvent::MouseMove ||
        event->type() == QEvent::MouseButtonDblClick ||
        event->type() == QEvent::MouseButtonPress ||
        event->type() == QEvent::ContextMenu ||
        event->type() == QEvent::MouseButtonRelease) {
      return true;
    }

    return false;
  }

};

namespace tlp {

QtGlSceneZoomAndPanAnimator::QtGlSceneZoomAndPanAnimator(GlMainWidget *glWidget, const BoundingBox &boundingBox, const std::string &layerName, const bool optimalPath, const double velocity, const double p) :
  GlSceneZoomAndPan(glWidget->getScene(), boundingBox, layerName, 0, optimalPath, p), glWidget(glWidget), animationDurationMsec(1000) {
  if (doZoomAndPan) {
    animationDurationMsec *= S/velocity;
  }

  nbAnimationSteps = animationDurationMsec / 40 + 1;
}

void QtGlSceneZoomAndPanAnimator::animateZoomAndPan() {
  QTimeLine timeLine(animationDurationMsec);
  timeLine.setFrameRange(0, nbAnimationSteps);
  connect(&timeLine, SIGNAL(frameChanged(int)), this, SLOT(zoomAndPanAnimStepSlot(int)));
  MouseEventDiscardFilter medf;
  glWidget->installEventFilter(&medf);

  if (doZoomAndPan || (!doZoomAndPan && additionalAnimation != NULL)) {
    timeLine.start();

    while (timeLine.state() != QTimeLine::NotRunning) {
      QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }
  }

  glWidget->removeEventFilter(&medf);
}

void QtGlSceneZoomAndPanAnimator::setDuration(const double duration) {
  animationDurationMsec = duration;
}

void QtGlSceneZoomAndPanAnimator::zoomAndPanAnimStepSlot(int animationStep) {
  zoomAndPanAnimationStep(animationStep);
  glWidget->draw();
}

}
