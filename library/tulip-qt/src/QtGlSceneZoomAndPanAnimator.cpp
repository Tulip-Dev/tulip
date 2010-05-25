/*
 * QtGlSceneZoomAndPanAnimator.cpp
 *
 *  Created on: 20 oct. 2009
 *      Author: antoine
 */

#include <QtGui/QApplication>
#include <QtCore/QTimeLine>

#include "tulip/QtGlSceneZoomAndPanAnimator.h"

namespace tlp {

QtGlSceneZoomAndPanAnimator::QtGlSceneZoomAndPanAnimator(GlMainWidget *glWidget, const BoundingBox &boundingBox, const bool optimalPath, const double velocity, const double p) :
	GlSceneZoomAndPan(glWidget->getScene(), boundingBox, 0, optimalPath, p), glWidget(glWidget), animationDurationMsec(1000) {
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
