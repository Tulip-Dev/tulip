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

#include <GL/glew.h>

#include <tulip/Camera.h>
#include <tulip/GlUtils.h>

#include <cmath>

using namespace std;
using namespace tlp;

static void getCameraParametersFromModelviewMatrix(const MatrixGL &mdvMat, Vec3f &eye, Vec3f &viewdir, Vec3f &up) {
  Vec3f xdir(mdvMat[0][0], mdvMat[1][0], mdvMat[2][0]);
  Vec3f ydir(mdvMat[0][1], mdvMat[1][1], mdvMat[2][1]);
  Vec3f zdir(mdvMat[0][2], mdvMat[1][2], mdvMat[2][2]);
  Vec3f bvec(mdvMat[3][0], mdvMat[3][1], mdvMat[3][2]);
  tlp::Mat3f basisMat;
  basisMat[0] = xdir;
  basisMat[1] = ydir;
  basisMat[2] = zdir;
  basisMat.inverse();
  eye = basisMat * -bvec;
  viewdir = -zdir;
  up =  ydir;
}

//====================================================
static void identityMatrix(MatrixGL &m) {
  m.fill(0.0f);
  for (int i = 0 ; i < 4 ; ++i) {
    m[i][i] = 1.0f;
  }
}
//====================================================
void Camera::ortho(float  left,  float  right,  float  bottom,  float  top,  float  nearVal,  float  farVal) {
  float tx = -(right+left)/(right-left);
  float ty = -(top+bottom)/(top-bottom);
  float tz = -(farVal+nearVal)/(farVal-nearVal);
  MatrixGL tmp;
  tmp.fill(0.0f);
  tmp[0][0] = 2.0f / (right-left);
  tmp[3][0] = tx;
  tmp[1][1] = 2.0f / (top-bottom);
  tmp[3][1] = ty;
  tmp[2][2] = -2.0f / (farVal-nearVal);
  tmp[3][2] = tz;
  tmp[3][3] = 1.0f;
  _projectionMatrix = tmp * _projectionMatrix;
}
//====================================================
void Camera::frustum(float  left,  float  right,  float  bottom,  float  top,  float  nearVal,  float  farVal) {
  float A = (right+left)/(right-left);
  float B = (top+bottom)/(top-bottom);
  float C = -(farVal+nearVal)/(farVal-nearVal);
  float D = -(2*farVal*nearVal)/(farVal-nearVal);
  MatrixGL tmp;
  tmp.fill(0.0f);
  tmp[0][0] = (2.0f*nearVal) / (right-left);
  tmp[2][0] = A;
  tmp[1][1] = (2.0f*nearVal) / (top-bottom);
  tmp[2][1] = B;
  tmp[2][2] = C;
  tmp[3][2] = D;
  tmp[2][3] = -1.0f;
  _projectionMatrix = tmp * _projectionMatrix;
}
//====================================================
void Camera::lookAt(float eyeX,  float eyeY,  float eyeZ,  float centerX,  float centerY,  float centerZ,  float upX,  float upY,  float upZ) {
  Vec3f f(centerX-eyeX, centerY-eyeY, centerZ-eyeZ);
  Vec3f upN(upX, upY, upZ);
  f /= f.norm();
  Vec3f s = f ^ upN;
  s /= s.norm();
  Vec3f u = s ^ f;
  MatrixGL M;
  identityMatrix(M);
  for (int i = 0 ; i < 3 ; ++i) {
    M[i][0] = s[i];
    M[i][1] = u[i];
    M[i][2] = -f[i];
  }
  _modelviewMatrix = M * _modelviewMatrix;

  translate(-eyeX, -eyeY, -eyeZ);
}
//====================================================
Camera::Camera(Coord center,Coord eyes, Coord up, double zoomFactor, double sceneRadius):
  _mdvMatCoherent(false),
  _projectionMatCoherent(false),
  _center(center),
  _eyes(eyes),
  _up(up),
  _zoomFactor(zoomFactor),
  _sceneRadius(sceneRadius),
  _3d(true),
  _viewOrtho(true) {
  identityMatrix(_modelviewMatrix);
  identityMatrix(_projectionMatrix);
  identityMatrix(_transformMatrix);
}
//====================================================
Camera::Camera(const Camera &camera) : Observable(camera) {
  *this = camera;
}
//====================================================
Camera::Camera(bool is3d) : Camera() {
  _3d = is3d;
}
//====================================================
Camera::~Camera() {}
//===================================================
Camera& Camera::operator=(const Camera &camera) {
  _mdvMatCoherent = camera._mdvMatCoherent;
  _projectionMatCoherent = camera._projectionMatCoherent;

  _center = camera._center;
  _eyes = camera._eyes;
  _up = camera._up;
  _zoomFactor = camera._zoomFactor;
  _sceneRadius = camera._sceneRadius;
  _sceneBoundingBox = camera._sceneBoundingBox;
  _viewport = camera._viewport;

  _modelviewMatrix = camera._modelviewMatrix;
  _projectionMatrix = camera._projectionMatrix;
  _transformMatrix = camera._transformMatrix;
  _transformMatrixBillboard = camera._transformMatrixBillboard;
  _normalMatrix = camera._normalMatrix;

  _3d = camera._3d;
  _viewOrtho = camera._viewOrtho;

  _projectionMatrixStack = camera._projectionMatrixStack;
  _modelViewMatrixStack = camera._modelViewMatrixStack;
  _centerStack = camera._centerStack;
  _eyesStack = camera._eyesStack;
  _upStack = camera._upStack;
  return *this;
}

//===================================================
BoundingBox Camera::getBoundingBox() {
  BoundingBox bb;
  bb.expand(viewportTo3DWorld(Coord(_viewport[0],_viewport[1],0)));
  bb.expand(viewportTo3DWorld(Coord(_viewport[0]+_viewport[2],_viewport[1]+_viewport[3],0)));
  return bb;
}
//====================================================
void Camera::initGl() {
  initProjection();
  initModelView();
  _transformMatrix = _modelviewMatrix * _projectionMatrix;
  MatrixGL modelviewMatrixBillboard = _modelviewMatrix;
  modelviewMatrixBillboard[0] = Vec4f(1,0,0,0);
  modelviewMatrixBillboard[1] = Vec4f(0,1,0,0);
  modelviewMatrixBillboard[2] = Vec4f(0,0,1,0);
  _transformMatrixBillboard = modelviewMatrixBillboard * _projectionMatrix;
}
//====================================================
void Camera::initProjection(const Vec4i& viewport) {

  if (_projectionMatCoherent) return;

  identityMatrix(_projectionMatrix);

  double _near;
  double _far;

  bool valid = _sceneBoundingBox.isValid();
  Vec3f v1 = _sceneBoundingBox[0];
  Vec3f v2 = _sceneBoundingBox[1];

  if(valid && v1 != v2) {
    Coord diagCoord(_sceneBoundingBox[1]-_sceneBoundingBox[0]);
    double diag=2*sqrt(diagCoord[0]*diagCoord[0]+diagCoord[1]*diagCoord[1]+diagCoord[2]*diagCoord[2]);
    _near=-diag;
    _far=diag;
  }
  else {
    _near=-_sceneRadius;
    _far=_sceneRadius;
  }

  if(_3d) {
    float ratio = double(viewport[2])/double(viewport[3]);

    if(_viewOrtho) {
      if (ratio>1)
        ortho(-ratio*_sceneRadius/2.0/_zoomFactor, ratio*_sceneRadius/2.0/_zoomFactor,
              -_sceneRadius/2.0/_zoomFactor, _sceneRadius/2.0/_zoomFactor,
              _near,_far);
      else
        ortho(-_sceneRadius/2.0/_zoomFactor, _sceneRadius/2.0/_zoomFactor,
              1./ratio * - _sceneRadius/2.0/_zoomFactor, 1./ratio * _sceneRadius/2.0/_zoomFactor,
              _near,_far);
    }
    else {
      if (ratio>1)
        frustum(-ratio/2.0/_zoomFactor, ratio/2.0/_zoomFactor,
                -0.5/_zoomFactor, 0.5/_zoomFactor,
                1.0 , _sceneRadius*2.0);
      else
        frustum(-0.5/_zoomFactor, 0.5/_zoomFactor,
                -1.0/(ratio/0.5*_zoomFactor), 1.0/(ratio/0.5*_zoomFactor),
                1.0 , _sceneRadius*2.0);
    }

  }
  else {
    ortho(0,viewport[2],0,viewport[3], -100, 100);
  }

  _projectionMatCoherent = true;

}
//====================================================
void Camera::initProjection() {
  initProjection(_viewport);
}
//====================================================
void Camera::initModelView() {

  if (_mdvMatCoherent) return;

  identityMatrix(_modelviewMatrix);

  if(_3d) {
    lookAt(_eyes[0], _eyes[1], _eyes[2],
        _center[0], _center[1], _center[2],
        _up[0], _up[1], _up[2]);
  }

  _normalMatrix = _modelviewMatrix;
  _normalMatrix.inverse();
  _normalMatrix.transpose();

  _mdvMatCoherent = true;
}
//====================================================
void Camera::setViewport(const tlp::Vec4i &viewport) {
  if (_viewport != viewport) {
    _projectionMatCoherent = false;
    _viewport = viewport;
    notifyModified();
  }
}
//====================================================
void Camera::setSceneRadius(double sceneRadius,const BoundingBox sceneBoundingBox) {
  if (_sceneRadius != sceneRadius || _sceneBoundingBox[0] != sceneBoundingBox[0] || _sceneBoundingBox[1] != sceneBoundingBox[1]) {
    _sceneRadius=sceneRadius;
    _sceneBoundingBox=sceneBoundingBox;
    _projectionMatCoherent=false;
    notifyModified();
  }
}
//====================================================
void Camera::setZoomFactor(double zoomFactor) {
  if(zoomFactor>1E10)
    return;
  if (_zoomFactor != zoomFactor) {
    _zoomFactor=zoomFactor;
    _projectionMatCoherent=false;
    notifyModified();
  }
}
//====================================================
void Camera::setEyes(const Coord& eyes) {
  if (_eyes != eyes) {
    _eyes=eyes;
    _mdvMatCoherent=false;
    notifyModified();
  }
}
//====================================================
void Camera::setCenter(const Coord& center) {
  if (_center != center) {
    _center=center;
    _mdvMatCoherent=false;
    notifyModified();
  }
}
//====================================================
void Camera::setUp(const Coord& up) {
  if (_up != up) {
    _up=up;
    _mdvMatCoherent=false;
    notifyModified();
  }
}
//====================================================
Coord Camera::viewportTo3DWorld(const Coord &point) {

  initGl();

  //try to find a good z-coordinate for reverse projection
  Coord pScr = projectPoint(Coord(0,0,0), _transformMatrix, _viewport);

  pScr[0] = _viewport[0] + _viewport[2] - point[0];
  pScr[1] = _viewport[1] + _viewport[3] - point[1];

  MatrixGL tmp(_transformMatrix);
  tmp.inverse();
  return unprojectPoint(pScr, tmp, _viewport);
}
//====================================================
Coord Camera::worldTo2DViewport(const Coord &obj) {
  initGl();
  return projectPoint(obj, _transformMatrix, _viewport) - Coord(_viewport[0], _viewport[1]);
}
//====================================================
Vec4i Camera::getViewport() const {
  return _viewport;
}
//====================================================
void Camera::pushProjectionMatrix() {
  _projectionMatrixStack.push(_projectionMatrix);
}
//====================================================
void Camera::popProjectionMatrix() {
  if (!_projectionMatrixStack.empty()) {
    _projectionMatrix = _projectionMatrixStack.top();
    _projectionMatrixStack.pop();
  } else {
    std::cerr << "Error : no projection matrix to pop" << std::endl;
  }
}
//====================================================
void Camera::pushModelViewMatrix() {
  _modelViewMatrixStack.push(_modelviewMatrix);
  _centerStack.push(_center);
  _eyesStack.push(_eyes);
  _upStack.push(_up);
}
//====================================================
void Camera::popModelViewMatrix() {
  if (!_modelViewMatrixStack.empty()) {
    _modelviewMatrix = _modelViewMatrixStack.top();
    _modelViewMatrixStack.pop();
    _center = _centerStack.top();
    _centerStack.pop();
    _eyes = _eyesStack.top();
    _eyesStack.pop();
    _up = _upStack.top();
    _upStack.pop();
  } else {
    std::cerr << "Error : no model view matrix to pop" << std::endl;
  }
}
//====================================================
void Camera::translate(const tlp::Vec3f &move) {
  if (move != Vec3f(0,0,0)) {
    MatrixGL translation;
    identityMatrix(translation);
    translation[3][0] = move[0];
    translation[3][1] = move[1];
    translation[3][2] = move[2];
    _modelviewMatrix = translation * _modelviewMatrix;
    float dist = _center.dist(_eyes);
    Vec3f viewdir;
    getCameraParametersFromModelviewMatrix(_modelviewMatrix, _eyes, viewdir, _up);
    Coord newCenter = _eyes + viewdir * dist;
    if (newCenter != _center) {
      _center = newCenter;
      notifyModified();
    }
  }
}
//====================================================
void Camera::translate(float x, float y, float z) {
  translate(Vec3f(x, y, z));
}
//====================================================
void Camera::rotateX(float angle) {
  rotate(angle, 1.0f, 0.0f, 0.0f);
}
//====================================================
void Camera::rotateY(float angle) {
  rotate(angle, 0.0f, 1.0f, 0.0f);
}
//====================================================
void Camera::rotateZ(float angle) {
  rotate(angle, 0.0f, 0.0f, 1.0f);
}
//====================================================
void Camera::centerScene(tlp::BoundingBox sceneBoundingBox) {

  if (!sceneBoundingBox.isValid()) {
    sceneBoundingBox = _sceneBoundingBox;
  }

  Coord maxC(sceneBoundingBox[1]);
  Coord minC(sceneBoundingBox[0]);

  double dx = maxC[0] - minC[0];
  double dy = maxC[1] - minC[1];

  Coord center=(maxC + minC) / 2.f;

  if ((dx==0) && (dy==0))
    dx = dy = 10.0;

  int width = _viewport[2];
  int height = _viewport[3];

  double wdx=width/dx;
  double hdy=height/dy;

  float sceneRadius = 0;
  if (dx<dy) {
    if (wdx<hdy) {
      sceneRadius=static_cast<float>(dx);
    } else {
      if (width < height)
        sceneRadius=static_cast<float>(dx*wdx/hdy);
      else
        sceneRadius=static_cast<float>(dy);
    }
  } else {
    if (wdx>hdy) {
      sceneRadius=static_cast<float>(dy);
    } else {
      if (height < width)
        sceneRadius=static_cast<float>(dy*hdy/wdx);
      else
        sceneRadius=static_cast<float>(dx);
    }
  }

  Coord eyes(0, 0, sceneRadius);
  eyes += center;

  setCenter(center);
  setSceneRadius(sceneRadius, sceneBoundingBox);
  setEyes(eyes);
  setUp(Coord(0,1,0));
  setZoomFactor(1);
}

void Camera::rotate(float angle, float x, float y, float z) {
  float c = cos(angle);
  float s = sin(angle);
  MatrixGL rotMatrix;
  rotMatrix.fill(0);
  rotMatrix[0][0] = x*x*(1-c)+c;
  rotMatrix[1][1] = y*y*(1-c)+c;
  rotMatrix[2][2] = z*z*(1-c)+c;
  rotMatrix[3][3] = 1;

  rotMatrix[0][1] = y*x*(1-c)+z*s;
  rotMatrix[0][2] = x*z*(1-c)-y*s;
  rotMatrix[1][0] = x*y*(1-c)-z*s;
  rotMatrix[1][2] = y*z*(1-c)+x*s;
  rotMatrix[2][0] = x*z*(1-c)+y*s;
  rotMatrix[2][1] = y*z*(1-c)-x*s;

  float dist = _eyes.dist(_center);
  _modelviewMatrix = rotMatrix * _modelviewMatrix;
  Vec3f viewdir;
  getCameraParametersFromModelviewMatrix(_modelviewMatrix, _eyes, viewdir, _up);
  _center = _eyes + viewdir * dist;

  notifyModified();
}

bool Camera::hasRotation() const {
  return _modelviewMatrix[0] != Vec4f(1,0,0,0) ||
      _modelviewMatrix[1] != Vec4f(0,1,0,0) ||
      _modelviewMatrix[2] != Vec4f(0,0,1,0);
}

void Camera::notifyModified() {
  sendEvent(Event(*this, Event::TLP_MODIFICATION));
}

void Camera::setModelViewMatrix(const tlp::MatrixGL &mdvMat) {
  _modelviewMatrix = mdvMat;
  _mdvMatCoherent = true;
  notifyModified();
}

void Camera::setProjectionMatrix(const tlp::MatrixGL &projMat) {
  _projectionMatrix = projMat;
  _projectionMatCoherent = true;
  notifyModified();
}
