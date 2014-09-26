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

#include <GL/glew.h>
#include <tulip/AbstractGlCurve.h>
#include <tulip/GlTextureManager.h>
#include <tulip/Curves.h>
#include <tulip/TlpTools.h>
#include <tulip/GlShaderProgram.h>

#include <sstream>

using namespace std;

static bool checkVboSupport() {
  static bool vboOk = glewIsSupported("GL_ARB_vertex_buffer_object") == GL_TRUE;
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

static string genCommonUniformVariables() {
  ostringstream oss;

  oss << "#version 120" << endl;
  oss << "uniform sampler1D controlPoints;" << endl;
  oss << "uniform int nbControlPoints;" << endl;
  oss << "const int controlPointsTexSize = 1024;" << endl;
  oss << "vec3 getControlPoint(int index) {" << endl;
  oss << "	return texture1D(controlPoints, float(index) / float(nbControlPoints - 1)).xyz;" << endl;
  oss << "}" << endl;

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

static string curveVertexShaderGeometryNormalMainSrc =
  "#version 120\n"
  "uniform float startSize;"
  "uniform float endSize;"
  "uniform vec4 startColor;"
  "uniform vec4 endColor;"


  "varying float size;"

  "vec3 computeCurvePoint(float t);"

  "void main () {"
  "	float t = gl_Vertex.x;"
  "	size = mix(startSize, endSize, t);"
  "	gl_Position = vec4(computeCurvePoint(t), t);"
  "	gl_FrontColor =  mix(startColor, endColor, t);"
  "}"
  ;

static const string curveExtrusionGeometryShaderSrc =
  "#version 120\n"
  "#extension GL_EXT_geometry_shader4 : enable\n"

  "const float M_PI = 3.141592653589793238462643;"

  "uniform bool topOutline;"
  "uniform bool bottomOutline;"

  "uniform int nbCurvePoints;"
  "uniform float texCoordFactor;"
  "uniform bool fisheye;"

  "varying in float size[4];"

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
  "}"

  "void computeExtrusionAndEmitVertices(vec3 pBefore, vec3 pCurrent, vec3 pAfter, float size, float t) {"
  "	vec3 u = pBefore - pCurrent;"
  "	vec3 v = pAfter - pCurrent;"
  "	vec3 xu = normalize(u);"
  "	vec3 xv = normalize(v);"
  "	vec3 bi_xu_xv = normalize(xu+xv);"
  "	float angle = M_PI - acos(dot(u,v)/(length(u)*length(v)));"


  //Nan check
  "	if(angle != angle) {"
  "		angle = 0.0;"
  "	}"

  "	float newSize = size;"

  "	float cosA = cos(angle / 2.0);"

  "	bool parallel = false;"

  "	if (cosA > 1e-1) {"
  "		newSize = size / cosA;"
  "	}"

  "	if (cosA < 1e-1 || angle < 1e-3) {"
  "		vec3 tmp = vec3(0.0);"
  "		tmp = normalize(pAfter - pCurrent);"
  "		bi_xu_xv = tmp;"
  "		bi_xu_xv.x = -tmp.y;"
  "		bi_xu_xv.y = tmp.x;"
  "		parallel = true;"
  "		angle = 0.0;"
  "	} "

  "	gl_TexCoord[0] = vec4(1.0);"

  "	if (parallel || cross(xu, xv)[2] < 0) {"
  "		if (topOutline) {"
  "			gl_TexCoord[0].st = vec2(t * float(nbCurvePoints - 1) * texCoordFactor, 1.0);"
  "			if (!fisheye)"
  "				gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent + bi_xu_xv * newSize, 1.0);"
  "			else"
  "				gl_Position = fisheyeDistortion(pCurrent + bi_xu_xv * newSize);"
  "			EmitVertex();"
  "		}"

  "		if (bottomOutline) {"
  "			gl_TexCoord[0].st = vec2(t * float(nbCurvePoints - 1) * texCoordFactor, 0.0);"
  "			if (!fisheye)"
  "				gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent - bi_xu_xv * newSize, 1.0);"
  "			else"
  "				gl_Position = fisheyeDistortion(pCurrent - bi_xu_xv * newSize);"
  "			EmitVertex();"
  "		}"

  "	} else {"
  "		if (topOutline) {"
  "			gl_TexCoord[0].st = vec2(t * float(nbCurvePoints - 1) * texCoordFactor, 1.0);"
  "			if (!fisheye)"
  "				gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent - bi_xu_xv * newSize, 1.0);"
  "			else"
  "				gl_Position = fisheyeDistortion(pCurrent - bi_xu_xv * newSize);"
  "			EmitVertex();"
  "		}"

  "		if (bottomOutline) {"
  "			gl_TexCoord[0].st = vec2(t * float(nbCurvePoints - 1) * texCoordFactor, 0.0);"
  "			if (!fisheye)"
  "				gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent + bi_xu_xv * newSize, 1.0);"
  "			else"
  "				gl_Position = fisheyeDistortion(pCurrent + bi_xu_xv * newSize);"
  "			EmitVertex();"
  "		}"
  "	}"
  "}"

  "void main() {"

  "	if (gl_PositionIn[0].w == 0.0) {"
  "		gl_FrontColor = gl_FrontColorIn[0];"
  "		computeExtrusionAndEmitVertices(gl_PositionIn[0].xyz - (gl_PositionIn[1].xyz - gl_PositionIn[0].xyz), gl_PositionIn[0].xyz, gl_PositionIn[1].xyz, size[0], gl_PositionIn[0].w);"
  "	}"

  "	gl_FrontColor = gl_FrontColorIn[1];"
  "	computeExtrusionAndEmitVertices(gl_PositionIn[0].xyz, gl_PositionIn[1].xyz, gl_PositionIn[2].xyz, size[1], gl_PositionIn[1].w);"
  "	gl_FrontColor = gl_FrontColorIn[2];"
  "	computeExtrusionAndEmitVertices(gl_PositionIn[1].xyz, gl_PositionIn[2].xyz, gl_PositionIn[3].xyz, size[2], gl_PositionIn[2].w);"

  "	if (gl_PositionIn[3].w == 1.0) {"
  "		gl_FrontColor = gl_FrontColorIn[3];"
  "		computeExtrusionAndEmitVertices(gl_PositionIn[2].xyz, gl_PositionIn[3].xyz, gl_PositionIn[3].xyz + (gl_PositionIn[3].xyz - gl_PositionIn[2].xyz), size[3], gl_PositionIn[3].w);"
  "	}"
  "}"
  ;

static const string curveExtrusionBillboardGeometryShaderSrc =
  "#version 120\n"
  "#extension GL_EXT_geometry_shader4 : enable\n"

  "const float M_PI = 3.141592653589793238462643;"

  "uniform bool topOutline;"
  "uniform bool bottomOutline;"

  "uniform int nbCurvePoints;"
  "uniform float texCoordFactor;"
  "uniform bool fisheye;"
  "uniform vec3 lookDir;"

  "varying in float size[4];"

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
  "}"

  "void computeExtrusionAndEmitVertices(vec3 pBefore, vec3 pCurrent, vec3 pAfter, float size, float t) {"
  "	vec3 dir = vec3(0.0);"
  "	float angle = 0.0;"
  "	if (t == 0.0) {"
  "		vec3 xu = normalize(pCurrent - pAfter);"
  "		dir = normalize(cross(xu, lookDir));"
  "	} else if (t == 1.0) {"
  "		vec3 xu = normalize(pBefore - pCurrent);"
  "		dir = normalize(cross(xu, lookDir));"
  "	} else {"
  "		vec3 u = normalize(pBefore - pCurrent);"
  "		vec3 v = normalize(pAfter - pCurrent);"
  "		vec3 xu = cross(u, lookDir);"
  "		vec3 xv = cross(v,-lookDir);"
  "		dir = normalize(xu+xv);"
  " 		float angle = M_PI - acos(dot(u,v));"
  //  Check to see if angle == NaN (GLSL Spec NaN != NaN) because the isnan builtin function is available since version 130 of GLSL
  "		if (angle != angle) angle = 0;"
  "	}"

  "	gl_TexCoord[0] = vec4(1.0);"
  "	gl_TexCoord[1] = vec4(1.0);"

  "	float newSize = size/cos(angle/2.0);"

  "	if (topOutline) {"
  "		gl_TexCoord[0].st = vec2(t * float(nbCurvePoints - 1) * texCoordFactor, 1.0);"
  "		gl_TexCoord[1].st = vec2(t * float(nbCurvePoints - 1) * texCoordFactor, 1.0);"
  "		if (!fisheye)"
  "			gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent + dir * newSize, 1.0);"
  "		else"
  "			gl_Position = fisheyeDistortion(pCurrent + dir * newSize);"
  "		EmitVertex();"
  "	}"

  "	if (bottomOutline) {"
  "		gl_TexCoord[0].st = vec2(t * float(nbCurvePoints - 1) * texCoordFactor, 0.0);"
  "		gl_TexCoord[1].st = vec2(t * float(nbCurvePoints - 1) * texCoordFactor, 0.0);"
  "		if (!fisheye)"
  "			gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent - dir * newSize, 1.0);"
  "		else"
  "			gl_Position = fisheyeDistortion(pCurrent - dir * newSize);"
  "		EmitVertex();"
  "	}"
  "}"

  "void main() {"

  "	if (gl_PositionIn[0].w == 0.0) {"
  "		gl_FrontColor = gl_FrontColorIn[0];"
  "		computeExtrusionAndEmitVertices(gl_PositionIn[0].xyz - (gl_PositionIn[1].xyz - gl_PositionIn[0].xyz), gl_PositionIn[0].xyz, gl_PositionIn[1].xyz, size[0], gl_PositionIn[0].w);"
  "	}"

  "	gl_FrontColor = gl_FrontColorIn[1];"
  "	computeExtrusionAndEmitVertices(gl_PositionIn[0].xyz, gl_PositionIn[1].xyz, gl_PositionIn[2].xyz, size[1], gl_PositionIn[1].w);"
  "	gl_FrontColor = gl_FrontColorIn[2];"
  "	computeExtrusionAndEmitVertices(gl_PositionIn[1].xyz, gl_PositionIn[2].xyz, gl_PositionIn[3].xyz, size[2], gl_PositionIn[2].w);"

  "	if (gl_PositionIn[3].w == 1.0) {"
  "		gl_FrontColor = gl_FrontColorIn[3];"
  "		computeExtrusionAndEmitVertices(gl_PositionIn[2].xyz, gl_PositionIn[3].xyz, gl_PositionIn[3].xyz + (gl_PositionIn[3].xyz - gl_PositionIn[2].xyz), size[3], gl_PositionIn[3].w);"
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
  //  Check to see if angle == NaN (GLSL Spec NaN != NaN) because the isnan builtin function is available since version 130 of GLSL
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
GlShader *AbstractGlCurve::fisheyeDistortionVertexShader(NULL);
GlShader *AbstractGlCurve::curveVertexShaderNormalMain(NULL);
GlShader *AbstractGlCurve::curveVertexShaderBillboardMain(NULL);
bool AbstractGlCurve::canUseGeometryShader = false;
std::map<std::string, std::pair<GlShaderProgram *, GlShaderProgram *> > AbstractGlCurve::curvesGeometryShadersMap;
GlShader *AbstractGlCurve::curveVertexGeometryShaderNormalMain(NULL);
std::map<std::string, std::pair<GlShaderProgram *, GlShaderProgram *> > AbstractGlCurve::curvesBillboardGeometryShadersMap;

AbstractGlCurve::AbstractGlCurve(const string &shaderProgramName, const string &curveSpecificShaderCode) :
  curveShaderProgramNormal(NULL), curveShaderProgramBillboard(NULL), curveShaderProgram(NULL),
  outlined(false), outlineColor(Color(0,0,0)), texture(""), texCoordFactor(1), billboardCurve(false),
  lookDir(Coord(0,0,1)), lineCurve(false), curveLineWidth(1.0f), curveQuadBordersWidth(1.0f),
  outlineColorInterpolation(false) {
  canUseGeometryShader = GlShaderProgram::geometryShaderSupported();
  initShader(shaderProgramName, curveSpecificShaderCode);
}

AbstractGlCurve::AbstractGlCurve(const string &shaderProgramName, const string &curveSpecificShaderCode, const vector<Coord> &controlPoints,
                                 const Color &startColor, const Color &endColor, const float startSize, const float endSize, const unsigned int nbCurvePoints) :
  shaderProgramName(shaderProgramName), curveShaderProgramNormal(NULL), curveShaderProgramBillboard(NULL), curveShaderProgram(NULL),
  controlPoints(controlPoints), startColor(startColor), endColor(endColor), startSize(startSize), endSize(endSize), nbCurvePoints(nbCurvePoints),
  outlined(false), outlineColor(Color(0,0,0)), texture(""), texCoordFactor(1), billboardCurve(false), lookDir(Coord(0,0,1)),
  lineCurve(false), curveLineWidth(1.0f), curveQuadBordersWidth(1.0f), outlineColorInterpolation(false) {

  canUseGeometryShader = GlShaderProgram::geometryShaderSupported();
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
    float t =  i / static_cast<float>(nbCurvePoints - 1);
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
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
}

void AbstractGlCurve::draw(float, Camera *) {
  drawCurve(controlPoints, startColor, endColor, startSize, endSize, nbCurvePoints);
}

void AbstractGlCurve::initShader(const std::string &shaderProgramName, const std::string &curveSpecificShaderCode) {
  // restrict shaders compilation on compatible hardware, crashs can happened when using the Mesa software rasterizer
  static string glVendor(reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
  static bool glVendorOk = (glVendor.find("NVIDIA")!=string::npos) || (glVendor.find("ATI")!=string::npos);

  if (glVendorOk && GlShaderProgram::shaderProgramsSupported()) {

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

    if (curveVertexGeometryShaderNormalMain == NULL) {
      curveVertexGeometryShaderNormalMain = new GlShader(Vertex);
      curveVertexGeometryShaderNormalMain->compileFromSourceCode(curveVertexShaderGeometryNormalMainSrc);
    }

    if (curvesShadersMap.find(shaderProgramName) == curvesShadersMap.end()) {
      curvesShadersMap[shaderProgramName] = new GlShaderProgram(shaderProgramName);
      curvesShadersMap[shaderProgramName]->addShaderFromSourceCode(Vertex, genCommonUniformVariables() + curveSpecificShaderCode);
      curvesShadersMap[shaderProgramName]->addShader(curveVertexShaderNormalMain);
      curvesShadersMap[shaderProgramName]->addShader(fisheyeDistortionVertexShader);
      curvesShadersMap[shaderProgramName]->link();
      curvesShadersMap[shaderProgramName]->printInfoLog();
    }

    if (canUseGeometryShader && curvesGeometryShadersMap.find(shaderProgramName) == curvesGeometryShadersMap.end()) {
      GlShaderProgram *polygonShader = new GlShaderProgram(shaderProgramName);
      polygonShader->addShaderFromSourceCode(Vertex, genCommonUniformVariables() + curveSpecificShaderCode);
      polygonShader->addShader(curveVertexGeometryShaderNormalMain);
      polygonShader->addGeometryShaderFromSourceCode(curveExtrusionGeometryShaderSrc, GL_LINES_ADJACENCY_EXT, GL_TRIANGLE_STRIP);
      polygonShader->setMaxGeometryShaderOutputVertices(6);
      polygonShader->link();
      polygonShader->printInfoLog();

      if (!polygonShader->isLinked()) {
        delete polygonShader;
        polygonShader = NULL;
      }

      GlShaderProgram *lineShader = new GlShaderProgram(shaderProgramName);
      lineShader->addShaderFromSourceCode(Vertex, genCommonUniformVariables() + curveSpecificShaderCode);
      lineShader->addShader(curveVertexGeometryShaderNormalMain);
      lineShader->addGeometryShaderFromSourceCode(curveExtrusionGeometryShaderSrc, GL_LINES_ADJACENCY_EXT, GL_LINE_STRIP);
      lineShader->setMaxGeometryShaderOutputVertices(6);
      lineShader->link();
      lineShader->printInfoLog();

      if (!lineShader->isLinked()) {
        delete lineShader;
        lineShader = NULL;
      }

      curvesGeometryShadersMap[shaderProgramName] = make_pair(polygonShader, lineShader);
    }

    if (curvesBillboardShadersMap.find(shaderProgramName) == curvesBillboardShadersMap.end()) {
      curvesBillboardShadersMap[shaderProgramName] = new GlShaderProgram(shaderProgramName);
      curvesBillboardShadersMap[shaderProgramName]->addShaderFromSourceCode(Vertex, genCommonUniformVariables() + curveSpecificShaderCode);

      curvesBillboardShadersMap[shaderProgramName]->addShader(curveVertexShaderBillboardMain);
      curvesBillboardShadersMap[shaderProgramName]->addShader(fisheyeDistortionVertexShader);
      curvesBillboardShadersMap[shaderProgramName]->link();
      curvesBillboardShadersMap[shaderProgramName]->printInfoLog();
    }

    if (canUseGeometryShader && curvesBillboardGeometryShadersMap.find(shaderProgramName) == curvesBillboardGeometryShadersMap.end()) {
      GlShaderProgram *polygonShader = new GlShaderProgram(shaderProgramName);
      polygonShader->addShaderFromSourceCode(Vertex, genCommonUniformVariables() + curveSpecificShaderCode);
      polygonShader->addShader(curveVertexGeometryShaderNormalMain);
      polygonShader->addGeometryShaderFromSourceCode(curveExtrusionBillboardGeometryShaderSrc, GL_LINES_ADJACENCY_EXT, GL_TRIANGLE_STRIP);
      polygonShader->setMaxGeometryShaderOutputVertices(6);
      polygonShader->link();
      polygonShader->printInfoLog();

      if (!polygonShader->isLinked()) {
        delete polygonShader;
        polygonShader = NULL;
      }

      GlShaderProgram *lineShader = new GlShaderProgram(shaderProgramName);
      lineShader->addShaderFromSourceCode(Vertex, genCommonUniformVariables() + curveSpecificShaderCode);
      lineShader->addShader(curveVertexGeometryShaderNormalMain);
      lineShader->addGeometryShaderFromSourceCode(curveExtrusionBillboardGeometryShaderSrc, GL_LINES_ADJACENCY_EXT, GL_LINE_STRIP);
      lineShader->setMaxGeometryShaderOutputVertices(6);
      lineShader->link();
      lineShader->printInfoLog();

      if (!lineShader->isLinked()) {
        delete lineShader;
        lineShader = NULL;
      }

      curvesBillboardGeometryShadersMap[shaderProgramName] = make_pair(polygonShader, lineShader);
    }

    if (curvesShadersMap[shaderProgramName]->isLinked()) {
      curveShaderProgramNormal = curvesShadersMap[shaderProgramName];
    }

    if (curvesBillboardShadersMap[shaderProgramName]->isLinked()) {
      curveShaderProgramBillboard = curvesBillboardShadersMap[shaderProgramName];
    }
  }
}

void AbstractGlCurve::drawCurve(std::vector<Coord> &controlPoints, const Color &startColor, const Color &endColor, const float startSize, const float endSize, const unsigned int nbCurvePoints) {

  GLint renderMode;
  glGetIntegerv(GL_RENDER_MODE, &renderMode);

  float currentLineWidth;
  glGetFloatv(GL_LINE_WIDTH, &currentLineWidth);

  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);

  if (texture != "") {
    unsigned int i = nbCurvePoints / 2;
    Coord firstCurvePoint(computeCurvePointOnCPU(controlPoints, i / static_cast<float>(nbCurvePoints - 1)));
    Coord nexCurvePoint(computeCurvePointOnCPU(controlPoints, (i+1) / static_cast<float>(nbCurvePoints - 1)));
    float dist = firstCurvePoint.dist(nexCurvePoint);
    texCoordFactor = dist / (startSize * 2.0f);
  }

  if (billboardCurve) {
    curveShaderProgram = curveShaderProgramBillboard;
  }
  else {
    curveShaderProgram = curveShaderProgramNormal;
  }

  pair<GlShaderProgram *, GlShaderProgram *> geometryShaders = std::make_pair((GlShaderProgram*)NULL,(GlShaderProgram*)NULL);
  pair<GlShaderProgram *, GlShaderProgram *> geometryBillboardShaders = std::make_pair((GlShaderProgram*)NULL,(GlShaderProgram*)NULL);

  if (canUseGeometryShader && curvesGeometryShadersMap.find(curveShaderProgram->getName()) != curvesGeometryShadersMap.end()) {
    geometryShaders = curvesGeometryShadersMap[curveShaderProgram->getName()];
    geometryBillboardShaders = curvesBillboardGeometryShadersMap[curveShaderProgram->getName()];
  }

  static bool canUseFloatTextures = (glewIsSupported("GL_ARB_texture_float") == GL_TRUE);

  if (curveShaderProgram != NULL && canUseFloatTextures && renderMode != GL_SELECT) {

    static bool vboOk = checkVboSupport();

    if (curveVertexBuffersData.find(nbCurvePoints) == curveVertexBuffersData.end()) {
      buildCurveVertexBuffers(nbCurvePoints, vboOk);
    }

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

    static GLuint controlPointsTexId = 0;

    if (controlPointsTexId == 0) {
      glGenTextures(1, &controlPointsTexId);
    }

    vector<float> controlPointsData(1024*4, 0);


    glEnable(GL_TEXTURE_1D);
    glBindTexture(GL_TEXTURE_1D, controlPointsTexId);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB32F_ARB, controlPoints.size(), 0, GL_RGB, GL_FLOAT, &controlPoints[0][0]);
    glBindTexture(GL_TEXTURE_1D, 0);
    glDisable(GL_TEXTURE_1D);


    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_1D, controlPointsTexId);


    bool geometryShaderActivated = false;

    if (canUseGeometryShader && nbCurvePoints > 3 && !lineCurve && geometryShaders.first) {
      if (!billboardCurve) {
        curveShaderProgram = geometryShaders.first;
      }
      else {
        curveShaderProgram = geometryBillboardShaders.first;
      }

      geometryShaderActivated = true;
    }

    curveShaderProgram->activate();
    curveShaderProgram->setUniformTextureSampler("controlPoints", 3);
    curveShaderProgram->setUniformInt("nbControlPoints", controlPoints.size());
    curveShaderProgram->setUniformInt("nbCurvePoints", nbCurvePoints);
    curveShaderProgram->setUniformFloat("startSize", startSize);
    curveShaderProgram->setUniformFloat("endSize", endSize);
    curveShaderProgram->setUniformColor("startColor", startColor);
    curveShaderProgram->setUniformColor("endColor", endColor);

    if (!geometryShaderActivated) {
      curveShaderProgram->setUniformFloat("step", 1.0f / (static_cast<float>(nbCurvePoints) - 1.0f));
    }
    else {
      curveShaderProgram->setUniformBool("topOutline", true);
      curveShaderProgram->setUniformBool("bottomOutline", true);
    }

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
    }
    else {
      glVertexPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), curveVertexBuffersData[nbCurvePoints]);
    }

    if (lineCurve) {
      glLineWidth(curveLineWidth);

      if (vboOk) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
        glDrawElements(GL_LINE_STRIP, nbCurvePoints, GL_UNSIGNED_SHORT, 0);
      }
      else {
        glDrawElements(GL_LINE_STRIP, nbCurvePoints, GL_UNSIGNED_SHORT, curveVertexBuffersIndices[nbCurvePoints][1]);
      }

    }
    else {
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
        if (geometryShaderActivated) {
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
          glDrawElements(GL_LINE_STRIP_ADJACENCY_EXT, nbCurvePoints, GL_UNSIGNED_SHORT, 0);
        }
        else {
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
          glDrawElements(GL_TRIANGLE_STRIP, nbCurvePoints * 2, GL_UNSIGNED_SHORT, 0);
        }
      }
      else {
        if (geometryShaderActivated) {
          glDrawElements(GL_LINE_STRIP_ADJACENCY_EXT, nbCurvePoints, GL_UNSIGNED_SHORT, curveVertexBuffersIndices[nbCurvePoints][1]);
        }
        else {
          glDrawElements(GL_TRIANGLE_STRIP, nbCurvePoints * 2, GL_UNSIGNED_SHORT, curveVertexBuffersIndices[nbCurvePoints][0]);
        }
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

        if (canUseGeometryShader && nbCurvePoints > 3 && geometryShaders.second) {
          curveShaderProgram->desactivate();

          if (billboardCurve) {
            curveShaderProgram = geometryBillboardShaders.second;
          }
          else {
            curveShaderProgram = geometryShaders.second;
          }

          curveShaderProgram->activate();
          curveShaderProgram->setUniformTextureSampler("controlPoints", 3);
          curveShaderProgram->setUniformInt("nbControlPoints", controlPoints.size());
          curveShaderProgram->setUniformInt("nbCurvePoints", nbCurvePoints);
          curveShaderProgram->setUniformFloat("startSize", startSize);
          curveShaderProgram->setUniformFloat("endSize", endSize);
          curveShaderProgram->setUniformBool("topOutline", true);
          curveShaderProgram->setUniformBool("bottomOutline", false);
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
        }

        if (outlineColorInterpolation) {
          curveShaderProgram->setUniformColor("startColor", startColor);
          curveShaderProgram->setUniformColor("endColor", endColor);
        }
        else {
          curveShaderProgram->setUniformColor("startColor", outlineColor);
          curveShaderProgram->setUniformColor("endColor", outlineColor);
        }

        glLineWidth(curveQuadBordersWidth);

        if (vboOk) {
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);

          if (geometryShaderActivated) {
            glDrawElements(GL_LINE_STRIP_ADJACENCY_EXT, nbCurvePoints, GL_UNSIGNED_SHORT, 0);
          }
          else {
            glDrawElements(GL_LINE_STRIP, nbCurvePoints, GL_UNSIGNED_SHORT, 0);
          }
        }
        else {
          if (geometryShaderActivated) {
            glDrawElements(GL_LINE_STRIP_ADJACENCY_EXT, nbCurvePoints, GL_UNSIGNED_SHORT, curveVertexBuffersIndices[nbCurvePoints][2]);
          }
          else {
            glDrawElements(GL_LINE_STRIP, nbCurvePoints, GL_UNSIGNED_SHORT, curveVertexBuffersIndices[nbCurvePoints][2]);
          }
        }

        if (canUseGeometryShader && nbCurvePoints > 3 && geometryShaders.second) {
          curveShaderProgram->setUniformBool("topOutline", false);
          curveShaderProgram->setUniformBool("bottomOutline", true);
        }

        if (vboOk) {
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[4]);

          if (geometryShaderActivated) {
            glDrawElements(GL_LINE_STRIP_ADJACENCY_EXT, nbCurvePoints, GL_UNSIGNED_SHORT, 0);
          }
          else {
            glDrawElements(GL_LINE_STRIP, nbCurvePoints, GL_UNSIGNED_SHORT, 0);
          }
        }
        else {
          if (geometryShaderActivated) {
            glDrawElements(GL_LINE_STRIP_ADJACENCY_EXT, nbCurvePoints, GL_UNSIGNED_SHORT, curveVertexBuffersIndices[nbCurvePoints][3]);
          }
          else {
            glDrawElements(GL_LINE_STRIP, nbCurvePoints, GL_UNSIGNED_SHORT, curveVertexBuffersIndices[nbCurvePoints][3]);
          }
        }
      }
    }


    if (vboOk) {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    glDisableClientState(GL_VERTEX_ARRAY);

    curveShaderProgram->desactivate();

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_1D, 0);

    glActiveTexture(GL_TEXTURE0);

    cleanupAfterCurveVertexShaderRendering();

    if (fisheyeActivated) {
      currentActiveShader->activate();
    }

  }
  else {
    vector<Coord> curvePoints;
    computeCurvePointsOnCPU(controlPoints, curvePoints, nbCurvePoints);

    if (lineCurve) {
      glLineWidth(curveLineWidth);
      polyLine(curvePoints, startColor, endColor);
    }
    else if (!billboardCurve) {
      polyQuad(curvePoints, startColor, endColor, startSize, endSize, Coord(2.f*curvePoints[0] - curvePoints[1]), Coord(2.f*curvePoints[curvePoints.size() - 1] - curvePoints[curvePoints.size() - 2]),outlineColorInterpolation,outlineColor,texture,curveQuadBordersWidth);
    }
    else {
      simpleQuad(curvePoints, startColor, endColor, startSize, endSize, Coord(2.f*curvePoints[0] - curvePoints[1]),
                 Coord(2.f*curvePoints[curvePoints.size() - 1] - curvePoints[curvePoints.size() - 2]), lookDir, !outlined,outlineColor,texture);
    }
  }

  glLineWidth(currentLineWidth);

  glEnable(GL_LIGHTING);
  glEnable(GL_CULL_FACE);

}

void AbstractGlCurve::translate(const Coord &move) {
  for (size_t i = 0 ; i < controlPoints.size() ; ++i) {
    controlPoints[i] += move;
  }

  boundingBox.translate(move);
}

void AbstractGlCurve::getXML(string &) {}

void AbstractGlCurve::setWithXML(const string &,unsigned int &) {}

}
