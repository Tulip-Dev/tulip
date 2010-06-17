/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#ifndef VIEWPLUGINEXAMPLE_H
#define VIEWPLUGINEXAMPLE_H

#include <QtGui/QLineEdit>
#include <QtGui/QGridLayout>
#include <tulip/Interactor.h>
#include <tulip/tulipconf.h>
#include <tulip/View.h>
#include <tulip/Graph.h>

// For this example we construct an simple view who display number of nodes/edges
class ViewPluginExample : public tlp::View
{
  Q_OBJECT
    
public:

  // Contruction of the view (this function is call by the controller when you open the view)
  QWidget *construct(QWidget *parent) 
  {
	// Create a QWidget and add some label on it 
	widget = new QWidget (parent);
	widget->resize (230, 60);
	QGridLayout *grid_layout = new QGridLayout (widget);
	grid_layout->setSpacing(0);
	grid_layout->setMargin(0);

	node_number = new QLineEdit (widget);
	node_number->setReadOnly(true);
	grid_layout->addWidget(node_number, 0, 0, 1, 1);

	edge_number = new QLineEdit (widget);
	edge_number->setReadOnly(true);
	grid_layout->addWidget(edge_number, 1, 0, 1, 1);

	// Return created widget 
	// Controller will add it to a new window
 	return widget;
  }

  // This function is call by the controller to link graph to this view
  void setData(tlp::Graph *graph,tlp::DataSet dataSet) 
  {
    // Store graph
    this->graph=graph;
    // Draw the view
    draw();
  }
   
  // This function is call by the controller to save this view
  void getData(tlp::Graph **graph,tlp::DataSet *dataSet) 
  {
    *graph=this->graph;
  }

  // Return main widget of this view
  QWidget *getWidget() {return widget;}

  // Return actual graph of this view
  tlp::Graph *getGraph() {return graph;}

  // In this example view doesn't have any interactor, so interactors function is empty
  void setInteractors(const std::list<tlp::Interactor *> &interactors) {}
  std::list<tlp::Interactor *> getInteractors(){return std::list<tlp::Interactor *>();}
  void setActiveInteractor(tlp::Interactor *interactor) {}

  // This view can't return picture so this function is empty  
  void createPicture(const std::string &pictureName,int width=0, int height=0) {}

protected :
		
	tlp::Graph *graph;
	QWidget *widget;
	QLineEdit *node_number;
	QLineEdit *edge_number;

public slots :

  // This function is call by the controller when graph data are modified and the view must be draw
  void draw() { 
	node_number->setText(QString("Number of nodes : ")+QString::number(graph->numberOfNodes()));	
	edge_number->setText(QString("Number of edges : ")+QString::number(graph->numberOfEdges()));
  }
  // This function is call by the controller when view have to be redraw (but graph data not modified)
  void refresh() {}
  // This function is call by the controller when graph data are modified
  // In node/link diagram view, in this function, we center view for example
  void init() {}
 
  // This function is call by the controller when the graph is changed
  void setGraph(tlp::Graph *graph) {
    this->graph=graph;
    draw();
  }

};

#endif
