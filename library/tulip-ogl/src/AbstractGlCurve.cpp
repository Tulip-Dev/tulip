/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include <GL/glew.h>

#include "tulip/AbstractGlCurve.h"
#include "tulip/GlTextureManager.h"
#include "tulip/Curves.h"
#include "tulip/TlpTools.h"

using namespace std;

static bool checkVboSupport() {
	static bool vboOk = glewIsSupported("GL_ARB_vertex_buffer_object");
	return vboOk;
}

namespace tlp {

static string fisheyeDistortionVertexShaderSrc =
		"#version 120\n"

		"uniform vec4 center;"
		"uniform float radius;"
		"uniform float height;"
		"uniform int fisheyeType;"

		"vec4 fisheyeDistortion(vec3 glScenePoint) {"
		"		vec4 position = gl_ModelViewMatrix * vec4(glScenePoint, 1.0);"
		"		float dist = distance(center, position);"
		"		if (fisheyeType == 1) {"
		"			if (dist < radius) {"
		"				float coeff = (height + 1.) * dist / (height * dist/ radius + 1.);"
		"				vec4 dir = normalize(position - center) * coeff;"
		"				return gl_ProjectionMatrix * (center + dir);"
		"			} else {"
		"				return gl_ProjectionMatrix * position;"
		"			}"
		"		} else if (fisheyeType == 2) {"
		"			float coeff = dist+dist*radius/(dist*dist+1.0+radius/height);"
		"			vec4 dir = normalize(position - center) * coeff;"
		"			return gl_ProjectionMatrix * (center + dir);"
		"		} else {"
		"			if (dist < radius) {"
		"				return gl_ProjectionMatrix * (center + height * (position - center));"
		"			} else {"
		"				return gl_ProjectionMatrix * (center + (1. + radius * (height - 1.) / dist) * (position - center));"
		"			}"
		"		}"
		"}";

static string genCommonUniformVariables(const unsigned int maxNbControlPoints) {
	ostringstream oss;

	oss << "#version 120" << endl;
	oss << "uniform vec3 controlPoints[" << maxNbControlPoints << "];" << endl;
	oss << "uniform int nbControlPoints;" << endl;

	return oss.str();
}

static string curveVertexShaderNormalMainSrc =
		"#version 120\n"
		"uniform int nbCurvePoints;"
		"uniform float startSize;"
		"uniform float endSize;"
		"uniform vec4 startColor;"
		"uniform vec4 endColor;"
		"uniform float step;"
		"uniform float texCoordFactor;"
		"uniform bool fisheye;"

		"vec4 fisheyeDistortion(vec3 glScenePoint);"

		"vec3 computeCurvePoint(float t);"

		"void main () {"
		"	float t = gl_Vertex.x;"
		"	float size = mix(startSize, endSize, t);"
		"	vec3 curvePoint = computeCurvePoint(t);"
		"	if (gl_Vertex.y != 0.0) {"
		"		vec3 tangent = vec3(0.0);"
		"		if (t != 1.0) {"
		"			vec3 nextCurvePoint = computeCurvePoint(t + step);"
		"			tangent = normalize(nextCurvePoint - curvePoint);"
		"		} else {"
		"			vec3 prevCurvePoint = computeCurvePoint(t - step);"
		"			tangent = normalize(curvePoint - prevCurvePoint);"
		"		}"
		"		vec3 normal = tangent;"
		"		normal.x = -tangent.y;"
		"		normal.y = tangent.x;"
		"		curvePoint += normal * (gl_Vertex.y * size);"
		"	}"
		"	if (!fisheye) {"
		"		gl_Position = gl_ModelViewProjectionMatrix * vec4(curvePoint, 1.0);"
		"	} else {"
		"		gl_Position = fisheyeDistortion(curvePoint);"
		"	}"
		"	gl_FrontColor =  mix(startColor, endColor, t);"
		"	if (gl_Vertex.y > 0.0) {"
		"		gl_TexCoord[0].st = vec2(t * float(nbCurvePoints - 1) * texCoordFactor, 1.0);"
		"	} else {"
		"		gl_TexCoord[0].st = vec2(t * float(nbCurvePoints - 1) * texCoordFactor, 0.0);"
		"	}"
		"}"
		;

static string curveVertexShaderBillboardMainSrc =
		"#version 120\n"
		"uniform int nbCurvePoints;"
		"uniform float startSize;"
		"uniform float endSize;"
		"uniform vec4 startColor;"
		"uniform vec4 endColor;"
		"uniform float step;"
		"uniform float texCoordFactor;"
		"uniform vec3 lookDir;"
		"uniform bool fisheye;"

		"const float PI = 3.141592653589793238462643;"

		"vec4 fisheyeDistortion(vec3 glScenePoint);"

		"vec3 computeCurvePoint(float t);"

		"void main () {"
		"	float t = gl_Vertex.x;"
		"	float size = mix(startSize, endSize, t);"
		"	vec3 curvePoint = computeCurvePoint(t);"
		"	vec3 nextCurvePoint = computeCurvePoint(clamp(t+step, 0.0, 1.0));"
		"	vec3 prevCurvePoint = computeCurvePoint(clamp(t-step, 0.0, 1.0));"
		"	vec3 dir = vec3(0.0);"
		"	if (t == 0.0) {"
		"		vec3 xu = normalize(curvePoint - nextCurvePoint);"
		"		dir = normalize(cross(xu, lookDir));"
		"	} else if (t == 1.0) {"
		"		vec3 xu = normalize(prevCurvePoint - curvePoint);"
		"		dir = normalize(cross(xu, lookDir));"
		"	} else {"
		"		vec3 u = normalize(prevCurvePoint - curvePoint);"
		"		vec3 v = normalize(nextCurvePoint - curvePoint);"
		"		vec3 xu = cross(u, lookDir);"
		"		vec3 xv = cross(v,-lookDir);"
		"		dir = normalize(xu+xv);"
		" 		float angle = PI - acos(dot(u,v));"
		//	Check to see if angle == NaN (GLSL Spec NaN != NaN) because the isnan builtin function is available since version 130 of GLSL
		"		if (angle != angle) angle = 0;"
		"		size = size/cos(angle/2.0);"
		"	}"
		"	curvePoint += dir * (gl_Vertex.y * size);"
		"	if (!fisheye) {"
		"		gl_Position = gl_ModelViewProjectionMatrix * vec4(curvePoint, 1.0);"
		"	} else {"
		"		gl_Position = fisheyeDistortion(curvePoint);"
		"	}"
		"	gl_FrontColor =  mix(startColor, endColor, t);"
		"	if (gl_Vertex.y > 0.0) {"
		"		gl_TexCoord[0].st = vec2(t * float(nbCurvePoints - 1) * texCoordFactor, 0.0);"
		"		gl_TexCoord[1].st = vec2(t * float(nbCurvePoints - 1) * texCoordFactor, 0.0);"
		"	} else {"
		"		gl_TexCoord[0].st = vec2(t * float(nbCurvePoints - 1) * texCoordFactor, 1.0);"
		"		gl_TexCoord[1].st = vec2(t * float(nbCurvePoints - 1) * texCoordFactor, 1.0);"
		"	}"
		"}"
		;

map<unsigned int, GLfloat *> AbstractGlCurve::curveVertexBuffersData;
map<unsigned int, vector<GLushort *> > AbstractGlCurve::curveVertexBuffersIndices;
map<unsigned int, GLuint* > AbstractGlCurve::curveVertexBuffersObject;
map<string, GlShaderProgram *> AbstractGlCurve::curvesShadersMap;
map<string, GlShaderProgram *> AbstractGlCurve::curvesBillboardShadersMap;
GLint AbstractGlCurve::MAX_SHADER_CONTROL_POINTS(0);
GlShader *AbstractGlCurve::fisheyeDistortionVertexShader(NULL);
GlShader *AbstractGlCurve::curveVertexShaderNormalMain(NULL);
GlShader *AbstractGlCurve::curveVertexShaderBillboardMain(NULL);

AbstractGlCurve::AbstractGlCurve(const string &shaderProgramName, const string &curveSpecificShaderCode) :
				curveShaderProgramNormal(NULL), curveShaderProgramBillboard(NULL), curveShaderProgram(NULL),
				outlined(false), outlineColor(Color(0,0,0)), texture(""), texCoordFactor(1), billboardCurve(false), lookDir(Coord(0,0,1)) {
	initShader(shaderProgramName, curveSpecificShaderCode);
}

AbstractGlCurve::AbstractGlCurve(const string &shaderProgramName, const string &curveSpecificShaderCode, const vector<Coord> &controlPoints,
		const Color &startColor, const Color &endColor, const float startSize, const float endSize, const unsigned int nbCurvePoints) :
		shaderProgramName(shaderProgramName), curveShaderProgramNormal(NULL), curveShaderProgramBillboard(NULL), curveShaderProgram(NULL),
		controlPoints(controlPoints), startColor(startColor), endColor(endColor), startSize(startSize), endSize(endSize), nbCurvePoints(nbCurvePoints),
		outlined(false), outlineColor(Color(0,0,0)), texture(""), texCoordFactor(1), billboardCurve(false), lookDir(Coord(0,0,1)) {

	initShader(shaderProgramName, curveSpecificShaderCode);

	for (size_t i = 0 ; i < controlPoints.size() ; ++i) {
		boundingBox.expand(controlPoints[i]);
	}
}

AbstractGlCurve::~AbstractGlCurve() {}

void AbstractGlCurve::buildCurveVertexBuffers(const unsigned int nbCurvePoints, bool vboOk) {
	curveVertexBuffersObject[nbCurvePoints] = new GLuint[5];
	curveVertexBuffersData[nbCurvePoints] = new GLfloat[nbCurvePoints * 6];
	curveVertexBuffersIndices[nbCurvePoints].resize(4);
	curveVertexBuffersIndices[nbCurvePoints][0] = new GLushort[nbCurvePoints * 2];
	curveVertexBuffersIndices[nbCurvePoints][1] = new GLushort[nbCurvePoints];
	curveVertexBuffersIndices[nbCurvePoints][2] = new GLushort[nbCurvePoints];
	curveVertexBuffersIndices[nbCurvePoints][3] = new GLushort[nbCurvePoints];

	for (unsigned int i = 0 ; i < nbCurvePoints ; ++i) {
		float t = (float) i / (float) (nbCurvePoints - 1);
		curveVertexBuffersData[nbCurvePoints][6*i] = t;
		curveVertexBuffersData[nbCurvePoints][6*i+1] = 1.0f;
		curveVertexBuffersData[nbCurvePoints][6*i+2] = t;
		curveVertexBuffersData[nbCurvePoints][6*i+3] = 0.0f;
		curveVertexBuffersData[nbCurvePoints][6*i+4] = t;
		curveVertexBuffersData[nbCurvePoints][6*i+5] = -1.0f;

		curveVertexBuffersIndices[nbCurvePoints][0][2*i] = 3*i;
		curveVertexBuffersIndices[nbCurvePoints][0][2*i+1] = 3*i+2;
		curveVertexBuffersIndices[nbCurvePoints][1][i] = 3*i+1;
		curveVertexBuffersIndices[nbCurvePoints][2][i] = 3*i;
		curveVertexBuffersIndices[nbCurvePoints][3][i] = 3*i+2;
	}

	if (vboOk) {
		glGenBuffers(5, curveVertexBuffersObject[nbCurvePoints]);
		glBindBuffer(GL_ARRAY_BUFFER, curveVertexBuffersObject[nbCurvePoints][0]);
		glBufferData(GL_ARRAY_BUFFER, 6 * nbCurvePoints * sizeof(GLfloat), curveVertexBuffersData[nbCurvePoints], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curveVertexBuffersObject[nbCurvePoints][1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * nbCurvePoints * sizeof(GLushort), curveVertexBuffersIndices[nbCurvePoints][0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curveVertexBuffersObject[nbCurvePoints][2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbCurvePoints * sizeof(GLushort), curveVertexBuffersIndices[nbCurvePoints][1], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curveVertexBuffersObject[nbCurvePoints][3]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbCurvePoints * sizeof(GLushort), curveVertexBuffersIndices[nbCurvePoints][2], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curveVertexBuffersObject[nbCurvePoints][4]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbCurvePoints * sizeof(GLushort), curveVertexBuffersIndices[nbCurvePoints][3], GL_STATIC_DRAW);
	}
}

void AbstractGlCurve::draw(float, Camera *) {
	drawCurve(&controlPoints, startColor, endColor, startSize, endSize, nbCurvePoints);
}

void AbstractGlCurve::initShader(const std::string &shaderProgramName, const std::string &curveSpecificShaderCode) {
	// restrict shaders compilation on compatible hardware, crashs can happened when using the Mesa software rasterizer
	static string glVendor(((const char*)glGetString(GL_VENDOR)));
	static bool glVendorOk = (glVendor.find("NVIDIA")!=string::npos) || (glVendor.find("ATI")!=string::npos);

	if (glVendorOk && GlShaderProgram::shaderProgramsSupported()) {
		if (MAX_SHADER_CONTROL_POINTS == 0) {
			// has been determined experimentally by testing the implementation
			// on several graphic cards with different video memory size and graphics drivers
			glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB, &MAX_SHADER_CONTROL_POINTS);
			MAX_SHADER_CONTROL_POINTS /= 4;
			MAX_SHADER_CONTROL_POINTS -= 44;
		}
		if (curveVertexShaderNormalMain == NULL) {
			curveVertexShaderNormalMain = new GlShader(Vertex);
			curveVertexShaderNormalMain->compileFromSourceCode(curveVertexShaderNormalMainSrc);
		}
		if (curveVertexShaderBillboardMain == NULL) {
			curveVertexShaderBillboardMain = new GlShader(Vertex);
			curveVertexShaderBillboardMain->compileFromSourceCode(curveVertexShaderBillboardMainSrc);
		}
		if (fisheyeDistortionVertexShader == NULL) {
			fisheyeDistortionVertexShader = new GlShader(Vertex);
			fisheyeDistortionVertexShader->compileFromSourceCode(fisheyeDistortionVertexShaderSrc);
		}
		if (curvesShadersMap.find(shaderProgramName) == curvesShadersMap.end()) {
			curvesShadersMap[shaderProgramName] = new GlShaderProgram(shaderProgramName);
			curvesShadersMap[shaderProgramName]->addShaderFromSourceCode(Vertex, genCommonUniformVariables(MAX_SHADER_CONTROL_POINTS) + curveSpecificShaderCode);
			curvesShadersMap[shaderProgramName]->addShader(curveVertexShaderNormalMain);
			curvesShadersMap[shaderProgramName]->addShader(fisheyeDistortionVertexShader);
			curvesShadersMap[shaderProgramName]->link();
			curvesShadersMap[shaderProgramName]->printInfoLog();
		}
		if (curvesBillboardShadersMap.find(shaderProgramName) == curvesBillboardShadersMap.end()) {
			curvesBillboardShadersMap[shaderProgramName] = new GlShaderProgram(shaderProgramName);
			curvesBillboardShadersMap[shaderProgramName]->addShaderFromSourceCode(Vertex, genCommonUniformVariables(MAX_SHADER_CONTROL_POINTS) + curveSpecificShaderCode);
			curvesBillboardShadersMap[shaderProgramName]->addShader(curveVertexShaderBillboardMain);
			curvesBillboardShadersMap[shaderProgramName]->addShader(fisheyeDistortionVertexShader);
			curvesBillboardShadersMap[shaderProgramName]->link();
			curvesBillboardShadersMap[shaderProgramName]->printInfoLog();
		}
		if (curvesShadersMap[shaderProgramName]->isLinked()) {
			curveShaderProgramNormal = curvesShadersMap[shaderProgramName];
		}
		if (curvesBillboardShadersMap[shaderProgramName]->isLinked()) {
			curveShaderProgramBillboard = curvesBillboardShadersMap[shaderProgramName];
		}
	}
}

void AbstractGlCurve::drawCurve(std::vector<Coord> *controlPoints, const Color &startColor, const Color &endColor, const float startSize, const float endSize, const unsigned int nbCurvePoints) {

	static bool vboOk = checkVboSupport();

	if (curveVertexBuffersData.find(nbCurvePoints) == curveVertexBuffersData.end()) {
		buildCurveVertexBuffers(nbCurvePoints, vboOk);
	}

	GLint renderMode;
	glGetIntegerv(GL_RENDER_MODE, &renderMode);

	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);

	if (texture != "") {
		unsigned int i = nbCurvePoints / 2;
		Coord firstCurvePoint(computeCurvePointOnCPU(*controlPoints, i / static_cast<float>(nbCurvePoints - 1)));
		Coord nexCurvePoint(computeCurvePointOnCPU(*controlPoints, (i+1) / static_cast<float>(nbCurvePoints - 1)));
		float dist = firstCurvePoint.dist(nexCurvePoint);
		texCoordFactor = dist / (startSize * 2.0f);
	}

	if (billboardCurve) {
		curveShaderProgram = curveShaderProgramBillboard;
	} else {
		curveShaderProgram = curveShaderProgramNormal;
	}

	if (curveShaderProgram != NULL && controlPoints->size() <= MAX_SHADER_CONTROL_POINTS && renderMode != GL_SELECT) {

		GLuint *vbo = curveVertexBuffersObject[nbCurvePoints];
		GlShaderProgram *currentActiveShader = GlShaderProgram::getCurrentActiveShader();
		bool fisheyeActivated = currentActiveShader != NULL && currentActiveShader->getName() == "fisheye";
		GLfloat fisheyeCenter[4];
		GLfloat fisheyeRadius;
		GLfloat fisheyeHeight;
		GLint   fisheyeType;
		if (fisheyeActivated) {
			currentActiveShader->getUniformFloatVariableValue("center", fisheyeCenter);
			currentActiveShader->getUniformFloatVariableValue("radius", &fisheyeRadius);
			currentActiveShader->getUniformFloatVariableValue("height", &fisheyeHeight);
			currentActiveShader->getUniformIntVariableValue("fisheyeType", (int*)(&fisheyeType));
			currentActiveShader->desactivate();
		}

		curveShaderProgram->activate();
		curveShaderProgram->setUniformVec3FloatArray("controlPoints", controlPoints->size(), &((*controlPoints)[0][0]));
		curveShaderProgram->setUniformInt("nbControlPoints", controlPoints->size());
		curveShaderProgram->setUniformInt("nbCurvePoints", nbCurvePoints);
		curveShaderProgram->setUniformFloat("startSize", startSize);
		curveShaderProgram->setUniformFloat("endSize", endSize);
		curveShaderProgram->setUniformColor("startColor", startColor);
		curveShaderProgram->setUniformColor("endColor", endColor);
		curveShaderProgram->setUniformFloat("step", 1.0f / (static_cast<float>(nbCurvePoints) - 1.0f));
		curveShaderProgram->setUniformFloat("texCoordFactor", texCoordFactor);
		if (billboardCurve) {
			curveShaderProgram->setUniformVec3Float("lookDir", lookDir);
		}

		curveShaderProgram->setUniformBool("fisheye", fisheyeActivated);
		if (fisheyeActivated) {
			curveShaderProgram->setUniformVec4Float("center", fisheyeCenter[0], fisheyeCenter[1], fisheyeCenter[2], fisheyeCenter[3]);
			curveShaderProgram->setUniformFloat("radius", fisheyeRadius);
			curveShaderProgram->setUniformFloat("height", fisheyeHeight);
			curveShaderProgram->setUniformInt("fisheyeType", fisheyeType);
		}

		setCurveVertexShaderRenderingSpecificParameters();

		glEnableClientState(GL_VERTEX_ARRAY);

		if (vboOk) {
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glVertexPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), 0);
		} else {
			glVertexPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), curveVertexBuffersData[nbCurvePoints]);
		}

		if (startSize == 1 && endSize == 1) {
			if (vboOk) {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
				glDrawElements(GL_LINE_STRIP, nbCurvePoints, GL_UNSIGNED_SHORT, 0);
			} else {
				glDrawElements(GL_LINE_STRIP, nbCurvePoints, GL_UNSIGNED_SHORT, curveVertexBuffersIndices[nbCurvePoints][1]);
			}
		} else {
			if (texture != "") {
				glActiveTexture(GL_TEXTURE0);
				GlTextureManager::getInst().activateTexture(texture);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
			}
			if (billboardCurve) {
				glActiveTexture(GL_TEXTURE1);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				GlTextureManager::getInst().activateTexture(TulipBitmapDir+"cylinderTexture.png");
			}
			if (vboOk) {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
				glDrawElements(GL_TRIANGLE_STRIP, nbCurvePoints * 2, GL_UNSIGNED_SHORT, 0);
			} else {
				glDrawElements(GL_TRIANGLE_STRIP, nbCurvePoints * 2, GL_UNSIGNED_SHORT, curveVertexBuffersIndices[nbCurvePoints][0]);
			}
			if (billboardCurve) {
				glActiveTexture(GL_TEXTURE1);
				GlTextureManager::getInst().desactivateTexture();
			}
			if (texture != "") {
				glActiveTexture(GL_TEXTURE0);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				GlTextureManager::getInst().desactivateTexture();
			}
			if (outlined) {
				curveShaderProgram->setUniformColor("startColor", outlineColor);
				curveShaderProgram->setUniformColor("endColor", outlineColor);
			}

			if (vboOk) {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
				glDrawElements(GL_LINE_STRIP, nbCurvePoints, GL_UNSIGNED_SHORT, 0);
			} else {
				glDrawElements(GL_LINE_STRIP, nbCurvePoints, GL_UNSIGNED_SHORT, curveVertexBuffersIndices[nbCurvePoints][2]);
			}

			if (vboOk) {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[4]);
				glDrawElements(GL_LINE_STRIP, nbCurvePoints, GL_UNSIGNED_SHORT, 0);
			} else {
				glDrawElements(GL_LINE_STRIP, nbCurvePoints, GL_UNSIGNED_SHORT, curveVertexBuffersIndices[nbCurvePoints][3]);
			}
		}

		if (vboOk) {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		glDisableClientState(GL_VERTEX_ARRAY);

		curveShaderProgram->desactivate();

		cleanupAfterCurveVertexShaderRendering();

		if (fisheyeActivated) {
			currentActiveShader->activate();
		}

	} else {
		vector<Coord> curvePoints;
		computeCurvePointsOnCPU(*controlPoints, curvePoints, nbCurvePoints);
		if (!billboardCurve) {
			polyQuad(curvePoints, startColor, endColor, startSize, endSize, Coord(2.f*curvePoints[0] - curvePoints[1]), Coord(2.f*curvePoints[curvePoints.size() - 1] - curvePoints[curvePoints.size() - 2]),!outlined,outlineColor,texture);
		} else {
			simpleQuad(curvePoints, startColor, endColor, startSize, endSize, Coord(2.f*curvePoints[0] - curvePoints[1]),
					 Coord(2.f*curvePoints[curvePoints.size() - 1] - curvePoints[curvePoints.size() - 2]), lookDir, !outlined,outlineColor,texture);
		}
	}

	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);

}

void AbstractGlCurve::translate(const Coord &move) {
	for (size_t i = 0 ; i < controlPoints.size() ; ++i) {
		controlPoints[i] += move;
	}
	boundingBox.translate(move);
}

void AbstractGlCurve::getXML(xmlNodePtr) {}

void AbstractGlCurve::setWithXML(xmlNodePtr) {}

}
