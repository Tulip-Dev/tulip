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
#ifndef MOUSETREEFISHEYES_H
#define MOUSETREEFISHEYES_H

#ifndef DOXYGEN_NOTFOR_USER

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
#endif //DOXYGEN_NOTFOR_USER
#endif
