/**
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

#ifndef MOUSEFREEHANDSELECTOR_H
#define MOUSEFREEHANDSELECTOR_H

#include <tulip/NodeLinkDiagramComponentInteractor.h>
#include <tulip/GlMainWidget.h>

#include <vector>

namespace tlp {

/*@{*/
/** \file
 *  \brief  Tulip Lasso Selector Interactor

 * This interactor plugin allow to select graph nodes with a lasso.
 * By defining a closed polygon with the mouse above the visualization,
 * all nodes under that polygon and edges connecting these nodes will be selected.
 */
class MouseLassoNodesSelectorInteractor : public NodeLinkDiagramComponentInteractor { /* GLInteractorComposite*/

public :

  PLUGININFORMATION("MouseLassoNodesSelectorInteractor", "Antoine Lambert", "19/06/2009", "Mouse Lasso Nodes Selector Interactor", "1.0", "Modification")

  MouseLassoNodesSelectorInteractor(const tlp::PluginContext *);

  void construct();

  bool isCompatible(const std::string &viewName) const;

};

class MouseLassoNodesSelectorInteractorComponent : public GLInteractorComponent {

public :

  MouseLassoNodesSelectorInteractorComponent();
  ~MouseLassoNodesSelectorInteractorComponent();

  bool draw(GlMainWidget *glWidget);
  bool eventFilter(QObject *obj, QEvent *e);

private :

  void selectGraphElementsUnderPolygon(GlMainWidget *glWidget);

  std::vector<Coord> polygon;
  Coord currentPointerScreenCoord;
  bool drawInteractor;
  Camera *camera;
  Graph *graph;
  BooleanProperty *viewSelection;
  bool dragStarted;

};
}

#endif // MOUSEFREEHANDSELECTOR_H
