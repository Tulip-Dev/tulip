/*
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
///@cond DOXYGEN_HIDDEN

#ifndef MOUSESELECTION_H
#define MOUSESELECTION_H

#include <tulip/GLInteractor.h>

namespace tlp {

class Graph;
class TLP_QT_SCOPE MouseSelector:public GLInteractorComponent {
public:
  enum SelectionMode {
    EdgesAndNodes = 0,
    EdgesOnly,
    NodesOnly
  };

  MouseSelector(Qt::MouseButton button = Qt::LeftButton,
                Qt::KeyboardModifier modifier = Qt::NoModifier,
                SelectionMode mode = EdgesAndNodes);
  ~MouseSelector() {}
  bool draw(GlMainWidget *);
  bool eventFilter(QObject *, QEvent *);
protected:
  Qt::MouseButton mButton;
  Qt::KeyboardModifier kModifier;
  Qt::KeyboardModifiers mousePressModifier;
  unsigned int x,y;
  int w,h;
  bool started;
  Graph *graph;
  SelectionMode _mode;
};


}
#endif
///@endcond
