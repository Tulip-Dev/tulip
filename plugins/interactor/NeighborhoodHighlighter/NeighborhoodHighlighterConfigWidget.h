/*
 * NeighbourhoodHighlighterConfigWidget.h
 *
 *  Created on: 2 sept. 2009
 *      Author: antoine
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
