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
#ifndef MOUSEMAGICSELECTION_H
#define MOUSEMAGICSELECTION_H

#include <vector>
#include <tulip/Graph.h>
#include <tulip/Coord.h>
#include <tulip/InteractorComponent.h>
namespace tlp {

/** \addtogroup Mouse_interactor */
/*@{*/
class _DEPRECATED TLP_QT_SCOPE MouseMagicSelector:public InteractorComponent
{
private:
  unsigned int x,y;
  int w,h;
  Graph *graph;
public:
  MouseMagicSelector();
  ~MouseMagicSelector(){}
  bool eventFilter(QObject *, QEvent *);
  InteractorComponent *clone() { return new MouseMagicSelector(); }};
/*@}*/

}
#endif
