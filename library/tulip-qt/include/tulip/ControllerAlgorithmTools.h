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
      
      static StructDef *getPluginParameters(TemplateFactoryInterface *factory, std::string name);
      
      static bool applyAlgorithm(Graph *graph,QWidget *parent,const std::string &name);
      template<typename PROPERTY> static bool changeProperty(Graph *graph,QWidget *parent,std::string name, std::string destination,View *view=NULL, bool query=true, bool redraw=false, bool push=true);
      static bool changeString(Graph *graph,QWidget *parent,const std::string &name,const std::string &propertyName,View *view=NULL);
      static bool changeBoolean(Graph *graph,QWidget *parent,const std::string &name,const std::string &propertyName,View *view=NULL);
      static bool changeMetric(Graph *graph,QWidget *parent,const std::string &name,const std::string &propertyName,View *view=NULL,bool mapMetric=false, const std::string &colorAlgorithmName="", const std::string &colorPropertyName="");
      static bool changeLayout(Graph *graph,QWidget *parent,const std::string &name,const std::string &propertyName,View *view=NULL);
      static bool changeInt(Graph *graph,QWidget *parent,const std::string &name,const std::string &propertyName,View *view=NULL);
      static bool changeColors(Graph *graph,QWidget *parent,const std::string &name,const std::string &propertyName,View *view=NULL);
      static bool changeSizes(Graph *graph,QWidget *parent,const std::string &name,const std::string &propertyName,View *view=NULL);
      
      static void isAcyclic(Graph *graph,QWidget *parent);
      static void makeAcyclic(Graph *graph,bool pushGraph=true);
      static void isSimple(Graph *graph,QWidget *parent);
      static void makeSimple(Graph *graph,bool pushGraph=true);
      static void isConnected(Graph *graph,QWidget *parent);
      static void makeConnected(Graph *graph,bool pushGraph=true);
      static void isBiconnected(Graph *graph,QWidget *parent);
      static void makeBiconnected(Graph *graph,bool pushGraph=true);
      static void isTriconnected(Graph *graph,QWidget *parent);
      static void isTree(Graph *graph,QWidget *parent);
      static void isFreeTree(Graph *graph,QWidget *parent);
      static void makeDirected(QWidget *parent,Graph *graph,bool pushGraph=true);
      static void isPlanar(Graph *graph,QWidget *parent);
      static void isOuterPlanar(Graph *graph,QWidget *parent);

  };

}

#endif
