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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (http://booggie.org/)                                                   |
// +-------------------------------------------------------------------------+

namespace tlp {



class ControllerViewsManager : tlp::Controller /Abstract/ {
%TypeHeaderCode
#include <tulip3/ControllerViewsManager.h>
%End

	public :
      
      virtual ~ControllerViewsManager();
    
      void attachMainWindow(tlp::MainWindowFacade facade);
      
      virtual tlp::Graph *getGraph();
      
      virtual tlp::Graph *getCurrentGraph();
      
      virtual void setCurrentGraph(tlp::Graph *graph);
      
      virtual tlp::View *getCurrentView();
      
      virtual unsigned int getViewsNumber()const;
     
      virtual tlp::Graph *getGraphOfView(tlp::View *view);
     
      virtual std::vector<tlp::View*> getViewsOfGraph(tlp::Graph *graph);

      virtual void setGraphOfView(tlp::View *view, tlp::Graph *graph);

      virtual void setDataOfView(tlp::View* view, tlp::Graph* graph, const tlp::DataSet& dataSet);

      virtual tlp::View *getViewOfWidget(QWidget *widget);
     
      virtual void setViewOfWidget(QWidget *widget, tlp::View *view);
      
      virtual void getViews(std::vector<tlp::View*> &views /Out/);

      virtual QWidget *getWidgetOfView(tlp::View *view);

      virtual std::string getNameOfView(tlp::View *view) const;
      
      virtual void setNameOfView(tlp::View *view, const std::string &name);

      virtual QWidget *getInteractorConfigurationWidgetOfView(tlp::View *view);

      virtual void changeWindowTitle(tlp::Graph *graph);
      
      virtual void closeAllViews();

      virtual void closeView(tlp::View *view);

      virtual void closeViewsRelatedToGraph(tlp::Graph* graph);

      virtual tlp::View* createView(const std::string &name, tlp::Graph *graph, tlp::DataSet dataSet, bool forceWidgetSize, const QRect &rect, bool maximized);

      void addView(tlp::View *view, tlp::Graph *graph, tlp::DataSet dataSet, bool forceWidgetSize, const QRect &rect, bool maximized, const std::string &viewName, QWidget *viewWidget);
      
      virtual void installInteractors(tlp::View *view);
      
      virtual void updateViewsOfGraph(tlp::Graph *graph);
      
      virtual void updateViewsOfSubGraphs(tlp::Graph *graph);
      
      virtual void changeGraphOfViews(tlp::Graph *oldGraph, tlp::Graph *newGraph);
      
      virtual void drawViews(bool init=false);
      
      virtual void saveViewsGraphsHierarchies();
      
      virtual void checkViewsGraphsHierarchy();
      
      
      
  };

};

