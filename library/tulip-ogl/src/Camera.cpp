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
#include "tulip/Camera.h"

#include <cmath>

#include "tulip/GlTools.h"
#include "tulip/GlScene.h"

#include <iostream>

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
    						objectTransformation(false),
    						d3(true){
}
//====================================================
Camera::Camera(GlScene* scene,bool d3) :
    						matrixCoherent(false),scene(scene),d3(d3) {
}
//====================================================
Camera::~Camera() {
  notifyDestroy((Camera*)this);
}
//===================================================
void Camera::setScene(GlScene* scene) {
	this->scene=scene;
}
//====================================================
void Camera::move(float speed) {
	Coord move = eyes - center;
	move *= speed/move.norm();
	eyes += move;
	center += move;
	matrixCoherent=false;
	notifyPointOfViewModified(this);
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
	notifyPointOfViewModified(this);
}
//====================================================
void Camera::strafeLeftRight(float speed) {
	Coord strafeVector=((eyes-center)^up);
	strafeVector *= speed / strafeVector.norm();
	center += strafeVector;
	eyes   += strafeVector;
	matrixCoherent=false;
	notifyPointOfViewModified(this);
}
//====================================================
void Camera::strafeUpDown(float speed) {
	Coord strafeVector(up);
	strafeVector *= speed / strafeVector.norm();
	center += strafeVector;
	eyes   += strafeVector;
	matrixCoherent=false;
	notifyPointOfViewModified(this);
}
//====================================================
void Camera::initGl() {
	initProjection();
	initModelView();
	initLight();
}
//====================================================
void Camera::initLight() {
	GLuint error = glGetError();
	if ( error != GL_NO_ERROR)
		cerr << "[OpenGL Error] => " << gluErrorString(error) << endl << "\tin : " << __PRETTY_FUNCTION__ << " begin" << endl;
	if(d3) {
		GLfloat pos[4];
		eyes.get(pos[0],pos[1],pos[2]);
		pos[0]=pos[0] + ((eyes[0]-center[0])/zoomFactor) + (eyes[0]-center[0])*4;
		pos[1]=pos[1] + ((eyes[1]-center[1])/zoomFactor) + (eyes[1]-center[1])*4;
		pos[2]=pos[2] + ((eyes[2]-center[2])/zoomFactor) + (eyes[2]-center[2])*4;
		pos[3]=1;
		GLfloat amb[4] = {0.3,0.3 , 0.3 ,0.3};
		GLfloat dif[4] = {0.5,0.5,0.5,1};
		GLfloat specular[4] = {0,0,0,1};
		GLfloat attC[3] = {1.,1.,1.};
		GLfloat attL[3] = {0,0,0};
		GLfloat attQ[3] = {0,0,0};

		glEnable( GL_LIGHTING );
		glEnable( GL_LIGHT0 );

		glLightfv( GL_LIGHT0, GL_POSITION, pos );
		glLightfv( GL_LIGHT0, GL_AMBIENT, amb );
		glLightfv( GL_LIGHT0, GL_DIFFUSE, dif );
		glLightfv( GL_LIGHT0, GL_CONSTANT_ATTENUATION, attC );
		glLightfv( GL_LIGHT0, GL_LINEAR_ATTENUATION, attL );
		glLightfv( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, attQ );
		glLightfv( GL_LIGHT0, GL_SPECULAR , specular);
	}else{
		glDisable( GL_LIGHTING );
		glDisable( GL_LIGHT0 );
	}
	error = glGetError();
	if ( error != GL_NO_ERROR)
		cerr << "[OpenGL Error] => " << gluErrorString(error) << endl << "\tin : " << __PRETTY_FUNCTION__ << "end" << endl;
}
//====================================================
void Camera::initProjection(const Vector<int, 4>& viewport,bool reset){
	glMatrixMode(GL_PROJECTION);
	if(reset) glLoadIdentity();

	double _near;
	double _far;
	if(sceneBoundingBox.isValid() && sceneBoundingBox[0]!=sceneBoundingBox[1]){
    sceneBoundingBox.expand(eyes);
    Coord diagCoord(sceneBoundingBox[1]-sceneBoundingBox[0]);
		double diag=2*sqrt(diagCoord[0]*diagCoord[0]+diagCoord[1]*diagCoord[1]+diagCoord[2]*diagCoord[2]);
		_near=-diag;
		_far=diag;
	}else{
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
		}else{
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
	}else{
		gluOrtho2D(viewport[0],viewport[0]+viewport[2],viewport[1],viewport[1]+viewport[3]);
		glDisable(GL_DEPTH_TEST);
	}
	GLenum error = glGetError();
	if ( error != GL_NO_ERROR)
		cerr << "[OpenGL Error] => " << gluErrorString(error) << endl << "\tin : " << __PRETTY_FUNCTION__ << endl;
}
//====================================================
void Camera::initProjection(bool reset) {
	Vector<int, 4> viewport=scene->getViewport();
	initProjection(viewport,reset);
}
//====================================================
void Camera::initModelView() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(d3) {
		gluLookAt(eyes[0], eyes[1], eyes[2],
				center[0], center[1], center[2],
				up[0], up[1], up[2]);


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
	}
	GLenum error = glGetError();
	if ( error != GL_NO_ERROR)
		cerr << "[OpenGL Error] => " << gluErrorString(error) << endl << "\tin : " << __PRETTY_FUNCTION__ << endl;
}
//====================================================
void Camera::setSceneRadius(double sceneRadius,const BoundingBox sceneBoundingBox) {
	this->sceneRadius=sceneRadius;
	this->sceneBoundingBox=sceneBoundingBox;
	matrixCoherent=false;
	notifyPointOfViewModified(this);
}
//====================================================
void Camera::setZoomFactor(double zoomFactor) {
	if(zoomFactor>1E10)
		return;

	this->zoomFactor=zoomFactor;
	matrixCoherent=false;
	notifyPointOfViewModified(this);
}
//====================================================
void Camera::setEyes(const Coord& eyes) {
	this->eyes=eyes;
	matrixCoherent=false;
	notifyPointOfViewModified(this);
}
//====================================================
void Camera::setCenter(const Coord& center) {
	this->center=center;
	matrixCoherent=false;
	notifyPointOfViewModified(this);
}
//====================================================
void Camera::setUp(const Coord& up) {
	this->up=up;
	matrixCoherent=false;
	notifyPointOfViewModified(this);
}
//====================================================
void Camera::addObjectTransformation(const Coord &translation,const Coord &scale,const Coord &baseCoord ) {
	objectScale.push_back(scale);
	objectTranslation.push_back(translation);
	objectCoord.push_back(baseCoord);

	objectTransformation=true;
}
//====================================================
void Camera::getObjectTransformation(vector<Coord> &translation,vector<Coord> &scale,vector<Coord> &objectCoord) {
	translation=objectTranslation;
	scale=objectScale;
	objectCoord=this->objectCoord;
}
//====================================================
bool Camera::haveObjectTransformation() {
	return objectTransformation;
}
//====================================================
void Camera::getProjAndMVMatrix(const Vector<int, 4>& viewport,Matrix<float, 4> &projectionMatrix,Matrix<float, 4> &modelviewMatrix){
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	initProjection(viewport);
	initModelView();
	projectionMatrix=this->projectionMatrix;
	modelviewMatrix=this->modelviewMatrix;
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
//====================================================
void Camera::getTransformMatrix(const Vector<int, 4>& viewport,Matrix<float, 4> &transformMatrix) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	initProjection(viewport);
	initModelView();
	transformMatrix=this->transformMatrix;
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
//====================================================
Coord Camera::screenTo3DWorld(const Coord &point) {
	initProjection();
	initModelView();

	Vector<int, 4> viewport = getViewport();
	//Try to find a good z-coordinate for reverse projection
	Coord pScr = projectPoint(Coord(0,0,0), transformMatrix, viewport);
	pScr[0] = (float)viewport[2] - point[0];
	pScr[1] = (float)viewport[3] - point[1] - 1.0;
	MatrixGL tmp(transformMatrix);
	tmp.inverse();
	return unprojectPoint(pScr, tmp, viewport);
}
//====================================================
Coord Camera::worldTo2DScreen(const Coord &obj) {
	initProjection();
	initModelView();

	Vector<int, 4> viewport = getViewport();
	return projectPoint(obj, transformMatrix, viewport);
}
//====================================================
Vector<int, 4> Camera::getViewport() {
	return scene->getViewport();
}
//====================================================
void Camera::getXML(xmlNodePtr rootNode){
	xmlNodePtr dataNode= NULL;

	stringstream strZF;
	stringstream strSR;
	stringstream strD3;

	GlXMLTools::createDataNode(rootNode,dataNode);
	GlXMLTools::getXML(dataNode,"center",center);
	GlXMLTools::getXML(dataNode,"eyes",eyes);
	GlXMLTools::getXML(dataNode,"up",up);
	GlXMLTools::getXML(dataNode,"zoomFactor",zoomFactor);
	GlXMLTools::getXML(dataNode,"sceneRadius",sceneRadius);
	GlXMLTools::getXML(dataNode,"d3",d3);
	if(sceneBoundingBox.isValid()){
		GlXMLTools::getXML(dataNode,"sceneBoundingBox0",Coord(sceneBoundingBox[0]));
		GlXMLTools::getXML(dataNode,"sceneBoundingBox1",Coord(sceneBoundingBox[1]));
	}
}
//====================================================
void Camera::setWithXML(xmlNodePtr rootNode){
	xmlNodePtr dataNode= NULL;

	GlXMLTools::getDataNodeDirectly(rootNode,dataNode);

	if(dataNode) {

		Coord bbTmp;

		GlXMLTools::setWithXML(dataNode,"center",center);
		GlXMLTools::setWithXML(dataNode,"eyes",eyes);
		GlXMLTools::setWithXML(dataNode,"up",up);
		GlXMLTools::setWithXML(dataNode,"zoomFactor",zoomFactor);
		GlXMLTools::setWithXML(dataNode,"sceneRadius",sceneRadius);
		GlXMLTools::setWithXML(dataNode,"d3",d3);
		GlXMLTools::setWithXML(dataNode,"sceneBoundingBox0",bbTmp);
		sceneBoundingBox.expand(bbTmp);
		GlXMLTools::setWithXML(dataNode,"sceneBoundingBox1",bbTmp);
		sceneBoundingBox.expand(bbTmp);

	}
}
}
