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

#include <tulip/GlRect.h>
#include <tulip/BoundingBox.h>
#include <tulip/TulipPluginHeaders.h>
#include <tulip/QtGlSceneZoomAndPanAnimator.h>

#include "PixelOrientedViewNavigator.h"

#include <QEvent>
#include <QMouseEvent>

using namespace std;

namespace tlp {

PixelOrientedViewNavigator::PixelOrientedViewNavigator() : pixelView(NULL), selectedOverview(NULL) {}

PixelOrientedViewNavigator::~PixelOrientedViewNavigator() {}

void PixelOrientedViewNavigator::viewChanged(View *view) {
  pixelView = static_cast<PixelOrientedView *>(view);
}

bool PixelOrientedViewNavigator::eventFilter(QObject *widget, QEvent *e) {

  if(e->type() != QEvent::MouseButtonDblClick && e->type() != QEvent::MouseMove)
    return false;

  GlMainWidget *glWidget = (GlMainWidget *) widget;

  if (!glWidget->hasMouseTracking()) {
    glWidget->setMouseTracking(true);
  }

  if (!pixelView->smallMultiplesViewSet() && !pixelView->interactorsEnabled()) {
    pixelView->toggleInteractors(true);
  }

  if (pixelView->getOverviews().size() == 0) {
    return false;
  }

  if (e->type() == QEvent::MouseMove && pixelView->smallMultiplesViewSet()) {
    QMouseEvent *me = (QMouseEvent *) e;
    int x = glWidget->width() - me->x();
    int y = me->y();
    Coord screenCoords((double) x, (double) y, 0);
    Coord sceneCoords = glWidget->getScene()->getGraphCamera().screenTo3DWorld(screenCoords);
    PixelOrientedOverview *overviewUnderPointer = getOverviewUnderPointer(sceneCoords);

    if (overviewUnderPointer != NULL && overviewUnderPointer != selectedOverview) {
      selectedOverview = overviewUnderPointer;
    }

    return true;
  }
  else if (e->type() == QEvent::MouseButtonDblClick) {
    if (selectedOverview != NULL && !selectedOverview->overviewGenerated()) {
      pixelView->generatePixelOverview(selectedOverview, glWidget);
      glWidget->draw();
    }
    else if (selectedOverview != NULL && pixelView->smallMultiplesViewSet()) {
      QtGlSceneZoomAndPanAnimator zoomAndPanAnimator(glWidget, selectedOverview->getBoundingBox());
      zoomAndPanAnimator.animateZoomAndPan();
      pixelView->switchFromSmallMultiplesToDetailView(selectedOverview);
      selectedOverview = NULL;
    }
    else if (!pixelView->smallMultiplesViewSet() && pixelView->getOverviews().size() > 1) {
      pixelView->switchFromDetailViewToSmallMultiples();
      QtGlSceneZoomAndPanAnimator zoomAndPanAnimator(glWidget, pixelView->getSmallMultiplesViewBoundingBox());
      zoomAndPanAnimator.animateZoomAndPan();
      pixelView->centerView();
    }

    return true;
  }

  return false;
}

PixelOrientedOverview *PixelOrientedViewNavigator::getOverviewUnderPointer(Coord &sceneCoords) {
  PixelOrientedOverview *ret = NULL;
  vector<PixelOrientedOverview *> overviews = pixelView->getOverviews();
  vector<PixelOrientedOverview *>::iterator it;

  for (it = overviews.begin() ; it != overviews.end() ; ++it) {
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
