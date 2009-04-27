//-*-c++-*-
/*
 Author: Antoine Lambert

 Email : antoine.lambert@labri.fr

 Last modification : 12/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef PARALLEL_AXIS_BASE_H
#define PARALLEL_AXIS_BASE_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <tulip/Graph.h>
#include <tulip/GlComposite.h>
#include <tulip/GlScene.h>
#include <tulip/Observable.h>
#include <tulip/ObservableGraph.h>

#include <QtGui/QWidget>

#include <vector>

#include "ParallelAxis.h"
#include "ParallelCoordinatesGraphProxy.h"

namespace tlp {

class ParallelCoordinatesDrawing : public GlComposite, public GraphObserver {

public :

	enum LayoutType {PARALLEL, CIRCULAR};

	enum LinesType {STRAIGHT, SPLINE};

  ParallelCoordinatesDrawing(ParallelCoordinatesGraphProxy *graphProxy, Graph *axisPointsGraph);

  ~ParallelCoordinatesDrawing();

  bool getDataIdFromGlEntity(GlEntity *glEntity, unsigned int &dataId);
  bool getDataIdFromAxisPoint(node axisPoint, unsigned int &dataId);

  unsigned int nbParallelAxis() const;
  const std::vector<std::string>& getAxisNames() const;
  void swapAxis(ParallelAxis *axis1, ParallelAxis *axis2);
  void removeAxis(ParallelAxis *axis);
  void addAxis(ParallelAxis *axis);

  void setAxisHeight(const unsigned int axisHeight) {this->height = axisHeight;}
  void setSpaceBetweenAxis(const unsigned int spaceBetweenAxis) {this->spaceBetweenAxis = spaceBetweenAxis;}
  void setAxisPointMinSize(const Size &axisPointMinSize) {this->axisPointMinSize = axisPointMinSize;}
  void setAxisPointMaxSize(const Size &axisPointMaxSize) {this->axisPointMaxSize = axisPointMaxSize;}
  void setDrawPointsOnAxis(const bool drawPointsOnAxis) {this->drawPointsOnAxis = drawPointsOnAxis;}
  void setLinesColorAlphaValue(const unsigned int linesColorAlphaValue) {this->linesColorAlphaValue = linesColorAlphaValue;}
  void setLineTextureFilename(std::string lineTextureFilename) {this->lineTextureFilename = lineTextureFilename;}
  void setBackgroundColor(const Color &backgroundColor) {this->backgroundColor = backgroundColor;}
  void setLayoutType(const LayoutType layoutType) {this->layoutType = layoutType;}
  void setLinesType(const LinesType linesType) {this->linesType = linesType;}
  std::vector<ParallelAxis *> getAllAxis();

  void update();
  void updateWithAxisSlidersRange(ParallelAxis *axis, bool multiFiltering);

  void resetAxisSlidersPosition();

  unsigned int getNbDataProcessed() const {return nbDataProcessed;}
  void resetNbDataProcessed() {nbDataProcessed = 0;}

  void addNode(Graph *,const node ){}
  void addEdge(Graph *,const edge ){}
  void delNode(Graph *,const node );
  void delEdge(Graph *,const edge );

 private :

  void computeResizeFactor();
  void createAxis();
  void destroyAxisIfNeeded();
  void plotAllData();
  void plotData(const unsigned int dataIdx, const Color &color);

  void erase();
  void eraseDataPlot();
  void eraseAxisPlot();
  void removeHighlightedElt(const unsigned int dataId);

  unsigned int nbAxis;
  Coord firstAxisPos;
  unsigned int width, height;
  unsigned int spaceBetweenAxis;
  unsigned int lineWidth;
  unsigned int linesColorAlphaValue;
  bool drawPointsOnAxis;


  std::vector<std::string> axisOrder;
  std::map<std::string, ParallelAxis *> parallelAxis;

  std::map<GlEntity *, unsigned int> glEntitiesDataMap;
  std::map<node, unsigned int> axisPointsDataMap;

  ParallelCoordinatesGraphProxy *graphProxy;

  Color backgroundColor;
  std::string lineTextureFilename;
  Size axisPointMinSize;
  Size axisPointMaxSize;
  Size resizeFactor;

  GlComposite *dataPlotComposite;
  GlComposite *axisPlotComposite;

  unsigned int nbDataProcessed;
  bool createAxisFlag;
  std::set<unsigned int> lastHighlightedElements;

  Graph *axisPointsGraph;
  LayoutProperty *axisPointsGraphLayout;
  SizeProperty *axisPointsGraphSize;
  IntegerProperty *axisPointsGraphShape;
  StringProperty *axisPointsGraphLabels;
  ColorProperty *axisPointsGraphColors;
  BooleanProperty *axisPointsGraphSelection;

  LayoutType layoutType;
  LinesType linesType;
};

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif
