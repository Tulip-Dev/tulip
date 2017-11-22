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

#include <QApplication>
#include <tulip/MouseNodeBuilder.h>
#include <tulip/MouseEdgeBuilder.h>
#include <tulip/MouseSelector.h>
#include <tulip/MouseEdgeBendEditor.h>

#include "GeographicViewInteractors.h"

#include "../../utils/StandardInteractorPriority.h"
#include "../../utils/PluginNames.h"

using namespace std;

GeographicViewInteractor::GeographicViewInteractor(const QString &iconPath, const QString &text)
    : GLInteractorComposite(QIcon(iconPath), text) {}

bool GeographicViewInteractor::isCompatible(const std::string &viewName) const {
  return (viewName == ViewName::GeographicViewName);
}

GeographicViewInteractorNavigation::GeographicViewInteractorNavigation(const PluginContext *)
    : GeographicViewInteractor(":/tulip/gui/icons/i_navigation.png", "Navigate in view") {}

unsigned int GeographicViewInteractorNavigation::priority() const {
  return StandardInteractorPriority::Navigation;
}

void GeographicViewInteractorNavigation::construct() {
  push_back(new GeographicViewNavigator);
}

QWidget *GeographicViewInteractorNavigation::configurationWidget() const {
  return nullptr;
}

GeographicViewNavigator::GeographicViewNavigator() : x(0), y(0), inRotation(false) {}

GeographicViewNavigator::~GeographicViewNavigator() {}

void GeographicViewNavigator::viewChanged(View *) {}

void trans(Coord &c1, Coord &c2, float angle1, float angle2) {
  float rho1 = sqrt(c1[0] * c1[0] + c1[1] * c1[1] + c1[2] * c1[2]);
  float theta1 = acos(c1[2] / rho1);
  float phi1 = acos(c1[0] / sqrt(c1[0] * c1[0] + c1[1] * c1[1]));

  float rho2 = sqrt(c2[0] * c2[0] + c2[1] * c2[1] + c2[2] * c2[2]);
  float theta2 = acos(c2[2] / rho2);
  float phi2 = acos(c2[0] / sqrt(c2[0] * c2[0] + c2[1] * c2[1]));

  if (c1[1] < 0)
    phi1 = 2 * M_PI - phi1;

  if (c1[0] == 0 && c1[1] == 0)
    phi1 = 0;

  if (c2[1] < 0)
    phi2 = 2 * M_PI - phi2;

  if (c2[0] == 0 && c2[1] == 0)
    phi2 = 0;

  if (theta1 + angle1 > 0.001 && theta1 + angle1 < M_PI && theta2 + angle1 > 0.001 &&
      theta2 + angle1 < M_PI) {
    theta1 += angle1;
    theta2 += angle1;

    if (theta2 > theta1) {
      float tmp = theta1;
      theta1 = theta2;
      theta2 = tmp;
    }
  }

  phi2 += angle2;
  phi1 = phi2;

  c1[0] = rho1 * sin(theta1) * cos(phi1);
  c1[1] = rho1 * sin(theta1) * sin(phi1);
  c1[2] = rho1 * cos(theta1);

  c2[0] = rho2 * sin(theta2) * cos(phi2);
  c2[1] = rho2 * sin(theta2) * sin(phi2);
  c2[2] = rho2 * cos(theta2);
}

bool GeographicViewNavigator::eventFilter(QObject *widget, QEvent *e) {
  GeographicView *geoView = static_cast<GeographicView *>(view());
  GlMainWidget *g = static_cast<GlMainWidget *>(widget);
  QMouseEvent *qMouseEv = dynamic_cast<QMouseEvent *>(e);
  QWheelEvent *qWheelEv = dynamic_cast<QWheelEvent *>(e);

  if (geoView->viewType() == GeographicView::GoogleRoadMap ||
      geoView->viewType() == GeographicView::GoogleSatellite ||
      geoView->viewType() == GeographicView::GoogleTerrain ||
      geoView->viewType() == GeographicView::GoogleHybrid) {

    if (qMouseEv || qWheelEv) {
      GeographicView *geoView = static_cast<GeographicView *>(view());
      QApplication::sendEvent(geoView->getGoogleMap(), e);
    }

    return false;
  } else if (geoView->viewType() == GeographicView::Globe) {
    if (e->type() == QEvent::Wheel && qWheelEv->orientation() == Qt::Vertical) {
#define WHEEL_DELTA 120
      g->getScene()->zoomXY(qWheelEv->delta() / WHEEL_DELTA, g->width() / 2., g->height() / 2.);
      view()->draw();
      return true;
    }

    if (e->type() == QEvent::MouseButtonPress && !inRotation) {
      if (qMouseEv->button() == Qt::LeftButton) {
        x = qMouseEv->x();
        y = qMouseEv->y();
        inRotation = true;
        return true;
      }
    }

    if (e->type() == QEvent::MouseButtonRelease) {
      if (qMouseEv->button() == Qt::LeftButton) {
        inRotation = false;
        return true;
      }
    }

    if (e->type() == QEvent::MouseMove && inRotation) {

      Camera &camera = g->getScene()->getGraphCamera();
      Coord c1 = camera.getEyes() - camera.getCenter();
      Coord c2 = camera.getEyes() - camera.getCenter() + camera.getUp();
      trans(c1, c2, -0.005 * (qMouseEv->y() - y), -0.005 * (qMouseEv->x() - x));
      camera.setCenter(Coord(0, 0, 0));
      camera.setEyes(c1);
      camera.setUp(c2 - camera.getEyes());

      x = qMouseEv->x();
      y = qMouseEv->y();

      view()->draw();
      return true;
    }

    if (e->type() == QEvent::KeyPress) {

      float angle1 = 0;
      float angle2 = 0;

      switch (static_cast<QKeyEvent *>(e)->key()) {
      case Qt::Key_Left:
        angle2 = -0.05f;
        break;

      case Qt::Key_Right:
        angle2 = 0.05f;
        break;

      case Qt::Key_Up:
        angle1 = 0.05f;
        break;

      case Qt::Key_Down:
        angle1 = -0.05f;
        break;
      }

      Camera &camera = g->getScene()->getGraphCamera();
      Coord c1 = camera.getEyes() - camera.getCenter();
      Coord c2 = camera.getEyes() - camera.getCenter() + camera.getUp();
      trans(c1, c2, angle1, angle2);
      camera.setCenter(Coord(0, 0, 0));
      camera.setEyes(c1);
      camera.setUp(c2 - camera.getEyes());

      view()->draw();

      return true;
    }

    return false;
  } else {
    return MouseNKeysNavigator::eventFilter(widget, e);
  }
}

PLUGIN(GeographicViewInteractorNavigation)

GeographicViewInteractorAddEdges::GeographicViewInteractorAddEdges(const PluginContext *)
    : NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_addedge.png", "Add nodes/edges") {
  setPriority(StandardInteractorPriority::AddNodesOrEdges);
  setConfigurationWidgetText("<h3>Add nodes/edges</h3>To add a node: <b>Mouse left</b> click "
                             "outside any node.<br/>To add an edge: <b>Mouse left</b> click on the "
                             "source node,<br/>then <b>Mouse left</b> click on the target "
                             "node.<br/>Any <b>Mouse left</b> click outside a node before the "
                             "click on the target node will add an edge bend,<br/><b>Mouse "
                             "middle</b> click will cancel the current edge construction.");
}

void GeographicViewInteractorAddEdges::construct() {
  push_back(new GeographicViewNavigator);
  push_back(new MouseNodeBuilder);
  push_back(new MouseEdgeBuilder);
}

QCursor GeographicViewInteractorAddEdges::cursor() const {
  return QCursor(Qt::PointingHandCursor);
}

bool GeographicViewInteractorAddEdges::isCompatible(const std::string &viewName) const {
  return (viewName == ViewName::GeographicViewName);
}

PLUGIN(GeographicViewInteractorAddEdges)

GeographicViewInteractorEditEdgeBends::GeographicViewInteractorEditEdgeBends(const PluginContext *)
    : NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_bends", "Edit edge bends") {
  setPriority(StandardInteractorPriority::EditEdgeBends);
  setConfigurationWidgetText(
      QString("<h3>Edit edge bends</h3>") + "Modify edge bends<br/><br/>" +
      "Select edge: <ul><li>use rectangle selection</li></ul>" +
      "Translate bend: <ul><li><b>Mouse left</b> down on a selected bend + moves</li></ul>" +
      "Change source node: <ul><li><b>Drag and drop circle on source node</li></ul>" +
      "Change target node: <ul><li><b>Drag and drop triangle on target node</li></ul>" +
      "Add bend: <ul><li><b>Double click with mouse left</b> click on the selected edge</li></ul>" +
#if !defined(__APPLE__)
      "Delete bend: <ul><li><b>Ctrl + Mouse left</b> click on a selected bend</li></ul>"
#else
      "Delete bend: <ul><li><b>Alt + Mouse left</b> click on a selected bend</li></ul>"
#endif
      );
}

void GeographicViewInteractorEditEdgeBends::construct() {
  push_back(new GeographicViewNavigator);
  push_back(new MouseSelector);
  push_back(new MouseEdgeBendEditor);
}

bool GeographicViewInteractorEditEdgeBends::isCompatible(const std::string &viewName) const {
  return (viewName == ViewName::GeographicViewName);
}

PLUGIN(GeographicViewInteractorEditEdgeBends)
