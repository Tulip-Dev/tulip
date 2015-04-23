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

#ifndef TLPCAMERA_H
#define TLPCAMERA_H

#include <tulip/Coord.h>
#include <tulip/Matrix.h>
#include <tulip/BoundingBox.h>
#include <tulip/Observable.h>

namespace tlp {

class GlScene;

/**
 * \ingroup OpenGL
 * \brief Tulip OpenGL camera object
 *
 * This camera can be a 2D or 3D camera
 * After setup you can do some basic operation :
 *  - Move, rotate, strafeLeftRight and strafeUpDown to modify poitn of view
 *  - You can directly modify camera parameters with setSceneRadius, setZoomFactor, setEyes, setCenter and setUp
 *  - You can transform screen coordinates to 3D world coordinates with screenTo3DWorld() function and 3D world coordinates to screen coordinates with worldTo2DScreen() function
 * A camera is a main component of GlLayer and GlScene
 * @see GlLayer
 * @see GlScene
 */
class TLP_GL_SCOPE Camera : public Observable {
public:

  /**
   * @brief Constructor
   * @param scene A layer is attached to a scene so we have to specify it in the constructor
   * @param center 3D coordinates of point visualized by the camera
   * @param eye 3D position of the camera
   * @param up normalized up 3D coordinates of the camera
   * @param zoomFactor level of zoom of the camera
   * @param sceneRadius scene radius of the camera
   */
  Camera(GlScene* scene,Coord center=Coord(0,0,0),
         Coord eyes=Coord(0,0,10), Coord up=Coord(0,-1,0),
         double zoomFactor=0.5, double sceneRadius=10);
  /**
   * @brief Constructor : used to create a 2D camera
   */
  Camera(GlScene* scene,bool d3);

  Camera& operator=(const Camera& camera);

  /**
   * @brief Destructor
   */
  ~Camera();

  /**
   * @brief Set the camera's scene
   * The viewport is store in the scene, so we must attach camera to a scene
   */
  void setScene(GlScene *scene);

  /**
   * @brief Return the camera's scene
   */
  GlScene *getScene() const {
    return scene;
  }

  /**
   * @brief Load this camera parameters (eye, center, zoom factor) with an other camera parameters
   */
  void loadCameraParametersWith(const Camera &camera) {
    *this=camera;
  }

  /**
   * @brief Return the camera bounding box
   *
   * This bounding box is the part of the scene visualized by this camera.
   */
  BoundingBox getBoundingBox() const;

  /**
   * @brief This function moves the camera forward or backward depending on the speed
   */
  void move(float speed);
  /**
   * @brief This function strafes the camera left and right depending on the speed (-/+)
   */
  void strafeLeftRight(float speed);
  /**
   * @brief This function strafes the camera up and down depending on the speed (-/+)
   */
  void strafeUpDown(float speed);
  /**
   * @brief This function rotates the camera's eyes around the center depending on the values passed in.
   */
  void rotate(float angle, float x, float y, float z);

  /**
   * @brief Return if the camera is a 3D one
   */
  bool is3D() const {
    return d3;
  }

  /**
   * @brief Return the viewport of the attached scene
   */
  Vector<int, 4> getViewport() const;

  /**
   * @brief Return the scene radius
   */
  double getSceneRadius() const {
    return sceneRadius;
  }

  /**
   * @brief Set the zoom factor
   *
   * level of zoom of the camera
   */
  void setZoomFactor(double zoomFactor);
  /**
   * @brief Return the zoom factor
   *
   * level of zoom of the camera
   */
  double getZoomFactor() const {
    return zoomFactor;
  }

  /**
   * @brief Set the eye
   *
   * 3D position of the camera
   */
  void setEyes(const Coord& eyes);
  /**
   * @brief Return the eyes
   *
   * 3D position of the camera
   */
  Coord getEyes() const {
    return eyes;
  }

  /**
   * @brief Set the center
   *
   * 3D coordinates of point visualized by the camera
   */
  void setCenter(const Coord& center);
  /**
   * @brief Return the center
   *
   * 3D coordinates of point visualized by the camera
   */
  Coord getCenter() const {
    return center;
  }

  /**
   * @brief Set the up vector
   *
   * normalized up 3D coordinates of the camera
   */
  void setUp(const Coord& up);
  /**
   * @brief Return the up vector
   *
   * normalized up 3D coordinates of the camera
   */
  Coord getUp() const {
    return up;
  }

  /**
   * @brief Return the 3D world coordinate for the given screen point
   * \warning This function set up the projection and modelview matrix
   */
  Coord screenTo3DWorld(const Coord &point) const;

  /**
   * @brief Return the screen position for the given 3D coordinate
   * \warning This function set up the projection and modelview matrix
   */
  Coord worldTo2DScreen(const Coord &obj) const;

  /**
   * @brief Function to export data in outString (in XML format)
   */
  virtual void getXML(std::string &outString);

  /**
   * @brief Function to set data with inString (in XML format)
   */
  virtual void setWithXML(const std::string &inString, unsigned int &currentPosition);

///@cond DOXYGEN_HIDDEN

  /**
   * Get the modelview matrix
   */
  void getModelviewMatrix(Matrix<float, 4> &modelviewMatrix) const {
    modelviewMatrix=this->modelviewMatrix;
  }

  /**
   * Get the projection matrix
   */
  void getProjectionMatrix(Matrix<float, 4> &projectionMatrix) const {
    projectionMatrix=this->projectionMatrix;
  }

  /**
   * Get the transform matrix : transformMatrix = projectionMatrix * modelviewMatrix
   */
  void getTransformMatrix(Matrix<float, 4> &transformMatrix) const {
    transformMatrix=this->transformMatrix;
  }

  /**
   * Get the projection and the modelview matrix generated with the given viewport
   */
  void getProjAndMVMatrix(const Vector<int, 4>& viewport,Matrix<float, 4> &projectionMatrix,Matrix<float, 4> &modelviewMatrix) const;

  /**
   * Get the transform matrix generated with the given viewport
   */
  void getTransformMatrix(const Vector<int, 4>& viewport,Matrix<float, 4> &transformMatrix) const;

  /**
   * @brief Init Gl parameters
   */
  void initGl();

  /**
   * @brief Init light
   */
  void initLight();

  /**
   * @brief Init projection with the gived viewport. Load identity matrix if reset is set as true
   */
  void initProjection(const Vector<int, 4>& viewport,bool reset=true);

  /**
   * @brief Init projection with the scene viewport. Load identity matrix if reset is set as true
   */
  void initProjection(bool reset=true);

  /**
   * @brief Init modelview
   */
  void initModelView();

  /**
   * @brief Set the scene radius
   */
  void setSceneRadius(double sceneRadius,const BoundingBox sceneBoundingBox=BoundingBox());

///@endcond

private:

  bool matrixCoherent;

  Coord center,eyes,up;
  double zoomFactor;
  double sceneRadius;
  BoundingBox sceneBoundingBox;

  GlScene* scene;

  Matrix<float, 4> modelviewMatrix;
  Matrix<float, 4> projectionMatrix;
  Matrix<float, 4> transformMatrix;

  bool d3;

};

}

#endif
