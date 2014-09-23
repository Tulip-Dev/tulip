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

#include "FishEyesScreenFurnas.h"

using namespace tlp;
/* static double ty3(double y, double R) {
  double t4 = sqrt(3.0);
  double t5 = R*R;
  double t9 = y*y;
  double t12 = sqrt(16.0*t5-40.0*R*y+27.0*t9);
  double t17 = pow((-80.0*R+108.0*y+12.0*t4*t12)*t5,0.3333333333333333);
  return 1.0/R*t17/12.0-2.0/3.0*R/t17+1.0/3.0;
  }*/
inline double xt3(double t, double R) {
  double t1 = 1.0-t;
  double t2 = t1*t1;
  double t4 = t*t;
  double t9 = t4*t4;
  return 4.0*R*t2*t4+4.0*R*t1*t4*t+4.0*R*t9;
}

/*inline double tx3(double x, double R) {
double t = 0.5;
double mint = 0.;
double maxt = 1.;
double val = xt3(t,R);

while (fabs(val-x)>1E-4) {
  if (val > x) {
    maxt = t;
    t = (t + mint) / 2.0;
  }
  else if (val < x) {
    mint = t;
    t = (maxt + t) / 2.0;

  }

  val = xt3(t,R);
}

return t;
}*/
//====================================================================
/*static double yt3(double t, double R) {
  //y = R*4*(1-t)^3*t + R*4*(1-t)*t^3 + 4*R*t^4
  //y = 4Rt - 12Rt^2 + 16Rt^3 - 4Rt^4;
  return
    R*4.* pow((1-t),3) * t +
    2./3. * R * 6.* pow((1-t),2) * pow(t,2)+
    R*4.* (1-t) * pow(t,3) +
    R*4.* pow(t,4);
    }*/

//==============================================================
static double unprojectRho(double rho_s, double R, double k, double) {
  //    double rho = rho_s;
  if (rho_s < R) {
    return rho_s * R / ( R * k + R - rho_s * k );
  }
  else
    return rho_s;

  //    if (rho_s <  3.*R) {
  //      double t = ty3(rho, R);
  //      rho = fabs(xt3(t, R));
  //    }
  //    return rho;
}
//==============================================================
static double projectRho(double rho, double R, double k, double) {
  if (rho < R)
    return (k+1) * rho / (k * rho/R + 1);
  else
    return rho;

  /*
  double rhos = rho;
  if (rho <  3.*R) {
    double t = tx3(rho, R);
    rhos = fabs(yt3(t, R));
  }
  return rhos;
  */
}

namespace pocore {
//==============================================================
FishEyesScreenFurnas::FishEyesScreenFurnas() {
  R = 50;
  k = 4;
  l = 5;
  fisheyesCenter.fill(0);
}
//==============================================================
void FishEyesScreenFurnas::setRadius(double r) {
  R = r;
}
void FishEyesScreenFurnas::setHeight(double h) {
  k = h;

  if (k <= 0.1) k = 0.1;

  if (k > 8) k = 8.0;
}
void FishEyesScreenFurnas::setCenter(double x, double y) {
  fisheyesCenter[0] = x;
  fisheyesCenter[1] = y;
}
//==============================================================
Vec2f FishEyesScreenFurnas::project(const Vec2f &p) const {
  Vec2f dir = p - fisheyesCenter;
  double rho_s = fabs(dir.norm());

  if (rho_s > 1E-6) {
    dir /= rho_s;
    double rho = projectRho(rho_s, R, k, l);
    dir *= rho;
  }

  Vec2f result = fisheyesCenter + dir;
  return result;
}
//==============================================================
Vec2f FishEyesScreenFurnas::unproject(const Vec2f &p) const {
  Vec2f dir = p - fisheyesCenter;
  double rho_s = fabs(dir.norm());

  if (rho_s > 1E-6) {
    dir /= rho_s;
    double rho = unprojectRho(rho_s, R, k, l);

    if (fabs(rho-rho_s) < 1E-6) return p;

    dir *= rho;
  }

  Vec2f result = fisheyesCenter + dir;
  return result;
}
//==============================================================
}
