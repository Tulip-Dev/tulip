/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#ifndef SCATTERPLOTOVERVIEW_H_
#define SCATTERPLOTOVERVIEW_H_

#include <tulip/Graph.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/GlQuantitativeAxis.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlProgressBar.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/GlRect.h>
#include <tulip/GlLabel.h>

namespace tlp {

class ScatterPlot2D : public GlComposite {

public :

  ScatterPlot2D(Graph *graph, std::string xDim, std::string yDim, Coord blCorner, unsigned int size, const Color &backgroundColor, const Color &foregroundColor);
  ~ScatterPlot2D();

  void setBLCorner(const Coord &blCorner);
  void setUniformBackgroundColor(const Color &backgroundColor);
  void mapBackgroundColorToCorrelCoeff(const bool mapBackgroundColor, const Color &minusOneColor, const Color &zeroColor, const Color &oneColor);
  void setForegroundColor(const Color &foregroundColor);
  void setSizeProperty(SizeProperty *sizeProperty);

  const Color &getBackgroundColor() const {
    return backgroundColor;
  }

  void generateOverview(GlMainWidget *glWidget = NULL, LayoutProperty *reverseLayout = NULL);
  bool overviewGenerated() const {
    return overviewGen;
  }

  std::string getXDim() const {
    return xDim;
  }
  std::string getYDim() const {
    return yDim;
  }
  Coord getOverviewCenter() const;
  float getOverviewSize() const {
    return static_cast<float>(size);
  }
  LayoutProperty *getScatterPlotLayout() const {
    return scatterLayout;
  }
  GlQuantitativeAxis *getXAxis() const {
    return xAxis;
  }
  GlQuantitativeAxis *getYAxis() const {
    return yAxis;
  }

  double getCorrelationCoefficient() const {
    return correlationCoeff;
  }

  GlGraphComposite *getGlGraphComposite() const {
    return glGraphComposite;
  }
  void setDisplayGraphEdges(const bool displayGraphEdges) {
    displayEdges = displayGraphEdges;
  }

private :

  void computeBoundingBox() {
    GlBoundingBoxSceneVisitor glBBSV(NULL);
    acceptVisitor(&glBBSV);
    boundingBox = glBBSV.getBoundingBox();
  }

  void createAxis();
  void computeScatterPlotLayout(GlMainWidget *glWidget, LayoutProperty *reverseLayout);
  void clean();

  std::string xDim, yDim;
  std::string xType, yType;
  Coord blCorner;
  unsigned int size;
  Graph *graph;
  GlGraphComposite *glGraphComposite;
  LayoutProperty *scatterLayout;
  GlQuantitativeAxis *xAxis;
  GlQuantitativeAxis *yAxis;
  std::string textureName;
  GlProgressBar *glProgressBar;
  int currentStep;
  int maxStep;
  int drawStep;
  bool overviewGen;
  Color backgroundColor, foregroundColor;
  GlLabel *clickLabel;
  GlRect *backgroundRect;

  bool mapBackgroundColorToCoeff;
  Color minusOneColor, zeroColor, oneColor;

  double correlationCoeff;

  bool displayEdges;

  int overviewId;
  static int overviewCpt;

};

}

#endif /* SCATTERPLOTOVERVIEW_H_ */
