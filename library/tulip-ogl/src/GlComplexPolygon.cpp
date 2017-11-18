/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

#include <GL/glew.h>

// libtess2
#include <tesselator.h>

#include <tulip/GlComplexPolygon.h>
#include <tulip/GlTools.h>
#include <tulip/GlTextureManager.h>
#include <tulip/ParametricCurves.h>
#include <tulip/GlShaderProgram.h>
#include <tulip/GlXMLTools.h>

using namespace std;

// clang-format off

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

// clang-format on

const int nbFloatPerVertex = 5;

namespace tlp {

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
//=====================================================
void GlComplexPolygon::runTesselation() {
  verticesData.clear();
  verticesIndices.clear();
  // instantiate the tesselator from libtess2
  TESStesselator* tess = tessNewTess(NULL);

  // add contours
  for (size_t i = 0 ; i < points.size() ; ++i) {
    tessAddContour(tess, 3, &points[i][0], sizeof(float)*3, points[i].size());
  }

  // the tesselation will generate a set of polygons with maximum nvp vertices
  const int nvp = 6;

  // run tesselation with the same default winding rule as in the GLU tesselator
  if (tessTesselate(tess, TESS_WINDING_ODD, TESS_POLYGONS, nvp, 3, 0)) {
    const float* verts = tessGetVertices(tess);
    const int* elems = tessGetElements(tess);
    const int nelems = tessGetElementCount(tess);
    std::map<Coord, unsigned int> vidx;

    // iterate over polygons computed by tesselation
    for (int i = 0; i < nelems; ++i) {
      std::vector<tlp::Coord> verticesTmp;
      const int* p = &elems[i*nvp];

      for (int j = 0; j < nvp && p[j] != TESS_UNDEF; ++j) {
        int idxx = p[j]*3;
        int idxy = p[j]*3+1;
        int idxz = p[j]*3+2;
        Coord p(verts[idxx], verts[idxy], verts[idxz]);
        verticesTmp.push_back(p);

        // if we did not encounter the vertex so far, add it in the verticesData vector
        if (vidx.find(p) == vidx.end()) {
          vidx[p] = verticesData.size() / nbFloatPerVertex;
          verticesData.push_back(p[0]); // x
          verticesData.push_back(p[1]); // y
          verticesData.push_back(p[2]); // z
          verticesData.push_back(((p[0] - boundingBox[0][0]) / boundingBox.width()) / textureZoom); // s
          verticesData.push_back(((p[1] - boundingBox[0][1]) / boundingBox.height()) / textureZoom); // t
        }
      }

      // transform the polygon to a list of triangles
      Coord centerPoint = verticesTmp[0];

      for (size_t j = 1 ; j < verticesTmp.size() - 1 ; ++j) {
        verticesIndices.push_back(vidx[centerPoint]);
        verticesIndices.push_back(vidx[verticesTmp[j]]);
        verticesIndices.push_back(vidx[verticesTmp[j+1]]);
      }
    }
  }

  // free memory allocated by the tesselator from libtess2
  tessDeleteTess(tess);
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

  if(!textureName.empty()) {
    if(GlTextureManager::getInst().activateTexture(textureName))
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  }

  glNormal3f(0.0f, 0.0f, 1.0f);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  setMaterial(fillColor);

  glVertexPointer(3, GL_FLOAT, nbFloatPerVertex*sizeof(GLfloat), &verticesData[0]);
  glTexCoordPointer(2, GL_FLOAT, nbFloatPerVertex*sizeof(GLfloat), &verticesData[3]);
  glDrawElements(GL_TRIANGLES, verticesIndices.size(), GL_UNSIGNED_INT, &verticesIndices[0]);

  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  if(!textureName.empty()) {
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

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
