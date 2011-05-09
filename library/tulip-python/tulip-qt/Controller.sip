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

%ModuleHeaderCode
#include <QtGui/QDockWidget>
typedef std::pair<QDockWidget *,QDockWidget *> std_pair_0101QDockWidget_0101QDockWidget;
#include <tulip/MainController.h>
%End

typedef std::pair<QDockWidget *,QDockWidget *> std_pair_0101QDockWidget_0101QDockWidget;

namespace tlp {

class MainWindowFacade {
%TypeHeaderCode
#include <tulip/Controller.h>
%End

public :

	MainWindowFacade();
	
	MainWindowFacade(QMainWindow *mainWindow,QToolBar *toolBar,QToolBar *interactorsToolBar,QWorkspace *workspace);

	QWidget *getParentWidget();
	
	QMenuBar *getMenuBar();
	
	QToolBar *getToolBar();
	
	QToolBar *getInteractorsToolBar();
	
	QWorkspace *getWorkspace();
	
	QStatusBar *getStatusBar();

	void addDockWidget( Qt::DockWidgetArea area, QDockWidget * dockWidget);
	
	void removeDockWidget(QDockWidget* dockWidet);

	void tabifyDockWidget ( QDockWidget * first, QDockWidget * second );

	std::vector<std_pair_0101QDockWidget_0101QDockWidget> getTabifiedDockWidget();

	void setCorner(Qt::Corner corner, Qt::DockWidgetArea area);

	void setDockOptions(QMainWindow::DockOptions options);

	QWidget* centralWidget() const;

	void addToolBar(Qt::ToolBarArea area, QToolBar *toolBar);

	Qt::DockWidgetArea dockWidgetArea(QDockWidget *dock);

	QByteArray saveState(int version = 0);

	bool restoreState(const QByteArray& state, int version = 0);
	
};


class Controller : QObject, tlp::WithParameter, tlp::WithDependency {
%TypeHeaderCode
#include <tulip/Controller.h>
%End

%ConvertToSubClassCode
   if (dynamic_cast<tlp::MainController*>(sipCpp)) {
   		sipType = sipType_tlp_MainController;
   } else if (dynamic_cast<tlp::ControllerViewsManager*>(sipCpp)) {
   		sipType = sipType_tlp_ControllerViewsManager;
   } else {
   		sipType = NULL;
   } 
%End

public:

	virtual ~Controller();
	
	virtual void attachMainWindow(tlp::MainWindowFacade facade);
	
	virtual void setData(tlp::Graph *graph=0, tlp::DataSet dataSet=tlp::DataSet()) =0;
	
	virtual tlp::Graph *getGraph() =0;
	
	virtual void getData(tlp::Graph **graph /Out/, tlp::DataSet *data /Out/) =0;

	tlp::MainWindowFacade* getMainWindowFacade();

	static tlp::Controller *getCurrentController();

signals:

	void willBeClosed();
};

};