/*
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
#ifndef GLSCENEZOOMANDPAN_H_
#define GLSCENEZOOMANDPAN_H_

#include <tulip/tulipconf.h>
#include <tulip/Vector.h>
#include <tulip/Coord.h>

namespace tlp {

class GlScene;
struct BoundingBox;
class Camera;

/**
 * \brief A class which encapsulate a Tulip OpenGL scene animation
 *  This class aims to encapsulate a Tulip OpenGL scene animation.
 *  Derive it if you want to add extra animations to the Tulip OpenGL scene while a Zoom and Pan is performed
 */
class TLP_GL_SCOPE AdditionalGlSceneAnimation {

public :

  virtual ~AdditionalGlSceneAnimation() {}

  /**
   * Method to set the total number of animation steps. No need to call it because the GlSceneZoomAndPan class do that task.
   *
   */
  inline void setNbAnimationSteps(int nbAnimationSteps) {
    this->nbAnimationSteps = nbAnimationSteps;
  }

  /**
   * Pure virtual method called at each step of the Zoom and Pan animation.
   *
   * Implement it in your derived class to perform any extra animation you want on the Tulip OpenGL scene.
   */
  virtual void animationStep(int animationStep) = 0;

protected :

  int nbAnimationSteps;
};

/** \brief A convenient class to perform Zoom and Pan animation on Tulip OpenGL scene
 *
 *  This class allow to perform a smooth and efficient zooming and panning on Tulip OpenGL scene.
 *  The algorithm used to perform this task is the one published in: Jarke J. van Wijk and Wim A.A. Nuij, "Smooth and efficient zooming and panning"
 *  For more details, the paper can be downloaded at the following url: http://www.win.tue.nl/~vanwijk/zoompan.pdf
 *  Even if this class contains the whole Zoom and Pan implementation, it is not aimed to be used directly because its role is only to compute new camera parameters.
 *  Use the derived class QtGlSceneZoomAndPanAnimator in the tulip-gui library instead to perform the animation.
 */
class TLP_GL_SCOPE GlSceneZoomAndPan  {

public :

  virtual ~GlSceneZoomAndPan() {}

  /**
   * GlSceneZoomAndPan constructor
   *
   * \param glScene the Tulip OpenGL scene on which to perform zooming and panning
   * \param boundingBox the bounding box in scene coordinates on which the Tulip OpenGL scene has to be zoomed and panned.
   *            At the end of the animation, the viewport will be zoomed and centered on the content of that bounding box.
   * \param layerName The name of the layer animation should be done on
   * \param nbAnimationSteps the number of steps to perform during the animation
   * \param optimalPath if true zooming and panning will be combined at each step of the animation, if false the scene will be zoomed out/in, panned then zoomed in/out
   * \param p zoom/pan trade-off parameter, adjust it according to your needs
   */
  GlSceneZoomAndPan(GlScene *glScene, const BoundingBox &boundingBox, const std::string &layerName="Main", const int nbAnimationSteps = 50, const bool optimalPath = true, const double p = sqrt(1.6));

  /**
   * Method to add an additional scene animation while zooming and panning
   *
   * \param additionalAnimation The animation to add
   */
  void setAdditionalGlSceneAnimation(AdditionalGlSceneAnimation *additionalAnimation);

  /**
   * Method which return the number of animation steps
   */
  inline int getNbAnimationsStep() const {
    return nbAnimationSteps;
  }

  /**
   * Method to set the number of animation steps
   */
  inline void setNbAnimationSteps(const int nbAnimationSteps) {
    this->nbAnimationSteps = nbAnimationSteps;
  }

  /**
   * Method which performs the zoom and pan animation. Its role is to compute new camera parameters at step animationStep.
   * The scene is not redrawn with this method, you have to call the draw method on the associated GlScene object
   */
  void zoomAndPanAnimationStep(int animationStep);

protected :

  Camera &camera;
  Vector<int, 4> viewport;
  int nbAnimationSteps;
  bool optimalPath;
  double p;
  Coord camCenterStart, camCenterEnd;
  double w0, w1, u0, u1, b0, b1, r0, r1, S, sA, sB, wm;
  AdditionalGlSceneAnimation *additionalAnimation;
  float zoomAreaWidth, zoomAreaHeight;
  bool doZoomAndPan;

};

}

#endif /* GLSCENEZOOMANDPAN_H_ */
