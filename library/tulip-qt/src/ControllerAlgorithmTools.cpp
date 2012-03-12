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
#include "tulip/PreferenceManager.h"

#include "tulip/ThreadedComputeProperty.h"

using namespace std;

namespace tlp {

static TLP_HASH_MAP<unsigned long, TLP_HASH_MAP<std::string, ParameterDescriptionList> > paramMaps;

ParameterDescriptionList& ControllerAlgorithmTools::getPluginParameters(TemplateFactoryInterface *factory, const std::string &name) {
  TLP_HASH_MAP<std::string, ParameterDescriptionList>::const_iterator it;
  it = paramMaps[(unsigned long) factory].find(name);

  if (it == paramMaps[(unsigned long) factory].end()) {
    paramMaps[(unsigned long) factory][name] = factory->getPluginParameters(name);
  }

  return paramMaps[(unsigned long) factory][name];
}

void ControllerAlgorithmTools::cleanPluginParameters() {
  TLP_HASH_MAP<unsigned long, TLP_HASH_MAP<std::string, ParameterDescriptionList> >::iterator it = paramMaps.begin();

  for (; it != paramMaps.end() ; ++it) {
    vector<string> entriesToErase;
    TLP_HASH_MAP<std::string, ParameterDescriptionList>::const_iterator it2 = it->second.begin();

    for (; it2 != it->second.end() ; ++it2) {
      if (!reinterpret_cast<TemplateFactoryInterface *>(it->first)->pluginExists(it2->first)) {
        entriesToErase.push_back(it2->first);
      }
    }

    for (size_t i = 0 ; i < entriesToErase.size() ; ++i) {
      it->second.erase(entriesToErase[i]);
    }
  }
}

//**********************************************************************
bool ControllerAlgorithmTools::applyAlgorithm(Graph *graph,QWidget *parent,const string &name,DataSet *dataSet) {
  Observable::holdObservers();
  QtProgress myProgress(parent,name);
  graph->push();

  bool ok=true;
  string erreurMsg;

  if (!tlp::applyAlgorithm(graph, erreurMsg, dataSet, name, &myProgress  )) {
    QMessageBox::critical( 0, "Tulip Algorithm Check Failed", QString::fromUtf8((name + ":\n" + erreurMsg).c_str()));
    // no possible redo
    graph->pop(false);
    ok=false;
  }

  Observable::unholdObservers();
  return ok;
}
//**********************************************************************
bool ControllerAlgorithmTools::applyAlgorithm(Graph *graph, QWidget *parent,
    const string &name) {
  DataSet dataSet;
  ParameterDescriptionList& params = getPluginParameters(AlgorithmPlugin::factory, name);
  const ParameterDescriptionList& sysDef =
    AlgorithmPlugin::factory->getPluginParameters(name);
  params.buildDefaultDataSet(dataSet, graph);
  string title = string("Tulip Parameter Editor: ") + name;
  bool ok = tlp::openDataSetDialog(dataSet, sysDef, params, &dataSet,
                                   title.c_str(), graph, parent);

  if (ok) {
    return applyAlgorithm(graph,parent,name,&dataSet);
  }

  return ok;
}
//**********************************************************************
string dataSetToString(DataSet &dataSet) {
  stringstream ss;
  pair<string, DataType*> p;
  forEach(p, dataSet.getValues()) {
    DataTypeSerializer *serializer = DataSet::typenameToSerializer(p.second->getTypeName());

    if (serializer) {
      ss << "\"" << p.first << "\"=";
      serializer->writeData(ss, p.second);
      ss << " ";
    }
  }
  return ss.str();
}
//**********************************************************************
// structure to hold an output property parameter
struct OutPropertyParam {
  string name; // the name of the parameter
  PropertyInterface* dest; // the destination property
  PropertyInterface* tmp;  // the temporary property

  OutPropertyParam(const string& pName): name(pName), dest(NULL), tmp(NULL) {
  }
};

#define TN( T )   typeid(T).name()

bool ControllerAlgorithmTools::changeProperty(Graph *graph, QWidget *parent,
    const string& name,
    const string& destination,
    const string& destType,
    View *view,
    DataSet dataSet,
    bool query,
    bool redraw,
    bool push) {
  NodeLinkDiagramComponent *nldc=NULL;

  if(view)
    nldc=dynamic_cast<NodeLinkDiagramComponent*>(view);

  unsigned int holdCount=Observable::observersHoldCounter();
  Observable::holdObservers();

  string erreurMsg;
  bool   resultBool=true;
  TemplateFactoryInterface* factory = AlgorithmPlugin::factory;

  if (push)
    graph->push();

  // ensure destination exists
  graph->getLocalProperty(destination, destType);

  ParameterDescriptionList& params =
    ControllerAlgorithmTools::getPluginParameters(factory, name);
  params.buildDefaultDataSet(dataSet, graph);

  // plugin parameters dialog
  if (query) {
    const ParameterDescriptionList& sysDef =
      factory->getPluginParameters(name);
    string title = string("Tulip Parameter Editor: ") + name;
    resultBool = tlp::openDataSetDialog(dataSet, sysDef, params, &dataSet,
                                        title.c_str(), graph, parent);
  }

  QtProgress *myProgress=new QtProgress(parent, name, redraw ? view : 0);

  if (resultBool) {
    // check if the destination property is a layout
    bool updateLayout =  (nldc != NULL) &&
                         (destType == LayoutProperty::propertyTypename);
    PropertyInterface* tmpLayout = NULL;
    // keep track of out properties
    // to ensure only that ones are updated
    vector<OutPropertyParam> outPropertyParams;
    Iterator<ParameterDescription> * paramIt = params.getParameters();

    while( paramIt->hasNext() ) {
      ParameterDescription param = paramIt->next();

      // forget in param
      if (param.getDirection() == IN_PARAM)
        continue;

      string typeName(param.getTypeName());

      // forget non property out param
      if (typeName != TN(PropertyInterface*)
          && typeName != TN(BooleanProperty)
          && typeName != TN(DoubleProperty)
          && typeName != TN(LayoutProperty)
          && typeName != TN(StringProperty)
          && typeName != TN(IntegerProperty)
          && typeName != TN(SizeProperty)
          && typeName != TN(ColorProperty))
        continue;

      OutPropertyParam outPropParam(param.getName());
      // get destination property
      dataSet.get(param.getName(), outPropParam.dest);
      // clone it in a temporary property
      outPropParam.tmp = outPropParam.dest->clonePrototype(graph, "");

      if (param.getDirection() == OUT_PARAM) {
        // get tmpLayout if needed
        if (updateLayout &&
            outPropParam.dest->getTypename() == LayoutProperty::propertyTypename)
          tmpLayout = outPropParam.tmp;

        outPropParam.tmp->setAllNodeDataMemValue(outPropParam.dest->getNodeDefaultDataMemValue());
        outPropParam.tmp->setAllEdgeDataMemValue(outPropParam.dest->getEdgeDefaultDataMemValue());
      }
      else
        // inout property
        outPropParam.tmp->copy(outPropParam.dest);

      // set the temporary as the destination property
      dataSet.set(param.getName(), outPropParam.tmp);
      outPropertyParams.push_back(outPropParam);
    }

    // ensure that only out properties are updated
    graph->push(false);

    // If property is a layout property : we change the LOD Calculator to optimise morphing rendering
    GlLODCalculator *oldLODCalculator = NULL;

    if (updateLayout) {
      graph->setAttribute("viewLayout", tmpLayout);

      if(nldc) {
        nldc->getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->reloadLayoutProperty();
        oldLODCalculator=nldc->getGlMainWidget()->getScene()->getCalculator();
        nldc->getGlMainWidget()->getScene()->setCalculator(new GlCPULODCalculator);
        nldc->getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getGlVertexArrayManager()->activate(false);
      }
    }

    resultBool = graph->applyAlgorithm(name, erreurMsg, &dataSet, myProgress);
    // ensure that only out properties are updated
    graph->pop();

    if (updateLayout) {
      graph->removeAttribute("viewLayout");

      if(nldc) {
        nldc->getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->reloadLayoutProperty();
        delete nldc->getGlMainWidget()->getScene()->getCalculator();
        nldc->getGlMainWidget()->getScene()->setCalculator(oldLODCalculator);
        nldc->getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getGlVertexArrayManager()->activate(true);
      }
    }

    if (!resultBool) {
      QMessageBox::critical(parent, "Tulip Algorithm Check Failed", QString::fromUtf8((name + ":\n" + erreurMsg).c_str()) );
      // no possible redo
      graph->pop(false);
    }
    else {
      resultBool = myProgress->state() != TLP_CANCEL;

      // copy or cleanup out properties
      for (unsigned int i = 0; i < outPropertyParams.size(); ++i) {
        OutPropertyParam outPropParam(outPropertyParams[i]);

        if (resultBool)
          outPropParam.dest->copy(outPropParam.tmp);

        delete outPropParam.tmp;
      }

      if (!resultBool)
        // no possible redo
        graph->pop(false);
    }

    if (PreferenceManager::getInst().getUseSpecificMetric() &&
        destType == DoubleProperty::propertyTypename) {
      string copyName = name + " " + dataSetToString(dataSet);

      if (!graph->existProperty(copyName) ||
          graph->getProperty(copyName)->getTypename() == DoubleProperty::propertyTypename) {

        PropertyInterface*copy = graph->getProperty<DoubleProperty>(copyName);
        assert(copy != NULL);
        copy->copy(graph->getLocalProperty(destination, destType));
      }
    }
  }

  Observable::unholdObservers();
  assert(Observable::observersHoldCounter()==holdCount);

  if(Observable::observersHoldCounter()!=holdCount) {
    cerr << "Algorithm hold/unhold observers error for " << name << " plugin" << endl;
  }

  delete myProgress;
  return resultBool;
}
//**********************************************************************
bool ControllerAlgorithmTools::changeString(Graph *graph, QWidget *parent,
    const string &name,
    const string &propertyName,
    View *view) {
  return changeProperty(graph, parent, name, propertyName,
                        StringProperty::propertyTypename, view);
}
//**********************************************************************
bool ControllerAlgorithmTools::changeBoolean(Graph *graph, QWidget *parent,
    const string &name,
    const string &propertyName,
    View *view) {
  return changeProperty(graph, parent, name, propertyName,
                        BooleanProperty::propertyTypename, view);
}
//**********************************************************************
bool ControllerAlgorithmTools::changeMetric(Graph *graph, QWidget *parent,
    const string &name,
    const string &propertyName,
    View *view, bool mapMetric,
    const string &colorAlgorithmName,
    const string &colorPropertyName) {
  bool result = changeProperty(graph, parent, name, propertyName,
                               DoubleProperty::propertyTypename, view,
                               DataSet(), true);

  if (result && mapMetric)
    return changeProperty(graph, parent, colorAlgorithmName, colorPropertyName,
                          ColorProperty::propertyTypename, view,
                          DataSet(), false, true, false);

  return result;
}
//**********************************************************************
bool ControllerAlgorithmTools::changeLayout(Graph *graph, QWidget *parent,
    const string &name,
    const string &propertyName,
    View *view) {
  return changeProperty(graph, parent, name, propertyName,
                        LayoutProperty::propertyTypename, view,
                        DataSet(), true, true);
}
//**********************************************************************
bool ControllerAlgorithmTools::changeInt(Graph *graph, QWidget *parent,
    const string &name,
    const string &propertyName,
    View *view) {
  return changeProperty(graph, parent, name, propertyName,
                        IntegerProperty::propertyTypename, view);
}
//**********************************************************************
bool ControllerAlgorithmTools::changeColors(Graph *graph, QWidget *parent,
    const string &name,
    const string &propertyName,
    View *view) {
  return changeProperty(graph, parent, name, propertyName,
                        ColorProperty::propertyTypename, view);
}
//**********************************************************************
bool ControllerAlgorithmTools::changeSizes(Graph *graph, QWidget *parent,
    const string &name,
    const string &propertyName,
    View *view) {
  return changeProperty(graph, parent, name, propertyName,
                        SizeProperty::propertyTypename, view);
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
      break;
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

