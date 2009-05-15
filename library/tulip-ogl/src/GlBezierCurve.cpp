/*
	  GlBezierCurve.cpp

   Created on: 10 avr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#include <GL/glew.h>

#include <tulip/GlTextureManager.h>
#include <tulip/GlTools.h>
#include <tulip/Curves.h>

#include "tulip/GlBezierCurve.h"
#include "tulip/FastBezier.h"
#include "tulip/GlShaderManager.h"

#include <sstream>

using namespace std;

static void drawCurve(const vector<tlp::Coord> &curvePoints, const tlp::Color &startColor, const tlp::Color &endColor,
					  const float startSize, const float endSize, const tlp::Coord &startN, const tlp::Coord &endN,
					  const string &texture) {

	unsigned int size;
	GLfloat *points = tlp::buildCurvePoints(curvePoints, tlp::getSizes(curvePoints, startSize, endSize), startN, endN ,size);
	vector<tlp::Color> curveColors = tlp::getColors(curvePoints, startColor, endColor);
	if(texture != "") {
		tlp::GlTextureManager::getInst().activateTexture(texture);
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	glBegin(GL_QUAD_STRIP);
	for (unsigned int i = 0; i < size; ++i) {
		tlp::setMaterial(curveColors[i]);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3fv(&points[i*3]);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3fv(&points[i*3 + size*3]);
	}
	glEnd();

	// if no texture set, draw the curve outline for anti-aliasing
	if(texture == "") {
		glEnable(GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_LINE_STRIP);
		for (unsigned int i = 0 ; i < size ; ++i) {
			tlp::setMaterial(curveColors[i]);
			glVertex3fv(&points[i*3]);
		}
		glEnd();

		glBegin(GL_LINE_STRIP);
		for (unsigned int i = 0 ; i < size ; ++i) {
			tlp::setMaterial(curveColors[i]);
			glVertex3fv(&points[i*3+size*3]);
		}
		glEnd();
	} else {
		tlp::GlTextureManager::getInst().desactivateTexture();
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);

	delete [] points;
}

static bool checkVboSupport() {
	return ((void*)(glGenBuffers)!=NULL) &&
	((void*)(glBindBuffer)!=NULL) &&
	((void*)(glBufferData)!=NULL);
}

static string generateBezierVertexShaderSrc(const unsigned int nbControlPoints) {
	ostringstream oss;
	oss << "#version 120" << endl;
	oss << "uniform vec3 controlPoints[" << nbControlPoints << "];" << endl;
	oss << "uniform float startWidth;" << endl;
	oss << "uniform float endWidth;" << endl;
	oss << "uniform vec4 startColor;" << endl;
	oss << "uniform vec4 endColor;" << endl;
	oss << "vec3 controlPointsCp[" << nbControlPoints << "];" << endl;
	oss << "// compute Bezier Point and its tangent associated to parameter t (0 <= t <= 1)" << endl;
	oss << "// using  de Casteljau 's algorithm" << endl;
	oss << "// return value is an array containing two vector of 3 float :" << endl;
	oss << "// the first one is the bezier point, the second is the tangent vector" << endl;
	oss << "vec3[2] computeBezierPointAndTangent(float t) {" << endl;
	oss << "	controlPointsCp = controlPoints;" << endl;
	oss << "    vec3[2] ret;" << endl;
	oss << "    ret[1] = vec3(0.,0.,0.);" << endl;
	oss << "    for (int i = " << (nbControlPoints - 1) << " ; i > 0 ; --i) {" << endl;
	oss << "		if (i ==1) {" << endl;
	oss << "			ret[1] = normalize(controlPointsCp[1] - controlPointsCp[0]);" << endl;
	oss << "		}" << endl;
	oss << "		for (int j = 0 ; j < i ; ++j) {" << endl;
	oss << "			controlPointsCp[j] += t * (controlPointsCp[j+1] - controlPointsCp[j]);" << endl;
	oss << "		}" << endl;
	oss << "	}" << endl;
	oss << "	ret[0] = controlPointsCp[0];" << endl;
	oss << "	return ret;" << endl;
	oss << "}" << endl;
	oss << "void main() {" << endl;
	oss << "	// the value of parameter t is stored in the x coordinates of the incoming vertex" << endl;
	oss << "  	vec3[2] tmp = computeBezierPointAndTangent(gl_Vertex.x);" << endl;
	oss << "  	vec3 bezierVertex = tmp[0];" << endl;
	oss << " 	vec3 tangent = tmp[1];" << endl;
	oss << "	vec3 normal = tangent;" << endl;
	oss << "  	normal.x = - tangent.y;" << endl;
	oss << " 	normal.y = tangent.x;" << endl;
	oss << "    float width = startWidth + gl_Vertex.x * (endWidth - startWidth);" << endl;
	oss <<"	    // the orientation of the quad vertiex to generate is stored in the y coordinates of te incoming vertex" << endl;
	oss <<"	    vec3 finalVertex = bezierVertex + gl_Vertex.y * (normal * width);" << endl;
	oss <<"  	gl_Position = gl_ModelViewProjectionMatrix * vec4(finalVertex, 1.0);" << endl;
	oss <<"  	gl_FrontColor = startColor + gl_Vertex.x * (endColor - startColor);" << endl;
	oss <<"     if (gl_Vertex.y > 0.0)" << endl;
	oss <<"     	gl_TexCoord[0].st = vec2(0.,0.);" << endl;
	oss <<"     else" << endl;
	oss << "    	gl_TexCoord[0].st = vec2(1.,1.);" << endl;
	oss <<"}" << endl;

	return oss.str();
}

static GLfloat curveQuadVertexData[160] = {
		0.0f, 1.0f,
		0.0f, -1.0f,
		1.0f / 39.0f, 1.0f,
		1.0f / 39.0f, -1.0f,
		2.0f / 39.0f, 1.0f,
		2.0f / 39.0f, -1.0f,
		3.0f / 39.0f, 1.0f,
		3.0f / 39.0f, -1.0f,
		4.0f / 39.0f, 1.0f,
		4.0f / 39.0f, -1.0f,
		5.0f / 39.0f, 1.0f,
		5.0f / 39.0f, -1.0f,
		6.0f / 39.0f, 1.0f,
		6.0f / 39.0f, -1.0f,
		7.0f / 39.0f, 1.0f,
		7.0f / 39.0f, -1.0f,
		8.0f / 39.0f, 1.0f,
		8.0f / 39.0f, -1.0f,
		9.0f / 39.0f, 1.0f,
		9.0f / 39.0f, -1.0f,
		10.0f / 39.0f, 1.0f,
		10.0f / 39.0f, -1.0f,
		11.0f / 39.0f, 1.0f,
		11.0f / 39.0f, -1.0f,
		12.0f / 39.0f, 1.0f,
		12.0f / 39.0f, -1.0f,
		13.0f / 39.0f, 1.0f,
		13.0f / 39.0f, -1.0f,
		14.0f / 39.0f, 1.0f,
		14.0f / 39.0f, -1.0f,
		15.0f / 39.0f, 1.0f,
		15.0f / 39.0f, -1.0f,
		16.0f / 39.0f, 1.0f,
		16.0f / 39.0f, -1.0f,
		17.0f / 39.0f, 1.0f,
		17.0f / 39.0f, -1.0f,
		18.0f / 39.0f, 1.0f,
		18.0f / 39.0f, -1.0f,
		19.0f / 39.0f, 1.0f,
		19.0f / 39.0f, -1.0f,
		20.0f / 39.0f, 1.0f,
		20.0f / 39.0f, -1.0f,
		21.0f / 39.0f, 1.0f,
		21.0f / 39.0f, -1.0f,
		22.0f / 39.0f, 1.0f,
		22.0f / 39.0f, -1.0f,
		23.0f / 39.0f, 1.0f,
		23.0f / 39.0f, -1.0f,
		24.0f / 39.0f, 1.0f,
		24.0f / 39.0f, -1.0f,
		25.0f / 39.0f, 1.0f,
		25.0f / 39.0f, -1.0f,
		26.0f / 39.0f, 1.0f,
		26.0f / 39.0f, -1.0f,
		27.0f / 39.0f, 1.0f,
		27.0f / 39.0f, -1.0f,
		28.0f / 39.0f, 1.0f,
		28.0f / 39.0f, -1.0f,
		29.0f / 39.0f, 1.0f,
		29.0f / 39.0f, -1.0f,
		30.0f / 39.0f, 1.0f,
		30.0f / 39.0f, -1.0f,
		31.0f / 39.0f, 1.0f,
		31.0f / 39.0f, -1.0f,
		32.0f / 39.0f, 1.0f,
		32.0f / 39.0f, -1.0f,
		33.0f / 39.0f, 1.0f,
		33.0f / 39.0f, -1.0f,
		34.0f / 39.0f, 1.0f,
		34.0f / 39.0f, -1.0f,
		35.0f / 39.0f, 1.0f,
		35.0f / 39.0f, -1.0f,
		36.0f / 39.0f, 1.0f,
		36.0f / 39.0f, -1.0f,
		37.0f / 39.0f, 1.0f,
		37.0f / 39.0f, -1.0f,
		38.0f / 39.0f, 1.0f,
		38.0f / 39.0f, -1.0f,
		1.0f, 1.0f,
		1.0f, -1.0f
};

static GLfloat curveQuadTopOutlineVertexData[80] = {
		0.0f, 1.0f,
		1.0f / 39.0f, 1.0f,
		2.0f / 39.0f, 1.0f,
		3.0f / 39.0f, 1.0f,
		4.0f / 39.0f, 1.0f,
		5.0f / 39.0f, 1.0f,
		6.0f / 39.0f, 1.0f,
		7.0f / 39.0f, 1.0f,
		8.0f / 39.0f, 1.0f,
		9.0f / 39.0f, 1.0f,
		10.0f / 39.0f, 1.0f,
		11.0f / 39.0f, 1.0f,
		12.0f / 39.0f, 1.0f,
		13.0f / 39.0f, 1.0f,
		14.0f / 39.0f, 1.0f,
		15.0f / 39.0f, 1.0f,
		16.0f / 39.0f, 1.0f,
		17.0f / 39.0f, 1.0f,
		18.0f / 39.0f, 1.0f,
		19.0f / 39.0f, 1.0f,
		20.0f / 39.0f, 1.0f,
		21.0f / 39.0f, 1.0f,
		22.0f / 39.0f, 1.0f,
		23.0f / 39.0f, 1.0f,
		24.0f / 39.0f, 1.0f,
		25.0f / 39.0f, 1.0f,
		26.0f / 39.0f, 1.0f,
		27.0f / 39.0f, 1.0f,
		28.0f / 39.0f, 1.0f,
		29.0f / 39.0f, 1.0f,
		30.0f / 39.0f, 1.0f,
		31.0f / 39.0f, 1.0f,
		32.0f / 39.0f, 1.0f,
		33.0f / 39.0f, 1.0f,
		34.0f / 39.0f, 1.0f,
		35.0f / 39.0f, 1.0f,
		36.0f / 39.0f, 1.0f,
		37.0f / 39.0f, 1.0f,
		38.0f / 39.0f, 1.0f,
		1.0f, 1.0f,
};

static GLfloat curveQuadBottomOutlineVertexData[80] = {
		0.0f, -1.0f,
		1.0f / 39.0f, -1.0f,
		2.0f / 39.0f, -1.0f,
		3.0f / 39.0f, -1.0f,
		4.0f / 39.0f, -1.0f,
		5.0f / 39.0f, -1.0f,
		6.0f / 39.0f, -1.0f,
		7.0f / 39.0f, -1.0f,
		8.0f / 39.0f, -1.0f,
		9.0f / 39.0f, -1.0f,
		10.0f / 39.0f, -1.0f,
		11.0f / 39.0f, -1.0f,
		12.0f / 39.0f, -1.0f,
		13.0f / 39.0f, -1.0f,
		14.0f / 39.0f, -1.0f,
		15.0f / 39.0f, -1.0f,
		16.0f / 39.0f, -1.0f,
		17.0f / 39.0f, -1.0f,
		18.0f / 39.0f, -1.0f,
		19.0f / 39.0f, -1.0f,
		20.0f / 39.0f, -1.0f,
		21.0f / 39.0f, -1.0f,
		22.0f / 39.0f, -1.0f,
		23.0f / 39.0f, -1.0f,
		24.0f / 39.0f, -1.0f,
		25.0f / 39.0f, -1.0f,
		26.0f / 39.0f, -1.0f,
		27.0f / 39.0f, -1.0f,
		28.0f / 39.0f, -1.0f,
		29.0f / 39.0f, -1.0f,
		30.0f / 39.0f, -1.0f,
		31.0f / 39.0f, -1.0f,
		32.0f / 39.0f, -1.0f,
		33.0f / 39.0f, -1.0f,
		34.0f / 39.0f, -1.0f,
		35.0f / 39.0f, -1.0f,
		36.0f / 39.0f, -1.0f,
		37.0f / 39.0f, -1.0f,
		38.0f / 39.0f, -1.0f,
		1.0f, -1.0f,
};

static GLfloat curveVertexData[80] = {
		0.0f, 0.0f,
		1.0f / 39.0f, 0.0f,
		2.0f / 39.0f, 0.0f,
		3.0f / 39.0f, 0.0f,
		4.0f / 39.0f, 0.0f,
		5.0f / 39.0f, 0.0f,
		6.0f / 39.0f, 0.0f,
		7.0f / 39.0f, 0.0f,
		8.0f / 39.0f, 0.0f,
		9.0f / 39.0f, 0.0f,
		10.0f / 39.0f, 0.0f,
		11.0f / 39.0f, 0.0f,
		12.0f / 39.0f, 0.0f,
		13.0f / 39.0f, 0.0f,
		14.0f / 39.0f, 0.0f,
		15.0f / 39.0f, 0.0f,
		16.0f / 39.0f, 0.0f,
		17.0f / 39.0f, 0.0f,
		18.0f / 39.0f, 0.0f,
		19.0f / 39.0f, 0.0f,
		20.0f / 39.0f, 0.0f,
		21.0f / 39.0f, 0.0f,
		22.0f / 39.0f, 0.0f,
		23.0f / 39.0f, 0.0f,
		24.0f / 39.0f, 0.0f,
		25.0f / 39.0f, 0.0f,
		26.0f / 39.0f, 0.0f,
		27.0f / 39.0f, 0.0f,
		28.0f / 39.0f, 0.0f,
		29.0f / 39.0f, 0.0f,
		30.0f / 39.0f, 0.0f,
		31.0f / 39.0f, 0.0f,
		32.0f / 39.0f, 0.0f,
		33.0f / 39.0f, 0.0f,
		34.0f / 39.0f, 0.0f,
		35.0f / 39.0f, 0.0f,
		36.0f / 39.0f, 0.0f,
		37.0f / 39.0f, 0.0f,
		38.0f / 39.0f, 0.0f,
		1.0f, 0.0f,
};

static GLuint vboId[4] = {0, 0, 0, 0};

namespace tlp {
//=============================================
GlBezierCurve::GlBezierCurve(const vector<Coord> &controlPoints,
		const Color &beginColor,
		const Color &endColor,
		const float &beginSize,
		const float &endSize,
		const string &texture):
			beginColor(beginColor),
			endColor(endColor),
			beginSize(beginSize),
			endSize(endSize),
			texture(texture) {

	nbControlPoints = controlPoints.size();
	for (int i = 0 ; i < nbControlPoints ; ++i) {
		boundingBox.check(controlPoints[i]);
	}

	ostringstream oss;
	oss << nbControlPoints;

	shaderProgramName = "bezier" + oss.str();
	shaderProgramOk = GlShaderManager::getInstance()->getShaderProgramId(shaderProgramName) != 0 ||
					  GlShaderManager::getInstance()->registerShaderProgramFromStrings(shaderProgramName, generateBezierVertexShaderSrc(nbControlPoints), "");


	if (shaderProgramOk) {
		controlPointsArray = new GLfloat[nbControlPoints * 3];
		for (int i = 0 ; i < nbControlPoints ; ++i) {
			controlPointsArray[3 * i] = controlPoints[i][0];
			controlPointsArray[3 * i + 1] = controlPoints[i][1];
			controlPointsArray[3 * i + 2] = controlPoints[i][2];
		}
	} else {
		this->controlPoints = controlPoints;
		controlPointsArray = NULL;
	}

	vboOk = checkVboSupport();
	if (shaderProgramOk && vboOk && vboId[0] == 0) {
		glGenBuffers(4, vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(curveQuadVertexData), curveQuadVertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, vboId[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(curveVertexData), curveVertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, vboId[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(curveQuadTopOutlineVertexData), curveQuadTopOutlineVertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, vboId[3]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(curveQuadBottomOutlineVertexData), curveQuadBottomOutlineVertexData, GL_STATIC_DRAW);
	}
}
//=====================================================
GlBezierCurve::~GlBezierCurve() {
	if (controlPointsArray != NULL) {
		delete [] controlPointsArray;
	}
}
//=====================================================
void GlBezierCurve::draw(float lod, Camera *camera) {

	if (texture != "") {
		GlTextureManager::getInst().activateTexture(texture);
	}

	if (shaderProgramOk) {

		GLuint shaderId = GlShaderManager::getInstance()->getShaderProgramId(shaderProgramName);
		GlShaderManager::getInstance()->activateShaderProgram(shaderProgramName);

		GLint controlPointsArrayloc = glGetUniformLocation(shaderId, "controlPoints");
		GLint startWidthLoc = glGetUniformLocation(shaderId, "startWidth");
		GLint endWidthLoc = glGetUniformLocation(shaderId, "endWidth");
		GLint startColorLoc = glGetUniformLocation(shaderId, "startColor");
		GLint endColorLoc = glGetUniformLocation(shaderId, "endColor");

		glUniform3fv(controlPointsArrayloc, nbControlPoints, controlPointsArray);
		glUniform1f(startWidthLoc, beginSize);
		glUniform1f(endWidthLoc, endSize);
		glUniform4f(startColorLoc, beginColor[0] / 255.0f, beginColor[1] / 255.0f, beginColor[2] / 255.0f, beginColor[3] / 255.0f);
		glUniform4f(endColorLoc, endColor[0] / 255.0f, endColor[1] / 255.0f, endColor[2] / 255.0f, endColor[3] / 255.0f);

		glEnableClientState(GL_VERTEX_ARRAY);

		if (vboOk) {
			if (beginSize == 1 && endSize == 1) {
				glBindBuffer(GL_ARRAY_BUFFER, vboId[1]);
			} else {
				glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
			}
			glVertexPointer(2, GL_FLOAT, 2 * sizeof(float), 0);
		} else {
			if (beginSize == 1 && endSize == 1) {
				glVertexPointer(2, GL_FLOAT, 0, curveVertexData);
			} else {
				glVertexPointer(2, GL_FLOAT, 0, curveQuadVertexData);
			}
		}

		if (beginSize == 1 && endSize == 1) {
			glDrawArrays(GL_LINE_STRIP, 0, 40);
		} else {
			glDrawArrays(GL_QUAD_STRIP, 0, 80);
			glEnable(GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			if (vboOk) {
				glBindBuffer(GL_ARRAY_BUFFER, vboId[2]);
			}
			glVertexPointer(2, GL_FLOAT, 2 * sizeof(float), 0);
			glDrawArrays(GL_LINE_STRIP, 0, 40);
			if (vboOk) {
				glBindBuffer(GL_ARRAY_BUFFER, vboId[3]);
			}
			glVertexPointer(2, GL_FLOAT, 2 * sizeof(float), 0);
			glDrawArrays(GL_LINE_STRIP, 0, 40);
		}

		if (vboOk) {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		glDisableClientState(GL_VERTEX_ARRAY);

		GlShaderManager::getInstance()->desactivateShaderProgram();

	} else {
		vector<Coord> curvePoints;
		FastBezier::computeBezierPoints(controlPoints,curvePoints);
		drawCurve(curvePoints, beginColor, endColor, beginSize, endSize, curvePoints[0] - Coord(1,0,0), curvePoints[curvePoints.size() - 1] + Coord(1,0,0), texture);
	}

	if(texture != "") {
		GlTextureManager::getInst().desactivateTexture();
	}
}
//=====================================================
void GlBezierCurve::setTexture(const std::string &texture) {
	this->texture=texture;
}
//=====================================================
void GlBezierCurve::translate(const Coord& mouvement){
	boundingBox.first+=mouvement;
	boundingBox.second+=mouvement;
	if (controlPointsArray != NULL) {
		for (int i = 0 ; i < nbControlPoints ; ++i) {
			controlPointsArray[3 * i] += mouvement[0];
			controlPointsArray[3 * i + 1] += mouvement[1];
			controlPointsArray[3 * i + 2] += mouvement[2];
		}
	} else {
		for (unsigned int i = 0 ; i < controlPoints.size() ; ++i) {
			controlPoints[i] += mouvement;
		}
	}
}
//=====================================================
void GlBezierCurve::getXML(xmlNodePtr rootNode) {}
//============================================================
void GlBezierCurve::setWithXML(xmlNodePtr rootNode) {}

}
