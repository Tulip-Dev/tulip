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

#ifndef FISHEYESSCREENFURNAS_H
#define FISHEYESSCREENFURNAS_H
#include <string>
#include "ScreenFunction.h"

namespace pocore {

class FishEyesScreenFurnas : public ScreenFunction {

public:
  FishEyesScreenFurnas ();
  Vec2f project (const Vec2f& ) const;
  Vec2f unproject (const Vec2f& ) const;
  void  setCenter (double x, double y);
  void  setRadius (double r);
  void  setHeight (double h);

private:
  double R;
  double k;
  double l;
  Vec2f fisheyesCenter;
};
}
#endif //FISHEYESSCREENFURNAS_H

