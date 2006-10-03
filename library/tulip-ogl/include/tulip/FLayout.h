#ifndef _FLAYOUT_H_
#define _FLAYOUT_H_
#ifndef DOXYGEN_NOTFOR_DEVEL

#include "Frame.h"

namespace tlp {

struct FLayout : public Frame{
  virtual void draw(float w_max, float& w)const=0;
  virtual void getBoundingBox(float w_max, float& h, float& w)const=0;
  virtual bool addFrame(Frame* f)=0;
  virtual ~FLayout()=0;
};

}

#endif //DOXYGEN_NOTFOR_DEVEL
#endif
