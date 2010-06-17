/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#ifndef MOUSEZOOMBOX_H
#define MOUSEZOOMBOX_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <tulip/InteractorComponent.h>

class QMouseEvent;
class QKeyEvent;

namespace tlp {

class Graph;
/** \addtogroup Mouse_interactor */
/*@{*/
class TLP_QT_SCOPE MouseBoxZoomer : public InteractorComponent {
private:
  Qt::MouseButton mButton;
  Qt::KeyboardModifier kModifier;
public:
  MouseBoxZoomer(Qt::MouseButton button = Qt::LeftButton,
		 Qt::KeyboardModifier modifier = Qt::NoModifier);
  ~MouseBoxZoomer();
  bool draw(GlMainWidget *);
  bool eventFilter(QObject *, QEvent *);
  InteractorComponent *clone() { return new MouseBoxZoomer(mButton, kModifier); }
private:
  unsigned int x, y;
  int w, h;
  bool started;
  Graph *graph;
};
/*@}*/

}
#endif //MOUSEZOOMBOX_H
