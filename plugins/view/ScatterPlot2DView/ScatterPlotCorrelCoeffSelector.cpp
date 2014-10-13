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

#include <algorithm>

#include <tulip/GlLines.h>
#include <tulip/GlNode.h>
#include <tulip/GlEdge.h>
#include <tulip/GlComplexPolygon.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlQuantitativeAxis.h>
#include <tulip/GlLabel.h>

#include <QMenu>
#include <QEvent>
#include <QMouseEvent>

#include "ScatterPlotCorrelCoeffSelector.h"
#include "ScatterPlotCorrelCoeffSelectorOptionsWidget.h"
#include "ScatterPlot2DView.h"
#include "ScatterPlot2D.h"

using namespace std;

namespace tlp {

const float POINT_RADIUS = 3.0f;

bool pointInPolygon(const vector<Coord> &polygon, const Coord &point) {
  size_t i, j;
  bool ret = false;

  for (i = 0, j = polygon.size() - 1 ; i < polygon.size() ; j = i++) {
    if ((((polygon[i].getY() <= point.getY()) && (point.getY() < polygon[j].getY())) ||
         ((polygon[j].getY() <= point.getY()) && (point.getY() < polygon[i].getY()))) &&
        (point.getX() < (polygon[j].getX() - polygon[i].getX()) * (point.getY() - polygon[i].getY()) / (polygon[j].getY() - polygon[i].getY()) + polygon[i].getX()))
      ret = !ret;
  }

  return ret;
}

bool isPolygonAincludesInB(const vector<Coord> &A, const vector<Coord> &B) {
  bool ret = true;

  for (size_t i = 0 ; i < A.size() ; ++i) {
    ret = ret && pointInPolygon(B, A[i]);

    if (!ret) break;
  }

  return ret;
}

GlEditableComplexPolygon::GlEditableComplexPolygon(std::vector<Coord> polygonPoints, const Color &color) : polygonPoints(polygonPoints), color(color), selected(false) {
  basicCircle.resizePoints(30);
  basicCircle.setFillMode(true);
  basicCircle.setOutlineMode(true);
  basicCircle.setFillColor(Color(204, 255, 0, 255));
  basicCircle.setOutlineColor(Color(0, 0, 255, 255));
}

void GlEditableComplexPolygon::translate(const Coord &move) {
  for (size_t i = 0 ; i < polygonPoints.size() ; ++i) {
    polygonPoints[i] += move;
  }
}

BoundingBox GlEditableComplexPolygon::getBoundingBox() {
  BoundingBox ret;

  for (size_t i = 0 ; i < polygonPoints.size() ; ++i) {
    ret.expand(polygonPoints[i]);
  }

  return ret;
}

bool GlEditableComplexPolygon::pointInsidePolygon(const Coord &point) {
  return pointInPolygon(polygonPoints, point);
}

Coord *GlEditableComplexPolygon::getPolygonVertexUnderPointerIfAny(const Coord &pointerScreenCoord, Camera *camera) {
  Coord *ret = NULL;
  vector<Coord>::iterator it;
  camera->initGl();

  for (it = polygonPoints.begin() ; it != polygonPoints.end() ; ++it) {
    Coord pointCenter = camera->worldTo2DScreen(*it);

    if (pointerScreenCoord.getX() > (pointCenter.getX() - POINT_RADIUS) && pointerScreenCoord.getX() < (pointCenter.getX() + POINT_RADIUS) &&
        pointerScreenCoord.getY() > (pointCenter.getY() - POINT_RADIUS) && pointerScreenCoord.getY() < (pointCenter.getY() + POINT_RADIUS)) {
      ret = new Coord(*it);
      break;
    }
  }

  return ret;
}

std::pair<Coord, Coord> *GlEditableComplexPolygon::getPolygonSegmentUnderPointerIfAny(const Coord &pointerSceneCoord) {
  vector<Coord> polygonPointsCp(polygonPoints);
  polygonPointsCp.push_back(polygonPoints.front());

  for (size_t i = 0 ; i < polygonPointsCp.size() - 1 ; ++i) {
    double startToEndDist = polygonPointsCp[i].dist(polygonPointsCp[i+1]);
    double startToPointDist = polygonPointsCp[i].dist(pointerSceneCoord);
    double pointToEndDist = pointerSceneCoord.dist(polygonPointsCp[i+1]);

    if ((((startToPointDist + pointToEndDist) - startToEndDist)/startToEndDist < 1E-3)) {
      return new pair<Coord, Coord>(polygonPointsCp[i], polygonPointsCp[i+1]);
    }
  }

  return NULL;
}

void GlEditableComplexPolygon::movePolygonVertexToPoint(const Coord &polygonVertex, const Coord &targetPoint) {
  std::replace(polygonPoints.begin(), polygonPoints.end(), polygonVertex, targetPoint);
}

void GlEditableComplexPolygon::addPolygonVertex(std::pair<Coord, Coord> polygonSegment, const Coord &newVertex) {
  vector<Coord>::iterator it;

  for (it = polygonPoints.begin() ; it != polygonPoints.end() ; ++it) {
    if ((*it) == polygonSegment.second) {
      polygonPoints.insert(it, newVertex);
      return;
    }
  }

  if (polygonSegment.first == polygonPoints.back() && polygonSegment.second == polygonPoints.front()) {
    polygonPoints.push_back(newVertex);
  }
}

void GlEditableComplexPolygon::removePolygonVertex(const Coord &vertex) {
  if (find(polygonPoints.begin(), polygonPoints.end(), vertex) != polygonPoints.end()) {
    polygonPoints.erase(std::remove(polygonPoints.begin(), polygonPoints.end(), vertex), polygonPoints.end());
  }
}

void GlEditableComplexPolygon::draw(float lod,Camera* camera) {
  camera->initGl();
  GlComplexPolygon complexPolygon(polygonPoints, color, color, 1);
  glDisable(GL_DEPTH_TEST);
  complexPolygon.draw(lod, camera);

  if (selected) {
    Camera camera2D(camera->getScene(), false);
    camera2D.setScene(camera->getScene());

    for (size_t i = 0 ; i < polygonPoints.size() ; ++i) {
      camera->initGl();
      Coord tmp = camera->worldTo2DScreen(polygonPoints[i]);
      camera2D.initGl();
      basicCircle.set(tmp, POINT_RADIUS, 0.);
      basicCircle.draw(lod, camera);
    }
  }
}

ScatterPlotCorrelCoeffSelector::ScatterPlotCorrelCoeffSelector(ScatterPlotCorrelCoeffSelectorOptionsWidget *optionsWidget) :
  optionsWidget(optionsWidget), scatterView(NULL), selectedPolygon(NULL), selectedPolygonPoint(NULL), dragStarted(false), x(0), y(0) {
  basicCircle.resizePoints(30);
  basicCircle.setFillMode(true);
  basicCircle.setOutlineMode(true);
  basicCircle.setFillColor(Color(255, 0, 0, 255));
  basicCircle.setOutlineColor(Color(0, 0, 255, 255));
}

ScatterPlotCorrelCoeffSelector::ScatterPlotCorrelCoeffSelector(const ScatterPlotCorrelCoeffSelector &scatterPlotCorrelCoeffSelector) :
  scatterView(NULL), selectedPolygon(NULL), selectedPolygonPoint(NULL),
  dragStarted(false), x(0), y(0) {
  optionsWidget = scatterPlotCorrelCoeffSelector.optionsWidget;
  basicCircle.resizePoints(30);
  basicCircle.setFillMode(true);
  basicCircle.setOutlineMode(true);
  basicCircle.setFillColor(Color(255, 0, 0, 255));
  basicCircle.setOutlineColor(Color(0, 0, 255, 255));
}

ScatterPlotCorrelCoeffSelector::~ScatterPlotCorrelCoeffSelector() {}

bool ScatterPlotCorrelCoeffSelector::eventFilter(QObject *obj, QEvent *e) {
  GlMainWidget *glWidget = dynamic_cast<GlMainWidget *>(obj);

  if(glWidget==NULL)
    return false;

  Camera &camera = glWidget->getScene()->getLayer("Main")->getCamera();
  Graph *graph = glWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
  BooleanProperty *viewSelection = graph->getProperty<BooleanProperty>("viewSelection");

  if (!glWidget->hasMouseTracking()) {
    glWidget->setMouseTracking(true);
  }

  if (e->type() == QEvent::MouseMove) {
    QMouseEvent *me = (QMouseEvent *) e;
    x = glWidget->width() - me->x();
    y = me->y();
    Coord newPointerSceneCoords = camera.screenTo3DWorld(Coord(x, y, 0));
    Coord translationVectorScene = newPointerSceneCoords - currentPointerSceneCoords;
    currentPointerSceneCoords = newPointerSceneCoords;

    if (polygonEdit.empty()) {
      if (!dragStarted) {
        getPolygonAndPointUnderPointerIfAny(currentPointerSceneCoords, &camera);

        if (selectedPolygonPoint != NULL) {
          glWidget->setCursor(QCursor(Qt::ClosedHandCursor));
        }
        else if (selectedPolygon != NULL) {
          glWidget->setCursor(QCursor(Qt::SizeAllCursor));
        }
        else {
          glWidget->setCursor(QCursor(Qt::ArrowCursor));
        }
      }
      else {
        if (selectedPolygon != NULL && selectedPolygonPoint != NULL) {
          selectedPolygon->movePolygonVertexToPoint(*selectedPolygonPoint, currentPointerSceneCoords);
          delete selectedPolygonPoint;
          selectedPolygonPoint = new Coord(currentPointerSceneCoords);
        }
        else if (selectedPolygon != NULL) {
          selectedPolygon->translate(translationVectorScene);

          if (selectedPolygonPoint != NULL) {
            *selectedPolygonPoint += translationVectorScene;
          }
        }
      }
    }
    else if (polygonEdit.size() > 2) {
      Coord startPolygonPointScr = camera.worldTo2DScreen(polygonEdit[0]);
      Coord pointerGlScreenCoord = Coord(me->x(), glWidget->height() - me->y());
      bool underFirstPoint = (pointerGlScreenCoord.getX() > (startPolygonPointScr.getX() - POINT_RADIUS) && pointerGlScreenCoord.getX() < (startPolygonPointScr.getX() + POINT_RADIUS) &&
                              pointerGlScreenCoord.getY() > (startPolygonPointScr.getY() - POINT_RADIUS) && pointerGlScreenCoord.getY() < (startPolygonPointScr.getY() + POINT_RADIUS));

      if (underFirstPoint) {
        glWidget->setCursor(QCursor(Qt::SizeAllCursor));
      }
      else {
        glWidget->setCursor(QCursor(Qt::ArrowCursor));
      }
    }

    glWidget->redraw();
    return true;
  }
  else if (e->type() == QEvent::MouseButtonPress) {
    QMouseEvent *me = (QMouseEvent *) e;
    x = glWidget->width() - me->x();
    y = me->y();
    currentPointerSceneCoords = camera.screenTo3DWorld(Coord(x, y, 0));

    if (me->buttons() == Qt::LeftButton) {
      if (selectedPolygon != NULL || selectedPolygonPoint != NULL) {
        dragStarted = true;
      }
      else if (polygonEdit.size() < 2) {
        polygonEdit.push_back(currentPointerSceneCoords);
      }
      else {
        Coord startPolygonPointScr = camera.worldTo2DScreen(polygonEdit[0]);
        Coord pointerGlScreenCoord = Coord(me->x(), glWidget->height() - me->y());
        bool underFirstPoint = (pointerGlScreenCoord.getX() > (startPolygonPointScr.getX() - POINT_RADIUS) && pointerGlScreenCoord.getX() < (startPolygonPointScr.getX() + POINT_RADIUS) &&
                                pointerGlScreenCoord.getY() > (startPolygonPointScr.getY() - POINT_RADIUS) && pointerGlScreenCoord.getY() < (startPolygonPointScr.getY() + POINT_RADIUS));

        if (underFirstPoint) {
          polygons.push_back(new GlEditableComplexPolygon(polygonEdit, Color(0,255,0,100)));
          polygonEdit.clear();
          mapPolygonColorToCorrelCoeffOfData(polygons.back(), glWidget);
          glWidget->setCursor(QCursor(Qt::ArrowCursor));
        }
        else {
          polygonEdit.push_back(currentPointerSceneCoords);
        }

        glWidget->redraw();
      }
    }
    else if (me->buttons() == Qt::RightButton) {
      if (!polygonEdit.empty()) {
        polygonEdit.clear();
        glWidget->redraw();
      }
      else if (selectedPolygon != NULL) {
        QMenu contextMenu(glWidget);
        QAction *deletePoly = contextMenu.addAction("Remove polygon");
        QAction *selectData = contextMenu.addAction("Select nodes under polygon");
        QAction *selectedAction = contextMenu.exec(me->globalPos());

        if (selectedAction == deletePoly) {
          polygons.erase(std::remove(polygons.begin(), polygons.end(), selectedPolygon), polygons.end());
          polygonsToNodesSubsetAndCorrelCoeff.erase(selectedPolygon);
          delete selectedPolygon;
          selectedPolygon = NULL;
          delete selectedPolygonPoint;
          selectedPolygonPoint = NULL;
          glWidget->redraw();
        }
        else if (selectedAction == selectData) {
          Observable::holdObservers();

          for (size_t i = 0 ; i < polygonsToNodesSubsetAndCorrelCoeff[selectedPolygon].first.size() ; ++i) {
            viewSelection->setNodeValue(polygonsToNodesSubsetAndCorrelCoeff[selectedPolygon].first[i], true);
          }

          for (size_t i = 0 ; i < polygonsToNodesSubsetAndCorrelCoeff[selectedPolygon].first.size() ; ++i) {
            for (size_t j = i+1 ; j < polygonsToNodesSubsetAndCorrelCoeff[selectedPolygon].first.size() ; ++j) {
              edge e = graph->existEdge(polygonsToNodesSubsetAndCorrelCoeff[selectedPolygon].first[i], polygonsToNodesSubsetAndCorrelCoeff[selectedPolygon].first[j], false);

              if (e.isValid()) {
                viewSelection->setEdgeValue(e, true);
              }
            }
          }

          Observable::unholdObservers();
        }
      }
    }

    return true;
  }
  else if (e->type() == QEvent::MouseButtonRelease) {
    dragStarted = false;

    if (selectedPolygon != NULL) {
      mapPolygonColorToCorrelCoeffOfData(selectedPolygon, glWidget);
      glWidget->redraw();
    }

    return true;
  }
  else if (e->type() == QEvent::MouseButtonDblClick) {
    QMouseEvent *me = (QMouseEvent *) e;
    x = glWidget->width() - me->x();
    y = me->y();
    currentPointerSceneCoords = camera.screenTo3DWorld(Coord(x, y, 0.0f));

    if (selectedPolygon != NULL && selectedPolygonPoint != NULL) {
      if (selectedPolygon->getNumberOfVertex() > 3) {
        selectedPolygon->removePolygonVertex(*selectedPolygonPoint);
        delete selectedPolygonPoint;
        selectedPolygonPoint = NULL;
      }
    }
    else if (selectedPolygon != NULL) {
      pair<Coord, Coord> *polygonSegment = selectedPolygon->getPolygonSegmentUnderPointerIfAny(currentPointerSceneCoords);

      if (polygonSegment != NULL) {
        selectedPolygon->addPolygonVertex(*polygonSegment, currentPointerSceneCoords);
        delete polygonSegment;
      }
    }

    glWidget->redraw();
  }

  return false;
}

bool ScatterPlotCorrelCoeffSelector::draw(GlMainWidget *glMainWidget) {
  Camera &camera = glMainWidget->getScene()->getLayer("Main")->getCamera();

  glDisable(GL_STENCIL_TEST);
  glEnable(GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for (size_t i = 0 ; i < polygons.size() ; ++i) {
    polygons[i]->draw(0, &camera);
  }

  camera.initGl();

  if (selectedPolygon != NULL) {
    GlQuantitativeAxis *xAxis = scatterView->getDetailedScatterPlot()->getXAxis();
    GlQuantitativeAxis *yAxis = scatterView->getDetailedScatterPlot()->getYAxis();
    GlLabel coeffLabel(Coord(xAxis->getAxisBaseCoord().getX() + (1.0f / 2.0f) * xAxis->getAxisLength(), yAxis->getAxisBaseCoord().getY() + yAxis->getAxisLength() + yAxis->getLabelHeight()),
                       Size(3.0f * (xAxis->getAxisLength() / 4.0f), yAxis->getLabelHeight()), selectedPolygon->getColor());
    ostringstream oss;
    oss << "correlation coefficient for data under polygon = " << polygonsToNodesSubsetAndCorrelCoeff[selectedPolygon].second;
    coeffLabel.setText(oss.str());
    coeffLabel.draw(0,&camera);
  }

  Camera camera2D(camera.getScene(), false);

  Color backgroundColor(glMainWidget->getScene()->getBackgroundColor());
  Color foregroundColor;
  int bgV = backgroundColor.getV();

  if (bgV < 128) {
    foregroundColor = Color(255,255,255);
  }
  else {
    foregroundColor = Color(0,0,0);
  }

  if (!polygonEdit.empty()) {
    for (size_t i = 0 ; i < polygonEdit.size() - 1 ; ++i) {
      camera.initGl();
      Coord lineStart(camera.worldTo2DScreen(polygonEdit[i]));
      Coord lineEnd(camera.worldTo2DScreen(polygonEdit[i+1]));
      camera2D.initGl();
      GlLines::glDrawLine(lineStart, lineEnd, 1.0f, GlLines::TLP_DASHED, foregroundColor, foregroundColor);
    }

    camera.initGl();
    Coord lineStart(camera.worldTo2DScreen(polygonEdit[polygonEdit.size() - 1]));
    Coord lineEnd(camera.worldTo2DScreen(currentPointerSceneCoords));
    camera2D.initGl();
    GlLines::glDrawLine(lineStart, lineEnd, 1.0f, GlLines::TLP_DASHED, foregroundColor, foregroundColor);
  }

  for (size_t i = 0 ; i < polygonEdit.size() ; ++i) {
    camera.initGl();
    Coord pointCoord(camera.worldTo2DScreen(polygonEdit[i]));
    camera2D.initGl();
    basicCircle.set(pointCoord, POINT_RADIUS, 0.);
    basicCircle.draw(0, 0);
  }

  return true;
}

bool ScatterPlotCorrelCoeffSelector::compute(GlMainWidget*) {
  return false;
}

void ScatterPlotCorrelCoeffSelector::viewChanged(View *view) {
  scatterView = dynamic_cast<ScatterPlot2DView *>(view);
}

void ScatterPlotCorrelCoeffSelector::getPolygonAndPointUnderPointerIfAny(const Coord &pointerSceneCoord, Camera *camera) {
  selectedPolygon = NULL;
  delete selectedPolygonPoint;
  selectedPolygonPoint = NULL;
  Coord pointerScreenCoord(camera->worldTo2DScreen(pointerSceneCoord));

  for (size_t i = 0 ; i < polygons.size() ; ++i) {
    selectedPolygonPoint = polygons[i]->getPolygonVertexUnderPointerIfAny(pointerScreenCoord, camera);

    if (selectedPolygonPoint != NULL) {
      selectedPolygon = polygons[i];
      break;
    }
  }

  if (selectedPolygon == NULL) {
    for (size_t i = 0 ; i < polygons.size() ; ++i) {
      if (polygons[i]->pointInsidePolygon(pointerSceneCoord)) {
        selectedPolygon = polygons[i];
        break;
      }
    }
  }

  for (size_t i = 0 ; i < polygons.size() ; ++i) {
    if (polygons[i] == selectedPolygon) {
      polygons[i]->setSelected(true);
    }
    else {
      polygons[i]->setSelected(false);
    }
  }
}

void ScatterPlotCorrelCoeffSelector::mapPolygonColorToCorrelCoeffOfData(GlEditableComplexPolygon *polygon, GlMainWidget *glWidget) {

  Graph *graph = glWidget->getScene()->getGlGraphComposite()->getInputData()->getGraph();
  Camera &camera = glWidget->getScene()->getLayer("Main")->getCamera();

  BoundingBox polygonSceneBB(polygon->getBoundingBox());
  BoundingBox polygonScrBB;
  polygonScrBB.expand(camera.worldTo2DScreen(Coord(polygonSceneBB[0])));
  polygonScrBB.expand(camera.worldTo2DScreen(Coord(polygonSceneBB[1])));
  vector<Coord> polygonScr;
  const vector<Coord> &polygonVertices = polygon->getPolygonVertices();

  for (size_t i = 0 ; i < polygonVertices.size() ; ++i) {
    polygonScr.push_back(camera.worldTo2DScreen(polygonVertices[i]));
  }

  polygonScr.push_back(camera.worldTo2DScreen(polygonVertices[0]));

  int xStart = static_cast<int>(polygonScrBB[0][0]);
  int yStart = static_cast<int>(polygonScrBB[0][1]);
  int xEnd = static_cast<int>(polygonScrBB[1][0]);
  int yEnd = static_cast<int>(polygonScrBB[1][1]);

  vector<SelectedEntity> tmpNodes;
  vector<SelectedEntity> tmpEdges;
  glWidget->pickNodesEdges(xStart, glWidget->height() - yEnd, (xEnd - xStart), (yEnd - yStart), tmpNodes, tmpEdges);
  vector<node> selectedNodes;
  double correlationCoeff = 0;

  if (!tmpNodes.empty()) {
    GlNode glNode(0);

    for (size_t i = 0 ; i < tmpNodes.size() ; ++i) {
      glNode.id = tmpNodes[i].getComplexEntityId();
      BoundingBox nodeBB (glNode.getBoundingBox(glWidget->getScene()->getGlGraphComposite()->getInputData()));
      float dx = nodeBB[1][0] - nodeBB[0][0];
      float dy = nodeBB[1][1] - nodeBB[0][1];
      float dz = nodeBB[1][2] - nodeBB[0][2];
      const float f = 0.2f;
      nodeBB[0][0] = nodeBB[0][0] + f * dx;
      nodeBB[0][1] = nodeBB[0][1] + f * dy;
      nodeBB[0][2] = nodeBB[0][2] + f * dz;
      nodeBB[1][0] = nodeBB[1][0] - f * dx;
      nodeBB[1][1] = nodeBB[1][1] - f * dy;
      nodeBB[1][2] = nodeBB[1][2] - f * dz;
      vector<float> xVec;
      vector<float> yVec;
      Coord nodeBBBLBScr(camera.worldTo2DScreen(Coord(nodeBB[0])));
      xVec.push_back(nodeBBBLBScr.getX());
      yVec.push_back(nodeBBBLBScr.getY());
      Coord nodeBBTLBScr(camera.worldTo2DScreen(Coord(nodeBB[0][0], nodeBB[1][1], nodeBB[0][2])));
      xVec.push_back(nodeBBTLBScr.getX());
      yVec.push_back(nodeBBTLBScr.getY());
      Coord nodeBBTRBScr(camera.worldTo2DScreen(Coord(nodeBB[1][0], nodeBB[1][1], nodeBB[0][2])));
      xVec.push_back(nodeBBTRBScr.getX());
      yVec.push_back(nodeBBTRBScr.getY());
      Coord nodeBBBRBScr(camera.worldTo2DScreen(Coord(nodeBB[1][0], nodeBB[0][1], nodeBB[0][2])));
      xVec.push_back(nodeBBBRBScr.getX());
      yVec.push_back(nodeBBBRBScr.getY());
      Coord nodeBBBLFScr(camera.worldTo2DScreen(Coord(nodeBB[0][0], nodeBB[0][1], nodeBB[1][2])));
      xVec.push_back(nodeBBBLFScr.getX());
      yVec.push_back(nodeBBBLFScr.getY());
      Coord nodeBBTLFScr(camera.worldTo2DScreen(Coord(nodeBB[0][0], nodeBB[1][1], nodeBB[1][2])));
      xVec.push_back(nodeBBTLFScr.getX());
      yVec.push_back(nodeBBTLFScr.getY());
      Coord nodeBBTRFScr(camera.worldTo2DScreen(Coord(nodeBB[1])));
      xVec.push_back(nodeBBTRFScr.getX());
      yVec.push_back(nodeBBTRFScr.getY());
      Coord nodeBBBRFScr(camera.worldTo2DScreen(Coord(nodeBB[1][0], nodeBB[0][1], nodeBB[1][2])));
      xVec.push_back(nodeBBBRFScr.getX());
      yVec.push_back(nodeBBBRFScr.getY());
      vector<Coord> quad;
      quad.push_back(Coord(*min_element(xVec.begin(), xVec.end()), *min_element(yVec.begin(), yVec.end())));
      quad.push_back(Coord(*min_element(xVec.begin(), xVec.end()), *max_element(yVec.begin(), yVec.end())));
      quad.push_back(Coord(*max_element(xVec.begin(), xVec.end()), *max_element(yVec.begin(), yVec.end())));
      quad.push_back(Coord(*max_element(xVec.begin(), xVec.end()), *min_element(yVec.begin(), yVec.end())));
      quad.push_back(quad[0]);

      if (isPolygonAincludesInB(quad, polygonScr)) {
        selectedNodes.push_back(node(tmpNodes[i].getComplexEntityId()));
      }
    }
  }

  if (!selectedNodes.empty()) {
    string xDim(scatterView->getDetailedScatterPlot()->getXDim());
    string yDim(scatterView->getDetailedScatterPlot()->getYDim());
    string xType(graph->getProperty(xDim)->getTypename());
    string yType(graph->getProperty(yDim)->getTypename());
    DoubleProperty *xDoubleProp = NULL, *yDoubleProp = NULL;
    IntegerProperty *xIntProp = NULL, *yIntProp = NULL;

    if (xType == "double") {
      xDoubleProp = graph->getProperty<DoubleProperty>(xDim);
    }
    else {
      xIntProp = graph->getProperty<IntegerProperty>(xDim);
    }

    if (yType == "double") {
      yDoubleProp = graph->getProperty<DoubleProperty>(yDim);
    }
    else {
      yIntProp = graph->getProperty<IntegerProperty>(yDim);
    }

    double sumxiyi = 0, sumxi = 0, sumyi = 0, sumxi2 = 0, sumyi2 = 0;

    for (size_t i = 0 ; i < selectedNodes.size() ; ++i) {
      node n = selectedNodes[i];
      double xValue = 0, yValue = 0;

      if (xDoubleProp) {
        xValue = xDoubleProp->getNodeValue(n);
      }
      else {
        xValue = static_cast<double>(xIntProp->getNodeValue(n));
      }

      sumxi += xValue;
      sumxi2 += (xValue * xValue);

      if (yDoubleProp) {
        yValue = yDoubleProp->getNodeValue(n);
      }
      else {
        yValue = static_cast<double>(yIntProp->getNodeValue(n));
      }

      sumyi += yValue;
      sumyi2 += (yValue * yValue);
      sumxiyi += (xValue * yValue);
    }

    double numerator = sumxiyi - (1./selectedNodes.size()) * sumxi * sumyi;
    double denominator = sqrt(sumxi2 - (1./selectedNodes.size()) * (sumxi * sumxi)) * sqrt(sumyi2 - (1./selectedNodes.size()) * (sumyi * sumyi));

    if (denominator == 0) {
      correlationCoeff = 0.0;
    }
    else {
      correlationCoeff = numerator / denominator;
    }

    Color startColor(optionsWidget->getZeroColor()), endColor, polygonColor;

    if (correlationCoeff < 0) {
      endColor = optionsWidget->getMinusOneColor();
    }
    else {
      endColor = optionsWidget->getOneColor();
    }

    for (unsigned int i = 0; i < 4; ++i) {
      polygonColor[i] = static_cast<unsigned char>((double(startColor[i]) + (double(endColor[i])- double(startColor[i])) * abs(correlationCoeff)));
    }

    polygon->setColor(polygonColor);
  }
  else {
    polygon->setColor(optionsWidget->getZeroColor());
  }

  polygonsToNodesSubsetAndCorrelCoeff[polygon] = make_pair(selectedNodes, correlationCoeff);
}

}
