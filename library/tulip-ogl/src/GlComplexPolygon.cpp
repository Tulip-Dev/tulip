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
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#endif

#include "tulip/GlComplexPolygon.h"
#include "tulip/GlTools.h"
#include "tulip/GlLayer.h"
#include "tulip/GlTextureManager.h"
#include "tulip/ParametricCurves.h"

#include <tulip/TlpTools.h>

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
#elif defined (WIN32)
typedef void (CALLBACK*GLUTesselatorFunction)(void);
#else
#error "Error - need to define type GLUTesselatorFunction for this platform/compiler"
#endif


using namespace std;

namespace tlp {

void CALLBACK beginCallback(GLenum which, GLvoid *polygonData) {
    GlComplexPolygon *complexPolygon = static_cast<GlComplexPolygon *>(polygonData);
    complexPolygon->startPrimitive(which);
}

void CALLBACK errorCallback(GLenum errorCode) {
	const GLubyte *estring = gluErrorString(errorCode);
	cerr << "Tessellation Error: " << estring << endl;
}

void CALLBACK endCallback(GLvoid *polygonData) {
    GlComplexPolygon *complexPolygon = static_cast<GlComplexPolygon *>(polygonData);
	complexPolygon->endPrimitive();
}

void CALLBACK vertexCallback(GLvoid *vertex, GLvoid *polygonData) {
	const GLdouble *pointer = static_cast<GLdouble *>(vertex);
	GlComplexPolygon *complexPolygon = static_cast<GlComplexPolygon *>(polygonData);
	Coord v(pointer[0], pointer[1], pointer[2]);
	Color color(pointer[3], pointer[4], pointer[5], pointer[6]);
	Vec2f texCoord;
	texCoord[0] = pointer[0];
	texCoord[1] = pointer[1];
	complexPolygon->addVertex(v, color, texCoord);
}

void CALLBACK combineCallback(GLdouble coords[3], VERTEX *d[4], GLfloat w[4], VERTEX** dataOut, GLvoid *polygonData) {
	GlComplexPolygon *complexPolygon = static_cast<GlComplexPolygon *>(polygonData);
	VERTEX *vertex = complexPolygon->allocateNewVertex();
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

GlComplexPolygon::GlComplexPolygon(const vector<Coord> &coords,Color fcolor,int polygonEdgesType,const string &textureName):
    												currentVector(0),
    												outlined(false),
    												fillColor(fcolor),
    												outlineSize(1),
    												textureName(textureName){
	createPolygon(coords,polygonEdgesType);
	runTesselation();
}
//=====================================================
GlComplexPolygon::GlComplexPolygon(const vector<Coord> &coords,Color fcolor,Color ocolor,int polygonEdgesType,const string &textureName):
    												currentVector(0),
    												outlined(true),
    												fillColor(fcolor),
    												outlineColor(ocolor),
    												outlineSize(1),
    												textureName(textureName) {
	createPolygon(coords,polygonEdgesType);
	runTesselation();
}
//=====================================================
GlComplexPolygon::GlComplexPolygon(const vector<vector<Coord> >&coords,Color fcolor,int polygonEdgesType,const string &textureName):
    												currentVector(0),
    												outlined(false),
    												fillColor(fcolor),
    												outlineSize(1),
    												textureName(textureName){
	for(unsigned int i=0;i<coords.size();++i) {
		createPolygon(coords[i],polygonEdgesType);
		beginNewHole();
	}
	runTesselation();
}
//=====================================================
GlComplexPolygon::GlComplexPolygon(const vector<vector<Coord> >&coords,Color fcolor,Color ocolor,int polygonEdgesType,const string &textureName):
    												currentVector(0),
    												outlined(true),
    												fillColor(fcolor),
    												outlineColor(ocolor),
    												outlineSize(1),
    												textureName(textureName) {
	for(unsigned int i=0;i<coords.size();++i) {
		createPolygon(coords[i],polygonEdgesType);
		beginNewHole();
	}
	runTesselation();
}
//=====================================================
void GlComplexPolygon::createPolygon(const vector<Coord> &coords,int polygonEdgesType) {
	points.push_back(vector<Coord>());

	if (polygonEdgesType == 1){
		vector<Coord> catmullPoints;
		computeCatmullRomPoints(coords, catmullPoints, true, coords.size() * 20);
		for (size_t i = 0 ; i < catmullPoints.size() ; ++i) {
			addPoint(catmullPoints[i]);
		}
	} else if (polygonEdgesType == 2) {

		const unsigned int nbCurvePoints = 20;
		 addPoint(coords[0]);
		 double dec=1./nbCurvePoints;

	 	for(size_t i = 0 ; i+3 < coords.size() ; i+=3) {
	 		vector<Coord> controlPoints;
	 		vector<Coord> curvePoints;
	 		controlPoints.push_back(coords[i]);
	 		controlPoints.push_back(coords[i+1]);
	 		controlPoints.push_back(coords[i+2]);
	 		controlPoints.push_back(coords[i+3]);
	 		computeBezierPoints(controlPoints, curvePoints, nbCurvePoints);
	 		for (size_t j = 0 ; j < curvePoints.size() ; ++j) {
	 			addPoint(curvePoints[j]);
	 		}
	 	}

		addPoint(coords[coords.size()-1]);
	} else {
		for(vector<Coord>::const_iterator it=coords.begin();it!=coords.end();++it) {
			addPoint(*it);
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

	GLUtesselator *tobj;
	tobj = gluNewTess();

    gluTessCallback(tobj, GLU_TESS_BEGIN_DATA, reinterpret_cast<GLUTesselatorFunction>(&beginCallback));
    gluTessCallback(tobj, GLU_TESS_VERTEX_DATA, reinterpret_cast<GLUTesselatorFunction>(&vertexCallback));
    gluTessCallback(tobj, GLU_TESS_END_DATA, reinterpret_cast<GLUTesselatorFunction>(&endCallback));
    gluTessCallback(tobj, GLU_TESS_COMBINE_DATA, reinterpret_cast<GLUTesselatorFunction>(&combineCallback));
    gluTessCallback(tobj, GLU_TESS_ERROR, reinterpret_cast<GLUTesselatorFunction>(&errorCallback));
	
	//Compute number of points to compute and create a big tab to store points' informations
	unsigned int numberOfPoints=0;
	for(unsigned int v=0;v<points.size();++v) {
		numberOfPoints+=points[v].size();
	}

	GLdouble *pointsData = new GLdouble[7*numberOfPoints];

	unsigned int pointNumber=0;
	gluTessBeginPolygon(tobj, static_cast<void *>(this));
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

	for (size_t i = 0 ; i < allocatedVertices.size() ; ++i) {
		delete allocatedVertices[i];
	}
	allocatedVertices.clear();
	
}
//=====================================================
void GlComplexPolygon::startPrimitive(GLenum primitive) {
	currentPrimitive = primitive;
	nbPrimitiveVertices = 0;
	startIndicesMap[primitive].push_back(verticesMap[primitive].size());
	primitivesSet.insert(primitive);
}
//=====================================================	
void GlComplexPolygon::endPrimitive() {
	verticesCountMap[currentPrimitive].push_back(nbPrimitiveVertices);
}
//=====================================================	
void GlComplexPolygon::addVertex(const Coord &vertexCoord, const Color &vertexColor, const Vec2f &vertexTexCoord) {
	verticesMap[currentPrimitive].push_back(vertexCoord);
	colorsMap[currentPrimitive].push_back(vertexColor);
	texCoordsMap[currentPrimitive].push_back(vertexTexCoord);
	++nbPrimitiveVertices;
}
//=====================================================
VERTEX *GlComplexPolygon::allocateNewVertex() {
	VERTEX *vertex = new VERTEX();
	allocatedVertices.push_back(vertex);
	return vertex;
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

	for (set<GLenum>::iterator it = primitivesSet.begin() ; it != primitivesSet.end() ; ++it) {
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
