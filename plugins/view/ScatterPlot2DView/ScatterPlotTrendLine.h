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

#ifndef SCATTERPLOTTRENDLINE_H_
#define SCATTERPLOTTRENDLINE_H_

#include <tulip/GLInteractor.h>

namespace tlp {

class ScatterPlot2DView;

class ScatterPlotTrendLine : public GLInteractorComponent {

public :

  ScatterPlotTrendLine();
  ~ScatterPlotTrendLine();

  bool eventFilter(QObject *, QEvent *);
  bool draw(GlMainWidget *glMainWidget);
  bool compute(GlMainWidget *glMainWidget);
  void viewChanged(View *view);

private :

  ScatterPlot2DView *scatterView;
  float a, b;

};

}

#endif /* SCATTERPLOTTRENDLINE_H_ */
