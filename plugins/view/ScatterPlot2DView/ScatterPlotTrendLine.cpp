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

#include <tulip/IntegerProperty.h>
#include <tulip/ForEach.h>
#include <tulip/GlLine.h>
#include <tulip/DoubleProperty.h>
#include <tulip/GlQuantitativeAxis.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlLabel.h>

#include <QEvent>

#include "ScatterPlotTrendLine.h"
#include "ScatterPlot2DView.h"
#include "ScatterPlot2D.h"

using namespace std;

namespace tlp {

// Computes the Linear regression function of a graph on two metrics (y = ax +b)
void computeLinearRegressionFunction(Graph *graph, DoubleProperty *xk, DoubleProperty *yk, float &a, float &b) {
  float sxk, syk, sxkxk, sxkyk;
  sxk   = 0.0f;
  syk   = 0.0f;
  sxkxk = 0.0f;
  sxkyk = 0.0f;

  // We compute the sum of xk, yk, xk² and xkyk for the whole set of nodes
  Iterator<node> *itN = graph->getNodes();

  while(itN->hasNext()) {
    node n = itN->next();
    float nodeValx = xk->getNodeValue(n), nodeValy = yk->getNodeValue(n);
    sxk   += nodeValx;
    sxkxk += (nodeValx * nodeValx);
    syk   += nodeValy;
    sxkyk += (nodeValx * nodeValy);
  }

  delete itN;

  float n = graph->numberOfNodes();

  // Then we compute a and b :
  a = (sxkyk - ((sxk * syk) / n)) / (sxkxk - ((sxk * sxk) / n));
  b = (syk / n) - a * (sxk / n);
}

ScatterPlotTrendLine::ScatterPlotTrendLine() :
  scatterView(NULL), a(0.0f), b(0.0f) {}

ScatterPlotTrendLine:: ~ScatterPlotTrendLine() {}

bool ScatterPlotTrendLine::eventFilter(QObject*, QEvent *e) {
  if (e->type() == QEvent::MouseMove) {
    scatterView->refresh();
    return true;
  }

  return false;
}

bool ScatterPlotTrendLine::draw(GlMainWidget *glMainWidget) {

  ScatterPlot2D *currentScatterPlot = scatterView->getDetailedScatterPlot();

  if (currentScatterPlot == NULL || (a == 0.0f && b == 0.0f)) {
    return false;
  }

  GlQuantitativeAxis *xAxis = currentScatterPlot->getXAxis();
  GlQuantitativeAxis *yAxis = currentScatterPlot->getYAxis();
  float xStart = xAxis->getAxisMinValue();
  float xEnd = xAxis->getAxisMaxValue();

  float yStart = a * xStart + b;
  float yEnd = a * xEnd + b;

  Camera &camera = glMainWidget->getScene()->getLayer("Main")->getCamera();
  camera.initGl();

  GlLine trendLine;
  trendLine.addPoint(Coord(xAxis->getAxisBaseCoord().getX(), yAxis->getAxisPointCoordForValue(yStart).getY(), 0.0f), Color(0,255,0));
  trendLine.addPoint(Coord(xAxis->getAxisBaseCoord().getX() + xAxis->getAxisLength(), yAxis->getAxisPointCoordForValue(yEnd).getY(), 0.0f), Color(0,255,0));
  trendLine.setLineWidth(3);

  glDisable(GL_STENCIL_TEST);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  trendLine.draw(0,0);
  glDisable(GL_BLEND);

  GlLabel lineEquationLabel(Coord(xAxis->getAxisBaseCoord().getX() + xAxis->getAxisLength() + xAxis->getAxisLength() / 8.0f, yAxis->getAxisPointCoordForValue(yEnd).getY(), 0.0f),
                            Size(xAxis->getAxisLength() / 4.0f, yAxis->getAxisLength() / 10.0f), Color(0,255,0));
  ostringstream oss;
  oss << "y = " << a << " * x + " << b;
  lineEquationLabel.setText(oss.str());
  lineEquationLabel.draw(0,&glMainWidget->getScene()->getLayer("Main")->getCamera());

  return true;

}

bool ScatterPlotTrendLine::compute(GlMainWidget*) {
  ScatterPlot2D *currentScatterPlot = scatterView->getDetailedScatterPlot();

  if (currentScatterPlot == NULL) {
    return false;
  }

  Graph *graph = scatterView->getScatterPlotGraph();
  DoubleProperty *xDim, *yDim;
  string xDimName(currentScatterPlot->getXDim());
  string yDimName(currentScatterPlot->getYDim());
  string xDimType(graph->getProperty(xDimName)->getTypename());
  string yDimType(graph->getProperty(yDimName)->getTypename());

  if (xDimType == "double") {
    xDim = graph->getProperty<DoubleProperty>(xDimName);
  }
  else {
    IntegerProperty *xDimInt = graph->getProperty<IntegerProperty>(xDimName);
    xDim = new DoubleProperty(graph);
    node n;
    forEach(n, graph->getNodes()) {
      xDim->setNodeValue(n, static_cast<double>(xDimInt->getNodeValue(n)));
    }
  }

  if (yDimType == "double") {
    yDim = graph->getProperty<DoubleProperty>(yDimName);
  }
  else {
    IntegerProperty *yDimInt = graph->getProperty<IntegerProperty>(yDimName);
    yDim = new DoubleProperty(graph);
    node n;
    forEach(n, graph->getNodes()) {
      yDim->setNodeValue(n, static_cast<double>(yDimInt->getNodeValue(n)));
    }
  }


  computeLinearRegressionFunction(graph, xDim, yDim, a, b);

  if (xDimType == "int") {
    delete xDim;
  }

  if (yDimType == "int") {
    delete yDim;
  }

  return true;
}

void ScatterPlotTrendLine::viewChanged(View *view) {
  if (view == NULL) {
    scatterView = NULL;
    return;
  }

  scatterView = dynamic_cast<ScatterPlot2DView *>(view);
  compute(0);
  scatterView->refresh();
}


}
