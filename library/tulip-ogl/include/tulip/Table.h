#ifndef _TABLE_H_
#define _TABLE_H_

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
#endif
