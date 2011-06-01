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
#ifndef Tulip_CONTROLLERVIEWSTOOLS_H
#define Tulip_CONTROLLERVIEWSTOOLS_H

#include <tulip/tulipconf.h>
#include <tulip/View.h>

#include <QtGui/QWidget>

namespace tlp {
  class Graph;
  /** \brief Views manager tools for Controller 
   *
   * This class contain tools to use views in Controllers
   */
  class TLP3_COMPAT_SCOPE ControllerViewsTools {

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
