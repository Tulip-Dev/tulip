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

#include <tulip/GlComplexPolygon.h>
#include <tulip/GlTools.h>
#include <tulip/GlTextureManager.h>
#include <tulip/ParametricCurves.h>
#include <tulip/GlShaderProgram.h>
#include <tulip/GlXMLTools.h>

#ifdef __APPLE_CC__
#if __APPLE_CC__ < 5400 && ( !defined(__GNUC__) || __GNUC_MINOR__ <= 2)
// Tiger
typedef GLvoid (*GLUTesselatorFunction)(...);
#else
// Leopard and higher
typedef GLvoid (*GLUTesselatorFunction)();
#endif
#elif defined( __mips ) || defined( __unix__ ) || defined( __FreeBSD_kernel__) || defined( __FreeBSD__ ) || defined( __OpenBSD__ ) || defined( __sun ) || defined (__CYGWIN__)
typedef GLvoid (*GLUTesselatorFunction)();
#elif defined (WIN32)
typedef void (CALLBACK*GLUTesselatorFunction)(void);
#else
#error "Error - need to define type GLUTesselatorFunction for this platform/compiler"
#endif


using namespace std;

static const string outlineExtrusionVertexShaderSrc =
  "#version 120\n"
  "attribute float indice;"

  "void main() {"
  "	gl_Position = vec4(gl_Vertex.xyz, indice);"
  "	gl_FrontColor = gl_Color;"
  "}"
  ;

static const string outlineExtrusionGeometryShaderSrc =
  "#version 120\n"
  "#extension GL_EXT_geometry_shader4 : enable\n"

  "#define M_PI 3.141592653589793238462643\n"

  "uniform vec3 firstPoint;"
  "uniform vec3 secondPoint;"
  "uniform vec3 lastPoint;"

  "uniform float size;"
  "uniform int nbVertices;"
  "uniform int outlinePos;"
  "uniform float texFactor;"


  "float computeExtrusionAndEmitVertices(vec3 pBefore, vec3 pCurrent, vec3 pAfter, float s, float d) {"
  "	vec3 u = pBefore - pCurrent;"
  "	vec3 v = pAfter - pCurrent;"
  "	vec3 xu = normalize(u);"
  "	vec3 xv = normalize(v);"
  "	vec3 bi_xu_xv = normalize(xu+xv);"
  "	float angle = M_PI - acos((u[0]*v[0]+u[1]*v[1]+u[2]*v[2])/(length(u)*length(v)));"
  // Nan check
  "	if(angle != angle)"
  "		angle=0.0;"
  "	float newSize = size/cos(angle/2.0);"
  "	float dec = 0.0;"

  "	gl_FrontColor = gl_FrontColorIn[1];"

  "	if (angle < M_PI/2+M_PI/4) {"
  "		if (cross(xu, xv)[2] > 0) {"
  "			if (outlinePos <= 1) {"
  "				gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent + bi_xu_xv * newSize, 1.0);"
  "			} else {"
  "				gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent, 1.0);"
  "			}"
  "			gl_TexCoord[0].st = vec2((s+d)*texFactor, 0.0);"
  "			EmitVertex();"
  "			if (outlinePos == 0) {"
  "				gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent, 1.0);"
  "			} else {"
  "				gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent - bi_xu_xv * newSize, 1.0);"
  "			}"
  "			gl_TexCoord[0].st = vec2((s+d)*texFactor, 1.0);"
  "			EmitVertex();"
  "		} else {"
  "			if (outlinePos <= 1) {"
  "				gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent - bi_xu_xv * newSize, 1.0);"
  "			} else {"
  "				gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent, 1.0);"
  "			}"
  "			gl_TexCoord[0].st = vec2((s+d)*texFactor, 0.0);"
  "			EmitVertex();"
  "			if (outlinePos == 0) {"
  "				gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent, 1.0);"
  "			} else {"
  "				gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent + bi_xu_xv * newSize, 1.0);"
  "			}"
  "			gl_TexCoord[0].st = vec2((s+d)*texFactor, 1.0);"
  "			EmitVertex();"
  "		}"
  "	} else {"
  "		vec3 vectUnit = vec3(-bi_xu_xv[1],bi_xu_xv[0],bi_xu_xv[2]);"
  "		if (!(newSize > length(u) || newSize> length(v) || abs(angle-M_PI)<1E-5)) {"
  "			if (cross(xu,xv)[2] > 0) {"
  "				if (outlinePos <= 1) {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent + bi_xu_xv * newSize, 1.0);"
  "				} else {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent, 1.0);"
  "				}"
  "				gl_TexCoord[0].st = vec2((s+d)*texFactor, 0.0);"
  "				EmitVertex();"
  "				if (outlinePos == 0) {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent, 1.0);"
  "				} else {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent - vectUnit * size, 1.0);"
  "				}"
  "				gl_TexCoord[0].st = vec2((s+d)*texFactor, 1.0);"
  "				EmitVertex();"
  "				if (outlinePos <= 1) {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent + bi_xu_xv * newSize, 1.0);"
  "				} else {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent, 1.0);"
  "				}"
  "				gl_TexCoord[0].st = vec2((s+d+1.0)*texFactor, 0.0);"
  "				EmitVertex();"
  "				if (outlinePos == 0) {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent, 1.0);"
  "				} else {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent + vectUnit * size, 1.0);"
  "				}"
  "				gl_TexCoord[0].st = vec2((s+d+1.0)*texFactor, 1.0);"
  "				EmitVertex();"
  "				dec = 1.0;"
  "			} else {"
  "				if (outlinePos <= 1) {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent + bi_xu_xv * newSize, 1.0);"
  "				} else {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent, 1.0);"
  "				}"
  "				gl_TexCoord[0].st = vec2((s+d)*texFactor, 0.0);"
  "				EmitVertex();"
  "				if (outlinePos == 0) {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent, 1.0);"
  "				} else {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent + vectUnit * size, 1.0);"
  "				}"
  "				gl_TexCoord[0].st = vec2((s+d)*texFactor, 1.0);"
  "				EmitVertex();"
  "				if (outlinePos <= 1) {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent + bi_xu_xv * newSize, 1.0);"
  "				} else {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent, 1.0);"
  "				}"
  "				gl_TexCoord[0].st = vec2((s+d+1.0)*texFactor, 0.0);"
  "				EmitVertex();"
  "				if (outlinePos == 0) {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent, 1.0);"
  "				} else {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent - vectUnit * size, 1.0);"
  "				}"
  "				gl_TexCoord[0].st = vec2((s+d+1.0)*texFactor, 1.0);"
  "				EmitVertex();"
  "				dec = 1.0;"
  "			}"
  "		} else {"
  "			if (cross(xu,xv)[2] > 0) {"
  "				if (outlinePos <= 1) {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent + vectUnit * size, 1.0);"
  "				} else {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent, 1.0);"
  "				}"
  "				gl_TexCoord[0].st = vec2((s+d)*texFactor, 0.0);"
  "				EmitVertex();"
  "				if (outlinePos == 0) {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent, 1.0);"
  "				} else {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent - vectUnit * size, 1.0);"
  "				}"
  "				gl_TexCoord[0].st = vec2((s+d)*texFactor, 1.0);"
  "				EmitVertex();"
  "			} else {"
  "				if (outlinePos <= 1) {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent - vectUnit * size, 1.0);"
  "				} else {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent, 1.0);"
  "				}"
  "				gl_TexCoord[0].st = vec2((s+d)*texFactor, 1.0);"
  "				EmitVertex();"
  "				if (outlinePos == 0) {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent, 1.0);"
  "				} else {"
  "					gl_Position = gl_ModelViewProjectionMatrix * vec4(pCurrent + vectUnit * size, 1.0);"
  "				}"

  "				gl_TexCoord[0].st = vec2((s+d)*texFactor, 0.0);"
  "				EmitVertex();"
  "			}"
  "		}"
  "	}"
  "	return dec;"
  "}"

  "void main() {"
  "	vec3 tangent, normal;"

  "	gl_TexCoord[0].z = 0.0;"
  "	gl_TexCoord[0].w = 1.0;"

  "	float dec = 0.0;"

  "	if (int(gl_PositionIn[0].w) == 0) {"
  "		gl_FrontColor = gl_FrontColorIn[0];"
  "		dec = computeExtrusionAndEmitVertices(lastPoint, gl_PositionIn[0].xyz, gl_PositionIn[1].xyz, gl_PositionIn[0].w, dec);"
  "	}"

  "	dec = computeExtrusionAndEmitVertices(gl_PositionIn[0].xyz, gl_PositionIn[1].xyz, gl_PositionIn[2].xyz, gl_PositionIn[1].w, 0.0);"
  "	dec = computeExtrusionAndEmitVertices(gl_PositionIn[1].xyz, gl_PositionIn[2].xyz, gl_PositionIn[3].xyz, gl_PositionIn[2].w, dec);"

  "	if (int(gl_PositionIn[3].w) == (nbVertices - 1)) {"
  "		gl_FrontColor = gl_FrontColorIn[3];"
  "		dec = computeExtrusionAndEmitVertices(gl_PositionIn[2].xyz, gl_PositionIn[3].xyz, firstPoint, gl_PositionIn[3].w, dec);"
  "		dec = computeExtrusionAndEmitVertices(gl_PositionIn[3].xyz, firstPoint, secondPoint, gl_PositionIn[3].w+1, dec);"

  "	}"
  "	EndPrimitive();"
  "}"

  ;

namespace tlp {

void CALLBACK beginCallback(GLenum which, GLvoid *polygonData) {
  GlComplexPolygon *complexPolygon = static_cast<GlComplexPolygon *>(polygonData);
  complexPolygon->startPrimitive(which);
}

void CALLBACK errorCallback(GLenum errorCode) {
  const GLubyte *estring = gluErrorString(errorCode);
  tlp::error() << "Tessellation Error: " << estring << endl;
}

void CALLBACK endCallback(GLvoid *polygonData) {
  GlComplexPolygon *complexPolygon = static_cast<GlComplexPolygon *>(polygonData);
  complexPolygon->endPrimitive();
}

void CALLBACK vertexCallback(GLvoid *vertex, GLvoid *polygonData) {
  const GLdouble *pointer = static_cast<GLdouble *>(vertex);
  GlComplexPolygon *complexPolygon = static_cast<GlComplexPolygon *>(polygonData);
  Coord v(pointer[0], pointer[1], pointer[2]);
  Vec2f texCoord;
  texCoord[0] = pointer[0] / complexPolygon->getTextureZoom();
  texCoord[1] = pointer[1] / complexPolygon->getTextureZoom();
  complexPolygon->addVertex(v, texCoord);
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
  currentVector(-1),
  outlined(false),
  fillColor(fcolor),
  outlineSize(1),
  textureName(textureName),
  textureZoom(1.) {
  createPolygon(coords,polygonEdgesType);
  runTesselation();
}
//=====================================================
GlComplexPolygon::GlComplexPolygon(const vector<Coord> &coords,Color fcolor,Color ocolor,int polygonEdgesType,const string &textureName):
  currentVector(-1),
  outlined(true),
  fillColor(fcolor),
  outlineColor(ocolor),
  outlineSize(1),
  textureName(textureName),
  textureZoom(1.) {
  if (!coords.empty()) {
    createPolygon(coords,polygonEdgesType);
    runTesselation();
  }
}
//=====================================================
GlComplexPolygon::GlComplexPolygon(const vector<vector<Coord> >&coords,Color fcolor,int polygonEdgesType,const string &textureName):
  currentVector(-1),
  outlined(false),
  fillColor(fcolor),
  outlineSize(1),
  textureName(textureName),
  textureZoom(1.) {
  for (size_t i = 0 ; i < coords.size() ; ++i) {
    createPolygon(coords[i],polygonEdgesType);
  }

  runTesselation();
}
//=====================================================
GlComplexPolygon::GlComplexPolygon(const vector<vector<Coord> >&coords,Color fcolor,Color ocolor,int polygonEdgesType,const string &textureName):
  currentVector(-1),
  outlined(true),
  fillColor(fcolor),
  outlineColor(ocolor),
  outlineSize(1),
  textureName(textureName),
  textureZoom(1.) {
  for(unsigned int i=0; i<coords.size(); ++i) {
    createPolygon(coords[i],polygonEdgesType);
  }

  runTesselation();
}
//=====================================================
void GlComplexPolygon::createPolygon(const vector<Coord> &coords,int polygonEdgesType) {
  beginNewHole();

  if (polygonEdgesType == 1) {
    vector<Coord> catmullPoints;
    computeCatmullRomPoints(coords, catmullPoints, true, coords.size() * 20);

    for (size_t i = 0 ; i < catmullPoints.size() ; ++i) {
      addPoint(catmullPoints[i]);
    }
  }
  else if (polygonEdgesType == 2) {

    const unsigned int nbCurvePoints = 20;
    addPoint(coords[0]);

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
  }
  else {
    for(vector<Coord>::const_iterator it=coords.begin(); it!=coords.end(); ++it) {
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
string GlComplexPolygon::getTextureName() {
  return textureName;
}
//=====================================================
void GlComplexPolygon::setTextureName(const string &name) {
  textureName=name;
}
//=====================================================
void GlComplexPolygon::addPoint(const Coord& point) {
  pointsIdx[currentVector].push_back(points[currentVector].size());
  points[currentVector].push_back(point);
  boundingBox.expand(point);
}
//=====================================================
void GlComplexPolygon::beginNewHole() {
  ++currentVector;
  points.push_back(vector<Coord>());
  pointsIdx.push_back(vector<GLfloat>());
  quadBorderActivated.push_back(false);
  quadBorderColor.push_back(Color(255,255,255));
  quadBorderTexture.push_back("");
  quadBorderWidth.push_back(0);
  quadBorderPosition.push_back(1);
  quadBorderTexFactor.push_back(1.f);
}
void GlComplexPolygon::runTesselation() {

  primitivesSet.clear();
  startIndicesMap.clear();
  verticesCountMap.clear();
  verticesMap.clear();
  texCoordsMap.clear();

  GLUtesselator *tobj;
  tobj = gluNewTess();

  gluTessCallback(tobj, GLU_TESS_BEGIN_DATA, reinterpret_cast<GLUTesselatorFunction>(&beginCallback));
  gluTessCallback(tobj, GLU_TESS_VERTEX_DATA, reinterpret_cast<GLUTesselatorFunction>(&vertexCallback));
  gluTessCallback(tobj, GLU_TESS_END_DATA, reinterpret_cast<GLUTesselatorFunction>(&endCallback));
  gluTessCallback(tobj, GLU_TESS_COMBINE_DATA, reinterpret_cast<GLUTesselatorFunction>(&combineCallback));
  gluTessCallback(tobj, GLU_TESS_ERROR, reinterpret_cast<GLUTesselatorFunction>(&errorCallback));

  //Compute number of points to compute and create a big tab to store points' information
  unsigned int numberOfPoints = 0;

  for(size_t v = 0 ; v < points.size() ; ++v) {
    numberOfPoints += points[v].size();
  }

  GLdouble *pointsData = new GLdouble[7*numberOfPoints];
  memset(pointsData, 0, 7*numberOfPoints*sizeof(GLdouble));

  unsigned int pointNumber=0;
  gluTessBeginPolygon(tobj, static_cast<void *>(this));

  for(size_t v = 0 ; v < points.size() ; ++v) {
    gluTessBeginContour(tobj);

    for(size_t i = 0 ; i < points[v].size() ; ++i) {
      pointsData[pointNumber*7]=points[v][i][0];
      pointsData[pointNumber*7+1]=points[v][i][1];
      pointsData[pointNumber*7+2]=points[v][i][2];
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
void GlComplexPolygon::addVertex(const Coord &vertexCoord, const Vec2f &vertexTexCoord) {
  verticesMap[currentPrimitive].push_back(vertexCoord);
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
void GlComplexPolygon::activateQuadBorder(const float borderWidth, const Color &color, const string &texture, const int position, const float texCoordFactor, const int polygonId) {
  if (static_cast<size_t>(polygonId) < quadBorderActivated.size()) {
    quadBorderActivated[polygonId] = true;
    quadBorderWidth[polygonId] = borderWidth;
    quadBorderColor[polygonId] = color;
    quadBorderTexture[polygonId] = texture;
    quadBorderPosition[polygonId] = position;
    quadBorderTexFactor[polygonId] = texCoordFactor;
  }
}
//=====================================================
void GlComplexPolygon::desactivateQuadBorder(const int polygonId) {
  if (static_cast<size_t>(polygonId) < quadBorderActivated.size()) {
    quadBorderActivated[polygonId] = false;
  }
}
//=====================================================
void GlComplexPolygon::draw(float,Camera *) {

  if(cameraIs3D()) {
    glEnable(GL_LIGHTING);
  }
  else {
    glDisable(GL_LIGHTING);
  }

  glDisable(GL_CULL_FACE);
  glEnable(GL_COLOR_MATERIAL);

  if(textureName!="") {
    if(GlTextureManager::getInst().activateTexture(textureName))
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  }

  glNormal3f(0.0f, 0.0f, 1.0f);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  setMaterial(fillColor);

  for (set<GLenum>::iterator it = primitivesSet.begin() ; it != primitivesSet.end() ; ++it) {
    glVertexPointer(3, GL_FLOAT, 3*sizeof(GLfloat), &verticesMap[*it][0][0]);
    glTexCoordPointer(2, GL_FLOAT, 2*sizeof(GLfloat), &texCoordsMap[*it][0]);

    for (size_t i = 0 ; i < verticesCountMap[*it].size() ; ++i) {
      glDrawArrays(*it, startIndicesMap[*it][i], verticesCountMap[*it][i]);
    }
  }

  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  if(textureName!="") {
    GlTextureManager::getInst().desactivateTexture();
  }

  if (outlined) {
    float lineWidth=outlineSize;

    if(lineWidth < 1e-6f)
      lineWidth=1e-6f;

    glLineWidth(lineWidth);
    setMaterial(outlineColor);

    for(size_t v = 0 ; v < points.size() ; ++v) {
      glVertexPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), &points[v][0]);
      glDrawArrays(GL_LINE_LOOP, 0, points[v].size());
    }
  }


  for (size_t v = 0 ; v < points.size() ; ++v) {

    if (quadBorderActivated[v]) {

      if (GlShaderProgram::shaderProgramsSupported() && GlShaderProgram::geometryShaderSupported()) {

        static GlShaderProgram *outlineExtrusionShader = NULL;

        if (!outlineExtrusionShader) {
          outlineExtrusionShader = new GlShaderProgram();
          outlineExtrusionShader->addShaderFromSourceCode(Vertex, outlineExtrusionVertexShaderSrc);
          outlineExtrusionShader->addGeometryShaderFromSourceCode(outlineExtrusionGeometryShaderSrc, GL_LINES_ADJACENCY_EXT, GL_TRIANGLE_STRIP);
          outlineExtrusionShader->link();
          outlineExtrusionShader->printInfoLog();
        }

        if (outlineExtrusionShader->isLinked()) {

          outlineExtrusionShader->activate();

          GLint vertexPosLoc = glGetAttribLocation(outlineExtrusionShader->getShaderProgramId(),"indice");
          glEnableVertexAttribArray(vertexPosLoc);

          if (quadBorderTexture[v] != "") {
            GlTextureManager::getInst().activateTexture(quadBorderTexture[v]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
          }

          setMaterial(quadBorderColor[v]);

          glVertexPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), &points[v][0]);
          glVertexAttribPointer(vertexPosLoc, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), &pointsIdx[v][0]);

          outlineExtrusionShader->setUniformInt("outlinePos", quadBorderPosition[v]);
          outlineExtrusionShader->setUniformFloat("size", quadBorderWidth[v]);
          outlineExtrusionShader->setUniformInt("nbVertices", points[v].size());
          outlineExtrusionShader->setUniformVec3Float("firstPoint", points[v][0]);
          outlineExtrusionShader->setUniformVec3Float("secondPoint", points[v][1]);
          outlineExtrusionShader->setUniformVec3Float("lastPoint", points[v][points[v].size() - 1]);
          outlineExtrusionShader->setUniformFloat("texFactor", quadBorderTexFactor[v]);

          glDrawArrays(GL_LINE_STRIP_ADJACENCY_EXT, 0, points[v].size());

          if (quadBorderTexture[v] != "") {
            GlTextureManager::getInst().desactivateTexture();
          }

          outlineExtrusionShader->desactivate();

        }
      }

    }

  }

  glDisableClientState(GL_VERTEX_ARRAY);

  glTest(__PRETTY_FUNCTION__);
}
//===========================================================
void GlComplexPolygon::translate(const Coord& vec) {
  boundingBox.translate(vec);

  for(vector<vector<Coord> >::iterator it = points.begin(); it != points.end(); ++it) {
    for(vector<Coord>::iterator it2 = (*it).begin(); it2 != (*it).end(); ++it2) {
      (*it2) += vec;
    }
  }

  runTesselation();
}
//===========================================================
void GlComplexPolygon::getXML(string &outString) {

  GlXMLTools::createProperty(outString,"type","GlComplexPolygon","GlEntity");

  getXMLOnlyData(outString);

}
//===========================================================
void GlComplexPolygon::getXMLOnlyData(string &outString) {


  GlXMLTools::getXML(outString,"numberOfVector",points.size());

  for (size_t i = 0 ; i < points.size() ; ++i) {
    stringstream str;
    str << i ;

    if(!points[i].empty()) {
      GlXMLTools::getXML(outString,"points"+str.str(),points[i]);
    }
    else {
      GlXMLTools::getXML(outString,"points"+str.str(),vector<Coord>());
    }
  }

  GlXMLTools::getXML(outString,"fillColor",fillColor);
  GlXMLTools::getXML(outString,"outlineColor",outlineColor);
  GlXMLTools::getXML(outString,"outlined",outlined);
  GlXMLTools::getXML(outString,"outlineSize",outlineSize);
  GlXMLTools::getXML(outString,"textureName",textureName);
}
//============================================================
void GlComplexPolygon::setWithXML(const string &inString,unsigned int &currentPosition) {

  int numberOfVector;
  GlXMLTools::setWithXML(inString,currentPosition,"numberOfVector",numberOfVector);

  for(int i=0; i<numberOfVector; ++i) {
    stringstream str;
    str << i ;
    points.push_back(vector<Coord>());
    GlXMLTools::setWithXML(inString,currentPosition,"points"+str.str(),points[i]);
  }

  GlXMLTools::setWithXML(inString,currentPosition,"fillColor",fillColor);
  GlXMLTools::setWithXML(inString,currentPosition,"outlineColor",outlineColor);
  GlXMLTools::setWithXML(inString,currentPosition,"outlined",outlined);
  GlXMLTools::setWithXML(inString,currentPosition,"outlineSize",outlineSize);
  GlXMLTools::setWithXML(inString,currentPosition,"textureName",textureName);

  for(vector<vector<Coord> >::iterator it= points.begin(); it!=points.end(); ++it) {
    for(vector<Coord>::iterator it2=(*it).begin(); it2!=(*it).end(); ++it2) {
      boundingBox.expand(*it2);
    }
  }
}
}
