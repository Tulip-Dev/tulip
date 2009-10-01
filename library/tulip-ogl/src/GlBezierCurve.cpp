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
#include "tulip/GlCatmullRomCurve.h"

#include <sstream>

using namespace std;

static void drawCurve(const vector<tlp::Coord> &curvePoints, const tlp::Color &startColor, const tlp::Color &endColor,
		const float startSize, const float endSize, const tlp::Coord &startN, const tlp::Coord &endN,
		const string &texture) {

	unsigned int size = curvePoints.size();
	GLfloat *points = NULL;
	if (startSize != 1 && endSize != 1) {
		points = tlp::buildCurvePoints(curvePoints, tlp::getSizes(curvePoints, startSize, endSize), startN, endN ,size);
	}
	vector<tlp::Color> curveColors = tlp::getColors(curvePoints, startColor, endColor);
	if(texture != "") {
		tlp::GlTextureManager::getInst().activateTexture(texture);
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	if (startSize != 1 && endSize != 1) {
		glBegin(GL_TRIANGLE_STRIP);
		for (unsigned int i = 0; i < size; ++i) {
			tlp::setMaterial(curveColors[i]);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3fv(&points[i*3]);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3fv(&points[i*3 + size*3]);
		}
		glEnd();
	} else {
		glBegin(GL_LINE_STRIP);
		for (unsigned int i = 0; i < size; ++i) {
			tlp::setMaterial(curveColors[i]);
			glVertex3fv((float *) &curvePoints[i]);
		}
		glEnd();
	}

	// if no texture set, draw the curve outline for anti-aliasing
	if(texture == "" && points != NULL) {
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

	if (points != NULL) {
		delete [] points;
	}
}

static bool checkVboSupport() {
	return ((void*)(glGenBuffers)!=NULL) &&
	((void*)(glBindBuffer)!=NULL) &&
	((void*)(glBufferData)!=NULL);
}

static string genBezierVertexShaderCasteljauSrc(const unsigned int nbControlPoints) {
	ostringstream oss;
	oss << "#version 120" << endl;
	oss << "uniform vec3 controlPoints[" << nbControlPoints << "];" << endl;
	oss << "uniform float startWidth;" << endl;
	oss << "uniform float endWidth;" << endl;
	oss << "uniform vec4 startColor;" << endl;
	oss << "uniform vec4 endColor;" << endl;
	oss << "vec3 controlPointsCp[" << nbControlPoints << "];" << endl;
	oss << "uniform bool fisheye;" << endl;
	oss << "uniform vec4 center;" << endl;
	oss << "uniform float radius;" << endl;
	oss << "uniform float height;" << endl;
	oss << "uniform int fisheyeType;" << endl;
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
	oss <<"	    // the orientation of the quad vertex to generate is stored in the y coordinates of the incoming vertex" << endl;
	oss <<"	    vec3 finalVertex = bezierVertex + gl_Vertex.y * (normal * width);" << endl;
	oss <<"     if (!fisheye) {" << endl;
	oss <<"  		gl_Position = gl_ModelViewProjectionMatrix * vec4(finalVertex, 1.0);" << endl;
	oss <<"		} else {" << endl;
	oss <<"			gl_Position = gl_ModelViewMatrix * vec4(finalVertex, 1.0);" << endl;
	oss <<"			float dist = distance(center, gl_Position);" << endl;
	oss <<"			if (fisheyeType == 1) {" << endl;
	oss <<"				if (dist < radius) {" << endl;
	oss <<"					float coeff = (height + 1.0) * dist / (height * dist/ radius + 1.0);" << endl;
	oss <<"					vec4 dir = normalize(gl_Position - center) * coeff;" << endl;
	oss <<"					gl_Position = gl_ProjectionMatrix * (center + dir);" << endl;
	oss <<"				} else {" << endl;
	oss <<"					gl_Position = gl_ProjectionMatrix * gl_Position;" << endl;
	oss <<"				}" << endl;
	oss <<"			} else if (fisheyeType == 2) {" << endl;
	oss <<"				float coeff = dist+dist*radius/(dist*dist+1.0+radius/height);" << endl;
	oss <<"				vec4 dir = normalize(gl_Position - center) * coeff;" << endl;
	oss <<"				gl_Position = gl_ProjectionMatrix * (center + dir);" << endl;
	oss <<"			} else {" << endl;
	oss <<"				if (dist < radius) {" << endl;
	oss <<"					gl_Position = gl_ProjectionMatrix * (center + height * (gl_Position - center));" << endl;
	oss <<"				} else {" << endl;
	oss <<"					gl_Position = gl_ProjectionMatrix * (center + (1.0 + radius * (height - 1.0) / dist) * (gl_Position - center));" << endl;
	oss <<"				}" << endl;
	oss <<"			}" << endl;
	oss <<"		}" << endl;
	oss <<"  	gl_FrontColor = startColor + gl_Vertex.x * (endColor - startColor);" << endl;
	oss <<"     if (gl_Vertex.y > 0.0)" << endl;
	oss <<"     	gl_TexCoord[0].st = vec2(0.,0.);" << endl;
	oss <<"     else" << endl;
	oss << "    	gl_TexCoord[0].st = vec2(1.,1.);" << endl;
	oss <<"}" << endl;

	cout << oss.str() << endl;

	return oss.str();
}

vector<vector<unsigned long long> > buildPascalTriangle(unsigned int n) {

	vector<vector<unsigned long long> > pascalTriangle;
	pascalTriangle.resize(n);
	for (unsigned int i = 0; i < n; ++i) {
		pascalTriangle[i].resize(i+1);
	}

	pascalTriangle[0][0] = 1;
	for (unsigned int i = 1; i < n; i++) {
		pascalTriangle[i][0] = 1;
		pascalTriangle[i][i] = 1;
		for (unsigned int j = 1; j < i; ++j) {
			pascalTriangle[i][j] = pascalTriangle[i-1][j-1] + pascalTriangle[i-1][j];
		}
	}
	return pascalTriangle;
}

string genBezierEquation(unsigned int n) {
	vector<vector<unsigned long long> > pascalTriangle = buildPascalTriangle(n+1);

	ostringstream oss;
	for (unsigned int i = 0 ; i <= n ; ++i) {
		oss << "controlPoints[" << i << "] ";
		if (i != 0 && i != n) {
			oss << "* " << pascalTriangle[n][i] << ".0 ";
		}
		if (i >= 1) {
			if (i == 1) {
				oss << "* t ";
			} else {
				oss << "* t" << i << " ";
			}
		}
		if (i < n) {
			if (i == n-1) {
				oss << "* s ";
			} else {
				oss << "* s" << (n-i) << " ";
			}
		}
		//	oss << "* pow(t, " << i << ".0) * pow(s, " << (n-i) << ".0) ";
		if (i != n) {
			oss << "+ ";
		}
	}
	return oss.str();
}

string genBezierVertexShaderPolynomEvalSrc(unsigned int nbControlPoints) {

	ostringstream oss;
	oss << "uniform vec3 controlPoints[" << nbControlPoints << "];" << endl;
	oss << "uniform float startWidth;" << endl;
	oss << "uniform float endWidth;" << endl;
	oss << "uniform vec4 startColor;" << endl;
	oss << "uniform vec4 endColor;" << endl;
	oss << "uniform float step;" << endl;
	oss << "uniform bool fisheye;" << endl;
	oss << "uniform vec4 center;" << endl;
	oss << "uniform float radius;" << endl;
	oss << "uniform float height;" << endl;
	oss << "uniform int fisheyeType;" << endl;
	oss << "void" << endl;
	oss << "main (void) {" << endl;
	oss << "float t = gl_Vertex.x;" << endl;
	oss << "float t2 = t * t;" << endl;
	for (unsigned int i = 3 ; i <= nbControlPoints - 1 ; ++i) {
		oss << "float t" << i << " = t * t" << (i-1) << ";" << endl;
	}
	oss << "float s = (1.0 - t);" << endl;
	oss << "float s2 = s * s;" << endl;
	for (unsigned int i = 3 ; i <= nbControlPoints - 1 ; ++i) {
		oss << "float s" << i << " = s * s" << (i-1) << ";" << endl;
	}
	oss << "vec3 currentBezierVertex = " << genBezierEquation(nbControlPoints-1) << ";" << endl;
	oss << "if (gl_Vertex.y != 0.0) {" << endl;
	oss << "t += step;" << endl;
	oss << "t2 = t * t;" << endl;
	for (unsigned int i = 3 ; i <= nbControlPoints - 1 ; ++i) {
		oss << "t" << i << " = t * t" << (i-1) << ";" << endl;
	}
	oss << "s = (1.0 - t);" << endl;
	oss << "s2 = s * s;" << endl;
	for (unsigned int i = 3 ; i <= nbControlPoints - 1 ; ++i) {
		oss << "s" << i << " = s * s" << (i-1) << ";" << endl;
	}
	oss << "vec3 nextBezierVertex = " + genBezierEquation(nbControlPoints-1) << ";" << endl;
	oss <<  "vec3 tangent = normalize(nextBezierVertex - currentBezierVertex);" << endl;
	oss <<  "vec3 normal = tangent;" << endl;
	oss << "normal.x = - tangent.y;" << endl;
	oss << "normal.y = tangent.x;" << endl;
	oss << "float width = mix(startWidth, endWidth, t);" << endl;
	oss << "currentBezierVertex += normal * (gl_Vertex.y * width);" << endl;
	oss << "}" << endl;
	oss <<"if (!fisheye) {" << endl;
	oss <<"	gl_Position = gl_ModelViewProjectionMatrix * vec4(currentBezierVertex, 1.0);" << endl;
	oss <<"} else {" << endl;
	oss <<"	gl_Position = gl_ModelViewMatrix * vec4(currentBezierVertex, 1.0);" << endl;
	oss <<"	float dist = distance(center, gl_Position);" << endl;
	oss <<"	if (fisheyeType == 1) {" << endl;
	oss <<"		if (dist < radius) {" << endl;
	oss <<"			float coeff = (height + 1.) * dist / (height * dist/ radius + 1.);" << endl;
	oss <<"			vec4 dir = normalize(gl_Position - center) * coeff;" << endl;
	oss <<"			gl_Position = gl_ProjectionMatrix * (center + dir);" << endl;
	oss <<"		} else {" << endl;
	oss <<"			gl_Position = gl_ProjectionMatrix * gl_Position;" << endl;
	oss <<"		}" << endl;
	oss <<"	} else if (fisheyeType == 2) {" << endl;
	oss <<"		float coeff = dist+dist*radius/(dist*dist+1.0+radius/height);" << endl;
	oss <<"		vec4 dir = normalize(gl_Position - center) * coeff;" << endl;
	oss <<"		gl_Position = gl_ProjectionMatrix * (center + dir);" << endl;
	oss <<"	} else {" << endl;
	oss <<"		if (dist < radius) {" << endl;
	oss <<"			gl_Position = gl_ProjectionMatrix * (center + height * (gl_Position - center));" << endl;
	oss <<"		} else {" << endl;
	oss <<"			gl_Position = gl_ProjectionMatrix * (center + (1. + radius * (height - 1.) / dist) * (gl_Position - center));" << endl;
	oss <<"		}" << endl;
	oss <<"	}" << endl;
	oss <<"}" << endl;
	oss << "gl_FrontColor = mix(startColor, endColor, t);" << endl;
	oss << "if (gl_Vertex.y > 0.0)" << endl;
	oss << "	gl_TexCoord[0].st = vec2(0.,0.);" << endl;
	oss << "else" << endl;
	oss << "	gl_TexCoord[0].st = vec2(1.,1.);" << endl;
	oss << "}" << endl;
	return oss.str();
}

static const unsigned int CONTROL_POINTS_LIMIT = 70;

namespace tlp {

map<unsigned int, GLfloat *> GlBezierCurve::bezierVertexBuffersData = map<unsigned int, GLfloat * >();
map<unsigned int, vector<GLushort *> > GlBezierCurve::bezierVertexBuffersIndices = map<unsigned int, vector<GLushort * > >();
map<unsigned int, GLuint *> GlBezierCurve::bezierVertexBuffersObject = map<unsigned int, GLuint*>();
map<unsigned int, std::vector<GLfloat *> > GlBezierCurve::bezierVertexBuffersDataNoVbo = map<unsigned int, std::vector<GLfloat *> >();


//=============================================
GlBezierCurve::GlBezierCurve(const vector<Coord> &controlPoints,
		const Color &beginColor,
		const Color &endColor,
		const float &beginSize,
		const float &endSize,
		const unsigned int nbCurvePoints,
		const string &texture):
			bezierVertexShader(NULL),
			controlPoints(controlPoints),
			beginColor(beginColor),
			endColor(endColor),
			beginSize(beginSize),
			endSize(endSize),
			texture(texture),
			nbCurvePoints(nbCurvePoints)
			{

	vboOk = checkVboSupport();

	nbControlPoints = controlPoints.size();
	for (int i = 0 ; i < nbControlPoints ; ++i) {
		boundingBox.check(controlPoints[i]);
	}

	ostringstream oss;
	oss << nbControlPoints;

	shaderProgramName = "bezier" + oss.str();

	controlPointsArray = new GLfloat[nbControlPoints * 3];
	for (int i = 0 ; i < nbControlPoints ; ++i) {
		controlPointsArray[3 * i] = controlPoints[i][0];
		controlPointsArray[3 * i + 1] = controlPoints[i][1];
		controlPointsArray[3 * i + 2] = controlPoints[i][2];
	}


	if ((vboOk && bezierVertexBuffersData.find(nbCurvePoints) == bezierVertexBuffersData.end()) ||
	    (!vboOk && bezierVertexBuffersDataNoVbo.find(nbCurvePoints) == bezierVertexBuffersDataNoVbo.end())) {
		buildBezierVertexBuffers(nbCurvePoints);

		if (vboOk) {
			glGenBuffers(5, bezierVertexBuffersObject[nbCurvePoints]);
			glBindBuffer(GL_ARRAY_BUFFER, bezierVertexBuffersObject[nbCurvePoints][0]);
			glBufferData(GL_ARRAY_BUFFER, 6 * nbCurvePoints * sizeof(GLfloat), bezierVertexBuffersData[nbCurvePoints], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierVertexBuffersObject[nbCurvePoints][1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * nbCurvePoints * sizeof(GLushort), bezierVertexBuffersIndices[nbCurvePoints][0], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierVertexBuffersObject[nbCurvePoints][2]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbCurvePoints * sizeof(GLushort), bezierVertexBuffersIndices[nbCurvePoints][1], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierVertexBuffersObject[nbCurvePoints][3]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbCurvePoints * sizeof(GLushort), bezierVertexBuffersIndices[nbCurvePoints][2], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierVertexBuffersObject[nbCurvePoints][4]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbCurvePoints * sizeof(GLushort), bezierVertexBuffersIndices[nbCurvePoints][3], GL_STATIC_DRAW);
		}
	}

	if (GlShaderManager::getInstance()->isShaderProgramsSupported() && controlPoints.size() <= CONTROL_POINTS_LIMIT) {
		if (!GlShaderManager::getInstance()->shaderProgramAlreadyCompiled(shaderProgramName)) {
			bezierVertexShader = GlShaderManager::getInstance()->createVertexShaderFromString(shaderProgramName, genBezierVertexShaderPolynomEvalSrc(nbControlPoints));
		} else {
			bezierVertexShader = GlShaderManager::getInstance()->getShaderProgram(shaderProgramName);
		}
	}
}
//=====================================================
GlBezierCurve::~GlBezierCurve() {
	if (controlPointsArray != NULL) {
		delete [] controlPointsArray;
	}
}
void GlBezierCurve::buildBezierVertexBuffers(const unsigned int nbCurvePoints) {

	if (vboOk) {
		bezierVertexBuffersObject[nbCurvePoints] = new GLuint[5];
		bezierVertexBuffersData[nbCurvePoints] = new GLfloat[nbCurvePoints * 6];
		bezierVertexBuffersIndices[nbCurvePoints].resize(4);
		bezierVertexBuffersIndices[nbCurvePoints][0] = new GLushort[nbCurvePoints * 2];
		bezierVertexBuffersIndices[nbCurvePoints][1] = new GLushort[nbCurvePoints];
		bezierVertexBuffersIndices[nbCurvePoints][2] = new GLushort[nbCurvePoints];
		bezierVertexBuffersIndices[nbCurvePoints][3] = new GLushort[nbCurvePoints];
	} else {
		bezierVertexBuffersDataNoVbo[nbCurvePoints].resize(4);
		bezierVertexBuffersDataNoVbo[nbCurvePoints][0] = new GLfloat[nbCurvePoints * 4];
		bezierVertexBuffersDataNoVbo[nbCurvePoints][1] = new GLfloat[nbCurvePoints * 2];
		bezierVertexBuffersDataNoVbo[nbCurvePoints][2] = new GLfloat[nbCurvePoints * 2];
		bezierVertexBuffersDataNoVbo[nbCurvePoints][3] = new GLfloat[nbCurvePoints * 2];
	}

	for (unsigned int i = 0 ; i < nbCurvePoints ; ++i) {
		float t = (float) i / (float) (nbCurvePoints - 1);
		if (vboOk) {
			bezierVertexBuffersData[nbCurvePoints][6*i] = t;
			bezierVertexBuffersData[nbCurvePoints][6*i+1] = 1.0f;
			bezierVertexBuffersData[nbCurvePoints][6*i+2] = t;
			bezierVertexBuffersData[nbCurvePoints][6*i+3] = 0.0f;
			bezierVertexBuffersData[nbCurvePoints][6*i+4] = t;
			bezierVertexBuffersData[nbCurvePoints][6*i+5] = -1.0f;

			bezierVertexBuffersIndices[nbCurvePoints][0][2*i] = 3*i;
			bezierVertexBuffersIndices[nbCurvePoints][0][2*i+1] = 3*i+2;
			bezierVertexBuffersIndices[nbCurvePoints][1][i] = 3*i+1;
			bezierVertexBuffersIndices[nbCurvePoints][2][i] = 3*i;
			bezierVertexBuffersIndices[nbCurvePoints][3][i] = 3*i+2;
		} else {
			bezierVertexBuffersDataNoVbo[nbCurvePoints][0][4*i] = t;
			bezierVertexBuffersDataNoVbo[nbCurvePoints][0][4*i+1] = 1.0f;
			bezierVertexBuffersDataNoVbo[nbCurvePoints][0][4*i+2] = t;
			bezierVertexBuffersDataNoVbo[nbCurvePoints][0][4*i+3] = -1.0f;
			bezierVertexBuffersDataNoVbo[nbCurvePoints][1][2*i] = t;
			bezierVertexBuffersDataNoVbo[nbCurvePoints][1][2*i+1] = 0.0f;
			bezierVertexBuffersDataNoVbo[nbCurvePoints][2][2*i] = t;
			bezierVertexBuffersDataNoVbo[nbCurvePoints][2][2*i+1] = 1.0f;
			bezierVertexBuffersDataNoVbo[nbCurvePoints][3][2*i] = t;
			bezierVertexBuffersDataNoVbo[nbCurvePoints][3][2*i+1] = -1.0f;
		}
	}

}
//=====================================================
void GlBezierCurve::draw(float lod, Camera *camera) {

	if (texture != "") {
		GlTextureManager::getInst().activateTexture(texture);
	}

	GLuint *vbo = bezierVertexBuffersObject[nbCurvePoints];

	if (controlPoints.size() <= CONTROL_POINTS_LIMIT) {

		if (bezierVertexShader != NULL) {
			bool fisheyeActivated = GlShaderManager::getInstance()->getCurrentShaderProgramName() == "fisheye";
			GLfloat fisheyeCenter[4];
			GLfloat fisheyeRadius;
			GLfloat fisheyeHeight;
			GLint   fisheyeType;
			if (fisheyeActivated) {
				GlShaderProgram *fisheyeShader = GlShaderManager::getInstance()->getShaderProgram("fisheye");
				fisheyeShader->getUniformFloatVariableValue("center", fisheyeCenter);
				fisheyeShader->getUniformFloatVariableValue("radius", &fisheyeRadius);
				fisheyeShader->getUniformFloatVariableValue("height", &fisheyeHeight);
				fisheyeShader->getUniformIntVariableValue("fisheyeType", &fisheyeType);
			}

			GlShaderManager::getInstance()->activateShaderProgram(bezierVertexShader);

			bezierVertexShader->setUniformVec3FloatArray("controlPoints", nbControlPoints, controlPointsArray);
			bezierVertexShader->setUniformFloat("startWidth", beginSize);
			bezierVertexShader->setUniformFloat("endWidth", endSize);
			bezierVertexShader->setUniformColor("startColor", beginColor);
			bezierVertexShader->setUniformColor("endColor", endColor);
			bezierVertexShader->setUniformFloat("step", 1.0 / (nbCurvePoints - 1.0));

			bezierVertexShader->setUniformBool("fisheye", fisheyeActivated);
			if (fisheyeActivated) {
				bezierVertexShader->setUniformVec4Float("center", fisheyeCenter[0], fisheyeCenter[1], fisheyeCenter[2], fisheyeCenter[3]);
				bezierVertexShader->setUniformFloat("radius", fisheyeRadius);
				bezierVertexShader->setUniformFloat("height", fisheyeHeight);
				bezierVertexShader->setUniformInt("fisheyeType", fisheyeType);
			}


			glEnableClientState(GL_VERTEX_ARRAY);

			if (vboOk) {
				glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
				glVertexPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), 0);
			}

			if (beginSize == 1 && endSize == 1) {
				if (vboOk) {
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
					glDrawElements(GL_LINE_STRIP, nbCurvePoints, GL_UNSIGNED_SHORT, 0);
				} else {
					glVertexPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), bezierVertexBuffersDataNoVbo[nbCurvePoints][1]);
					glDrawArrays(GL_LINE_STRIP, 0, nbCurvePoints);
				}

			} else {
				if (vboOk) {
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
					glDrawElements(GL_TRIANGLE_STRIP, nbCurvePoints * 2, GL_UNSIGNED_SHORT, 0);
				} else {
					glVertexPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), bezierVertexBuffersDataNoVbo[nbCurvePoints][0]);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, nbCurvePoints * 2);
				}

				if (texture == "") {
					glEnable(GL_BLEND);
					glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					if (vboOk) {
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
						glDrawElements(GL_LINE_STRIP, nbCurvePoints, GL_UNSIGNED_SHORT, 0);
					} else {
						glVertexPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), bezierVertexBuffersDataNoVbo[nbCurvePoints][1]);
						glDrawArrays(GL_LINE_STRIP, 0, nbCurvePoints);
					}

					if (vboOk) {
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[4]);
						glDrawElements(GL_LINE_STRIP, nbCurvePoints, GL_UNSIGNED_SHORT, 0);
					} else {
						glVertexPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), bezierVertexBuffersDataNoVbo[nbCurvePoints][1]);
						glDrawArrays(GL_LINE_STRIP, 0, nbCurvePoints);
					}
				}
			}

			if (vboOk) {
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}

			glDisableClientState(GL_VERTEX_ARRAY);

			GlShaderManager::getInstance()->desactivateShaderProgram();

			if (fisheyeActivated) {
				GlShaderManager::getInstance()->activateShaderProgram(GlShaderManager::getInstance()->getShaderProgram("fisheye"));
			}
		} else {
			vector<Coord> curvePoints;
			FastBezier::computeBezierPoints(controlPoints,curvePoints,nbCurvePoints);
			drawCurve(curvePoints, beginColor, endColor, beginSize, endSize, curvePoints[0] - Coord(1,0,0), curvePoints[curvePoints.size() - 1] + Coord(1,0,0), texture);
		}
	} else {
		vector<Coord> curvePoints;
		vector<Coord> splinePoints;
		FastBezier::computeBezierPoints(controlPoints,curvePoints, 20);
		GlCatmullRomCurve curve(curvePoints, beginColor, endColor, beginSize, endSize, "", false, 20);
		curve.draw(0,0);
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
