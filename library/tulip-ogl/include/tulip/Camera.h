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
#ifndef TLPCAMERA_H
#define TLPCAMERA_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/Coord.h>
#include <tulip/Vector.h>
#include <tulip/Matrix.h>
#include <tulip/BoundingBox.h>

#include "tulip/CameraObserver.h"
#include "tulip/GlXMLTools.h"

namespace tlp {

  class GlScene;

  /**
   * \brief Tulip OpenGL camera object
   *
   * This camera can be a 2D or 3D camera
   * After setup you can do some basic operation :
   *  - Move, rotate, strafeLeftRight and strafeUpDown to modify poitn of view
   *  - You can directly modify camera infor mation with setSceneRadius, setZoomFactor, setEyes, setCenter and setUp
   *  - You can transform screen coordinates to 3D world coordinates with screenTo3DWorld function and vise versa with worldTo2DScreen function
   */
  class TLP_GL_SCOPE Camera : public ObservableCamera {
  public:
    
    /**
     * Constructor 
     */
    Camera(GlScene* scene,Coord center=Coord(0,0,0),
           Coord eyes=Coord(0,0,10), Coord up=Coord(0,-1,0),
           double zoomFactor=0.5, double sceneRadius=10);
    /**
     * Constructor : use for 2D camera 
     */
    Camera(GlScene* scene,bool d3);

    /**
     * Destructor
     */
    ~Camera();

    /**
     * Set the camera's scene : the viewport is store in the scene, so we must attach camera to a scene
     */
    void setScene(GlScene *scene);
    /**
     * Return the camera's scene
     */
    GlScene *getScene() {return scene;}

    /**
     * This moves the camera forward or backward depending on the speed
     */
    void move(float speed);
    /**
     * This strafes the camera left and right depending on the speed (-/+)
     */
    void strafeLeftRight(float speed);
    /**
     * This strafes the camera up and down depending on the speed (-/+)
     */
    void strafeUpDown(float speed);
    /**
     * This rotates the camera's eyes around the center depending on the values passed in.
     */
    void rotate(float angle, float x, float y, float z);

    /**
     * Return if the camera is a 3D one
     */
    bool is3D() {return d3;}

    /**
     * Return the viewport of the attached scene
     */
    Vector<int, 4> getViewport();
    
    /**
     * Init Gl parameters
     */
    void initGl();
    
    /**
     * Init light
     */
    void initLight();
    
    /**
     * Init projection with the gived viewport. Load identity matrix if reset is set as true
     */
    void initProjection(const Vector<int, 4>& viewport,bool reset=true);
    
    /**
     * Init projection with the scene viewport. Load identity matrix if reset is set as true
     */
    void initProjection(bool reset=true);
    
    /**
     * Init modelview
     */
    void initModelView();

    /**
     * Set the scene radius
     */
    void setSceneRadius(double sceneRadius,const BoundingBox sceneBoundingBox=BoundingBox());
    /**
     * Return the scene radius
     */
    double getSceneRadius() {return sceneRadius;}
    
    /**
     * Set the zoom factor
     */
    void setZoomFactor(double zoomFactor);
    /**
     * Return the zoom factor
     */
    double getZoomFactor() {return zoomFactor;}

    /**
     * Set the eye
     */
    void setEyes(const Coord& eyes);
    /**
     * Return the eyes
     */
    Coord getEyes() {return eyes;}
    
    /**
     * Set the center
     */
    void setCenter(const Coord& center);
    /**
     * Return the center
     */
    Coord getCenter() {return center;}

    /**
     * Set the up vector
     */
    void setUp(const Coord& up);
    /**
     * Return the up vector
     */
    Coord getUp() {return up;}

    /**
     * Set translate/scale transformation of object 
     * It use to compute lod of nodes/edges in metanodes
     */
    void addObjectTransformation(const Coord &translation,const Coord &scale, const Coord &baseCoord);

    /**
     * Get translate/scale transformation of object 
     * It use to compute lod of nodes/edges in metanodes
     */
    void getObjectTransformation(std::vector<Coord> &translation, std::vector<Coord> &scale, std::vector<Coord> &objectCoord);

    /**
     * Return true if object transformation is set
     */
    bool haveObjectTransformation();
    
    /**
     * Get the modelview matrix
     */
    void getModelviewMatrix(Matrix<float, 4> &modelviewMatrix) { modelviewMatrix=this->modelviewMatrix;}

    /**
     * Get the projection matrix
     */
    void getProjectionMatrix(Matrix<float, 4> &projectionMatrix) { projectionMatrix=this->projectionMatrix;}

    /**
     * Get the transform matrix : transformMatrix = projectionMatrix * modelviewMatrix
     */
    void getTransformMatrix(Matrix<float, 4> &transformMatrix) { transformMatrix=this->transformMatrix;} 

    /**
     * Get the projection and the modelview matrix generated with the given viewport
     */
    void getProjAndMVMatrix(const Vector<int, 4>& viewport,Matrix<float, 4> &projectionMatrix,Matrix<float, 4> &modelviewMatrix);
    
    /**
     * Get the transform matrix generated with the given viewport
     */
    void getTransformMatrix(const Vector<int, 4>& viewport,Matrix<float, 4> &transformMatrix);

    /**
     * Return the 3D world coordinate for the given screen point
     * \warning This function set up the projection and modelview matrix
     */
    Coord screenTo3DWorld(const Coord &point);
    
    /**
     * Return the screen position for the given 3D coordinate
     * \warning This function set up the projection and modelview matrix
     */
    Coord worldTo2DScreen(const Coord &obj);
    
    /**
     * Get the camera's data in XML form
     */
    void getXML(xmlNodePtr rootNode);

    /**
     * Set the camera's data with XML
     */
    void setWithXML(xmlNodePtr rootNode);

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

    std::vector<Coord> objectTranslation;
    std::vector<Coord> objectScale;
    std::vector<Coord> objectCoord;
    bool objectTransformation;

    bool d3;
    
  };

}

#endif
