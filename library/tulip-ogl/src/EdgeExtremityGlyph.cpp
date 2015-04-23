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
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/Glyph.h>
#include <tulip/Vector.h>

using namespace tlp;

EdgeExtremityGlyph::EdgeExtremityGlyph(const PluginContext* context) : edgeExtGlGraphInputData(NULL) {
  if(context != NULL) {
    const GlyphContext* glyphContext = dynamic_cast<const GlyphContext*>(context);
    assert(glyphContext != NULL);
    edgeExtGlGraphInputData = glyphContext->glGraphInputData;
  }
}

EdgeExtremityGlyph::~EdgeExtremityGlyph() {

}

void EdgeExtremityGlyph::get3DTransformationMatrix(const Coord &src, const Coord &dest, const Size &glyphSize, MatrixGL& transformationMatrix, MatrixGL& scalingMatrix) {
  //Vecteur AB
  Vector<float, 3> vAB;
  //Vecteur V
  Vector<float, 3> vV;
  //Vecteur W
  Vector<float, 3> vW;

  vAB = dest - src;
  float nAB; //|AB|
  nAB = vAB.norm();

  if (fabs(nAB) > 1E-6)
    vAB /= nAB;

  //vAB * vV = xAB * xV + yAB*yV + zAB * zV = |AB| * |V| * cos(alpha) = 0;
  if (fabs(vAB[2]) < 1E-6) {
    vV[0] = 0;
    vV[1] = 0;
    vV[2] = 1.0;
  }
  else if (fabs(vAB[1]) < 1E-6) {
    vV[0] = 0;
    vV[1] = 1.0;
    vV[2] = 0;
  }
  else {
    vV[0] = 0;
    vV[1] = 1. / vAB[1];
    vV[2] = -1. / vAB[2];
    vV /= vV.norm();
  }

  vW = vAB ^ vV;
  float nW = vW.norm();

  if (fabs(nW) > 1E-6)
    vW /= nW;

  for (unsigned int i = 0; i < 3; ++i) {
    transformationMatrix[0][i] = vW[i];
    transformationMatrix[1][i] = vV[i];
    transformationMatrix[2][i] = vAB[i];
    transformationMatrix[3][i] = dest[i] + (-glyphSize[0] * .5 * vAB[i]);
  }

  transformationMatrix[0][3] = 0;
  transformationMatrix[1][3] = 0;
  transformationMatrix[2][3] = 0;
  transformationMatrix[3][3] = 1;

  scalingMatrix.fill(0);
  scalingMatrix[0][0] = glyphSize[1];
  scalingMatrix[1][1] = glyphSize[2];
  scalingMatrix[2][2] = glyphSize[0];
  scalingMatrix[3][3] = 1;
}

void EdgeExtremityGlyph::get2DTransformationMatrix(const Coord &src, const Coord &dest, const Size &glyphSize, MatrixGL& transformationMatrix, MatrixGL& scalingMatrix) {
  //  Vecteur AB
  Vector<float, 3> vAB;
  //Vecteur V
  Vector<float, 3> vV;
  //Vecteur W
  Vector<float, 3> vW;

  vAB = dest - src;
  float nAB; //|AB|
  nAB = vAB.norm();

  if (fabs(nAB) > 1E-6)
    vAB /= nAB;

  //vAB * vV = xAB * xV + yAB*yV + zAB * zV = |AB| * |V| * cos(alpha) = 0;
  if (fabs(vAB[2]) < 1E-6) {
    vV[0] = 0;
    vV[1] = 0;
    vV[2] = 1.0;
  }
  else if (fabs(vAB[1]) < 1E-6) {
    vV[0] = 0;
    vV[1] = 1.0;
    vV[2] = 0;
  }
  else {
    vV[0] = 0;
    vV[1] = 1. / vAB[1];
    vV[2] = -1. / vAB[2];
    vV /= vV.norm();
  }

  vW = vAB ^ vV;
  float nW = vW.norm();

  if (fabs(nW) > 1E-6)
    vW /= nW;

  for (unsigned int i = 0; i < 3; ++i) {
    transformationMatrix[0][i] = vAB[i];
    transformationMatrix[1][i] = vW[i];
    transformationMatrix[2][i] = vV[i];
    transformationMatrix[3][i] = dest[i] + (-glyphSize[0] * .5 * vAB[i]);
  }

  transformationMatrix[0][3] = 0;
  transformationMatrix[1][3] = 0;
  transformationMatrix[2][3] = 0;
  transformationMatrix[3][3] = 1;

  scalingMatrix.fill(0);
  scalingMatrix[0][0] = glyphSize[0];
  scalingMatrix[1][1] = glyphSize[1];
  scalingMatrix[2][2] = glyphSize[2];
  scalingMatrix[3][3] = 1;
}
