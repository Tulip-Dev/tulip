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

#include <tulip/QtGlSceneZoomAndPanAnimator.h>
#include <tulip/GlMainWidget.h>
#include <tulip/Camera.h>

#include "HistogramViewNavigator.h"
#include "HistogramView.h"

#include <QMouseEvent>

using namespace std;

namespace tlp {

HistogramViewNavigator::HistogramViewNavigator()
    : histoView(nullptr), selectedHistoOverview(nullptr) {}

HistogramViewNavigator::~HistogramViewNavigator() {}

void HistogramViewNavigator::viewChanged(View *view) {
  histoView = static_cast<HistogramView *>(view);
}

bool HistogramViewNavigator::eventFilter(QObject *widget, QEvent *e) {

  GlMainWidget *glWidget = static_cast<GlMainWidget *>(widget);

  if (!glWidget->hasMouseTracking()) {
    glWidget->setMouseTracking(true);
  }

  if (!histoView->smallMultiplesViewSet() && !histoView->interactorsEnabled()) {
    histoView->toggleInteractors(true);
  }

  if (histoView->getHistograms().size() == 1) {
    return false;
  }

  if (e->type() == QEvent::MouseMove && histoView->smallMultiplesViewSet()) {
    QMouseEvent *me = static_cast<QMouseEvent *>(e);
    int x = glWidget->width() - me->x();
    int y = me->y();
    Coord screenCoords(x, y, 0);
    Coord &&sceneCoords = glWidget->getScene()->getGraphCamera().viewportTo3DWorld(
        glWidget->screenToViewport(Coord(x, y, 0)));
    selectedHistoOverview = getOverviewUnderPointer(sceneCoords);
    return true;
  } else if (e->type() == QEvent::MouseButtonDblClick) {
    if (selectedHistoOverview != nullptr && histoView->smallMultiplesViewSet()) {
      QtGlSceneZoomAndPanAnimator zoomAndPanAnimator(glWidget,
                                                     selectedHistoOverview->getBoundingBox());
      zoomAndPanAnimator.animateZoomAndPan();
      histoView->switchFromSmallMultiplesToDetailedView(selectedHistoOverview);
      selectedHistoOverview = nullptr;
    } else if (!histoView->smallMultiplesViewSet()) {
      histoView->switchFromDetailedViewToSmallMultiples();
      QtGlSceneZoomAndPanAnimator zoomAndPanAnimator(glWidget,
                                                     histoView->getSmallMultiplesBoundingBox());
      zoomAndPanAnimator.animateZoomAndPan();
    }

    return true;
  }

  return false;
}

Histogram *HistogramViewNavigator::getOverviewUnderPointer(const Coord &sceneCoords) const {
  for (auto histo : histoView->getHistograms()) {
    BoundingBox &&overviewBB = histo->getBoundingBox();
    if (sceneCoords.getX() >= overviewBB[0][0] && sceneCoords.getX() <= overviewBB[1][0] &&
        sceneCoords.getY() >= overviewBB[0][1] && sceneCoords.getY() <= overviewBB[1][1])
      return histo;
  }

  return nullptr;
}
} // namespace tlp
