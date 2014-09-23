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

#ifndef NEIGHBOURHOODHIGHLIGHTERINTERACTOR_H_
#define NEIGHBOURHOODHIGHLIGHTERINTERACTOR_H_

#include "NeighborhoodHighlighterConfigWidget.h"
#include "NodeNeighborhoodView.h"
#include "../../utils/StandardInteractorPriority.h"

#include <QObject>

#include <tulip/GLInteractor.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlLayer.h>
#include <tulip/GlScene.h>

namespace tlp {

class AdditionalGlSceneAnimation;

/*@{*/
/** \file
 *  \brief  Tulip Node Neighbourhood Highlighter

 * This interactor plugin allow to get information regarding the neighbourhood of a node by highlighting
 * the nodes connected to it. A "Bring and Go" feature is also implemented allowing to navigate
 */
class NeighborhoodHighlighterInteractor : public GLInteractorComposite {

public :

  PLUGININFORMATION("NeighborhoodHighlighterInteractor", "Antoine Lambert", "19/05/2009", "Node neighborhood highlighter", "1.0", "Navigation")

  NeighborhoodHighlighterInteractor(const PluginContext *);

  virtual ~NeighborhoodHighlighterInteractor();

  void construct();

  QWidget *configurationWidget() const {
    return configWidget;
  }

  unsigned int priority() const {
    return StandardInteractorPriority::NeighborhoodHighlighter;
  }

  virtual bool isCompatible(const std::string &viewName) const;

private :

  NeighborhoodHighlighterConfigWidget *configWidget;

};


class NeighborhoodHighlighter : public GLInteractorComponent {

  Q_OBJECT

public :

  NeighborhoodHighlighter();

  NeighborhoodHighlighter(const NeighborhoodHighlighter &neighborhoodHighlighter);

  ~NeighborhoodHighlighter();

  bool eventFilter(QObject *widget, QEvent *e);

  bool draw(GlMainWidget *glMainWidget);

  void viewChanged(View *view);

  void setConfigWidget(NeighborhoodHighlighterConfigWidget *configWidget) {
    this->configWidget = configWidget;
  }

public slots :

  void updateNeighborhoodGraph();

  void morphCircleAlphaAnimStep(int animStep);

private :

  node selectNodeInOriginalGraph(GlMainWidget *glWidget, int x, int y);

  void buildNeighborhoodGraph(node n, Graph *g);

  void computeNeighborhoodGraphCircleLayout();

  float computeNeighborhoodGraphRadius(LayoutProperty *neighborhoodGraphLayoutProp);

  void cleanupNeighborhoodGraph();

  bool selectInAugmentedDisplayGraph(const int x, const int y, SelectedEntity &);

  void updateNeighborhoodGraphLayoutAndColors();

  void updateGlNeighborhoodGraph();

  void computeNeighborhoodGraphBoundingBoxes();

  void performZoomAndPan(const BoundingBox &destBB, AdditionalGlSceneAnimation *additionalAnimation=NULL);

  void morphCircleAlpha(unsigned char startAlpha, unsigned endAlpha, int nbAnimationSteps=40);

  Graph *originalGraph;
  GlGraphComposite *originalGlGraphComposite;
  node selectedNode;
  NodeNeighborhoodView *neighborhoodGraph;
  GlGraphComposite *glNeighborhoodGraph;
  Camera *glNeighborhoodCamera;
  Coord circleCenter;

  node neighborhoodGraphCentralNode;
  LayoutProperty *neighborhoodGraphLayout;
  LayoutProperty *neighborhoodGraphCircleLayout;
  LayoutProperty *neighborhoodGraphOriginalLayout;
  ColorProperty *neighborhoodGraphColors;
  ColorProperty *neighborhoodGraphBackupColors;

  bool centralNodeLocked;
  bool circleLayoutSet;
  GlMainWidget *glWidget;
  node selectedNeighborNode;
  unsigned int neighborhoodDist;

  NeighborhoodHighlighterConfigWidget *configWidget;

  BoundingBox neighborhoodGraphCircleLayoutBB, neighborhoodGraphOriginalLayoutBB;

  unsigned char circleAlphaValue;
  unsigned char startAlpha, endAlpha;
  int nbAnimSteps;

};

}

#endif /* NEIGHBOURHOODHIGHLIGHTERINTERACTOR_H_ */
