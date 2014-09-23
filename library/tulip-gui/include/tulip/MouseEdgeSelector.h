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

#ifndef MOUSEEDGESELECTION_H
#define MOUSEEDGESELECTION_H

#include <tulip/InteractorComposite.h>

namespace tlp {

class Graph;

class TLP_QT_SCOPE MouseEdgeSelector:public InteractorComponent {
private:
  unsigned int x,y;
  int w,h;
  bool started;
  Graph *graph;
public:
  MouseEdgeSelector();
  ~MouseEdgeSelector() {}
  bool draw(GlMainWidget *);
  bool eventFilter(QObject *, QEvent *);
};


}
#endif
///@endcond
