/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#ifndef _TABLE_H_
#define _TABLE_H_
#ifndef DOXYGEN_NOTFOR_DEVEL

#include "FLayout.h"

#include <vector>

namespace tlp {

typedef std::vector<Frame*, std::allocator<Frame*> > vFrame;
typedef std::vector<vFrame, std::allocator<vFrame> > vvFrame;

class Context;

class Table : public FLayout{
  vvFrame cellules;
  unsigned char couleur[3];
  Context & _context;
 public:
  Table(Context &c):_context(c){cellules.clear(); couleur[0] = couleur[1] = couleur[2] = 255;}
  Table(Context &, int row, int col);
  virtual ~Table();
  void draw(float w_max, float& w) const;
  void getBoundingBox(float w_max, float& h, float& w) const;
  bool addFrame(Frame* f);
  bool addFrame(Frame* f, int row, int col);

  void getColor(unsigned char& r, unsigned char& v, unsigned char& b) const;
  void setColor(unsigned char r, unsigned char v, unsigned char b);
};

}
#endif //DOXYGEN_NOTFOR_DEVEL
#endif
