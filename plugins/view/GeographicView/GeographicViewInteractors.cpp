/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#include <tulip/MouseNodeBuilder.h>
#include <tulip/MouseEdgeBuilder.h>
#include <tulip/MouseSelector.h>
#include <tulip/MouseSelectionEditor.h>
#include <tulip/MouseEdgeBendEditor.h>
#include <tulip/StandardInteractorPriority.h>

#include "GeographicViewInteractors.h"

#include "../../utils/PluginNames.h"

using namespace std;
using namespace tlp;

GeographicViewInteractor::GeographicViewInteractor(const QString &iconPath, const QString &text,
                                                   unsigned int priority)
    : NodeLinkDiagramComponentInteractor(iconPath, text, priority) {}

bool GeographicViewInteractor::isCompatible(const std::string &viewName) const {
  return (viewName == ViewName::GeographicViewName);
}

GeographicViewInteractorZoom::GeographicViewInteractorZoom(const PluginContext *)
    : GeographicViewInteractor(":/tulip/gui/icons/i_zoom.png", "Zoom on rectangle",
                               StandardInteractorPriority::ZoomOnRectangle) {}

void GeographicViewInteractorZoom::construct() {
  setConfigurationWidgetText(
      QString("<h3>Zoom on rectangle</h3>") +
      "<b>Mouse left</b> down indicates "
      "the first corner.<br/> drag the mouse then <b>Mouse left</b> up indicates the opposite "
      "corner.<br/> <b>Double Mouse left</b> click to center the view.<br/><br/>" +
      "Zoom/Unzoom: <b>Mouse wheel</b> down/up");
  push_back(new GeographicViewBoxZoomer);
}

GeographicViewInteractorSelection::GeographicViewInteractorSelection(const PluginContext *)
    : GeographicViewInteractor(":/tulip/gui/icons/i_selection.png", "selection in view",
                               StandardInteractorPriority::RectangleSelection) {}

void GeographicViewInteractorSelection::construct() {
  setConfigurationWidgetText(
      QString("<h3>Selection nodes/edges in a rectangle</h3>") +
      "<u>Draw selection rectangle</u><br/><b>Mouse left</b> down indicates the first corner, drag the mouse then "
      "<b>Mouse "
      "left</b> up indicates the opposite corner,<br/>all graph elements instersecting the "
      "rectangle are selected<br/><br/>" +
#if !defined(__APPLE__)
      "Add to selection: <ul><li><b>Ctrl + Mouse left</b> down, drag and up</li></ul>" +
#else
      "Add to selection: <ul><li><b>⌥ + Mouse left</b> down, drag and up</li></ul>" +
#endif
      "Remove from selection: <ul><li><b>Shift + Mouse left</b> down, drag and up</li></ul><br/><br/>" +
      "Zoom/Unzoom: <b>Mouse wheel</b> down/up");
  push_back(new GeographicViewNavigator);
  push_back(new MouseSelector);
}

QCursor GeographicViewInteractorSelection::cursor() const {
  return Qt::CrossCursor;
}

PLUGIN(GeographicViewInteractorSelection)

GeographicViewInteractorSelectionEditor::GeographicViewInteractorSelectionEditor(
    const PluginContext *)
    : GeographicViewInteractor(":/tulip/gui/icons/i_move.png", "selection edition in view",
                               StandardInteractorPriority::RectangleSelectionModifier) {}

void GeographicViewInteractorSelectionEditor::construct() {
  setConfigurationWidgetText(
      QString("<h3>Move/Reshape rectangle selection</h3>") + "<u>Modify selection</u><br/><br/>" +
      "Resize: <ul><li><b>Mouse left</b> down on triangle + moves</li></ul>" +
      "<ul><li><b>Mouse left</b> down on square + moves</li></ul>" +
#if !defined(__APPLE__)
      "Only change node size: <ul><li><b>Ctrl + Mouse left</b> down on triangle + "
#else
      "Only change node size: <ul><li><b>⌥ + Mouse left</b> down on triangle + "
#endif
      "moves</li></ul>" +
      "Only change selection size: <ul><li><b>Shift + Mouse left</b> down on triangle + "
      "moves</li></ul>" +
      "Rotate: <ul><li><b>Mouse left</b> down on circle + moves</li></ul>" +
#if !defined(__APPLE__)
      "Only rotate nodes: <ul><li><b>Ctrl + Mouse left</b> down on circle + moves</li></ul>" +
#else
      "Only rotate nodes: <ul><li><b>⌥ + Mouse left</b> down on circle + moves</li></ul>" +
#endif
      "Only rotate selection: <ul><li><b>Shift + Mouse left</b> down on circle + "
      "moves</li></ul>" +
      "Translate: <b>Mouse left</b> down inside rectangle + moves" +
      "Align vertically/horizontally: <ul><li><b>Mouse left</b> click on two arrows icon in top "
      "right zone</li></ul>" +
      "Align left/right/top/bottom: <ul><li><b>Mouse left</b> click on simple arrow icon in top "
      "right zone</li></ul><br/><br/>" +
      "Zoom/Unzoom: <b>Mouse wheel</b> up/down");
  push_back(new GeographicViewNavigator);
  push_back(new MouseSelector);
  push_back(new MouseSelectionEditor);
}

QCursor GeographicViewInteractorSelectionEditor::cursor() const {
  return Qt::CrossCursor;
}

PLUGIN(GeographicViewInteractorSelectionEditor)

GeographicViewNavigator::GeographicViewNavigator() : x(0), y(0), inRotation(false) {}

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

  if (geoView->mapType() <= GeographicView::CustomTileLayer) {
    return false;
  } else if (geoView->mapType() == GeographicView::Globe) {
    if (e->type() == QEvent::Wheel) {
      int vDelta = static_cast<QWheelEvent *>(e)->angleDelta().y();
      if (vDelta != 0) {
#define WHEEL_DELTA 120
        g->getScene()->zoomXY(vDelta / WHEEL_DELTA, g->width() / 2., g->height() / 2.);
        view()->draw();
        return true;
      }
      return false;
    }

    if (e->type() == QEvent::MouseButtonPress && !inRotation) {
      if (qMouseEv->button() == Qt::LeftButton) {
        x = qMouseEv->pos().x();
        y = qMouseEv->pos().y();
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
      Coord &&c1 = camera.getEyes() - camera.getCenter();
      Coord &&c2 = c1 + camera.getUp();
      trans(c1, c2, -0.005 * (qMouseEv->pos().y() - y), -0.005 * (qMouseEv->pos().x() - x));
      camera.setCenter(Coord(0, 0, 0));
      camera.setEyes(c1);
      camera.setUp(c2 - camera.getEyes());

      x = qMouseEv->pos().x();
      y = qMouseEv->pos().y();

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
      Coord &&c1 = camera.getEyes() - camera.getCenter();
      Coord &&c2 = c1 + camera.getUp();
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

GeographicViewBoxZoomer::GeographicViewBoxZoomer()
    : MouseBoxZoomer(Qt::LeftButton, Qt::NoModifier, false /* no viewport update*/) {}

bool GeographicViewBoxZoomer::eventFilter(QObject *widget, QEvent *e) {
  GeographicView *geoView = static_cast<GeographicView *>(view());

  if (geoView->mapType() >= GeographicView::CustomTileLayer)
    return false;

  if (e->type() == QEvent::MouseButtonDblClick) {
    geoView->centerView();
    // abort zoom on rectangle if any
    started = false;
    return true;
  }

  if (started) {
    bool ok = MouseBoxZoomer::eventFilter(widget, e);

    auto gmw = geoView->getGeoMapWidget();
    QMouseEvent *qMouseEv = dynamic_cast<QMouseEvent *>(e);

    if (ok && !started && (e->type() == QEvent::MouseButtonRelease) && graph &&
        (qMouseEv->button() & mButton)) {
      GlMainWidget *glw = static_cast<GlMainWidget *>(widget);

      auto tl = gmw->getScreenViewport().topLeft();
      auto corner1 = gmw->screenToGeoPos(tl.x() + x, tl.y() + glw->height() - y);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
      auto evPos = qMouseEv->pos();
#else
      auto evPos = qMouseEv->position().toPoint();
#endif
      auto corner2 = gmw->screenToGeoPos(tl.x() + evPos.x(), tl.y() + evPos.y());
      gmw->zoomOnRectangle(corner1, corner2);
    }
    return ok;
  }
  return MouseBoxZoomer::eventFilter(widget, e);
}

PLUGIN(GeographicViewInteractorZoom)

GeographicViewInteractorAddEdges::GeographicViewInteractorAddEdges(const PluginContext *)
    : GeographicViewInteractor(":/tulip/gui/icons/i_addedge.png", "Add nodes/edges",
                               StandardInteractorPriority::AddNodesOrEdges) {}

void GeographicViewInteractorAddEdges::construct() {
  setConfigurationWidgetText("<h3>Add nodes/edges</h3>To add a node: <b>Mouse left</b> click "
                             "outside any node.<br/>To add an edge: <b>Mouse left</b> click on the "
                             "source node,<br/>then <b>Mouse left</b> click on the target "
                             "node.<br/>Any <b>Mouse left</b> click outside a node before the "
                             "click on the target node will add an edge bend,<br/><b>Mouse "
                             "middle</b> click will cancel the current edge construction.");
  push_back(new GeographicViewNavigator);
  push_back(new MouseNodeBuilder);
  push_back(new MouseEdgeBuilder);
}

QCursor GeographicViewInteractorAddEdges::cursor() const {
  return QCursor(Qt::PointingHandCursor);
}

PLUGIN(GeographicViewInteractorAddEdges)

GeographicViewInteractorEditEdgeBends::GeographicViewInteractorEditEdgeBends(const PluginContext *)
    : GeographicViewInteractor(":/tulip/gui/icons/i_bends", "Edit edge bends",
                               StandardInteractorPriority::EditEdgeBends) {}

void GeographicViewInteractorEditEdgeBends::construct() {
  setConfigurationWidgetText(
      QString("<h3>Edit edge bends</h3>") + "<>Modify edge bends</u><br/><br/>" +
      "Select edge: <ul><li>use rectangle selection</li></ul>" +
      "Translate bend: <ul><li><b>Mouse left</b> down on a selected bend + moves</li></ul>" +
      "Change source node: <ul><li><b>Drag and drop circle</b> on source node</li></ul>" +
      "Change target node: <ul><li><b>Drag and drop triangle</b> on target node</li></ul>" +
      "Add bend: <ul><li><b>Double Mouse left</b> click on the selected "
      "edge</li></ul>" +
#if !defined(__APPLE__)
      "Delete bend: <ul><li><b>Ctrl + Mouse left</b> click on a selected bend</li></ul>"
#else
      "Delete bend: <ul><li><b>⌥ + Mouse left</b> click on a selected bend</li></ul>"
#endif
      + "<br/><br/>Zoom/Unzoom: <b>Mouse wheel</b> down/up");
  push_back(new GeographicViewNavigator);
  push_back(new MouseSelector);
  push_back(new MouseEdgeBendEditor);
}

PLUGIN(GeographicViewInteractorEditEdgeBends)
