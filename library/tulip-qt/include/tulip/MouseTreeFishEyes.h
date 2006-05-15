//-*-c++-*-
#ifndef MOUSETREEFISHEYES_H
#define MOUSETREEFISHEYES_H

#include <tulip/MouseInterface.h>
///
namespace tlp {

class MouseTreeFishEyes:public MouseInterface
{
public:
  int x,y;
  MouseTreeFishEyes(){}
  ~MouseTreeFishEyes(){}
  void mPressEvent(GlGraphWidget *,QMouseEvent *);
  void mReleaseEvent(GlGraphWidget *,QMouseEvent *);
  void mMoveEvent(GlGraphWidget *,QMouseEvent *){}
  void mPaint(GlGraphWidget *){}
  void keyPressEvent (GlGraphWidget *, QKeyEvent *){}
  void keyReleaseEvent(GlGraphWidget *, QKeyEvent *){}
  void wheelEvent(GlGraphWidget *, QWheelEvent *){}
  MouseInterface * clone() { return new MouseTreeFishEyes();}
};

}
#endif
