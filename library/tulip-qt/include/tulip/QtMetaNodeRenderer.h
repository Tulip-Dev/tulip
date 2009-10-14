//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 06/04/2009
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/
#ifndef Tulip_QTMETANODERENDERER_H
#define Tulip_QTMETANODERENDERER_H

#include <tulip/tulipconf.h>
#include <tulip/tuliphash.h>
#include <tulip/GlMetaNodeRenderer.h>
#include <tulip/Color.h>

#include <tulip/ObservableGraph.h>
#include <tulip/ObservableProperty.h>

#include "tulip/GlMainView.h"

class QGLFramebufferObject;

namespace tlp {

  class GlGraphInputData;

  /**
   * Class use to render a meta node, this version render nothing
   */
  class TLP_QT_SCOPE QtMetaNodeRenderer : public GlMetaNodeRenderer, public Observer, public GraphObserver, public PropertyObserver{

  public:

    QtMetaNodeRenderer(QWidget *parent, GlMainWidget *widget,GlGraphInputData *inputData);
    ~QtMetaNodeRenderer();

    void update(std::set<Observable *>::iterator begin ,std::set<Observable *>::iterator end) {}
    void observableDestroyed(Observable *) {}

    void addNode(Graph *graph,const node ){haveToRenderGraph[graph]=true;}
    void addEdge(Graph *graph,const edge ){haveToRenderGraph[graph]=true;}
    void delNode(Graph *graph,const node ){haveToRenderGraph[graph]=true;}
    void delEdge(Graph *graph,const edge ){haveToRenderGraph[graph]=true;}
    void reverseEdge(Graph *graph,const edge ){haveToRenderGraph[graph]=true;}
    void destroy(Graph *graph){}
    void addSubGraph(Graph *graph, Graph *){haveToRenderGraph[graph]=true;}
    void delSubGraph(Graph *graph, Graph*){haveToRenderGraph[graph]=true;}
    void addLocalProperty(Graph *graph, const std::string&){

      haveToRenderGraph[graph]=true;
      std::map<Graph *,std::list<Graph *> >::iterator it=parentGraphToGraph.find(graph);
      if(it!=parentGraphToGraph.end()){
        for(std::list<Graph *>::iterator itP=(*it).second.begin();itP!=(*it).second.end();++itP){
          haveToRenderGraph[*itP]=true;
        }
      }
    }
    void delLocalProperty(Graph *graph, const std::string&){

      haveToRenderGraph[graph]=true;
      std::map<Graph *,std::list<Graph *> >::iterator it=parentGraphToGraph.find(graph);
      if(it!=parentGraphToGraph.end()){
        for(std::list<Graph *>::iterator itP=(*it).second.begin();itP!=(*it).second.end();++itP){
          haveToRenderGraph[*itP]=true;
        }
      }
    }

    void checkPropertyInterface(PropertyInterface *property){
      if(propertyToGraph.count(property)!=0){
        std::multimap<PropertyInterface *, Graph*>::iterator itUpper=propertyToGraph.upper_bound(property);
        for(std::multimap<PropertyInterface *, Graph*>::iterator it=propertyToGraph.lower_bound(property);it!=itUpper;++it){
          haveToRenderGraph[(*it).second]=true;
        }
      }
    }

    void beforeSetNodeValue(PropertyInterface *property, const node){checkPropertyInterface(property);}
    void afterSetNodeValue(PropertyInterface *property, const node){checkPropertyInterface(property);}
    void beforeSetEdgeValue(PropertyInterface *property, const edge){checkPropertyInterface(property);}
    void afterSetEdgeValue(PropertyInterface *property, const edge){checkPropertyInterface(property);}
    void beforeSetAllNodeValue(PropertyInterface *property){checkPropertyInterface(property);}
    void afterSetAllNodeValue(PropertyInterface *property){checkPropertyInterface(property);}
    void beforeSetAllEdgeValue(PropertyInterface *property){checkPropertyInterface(property);}
    void afterSetAllEdgeValue(PropertyInterface *property){checkPropertyInterface(property);}
    void destroy(PropertyInterface *property){checkPropertyInterface(property);propertyToGraph.erase(propertyToGraph.lower_bound(property),propertyToGraph.upper_bound(property));}

    void clearObservers();

    virtual void prerender(node n,float lod,Camera *camera);
    virtual void render(node n,float lod,Camera *camera);
    virtual bool havePrerender() {return true;}

    void setBackgroundColor(const Color &color){backgroundColor=color;}
    void setStopUpdateMetaNodes(bool stopUpdate){stopUpdateMetaNodes=stopUpdate;}

  protected :

    QWidget *parent;
    GlMainWidget *parentGlMainWidget;
    std::map<node,std::pair<int,int> > metaNodeTextureSize;
    std::map<Graph *,node> graphToMetaNode;
    std::list<std::string> textureName;

    std::map<Graph *,std::list<Graph *> > parentGraphToGraph;
    std::multimap<PropertyInterface *, Graph*> propertyToGraph;
    TLP_HASH_MAP<Graph *,bool> haveToRenderGraph;

    GlMainWidget *glMainWidget;
    GlGraphInputData *inputData;

    Color backgroundColor;
    bool stopUpdateMetaNodes;

  };

}

#endif // Tulip_QTMETANODERENDERER_H
