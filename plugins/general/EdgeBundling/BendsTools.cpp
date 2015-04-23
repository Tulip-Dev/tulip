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

#include "BendsTools.h"
#include <tulip/Vector.h>

using namespace tlp;
using namespace std;

BendsTools::BendsTools() {
}

//============================================
double BendsTools::cosAlpha(LayoutProperty * layout, const node a, const node b, const node c) {
  Vector<double, 2> point[3];
  Coord aC = layout->getNodeValue(a);
  point[0][0] = aC[0];
  point[0][1] = aC[1];
  Coord bC = layout->getNodeValue(b);
  point[1][0] = bC[0];
  point[1][1] = bC[1];
  Coord cC = layout->getNodeValue(c);
  point[2][0] = cC[0];
  point[2][1] = cC[1];

  Vector<double, 2> ba = point[0] - point[1];
  ba /= ba.norm();
  Vector<double, 2> bc = point[2] - point[1];
  bc /= bc.norm();

  return ba.dotProduct(bc) / (ba.norm() * bc.norm());
}
//============================================
bool BendsTools::straightLine( LayoutProperty *  layout, const node a, const node b, const node c) {
  Vector<double, 2> point[3];
  Coord aC = layout->getNodeValue(a);
  point[0][0] = aC[0];
  point[0][1] = aC[1];
  Coord bC = layout->getNodeValue(b);
  point[1][0] = bC[0];
  point[1][1] = bC[1];
  Coord cC = layout->getNodeValue(c);
  point[2][0] = cC[0];
  point[2][1] = cC[1];

  Vector<double, 2> ba = point[0] - point[1];
  Vector<double, 2> bc = point[2] - point[1];
  Vector<double, 2> ac = point[2] - point[0];

  //assert(ba.norm() + bc.norm() - ac.norm() >= 0.);
  if (fabs(ba.norm() + bc.norm() - ac.norm()) < 1E-9) {
    //cout << fabs(ac.norm() - ba.norm() - bc.norm()) << ",";
    return true;
  }
  else
    return false;
}
//============================================
vector<node> BendsTools::bendsSimplification(vector<node> &bends,  LayoutProperty *  layout) {
  //remove all <=90° angles if the 3 point are in a freecell
  vector<node> result1;
  bool redo = true;

  while (redo) {
    redo = false;
    result1.push_back(bends[0]);

    for (size_t i = 1; i<bends.size()-1; ++i) {
      double cAlpha = cosAlpha(layout, bends[i-1], bends[i], bends[i+1]);

      if ( fabs(cAlpha) < 1E-9) {
        //found 90
        bends[i] = bends[i-1];
        redo = true;
      }
      else {
        result1.push_back(bends[i]);
      }
    }

    result1.push_back(bends[bends.size()-1]);

    bends = result1;
    result1.clear();
  }

  //  return bends;
  //remove all straight line points
  result1.push_back(bends[0]);

  for (size_t i = 1; i<bends.size()-1; ++i) {
    //double cAlpha = cosAlpha(layout, bends[i-1], bends[i], bends[i+1]);
    bool straight = straightLine(layout, bends[i-1], bends[i], bends[i+1]);

    //        cout << 1. - fabs(cAlpha) << ",";
//        assert(1. - fabs(cAlpha) > 0.);
    if ( straight) {
      //found 180°
      bends[i] = bends[i-1];
    }
    else {
      result1.push_back(bends[i]);
    }
  }

  result1.push_back(bends[bends.size()-1]);
  return result1;
}
