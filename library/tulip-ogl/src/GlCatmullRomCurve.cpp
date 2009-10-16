/*
	  GlCatmullRomCurve.cpp

   Created on: 29 avr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#include <GL/glew.h>

#include <tulip/Curves.h>

#include <sstream>

#include <tulip/GlTools.h>
#include "tulip/GlCatmullRomCurve.h"
#include <tulip/GlTextureManager.h>


using namespace std;

static string genCatmullRomShaderSrc(unsigned int nbCurvePassPoints) {
	ostringstream oss;

	oss << "uniform vec3 curvePoints[" << nbCurvePassPoints << "];" << endl;
	oss << "uniform float startWidth;" << endl;
	oss << "uniform float endWidth;" << endl;
	oss << "uniform vec4 startColor;" << endl;
	oss << "uniform vec4 endColor;" << endl;
	oss << "uniform float step;" << endl;
	oss << "uniform bool closedCurve;" << endl;
	oss << "uniform bool fisheye;" << endl;
	oss << "uniform vec4 center;" << endl;
	oss << "uniform float radius;" << endl;
	oss << "uniform float height;" << endl;
	oss << "uniform int fisheyeType;" << endl;

	oss << "vec3 bezierControlPoints[4];" << endl;

	oss << "void computeBezierSegmentControlPoints(vec3 pBefore, vec3 pStart, vec3 pEnd, vec3 pAfter) {" << endl;
	oss << "	bezierControlPoints[0] = pStart;" << endl;
	oss << "	vec3 d0, d1;" << endl;
	oss << "	d0 = (pEnd - pBefore) * 0.5;" << endl;
	oss << "	bezierControlPoints[1] = pStart + d0 / 3.;" << endl;
	oss << "	d1 = (pAfter - pStart) * 0.5;" << endl;
	oss << "	bezierControlPoints[2] = pEnd - d1 / 3.;" << endl;
	oss << "	bezierControlPoints[3] = pEnd;" << endl;
	oss << "}" << endl;

	oss << "void" << endl;
	oss << "main (void) {" << endl;

	oss << "	float t = gl_Vertex.x;" << endl;
	oss << "	int i = int(floor(t));" << endl;
	oss << "	t = fract(t);" << endl;
	oss << "	float nbCurvePoints = closedCurve ? float(curvePoints.length): float(curvePoints.length - 1);" << endl;
	oss << "	float globalT = gl_Vertex.x / nbCurvePoints;" << endl;

	oss << "	bezierControlPoints[0] = vec3(0.0,0.0,0.0);" << endl;
	oss << "	bezierControlPoints[1] = vec3(0.0,0.0,0.0);" << endl;
	oss << "	bezierControlPoints[2] = vec3(0.0,0.0,0.0);" << endl;
	oss << "	bezierControlPoints[3] = vec3(0.0,0.0,0.0);" << endl;

	oss << "	if (i == 0 && !closedCurve) {" << endl;
	oss << "		computeBezierSegmentControlPoints(curvePoints[i], curvePoints[i], curvePoints[i+1], curvePoints[i+2]);" << endl;
	oss << "	} else if (i == 0 && closedCurve) {" << endl;
	oss << "		computeBezierSegmentControlPoints(curvePoints[curvePoints.length - 1], curvePoints[i], curvePoints[i+1], curvePoints[i+2]);" << endl;
	oss << "	} else if (i == curvePoints.length - 2 && !closedCurve) {" << endl;
	oss << "		computeBezierSegmentControlPoints(curvePoints[i-1], curvePoints[i], curvePoints[i+1], curvePoints[i+1]);" << endl;
	oss << "	} else if (i == curvePoints.length - 1 && !closedCurve) {" << endl;
	oss << "		t = 1.0;" << endl;
	oss << "		computeBezierSegmentControlPoints(curvePoints[i-2], curvePoints[i-1], curvePoints[i], curvePoints[i]);" << endl;
	oss << "	} else if (i == curvePoints.length - 2 && closedCurve) {" << endl;
	oss << "		computeBezierSegmentControlPoints(curvePoints[i-1], curvePoints[i], curvePoints[i+1], curvePoints[0]);" << endl;
	oss << "	} else if (i == curvePoints.length -1 && closedCurve) {" << endl;
	oss << "		computeBezierSegmentControlPoints(curvePoints[i-1], curvePoints[i], curvePoints[0], curvePoints[1]);" << endl;
	oss << "	} else if (i == curvePoints.length && closedCurve) {" << endl;
	oss << "		t = 1.0;" << endl;
	oss << "		computeBezierSegmentControlPoints(curvePoints[i-2], curvePoints[i-1], curvePoints[0], curvePoints[1]);" << endl;
	oss << "	} else if (i != curvePoints.length - 1) {" << endl;
	oss << "		computeBezierSegmentControlPoints(curvePoints[i-1], curvePoints[i], curvePoints[i+1], curvePoints[i+2]);" << endl;
	oss << "}" << endl;

	oss << "float t2 = t * t;" << endl;
	oss << "float t3 = t2 * t;" << endl;
	oss << "float s = 1.0 - t;" << endl;
	oss << "float s2 = s * s;" << endl;
	oss << "float s3 = s2	* s;" << endl;
	oss << "vec3 currentBezierVertex = bezierControlPoints[0] * s3 + bezierControlPoints[1] * 3.0 * t * s2 + bezierControlPoints[2] * 3.0 * t2 * s + bezierControlPoints[3] * t3;" << endl;
	oss << "if (gl_Vertex.y != 0.0) {" << endl;
	oss << "	t += step;" << endl;
	oss << "	t2 = t * t;" << endl;
	oss << "	t3 = t2 * t;" << endl;
	oss << "	s = 1.0 - t;" << endl;
	oss << "	s2 = s * s;" << endl;
	oss << "	s3 = s2 * s;" << endl;
	oss << "	vec3 nextBezierVertex = bezierControlPoints[0] * s3 + bezierControlPoints[1] * 3.0 * t * s2 + bezierControlPoints[2] * 3.0 * t2 * s + bezierControlPoints[3] * t3;" << endl;
	oss << "	vec3 tangent = normalize(nextBezierVertex - currentBezierVertex);" << endl;
	oss << "	vec3 normal = tangent;" << endl;
	oss << "	normal.x = - tangent.y;" << endl;
	oss << "	normal.y = tangent.x;" << endl;
	oss << "	float width = mix(startWidth, endWidth, globalT);" << endl;
	oss << "	currentBezierVertex += normal * (gl_Vertex.y * width);" << endl;
	oss << "}" << endl;
	oss <<"if (!fisheye) {" << endl;
	oss <<"		gl_Position = gl_ModelViewProjectionMatrix * vec4(currentBezierVertex, 1.0);" << endl;
	oss <<"} else {" << endl;
	oss <<"		gl_Position = gl_ModelViewMatrix * vec4(currentBezierVertex, 1.0);" << endl;
	oss <<"		float dist = distance(center, gl_Position);" << endl;
	oss <<"		if (fisheyeType == 1) {" << endl;
	oss <<"			if (dist < radius) {" << endl;
	oss <<"				float coeff = (height + 1.) * dist / (height * dist/ radius + 1.);" << endl;
	oss <<"				vec4 dir = normalize(gl_Position - center) * coeff;" << endl;
	oss <<"				gl_Position = gl_ProjectionMatrix * (center + dir);" << endl;
	oss <<"			} else {" << endl;
	oss <<"				gl_Position = gl_ProjectionMatrix * gl_Position;" << endl;
	oss <<"			}" << endl;
	oss <<"		} else if (fisheyeType == 2) {" << endl;
	oss <<"			float coeff = dist+dist*radius/(dist*dist+1.0+radius/height);" << endl;
	oss <<"			vec4 dir = normalize(gl_Position - center) * coeff;" << endl;
	oss <<"			gl_Position = gl_ProjectionMatrix * (center + dir);" << endl;
	oss <<"		} else {" << endl;
	oss <<"			if (dist < radius) {" << endl;
	oss <<"				gl_Position = gl_ProjectionMatrix * (center + height * (gl_Position - center));" << endl;
	oss <<"			} else {" << endl;
	oss <<"				gl_Position = gl_ProjectionMatrix * (center + (1. + radius * (height - 1.) / dist) * (gl_Position - center));" << endl;
	oss <<"			}" << endl;
	oss <<"		}" << endl;
	oss <<"}" << endl;
	oss << "gl_FrontColor = mix(startColor, endColor, globalT);" << endl;
	oss << "if (gl_Vertex.y > 0.0)" << endl;
	oss << "	gl_TexCoord[0].st = vec2(0.,0.);" << endl;
	oss << "else" << endl;
	oss << "	gl_TexCoord[0].st = vec2(1.,1.);" << endl;
	oss << "}" << endl;

	return oss.str();
}

static bool checkVboSupport() {
	return ((void*)(glGenBuffers)!=NULL) &&
	((void*)(glBindBuffer)!=NULL) &&
	((void*)(glBufferData)!=NULL);
}

namespace tlp {

std::map<std::pair<unsigned int, unsigned int>, GLfloat *> GlCatmullRomCurve::vertexBuffersDataMap = std::map<std::pair<unsigned int, unsigned int>, GLfloat *>();
std::map<std::pair<unsigned int, unsigned int>, vector<GLushort *> > GlCatmullRomCurve::vertexBuffersIndicesMap = std::map<std::pair<unsigned int, unsigned int>, vector<GLushort *> >();
std::map<std::pair<unsigned int, unsigned int>, GLuint *> GlCatmullRomCurve::vertexBuffersObjectMap = std::map<std::pair<unsigned int, unsigned int>, GLuint *>();

GlCatmullRomCurve::GlCatmullRomCurve(const vector<Coord> &curvePassPoints, const Color &beginColor, const Color &endColor,
		const float beginSize, const float endSize, const string &texture, const bool closedCurve, const unsigned int nbPointsPerBezierSegments) :
			beginColor(beginColor), endColor(endColor), beginSize(beginSize), endSize(endSize), texture(texture), closedCurve(closedCurve), nbPointsPerBezierSegments(nbPointsPerBezierSegments),
			curvePassPointsArray(NULL), nbCurvePassPoints(curvePassPoints.size()), catmullShaderProgram(NULL) {


	for (unsigned int i = 0 ; i < nbCurvePassPoints ; ++i) {
		boundingBox.check(curvePassPoints[i]);
	}

	vboOk = checkVboSupport();

	ostringstream oss;
	oss << nbCurvePassPoints;

	shaderProgramName = "catmull" + oss.str();

	if (GlShaderManager::getInstance()->isShaderProgramsSupported()) {
		if (!GlShaderManager::getInstance()->shaderProgramAlreadyCompiled(shaderProgramName)) {
			catmullShaderProgram = GlShaderManager::getInstance()->createVertexShaderFromString(shaderProgramName, genCatmullRomShaderSrc(nbCurvePassPoints));
		} else {
			catmullShaderProgram = GlShaderManager::getInstance()->getShaderProgram(shaderProgramName);
		}
	}

	if (catmullShaderProgram != NULL) {
		curvePassPointsArray = new GLfloat[nbCurvePassPoints * 3];
		for (unsigned int i = 0 ; i < nbCurvePassPoints ; ++i) {
			curvePassPointsArray[3 * i] = curvePassPoints[i][0];
			curvePassPointsArray[3 * i + 1] = curvePassPoints[i][1];
			curvePassPointsArray[3 * i + 2] = curvePassPoints[i][2];
		}
		if (vboOk) {
			genVertexBuffers();
		}
	} else {
		genBezierSegments(curvePassPoints);
	}
}

GlCatmullRomCurve::~GlCatmullRomCurve() {
	if (curvePassPointsArray != NULL) {
		delete [] curvePassPointsArray;
	} else {
		for (unsigned int i = 0 ; i < bezierSegments.size() ; ++i) {
			delete bezierSegments[i];
		}
	}
}

void GlCatmullRomCurve::genVertexBuffers() {
	float step = 1.0 / (nbPointsPerBezierSegments - 1.0);
	unsigned int nbBezierSegments = closedCurve ? nbCurvePassPoints : nbCurvePassPoints - 1;

	for (unsigned int i = 0 ; i < nbBezierSegments ; ++i) {

		pair<unsigned int, unsigned int> idx = make_pair(i, nbPointsPerBezierSegments);

		if (vertexBuffersDataMap.find(idx) == vertexBuffersDataMap.end()) {

			vertexBuffersDataMap[idx] = new GLfloat[nbPointsPerBezierSegments * 6];
			vertexBuffersIndicesMap[idx].resize(4);
			vertexBuffersIndicesMap[idx][0] = new GLushort[nbPointsPerBezierSegments * 2];
			vertexBuffersIndicesMap[idx][1] = new GLushort[nbPointsPerBezierSegments];
			vertexBuffersIndicesMap[idx][2] = new GLushort[nbPointsPerBezierSegments];
			vertexBuffersIndicesMap[idx][3] = new GLushort[nbPointsPerBezierSegments];

			for (unsigned int j = 0 ; j < nbPointsPerBezierSegments ; ++j) {
				float t = i + j * step;
				vertexBuffersDataMap[idx][6*j] = t;
				vertexBuffersDataMap[idx][6*j+1] = 1.0f;
				vertexBuffersDataMap[idx][6*j+2] = t;
				vertexBuffersDataMap[idx][6*j+3] = 0.0f;
				vertexBuffersDataMap[idx][6*j+4] = t;
				vertexBuffersDataMap[idx][6*j+5] = -1.0f;

				vertexBuffersIndicesMap[idx][0][2*j] = 3*j;
				vertexBuffersIndicesMap[idx][0][2*j+1] = 3*j+2;
				vertexBuffersIndicesMap[idx][1][j] = 3*j+1;
				vertexBuffersIndicesMap[idx][2][j] = 3*j;
				vertexBuffersIndicesMap[idx][3][j] = 3*j+2;
			}

			vertexBuffersObjectMap[idx] = new GLuint[5];
			glGenBuffers(5, vertexBuffersObjectMap[idx]);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffersObjectMap[idx][0]);
			glBufferData(GL_ARRAY_BUFFER, 6 * nbPointsPerBezierSegments * sizeof(GLfloat), vertexBuffersDataMap[idx], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffersObjectMap[idx][1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * nbPointsPerBezierSegments * sizeof(GLushort), vertexBuffersIndicesMap[idx][0], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffersObjectMap[idx][2]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbPointsPerBezierSegments * sizeof(GLushort), vertexBuffersIndicesMap[idx][1], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffersObjectMap[idx][3]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbPointsPerBezierSegments * sizeof(GLushort), vertexBuffersIndicesMap[idx][2], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffersObjectMap[idx][4]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbPointsPerBezierSegments * sizeof(GLushort), vertexBuffersIndicesMap[idx][3], GL_STATIC_DRAW);
		}

	}
}


void GlCatmullRomCurve::computeBezierSegmentControlPoints(const Coord &pBefore, const Coord &pStart, const Coord &pEnd, const Coord &pAfter, vector<Coord> &bezierSegmentControlPoints) {
	bezierSegmentControlPoints.push_back(pStart);
	Coord d0, d1;
	d0 = (pEnd - pBefore) / 2.;
	bezierSegmentControlPoints.push_back(pStart + d0 / 3.);
	d1 = (pAfter - pStart) / 2.;
	bezierSegmentControlPoints.push_back(pEnd - d1 / 3.);
	bezierSegmentControlPoints.push_back(pEnd);
}

void GlCatmullRomCurve::genBezierSegments(const vector<Coord> &curvePassPoints) {

	if (curvePassPoints.size() == 2) {
		GlBezierCurve *curve = new GlBezierCurve(curvePassPoints, beginColor, endColor, beginSize, endSize, nbPointsPerBezierSegments, texture);
		bezierSegments.push_back(curve);
		return;
	}

	vector<Color> colors = getColors(curvePassPoints, beginColor, endColor);
	vector<float> sizes = getSizes(curvePassPoints, beginSize, endSize);

	for (unsigned int i = 0 ; i < curvePassPoints.size() ; ++i) {
		vector<Coord> bezierSegmentControlPoints;
		if (i == 0 && !closedCurve) {
			computeBezierSegmentControlPoints(curvePassPoints[i], curvePassPoints[i], curvePassPoints[i+1], curvePassPoints[i+2], bezierSegmentControlPoints);
		} else if (i ==0 && closedCurve) {
			computeBezierSegmentControlPoints(curvePassPoints[curvePassPoints.size() - 1], curvePassPoints[i], curvePassPoints[i+1], curvePassPoints[i+2], bezierSegmentControlPoints);
		} else if (i == curvePassPoints.size() - 2 && !closedCurve) {
			computeBezierSegmentControlPoints(curvePassPoints[i-1], curvePassPoints[i], curvePassPoints[i+1], curvePassPoints[i+1], bezierSegmentControlPoints);
		} else if (i == curvePassPoints.size() - 2 && closedCurve) {
			computeBezierSegmentControlPoints(curvePassPoints[i-1], curvePassPoints[i], curvePassPoints[i+1], curvePassPoints[0], bezierSegmentControlPoints);
		} else if (i == curvePassPoints.size() -1 && closedCurve) {
			computeBezierSegmentControlPoints(curvePassPoints[i-1], curvePassPoints[i], curvePassPoints[0], curvePassPoints[1], bezierSegmentControlPoints);
		} else if (i != curvePassPoints.size() - 1) {
			computeBezierSegmentControlPoints(curvePassPoints[i-1], curvePassPoints[i], curvePassPoints[i+1], curvePassPoints[i+2], bezierSegmentControlPoints);
		}
		if (bezierSegmentControlPoints.size() > 0) {
			float beginSize, endSize;
			Color beginColor, endColor;
			if (i != curvePassPoints.size() - 1) {
				beginSize = sizes[i];
				endSize = sizes[i+1];
				beginColor = colors[i];
				endColor = colors[i+1];
			} else {
				beginSize = sizes[i];
				endSize = sizes[0];
				beginColor = colors[i];
				endColor = colors[0];
			}
			GlBezierCurve *curve = new GlBezierCurve(bezierSegmentControlPoints, beginColor, endColor, beginSize, endSize, nbPointsPerBezierSegments, texture);
			bezierSegments.push_back(curve);
		}
	}
}

void GlCatmullRomCurve::draw(float lod, Camera *camera) {

	if (catmullShaderProgram != NULL) {
		if (nbCurvePassPoints == 2) {
			vector<Coord> curvePassPoints;
			for (unsigned int i = 0 ; i < 2 ; ++i) {
				curvePassPoints.push_back(Coord(curvePassPointsArray[i*3], curvePassPointsArray[i*3+1], curvePassPointsArray[i*3+2]));
			}
			GlBezierCurve curve(curvePassPoints, beginColor, endColor, beginSize, endSize, nbPointsPerBezierSegments, texture);
			curve.draw(lod, camera);
			return;
		}

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
			fisheyeShader->getUniformIntVariableValue("fisheyeType", (int*)(&fisheyeType));
		}

		GlShaderManager::getInstance()->activateShaderProgram(catmullShaderProgram);

		float step = 1.0 / (nbPointsPerBezierSegments - 1.0);

		catmullShaderProgram->setUniformVec3FloatArray("curvePoints", nbCurvePassPoints, curvePassPointsArray);
		catmullShaderProgram->setUniformFloat("startWidth", beginSize);
		catmullShaderProgram->setUniformFloat("endWidth", endSize);
		catmullShaderProgram->setUniformColor("startColor", beginColor);
		catmullShaderProgram->setUniformColor("endColor", endColor);
		catmullShaderProgram->setUniformFloat("step", step);
		catmullShaderProgram->setUniformBool("closedCurve", closedCurve);
		catmullShaderProgram->setUniformBool("fisheye", fisheyeActivated);
		if (fisheyeActivated) {
			catmullShaderProgram->setUniformVec4Float("center", fisheyeCenter[0], fisheyeCenter[1], fisheyeCenter[2], fisheyeCenter[3]);
			catmullShaderProgram->setUniformFloat("radius", fisheyeRadius);
			catmullShaderProgram->setUniformFloat("height", fisheyeHeight);
			catmullShaderProgram->setUniformInt("fisheyeType", fisheyeType);
		}

		unsigned int nbBezierSegments = closedCurve ? nbCurvePassPoints : nbCurvePassPoints - 1;

		if (texture != "") {
			GlTextureManager::getInst().activateTexture(texture);
		}

		if (vboOk) {

			glEnableClientState(GL_VERTEX_ARRAY);

			for (unsigned int i = 0 ; i < nbBezierSegments ; ++i) {
				pair<unsigned int, unsigned int> idx = make_pair(i, nbPointsPerBezierSegments);
				glBindBuffer(GL_ARRAY_BUFFER, vertexBuffersObjectMap[idx][0]);
				glVertexPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), 0);

				GLuint *vbo = vertexBuffersObjectMap[idx];

				if (beginSize != 1 || endSize != 1) {
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
					glDrawElements(GL_TRIANGLE_STRIP, nbPointsPerBezierSegments * 2, GL_UNSIGNED_SHORT, 0);
				} else {
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
					glDrawElements(GL_LINE_STRIP, nbPointsPerBezierSegments, GL_UNSIGNED_SHORT, 0);
				}

				if ((beginSize != 1 || endSize != 1) && texture == "") {
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
					glDrawElements(GL_LINE_STRIP, nbPointsPerBezierSegments, GL_UNSIGNED_SHORT, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[4]);
					glDrawElements(GL_LINE_STRIP, nbPointsPerBezierSegments, GL_UNSIGNED_SHORT, 0);
				}
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glDisableClientState(GL_VERTEX_ARRAY);

		} else {

			if (beginSize != 1 || endSize != 1) {
				glBegin(GL_TRIANGLE_STRIP);
				for (unsigned int i = 0 ; i < nbBezierSegments ; ++i) {
					for (unsigned int j = 0 ; j < nbPointsPerBezierSegments ; ++j) {
						glVertex2f(i + j * step, 1.0);
						glTexCoord2f(0.0, 0.0);
						glVertex2f(i + j * step, -1.0);
						glTexCoord2f(1.0, 1.0);
					}
				}
				glEnd();

			} else {
				glBegin(GL_LINE_STRIP);
				for (unsigned int i = 0 ; i < nbBezierSegments ; ++i) {
					for (unsigned int j = 0 ; j < nbPointsPerBezierSegments ; ++j) {
						glVertex2f(i + j * step, 0.0);
					}
				}
				glEnd();
			}

			if ((beginSize != 1 || endSize != 1) && texture == "") {
				glBegin(GL_LINE_STRIP);
				for (unsigned int i = 0 ; i < nbBezierSegments ; ++i) {
					for (unsigned int j = 0 ; j < nbPointsPerBezierSegments ; ++j) {
						glVertex2f(i + j * step, 1.0);
					}
				}
				glEnd();
				glBegin(GL_LINE_STRIP);
				for (unsigned int i = 0 ; i < nbBezierSegments ; ++i) {
					for (unsigned int j = 0 ; j < nbPointsPerBezierSegments ; ++j) {
						glVertex2f(i + j * step, -1.0);
					}
				}
				glEnd();
			}
		}

		if (texture != "") {
			GlTextureManager::getInst().desactivateTexture();
		}

		GlShaderManager::getInstance()->desactivateShaderProgram();


		if (fisheyeActivated) {
			GlShaderManager::getInstance()->activateShaderProgram(GlShaderManager::getInstance()->getShaderProgram("fisheye"));
		}

	} else {
		for (unsigned int i = 0 ; i < bezierSegments.size() ; ++i) {
			bezierSegments[i]->draw(lod, camera);
		}
	}

}



}
