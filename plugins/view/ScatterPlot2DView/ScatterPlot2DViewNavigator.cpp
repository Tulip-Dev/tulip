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

#include <tulip/QtGlSceneZoomAndPanAnimator.h>
#include <tulip/GlMainWidget.h>
#include <tulip/Camera.h>

#include <QMouseEvent>

#include "ScatterPlot2DView.h"
#include "ScatterPlot2DViewNavigator.h"
#include "ScatterPlot2D.h"


using namespace std;

namespace tlp {

ScatterPlot2DViewNavigator::ScatterPlot2DViewNavigator() : scatterPlot2dView(NULL), selectedScatterPlotOverview(NULL), glWidget(NULL) {}

ScatterPlot2DViewNavigator::~ScatterPlot2DViewNavigator() {}

void ScatterPlot2DViewNavigator::viewChanged(View *view) {
  scatterPlot2dView = dynamic_cast<ScatterPlot2DView *>(view);
}

bool ScatterPlot2DViewNavigator::eventFilter(QObject *widget, QEvent *e) {

  if (glWidget == NULL) {
    glWidget = dynamic_cast<GlMainWidget *>(widget);
  }

  if (!glWidget->hasMouseTracking()) {
    glWidget->setMouseTracking(true);
  }

  if (!scatterPlot2dView->matrixViewSet() && !scatterPlot2dView->interactorsEnabled()) {
    scatterPlot2dView->toggleInteractors(true);
  }

  if (e->type() == QEvent::MouseMove && scatterPlot2dView->matrixViewSet()) {
    QMouseEvent *me = (QMouseEvent *) e;
    int x = glWidget->width() - me->x();
    int y = me->y();
    Coord screenCoords(x, y, 0.0f);
    Coord sceneCoords(glWidget->getScene()->getGraphCamera().screenTo3DWorld(screenCoords));
    selectedScatterPlotOverview = getOverviewUnderPointer(sceneCoords);
    return true;
  }
  else if (e->type() == QEvent::MouseButtonDblClick) {
    if (selectedScatterPlotOverview != NULL && !selectedScatterPlotOverview->overviewGenerated()) {
      scatterPlot2dView->generateScatterPlot(selectedScatterPlotOverview, glWidget);
      glWidget->draw();
    }
    else if (selectedScatterPlotOverview != NULL && scatterPlot2dView->matrixViewSet()) {
      QtGlSceneZoomAndPanAnimator zoomAndPanAnimator(glWidget, selectedScatterPlotOverview->getBoundingBox());
      zoomAndPanAnimator.animateZoomAndPan();
      scatterPlot2dView->switchFromMatrixToDetailView(selectedScatterPlotOverview, true);
      selectedScatterPlotOverview = NULL;
    }
    else if (!scatterPlot2dView->matrixViewSet()) {
      scatterPlot2dView->switchFromDetailViewToMatrixView();
      QtGlSceneZoomAndPanAnimator zoomAndPanAnimator(glWidget, scatterPlot2dView->getMatrixBoundingBox());
      zoomAndPanAnimator.animateZoomAndPan();
    }

    return true;
  }

  return false;
}

ScatterPlot2D *ScatterPlot2DViewNavigator::getOverviewUnderPointer(const Coord &sceneCoords) {
  ScatterPlot2D *ret = NULL;
  vector<ScatterPlot2D *> overviews =
    scatterPlot2dView->getSelectedScatterPlots();
  vector<ScatterPlot2D *>::iterator it;

  for (it = overviews.begin() ; it != overviews.end() ; ++it) {
    if(!(*it))
      continue;

    BoundingBox overviewBB = (*it)->getBoundingBox();

    if (sceneCoords.getX() >= overviewBB[0][0] && sceneCoords.getX() <= overviewBB[1][0] &&
        sceneCoords.getY() >= overviewBB[0][1] && sceneCoords.getY() <= overviewBB[1][1]) {
      ret = *it;
      break;
    }
  }

  return ret;
}

}
