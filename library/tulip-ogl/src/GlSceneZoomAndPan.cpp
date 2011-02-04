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

#include "tulip/GlSceneZoomAndPan.h"
#include <algorithm>

using namespace std;

namespace tlp {

GlSceneZoomAndPan::GlSceneZoomAndPan(GlScene *glScene, const BoundingBox &boundingBox, const std::string &layerName, const int nbAnimationSteps, const bool optimalPath, const double p) :
  camera(glScene->getLayer(layerName)->getCamera()),nbAnimationSteps(nbAnimationSteps), optimalPath(optimalPath), p(p), additionalAnimation(NULL) {

	viewport = glScene->getViewport();

	camCenterStart = camera.getCenter();
	camCenterEnd = Coord(boundingBox.center());

	Coord blScene(camera.screenTo3DWorld(Coord(0, 0, 0)));
	Coord trScene(camera.screenTo3DWorld(Coord(viewport[2], viewport[3], 0)));

	w0 = min(abs(trScene.getX() - blScene.getX()), abs(trScene.getY() - blScene.getY()));
	w1 = max(boundingBox[1][0] - boundingBox[0][0], boundingBox[1][1] - boundingBox[0][1]);

	u0 = 0;
	u1 = camCenterStart.dist(camCenterEnd);

	if (u1 < 1e-5) u1 = 0;

	if (optimalPath) {
		if (u0 != u1) {
			b0 = (w1 * w1 - w0 * w0 + p * p * p * p * u1 * u1) / (2 * w0 * p * p * u1);
			b1 = (w1 * w1 - w0 * w0 - p * p * p * p * u1 * u1) / (2 * w1 * p * p * u1);
			r0 = log(-b0 + sqrt(b0 * b0 + 1));
			r1 = log(-b1 + sqrt(b1 * b1 + 1));

			S = (r1 - r0) / p;
		} else {
			S = abs(log(w1 / w0)) / p;
		}
	} else {
		wm = max(w0, max(w1, p * p * (u1 - u0) / 2));
		sA = log(wm / w0) / p;
		sB = sA + p * (u1 - u0) / wm;
		S = sB + log(wm / w1) / p;
	}

	if (abs(w0 - w1) > 1e-3 || u1 > 0) {
		doZoomAndPan = true;
	} else {
		doZoomAndPan = false;
	}
}

void GlSceneZoomAndPan::setAdditionalGlSceneAnimation(AdditionalGlSceneAnimation *additionalAnimation) {
	this->additionalAnimation = additionalAnimation;
	if (additionalAnimation != NULL) {
		additionalAnimation->setNbAnimationSteps(nbAnimationSteps);
	}
}

void GlSceneZoomAndPan::zoomAndPanAnimationStep(int animationStep) {
	if (doZoomAndPan) {
		double t = (double) animationStep / (double) nbAnimationSteps;
		double s = t * S;
		double u = 0, w = 0, f = 0;
		if (optimalPath) {
			if (u0 != u1) {
				u = w0 / (p * p) * cosh(r0) * tanh(p * s + r0) - w0 / (p * p) * sinh(r0) + u0;
				w = w0 * cosh(r0) / cosh(p * s + r0);
				f = u / u1;
			} else {
				double k = (w1 < w0) ? -1 : 1;
				w = w0 * exp(k * p * s);
				f = 0;
			}
		} else {
			if (s >= 0 && s < sA) {
				u = u0;
				w = w0 * exp(p * s);
			} else if (s >= sA && s < sB) {
				u = wm * (s - sA) / p + u0;
				w = wm;
			} else {
				u = u1;
				w = wm * exp(p * (sB - s));
			}
			if (u0 != u1) {
				f = u / u1;
			} else {
				f = 0;
			}
		}
		camera.setCenter(camCenterStart + (camCenterEnd - camCenterStart) *f);
		camera.setEyes(Coord(0, 0, camera.getSceneRadius()));
		camera.setEyes(camera.getEyes() + camera.getCenter());
		camera.setUp(Coord(0, 1., 0));

		Coord bbScreenFirst = camera.worldTo2DScreen(camera.getCenter() - Coord(w/2, w/2, 0));
		Coord bbScreenSecond = camera.worldTo2DScreen(camera.getCenter() + Coord(w/2, w/2, 0));
		float bbWidthScreen = bbScreenSecond.getX() - bbScreenFirst.getX();
		double newZoomFactor = min(viewport[2], viewport[3]) / bbWidthScreen;
		camera.setZoomFactor(camera.getZoomFactor() * newZoomFactor);
	}
	if (additionalAnimation != NULL) {
		additionalAnimation->animationStep(animationStep);
	}
}

}
