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

class Histogram : public GlComposite, public Observable {

public :

  Histogram(Graph *graph, const std::string& propertyName, const ElementType &dataLocation, const Coord &blCorner, unsigned int size, const Color &backgroundColor, const Color &textColor);
  ~Histogram();

  std::string getPropertyName() const {
    return propertyName;
  }
  GlQuantitativeAxis *getXAxis() const {
    return xAxis;
  }
  GlQuantitativeAxis *getYAxis() const {
    return yAxis;
  }
  GlComposite *getBinsComposite() const  {
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

  void treatEvent(const Event &message);

  void afterSetNodeValue(PropertyInterface*, const node);
  void afterSetEdgeValue(PropertyInterface*, const edge);
  void afterSetAllNodeValue(PropertyInterface*);
  void afterSetAllEdgeValue(PropertyInterface*);

  virtual void addNode(Graph *, const node );
  virtual void addEdge(Graph *, const edge );
  virtual void delNode(Graph *,const node );
  virtual void delEdge(Graph *,const edge );

private :

  void computeBoundingBox() {
    GlBoundingBoxSceneVisitor glBBSV(NULL);
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
  std::vector<std::vector<unsigned int> > histogramBins;
  double binWidth;
  unsigned int maxBinSize;
  GlQuantitativeAxis *xAxis, *yAxis;
  bool xAxisLogScale, yAxisLogScale;
  unsigned int nbXGraduations;
  unsigned int yAxisIncrementStep;
  LayoutProperty *histogramLayout;
  SizeProperty *histogramSize;
  GlComposite *histoBinsComposite;
  double min, max;
  float refSize, refSizeX, refSizeY;
  std::vector<std::string> uniformQuantificationAxisLabels;
  bool uniformQuantification;
  bool cumulativeFreqHisto, lastCumulHisto;
  std::map<unsigned int, std::pair<double, double> > binMinMaxMap;
  std::string textureName;
  GlGraphComposite *graphComposite;
  Graph *edgeAsNodeGraph;
  std::map<edge, node> edgeToNode;
  std::map<node, edge> nodeToEdge;
  Color backgroundColor;
  Color textColor;
  bool integerScale;
  ElementType dataLocation;
  bool displayEdges;
  bool layoutUpdateNeeded;
  bool sizesUpdateNeeded;
  bool textureUpdateNeeded;

  int overviewId;
  static int overviewCpt;
};

}

#endif /* HISTOGRAMOVERVIEW_H_ */
