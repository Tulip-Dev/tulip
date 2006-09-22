#include <cmath>
#include "tulip/Camera.h"

using namespace tlp;

Camera::Camera(Coord center,Coord eyes, Coord up, double zoomFactor, double sceneRadius):
  center(center),
  eyes(eyes),
  up(up),
  zoomFactor(zoomFactor),
  sceneRadius(sceneRadius) {
}

void Camera::move(float speed) {
  Coord move = eyes - center;
  move *= speed/move.norm();
  eyes += move;
  center += move;
}

/// This rotates the camera's eyes around the center depending on the values passed in.
void Camera::rotate(float angle, float x, float y, float z) {
  Coord vNewEyes;
  Coord vNewUp;

  // Get the eyes vector (The direction we are facing)
  Coord vEyes = eyes - center;  

  // Calculate the sine and cosine of the angle once
  float cosTheta = (float)cos(angle);
  float sinTheta = (float)sin(angle);

  // Find the new x position for the new rotated point
  vNewEyes[0]  = (cosTheta + (1 - cosTheta) * x * x)       * vEyes[0];
  vNewEyes[0] += ((1 - cosTheta) * x * y - z * sinTheta)   * vEyes[1];
  vNewEyes[0] += ((1 - cosTheta) * x * z + y * sinTheta)   * vEyes[2];

  // Find the new y position for the new rotated point
  vNewEyes[1]  = ((1 - cosTheta) * x * y + z * sinTheta)   * vEyes[0];
  vNewEyes[1] += (cosTheta + (1 - cosTheta) * y * y)       * vEyes[1];
  vNewEyes[1] += ((1 - cosTheta) * y * z - x * sinTheta)   * vEyes[2];

  // Find the new z position for the new rotated point
  vNewEyes[2]  = ((1 - cosTheta) * x * z - y * sinTheta)   * vEyes[0];
  vNewEyes[2] += ((1 - cosTheta) * y * z + x * sinTheta)   * vEyes[1];
  vNewEyes[2] += (cosTheta + (1 - cosTheta) * z * z)       * vEyes[2];

  // Find the new x position for the new rotated point
  vNewUp[0]  = (cosTheta + (1 - cosTheta) * x * x)       * up[0];
  vNewUp[0] += ((1 - cosTheta) * x * y - z * sinTheta)   * up[1];
  vNewUp[0] += ((1 - cosTheta) * x * z + y * sinTheta)   * up[2];

  // Find the new y position for the new rotated point
  vNewUp[1]  = ((1 - cosTheta) * x * y + z * sinTheta)   * up[0];
  vNewUp[1] += (cosTheta + (1 - cosTheta) * y * y)       * up[1];
  vNewUp[1] += ((1 - cosTheta) * y * z - x * sinTheta)   * up[2];

  // Find the new z position for the new rotated point
  vNewUp[2]  = ((1 - cosTheta) * x * z - y * sinTheta)   * up[0];
  vNewUp[2] += ((1 - cosTheta) * y * z + x * sinTheta)   * up[1];
  vNewUp[2] += (cosTheta + (1 - cosTheta) * z * z)       * up[2];


  // Now we just add the newly rotated vector to our position to set
  // our new rotated eyes of our camera.
  eyes = center + vNewEyes;
  up   = vNewUp;
}

///  This strafes the camera left and right depending on the speed (-/+)
void Camera::strafeLeftRight(float speed) {   
  Coord strafeVector=((eyes-center)^up);
  strafeVector *= speed / strafeVector.norm();
  center += strafeVector;
  eyes   += strafeVector;
}

///  This strafes the camera left and right depending on the speed (-/+)
void Camera::strafeUpDown(float speed) {   
  Coord strafeVector(up);
  strafeVector *= speed / strafeVector.norm();
  center += strafeVector;
  eyes   += strafeVector;
}
