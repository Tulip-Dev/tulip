#ifndef _HLINE_H_
#define _HLINE_H_
#ifndef DOXYGEN_NOTFOR_DEVEL

#include "Frame.h"
namespace tlp {

class Renderer;

const unsigned char ROUGE=255;
const unsigned char VERT=255;
const unsigned char BLEU=255;

class Hline : public Frame{

  unsigned char couleur[3]; // couleur que prend le tracé
  const Renderer &renderer;
 public:
  Hline(const Renderer &renderer):renderer(renderer) {couleur[0] = ROUGE; couleur[1] = VERT; couleur[2] = BLEU;}
  virtual ~Hline(){}
  void draw(float w_max, float& w) const;
  void getBoundingBox(float w_max, float& h, float& w) const;
  void getColor(unsigned char& r, unsigned char& v, unsigned char& b) const;
  void setColor(unsigned char r, unsigned char v, unsigned char b);
};

}

#endif //OXYGEN_NOTFOR_DEVEL
#endif
