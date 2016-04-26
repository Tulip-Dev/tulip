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

#include <tulip/GlConvexPolygon.h>
#include <tulip/ShaderManager.h>
#include <tulip/GlShaderProgram.h>
#include <tulip/GlBuffer.h>
#include <tulip/Camera.h>
#include <tulip/GlUtils.h>
#include <tulip/GlTextureManager.h>

#include <tulip/Matrix.h>
#include <tulip/DrawingTools.h>

using namespace tlp;
using namespace std;

GlConvexPolygon::GlConvexPolygon() :
  _filled(true), _outlined(true), _fillColor(Color::Red), _outlineColor(Color::Black), _outlineWidth(1.f) {}

GlConvexPolygon::GlConvexPolygon(const std::vector<tlp::Coord> &contour, const tlp::Color &fillColor) :
  _filled(true), _outlined(false), _fillColor(fillColor), _outlineColor(Color::Black), _outlineWidth(1.f) {
  setPolygonContour(contour);
}

GlConvexPolygon::GlConvexPolygon(const std::vector<tlp::Coord> &contour, const tlp::Color &fillColor, const tlp::Color &outlineColor, const float outlineWidth) :
  _filled(true), _outlined(true), _fillColor(fillColor), _outlineColor(outlineColor), _outlineWidth(outlineWidth) {
  setPolygonContour(contour);
}

GlConvexPolygon::~GlConvexPolygon() {
  delete _polygonDataBuffer;
  delete _polygonIndicesBuffer;
}

void GlConvexPolygon::setPolygonContour(const std::vector<tlp::Coord> &contour) {
  assert(contour.size() > 2);
  _contour = contour;
  _needRenderingDataUpload = true;
}

void GlConvexPolygon::uploadRenderingData() {
  if (!_needRenderingDataUpload) {
    return;
  }
  Mat3f invTransformMatrix;

  vector<Coord> polygonVertices;
  vector<unsigned short> polygonIndices;

  _boundingBox = BoundingBox();
  polygonVertices = _contour;
  vector<Vec2f> projectedVertices;
  for(size_t i = 0 ; i < _contour.size() ; ++i) {
    Coord p = Coord(invTransformMatrix * _contour[i]);
    projectedVertices.push_back(Vec2f(p[0], p[1]));
    _boundingBox.expand(_contour[i]);
  }

  if (clockwiseOrdering(projectedVertices)) {
    std::reverse(projectedVertices.begin(), projectedVertices.end());
    std::reverse(polygonVertices.begin(), polygonVertices.end());
  }

  Coord centroid = computePolygonCentroid(polygonVertices);
  polygonVertices.insert(polygonVertices.begin(), centroid);

  for (size_t i = 0 ; i < _contour.size()-1 ; ++i) {
    polygonIndices.push_back(0);
    polygonIndices.push_back(i+1);
    polygonIndices.push_back(i+2);
  }
  polygonIndices.push_back(0);
  polygonIndices.push_back(_contour.size());
  polygonIndices.push_back(1);

  _nbVertices = polygonVertices.size();
  _nbIndices = polygonIndices.size();

  vector<float> polygonData;

  for(size_t i = 0 ; i < polygonVertices.size() ; ++i) {
    addTlpVecToVecFloat(polygonVertices[i], polygonData);
    addTlpVecToVecFloat(Vec2f((polygonVertices[i][0] - _boundingBox[0][0])/_boundingBox.width(), (polygonVertices[i][1] - _boundingBox[0][1])/_boundingBox.height()), polygonData);
  }

  if (!_polygonDataBuffer) {
    _polygonDataBuffer = new GlBuffer(GlBuffer::VertexBuffer);
    _polygonIndicesBuffer = new GlBuffer(GlBuffer::IndexBuffer);
  }

  _polygonDataBuffer->allocate(polygonData);
  _polygonIndicesBuffer->allocate(polygonIndices);

  _needRenderingDataUpload = false;
}

void GlConvexPolygon::draw(const Camera &camera, bool pickingMode) {
  GlEntity::draw(camera, pickingMode);
}

void GlConvexPolygon::draw(const Camera &camera, const Light &, bool pickingMode) {

  uploadRenderingData();

  GlTextureManager::instance()->addTextureFromFile(_texture);
  GlTextureManager::instance()->bindTexture(_texture);

  GlShaderProgram *shader = ShaderManager::getInstance()->getFlatRenderingShader();
  shader->activate();
  shader->setUniformMat4Float("u_projectionMatrix", camera.projectionMatrix());
  shader->setUniformMat4Float("u_modelviewMatrix", camera.modelviewMatrix());
  shader->setUniformBool("u_globalColor", true);
  shader->setUniformBool("u_pointsRendering", false);

  _polygonDataBuffer->bind();
  shader->setVertexAttribPointer("a_position", 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), BUFFER_OFFSET(0));

  if (!_texture.empty() && !pickingMode) {
    shader->setUniformBool("u_textureActivated", true);
    shader->setUniformBool("u_globalTexture", true);
    shader->setUniformTextureSampler("u_texture", GlTextureManager::instance()->getSamplerIdForTexture(_texture));
    shader->setUniformVec4Float("u_texCoordOffsets", GlTextureManager::instance()->getCoordinatesOffsetsForTexture(_texture));
    shader->setVertexAttribPointer("a_texCoord", 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
  } else {
    shader->setUniformBool("u_textureActivated", false);
  }

  if (_filled) {
    if (!pickingMode) {
      shader->setUniformColor("u_color", _fillColor);
    } else {
      shader->setUniformColor("u_color", _pickingColor);
    }
    _polygonIndicesBuffer->bind();
    glDrawElements(GL_TRIANGLES, _nbIndices, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
    _polygonIndicesBuffer->release();
  }

  shader->setUniformBool("u_textureActivated", false);

  if (_outlined && _outlineWidth > 0.f) {
    if (!pickingMode) {
      shader->setUniformColor("u_color", _outlineColor);
    } else {
      shader->setUniformColor("u_color", _pickingColor);
    }
    glLineWidth(_outlineWidth);
    glDrawArrays(GL_LINE_LOOP, 1, _nbVertices-1);
  }
  _polygonDataBuffer->release();

  GlTextureManager::instance()->unbindTexture(_texture);
}

void GlConvexPolygon::translate(const Coord &move) {
  for (size_t i = 0 ; i < _contour.size() ; ++i) {
    _contour[i] += move;
  }
  setPolygonContour(_contour);
  notifyModified();
}


