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
#if defined(__APPLE__)
#include <OpenGL/glew.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#ifndef CALLBACK
#define CALLBACK
#endif

#ifdef __APPLE_CC__
#if __APPLE_CC__ < 5400
// Tiger
typedef GLvoid (*GLUTesselatorFunction)(...);
#else
// Leopard
typedef GLvoid (*GLUTesselatorFunction)();
#endif
#elif defined( __mips ) || defined( __linux__ ) || defined( __FreeBSD_kernel__) || defined( __FreeBSD__ ) || defined( __OpenBSD__ ) || defined( __sun ) || defined (__CYGWIN__)
typedef GLvoid (*GLUTesselatorFunction)();
#elif defined ( WIN32)
typedef void (__stdcall*GLUTesselatorFunction)(void);
#else
#error "Error - need to define type GLUTesselatorFunction for this platform/compiler"
#endif

#include "tulip/GlComplexPolygon.h"
#include "tulip/GlTools.h"
#include "tulip/GlLayer.h"
#include "tulip/GlTextureManager.h"
#include "tulip/ParametricCurves.h"

#include <tulip/TlpTools.h>

using namespace std;

namespace tlp {

static GLenum currentMode;
static std::map<int, std::vector<Coord> > *verticesMapP;
static std::map<int, std::vector<Color> > *colorsMapP;
static std::map<int, std::vector<Vec2f> > *texCoordsMapP;
static std::map<int, std::vector<int> >*startIndicesMapP;
static std::map<int, std::vector<int> >*verticesCountMapP;
static std::set<int> *primitivesSetP;
static unsigned int nbVertices = 0;

void beginCallback(GLenum which)
{
	currentMode = which;
	nbVertices = 0;
	(*startIndicesMapP)[which].push_back((*verticesMapP)[which].size());
	(*primitivesSetP).insert(which);
}

void errorCallback(GLenum errorCode)
{
	const GLubyte *estring;

	estring = gluErrorString(errorCode);
	cerr << "Tessellation Error: " << estring << endl;
}

void endCallback(void)
{
	(*verticesCountMapP)[currentMode].push_back(nbVertices);
}


void vertexCallback(GLvoid *vertex)
{
	const GLdouble *pointer = static_cast<GLdouble *>(vertex);
	Coord v(pointer[0], pointer[1], pointer[2]);
	Vec2f texCoord;
	texCoord[0] = pointer[0];
	texCoord[1] = pointer[1];
	Color color(pointer[3], pointer[4], pointer[5], pointer[6]);
	(*verticesMapP)[currentMode].push_back(v);
	(*colorsMapP)[currentMode].push_back(color);
	(*texCoordsMapP)[currentMode].push_back(texCoord);
	++nbVertices;
}

typedef struct {
	GLdouble x; /* x vertex coordinate */
	GLdouble y; /* y vertex coordinate */
	GLdouble z; /* z vertex coordinate */
	GLdouble r; /* red color component of vertex */
	GLdouble g; /* green color component of vertex */
	GLdouble b; /* blue color component of vertex */
	GLdouble a; /* alpha color component of vertex */
} VERTEX;

static vector<VERTEX *> createdVerticesAfterCombine;

void combineCallback(GLdouble coords[3], VERTEX *d[4], GLfloat w[4], VERTEX** dataOut)
{
	VERTEX *vertex = new VERTEX;
	createdVerticesAfterCombine.push_back(vertex);
	vertex->x = coords[0];
	vertex->y = coords[1];
	vertex->z = coords[2];
	vertex->r = vertex->g = vertex->b = vertex->a = 0;
	for (unsigned int i = 0 ; i < 4 ; ++i) {
		if (d[i]) {
			vertex->r += w[i]*d[i]->r;
			vertex->g += w[i]*d[i]->g;
			vertex->b += w[i]*d[i]->b;
			vertex->a += w[i]*d[i]->a;
		}
	}
	*dataOut = vertex;
}

GlComplexPolygon::GlComplexPolygon(const vector<Coord> &coords,Color fcolor,int bezier,const string &textureName):
    												currentVector(0),
    												outlined(false),
    												fillColor(fcolor),
    												outlineSize(1),
    												textureName(textureName){
	createPolygon(coords,bezier);
	runTesselation();
}
//=====================================================
GlComplexPolygon::GlComplexPolygon(const vector<Coord> &coords,Color fcolor,Color ocolor,int bezier,const string &textureName):
    												currentVector(0),
    												outlined(true),
    												fillColor(fcolor),
    												outlineColor(ocolor),
    												outlineSize(1),
    												textureName(textureName) {
	createPolygon(coords,bezier);
	runTesselation();
}
//=====================================================
GlComplexPolygon::GlComplexPolygon(const vector<vector<Coord> >&coords,Color fcolor,int bezier,const string &textureName):
    												currentVector(0),
    												outlined(false),
    												fillColor(fcolor),
    												outlineSize(1),
    												textureName(textureName){
	for(unsigned int i=0;i<coords.size();++i) {
		createPolygon(coords[i],bezier);
		beginNewHole();
	}
	runTesselation();
}
//=====================================================
GlComplexPolygon::GlComplexPolygon(const vector<vector<Coord> >&coords,Color fcolor,Color ocolor,int bezier,const string &textureName):
    												currentVector(0),
    												outlined(true),
    												fillColor(fcolor),
    												outlineColor(ocolor),
    												outlineSize(1),
    												textureName(textureName) {
	for(unsigned int i=0;i<coords.size();++i) {
		createPolygon(coords[i],bezier);
		beginNewHole();
	}
	runTesselation();
}
//=====================================================
GlComplexPolygon::~GlComplexPolygon() {
}
//=====================================================
void GlComplexPolygon::createPolygon(const vector<Coord> &coords,int bezier) {
	points.push_back(vector<Coord>());
	if(bezier==0) {
		for(vector<Coord>::const_iterator it=coords.begin();it!=coords.end();++it) {
			addPoint(*it);
		}
	}else{
		vector<Coord> catmullPoints;
		computeCatmullRomPoints(coords, catmullPoints, true, coords.size() * 20);
		for (size_t i = 0 ; i < catmullPoints.size() ; ++i) {
			addPoint(catmullPoints[i]);
		}
	}
}
//=====================================================
void GlComplexPolygon::setOutlineMode(const bool outlined) {
	this->outlined = outlined;
}
//=====================================================
void GlComplexPolygon::setOutlineSize(double size) {
	outlineSize=size;
}
//=====================================================
void GlComplexPolygon::addPoint(const Coord& point) {
	points[currentVector].push_back(point);
	boundingBox.expand(point);
}
//=====================================================
void GlComplexPolygon::beginNewHole() {
	currentVector++;
}
void GlComplexPolygon::runTesselation() {

	verticesMapP = &verticesMap;
	colorsMapP = &colorsMap;
	texCoordsMapP = &texCoordsMap;
	startIndicesMapP = &startIndicesMap;
	verticesCountMapP = &verticesCountMap;
	primitivesSetP = &primitivesSet;

	GLUtesselator *tobj;
	tobj = gluNewTess();

	gluTessCallback(tobj, GLU_TESS_VERTEX,
			(GLUTesselatorFunction)vertexCallback);
	gluTessCallback(tobj, GLU_TESS_BEGIN,
			(GLUTesselatorFunction)beginCallback);
	gluTessCallback(tobj, GLU_TESS_END,
			(GLUTesselatorFunction)endCallback);
	gluTessCallback(tobj, GLU_TESS_ERROR,
			(GLUTesselatorFunction)errorCallback);
	gluTessCallback(tobj, GLU_TESS_COMBINE,
			(GLUTesselatorFunction)combineCallback);

	//Compute number of points to compute and create a big tab to store points' informations
	unsigned int numberOfPoints=0;
	for(unsigned int v=0;v<points.size();++v) {
		numberOfPoints+=points[v].size();
	}

	GLdouble *pointsData = new GLdouble[7*numberOfPoints];

	unsigned int pointNumber=0;
	gluTessBeginPolygon(tobj, NULL);
	for(unsigned int v=0;v<points.size();++v) {
		gluTessBeginContour(tobj);
		for(unsigned int i=0; i < points[v].size(); ++i) {
			pointsData[pointNumber*7]=points[v][i][0];
			pointsData[pointNumber*7+1]=points[v][i][1];
			pointsData[pointNumber*7+2]=points[v][i][2];
			pointsData[pointNumber*7+3]=fillColor[0];
			pointsData[pointNumber*7+4]=fillColor[1];
			pointsData[pointNumber*7+5]=fillColor[2];
			pointsData[pointNumber*7+6]=fillColor[3];
			gluTessVertex(tobj,&pointsData[pointNumber*7],&pointsData[pointNumber*7]);
			pointNumber++;
		}
		gluTessEndContour(tobj);
	}
	gluTessEndPolygon(tobj);
	gluDeleteTess(tobj);
	delete [] pointsData;

	for (size_t i = 0 ; i < createdVerticesAfterCombine.size() ; ++i) {
		delete createdVerticesAfterCombine[i];
	}
	createdVerticesAfterCombine.clear();
}
//=====================================================
void GlComplexPolygon::draw(float,Camera *) {

	if(cameraIs3D()){
		glEnable(GL_LIGHTING);
	}else{
		glDisable(GL_LIGHTING);
	}

	glDisable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if(textureName!=""){
		if(GlTextureManager::getInst().activateTexture(textureName))
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	}

	glNormal3f(0.0f, 0.0f, 1.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	for (set<int>::iterator it = primitivesSet.begin() ; it != primitivesSet.end() ; ++it) {
		glVertexPointer(3, GL_FLOAT, 3*sizeof(GLfloat), &verticesMap[*it][0][0]);
		glColorPointer(4,GL_UNSIGNED_BYTE, 4*sizeof(GLubyte), &colorsMap[*it][0][0]);
		glTexCoordPointer(2, GL_FLOAT, 2*sizeof(GLfloat), &texCoordsMap[*it][0]);
		glMultiDrawArrays(*it, &startIndicesMap[*it][0], &verticesCountMap[*it][0], verticesCountMap[*it].size());
	}

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if(textureName!="") {
		GlTextureManager::getInst().desactivateTexture();
	}

	if (outlined) {
		glLineWidth(outlineSize);
		setMaterial(outlineColor);
		for(unsigned int v=0;v<points.size();++v) {
			glVertexPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), &points[v][0]);
			glDrawArrays(GL_LINE_LOOP, 0, points[v].size());
		}
	}

	glDisableClientState(GL_VERTEX_ARRAY);

	glTest(__PRETTY_FUNCTION__);
}
//===========================================================
void GlComplexPolygon::translate(const Coord& vec) {
	boundingBox.translate(vec);
	for(vector<vector<Coord> >::iterator it = points.begin(); it != points.end(); ++it){
		for(vector<Coord>::iterator it2 = (*it).begin(); it2 != (*it).end(); ++it2) {
			(*it2) += vec;
		}
	}
}
//===========================================================
void GlComplexPolygon::getXML(xmlNodePtr rootNode) {

	GlXMLTools::createProperty(rootNode, "type", "GlComplexPolygon");

	getXMLOnlyData(rootNode);

}
//===========================================================
void GlComplexPolygon::getXMLOnlyData(xmlNodePtr rootNode) {
	xmlNodePtr dataNode=NULL;

	GlXMLTools::getDataNode(rootNode,dataNode);

	GlXMLTools::getXML(dataNode,"numberOfVector",points.size());
	for(unsigned int i=0;i<points.size();++i){
		stringstream str;
		str << i ;

		if(!points[i].empty()){
			GlXMLTools::getXML(dataNode,"points"+str.str(),points[i]);
		}else{
			GlXMLTools::getXML(dataNode,"points"+str.str(),vector<Coord>());
		}
	}
	GlXMLTools::getXML(dataNode,"fillColor",fillColor);
	GlXMLTools::getXML(dataNode,"outlineColor",outlineColor);
	GlXMLTools::getXML(dataNode,"outlined",outlined);
	GlXMLTools::getXML(dataNode,"outlineSize",outlineSize);
	GlXMLTools::getXML(dataNode,"textureName",textureName);
}
//============================================================
void GlComplexPolygon::setWithXML(xmlNodePtr rootNode) {
	xmlNodePtr dataNode=NULL;

	GlXMLTools::getDataNode(rootNode,dataNode);

	// Parse Data
	if(dataNode) {
		int numberOfVector;
		GlXMLTools::setWithXML(dataNode,"numberOfVector",numberOfVector);
		for(int i=0;i<numberOfVector;++i) {
			stringstream str;
			str << i ;
			points.push_back(vector<Coord>());
			GlXMLTools::setWithXML(dataNode,"points"+str.str(),points[i]);
		}

		GlXMLTools::setWithXML(dataNode,"fillColor",fillColor);
		GlXMLTools::setWithXML(dataNode,"outlineColor",outlineColor);
		GlXMLTools::setWithXML(dataNode,"outlined",outlined);
		GlXMLTools::setWithXML(dataNode,"outlineSize",outlineSize);
		GlXMLTools::setWithXML(dataNode,"textureName",textureName);

		for(vector<vector<Coord> >::iterator it= points.begin();it!=points.end();++it) {
			for(vector<Coord>::iterator it2=(*it).begin();it2!=(*it).end();++it2) {
				boundingBox.expand(*it2);
			}
		}
	}
}
}
