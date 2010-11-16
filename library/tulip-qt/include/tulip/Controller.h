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
#ifndef _Tulip_CONTROLLER_H
#define _Tulip_CONTROLLER_H

#include <QtCore/QObject>
#include <QtGui/QDockWidget>
#include <QtGui/QMainWindow>

#include <tulip/Plugin.h>
#include <tulip/TulipRelease.h>
#include <tulip/TemplateFactory.h>
#include <tulip/Vector.h>
#include "Interactor.h"

class QMenuBar;
class QToolBar;
class QWorkspace;
class QStatusBar;

namespace tlp {

	/** \brief Facade of QMainWindow
   *
   * This class provide a facade of QMainWindow
   * It is used by Controller to acces to QMainWindow
   */
	class TLP_QT_SCOPE MainWindowFacade {

	public :

	  /**
	   * basic constructor : no use it
	   */
		MainWindowFacade();
		/**
		 * default constructor
		 */
		MainWindowFacade(QMainWindow *mainWindow,QToolBar *toolBar,QToolBar *interactorsToolBar,QWorkspace *workspace);

		/**
		 * Get the parent widget : use it as parent if you want to create a QDialog (for example)
		 * \warning QWidget returned is QMainWindow but for prevent segfault don't use it as QMainWindow
		 */
		QWidget *getParentWidget() {return mainWindow;}
		/**
		 * get menu bar
		 * \return the menu bar
		 */
		QMenuBar *getMenuBar() {return menuBar;}
		/*
		 * get tools bar
		 * \return the tool bar
		 */
		QToolBar *getToolBar() {return toolBar;}
		/*
		 * get interators tool bar
		 * \return the interactors tool bar
		 */
		QToolBar *getInteractorsToolBar() {return interactorsToolBar;}
		/**
		 * get workspace
		 * \return the workspace
         */
		QWorkspace *getWorkspace() {return workspace;}
		/**
		 * get status bar
		 * \return the status bar
		 */
		QStatusBar *getStatusBar() {return statusBar;}

		/**
		 * Add a dock widget to QMainWindow
		 * This function call QMainWindow::addDockWidget(Qt::DockWidgetArea,QDockWidget)
		 */
		void addDockWidget( Qt::DockWidgetArea area, QDockWidget * dockWidget) {
			mainWindow->addDockWidget(area,dockWidget);
		}

		/**
		 * Moves second dock widget on top of first dock widget, creating a tabbed docked area
		 */
		void tabifyDockWidget ( QDockWidget * first, QDockWidget * second ){
		  mainWindow->tabifyDockWidget(first,second);
		  tabifiedDockWidget.push_back(std::pair<QDockWidget *,QDockWidget *>(first,second));
		}

		/**
		 * Return couple of tabified QDockWidget
		 */
		std::vector<std::pair<QDockWidget *,QDockWidget *> > getTabifiedDockWidget(){
		  return tabifiedDockWidget;
		}
		
		/**
		 * Set the given dock widget area to occupy the specified corner.
		 */
		void setCorner(Qt::Corner corner, Qt::DockWidgetArea area) {
			mainWindow->setCorner(corner, area);
		}
		
		void setDockOptions(QMainWindow::DockOptions options) {
      mainWindow->setDockOptions(options);
		}

	private:

		QMainWindow *mainWindow;
		QMenuBar *menuBar;
		QToolBar *toolBar;
		QToolBar *interactorsToolBar;
		QWorkspace *workspace;
		QStatusBar *statusBar;
		std::vector<std::pair<QDockWidget *,QDockWidget *> > tabifiedDockWidget;

	};

  /** \brief Tulip controller interface class
   *
   *  Tulip controller interface class
   *  If you want to create a new Controller : implement setData and getData and extend attachMainWindow (see attachMainWindow description)
   */
  class TLP_QT_SCOPE Controller : public QObject, public WithParameter, public WithDependency{

    Q_OBJECT;

  public:

    virtual ~Controller() {}

    /**
     * By default in Tulip, if we have only one controller, we auto load it
     * If in your controller, you modify return of this function, you stop this process
     */
    //virtual bool autoLoadController() {return true;}

    /**
     * Attach the main window to this controller
     * Extend this function in your controller because attachMainWindow must construct the GUI
     * \warning In new attachMainWindow function call Controller::attachMainWindow(MainWindowFacade)
     * At default when tulip start with only one controller this controller is autoload and attachMainWindow function is call
     */
    virtual void attachMainWindow(MainWindowFacade facade);
    /**
     * Set data of controller
     * graph and dataSet may be empty
     */
    virtual void setData(Graph *graph=0,DataSet dataSet=DataSet()) =0;
    /**
     * Get the graph and the dataSet of this controller
     * \return the graph and the dataSet who will be store in tlp file
     */
    virtual void getData(Graph **graph,DataSet *data) =0;
    /**
     * Get the graph of this controller
     * \return the graph
     */
    virtual Graph *getGraph() =0;
    /**
     * Return MainWindowFacade use with this controller
     */
    MainWindowFacade* getMainWindowFacade(){
      return &mainWindowFacade;
    }

    /**
     * Set the current active controller
     */
    static void currentActiveController(Controller *controller){
      currentController=controller;
    }

    /**
     * Return the current active controller
     */
    static Controller *getCurrentController() {
      return currentController;
    }

  signals:

    void willBeClosed();

  protected :

    MainWindowFacade mainWindowFacade;

    static Controller *currentController;

  };

  class TLP_QT_SCOPE ControllerContext {

  };

  class TLP_QT_SCOPE ControllerFactory: public Plugin {
  public:
    virtual ~ControllerFactory() {}
    ///
    virtual Controller *createPluginObject(ControllerContext *ic)=0;

    virtual  std::string getMajor() const {
      return tlp::getMajor(getRelease());
    }
    virtual  std::string getMinor() const  {
      return tlp::getMinor(getRelease());
    }
    virtual  std::string getTulipMajor() const {
      return tlp::getMajor(getTulipRelease());
    }
    virtual  std::string getTulipMinor() const  {
      return tlp::getMinor(getTulipRelease());
    }

    static TemplateFactory<ControllerFactory,Controller,ControllerContext *> *factory;
    static void initFactory() {
      if (!factory) {
	factory = new TemplateFactory<ControllerFactory,Controller,ControllerContext *>;
      }
    }
  };

}

#define CONTROLLERPLUGINFACTORY(T,C,N,A,D,I,R,G)     \
class C##T##Factory:public tlp::T##Factory	 \
{                                                \
public:                                          \
  C##T##Factory(){				 \
    initFactory(); 			         \
    factory->registerPlugin(this);	         \
  }       					 \
  std::string getName() const { return std::string(N);}	 \
  std::string getGroup() const { return std::string(G);}	 \
  std::string getAuthor() const {return std::string(A);}	 \
  std::string getDate() const {return std::string(D);}	 \
  std::string getInfo() const {return std::string(I);}	 \
  std::string getRelease() const {return std::string(R);}\
  std::string getTulipRelease() const {return std::string(TULIP_RELEASE);} \
  tlp::T * createPluginObject(tlp::ControllerContext *)		\
  {						 \
    C *tmp = new C();				 \
    return ((tlp::T *) tmp);			 \
  }						 \
};                                               \
extern "C" {                                            \
  C##T##Factory C##T##FactoryInitializer;               \
}

#define CONTROLLERPLUGINOFGROUP(C,N,A,D,I,R,G) CONTROLLERPLUGINFACTORY(Controller,C,N,A,D,I,R,G)
#define CONTROLLERPLUGIN(C,N,A,D,I,R) CONTROLLERPLUGINOFGROUP(C,N,A,D,I,R,"")

#endif






