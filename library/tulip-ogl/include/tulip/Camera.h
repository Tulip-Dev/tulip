#ifndef TLPCAMERA_H
#define TLPCAMERA_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/Coord.h>

namespace tlp {

class TLP_GL_SCOPE Camera {
 public:
  Coord center,eyes,up;
  double zoomFactor;
  double sceneRadius;
  Camera(Coord center=Coord(0,0,0),Coord eyes=Coord(0,0,10), Coord up=Coord(0,-1,0), double zoomFactor=0.5, double sceneRadius=10);
  /// This moves the camera forward or backward depending on the speed
  void move(float speed);
  ///  This strafes the camera left and right depending on the speed (-/+)
  void strafeLeftRight(float speed);
  ///  This strafes the camera up and down depending on the speed (-/+)
  void strafeUpDown(float speed);
  /// This rotates the camera's eyes around the center depending on the values passed in.
  void rotate(float angle, float x, float y, float z);
  
};

}

#endif
