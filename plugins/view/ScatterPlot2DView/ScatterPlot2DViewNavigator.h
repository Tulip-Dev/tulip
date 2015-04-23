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

#ifndef SCATTERPLOTPREVIEWGENERATOR_H_
#define SCATTERPLOTPREVIEWGENERATOR_H_

#include <tulip/GLInteractor.h>
#include <tulip/Coord.h>

namespace tlp {

class ScatterPlot2DView;
class ScatterPlot2D;

class ScatterPlot2DViewNavigator : public GLInteractorComponent {

public :

  ScatterPlot2DViewNavigator();
  ~ScatterPlot2DViewNavigator();

  bool eventFilter(QObject *, QEvent *);
  bool draw(GlMainWidget*) {
    return false;
  }
  bool compute(GlMainWidget*) {
    return false;
  }
  void viewChanged(View *view);

private :

  ScatterPlot2D *getOverviewUnderPointer(const Coord &sceneCoord);

  ScatterPlot2DView *scatterPlot2dView;
  ScatterPlot2D *selectedScatterPlotOverview;

  GlMainWidget *glWidget;

};

}


#endif /* SCATTERPLOTPREVIEWGENERATOR_H_ */
