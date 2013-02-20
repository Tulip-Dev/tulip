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

#ifndef NEIGHBOURHOODHIGHLIGHTERCONFIGWIDGET_H_
#define NEIGHBOURHOODHIGHLIGHTERCONFIGWIDGET_H_

#include "NodeNeighborhoodView.h"
#include "ui_NeighborhoodHighlighterConfigWidget.h"

class NeighborhoodHighlighterConfigWidget : public QWidget, public Ui::NeighborhoodHighlighterConfigWidgetData {

public :

  NeighborhoodHighlighterConfigWidget(QWidget *parent = 0);

  NodeNeighborhoodView::NeighborNodesType getNeighborsType() const;
  bool computeReachableSubGraph() const;
  void setCurrentMaxDistanceForReachableNodes(unsigned int distance);
  bool bringAndGoAnimation1() const;
  std::string propertyToUse() const;
  int numberOfNodesToBring() const;
  void setPropertyToUse(std::string propertyName);
  void setNumberOfNodes(int nodesNb);
};

#endif /* NEIGHBOURHOODHIGHLIGHTERCONFIGWIDGET_H_ */
