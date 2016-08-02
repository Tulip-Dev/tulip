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

#include <tulip/GlLine.h>
#include <tulip/GlBuffer.h>
#include <tulip/GlShaderProgram.h>
#include <tulip/GlUtils.h>
#include <tulip/ShaderManager.h>
#include <tulip/Camera.h>

using namespace std;

namespace tlp {

static vector<Color> getColorVector(unsigned int size, const Color &c) {
  vector<Color> ret;
  ret.resize(size, c);
  return ret;
}

GlLine::GlLine(const vector<Coord> &points, const vector<Color> &colors):
  _points(points),_colors(colors),_width(1.0){

  for(vector<Coord>::iterator it= _points.begin(); it!=_points.end(); ++it)
    _boundingBox.expand(*it);
}
//=====================================================
GlLine::GlLine(const vector<Coord> &points, const Color &color):
  GlLine(points, getColorVector(points.size(), color)) {}
//=====================================================
GlLine::~GlLine() {
}
//=====================================================
void GlLine::resizePoints(const unsigned int nbPoints) {
  _points.resize(nbPoints);
}
//=====================================================
void GlLine::resizeColors(const unsigned int nbColors) {
  assert(nbColors >= 1);
  _points.resize(nbColors);
}
//=====================================================
const tlp::Coord& GlLine::point(const unsigned int i) const {
  return _points[i];
}
//=====================================================
tlp::Coord& GlLine::point(const unsigned int i) {
  return _points[i];
}
//=====================================================
void GlLine::addPoint(const Coord& point,const Color& color) {
  _points.push_back(point);
  _colors.push_back(color);
  _boundingBox.expand(point);
}
//=====================================================
const tlp::Color& GlLine::color(const unsigned int i) const {
  return _colors[i];
}
//=====================================================
tlp::Color& GlLine::color(const unsigned int i) {
  return _colors[i];
}
//=====================================================

const int lineRenderingDataStride = 16 * sizeof(float);

void GlLine::draw(const Camera &camera, const Light &, bool pickingMode) {
  assert(_points.size() == _colors.size());
  vector<float> lineRenderingData;
  auto packVertexData = [&](unsigned int i, float direction) {
    addTlpVecToVecFloat(_points[i], lineRenderingData);
    lineRenderingData.push_back(direction);
    if (i < _points.size() - 1) {
      addTlpVecToVecFloat(_points[i+1], lineRenderingData);
    } else {
      addTlpVecToVecFloat(_points[i], lineRenderingData);
    }
    if (i > 0) {
      addTlpVecToVecFloat(_points[i-1], lineRenderingData);
    } else {
      addTlpVecToVecFloat(_points[i], lineRenderingData);
    }
    if (pickingMode) {
      addColorToVecFloat(_pickingColor, lineRenderingData);
    } else {
      addColorToVecFloat(_colors[i], lineRenderingData);
    }
    addTlpVecToVecFloat(Vec2f(0,0), lineRenderingData);
  };

  for (size_t i = 0 ; i < _points.size() ; ++i) {
    packVertexData(i, -1);
    packVertexData(i, 1);
  }

  GlBuffer lineRenderingDataBuffer(GlBuffer::VertexBuffer);
  lineRenderingDataBuffer.bind();
  lineRenderingDataBuffer.allocate(lineRenderingData);

  const Vec4i viewport = camera.getViewport();

  GlShaderProgram *lineRenderingShader = ShaderManager::getInstance()->getLineRenderingShader();
  lineRenderingShader->activate();
  lineRenderingShader->setUniformMat4Float("u_modelviewMatrix", camera.modelviewMatrix());
  lineRenderingShader->setUniformMat4Float("u_projectionMatrix", camera.projectionMatrix());
  lineRenderingShader->setUniformBool("u_textureActivated", false);
  lineRenderingShader->setUniformFloat("u_aspect", viewport[2]/float(viewport[3]));
  lineRenderingShader->setUniformFloat("u_thickness", _width/100.f);
  lineRenderingShader->setUniformBool("u_miter", true);
  lineRenderingShader->setVertexAttribPointer("a_position", 3, GL_FLOAT, GL_FALSE, lineRenderingDataStride, BUFFER_OFFSET(0));
  lineRenderingShader->setVertexAttribPointer("a_direction", 1, GL_FLOAT, GL_FALSE, lineRenderingDataStride, BUFFER_OFFSET(3*sizeof(float)));
  lineRenderingShader->setVertexAttribPointer("a_next", 3, GL_FLOAT, GL_FALSE, lineRenderingDataStride, BUFFER_OFFSET(4*sizeof(float)));
  lineRenderingShader->setVertexAttribPointer("a_previous", 3, GL_FLOAT, GL_FALSE, lineRenderingDataStride, BUFFER_OFFSET(7*sizeof(float)));
  lineRenderingShader->setVertexAttribPointer("a_color", 4, GL_FLOAT, GL_FALSE, lineRenderingDataStride, BUFFER_OFFSET(10*sizeof(float)));
  lineRenderingShader->setVertexAttribPointer("a_texCoord", 2, GL_FLOAT, GL_FALSE, lineRenderingDataStride, BUFFER_OFFSET(14*sizeof(float)));
  glDrawArrays(GL_TRIANGLE_STRIP, 0, _points.size()*2);
  lineRenderingShader->desactivate();
  lineRenderingDataBuffer.release();
}
//=====================================================
void GlLine::draw(const Camera &camera, bool pickingMode) {
  GlEntity::draw(camera, pickingMode);
}

//=====================================================
void GlLine::setLineWidth(float width) {
  _width=width;
}

}
