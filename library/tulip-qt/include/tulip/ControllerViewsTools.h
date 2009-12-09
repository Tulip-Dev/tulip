#ifndef Tulip_CONTROLLERVIEWSTOOLS_H
#define Tulip_CONTROLLERVIEWSTOOLS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <QtGui/QWidget>

#include <tulip/Graph.h>
#include <tulip/View.h>


namespace tlp {
  
  /** \brief Views manager tools for Controller 
   *
   * This class contain tools to use views in Controllers
   */
  class TLP_QT_SCOPE ControllerViewsTools {

    public :

      /**
       * Create a view with given name, given graph, given dataSet and set view's widget parent
       * if given name isn't be valid : return a new name in createViewName
       * return created view and created widget in createdView and createdWidget
       */
      static void createView(const std::string &name,Graph *graph,DataSet dataSet,QWidget *parent,std::string *createdViewName, View **createdView, QWidget **createdWidget);
      /**
       * Create a Node Link Diagram Component view with given graph, given dataSet and set view's widget parent
       * return created view and created widget in createdView and createdWidget
       */
      static void createMainView(Graph *graph,DataSet dataSet,QWidget *parent,View **createdView, QWidget **createdWidget);
      /**
       * Install available interactors for given view in toolBar
       */
      static void installInteractors(View *view,QToolBar *toolBar);
      /**
       * Change active interactor on toolBar 
       * Return configuration widget of this interactor in createdConfigurationWidget argument
       */
      static void changeInteractor(View *view,QToolBar *toolBar,QAction* action,QWidget **createdConfigurationWidget); 
      /**
       * Return a default interactor's configuration widget
       */
      static QWidget *getNoInteractorConfigurationWidget();
      
    private :
      
      static QWidget *noInteractorConfigurationWidget;
      
  };

}

#endif
