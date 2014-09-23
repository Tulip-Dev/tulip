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

#ifndef UNIFORMDEFORMATIONSCREEN_H
#define UNIFORMDEFORMATIONSCREEN_H

#include <tulip/Matrix.h>
#include "ScreenFunction.h"

namespace pocore {
class UniformDeformationScreen : public ScreenFunction {
public:
  Vec2f  project (const Vec2f& ) const;
  Vec2f  unproject (const Vec2f& ) const;
  void   setTranslation (double x, double y);
  void   getTranslation(double &x,double &y);
  void   setZoom (double zoom);
  double getZoom ();
  UniformDeformationScreen ();
private:
  void updateMatrix();
private:
  double _tx;
  double _ty;
  double _zoom;
  tlp::Matrix<double, 3> _mat;
  tlp::Matrix<double, 3> _invMat;

};
}
#endif //UNIFORMDEFORMATIONSCREEN_H

