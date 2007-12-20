#ifndef TLPCAMERA_H
#define TLPCAMERA_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/Coord.h>
#include <tulip/Vector.h>
#include <tulip/Matrix.h>

namespace tlp {

  class GlScene;

  class TLP_GL_SCOPE Camera {
  public:
    
    /**
     * Constructor 
     */
    Camera(GlScene* scene,Coord center=Coord(0,0,0),Coord eyes=Coord(0,0,10), Coord up=Coord(0,-1,0), double zoomFactor=0.5, double sceneRadius=10);
    
    /**
     * Constructor : use for 2D camera 
     */
    Camera(GlScene* scene,bool d3);

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
    void setSceneRadius(double sceneRadius) {this->sceneRadius=sceneRadius;matrixCoherent=false;}
    /**
     * Return the scene radius
     */
    double getSceneRadius() {return sceneRadius;}
    
    /**
     * Set the zoom factor
     */
    void setZoomFactor(double zoomFactor) {this->zoomFactor=zoomFactor;matrixCoherent=false;}
    /**
     * Return the zoom factor
     */
    double getZoomFactor() {return zoomFactor;}

    /**
     * Set the eye
     */
    void setEyes(const Coord& eyes) {this->eyes=eyes;matrixCoherent=false;}
    /**
     * Return the eyes
     */
    Coord getEyes() {return eyes;}
    
    /**
     * Set the center
     */
    void setCenter(const Coord& center) {this->center=center;matrixCoherent=false;}
    /**
     * Return the center
     */
    Coord getCenter() {return center;}

    /**
     * Set the up vector
     */
    void setUp(const Coord& up) {this->up=up;matrixCoherent=false;}
    /**
     * Return the up vector
     */
    Coord getUp() {return up;}

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
     * Get the projection and the modelview matrix generated with the gived viewport
     */
    void getProjAndMVMatrix(const Vector<int, 4>& viewport,Matrix<float, 4> &projectionMatrix,Matrix<float, 4> &modelviewMatrix);

    /**
     * Return the 3D world coordinate for the gived screen point
     */
    Coord screenTo3DWorld(const Coord &point);
    
    /**
     * Return the screen position for the gived 3D coordinate
     */
    Coord worldTo2DScreen(const Coord &obj);

  private:

    bool matrixCoherent;
    
    Coord center,eyes,up;
    double zoomFactor;
    double sceneRadius;
    
    GlScene* scene;

    Matrix<float, 4> modelviewMatrix;
    Matrix<float, 4> projectionMatrix;
    Matrix<float, 4> transformMatrix;

    bool d3;
    
  };

}

#endif
