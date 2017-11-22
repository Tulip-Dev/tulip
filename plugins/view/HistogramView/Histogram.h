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

#ifndef HISTOGRAMOVERVIEW_H_
#define HISTOGRAMOVERVIEW_H_

#include <tulip/GlComposite.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlQuantitativeAxis.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>

namespace tlp {

const std::string BIN_RECT_TEXTURE = "histo_texture";

class Histogram : public GlComposite {

public:
  Histogram(Graph *graph, Graph *edgeGraph, std::map<edge, node> &edgeMap,
            const std::string &propertyName, const ElementType &dataLocation, const Coord &blCorner,
            unsigned int size, const Color &backgroundColor, const Color &textColor);
  ~Histogram();

  const std::string &getPropertyName() const {
    return propertyName;
  }
  GlQuantitativeAxis *getXAxis() const {
    return xAxis;
  }
  GlQuantitativeAxis *getYAxis() const {
    return yAxis;
  }
  GlComposite *getBinsComposite() const {
    return histoBinsComposite;
  }
  LayoutProperty *getHistogramLayout() const {
    return histogramLayout;
  }
  SizeProperty *getHistogramSize() const {
    return histogramSize;
  }
  GlGraphComposite *getGlGraphComposite() const {
    return graphComposite;
  }

  void setNbHistogramBins(const unsigned int nbHistogramBins) {
    this->nbHistogramBins = nbHistogramBins;
  }
  unsigned int getNbHistogramBins() const {
    return nbHistogramBins;
  }

  unsigned int getMaxBinSize() const {
    return maxBinSize;
  }
  double getHistogramBinsWidth() const {
    return binWidth;
  }

  void setYAxisIncrementStep(const unsigned int yAxisIncrementStep) {
    this->yAxisIncrementStep = yAxisIncrementStep;
  }
  unsigned int getYAxisIncrementStep() const {
    return yAxisIncrementStep;
  }

  void setLastCumulativeHistogram(const bool lastCumulHist) {
    lastCumulHisto = lastCumulHist;
  }
  void setCumulativeHistogram(const bool cumulHisto) {
    cumulativeFreqHisto = cumulHisto;
  }
  bool cumulativeFrequenciesHistogram() const {
    return cumulativeFreqHisto;
  }

  void setUniformQuantification(const bool uniformQuantification) {
    this->uniformQuantification = uniformQuantification;
  }
  bool uniformQuantificationHistogram() const {
    return uniformQuantification;
  }

  void setXAxisLogScale(const bool xAxisLogScale) {
    this->xAxisLogScale = xAxisLogScale;
  }
  bool xAxisLogScaleSet() const {
    return xAxisLogScale;
  }

  void setYAxisLogScale(const bool yAxisLogScale) {
    this->yAxisLogScale = yAxisLogScale;
  }
  bool yAxisLogScaleSet() const {
    return yAxisLogScale;
  }

  void setNbXGraduations(const unsigned int nbXGraduations) {
    this->nbXGraduations = nbXGraduations;
  }
  unsigned int getNbXGraduations() const {
    return nbXGraduations;
  }

  bool getXAxisScaleDefined() const {
    return xAxisScaleDefined;
  }
  void setXAxisScaleDefined(const bool value) {
    xAxisScaleDefined = value;
  }
  bool getYAxisScaleDefined() const {
    return yAxisScaleDefined;
  }
  void setYAxisScaleDefined(const bool value) {
    yAxisScaleDefined = value;
  }
  const std::pair<double, double> &getXAxisScale() const {
    return xAxisScale;
  }
  void setXAxisScale(const std::pair<double, double> &value) {
    xAxisScale = value;
  }
  const std::pair<double, double> &getYAxisScale() const {
    return yAxisScale;
  }
  void setYAxisScale(const std::pair<double, double> &value) {
    yAxisScale = value;
  }

  const std::pair<double, double> &getInitXAxisScale() const {
    return initXAxisScale;
  }
  void setInitXAxisScale(const std::pair<double, double> &value) {
    initXAxisScale = value;
  }
  const std::pair<double, double> &getInitYAxisScale() const {
    return initYAxisScale;
  }
  void setInitYAxisScale(const std::pair<double, double> &value) {
    initYAxisScale = value;
  }

  void setBLCorner(const Coord &blCorner);

  void setLayoutUpdateNeeded();
  void setSizesUpdateNeeded();
  void setTextureUpdateNeeded();
  void update();

  void setBackgroundColor(const Color &color) {
    backgroundColor = color;
  }
  void setTextColor(const Color &color);

  void setDataLocation(const ElementType &dataLocation);

  void setDisplayGraphEdges(const bool displayGraphEdges) {
    displayEdges = displayGraphEdges;
  }

  bool displayGraphEdges() const {
    return displayEdges;
  }

private:
  void computeBoundingBox() {
    GlBoundingBoxSceneVisitor glBBSV(nullptr);
    acceptVisitor(&glBBSV);
    boundingBox = glBBSV.getBoundingBox();
  }

  void computeHistogram();
  void createAxis();
  void updateLayout();
  void updateSizes();

  Graph *graph;
  std::string propertyName;
  Coord blCorner;
  unsigned int size;
  unsigned int nbHistogramBins;
  std::vector<std::vector<unsigned int>> histogramBins;
  double binWidth;
  unsigned int maxBinSize;
  GlQuantitativeAxis *xAxis, *yAxis;
  bool xAxisLogScale, yAxisLogScale;
  unsigned int nbXGraduations;
  unsigned int yAxisIncrementStep;
  LayoutProperty *histogramLayout, *histogramEdgeLayout;
  SizeProperty *histogramSize;
  GlComposite *histoBinsComposite;
  double min, max;
  float refSize, refSizeX, refSizeY;
  std::vector<std::string> uniformQuantificationAxisLabels;
  bool uniformQuantification;
  bool cumulativeFreqHisto, lastCumulHisto;
  std::map<unsigned int, std::pair<double, double>> binMinMaxMap;
  std::string textureName;
  GlGraphComposite *graphComposite;
  Graph *edgeAsNodeGraph;
  std::map<edge, node> &edgeToNode;
  Color backgroundColor;
  Color textColor;
  bool integerScale;
  ElementType dataLocation;
  bool displayEdges;
  bool layoutUpdateNeeded;
  bool sizesUpdateNeeded;
  bool textureUpdateNeeded;

  bool xAxisScaleDefined, yAxisScaleDefined;
  std::pair<double, double> xAxisScale, yAxisScale;
  std::pair<double, double> initXAxisScale, initYAxisScale;

  int overviewId;
  static int overviewCpt;
};
}

#endif /* HISTOGRAMOVERVIEW_H_ */
