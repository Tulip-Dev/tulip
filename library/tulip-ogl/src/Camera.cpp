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
#include <tulip/Camera.h>

#include <cmath>

#include <tulip/GlTools.h>
#include <tulip/GlScene.h>
#include <tulip/GlXMLTools.h>

#if defined(_MSC_VER)
#include <Windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

using namespace std;

namespace tlp {
//====================================================
Camera::Camera(GlScene* scene,Coord center,Coord eyes, Coord up, double zoomFactor, double sceneRadius):
  matrixCoherent(false),
  center(center),
  eyes(eyes),
  up(up),
  zoomFactor(zoomFactor),
  sceneRadius(sceneRadius),
  scene(scene),
  d3(true) {}
//====================================================
Camera::Camera(GlScene* scene,bool d3) :
  matrixCoherent(false),scene(scene),d3(d3) {}
//====================================================
Camera::~Camera() {
  observableDeleted();
}
//===================================================
Camera& Camera::operator=(const Camera& camera) {
  matrixCoherent=false;
  center=camera.getCenter();
  eyes=camera.getEyes();
  up=camera.getUp();
  zoomFactor=camera.getZoomFactor();
  sceneRadius=camera.getSceneRadius();
  sceneBoundingBox=camera.getBoundingBox();
  scene=camera.getScene();
  d3=camera.is3D();
  return *this;
}
//===================================================
void Camera::setScene(GlScene* scene) {
  this->scene=scene;
}
//===================================================
BoundingBox Camera::getBoundingBox() const {
  BoundingBox bb;
  bb.expand(screenTo3DWorld(Coord(scene->getViewport()[0],scene->getViewport()[1],0)));
  bb.expand(screenTo3DWorld(Coord(scene->getViewport()[0]+scene->getViewport()[2],scene->getViewport()[1]+scene->getViewport()[3],0)));
  return bb;
}
//====================================================
void Camera::move(float speed) {
  Coord move = eyes - center;
  move *= speed/move.norm();
  eyes += move;
  center += move;
  matrixCoherent=false;

  if (hasOnlookers())
    sendEvent(Event(*this, Event::TLP_MODIFICATION));
}
//====================================================
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
  matrixCoherent=false;

  if (hasOnlookers())
    sendEvent(Event(*this, Event::TLP_MODIFICATION));
}
//====================================================
void Camera::strafeLeftRight(float speed) {
  Coord strafeVector=((eyes-center)^up);
  strafeVector *= speed / strafeVector.norm();
  center += strafeVector;
  eyes   += strafeVector;
  matrixCoherent=false;

  if (hasOnlookers())
    sendEvent(Event(*this, Event::TLP_MODIFICATION));
}
//====================================================
void Camera::strafeUpDown(float speed) {
  Coord strafeVector(up);
  strafeVector *= speed / strafeVector.norm();
  center += strafeVector;
  eyes   += strafeVector;
  matrixCoherent=false;

  if (hasOnlookers())
    sendEvent(Event(*this, Event::TLP_MODIFICATION));
}
//====================================================
void Camera::initGl() {
  initProjection();
  initModelView();
  initLight();
}
//====================================================
void Camera::initLight() {

#ifndef NDEBUG
  GLuint error = glGetError();

  if ( error != GL_NO_ERROR)
    tlp::warning() << "[OpenGL Error] => " << glGetErrorDescription(error).c_str() << endl << "\tin : " << __PRETTY_FUNCTION__ << " begin" << endl;

#endif

  GLfloat pos[4];

  if(d3) {
    // set positional light for 3D camera
    eyes.get(pos[0],pos[1],pos[2]);
    pos[0]=pos[0] + ((eyes[0]-center[0])/zoomFactor) + (eyes[0]-center[0])*4;
    pos[1]=pos[1] + ((eyes[1]-center[1])/zoomFactor) + (eyes[1]-center[1])*4;
    pos[2]=pos[2] + ((eyes[2]-center[2])/zoomFactor) + (eyes[2]-center[2])*4;
    pos[3]=1;
  }
  else {
    // set directional light for 2D camera
    pos[0]=0;
    pos[1]=0;
    pos[2]=100;
    pos[3]=0;
  }

  GLfloat amb[4] = {0.3f,0.3f , 0.3f ,0.3f};
  GLfloat dif[4] = {0.5f,0.5f,0.5f,1.0f};
  GLfloat specular[4] = {0.0f,0.0f,0.0f,1.0f};
  GLfloat attC[3] = {1.0f,1.0f,1.0f};
  GLfloat attL[3] = {0.0f,0.0f,0.0f};
  GLfloat attQ[3] = {0.0f,0.0f,0.0f};

  glEnable( GL_LIGHTING );
  glEnable( GL_LIGHT0 );

  glLightfv( GL_LIGHT0, GL_POSITION, pos );
  glLightfv( GL_LIGHT0, GL_AMBIENT, amb );
  glLightfv( GL_LIGHT0, GL_DIFFUSE, dif );
  glLightfv( GL_LIGHT0, GL_CONSTANT_ATTENUATION, attC );
  glLightfv( GL_LIGHT0, GL_LINEAR_ATTENUATION, attL );
  glLightfv( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, attQ );
  glLightfv( GL_LIGHT0, GL_SPECULAR , specular);

#ifndef NDEBUG
  error = glGetError();

  if ( error != GL_NO_ERROR)
    tlp::warning() << "[OpenGL Error] => " << glGetErrorDescription(error).c_str() << endl << "\tin : " << __PRETTY_FUNCTION__ << "end" << endl;

#endif
}
//====================================================
void Camera::initProjection(const Vector<int, 4>& viewport,bool reset) {
  glMatrixMode(GL_PROJECTION);

  if(reset) glLoadIdentity();

  double _near;
  double _far;


  bool valid = sceneBoundingBox.isValid();
  Vec3f v1 = sceneBoundingBox[0];
  Vec3f v2 = sceneBoundingBox[1];

  if(valid && v1 != v2) {
    sceneBoundingBox.expand(eyes);
    Coord diagCoord(sceneBoundingBox[1]-sceneBoundingBox[0]);
    double diag=2*sqrt(diagCoord[0]*diagCoord[0]+diagCoord[1]*diagCoord[1]+diagCoord[2]*diagCoord[2]);
    _near=-diag;
    _far=diag;
  }
  else {
    _near=-sceneRadius;
    _far=sceneRadius;
  }

  if(d3) {
    float ratio = double(viewport[2])/double(viewport[3]);

    if(scene->isViewOrtho()) {
      if (ratio>1)
        glOrtho(-ratio*sceneRadius/2.0/zoomFactor, ratio*sceneRadius/2.0/zoomFactor,
                -sceneRadius/2.0/zoomFactor, sceneRadius/2.0/zoomFactor,
                _near,_far);
      else
        glOrtho(-sceneRadius/2.0/zoomFactor, sceneRadius/2.0/zoomFactor,
                1./ratio * - sceneRadius/2.0/zoomFactor, 1./ratio * sceneRadius/2.0/zoomFactor,
                _near,_far);
    }
    else {
      if (ratio>1)
        glFrustum(-ratio/2.0/zoomFactor, ratio/2.0/zoomFactor,
                  -0.5/zoomFactor, 0.5/zoomFactor,
                  1.0 , sceneRadius*2.0);
      else
        glFrustum(-0.5/zoomFactor, 0.5/zoomFactor,
                  -1.0/(ratio/0.5*zoomFactor), 1.0/(ratio/0.5*zoomFactor),
                  1.0 , sceneRadius*2.0);
    }

    glEnable(GL_DEPTH_TEST);
  }
  else {
    glOrtho(0,viewport[2],0,viewport[3], -100, 100);
    glDisable(GL_DEPTH_TEST);
  }

#ifndef NDEBUG
  GLenum error = glGetError();

  if ( error != GL_NO_ERROR)
    tlp::warning() << "[OpenGL Error] => " << glGetErrorDescription(error).c_str() << endl << "\tin : " << __PRETTY_FUNCTION__ << endl;

#endif
}
//====================================================
void Camera::initProjection(bool reset) {
  Vector<int, 4> viewport=scene->getViewport();
  assert(viewport[2]!=0 && viewport[3]!=0);
  initProjection(viewport,reset);
}
//====================================================
void Camera::initModelView() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if(d3) {
    /* original code was
    gluLookAt(eyes[0], eyes[1], eyes[2],
              center[0], center[1], center[2],
              up[0], up[1], up[2]);
    */
    // but gluLookAt is deprecated on MacOSX 10.9
    // so we prefer to reimplement it
    // from http://www.opengl.org/sdk/docs/man2/xhtml/gluLookAt.xml
    Coord f(center);
    f -= eyes;
    // normalize
    f /= f.norm();

    Coord up2(up);
    // normalize
    up2 /= up.norm();

    Coord s(f);
    s ^= up2;

    up2 = s/s.norm() ^ f;

    Matrix<float, 4> m;
    m[0][0] = s[0];
    m[1][0] = s[1];
    m[2][0] = s[2];
    m[0][3] = m[1][3] = m[2][3] = 0.0;
    m[0][1] = up2[0];
    m[1][1] = up2[1];
    m[2][1] = up2[2];
    m[0][2] = -f[0];
    m[1][2] = -f[1];
    m[2][2] = -f[2];
    m[3][0] = m[3][1] = m[3][2] = 0.0;
    m[3][3] = 1.0;

    glMultMatrixf((GLfloat*)&m);
    glTranslatef(-eyes[0], -eyes[1], -eyes[2]);
  }

  glGetFloatv (GL_MODELVIEW_MATRIX, (GLfloat*)&modelviewMatrix);
  glGetFloatv (GL_PROJECTION_MATRIX, (GLfloat*)&projectionMatrix);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glMultMatrixf((GLfloat*)&projectionMatrix);
  glMultMatrixf((GLfloat*)&modelviewMatrix);
  glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)&transformMatrix);
  glPopMatrix();
  matrixCoherent=true;

#ifndef NDEBUG
  GLenum error = glGetError();

  if ( error != GL_NO_ERROR)
    tlp::warning() << "[OpenGL Error] => " << glGetErrorDescription(error).c_str() << endl << "\tin : " << __PRETTY_FUNCTION__ << endl;

#endif
}
//====================================================
void Camera::setSceneRadius(double sceneRadius,const BoundingBox sceneBoundingBox) {
  this->sceneRadius=sceneRadius;
  this->sceneBoundingBox=sceneBoundingBox;
  matrixCoherent=false;

  if (hasOnlookers())
    sendEvent(Event(*this, Event::TLP_MODIFICATION));
}
//====================================================
void Camera::setZoomFactor(double zoomFactor) {
  if(zoomFactor>1E10)
    return;

  this->zoomFactor=zoomFactor;
  matrixCoherent=false;

  if (hasOnlookers())
    sendEvent(Event(*this, Event::TLP_MODIFICATION));
}
//====================================================
void Camera::setEyes(const Coord& eyes) {
  this->eyes=eyes;
  matrixCoherent=false;

  if (hasOnlookers())
    sendEvent(Event(*this, Event::TLP_MODIFICATION));
}
//====================================================
void Camera::setCenter(const Coord& center) {
  this->center=center;
  matrixCoherent=false;

  if (hasOnlookers())
    sendEvent(Event(*this, Event::TLP_MODIFICATION));
}
//====================================================
void Camera::setUp(const Coord& up) {
  this->up=up;
  matrixCoherent=false;

  if (hasOnlookers())
    sendEvent(Event(*this, Event::TLP_MODIFICATION));
}
//====================================================
void Camera::getProjAndMVMatrix(const Vector<int, 4>& viewport,Matrix<float, 4> &projectionMatrix,Matrix<float, 4> &modelviewMatrix) const {
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  // We have a cast to remove const on this
  ((Camera*)this)->initProjection(viewport);
  ((Camera*)this)->initModelView();
  projectionMatrix=this->projectionMatrix;
  modelviewMatrix=this->modelviewMatrix;
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}
//====================================================
void Camera::getTransformMatrix(const Vector<int, 4>& viewport,Matrix<float, 4> &transformMatrix) const {
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  // We have a cast to remove const on this
  ((Camera*)this)->initProjection(viewport);
  ((Camera*)this)->initModelView();
  transformMatrix=this->transformMatrix;
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}
//====================================================
Coord Camera::screenTo3DWorld(const Coord &point) const {
  // We have a cast to remove const on this
  ((Camera *)this)->initProjection();
  ((Camera *)this)->initModelView();

  Vector<int, 4> viewport = getViewport();

  //Try to find a good z-coordinate for reverse projection
  Coord pScr = projectPoint(Coord(0,0,0), transformMatrix, viewport);

  pScr[0] = viewport[0] + viewport[2] - point[0];
  pScr[1] = viewport[1] + viewport[3] - point[1];

  MatrixGL tmp(transformMatrix);
  tmp.inverse();
  return unprojectPoint(pScr, tmp, viewport);
}
//====================================================
Coord Camera::worldTo2DScreen(const Coord &obj) const {
  // We have a cast to remove const on this
  ((Camera *)this)->initProjection();
  ((Camera *)this)->initModelView();

  Vector<int, 4> viewport = getViewport();
  return projectPoint(obj, transformMatrix, viewport) - Coord(viewport[0], viewport[1]);
}
//====================================================
Vector<int, 4> Camera::getViewport() const {
  return scene->getViewport();
}
//====================================================
void Camera::getXML(string &outString) {
  GlXMLTools::beginDataNode(outString);
  GlXMLTools::getXML(outString,"center",center);
  GlXMLTools::getXML(outString,"eyes",eyes);
  GlXMLTools::getXML(outString,"up",up);
  GlXMLTools::getXML(outString,"zoomFactor",zoomFactor);
  GlXMLTools::getXML(outString,"sceneRadius",sceneRadius);
  GlXMLTools::getXML(outString,"d3",d3);

  if(sceneBoundingBox.isValid()) {
    GlXMLTools::getXML(outString,"sceneBoundingBox0",Coord(sceneBoundingBox[0]));
    GlXMLTools::getXML(outString,"sceneBoundingBox1",Coord(sceneBoundingBox[1]));
  }

  GlXMLTools::endDataNode(outString);
}
//====================================================
void Camera::setWithXML(const string &inString, unsigned int &currentPosition) {

  GlXMLTools::enterDataNode(inString,currentPosition);

  Coord bbTmp;

  GlXMLTools::setWithXML(inString,currentPosition,"center",center);
  GlXMLTools::setWithXML(inString,currentPosition,"eyes",eyes);
  GlXMLTools::setWithXML(inString,currentPosition,"up",up);
  GlXMLTools::setWithXML(inString,currentPosition,"zoomFactor",zoomFactor);
  GlXMLTools::setWithXML(inString,currentPosition,"sceneRadius",sceneRadius);
  GlXMLTools::setWithXML(inString,currentPosition,"d3",d3);

  if(GlXMLTools::checkNextXMLtag(inString,currentPosition,"sceneBoundingBox0")) {
    GlXMLTools::setWithXML(inString,currentPosition,"sceneBoundingBox0",bbTmp);
    sceneBoundingBox.expand(bbTmp);
  }

  if(GlXMLTools::checkNextXMLtag(inString,currentPosition,"sceneBoundingBox1")) {
    GlXMLTools::setWithXML(inString,currentPosition,"sceneBoundingBox1",bbTmp);
    sceneBoundingBox.expand(bbTmp);
  }

  GlXMLTools::leaveDataNode(inString,currentPosition);
}
}
