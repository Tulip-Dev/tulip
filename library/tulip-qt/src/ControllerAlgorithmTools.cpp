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
#include "tulip/ControllerAlgorithmTools.h"

#include <QtGui/QMessageBox>

#include <tulip/BooleanProperty.h>
#include <tulip/ForEach.h>
#include <tulip/Algorithm.h>

#include <tulip/AcyclicTest.h>
#include <tulip/SimpleTest.h>
#include <tulip/ConnectedTest.h>
#include <tulip/BiconnectedTest.h>
#include <tulip/TriconnectedTest.h>
#include <tulip/TreeTest.h>
#include <tulip/GraphTools.h>
#include <tulip/PlanarityTest.h>
#include <tulip/OuterPlanarTest.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlCPULODCalculator.h>

#include "tulip/NodeLinkDiagramComponent.h"
#include "tulip/QtProgress.h"
#include "tulip/TlpQtTools.h"

using namespace std;

namespace tlp {

  StructDef *ControllerAlgorithmTools::getPluginParameters(TemplateFactoryInterface *factory, std::string name) {
    static TLP_HASH_MAP<unsigned long, TLP_HASH_MAP<std::string, StructDef * > > paramMaps;
    TLP_HASH_MAP<std::string, StructDef *>::const_iterator it;
    it = paramMaps[(unsigned long) factory].find(name);
    if (it == paramMaps[(unsigned long) factory].end())
      paramMaps[(unsigned long) factory][name] = new StructDef(factory->getPluginParameters(name));
    return paramMaps[(unsigned long) factory][name];
  }
  //**********************************************************************
  bool ControllerAlgorithmTools::applyAlgorithm(Graph *graph,QWidget *parent,const string &name,DataSet *dataSet) {
    Observable::holdObservers();
    QtProgress myProgress(parent,name);
    myProgress.hide();
    graph->push();
    
    bool ok=true;
    string erreurMsg;
    if (!tlp::applyAlgorithm(graph, erreurMsg, dataSet, name, &myProgress  )) {
      QMessageBox::critical( 0, "Tulip Algorithm Check Failed",QString((name + ":\n" + erreurMsg).c_str()));
      graph->pop();
      ok=false;
    }
    Observable::unholdObservers();
    return ok;
  }
  //**********************************************************************
  bool ControllerAlgorithmTools::applyAlgorithm(Graph *graph,QWidget *parent,const string &name) {
    DataSet dataSet;
    StructDef *params = getPluginParameters(AlgorithmFactory::factory, name);
    StructDef sysDef = AlgorithmFactory::factory->getPluginParameters(name);
    params->buildDefaultDataSet(dataSet, graph );
    string title = string("Tulip Parameter Editor: ") + name;
    bool ok = tlp::openDataSetDialog(dataSet, &sysDef, params, &dataSet,
				     title.c_str(), graph, parent);
    if (ok) {
      return applyAlgorithm(graph,parent,name,&dataSet);
    }
    return ok;
  }
  //**********************************************************************
  template<typename PROPERTY>
  bool ControllerAlgorithmTools::changeProperty(Graph *graph,QWidget *parent,string name, string destination,View *view, bool query, bool redraw, bool push) {
    return changeProperty<PROPERTY>(graph,parent,name,destination,DataSet(),view,query,redraw,push);
  }
  //**********************************************************************
  template<typename PROPERTY>
  bool ControllerAlgorithmTools::changeProperty(Graph *graph,QWidget *parent,string name, string destination,DataSet dataSet,View *view, bool query, bool redraw, bool push) {
    NodeLinkDiagramComponent *nldc=NULL;
    if(view)
      nldc=dynamic_cast<NodeLinkDiagramComponent*>(view);
    
    unsigned int holdCount=Observable::observersHoldCounter();
    Observable::holdObservers();
    
    QtProgress *myProgress=new QtProgress(parent, name,redraw ? view : 0);

    string erreurMsg;
    bool   resultBool=true;
    if (query) {
      StructDef *params = ControllerAlgorithmTools::getPluginParameters(PROPERTY::factory, name);
      StructDef sysDef = PROPERTY::factory->getPluginParameters(name);
      params->buildDefaultDataSet(dataSet, graph );
      string title = string("Tulip Parameter Editor: ") + name;
      resultBool = tlp::openDataSetDialog(dataSet, &sysDef, params, &dataSet,
					  title.c_str(), graph, parent);
    }

    if (resultBool) {
      PROPERTY* tmp = new PROPERTY(graph);
      if (push)
	graph->push();
      // must be done after push because destination property
      // may not exist and thus the getLocalProperty call may create it
      // and so it must be deleted at pop time
      PROPERTY* dest = graph->template getLocalProperty<PROPERTY>(destination);
      tmp->setAllNodeValue(dest->getNodeDefaultValue());
      tmp->setAllEdgeValue(dest->getEdgeDefaultValue());
      graph->push(false);
      bool updateLayout = (typeid(PROPERTY) == typeid(LayoutProperty) && nldc);
      GlLODCalculator *oldLODCalculator;
      if (updateLayout) {
        graph->setAttribute("viewLayout", tmp);

        if(nldc){
          nldc->getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->reloadLayoutProperty();
          oldLODCalculator=nldc->getGlMainWidget()->getScene()->getCalculator();
          nldc->getGlMainWidget()->getScene()->setCalculator(new GlCPULODCalculator);
          nldc->getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getGlVertexArrayManager()->activate(false);
        }
      }
      resultBool = graph->computeProperty(name, tmp, erreurMsg, myProgress, &dataSet);
      
      /*bool hasNdldc = false;
      DataSet nodeLinkDiagramComponentDataSet;
      if ((typeid(PROPERTY) == typeid(ColorProperty) &&
	   viewNames[currentView]=="Node Link Diagram view"))
	hasNdldc = graph->getAttribute("NodeLinkDiagramComponent", nodeLinkDiagramComponentDataSet);*/
      graph->pop();
      if (updateLayout) {
	graph->removeAttribute("viewLayout");
	
  if(nldc){
	  nldc->getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->reloadLayoutProperty();
    delete nldc->getGlMainWidget()->getScene()->getCalculator();
    nldc->getGlMainWidget()->getScene()->setCalculator(oldLODCalculator);
    nldc->getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getGlVertexArrayManager()->activate(true);
  }
      }
      if (!resultBool) {
        QMessageBox::critical(parent, "Tulip Algorithm Check Failed", QString((name + ":\n" + erreurMsg).c_str()) );
	graph->pop();
      }else{
        switch(myProgress->state()){
        case TLP_CONTINUE:
        case TLP_STOP:
	  /*if (hasNdldc)
	    graph->setAttribute("NodeLinkDiagramComponent", nodeLinkDiagramComponentDataSet);*/
	  *dest = *tmp;

          break;
        case TLP_CANCEL:
          resultBool=false;
        };
      }
      delete tmp;
    }
    Observable::unholdObservers();
    assert(Observable::observersHoldCounter()==holdCount);
    if(Observable::observersHoldCounter()!=holdCount){
      cerr << "Algorithm hold/unhold observers error for " << name << " plugin" << endl;
    }
    delete myProgress;
    return resultBool;
  }
  //**********************************************************************
  bool ControllerAlgorithmTools::changeString(Graph *graph,QWidget *parent,const string &name,const string &propertyName,View *view) {
    return changeProperty<StringProperty>(graph,parent,name,propertyName, view);
  }
  //**********************************************************************
  bool ControllerAlgorithmTools::changeBoolean(Graph *graph,QWidget *parent,const string &name,const string &propertyName,View *view) {
    return changeProperty<BooleanProperty>(graph,parent,name,propertyName, view);
  }
  //**********************************************************************
  bool ControllerAlgorithmTools::changeMetric(Graph *graph,QWidget *parent,const string &name,const string &propertyName,View *view,bool mapMetric, const string &colorAlgorithmName, const string &colorPropertyName) {
    
    bool result = changeProperty<DoubleProperty>(graph,parent,name,propertyName, view, true);
    if (result && mapMetric) {
      return changeProperty<ColorProperty>(graph,parent,colorAlgorithmName,colorPropertyName,view, false,true, false);
    }
    return result;
  }
  //**********************************************************************
  bool ControllerAlgorithmTools::changeLayout(Graph *graph,QWidget *parent,const string &name,const string &propertyName,View *view) {
    return changeProperty<LayoutProperty>(graph,parent,name, propertyName, view, true, true);
  }
  //**********************************************************************
  bool ControllerAlgorithmTools::changeInt(Graph *graph,QWidget *parent,const string &name,const string &propertyName,View *view) {
    return changeProperty<IntegerProperty>(graph,parent,name, propertyName,view);
  }
  //**********************************************************************
  bool ControllerAlgorithmTools::changeColors(Graph *graph,QWidget *parent,const string &name,const string &propertyName,View *view) {
    return changeProperty<ColorProperty>(graph,parent,name,propertyName,view);
  }
  //**********************************************************************
  bool ControllerAlgorithmTools::changeSizes(Graph *graph,QWidget *parent,const string &name,const string &propertyName,View *view) {
    return changeProperty<SizeProperty>(graph,parent,name,propertyName,view);
  }
  //**********************************************************************
  void ControllerAlgorithmTools::isAcyclic(Graph *graph,QWidget *parent) {
    if(AcyclicTest::isAcyclic(graph))
      QMessageBox::information( parent, "Tulip test",
				"The graph is acyclic"
				);
    else
      QMessageBox::information( parent, "Tulip test",
				"The graph is not acyclic"
				);
  }
  //**********************************************************************
  void ControllerAlgorithmTools::makeAcyclic(Graph *graph,bool pushGraph) {
    Observable::holdObservers();
    vector<tlp::SelfLoops> tmpSelf;
    vector<edge> tmpReversed;
    
    if(pushGraph)
      graph->push();
    
    AcyclicTest::makeAcyclic(graph, tmpReversed, tmpSelf);
    Observable::unholdObservers();
  }
  //**********************************************************************
  void ControllerAlgorithmTools::isSimple(Graph *graph,QWidget *parent) {
    //if (glWidget == 0) return;
    if (SimpleTest::isSimple(graph))
      QMessageBox::information( parent, "Tulip test",
				"The graph is simple"
				);
    else
      QMessageBox::information( parent, "Tulip test",
				"The graph is not simple"
				);
  }
  //**********************************************************************
  void ControllerAlgorithmTools::makeSimple(Graph *graph,bool pushGraph) {
    Observable::holdObservers();
    vector<edge> removed;
    
    if(pushGraph)
      graph->push();

    SimpleTest::makeSimple(graph, removed);
    Observable::unholdObservers();
  }

  //**********************************************************************
  void ControllerAlgorithmTools::isConnected(Graph *graph,QWidget *parent) {
    if (ConnectedTest::isConnected(graph))
      QMessageBox::information( parent, "Tulip test",
				"The graph is connected"
				);
    else
      QMessageBox::information( parent, "Tulip test",
				"The graph is not connected"
				);
  }
  //**********************************************************************
  void ControllerAlgorithmTools::makeConnected(Graph *graph,bool pushGraph) {
    Observable::holdObservers();
    vector<edge> tmp;

    if(pushGraph)
      graph->push();

    ConnectedTest::makeConnected(graph, tmp);
    Observable::unholdObservers();
  }
  //**********************************************************************
  void ControllerAlgorithmTools::isBiconnected(Graph *graph,QWidget *parent) {
    if (BiconnectedTest::isBiconnected(graph))
      QMessageBox::information( parent, "Tulip test",
				"The graph is biconnected"
				);
    else
      QMessageBox::information( parent, "Tulip test",
				"The graph is not biconnected"
				);
  }
  //**********************************************************************
  void ControllerAlgorithmTools::makeBiconnected(Graph *graph,bool pushGraph) {
    Observable::holdObservers();
    vector<edge> tmp;
    
    if(pushGraph)
      graph->push();

    BiconnectedTest::makeBiconnected(graph, tmp);
    Observable::unholdObservers();
  }
  //**********************************************************************
  void ControllerAlgorithmTools::isTriconnected(Graph *graph,QWidget *parent) {
    if (TriconnectedTest::isTriconnected(graph))
      QMessageBox::information( parent, "Tulip test",
				"The graph is triconnected"
				);
    else
      QMessageBox::information( parent, "Tulip test",
				"The graph is not triconnected"
				);
  }
  //**********************************************************************
  void ControllerAlgorithmTools::isTree(Graph *graph,QWidget *parent) {
    if (TreeTest::isTree(graph))
      QMessageBox::information( parent, "Tulip test",
				"The graph is a directed tree"
				);
    else
      QMessageBox::information( parent, "Tulip test",
				"The graph is not a directed tree"
				);
  }
  //**********************************************************************
  void ControllerAlgorithmTools::isFreeTree(Graph *graph,QWidget *parent) {
    if (TreeTest::isFreeTree(graph))
      QMessageBox::information( parent, "Tulip test",
				"The graph is a free tree"
				);
    else
      QMessageBox::information( parent, "Tulip test",
				"The graph is not a free tree"
				);
  }
  //**********************************************************************
  void ControllerAlgorithmTools::makeDirected(QWidget *parent,Graph *graph,bool pushGraph) {
    if (!TreeTest::isFreeTree(graph))
      QMessageBox::information(parent, "Tulip test","The graph is not a free tree");
    node n, root;
    forEach(n, graph->getProperty<BooleanProperty>("viewSelection")->getNodesEqualTo(true)) {
      if (root.isValid()) {
        QMessageBox::critical(parent, "Make Rooted","Only one root node must be selected.");
        breakForEach;
      }
      root = n;
    }
    if (!root.isValid())
      root = graphCenterHeuristic(graph);

    Observable::holdObservers();

    if(pushGraph)
      graph->push();

    TreeTest::makeRootedTree(graph, root);
    Observable::unholdObservers();
  }
  //**********************************************************************
  void ControllerAlgorithmTools::isPlanar(Graph *graph,QWidget *parent) {
    if (PlanarityTest::isPlanar(graph))
      QMessageBox::information( parent, "Tulip test",
				"The graph is planar"
				);
    else
      QMessageBox::information( parent, "Tulip test",
				"The graph is not planar"
				);
  }
  //**********************************************************************
  void ControllerAlgorithmTools::isOuterPlanar(Graph *graph,QWidget *parent) {
    if (OuterPlanarTest::isOuterPlanar(graph))
      QMessageBox::information( parent, "Tulip test",
				"The graph is outer planar"
				);
    else
      QMessageBox::information( parent, "Tulip test",
				"The graph is not outer planar"
				);
  }

}

