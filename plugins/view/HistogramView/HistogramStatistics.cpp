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

#include <tulip/OpenGlConfigManager.h>
#include <tulip/ForEach.h>
#include <tulip/GlLines.h>
#include <tulip/GlMainWidget.h>
#include <tulip/Camera.h>

#include <algorithm>

#include <QEvent>

#include "HistoStatsConfigWidget.h"
#include "HistogramView.h"

#if defined(_MSC_VER) || (defined(__clang_major__) && __clang_major__ > 4)

#include <functional>

// when compose2 is missing, use implementation from boost

/* class for the compose_f_gx_hx adapter
 */
template <class OP1, class OP2, class OP3>
class compose_f_gx_hx_t
  : public std::unary_function<typename OP2::argument_type,
      typename OP1::result_type> {
private:
  OP1 op1;    // process: op1(op2(x),op3(x))
  OP2 op2;
  OP3 op3;
public:
  // constructor
  compose_f_gx_hx_t (const OP1& o1, const OP2& o2, const OP3& o3)
    : op1(o1), op2(o2), op3(o3) {
  }

  // function call
  typename OP1::result_type
  operator()(const typename OP2::argument_type& x) const {
    return op1(op2(x),op3(x));
  }
};

/* convenience functions for the compose_f_gx_hx adapter
 */
template <class OP1, class OP2, class OP3>
inline compose_f_gx_hx_t<OP1,OP2,OP3>
compose_f_gx_hx (const OP1& o1, const OP2& o2, const OP3& o3) {
  return compose_f_gx_hx_t<OP1,OP2,OP3>(o1,o2,o3);
}

#define compose_fn compose_f_gx_hx

#else

#include <ext/functional>
#define compose_fn stdext::compose2

#endif

#include "HistogramStatistics.h"

using namespace std;

template <class K, class V>
class map_value_greater_equal : public unary_function<pair<K, V>, bool> {
private:
  V value;
public:

  map_value_greater_equal(const V& v) : value(v) {}

  bool operator() (const pair<K, V> &elem) const {
    return elem.second >= value;
  }
};

template <class K, class V>
class map_value_less_equal : public unary_function<pair<K, V>, bool> {
private:
  V value;
public:

  map_value_less_equal(const V& v) : value(v) {}

  bool operator() (const pair<K, V> &elem) const {
    return elem.second <= value;
  }
};

namespace {
inline double square(double x) {
  return x*x;
}

/*inline double cube(double x) {
return x*x*x;
}*/
}
static void drawComposite(tlp::GlComposite *composite, float lod, tlp::Camera *camera) {
  map<string, tlp::GlSimpleEntity*> glEntities = composite->getGlEntities();
  map<string, tlp::GlSimpleEntity*>::iterator it2;

  for (it2 = glEntities.begin(); it2 != glEntities.end() ; ++it2) {
    tlp::GlSimpleEntity *entity = it2->second;
    tlp::GlComposite *compositeEntity = dynamic_cast<tlp::GlComposite *>(entity);

    if (compositeEntity != NULL) {
      drawComposite(compositeEntity, lod, camera);
    }
    else {
      entity->draw(lod, camera);
    }
  }
}

namespace tlp {

class UniformKernel : public KernelFunction {

public :

  double operator()(double val) {
    if (fabs(val) < 1) {
      return 1./2.;
    }
    else {
      return 0.;
    }
  }
};

class GaussianKernel : public KernelFunction {

public :

  double operator()(double val) {
    return (1./M_PI)*exp(-square(val)/2.);
  }

};

class TriangleKernel : public KernelFunction {

public :

  double operator()(double val) {
    double valAbs = fabs(val);

    if (valAbs < 1) {
      return 1 - valAbs;
    }
    else {
      return 0.;
    }
  }
};

class EpanechnikovKernel : public KernelFunction {

public :

  double operator()(double val) {
    double valAbs = fabs(val);

    if (valAbs < 1) {
      return (3./4.)*(1-square(val));
    }
    else {
      return 0.;
    }
  }

};

class QuarticKernel : public KernelFunction {

public :

  double operator()(double val) {
    double valAbs = fabs(val);

    if (valAbs < 1) {
      return (15./16.)*square(1-square(val));
    }
    else {
      return 0.;
    }
  }
};

class CubicKernel : public KernelFunction {

public :

  double operator()(double val) {
    double valAbs = fabs(val);

    if (valAbs < 1.) {
      double d = (35./32.) * pow((1. - square(val)), 3);
      return d;
    }
    else {
      return 0.;
    }
  }
};

class CosineKernel : public KernelFunction {

public :

  double operator()(double val) {
    double valAbs = fabs(val);

    if (valAbs < 1) {
      return (M_PI/4.)*cos((M_PI/2.)*val);
    }
    else {
      return 0.;
    }
  }
};


HistogramStatistics::HistogramStatistics(HistoStatsConfigWidget *ConfigWidget)
  : histoView(NULL) , histoStatsConfigWidget(ConfigWidget),
    propertyMean(0), propertyStandardDeviation(0),
    densityAxis(NULL), meanAxis(NULL), standardDeviationPosAxis(NULL),
    standardDeviationNegAxis(NULL), standardDeviation2PosAxis(NULL), standardDeviation2NegAxis(NULL),
    standardDeviation3PosAxis(NULL), standardDeviation3NegAxis(NULL) {
  initKernelFunctionsMap();
}

HistogramStatistics::HistogramStatistics(const HistogramStatistics &histoStats)
  : histoView(histoStats.histoView), histoStatsConfigWidget(histoStats.histoStatsConfigWidget), propertyMean(0), propertyStandardDeviation(0),
    densityAxis(NULL), meanAxis(NULL), standardDeviationPosAxis(NULL),
    standardDeviationNegAxis(NULL), standardDeviation2PosAxis(NULL), standardDeviation2NegAxis(NULL),
    standardDeviation3PosAxis(NULL), standardDeviation3NegAxis(NULL) {
  initKernelFunctionsMap();
}

HistogramStatistics::~HistogramStatistics() {
  cleanupAxis();

  for (map<QString, KernelFunction *>::iterator it = kernelFunctionsMap.begin() ; it != kernelFunctionsMap.end() ; ++it) {
    delete it->second;
  }
}

void HistogramStatistics::viewChanged(View *view) {
  histoView = static_cast<HistogramView *>(view);
  connect(histoStatsConfigWidget, SIGNAL(computeAndDrawInteractor()), this, SLOT(computeAndDrawInteractor()));
  //computeAndDrawInteractor();
}

void HistogramStatistics::initKernelFunctionsMap() {
  kernelFunctionsMap["Uniform"] = new UniformKernel();
  kernelFunctionsMap["Gaussian"] = new GaussianKernel();
  kernelFunctionsMap["Cubic"] = new CubicKernel();
  kernelFunctionsMap["Quartic"] = new QuarticKernel();
  kernelFunctionsMap["Triangle"] = new TriangleKernel();
  kernelFunctionsMap["Epanechnikov"] = new EpanechnikovKernel();
  kernelFunctionsMap["Cosine"] = new CosineKernel();
}

bool HistogramStatistics::eventFilter(QObject *, QEvent *e) {

  if (e->type() == QEvent::MouseMove) {
    histoView->refresh();
    return true;
  }

  return false;
}

void HistogramStatistics::cleanupAxis() {
  if (densityAxis != NULL) {
    delete densityAxis;
    densityAxis = NULL;
  }

  if (meanAxis != NULL) {
    delete meanAxis;
    meanAxis = NULL;
  }

  if (standardDeviationNegAxis != NULL) {
    delete standardDeviationNegAxis;
    standardDeviationNegAxis = NULL;
    delete standardDeviationPosAxis;
    standardDeviationPosAxis = NULL;
  }

  if (standardDeviation2PosAxis != NULL) {
    delete standardDeviation2PosAxis;
    standardDeviation2PosAxis = NULL;
    delete standardDeviation2NegAxis;
    standardDeviation2NegAxis = NULL;
  }

  if (standardDeviation3PosAxis != NULL) {
    delete standardDeviation3PosAxis;
    standardDeviation3PosAxis = NULL;
    delete standardDeviation3NegAxis;
    standardDeviation3NegAxis = NULL;
  }
}

void HistogramStatistics::computeInteractor() {
  GlQuantitativeAxis *histoXAxis = histoView->getDetailedHistogram()->getXAxis();
  GlQuantitativeAxis *histoYAxis = histoView->getDetailedHistogram()->getYAxis();

  if (histoYAxis == NULL) {
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
  double min ,max;

  if (propertyType == "double") {
    if (histoView->getDataLocation() == NODE) {
      min = graph->getProperty<DoubleProperty>(selectedProperty)->getNodeMin();
      max = graph->getProperty<DoubleProperty>(selectedProperty)->getNodeMax();
    }
    else {
      min = graph->getProperty<DoubleProperty>(selectedProperty)->getEdgeMin();
      max = graph->getProperty<DoubleProperty>(selectedProperty)->getEdgeMax();
    }
  }
  else {
    if (histoView->getDataLocation() == NODE) {
      min = (double) graph->getProperty<IntegerProperty>(selectedProperty)->getNodeMin();
      max = (double) graph->getProperty<IntegerProperty>(selectedProperty)->getNodeMax();
    }
    else {
      min = (double) graph->getProperty<IntegerProperty>(selectedProperty)->getEdgeMin();
      max = (double) graph->getProperty<IntegerProperty>(selectedProperty)->getEdgeMax();
    }
  }

  unsigned int nbElements = 0;

  if (histoView->getDataLocation() == NODE) {
    nbElements = graph->numberOfNodes();
    node n;
    forEach(n, graph->getNodes()) {
      double nodeVal;

      if (propertyType == "double") {
        nodeVal = graph->getProperty<DoubleProperty>(selectedProperty)->getNodeValue(n);
      }
      else {
        nodeVal = (double) graph->getProperty<IntegerProperty>(selectedProperty)->getNodeValue(n);
      }

      graphPropertyValueSet[n.id] = nodeVal;
      propertyMean += nodeVal;

    }

  }
  else {
    nbElements = graph->numberOfEdges();
    edge e;
    forEach(e, graph->getEdges()) {
      double edgeVal;

      if (propertyType == "double") {
        edgeVal = graph->getProperty<DoubleProperty>(selectedProperty)->getEdgeValue(e);
      }
      else {
        edgeVal = (double) graph->getProperty<IntegerProperty>(selectedProperty)->getEdgeValue(e);
      }

      graphPropertyValueSet[e.id] = edgeVal;
      propertyMean += edgeVal;

    }
  }


  propertyMean /= (nbElements);

  for (map<unsigned int, double>::iterator it = graphPropertyValueSet.begin() ; it != graphPropertyValueSet.end() ; ++it) {
    propertyStandardDeviation += square(it->second - propertyMean);
  }

  propertyStandardDeviation = sqrt(propertyStandardDeviation / (nbElements - 1));

  histoStatsConfigWidget->setMinMaxMeanAndSd(min, max, propertyMean, propertyStandardDeviation);

  if (histoStatsConfigWidget->densityEstimation()) {
    double bandwidth = histoStatsConfigWidget->getBandwidth();

    vector<double> estimatedDensity;
    float maxDensityValue = 0.;

    KernelFunction *kf = kernelFunctionsMap[histoStatsConfigWidget->getKernelFunctionName()];

    for (double val = min ; val <= max ; val += sampleStep) {
      float fx = 0;

      for (map<unsigned, double>::iterator it = graphPropertyValueSet.begin() ; it != graphPropertyValueSet.end() ; ++it) {
        fx +=  (*kf)((val - (it->second)) / (bandwidth / 2.));
      }

      fx *= (1. / (float) (graphPropertyValueSet.size() * (bandwidth / 2.)));
      estimatedDensity.push_back(fx);

      if (fx > maxDensityValue) {
        maxDensityValue = fx;
      }
    }

    double val;
    unsigned int i;

    for (val = min , i = 0 ; val <= max ; val += sampleStep , ++i) {
      float x = histoXAxis->getAxisPointCoordForValue(val).getX();
      float y = histoYAxis->getAxisPointCoordForValue((estimatedDensity[i] * (histoView->getDetailedHistogram()->getMaxBinSize())) / maxDensityValue).getY();
      densityEstimationCurvePoints.push_back(Coord(x, y, 0));
    }

    densityAxis = new GlQuantitativeAxis("density", Coord(histoXAxis->getAxisBaseCoord().getX() + histoXAxis->getAxisLength(), 0, 0),
                                         histoYAxis->getAxisLength(), GlAxis::VERTICAL_AXIS, Color(255,0,0), true);
    densityAxis->setAxisParameters((double) 0 ,(double) maxDensityValue, 15, GlAxis::RIGHT_OR_ABOVE, true);
    densityAxis->updateAxis();
    densityAxis->addCaption(GlAxis::LEFT, densityAxis->getSpaceBetweenAxisGrads(), false);
  }

  if (histoStatsConfigWidget->displayMeanAndStandardDeviation()) {
    float axisExtension = 2 * histoXAxis->getAxisGradsWidth();
    float y = histoXAxis->getAxisBaseCoord().getY() - axisExtension;
    float axisLength = histoYAxis->getAxisLength() + axisExtension;
    float captionHeight = histoXAxis->getAxisGradsWidth();
    float x = histoXAxis->getAxisPointCoordForValue(propertyMean).getX();
    meanAxis = new GlAxis("m", Coord(x, y, 0), axisLength, GlAxis::VERTICAL_AXIS, Color(255,0,0));
    meanAxis->addCaption(GlAxis::LEFT, captionHeight, false);
    x = histoXAxis->getAxisPointCoordForValue(propertyMean + propertyStandardDeviation).getX();
    standardDeviationPosAxis = new GlAxis("+sd", Coord(x, y, 0), axisLength, GlAxis::VERTICAL_AXIS, Color(255,0,0));
    standardDeviationPosAxis->addCaption(GlAxis::LEFT, captionHeight, false);
    x = histoXAxis->getAxisPointCoordForValue(propertyMean - propertyStandardDeviation).getX();
    standardDeviationNegAxis = new GlAxis("-sd", Coord(x, y, 0), axisLength, GlAxis::VERTICAL_AXIS, Color(255,0,0));
    standardDeviationNegAxis->addCaption(GlAxis::LEFT, captionHeight, false);

    if (propertyMean - 2 * propertyStandardDeviation > min) {
      x = histoXAxis->getAxisPointCoordForValue(propertyMean + 2 * propertyStandardDeviation).getX();
      standardDeviation2PosAxis = new GlAxis("+2sd", Coord(x, y, 0), axisLength, GlAxis::VERTICAL_AXIS, Color(255,0,0));
      standardDeviation2PosAxis->addCaption(GlAxis::LEFT, captionHeight, false);
      x = histoXAxis->getAxisPointCoordForValue(propertyMean - 2 * propertyStandardDeviation).getX();
      standardDeviation2NegAxis = new GlAxis("-2sd", Coord(x, y, 0), axisLength, GlAxis::VERTICAL_AXIS, Color(255,0,0));
      standardDeviation2NegAxis->addCaption(GlAxis::LEFT, captionHeight, false);
    }
    else {
      standardDeviation2NegAxis = NULL;
      standardDeviation2PosAxis = NULL;
    }

    if (propertyMean - 3 * propertyStandardDeviation > min) {
      x = histoXAxis->getAxisPointCoordForValue(propertyMean + 3 * propertyStandardDeviation).getX();
      standardDeviation3PosAxis = new GlAxis("+3sd", Coord(x, y, 0), axisLength, GlAxis::VERTICAL_AXIS, Color(255,0,0));
      standardDeviation3PosAxis->addCaption(GlAxis::LEFT, captionHeight, false);
      x = histoXAxis->getAxisPointCoordForValue(propertyMean - 3 * propertyStandardDeviation).getX();
      standardDeviation3NegAxis = new GlAxis("-3sd", Coord(x, y, 0), axisLength, GlAxis::VERTICAL_AXIS, Color(255,0,0));
      standardDeviation3NegAxis->addCaption(GlAxis::LEFT, captionHeight, false);
    }
    else {
      standardDeviation3NegAxis = NULL;
      standardDeviation3PosAxis = NULL;
    }

    if (histoStatsConfigWidget->nodesSelection()) {
      Observable::holdObservers();
      BooleanProperty *viewSelection = graph->getProperty<BooleanProperty>("viewSelection");
      viewSelection->setAllNodeValue(false);
      viewSelection->setAllEdgeValue(false);
      double lowerBound = histoStatsConfigWidget->getSelectionLowerBound();
      double upperBound = histoStatsConfigWidget->getSelectionUpperBound();
      map<unsigned int, double>::iterator pos = find_if(graphPropertyValueSet.begin(), graphPropertyValueSet.end(),
          compose_fn(logical_and<bool>(), map_value_greater_equal<unsigned int, double>(lowerBound),
                     map_value_less_equal<unsigned int, double>(upperBound)));

      while (pos != graphPropertyValueSet.end()) {
        if (histoView->getDataLocation() == EDGE) {
          viewSelection->setNodeValue(node(pos->first), true);
        }
        else {
          viewSelection->setEdgeValue(edge(pos->first), true);
        }

        pos = find_if(++pos, graphPropertyValueSet.end(),
                      compose_fn(logical_and<bool>(), map_value_greater_equal<unsigned int, double>(lowerBound),
                                 map_value_less_equal<unsigned int, double>(upperBound)));
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
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);

  if (!densityEstimationCurvePoints.empty()) {
    Color curveColor(255,0,0);
    Coord startPoint(densityEstimationCurvePoints[0]);
    Coord endPoint(densityEstimationCurvePoints[densityEstimationCurvePoints.size() - 1]);
    vector<Coord> densityEstimationCurvePointsCp(densityEstimationCurvePoints);
    densityEstimationCurvePointsCp.erase(densityEstimationCurvePointsCp.begin());
    densityEstimationCurvePointsCp.pop_back();
    GlLines::glDrawCurve(startPoint, densityEstimationCurvePointsCp, endPoint, 2., GlLines::TLP_PLAIN, curveColor, curveColor);
    drawComposite(densityAxis, 0, &camera);
  }

  if (meanAxis != NULL) {
    drawComposite(meanAxis, 0, &camera);
  }

  if (standardDeviationPosAxis != NULL) {
    drawComposite(standardDeviationPosAxis, 0, &camera);
    drawComposite(standardDeviationNegAxis, 0, &camera);
  }

  if (standardDeviation2PosAxis != NULL) {
    drawComposite(standardDeviation2PosAxis, 0, &camera);
    drawComposite(standardDeviation2NegAxis, 0, &camera);
  }

  if (standardDeviation3PosAxis != NULL) {
    drawComposite(standardDeviation3PosAxis, 0, &camera);
    drawComposite(standardDeviation3NegAxis, 0, &camera);
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  return true;
}

}
