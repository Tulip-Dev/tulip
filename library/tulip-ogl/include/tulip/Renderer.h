#ifndef _RENDERER_H_
#define _RENDERER_H_
#ifndef DOXYGEN_NOTFOR_DEVEL

#include "Fonts.h"

namespace tlp {

class Renderer : public Fonts {
public:
  virtual void translate(float x, float y, float z) const =0;
  virtual void setColor(unsigned char r, unsigned char v, unsigned char b) const =0;
  virtual void drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const =0;
  void setMode(FontMode  m);
  FontMode getMode();
  void setDepth(float p);
  float getDepth();

 protected:  
  FontMode mode;
  float depth;
};

}
#endif //DOXYGEN_NOTFOR_DEVEL
#endif
