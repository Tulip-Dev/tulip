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

/**
 *
 * tulip-ogles library : rewrite of old tulip-ogl library using OpenGL ES API
 * Copyright (c) 2016 Antoine Lambert, Thales Services
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#ifndef ZOOMANDPANANIMATION_H
#define ZOOMANDPANANIMATION_H

#include <tulip/Vector.h>
#include <tulip/Coord.h>
#include <tulip/BoundingBox.h>

#include <tulip/Camera.h>

namespace tlp {

/**
 * \brief A class which encapsulate a Tulip OpenGL scene animation
 *  This class aims to encapsulate a Tulip OpenGL scene animation.
 *  Derive it if you want to add extra animations to the Tulip OpenGL scene while a Zoom and Pan is performed
 */
class TLP_GLES_SCOPE AdditionalGlSceneAnimation {

public :

  virtual ~AdditionalGlSceneAnimation() {}

  /**
   * Pure virtual method called at each step of the Zoom and Pan animation.
   *
   * Implement it in your derived class to perform any extra animation you want on the Tulip OpenGL scene.
   */
  virtual void animationStep(double t) = 0;

};

/** \brief A convenient class to perform Zoom and Pan animation on Tulip OpenGL scene
 *
 *  This class allow to perform a smooth and efficient zooming and panning on Tulip OpenGL scene.
 *  The algorithm used to perform this task is the one published in: Jarke J. van Wijk and Wim A.A. Nuij, "Smooth and efficient zooming and panning"
 *  For more details, the paper can be downloaded at the following url: http://www.win.tue.nl/~vanwijk/zoompan.pdf
 *  Even if this class contains the whole Zoom and Pan implementation, it is not aimed to be used directly because its role is only to compute new camera parameters.
 *  Use the derived class QtGlSceneZoomAndPanAnimator in the tulip-gui library instead to perform the animation.
 */
class TLP_GLES_SCOPE ZoomAndPanAnimation  {

public :

  ZoomAndPanAnimation(Camera *camera, const tlp::BoundingBox &boundingBox, const unsigned int animationDuration = 1000, const bool optimalPath = true, const float velocity = 1.1f, const double p = sqrt(1.6));

  virtual ~ZoomAndPanAnimation() {}

  float getAnimationDuration() const {
    return _animationDuration;
  }

  void zoomAndPanAnimationStep(double t);

  bool canDoZoomAndPan() const {
    return _doZoomAndPan;
  }

  /**
   * Method to add an additional scene animation while zooming and panning
   *
   * \param additionalAnimation The animation to add
   */
  void setAdditionalGlSceneAnimation(AdditionalGlSceneAnimation *additionalAnimation) {
    _additionalAnimation = additionalAnimation;
  }

protected :

  Camera *_camera;
  tlp::Vec4i _viewport;
  unsigned int _animationDuration;
  bool _optimalPath;
  double _p;
  tlp::Coord _camCenterStart, _camCenterEnd;
  double _w0, _w1, _u0, _u1, _b0, _b1, _r0, _r1, _S, _sA, _sB, _wm;
  float _zoomAreaWidth, _zoomAreaHeight;
  bool _doZoomAndPan;
  AdditionalGlSceneAnimation *_additionalAnimation;

};

void TLP_GLES_SCOPE adjustViewToBoundingBox(Camera *camera, const tlp::BoundingBox &boundingBox);

}

#endif /* ZOOMANDPANANIMATION_H */
