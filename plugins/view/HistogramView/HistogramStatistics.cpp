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

#include <tulip/OpenGlConfigManager.h>
#include <tulip/GlLines.h>
#include <tulip/GlMainWidget.h>
#include <tulip/Camera.h>

#include <QEvent>

#include "HistoStatsConfigWidget.h"
#include "HistogramView.h"

#include <functional>

#include "HistogramStatistics.h"

using namespace std;

namespace {
inline double square(double x) {
  return x * x;
}
} // namespace

static void drawComposite(tlp::GlComposite *composite, float lod, tlp::Camera *camera) {
  map<string, tlp::GlSimpleEntity *> glEntities = composite->getGlEntities();

  for (auto it2 = glEntities.begin(); it2 != glEntities.end(); ++it2) {
    tlp::GlSimpleEntity *entity = it2->second;
    tlp::GlComposite *compositeEntity = dynamic_cast<tlp::GlComposite *>(entity);

    if (compositeEntity != nullptr) {
      drawComposite(compositeEntity, lod, camera);
    } else {
      entity->draw(lod, camera);
    }
  }
}

namespace tlp {

double UniformKernel(double val) {
  if (fabs(val) < 1) {
    return 1. / 2.;
  } else {
    return 0.;
  }
}

double GaussianKernel(double val) {
  return (1. / M_PI) * exp(-square(val) / 2.);
}

double TriangleKernel(double val) {
  double valAbs = fabs(val);

  if (valAbs < 1) {
    return 1 - valAbs;
  } else {
    return 0.;
  }
}

double EpanechnikovKernel(double val) {
  double valAbs = fabs(val);

  if (valAbs < 1) {
    return (3. / 4.) * (1 - square(val));
  } else {
    return 0.;
  }
}

double QuarticKernel(double val) {
  double valAbs = fabs(val);

  if (valAbs < 1) {
    return (15. / 16.) * square(1 - square(val));
  } else {
    return 0.;
  }
}

double CubicKernel(double val) {
  double valAbs = fabs(val);

  if (valAbs < 1.) {
    double d = (35. / 32.) * pow((1. - square(val)), 3);
    return d;
  } else {
    return 0.;
  }
}

double CosineKernel(double val) {
  double valAbs = fabs(val);

  if (valAbs < 1) {
    return (M_PI / 4.) * cos((M_PI / 2.) * val);
  } else {
    return 0.;
  }
}

HistogramStatistics::HistogramStatistics(HistoStatsConfigWidget *ConfigWidget)
    : histoView(nullptr), histoStatsConfigWidget(ConfigWidget), propertyMean(0),
      propertyStandardDeviation(0), densityAxis(nullptr), meanAxis(nullptr),
      standardDeviationPosAxis(nullptr), standardDeviationNegAxis(nullptr),
      standardDeviation2PosAxis(nullptr), standardDeviation2NegAxis(nullptr),
      standardDeviation3PosAxis(nullptr), standardDeviation3NegAxis(nullptr) {
  initKernelFunctionsMap();
}

HistogramStatistics::HistogramStatistics(const HistogramStatistics &histoStats)
    : histoView(histoStats.histoView), histoStatsConfigWidget(histoStats.histoStatsConfigWidget),
      propertyMean(0), propertyStandardDeviation(0), densityAxis(nullptr), meanAxis(nullptr),
      standardDeviationPosAxis(nullptr), standardDeviationNegAxis(nullptr),
      standardDeviation2PosAxis(nullptr), standardDeviation2NegAxis(nullptr),
      standardDeviation3PosAxis(nullptr), standardDeviation3NegAxis(nullptr) {
  initKernelFunctionsMap();
}

HistogramStatistics::~HistogramStatistics() {
  cleanupAxis();
}

void HistogramStatistics::viewChanged(View *view) {
  histoView = static_cast<HistogramView *>(view);
  connect(histoStatsConfigWidget, SIGNAL(computeAndDrawInteractor()), this,
          SLOT(computeAndDrawInteractor()));
  // computeAndDrawInteractor();
}

void HistogramStatistics::initKernelFunctionsMap() {
  kernelFunctionsMap["Uniform"] = &UniformKernel;
  kernelFunctionsMap["Gaussian"] = &GaussianKernel;
  kernelFunctionsMap["Cubic"] = &CubicKernel;
  kernelFunctionsMap["Quartic"] = &QuarticKernel;
  kernelFunctionsMap["Triangle"] = &TriangleKernel;
  kernelFunctionsMap["Epanechnikov"] = &EpanechnikovKernel;
  kernelFunctionsMap["Cosine"] = &CosineKernel;
}

bool HistogramStatistics::eventFilter(QObject *, QEvent *e) {

  if (e->type() == QEvent::MouseMove) {
    histoView->refresh();
    return true;
  }

  return false;
}

void HistogramStatistics::cleanupAxis() {
  if (densityAxis != nullptr) {
    delete densityAxis;
    densityAxis = nullptr;
  }

  if (meanAxis != nullptr) {
    delete meanAxis;
    meanAxis = nullptr;
  }

  if (standardDeviationNegAxis != nullptr) {
    delete standardDeviationNegAxis;
    standardDeviationNegAxis = nullptr;
    delete standardDeviationPosAxis;
    standardDeviationPosAxis = nullptr;
  }

  if (standardDeviation2PosAxis != nullptr) {
    delete standardDeviation2PosAxis;
    standardDeviation2PosAxis = nullptr;
    delete standardDeviation2NegAxis;
    standardDeviation2NegAxis = nullptr;
  }

  if (standardDeviation3PosAxis != nullptr) {
    delete standardDeviation3PosAxis;
    standardDeviation3PosAxis = nullptr;
    delete standardDeviation3NegAxis;
    standardDeviation3NegAxis = nullptr;
  }
}

void HistogramStatistics::computeInteractor() {
  GlQuantitativeAxis *histoXAxis = histoView->getDetailedHistogram()->getXAxis();
  GlQuantitativeAxis *histoYAxis = histoView->getDetailedHistogram()->getYAxis();

  if (histoYAxis == nullptr) {
    return;
  }

  Graph *graph = histoView->graph();
  string selectedProperty(histoView->getDetailedHistogram()->getPropertyName());

  double sampleStep = histoStatsConfigWidget->getSampleStep();

  graphPropertyValueSet.clear();
  densityEstimationCurvePoints.clear();
  propertyMean = 0;
  propertyStandardDeviation = 0;

  cleanupAxis();

  string propertyType(graph->getProperty(selectedProperty)->getTypename());
  double min, max;

  if (propertyType == "double") {
    if (histoView->getDataLocation() == NODE) {
      min = graph->getProperty<DoubleProperty>(selectedProperty)->getNodeMin();
      max = graph->getProperty<DoubleProperty>(selectedProperty)->getNodeMax();
    } else {
      min = graph->getProperty<DoubleProperty>(selectedProperty)->getEdgeMin();
      max = graph->getProperty<DoubleProperty>(selectedProperty)->getEdgeMax();
    }
  } else {
    if (histoView->getDataLocation() == NODE) {
      min = graph->getProperty<IntegerProperty>(selectedProperty)->getNodeMin();
      max = graph->getProperty<IntegerProperty>(selectedProperty)->getNodeMax();
    } else {
      min = graph->getProperty<IntegerProperty>(selectedProperty)->getEdgeMin();
      max = graph->getProperty<IntegerProperty>(selectedProperty)->getEdgeMax();
    }
  }

  unsigned int nbElements = 0;

  if (histoView->getDataLocation() == NODE) {
    nbElements = graph->numberOfNodes();
    for (auto n : graph->nodes()) {
      double nodeVal;

      if (propertyType == "double") {
        nodeVal = graph->getProperty<DoubleProperty>(selectedProperty)->getNodeValue(n);
      } else {
        nodeVal = graph->getProperty<IntegerProperty>(selectedProperty)->getNodeValue(n);
      }

      graphPropertyValueSet[n.id] = nodeVal;
      propertyMean += nodeVal;
    }

  } else {
    nbElements = graph->numberOfEdges();
    for (auto e : graph->edges()) {
      double edgeVal;

      if (propertyType == "double") {
        edgeVal = graph->getProperty<DoubleProperty>(selectedProperty)->getEdgeValue(e);
      } else {
        edgeVal = graph->getProperty<IntegerProperty>(selectedProperty)->getEdgeValue(e);
      }

      graphPropertyValueSet[e.id] = edgeVal;
      propertyMean += edgeVal;
    }
  }

  propertyMean /= (nbElements);

  for (auto it = graphPropertyValueSet.begin(); it != graphPropertyValueSet.end(); ++it) {
    propertyStandardDeviation += square(it->second - propertyMean);
  }

  propertyStandardDeviation = sqrt(propertyStandardDeviation / (nbElements - 1));

  histoStatsConfigWidget->setMinMaxMeanAndSd(min, max, propertyMean, propertyStandardDeviation);

  if (histoStatsConfigWidget->densityEstimation()) {
    double bandwidth = histoStatsConfigWidget->getBandwidth();

    vector<double> estimatedDensity;
    float maxDensityValue = 0.;

    KernelFunction *kf = kernelFunctionsMap[histoStatsConfigWidget->getKernelFunctionName()];

    for (double val = min; val <= max; val += sampleStep) {
      float fx = 0;

      for (auto it = graphPropertyValueSet.begin(); it != graphPropertyValueSet.end(); ++it) {
        fx += float((*kf)((val - (it->second)) / (bandwidth / 2.)));
      }

      fx *= (1.f / float(graphPropertyValueSet.size() * (bandwidth / 2.)));
      estimatedDensity.push_back(fx);

      if (fx > maxDensityValue) {
        maxDensityValue = fx;
      }
    }

    double val;
    unsigned int i;

    for (val = min, i = 0; val <= max; val += sampleStep, ++i) {
      float x = histoXAxis->getAxisPointCoordForValue(val).getX();
      float y =
          histoYAxis
              ->getAxisPointCoordForValue(
                  (estimatedDensity[i] * (histoView->getDetailedHistogram()->getMaxBinSize())) /
                  maxDensityValue)
              .getY();
      densityEstimationCurvePoints.push_back(Coord(x, y, 0));
    }

    densityAxis = new GlQuantitativeAxis(
        "density", Coord(histoXAxis->getAxisBaseCoord().getX() + histoXAxis->getAxisLength(), 0, 0),
        histoYAxis->getAxisLength(), GlAxis::VERTICAL_AXIS, Color(255, 0, 0), true);
    densityAxis->setAxisParameters(0.0, double(maxDensityValue), 15, GlAxis::RIGHT_OR_ABOVE, true);
    densityAxis->updateAxis();
    densityAxis->addCaption(GlAxis::LEFT, densityAxis->getSpaceBetweenAxisGrads(), false);
  }

  if (histoStatsConfigWidget->displayMeanAndStandardDeviation()) {
    float axisExtension = 2 * histoXAxis->getAxisGradsWidth();
    float y = histoXAxis->getAxisBaseCoord().getY() - axisExtension;
    float axisLength = histoYAxis->getAxisLength() + axisExtension;
    float captionHeight = histoXAxis->getAxisGradsWidth();
    float x = histoXAxis->getAxisPointCoordForValue(propertyMean).getX();
    meanAxis = new GlAxis("m", Coord(x, y, 0), axisLength, GlAxis::VERTICAL_AXIS, Color(255, 0, 0));
    meanAxis->addCaption(GlAxis::LEFT, captionHeight, false);
    x = histoXAxis->getAxisPointCoordForValue(propertyMean + propertyStandardDeviation).getX();
    standardDeviationPosAxis =
        new GlAxis("+sd", Coord(x, y, 0), axisLength, GlAxis::VERTICAL_AXIS, Color(255, 0, 0));
    standardDeviationPosAxis->addCaption(GlAxis::LEFT, captionHeight, false);
    x = histoXAxis->getAxisPointCoordForValue(propertyMean - propertyStandardDeviation).getX();
    standardDeviationNegAxis =
        new GlAxis("-sd", Coord(x, y, 0), axisLength, GlAxis::VERTICAL_AXIS, Color(255, 0, 0));
    standardDeviationNegAxis->addCaption(GlAxis::LEFT, captionHeight, false);

    if (propertyMean - 2 * propertyStandardDeviation > min) {
      x = histoXAxis->getAxisPointCoordForValue(propertyMean + 2 * propertyStandardDeviation)
              .getX();
      standardDeviation2PosAxis =
          new GlAxis("+2sd", Coord(x, y, 0), axisLength, GlAxis::VERTICAL_AXIS, Color(255, 0, 0));
      standardDeviation2PosAxis->addCaption(GlAxis::LEFT, captionHeight, false);
      x = histoXAxis->getAxisPointCoordForValue(propertyMean - 2 * propertyStandardDeviation)
              .getX();
      standardDeviation2NegAxis =
          new GlAxis("-2sd", Coord(x, y, 0), axisLength, GlAxis::VERTICAL_AXIS, Color(255, 0, 0));
      standardDeviation2NegAxis->addCaption(GlAxis::LEFT, captionHeight, false);
    } else {
      standardDeviation2NegAxis = nullptr;
      standardDeviation2PosAxis = nullptr;
    }

    if (propertyMean - 3 * propertyStandardDeviation > min) {
      x = histoXAxis->getAxisPointCoordForValue(propertyMean + 3 * propertyStandardDeviation)
              .getX();
      standardDeviation3PosAxis =
          new GlAxis("+3sd", Coord(x, y, 0), axisLength, GlAxis::VERTICAL_AXIS, Color(255, 0, 0));
      standardDeviation3PosAxis->addCaption(GlAxis::LEFT, captionHeight, false);
      x = histoXAxis->getAxisPointCoordForValue(propertyMean - 3 * propertyStandardDeviation)
              .getX();
      standardDeviation3NegAxis =
          new GlAxis("-3sd", Coord(x, y, 0), axisLength, GlAxis::VERTICAL_AXIS, Color(255, 0, 0));
      standardDeviation3NegAxis->addCaption(GlAxis::LEFT, captionHeight, false);
    } else {
      standardDeviation3NegAxis = nullptr;
      standardDeviation3PosAxis = nullptr;
    }

    if (histoStatsConfigWidget->nodesSelection()) {
      Observable::holdObservers();
      BooleanProperty *viewSelection = graph->getProperty<BooleanProperty>("viewSelection");
      viewSelection->setAllNodeValue(false);
      viewSelection->setAllEdgeValue(false);
      double lowerBound = histoStatsConfigWidget->getSelectionLowerBound();
      double upperBound = histoStatsConfigWidget->getSelectionUpperBound();
      auto val_in_bounds = [lowerBound, upperBound](const pair<unsigned int, double> &map_pair) {
        return (map_pair.second >= lowerBound) && (map_pair.second <= upperBound);
      };
      map<unsigned int, double>::iterator pos =
          find_if(graphPropertyValueSet.begin(), graphPropertyValueSet.end(), val_in_bounds);

      while (pos != graphPropertyValueSet.end()) {
        if (histoView->getDataLocation() == NODE) {
          viewSelection->setNodeValue(node(pos->first), true);
        } else {
          viewSelection->setEdgeValue(edge(pos->first), true);
        }

        pos = find_if(++pos, graphPropertyValueSet.end(), val_in_bounds);
      }

      Observable::unholdObservers();
    }
  }
}

void HistogramStatistics::computeAndDrawInteractor() {
  computeInteractor();
  histoView->refresh();
}

bool HistogramStatistics::draw(GlMainWidget *glMainWidget) {

  Camera &camera = glMainWidget->getScene()->getLayer("Main")->getCamera();
  camera.initGl();
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);

  if (!densityEstimationCurvePoints.empty()) {
    Color curveColor(255, 0, 0);
    Coord startPoint(densityEstimationCurvePoints[0]);
    Coord endPoint(densityEstimationCurvePoints[densityEstimationCurvePoints.size() - 1]);
    vector<Coord> densityEstimationCurvePointsCp(densityEstimationCurvePoints);
    densityEstimationCurvePointsCp.erase(densityEstimationCurvePointsCp.begin());
    densityEstimationCurvePointsCp.pop_back();
    GlLines::glDrawCurve(startPoint, densityEstimationCurvePointsCp, endPoint, 2.,
                         GlLines::TLP_PLAIN, curveColor, curveColor);
    drawComposite(densityAxis, 0, &camera);
  }

  if (meanAxis != nullptr) {
    drawComposite(meanAxis, 0, &camera);
  }

  if (standardDeviationPosAxis != nullptr) {
    drawComposite(standardDeviationPosAxis, 0, &camera);
    drawComposite(standardDeviationNegAxis, 0, &camera);
  }

  if (standardDeviation2PosAxis != nullptr) {
    drawComposite(standardDeviation2PosAxis, 0, &camera);
    drawComposite(standardDeviation2NegAxis, 0, &camera);
  }

  if (standardDeviation3PosAxis != nullptr) {
    drawComposite(standardDeviation3PosAxis, 0, &camera);
    drawComposite(standardDeviation3NegAxis, 0, &camera);
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  return true;
}
} // namespace tlp
