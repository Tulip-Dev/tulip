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

/**
 *
 * tulip-ogles library : rewrite of old tulip-ogl library using OpenGL ES API
 * Copyright (c) 2016 Antoine Lambert, Thales Services
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#include <GL/glew.h>

#include <tesselator.h>

#include <tulip/GlConcavePolygon.h>
#include <tulip/ShaderManager.h>
#include <tulip/GlShaderProgram.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlUtils.h>

typedef tlp::Vector<float,  5, double> Vec5f;

using namespace std;
using namespace tlp;

GlConcavePolygon::GlConcavePolygon(const vector<Coord> &coords,Color fcolor, const string &textureName):
  _currentVector(-1),
  _outlined(false),
  _fillColor(fcolor),
  _outlineSize(1),
  _polygonVerticesDataVBO(nullptr),
  _polygonIndicesVBO(nullptr),
  _nbIndices(0),
  _texture(textureName) {
  createPolygon(coords);
  runTesselation();
}
//=====================================================
GlConcavePolygon::GlConcavePolygon(const vector<Coord> &coords,Color fcolor,Color ocolor,const string &textureName):
  _currentVector(-1),
  _outlined(true),
  _fillColor(fcolor),
  _outlineColor(ocolor),
  _outlineSize(1),
  _polygonVerticesDataVBO(nullptr),
  _polygonIndicesVBO(nullptr),
  _nbIndices(0),
  _texture(textureName) {
  if (!coords.empty()) {
    createPolygon(coords);
    runTesselation();
  }
}
//=====================================================
GlConcavePolygon::GlConcavePolygon(const vector<vector<Coord> >&coords,Color fcolor,const string &textureName):
  _currentVector(-1),
  _outlined(false),
  _fillColor(fcolor),
  _outlineSize(1),
  _polygonVerticesDataVBO(nullptr),
  _polygonIndicesVBO(nullptr),
  _nbIndices(0),
  _texture(textureName) {
  for (size_t i = 0 ; i < coords.size() ; ++i) {
    createPolygon(coords[i]);
  }

  runTesselation();
}
//=====================================================
GlConcavePolygon::GlConcavePolygon(const vector<vector<Coord> >&coords,Color fcolor,Color ocolor,const string &textureName):
  _currentVector(-1),
  _outlined(true),
  _fillColor(fcolor),
  _outlineColor(ocolor),
  _outlineSize(1),
  _polygonVerticesDataVBO(nullptr),
  _polygonIndicesVBO(nullptr),
  _nbIndices(0),
  _texture(textureName) {
  for(unsigned int i=0; i<coords.size(); ++i) {
    createPolygon(coords[i]);
  }
  runTesselation();
}
//=====================================================
GlConcavePolygon::~GlConcavePolygon() {
  for (size_t i = 0 ; i < _pointsVBOs.size() ; ++i) {
    delete _pointsVBOs[i];
  }
}

//=====================================================
void GlConcavePolygon::createPolygon(const vector<Coord> &coords) {
  beginNewHole();

  for(vector<Coord>::const_iterator it=coords.begin(); it!=coords.end(); ++it) {
    addPoint(*it);
  }

  GlBuffer *pointsVBO = new GlBuffer(GlBuffer::VertexBuffer);
  pointsVBO->allocate(_points[_currentVector]);
  _pointsVBOs.push_back(pointsVBO);

}
//=====================================================
void GlConcavePolygon::setOutlineMode(const bool outlined) {
  this->_outlined = outlined;
}
//=====================================================
void GlConcavePolygon::setOutlineSize(double size) {
  _outlineSize=size;
}
//=====================================================
void GlConcavePolygon::addPoint(const Coord& point) {
  _points[_currentVector].push_back(point);
  _boundingBox.expand(point);
}
//=====================================================
void GlConcavePolygon::beginNewHole() {
  ++_currentVector;
  _points.push_back(vector<Coord>());
}
//=====================================================
void GlConcavePolygon::runTesselation() {
  TESStesselator* tess = tessNewTess(nullptr);
  for (size_t i = 0 ; i < _points.size() ; ++i) {
    tessAddContour(tess, 3, &_points[i][0], sizeof(float)*3, _points[i].size());
  }
  const int nvp = 6;
  if (tessTesselate(tess, TESS_WINDING_ODD, TESS_POLYGONS, nvp, 3, 0)) {
    const float* verts = tessGetVertices(tess);
    const int* elems = tessGetElements(tess);
    const int nelems = tessGetElementCount(tess);
    std::vector<Vec5f> vertices;
    std::vector<unsigned int> indices;
    std::map<Coord, unsigned int> vidx;
    for (int i = 0; i < nelems; ++i) {
      std::vector<tlp::Coord> verticesTmp;
      const int* p = &elems[i*nvp];
      for (int j = 0; j < nvp && p[j] != TESS_UNDEF; ++j) {
        int idxx = p[j]*3;
        int idxy = p[j]*3+1;
        int idxz = p[j]*3+2;
        Coord p(verts[idxx], verts[idxy], verts[idxz]);
        verticesTmp.push_back(p);
        if (vidx.find(p) == vidx.end()) {
          vidx[p] = vertices.size();
          Vec5f v;
          v[0] = p[0];
          v[1] = p[1];
          v[2] = p[2];
          v[3] = (p[0] - _boundingBox[0][0]) / _boundingBox.width();
          v[4] = (p[1] - _boundingBox[0][1]) / _boundingBox.height();
          vertices.push_back(v);
        }
      }
      Coord centerPoint = verticesTmp[0];
      for (size_t j = 1 ; j < verticesTmp.size() - 1 ; ++j) {
        indices.push_back(vidx[centerPoint]);
        indices.push_back(vidx[verticesTmp[j]]);
        indices.push_back(vidx[verticesTmp[j+1]]);
      }
    }

    delete _polygonVerticesDataVBO;
    _polygonVerticesDataVBO = new GlBuffer(GlBuffer::VertexBuffer);
    _polygonVerticesDataVBO->allocate(vertices);

    delete _polygonIndicesVBO;
    _polygonIndicesVBO = new GlBuffer(GlBuffer::IndexBuffer);
    _polygonIndicesVBO->allocate(indices);
    _nbIndices = indices.size();
  }
  tessDeleteTess(tess);
}
//=====================================================
void GlConcavePolygon::draw(const Camera &camera, const Light &, bool pickingMode) {


  GlShaderProgram *shader = ShaderManager::getInstance()->getFlatRenderingShader();
  shader->activate();
  shader->setUniformMat4Float("u_projectionMatrix", camera.projectionMatrix());
  shader->setUniformMat4Float("u_modelviewMatrix", camera.modelviewMatrix());
  shader->setUniformBool("u_globalColor", true);
  if (pickingMode) {
    shader->setUniformColor("u_color", _pickingColor);
  } else {
    shader->setUniformColor("u_color", _fillColor);
  }
  shader->setUniformBool("u_globalTexture", true);
  shader->setUniformBool("u_textureActivated", !_texture.empty());

  if (!_texture.empty()) {
    GlTextureManager::instance()->addTextureFromFile(_texture);
    GlTextureManager::instance()->bindTexture(_texture);
    shader->setUniformTextureSampler("u_texture", GlTextureManager::instance()->getSamplerIdForTexture(_texture));
    shader->setUniformVec4Float("u_texCoordOffsets", GlTextureManager::instance()->getCoordinatesOffsetsForTexture(_texture));
  } else {
    shader->setUniformBool("u_textureActivated", false);
  }

  if (_nbIndices > 0) {
    _polygonVerticesDataVBO->bind();
    shader->setVertexAttribPointer("a_position", 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), BUFFER_OFFSET(0));
    shader->setVertexAttribPointer("a_texCoord", 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), BUFFER_OFFSET(3*sizeof(float)));
    _polygonIndicesVBO->bind();
    glDrawElements(GL_TRIANGLES, _nbIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    _polygonIndicesVBO->release();
  }

  shader->setUniformBool("u_textureActivated", false);
  shader->disableAttributesArrays();

  if (_outlined) {
    float lineWidth=_outlineSize;

    if(lineWidth < 1e-6f)
      lineWidth=1e-6f;

    glLineWidth(lineWidth);
    if (!pickingMode) {
      shader->setUniformColor("u_color", _outlineColor);
    } else {
      shader->setUniformColor("u_color", _pickingColor);
    }

    for(size_t v = 0 ; v < _points.size() ; ++v) {
      _pointsVBOs[v]->bind();
      shader->setVertexAttribPointer("a_position", 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
      glDrawArrays(GL_LINE_LOOP, 0, _points[v].size());
    }
  }

  GlBuffer::release(GlBuffer::VertexBuffer);
  GlTextureManager::instance()->unbindTexture(_texture);

  shader->desactivate();
}

void GlConcavePolygon::draw(const Camera &camera, bool pickingMode) {
  GlEntity::draw(camera, pickingMode);
}
//===========================================================
void GlConcavePolygon::translate(const Coord& vec) {

  for(vector<vector<Coord> >::iterator it = _points.begin(); it != _points.end(); ++it) {
    for(vector<Coord>::iterator it2 = (*it).begin(); it2 != (*it).end(); ++it2) {
      (*it2) += vec;
    }
  }
  _boundingBox[0] += vec;
  _boundingBox[1] += vec;

  runTesselation();
}


