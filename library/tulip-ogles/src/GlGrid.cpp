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
 * Copyright (c) 2016 Antoine Lambert, Thales Services SAS
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#include <tulip/GlGrid.h>
#include <tulip/GlBuffer.h>
#include <tulip/ShaderManager.h>
#include <tulip/GlShaderProgram.h>
#include <tulip/Camera.h>

using namespace std;

namespace tlp {
//============================================================
GlGrid::GlGrid(const Coord &frontTopLeft, const Coord &backBottomRight, const Size &cell, const Color &color, bool displayDim[3])
    : frontTopLeft(frontTopLeft), backBottomRight(backBottomRight), color(color), cell(cell) {
  for (unsigned int i = 0; i < 3; ++i)
    this->displayDim[i] = displayDim[i];

  _boundingBox.expand(frontTopLeft);
  _boundingBox.expand(backBottomRight);
}
//============================================================
void GlGrid::draw(const Camera &camera, const Light &, bool pickingMode) {
  Coord delta = backBottomRight - frontTopLeft;
  Coord gridSize = backBottomRight - frontTopLeft;
  delta /= gridSize / cell;

  glLineWidth(1);

  vector<Coord> gridVertices;

  // xy-plane
  if (displayDim[0]) {
    Coord A(frontTopLeft);

    while (A[0] <= backBottomRight[0] + 1E-3) {
      gridVertices.push_back(Coord(A[0], A[1], A[2]));
      gridVertices.push_back(Coord(A[0], A[1] + gridSize[1], A[2]));
      A[0] += delta[0];
    }

    A = frontTopLeft;

    while (A[1] <= backBottomRight[1] + 1E-3) {
      gridVertices.push_back(Coord(A[0], A[1], A[2]));
      gridVertices.push_back(Coord(A[0] + gridSize[0], A[1], A[2]));
      A[1] += delta[1];
    }
  }

  // yz-plane
  if (displayDim[1]) {
    Coord A(frontTopLeft);

    while (A[2] <= backBottomRight[2] + 1E-3) {
      gridVertices.push_back(Coord(A[0], A[1], A[2]));
      gridVertices.push_back(Coord(A[0], A[1] + gridSize[1], A[2]));
      A[2] += delta[2];
    }

    A = frontTopLeft;

    while (A[1] <= backBottomRight[1] + 1E-3) {
      gridVertices.push_back(Coord(A[0], A[1], A[2]));
      gridVertices.push_back(Coord(A[0], A[1], A[2] + gridSize[2]));
      A[1] += delta[1];
    }
  }

  // xz-plane
  if (displayDim[2]) {
    Coord A(frontTopLeft);

    while (A[2] <= backBottomRight[2] + 1E-3) {
      gridVertices.push_back(Coord(A[0], A[1], A[2]));
      gridVertices.push_back(Coord(A[0] + gridSize[0], A[1], A[2]));
      A[2] += delta[2];
    }

    A = frontTopLeft;

    while (A[0] <= backBottomRight[0] + 1E-3) {
      gridVertices.push_back(Coord(A[0], A[1], A[2]));
      gridVertices.push_back(Coord(A[0], A[1], A[2] + gridSize[2]));
      A[0] += delta[0];
    }
  }

  GlShaderProgram *shader = ShaderManager::getInstance()->getFlatRenderingShader();
  shader->activate();
  shader->setUniformMat4Float("u_projectionMatrix", camera.projectionMatrix());
  shader->setUniformMat4Float("u_modelviewMatrix", camera.modelviewMatrix());
  shader->setUniformBool("u_globalColor", true);
  shader->setUniformBool("u_pointsRendering", false);

  GlBuffer verticesBuffer(GlBuffer::VertexBuffer);
  verticesBuffer.bind();
  verticesBuffer.allocate(gridVertices);

  shader->setVertexAttribPointer("a_position", 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), BUFFER_OFFSET(0));
  shader->setUniformBool("u_textureActivated", false);

  if (!pickingMode) {
    shader->setUniformColor("u_color", color);
  } else {
    shader->setUniformColor("u_color", _pickingColor);
  }

  glDrawArrays(GL_LINES, 0, gridVertices.size());
}
//============================================================
void GlGrid::getDisplayDim(bool displayDim[3]) const {
  for (int i = 0; i < 3; ++i)
    displayDim[i] = this->displayDim[i];
}
//============================================================
void GlGrid::setDisplayDim(bool displayDim[3]) {
  for (int i = 0; i < 3; i++)
    this->displayDim[i] = displayDim[i];
}
//============================================================
void GlGrid::translate(const Coord &vec) {
  _boundingBox[0] += vec;
  _boundingBox[1] += vec;
  frontTopLeft += vec;
  backBottomRight += vec;
}
}
