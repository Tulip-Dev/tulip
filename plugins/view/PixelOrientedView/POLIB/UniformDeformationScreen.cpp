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

#include "UniformDeformationScreen.h"

namespace pocore {

UniformDeformationScreen::UniformDeformationScreen():
  _tx(0), _ty(0), _zoom(1) {
  updateMatrix();
}
//=======================================================
Vec2f UniformDeformationScreen::project(const Vec2f &point) const {
  Vec2f scPoint;
  tlp::Vector<double, 3> vect, vectres;
  vect[0] = point[0];
  vect[1] = point[1];
  vect[2] = 1.0;
  vectres = _mat * vect;
  scPoint[0] = (vectres[0]/vectres[2]);
  scPoint[1] = (vectres[1]/vectres[2]);
  return scPoint;
}
//=======================================================
Vec2f UniformDeformationScreen::unproject(const Vec2f &point) const {
  Vec2f scPoint(point);
  tlp::Vector<double, 3> vect, vectres;
  vect[0] = point[0];
  vect[1] = point[1];
  vect[2] = 1.0;
  vectres = _invMat * vect;
  scPoint[0] = vectres[0]/vectres[2];
  scPoint[1] = vectres[1]/vectres[2];
  return scPoint;
}
//=======================================================
void UniformDeformationScreen::setTranslation(double x, double y) {
  _tx = x;
  _ty = y;
  updateMatrix();
}
//=======================================================
void UniformDeformationScreen::getTranslation(double &x, double &y) {
  x=_tx;
  y=_ty;
}
//=======================================================
void UniformDeformationScreen::setZoom(double zoom) {
  _zoom = zoom;
  updateMatrix();
}
//=======================================================
double UniformDeformationScreen::getZoom() {
  return _zoom;
}
//=======================================================
void UniformDeformationScreen::updateMatrix() {
  tlp::Matrix<double, 3> tmp;
  //zoom
  tmp.fill(0);
  tmp[0][0] = _zoom;
  tmp[1][1] = _zoom;
  tmp[2][2] = 1.0;
  tmp[2][0] = 0.;
  tmp[2][1] = 0.;
  //translate
  _mat.fill(0);
  _mat[0][0] = 1.;
  _mat[1][1] = 1.;
  _mat[2][2] = 1.;
  _mat[0][2] = -_tx;
  _mat[1][2] = -_ty;
  _mat =  tmp * _mat;
  _invMat = _mat;
  _invMat.inverse();
}
}
