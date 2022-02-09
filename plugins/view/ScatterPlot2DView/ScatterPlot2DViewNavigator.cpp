/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include <tulip/GlMainWidget.h>
#include <tulip/Camera.h>

#include <QMouseEvent>

#include "ScatterPlot2DView.h"
#include "ScatterPlot2DViewNavigator.h"
#include "ScatterPlot2D.h"

using namespace std;

namespace tlp {

ScatterPlot2DViewNavigator::ScatterPlot2DViewNavigator()
    : scatterPlot2dView(nullptr), selectedScatterPlotOverview(nullptr), glWidget(nullptr) {}

ScatterPlot2DViewNavigator::~ScatterPlot2DViewNavigator() {}

void ScatterPlot2DViewNavigator::viewChanged(View *view) {
  scatterPlot2dView = static_cast<ScatterPlot2DView *>(view);
}

bool ScatterPlot2DViewNavigator::eventFilter(QObject *widget, QEvent *e) {

  if (glWidget == nullptr) {
    glWidget = static_cast<GlMainWidget *>(widget);
  }

  if (glWidget != nullptr) {
    if (!glWidget->hasMouseTracking()) {
      glWidget->setMouseTracking(true);
    }

    if (!scatterPlot2dView->matrixViewSet() && !scatterPlot2dView->interactorsEnabled()) {
      scatterPlot2dView->toggleInteractors(true);
    }

    if (e->type() == QEvent::MouseMove && scatterPlot2dView->matrixViewSet()) {
      QMouseEvent *me = static_cast<QMouseEvent *>(e);
      int x = glWidget->width() - me->x();
      int y = me->y();
      Coord screenCoords(x, y, 0.0f);
      Coord sceneCoords(glWidget->getScene()->getGraphCamera().viewportTo3DWorld(
          glWidget->screenToViewport(screenCoords)));
      selectedScatterPlotOverview = getOverviewUnderPointer(sceneCoords);
      return true;
    } else if (e->type() == QEvent::MouseButtonDblClick) {
      if (selectedScatterPlotOverview != nullptr &&
          !selectedScatterPlotOverview->overviewGenerated()) {
        scatterPlot2dView->generateScatterPlot(selectedScatterPlotOverview);
        glWidget->draw();
      } else if (selectedScatterPlotOverview != nullptr && scatterPlot2dView->matrixViewSet()) {
        scatterPlot2dView->zoomAndPanAnimation(selectedScatterPlotOverview->getBoundingBox());
        scatterPlot2dView->switchFromMatrixToDetailView(selectedScatterPlotOverview, true);
        selectedScatterPlotOverview = nullptr;
      } else if (!scatterPlot2dView->matrixViewSet()) {
        scatterPlot2dView->switchFromDetailViewToMatrixView();
        scatterPlot2dView->zoomAndPanAnimation(scatterPlot2dView->getMatrixBoundingBox());
      }

      return true;
    }
  }

  return false;
}

ScatterPlot2D *ScatterPlot2DViewNavigator::getOverviewUnderPointer(const Coord &sceneCoords) {
  vector<ScatterPlot2D *> &&overviews = scatterPlot2dView->getSelectedScatterPlots();

  for (auto ov : overviews) {
    BoundingBox &&overviewBB = ov->getBoundingBox();
    if (sceneCoords.getX() >= overviewBB[0][0] && sceneCoords.getX() <= overviewBB[1][0] &&
        sceneCoords.getY() >= overviewBB[0][1] && sceneCoords.getY() <= overviewBB[1][1])
      return ov;
  }

  return nullptr;
}
} // namespace tlp
