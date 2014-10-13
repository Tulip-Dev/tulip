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

#include <iostream>
#include <cmath>
#include <climits>

#include "HilbertLayout.h"

using namespace std;
using namespace tlp;

namespace {
typedef tlp::Vector< int, 2> uPoint;
enum STATESID {KEY = 1, POINT = 0, NEXT = 2};

static const unsigned char states[4][3][4] = { { {0, 1, 3, 2},
    {0, 1, 3, 2},
    {1, 0, 0, 2}
  },
  { {0, 3, 1, 2},
    {0, 2, 3, 1},
    {0, 1, 1, 3}
  },
  { {2, 1, 3, 0},
    {3, 1, 0, 2},
    {3, 2, 2, 0}
  },
  { {2, 3, 1, 0},
    {3, 2, 0, 1},
    {2, 3, 3, 1}
  }
};


inline uPoint hilbertPoint(const unsigned int key, const unsigned char order) {
  //  cerr << "========" << endl;
  unsigned char state = 0;
  uPoint point;
  point.fill(0);

  for (char i = order - 1; i >= 0; --i) {
    //read two bits;
    unsigned char bits = (key >> (i << 1))  & 3;
    //    unsigned char co = grState.graph[state].key[bits];
    unsigned char co = states[state][KEY][bits];
    //    cerr << "state :" << (int) state << " bits" << bits << " co:" << (int)co;
    point[1] += (co & 1) << i;
    //    point[0] += ((co >> 1) & 1) << (i-1);
    point[0] += ((co >> 1) ) << i;
    //    state = grState.graph[state].next[bits];
    state = states[state][NEXT][bits];
    //    cerr << "  next state :" << (int)state << endl;
  }

  return point;
}

inline unsigned int hilbertKey(const uPoint &p, const unsigned char order) {
  //  cerr << "========" << endl;
  unsigned char state = 0;
  unsigned int key = 0;

  for (char i = order - 1; i >= 0; --i) {
    //read two bits;
    unsigned char bits = (p[1] >> i)  & 1;
    bits += ((p[0] >> i)  & 1) << 1;
    //    cerr << "p:" << p << " bits:" << bits << endl;
    //    unsigned char co = grState.graph[state].point[bits];
    unsigned char co = states[state][POINT][bits];
    //    cerr << "state :" << (int) state << " bits" << bits << " co:" << (int)co;
    key += co << (i << 1);
    //    state = grState.graph[state].next[co];
    state = states[state][NEXT][co];
    //    cerr << "  next state :" << (int)state << endl;
  }

  return key;
}
}

namespace pocore {
HilbertLayout::HilbertLayout(unsigned char order):order(order) {
  shift = int(rint(sqrt(pow(4., order))/2.));
}
//==============================================================
unsigned int HilbertLayout::unproject(const Vec2i &point) const {
  //    cerr << point << " ==> " << shift << " order=" << (int)order << endl;
  Vec2i p;

  if (point[0] <= -shift || point[0] >= shift)
    return UINT_MAX;

  if (point[1] <= -shift || point[1] >= shift)
    return UINT_MAX;

  p[0] = point[0] + shift;
  p[1] = point[1] + shift;
  return hilbertKey(p, order);
}
//==============================================================
Vector<int, 2> HilbertLayout::project(const unsigned int id) const {
  return hilbertPoint(id, order)-=shift;
}
}
//==============================================================================
