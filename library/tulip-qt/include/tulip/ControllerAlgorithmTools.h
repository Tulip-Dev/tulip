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
#ifndef Tulip_CONTROLLERALGORITHMTOOLS_H
#define Tulip_CONTROLLERALGORITHMTOOLS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <QtGui/QWidget>

#include <tulip/Reflect.h>
#include <tulip/TemplateFactory.h>
#include <tulip/Graph.h>

#include "tulip/View.h"

namespace tlp {
  
  /** \brief Algorithm tools for Controller 
   *
   * This class contain tools to use algorithm in Controllers
   */
  class TLP_QT_SCOPE ControllerAlgorithmTools {

    public :
      
      /**
       * Return parameters need by a plugin
       */
      static StructDef *getPluginParameters(TemplateFactoryInterface *factory, std::string name);
      
      /**
       * Apply an algorithm on graph with given name and given dataSet, if the algorithm has a problem : display it in a QDialog in parent widget and return false
       */
      static bool applyAlgorithm(Graph *graph,QWidget *parent,const std::string &name,DataSet *dataSet);
      /**
       * Apply an algorithm on graph with given name, and if the algorithm has a problem : display it in a QDialog in parent widget and return false
       */
      static bool applyAlgorithm(Graph *graph,QWidget *parent,const std::string &name);
      /**
       * Change a property on graph with algorithm with given name and store result in destination property
       * view is use to preview result if redraw are at true
       * if query is at true, open a parameters widget (if need)
       * if push is at true, save graph before modify it with push/pop system
       * if algorithm has a problem : display it in a QDialog in parent widget
       */
      template<typename PROPERTY> static bool changeProperty(Graph *graph,QWidget *parent,std::string name, std::string destination,View *view=NULL, bool query=true, bool redraw=false, bool push=true);
      /**
       * Change a property on graph with algorithm with given name and store result in destination property
       * Similar to previous changeProperty function but in this one you have to pass a dataset, this dataset is use by the algorithm
       */
      template<typename PROPERTY> static bool changeProperty(Graph *graph,QWidget *parent,std::string name, std::string destination,DataSet dataSet,View *view=NULL, bool query=true, bool redraw=false, bool push=true);
      /**
       * Apply a string property algorithm with given name on property with name : propertyName on graph
       * if algorithm has a problem : display it in a QDialog in parent widget
       * view is use to preview result
       */
      static bool changeString(Graph *graph,QWidget *parent,const std::string &name,const std::string &propertyName,View *view=NULL);
      /**
       * Apply a boolean property algorithm with given name on property with name : propertyName on graph
       * if algorithm has a problem : display it in a QDialog in parent widget
       * view is use to preview result
       */
      static bool changeBoolean(Graph *graph,QWidget *parent,const std::string &name,const std::string &propertyName,View *view=NULL);
      /**
       * Apply a metric property algorithm with given name on property with name : propertyName on graph
       * if algorithm has a problem : display it in a QDialog in parent widget
       * view is use to preview result
       * if mapMetric is at true : apply a color algoritm after metric algorithm
       */
      static bool changeMetric(Graph *graph,QWidget *parent,const std::string &name,const std::string &propertyName,View *view=NULL,bool mapMetric=false, const std::string &colorAlgorithmName="", const std::string &colorPropertyName="");
      /**
       * Apply a layout property algorithm with given name on property with name : propertyName on graph
       * if algorithm has a problem : display it in a QDialog in parent widget
       * view is use to preview result
       */
      static bool changeLayout(Graph *graph,QWidget *parent,const std::string &name,const std::string &propertyName,View *view=NULL);
      /**
       * Apply an integer property algorithm with given name on property with name : propertyName on graph
       * if algorithm has a problem : display it in a QDialog in parent widget
       * view is use to preview result
       */
      static bool changeInt(Graph *graph,QWidget *parent,const std::string &name,const std::string &propertyName,View *view=NULL);
      /**
       * Apply a color property algorithm with given name on property with name : propertyName on graph
       * if algorithm has a problem : display it in a QDialog in parent widget
       * view is use to preview result
       */
      static bool changeColors(Graph *graph,QWidget *parent,const std::string &name,const std::string &propertyName,View *view=NULL);
      /**
       * Apply a size property algorithm with given name on property with name : propertyName on graph
       * if algorithm has a problem : display it in a QDialog in parent widget
       * view is use to preview result
       */
      static bool changeSizes(Graph *graph,QWidget *parent,const std::string &name,const std::string &propertyName,View *view=NULL);
      
      /**
       * Test if graph is acyclic and display result in a QWidget in parent widget
       */
      static void isAcyclic(Graph *graph,QWidget *parent);
      /**
       * Modify graph to make it acyclic
       * if push is at true : save graph before modification with push/pop system
       */
      static void makeAcyclic(Graph *graph,bool pushGraph=true);
      /**
       * Test if graph is simple and display result in a QWidget in parent widget
       */
      static void isSimple(Graph *graph,QWidget *parent);
      /**
       * Modify graph to make it simple
       * if push is at true : save graph before modification with push/pop system
       */
      static void makeSimple(Graph *graph,bool pushGraph=true);
      /**
       * Test if graph is connected and display result in a QWidget in parent widget
       */
      static void isConnected(Graph *graph,QWidget *parent);
      /**
       * Modify graph to make it connected
       * if push is at true : save graph before modification with push/pop system
       */
      static void makeConnected(Graph *graph,bool pushGraph=true);
      /**
       * Test if graph is biconnected and display result in a QWidget in parent widget
       */
      static void isBiconnected(Graph *graph,QWidget *parent);
      /**
       * Modify graph to make it biconnected
       * if push is at true : save graph before modification with push/pop system
       */
      static void makeBiconnected(Graph *graph,bool pushGraph=true);
      /**
       * Test if graph is triconnected and display result in a QWidget in parent widget
       */
      static void isTriconnected(Graph *graph,QWidget *parent);
      /**
       * Test if graph is a tree and display result in a QWidget in parent widget
       */
      static void isTree(Graph *graph,QWidget *parent);
      /**
       * Test if graph is a free tree and display result in a QWidget in parent widget
       */
      static void isFreeTree(Graph *graph,QWidget *parent);
      /**
       * Modify graph to make it directed
       * if push is at true : save graph before modification with push/pop system
       */
      static void makeDirected(QWidget *parent,Graph *graph,bool pushGraph=true);
      /**
       * Test if graph is planar and display result in a QWidget in parent widget
       */
      static void isPlanar(Graph *graph,QWidget *parent);
      /**
       * Test if graph is outer planar and display result in a QWidget in parent widget
       */
      static void isOuterPlanar(Graph *graph,QWidget *parent);

  };

}

#endif
