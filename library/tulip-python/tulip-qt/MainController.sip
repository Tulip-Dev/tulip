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

class MainController : tlp::ControllerViewsManager  {
%TypeHeaderCode
#include <tulip/MainController.h>
%End

  public:

	MainController();

	virtual ~MainController();

	void attachMainWindow(tlp::MainWindowFacade facade);

    void getData(tlp::Graph **graph /Out/, tlp::DataSet *dataSet /Out/);
    
    void setData(tlp::Graph *graph=0, tlp::DataSet dataSet=tlp::DataSet());
    
  protected :

    virtual tlp::View* createView(const std::string &name, tlp::Graph *graph, tlp::DataSet dataSet, bool forceWidgetSize=true, const QRect &rect=QRect(0,0,0,0), bool maximized=false);

    virtual void drawViews(bool init=false);
    
    void buildMenu();
    
    
  };

};

