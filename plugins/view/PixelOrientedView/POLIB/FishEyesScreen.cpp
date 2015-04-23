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

#include "FishEyesScreen.h"

using namespace tlp;

namespace {
//==============================================================
double unprojectRho(double rho_s, double R, double k) {
  double t1 = 1./k;
  double t2 = rho_s*R;
  double t8 = rho_s*rho_s;
  double t12 = sqrt(3.0);
  double t13 = k*k;
  double t14 = t13*k;
  double t18 = t8*k;
  double t19 = R*R;
  double t22 = t19*R;
  double t25 = t19*t14;
  double t29 = R*t14;
  double t33 = t8*t8;
  double t39 = t19*t13;
  double t46 = R*t13;
  double t53 = 8.0*t8*t14+4.0*t14+8.0*t18*t19+4.0*t22*t14+12.0*t25+12.0*t22*t13+12.0*t29+12.0*t22*k+4.0*R*t33*t13-20.0*t29*t8-20.0*t39*t8+4.0*t33*t14+4.0*t22+24.0*t39+12.0*t46+12.0*t19*k-t25*t8+16.0*t46*t8;
  double t55 = sqrt(t53*t1);
  double t60 = pow((-36.0*t2*k+72.0*rho_s*k+72.0*t2+8.0*t8*rho_s*k+12.0*t12*t55)*t13,0.3333333333333333);
  double t72 = t1*t60/6.0+2.0/3.0*(-3.0*R*k-3.0*k-3.0*R+t18)/t60+rho_s/3.0;
  return t72;
}
//==============================================================
double projectRho(double rho, double R, double k) {
  double t2 = rho*rho;
  double t8 = rho+rho*R/(t2+1.0+R/k);
  return t8;
}
}
namespace pocore {
//==============================================================
FishEyesScreen::FishEyesScreen() {
  R = 50.*50./2.0;
  k = 7.;
  fisheyesCenter.fill(0);
}
//==============================================================
void FishEyesScreen::setRadius(double r) {
  R = r * r / 2.0;
}
//==============================================================
double FishEyesScreen::getRadius() {
  return sqrt(2.0 * R);
}
//==============================================================
void FishEyesScreen::setHeight(double h) {
  k = h;

  if (k <= 0.1) k = 0.1;

  if (k > 8) k = 8.0;
}
void FishEyesScreen::setCenter(double x, double y) {
  fisheyesCenter[0] = x;
  fisheyesCenter[1] = y;
}
//==============================================================
Vec2f FishEyesScreen::project(const Vec2f &p) const {
  Vec2f dir = p - fisheyesCenter;
  float rho_s = fabs(dir.norm());

  if (rho_s > 1E-6) {
    dir /= rho_s;
    float rho = projectRho(rho_s, R, k);
    dir *= rho;
  }

  Vec2f result = fisheyesCenter + dir;
  return result;
}
//==============================================================
Vec2f FishEyesScreen::unproject(const Vec2f &p) const {
  Vec2f dir = p - fisheyesCenter;
  double rho_s = fabs(dir.norm());

  if (rho_s > 1E-6) {
    dir /= rho_s;
    double rho = unprojectRho(rho_s, R, k);

    if (fabs(rho-rho_s) < 1E-6) return p;

    dir *= rho;
  }

  Vec2f result = fisheyesCenter + dir;
  return result;
}
//==============================================================
}
