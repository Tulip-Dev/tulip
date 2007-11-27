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
    
    Camera(GlScene* scene,Coord center=Coord(0,0,0),Coord eyes=Coord(0,0,10), Coord up=Coord(0,-1,0), double zoomFactor=0.5, double sceneRadius=10);
    Camera(GlScene* scene,bool d3,bool reversed=false);

    void setScene(GlScene *scene);

    /// This moves the camera forward or backward depending on the speed
    void move(float speed);
    ///  This strafes the camera left and right depending on the speed (-/+)
    void strafeLeftRight(float speed);
    ///  This strafes the camera up and down depending on the speed (-/+)
    void strafeUpDown(float speed);
    /// This rotates the camera's eyes around the center depending on the values passed in.
    void rotate(float angle, float x, float y, float z);

    bool is3D() {return d3;}

    Vector<int, 4> getViewport();
    
    void initGl();
    //void initGlParametters();
    void initLight();
    void initProjection(bool reset=true);
    void initModelView();

    double getSceneRadius() {return sceneRadius;}
    double getZoomFactor() {return zoomFactor;}
    Coord getEyes() {return eyes;}
    Coord getCenter() {return center;}
    Coord getUp() {return up;}

    void setSceneRadius(double sceneRadius) {this->sceneRadius=sceneRadius;matrixCoherent=false;}
    void setZoomFactor(double zoomFactor) {this->zoomFactor=zoomFactor;matrixCoherent=false;}
    void setEyes(const Coord& eyes) {this->eyes=eyes;matrixCoherent=false;}
    void setCenter(const Coord& center) {this->center=center;matrixCoherent=false;}
    void setUp(const Coord& up) {this->up=up;matrixCoherent=false;}

    Coord screenTo3DWorld(const Coord &point);
    Coord worldTo2DScreen(const Coord &obj);
    
    GlScene* scene;

  private:

    bool matrixCoherent;
    
    Coord center,eyes,up;
    double zoomFactor;
    double sceneRadius;
    
    Matrix<float, 4> modelviewMatrix;
    Matrix<float, 4> projectionMatrix;
    Matrix<float, 4> transformMatrix;

    bool d3;
    bool reversed;
    
  };

}

#endif
