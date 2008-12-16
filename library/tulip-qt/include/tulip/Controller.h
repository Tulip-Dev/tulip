#ifndef _Tulip_CONTROLLER_H
#define _Tulip_CONTROLLER_H

#include <QtCore/QObject>
#include <QtGui/QDockWidget>
#include <QtGui/QMainWindow>

#include <tulip/Plugin.h>
#include <tulip/TulipRelease.h>
#include <tulip/TemplateFactory.h>
#include <tulip/Vector.h>
#include "GWInteractor.h"

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

	private:

		QMainWindow *mainWindow;
		QMenuBar *menuBar;
		QToolBar *toolBar;
		QToolBar *interactorsToolBar;
		QWorkspace *workspace;
		QStatusBar *statusBar;

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

  signals:

    void willBeClosed();

  protected :

    MainWindowFacade mainWindowFacade;

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
  string getName() const { return string(N);}	 \
  string getGroup() const { return string(G);}	 \
  string getAuthor() const {return string(A);}	 \
  string getDate() const {return string(D);}	 \
  string getInfo() const {return string(I);}	 \
  string getRelease() const {return string(R);}\
  string getTulipRelease() const {return string(TULIP_RELEASE);} \
  tlp::T * createPluginObject(ControllerContext *ic)		 \
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






