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
#include <tulip/GlGrid.h>
#include <tulip/GlXMLTools.h>
#include <tulip/GlTools.h>

using namespace std;

namespace tlp {
//============================================================
GlGrid::GlGrid(const Coord& frontTopLeft, const Coord& backBottomRight,
               const Size& cell, const Color& color, bool displayDim[3]) :
  frontTopLeft(frontTopLeft),
  backBottomRight(backBottomRight),
  color(color),
  cell(cell) {
  for(unsigned int i=0; i < 3; ++i)
    this->displayDim[i] = displayDim[i];

  boundingBox.expand(frontTopLeft);
  boundingBox.expand(backBottomRight);
}
//============================================================
void GlGrid::draw(float,Camera *) {
  Coord delta = backBottomRight - frontTopLeft;
  Coord gridSize = backBottomRight - frontTopLeft;
  delta /= gridSize / cell;

  glDisable(GL_COLOR_MATERIAL);
  setMaterial(color);

  glLineWidth(1);
  glBegin(GL_LINES);

  //xy-plane
  if (displayDim[0]) {
    Coord A(frontTopLeft);

    while (A[0] <= backBottomRight[0] + 1E-3) {
      glVertex3f(A[0], A[1], A[2]);
      glVertex3f(A[0], A[1] + gridSize[1] , A[2]);
      A[0] += delta[0];
    }

    A = frontTopLeft;

    while (A[1] <= backBottomRight[1] + 1E-3) {
      glVertex3f(A[0], A[1], A[2]);
      glVertex3f(A[0] + gridSize[0], A[1]  , A[2]);
      A[1] += delta[1];
    }
  }

  //yz-plane
  if (displayDim[1]) {
    Coord A(frontTopLeft);

    while (A[2] <= backBottomRight[2] + 1E-3) {
      glVertex3f(A[0], A[1], A[2]);
      glVertex3f(A[0], A[1] + gridSize[1] , A[2]);
      A[2] += delta[2];
    }

    A = frontTopLeft;

    while (A[1] <= backBottomRight[1] + 1E-3) {
      glVertex3f(A[0], A[1], A[2]);
      glVertex3f(A[0], A[1] , A[2] + gridSize[2]);
      A[1] += delta[1];
    }
  }

  //xz-plane
  if (displayDim[2]) {
    Coord A(frontTopLeft);

    while (A[2] <= backBottomRight[2] + 1E-3) {
      glVertex3f(A[0], A[1], A[2]);
      glVertex3f(A[0]+ gridSize[0], A[1]  , A[2]);
      A[2] += delta[2];
    }

    A = frontTopLeft;

    while (A[0] <= backBottomRight[0] + 1E-3) {
      glVertex3f(A[0], A[1], A[2]);
      glVertex3f(A[0], A[1], A[2] + gridSize[2]);
      A[0] += delta[0];
    }
  }

  glEnd();
}
//============================================================
void GlGrid::getDisplayDim(bool displayDim[3]) const {
  for(int i=0; i < 3; ++i)
    displayDim[i] = this->displayDim[i];
}
//============================================================
void GlGrid::setDisplayDim(bool displayDim[3]) {
  for(int i=0; i < 3; i++)
    this->displayDim[i] = displayDim[i];
}
//============================================================
void GlGrid::translate(const Coord& vec) {
  boundingBox[0]  += vec;
  boundingBox[1]  += vec;
  frontTopLeft    += vec;
  backBottomRight += vec;
}
//============================================================
void GlGrid::getXML(string &outString) {

  GlXMLTools::createProperty(outString,"type","GlGrid","GlEntity");


  GlXMLTools::getXML(outString,"displayDim0",displayDim[0]);
  GlXMLTools::getXML(outString,"displayDim1",displayDim[1]);
  GlXMLTools::getXML(outString,"displayDim2",displayDim[2]);
  GlXMLTools::getXML(outString,"frontTopLeft",frontTopLeft);
  GlXMLTools::getXML(outString,"backBottomRight",backBottomRight);
  GlXMLTools::getXML(outString,"color",color);
  GlXMLTools::getXML(outString,"cell",cell);

}
//====================================================
void GlGrid::setWithXML(const string &inString,unsigned int &currentPosition) {

  GlXMLTools::setWithXML(inString,currentPosition,"displayDim0",displayDim[0]);
  GlXMLTools::setWithXML(inString,currentPosition,"displayDim1",displayDim[1]);
  GlXMLTools::setWithXML(inString,currentPosition,"displayDim2",displayDim[2]);
  GlXMLTools::setWithXML(inString,currentPosition,"frontTopLeft",frontTopLeft);
  GlXMLTools::setWithXML(inString,currentPosition,"backBottomRight",backBottomRight);
  GlXMLTools::setWithXML(inString,currentPosition,"color",color);
  GlXMLTools::setWithXML(inString,currentPosition,"cell",cell);

  GlGrid(frontTopLeft, backBottomRight,cell, color, displayDim);
}
}
