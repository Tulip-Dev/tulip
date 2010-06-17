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
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/Vector.h>

namespace tlp {
TemplateFactory<EdgeExtremityGlyphFactory, EdgeExtremityGlyph, EdgeExtremityGlyphContext *>
		*EdgeExtremityGlyphFactory::factory;
EdgeExtremityGlyph::EdgeExtremityGlyph(EdgeExtremityGlyphContext *gc) {
	if (gc != 0) {
		edgeExtGlGraphInputData = gc->glGraphInputData;
	} else {
		edgeExtGlGraphInputData = 0;
	}
}

EdgeExtremityGlyph::~EdgeExtremityGlyph() {

}

EdgeExtremityGlyphFrom3DGlyph::EdgeExtremityGlyphFrom3DGlyph(EdgeExtremityGlyphContext *gc) :
	EdgeExtremityGlyph(gc) {

}
EdgeExtremityGlyphFrom3DGlyph::~EdgeExtremityGlyphFrom3DGlyph() {

}

void EdgeExtremityGlyphFrom3DGlyph::getTransformationMatrix(const Coord &src, const Coord &dest,
		const Size &glyphSize, MatrixGL& transformationMatrix, MatrixGL& scalingMatrix) {
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
	} else if (fabs(vAB[1]) < 1E-6) {
		vV[0] = 0;
		vV[1] = 1.0;
		vV[2] = 0;
	} else {
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
//MatrixGL EdgeExtremityGlyphFrom3DGlyph::getTransformationMatrix(const Coord &A, const Coord &B,
//		const Size &glyphSize) {
//	MatrixGL matrix;
//
//	//Vecteur AB
//	Vector<float, 3> vAB;
//	//Vecteur V
//	Vector<float, 3> vV;
//	//Vecteur W
//	Vector<float, 3> vW;
//
//	vAB = B - A;
//	float nAB; //|AB|
//	nAB = vAB.norm();
//	if (fabs(nAB) > 1E-6)
//		vAB /= nAB;
//
//	//vAB * vV = xAB * xV + yAB*yV + zAB * zV = |AB| * |V| * cos(alpha) = 0;
//	if (fabs(vAB[2]) < 1E-6) {
//		vV[0] = 0;
//		vV[1] = 0;
//		vV[2] = 1.0;
//	} else if (fabs(vAB[1]) < 1E-6) {
//		vV[0] = 0;
//		vV[1] = 1.0;
//		vV[2] = 0;
//	} else {
//		vV[0] = 0;
//		vV[1] = 1. / vAB[1];
//		vV[2] = -1. / vAB[2];
//		vV /= vV.norm();
//	}
//
//	vW = vAB ^ vV;
//	float nW = vW.norm();
//	if (fabs(nW) > 1E-6)
//		vW /= nW;
//
//	for (unsigned int i = 0; i < 3; ++i) {
//		matrix[0][i] = vW[i];
//		matrix[1][i] = vV[i];
//		matrix[2][i] = vAB[i];
//		matrix[3][i] = B[i] + (-glyphSize[0] * .5 * vAB[i]);
//	}
//
//	matrix[0][3] = 0;
//	matrix[1][3] = 0;
//	matrix[2][3] = 0;
//	matrix[3][3] = 1;
//
//	return matrix;
//
//}
//
//MatrixGL EdgeExtremityGlyphFrom3DGlyph::getScalingMatrix(const Size &glyphSize) {
//
//	MatrixGL scale;
//	scale.fill(0);
//	scale[0][0] = glyphSize[1];
//	scale[1][1] = glyphSize[2];
//	scale[2][2] = glyphSize[0];
//	scale[3][3] = 1;
//
//	return scale;
//
//}

EdgeExtremityGlyphFrom2DGlyph::EdgeExtremityGlyphFrom2DGlyph(EdgeExtremityGlyphContext *gc) :
	EdgeExtremityGlyph(gc) {

}
EdgeExtremityGlyphFrom2DGlyph::~EdgeExtremityGlyphFrom2DGlyph() {

}

void EdgeExtremityGlyphFrom2DGlyph::getTransformationMatrix(const Coord &src, const Coord &dest,
		const Size &glyphSize, MatrixGL& transformationMatrix, MatrixGL& scalingMatrix) {
	//	Vecteur AB
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
	} else if (fabs(vAB[1]) < 1E-6) {
		vV[0] = 0;
		vV[1] = 1.0;
		vV[2] = 0;
	} else {
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
//MatrixGL EdgeExtremityGlyphFrom2DGlyph::getTransformationMatrix(const Coord &A, const Coord &B,
//		const Size &glyphSize) {
//
//	MatrixGL matrix;
//
//	//Vecteur AB
//	Vector<float, 3> vAB;
//	//Vecteur V
//	Vector<float, 3> vV;
//	//Vecteur W
//	Vector<float, 3> vW;
//
//	vAB = B - A;
//	float nAB; //|AB|
//	nAB = vAB.norm();
//	if (fabs(nAB) > 1E-6)
//		vAB /= nAB;
//
//	//vAB * vV = xAB * xV + yAB*yV + zAB * zV = |AB| * |V| * cos(alpha) = 0;
//	if (fabs(vAB[2]) < 1E-6) {
//		vV[0] = 0;
//		vV[1] = 0;
//		vV[2] = 1.0;
//	} else if (fabs(vAB[1]) < 1E-6) {
//		vV[0] = 0;
//		vV[1] = 1.0;
//		vV[2] = 0;
//	} else {
//		vV[0] = 0;
//		vV[1] = 1. / vAB[1];
//		vV[2] = -1. / vAB[2];
//		vV /= vV.norm();
//	}
//
//	vW = vAB ^ vV;
//	float nW = vW.norm();
//	if (fabs(nW) > 1E-6)
//		vW /= nW;
//
//	for (unsigned int i = 0; i < 3; ++i) {
//		matrix[0][i] = vAB[i];
//		matrix[1][i] = vW[i];
//		matrix[2][i] = vV[i];
//		matrix[3][i] = B[i] + (-glyphSize[0] * .5 * vAB[i]);
//	}
//
//	matrix[0][3] = 0;
//	matrix[1][3] = 0;
//	matrix[2][3] = 0;
//	matrix[3][3] = 1;
//
//	return matrix;
//
//}
//
//MatrixGL EdgeExtremityGlyphFrom2DGlyph::getScalingMatrix(const Size &glyphSize) {
//
//	MatrixGL scale;
//	scale.fill(0);
//	scale[0][0] = glyphSize[0];
//	scale[1][1] = glyphSize[1];
//	scale[2][2] = glyphSize[2];
//	scale[3][3] = 1;
//
//	return scale;
//
//}

//BoundingBox EdgeExtremityGlyphFrom2DGlyph::computeBoundingBox(const Coord& anchor,
//		const Coord& dest, const Coord& size) {
//
//}

}

