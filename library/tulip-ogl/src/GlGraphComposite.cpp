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
#include <tulip/GlGraphComposite.h>

#include <iostream>

#include <tulip/ForEach.h>
#include <tulip/GraphProperty.h>

#include <tulip/GlTools.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlLODSceneVisitor.h>
#include <tulip/GlScene.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlVertexArrayVisitor.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/OcclusionTest.h>

using namespace std;

namespace tlp {

/** \brief Storage class for Z ordering
 *
 */
struct EntityWithDistance {

  EntityWithDistance(const double &dist,EntityLODUnit *entity)
    :distance(dist),entity(entity),isComplexEntity(false) {
  }
  EntityWithDistance(const double &dist,ComplexEntityLODUnit *entity,bool isNode)
    :distance(dist),entity(entity),isComplexEntity(true),isNode(isNode) {
  }

  double distance;
  EntityLODUnit *entity;
  bool isComplexEntity;
  bool isNode;
};


struct entityWithDistanceCompare {
  static GlGraphInputData *inputData;
  bool operator()(const EntityWithDistance &e1, const EntityWithDistance &e2 ) const {
    if(e1.distance>e2.distance)
      return true;

    if(e1.distance<e2.distance)
      return false;

    BoundingBox &bb1 = e1.entity->boundingBox;
    BoundingBox &bb2 = e2.entity->boundingBox;

    if(bb1[1][0] - bb1[0][0] > bb2[1][0] - bb2[0][0])
      return false;
    else
      return true;

  }
};

GlGraphInputData *entityWithDistanceCompare::inputData=NULL;

//====================================================
class GreatThanNode {
public:
  DoubleProperty *metric;
  bool operator() (pair<node,float> n1,pair<node,float> n2)  {
    return (metric->getNodeValue(n1.first) > metric->getNodeValue(n2.first));
  }
};
//====================================================
class GreatThanEdge {
public:
  DoubleProperty *metric;
  bool operator() (pair<edge,float> e1,pair<edge,float> e2) {
    return (metric->getEdgeValue(e1.first) > metric->getEdgeValue(e2.first));
  }
};

void drawLabelsForComplexEntities(bool drawSelected,GlGraphComposite *glGraphComposite,
                                  OcclusionTest *occlusionTest,LayerLODUnit &layerLODUnit) {
  Graph *graph=glGraphComposite->getInputData()->getGraph();
  BooleanProperty *selectionProperty=glGraphComposite->getInputData()->getElementSelected();
  bool viewOutScreenLabel=glGraphComposite->getRenderingParameters().isViewOutScreenLabel();
  DoubleProperty *metric=NULL;

  if(graph->existProperty("viewMetric"))
    metric=graph->getProperty<DoubleProperty>("viewMetric");

  vector<pair<node,float> > nodesMetricOrdered;
  vector<pair<edge,float> > edgesMetricOrdered;
  GlNode glNode(0);
  GlEdge glEdge(0);

  Iterator<node> *nonDefaultLabelNodes = glGraphComposite->getInputData()->getElementLabel()->getNonDefaultValuatedNodes();
  Iterator<edge> *nonDefaultLabelEdges = glGraphComposite->getInputData()->getElementLabel()->getNonDefaultValuatedEdges();

  bool nodeLabelEmpty=(!nonDefaultLabelNodes->hasNext())
                      && glGraphComposite->getInputData()->getElementLabel()->getNodeDefaultStringValue()=="";
  bool edgeLabelEmpty=(!nonDefaultLabelEdges->hasNext())
                      && glGraphComposite->getInputData()->getElementLabel()->getEdgeDefaultStringValue()=="";

  delete nonDefaultLabelNodes;
  delete nonDefaultLabelEdges;

  bool viewNodeLabel=glGraphComposite->getInputData()->parameters->isViewNodeLabel();
  bool viewMetaLabel=glGraphComposite->getInputData()->parameters->isViewMetaLabel();

  // Draw Labels for Nodes
  if((viewNodeLabel || viewMetaLabel) && !nodeLabelEmpty) {
    node n;

    for(vector<ComplexEntityLODUnit>::iterator it=layerLODUnit.nodesLODVector.begin(); it!=layerLODUnit.nodesLODVector.end(); ++it) {
      if((*it).lod<0 && !viewOutScreenLabel)
        continue;

      float lod=(*it).lod;

      if(viewOutScreenLabel && lod<0)
        lod=-lod;

      n.id=(*it).id;

      if(selectionProperty->getNodeValue(n)==drawSelected) {
        if(!glGraphComposite->getInputData()->parameters->isElementOrdered() || !metric) {
          // Not metric ordered
          if((!graph->isMetaNode(n) && viewNodeLabel) || graph->isMetaNode(n)) {
            glNode.id=n.id;
            glNode.drawLabel(occlusionTest,glGraphComposite->getInputData(),lod,(Camera *)(layerLODUnit.camera));
          }
        }
        else {
          // Metric ordered
          if((!graph->isMetaNode(n) && viewNodeLabel) || graph->isMetaNode(n)) {
            nodesMetricOrdered.push_back(pair<node,float>(n,lod));
          }
        }
      }
    }

    // If not Metric ordered : a this point selected nodes are draw

    if(glGraphComposite->getInputData()->parameters->isElementOrdered()) {
      // Draw selected nodes label with metric ordering
      if(glGraphComposite->getInputData()->parameters->getElementOrderingProperty()) {
        metric = glGraphComposite->getInputData()->parameters->getElementOrderingProperty();
      }

      GreatThanNode ltn;
      ltn.metric=metric;
      sort(nodesMetricOrdered.begin(),nodesMetricOrdered.end(),ltn);

      for(vector<pair<node,float> >::iterator it=nodesMetricOrdered.begin(); it!=nodesMetricOrdered.end(); ++it) {
        glNode.id=(*it).first.id;
        glNode.drawLabel(occlusionTest,glGraphComposite->getInputData(),(*it).second,(Camera *)(layerLODUnit.camera));
      }
    }
  }

  // Draw Labels for Edges
  if(glGraphComposite->getInputData()->parameters->isViewEdgeLabel() && !edgeLabelEmpty) {
    edge e;

    for(vector<ComplexEntityLODUnit>::iterator it=layerLODUnit.edgesLODVector.begin(); it!=layerLODUnit.edgesLODVector.end(); ++it) {
      if((*it).lod<0 && !viewOutScreenLabel)
        continue;

      e.id=(*it).id;

      if(selectionProperty->getEdgeValue(e) == drawSelected) {
        if(!glGraphComposite->getInputData()->parameters->isElementOrdered() || !metric) {
          // Not metric ordered
          glEdge.id=e.id;
          glEdge.drawLabel(occlusionTest,glGraphComposite->getInputData(),(*it).lod,(Camera *)(layerLODUnit.camera));
        }
        else {
          // Metric ordered
          edgesMetricOrdered.push_back(pair<edge,float>(e,(*it).lod));
        }
      }
    }

    // If not Metric ordered : a this point selected edges are draw
    if(glGraphComposite->getInputData()->parameters->isElementOrdered()) {
      // Draw selected edges label with metric ordering
      if(glGraphComposite->getInputData()->parameters->getElementOrderingProperty()) {
        metric = glGraphComposite->getInputData()->parameters->getElementOrderingProperty();
      }

      GreatThanEdge lte;
      lte.metric=metric;
      sort(edgesMetricOrdered.begin(),edgesMetricOrdered.end(),lte);

      for(vector<pair<edge,float> >::iterator it=edgesMetricOrdered.begin(); it!=edgesMetricOrdered.end(); ++it) {
        glEdge.id=(*it).first.id;
        glEdge.drawLabel(occlusionTest,glGraphComposite->getInputData(),(*it).second,(Camera *)(layerLODUnit.camera));
      }
    }
  }
}

GlGraphComposite::GlGraphComposite(Graph* graph):inputData(graph,&parameters),rootGraph(graph->getRoot()),haveToSort(true),nodesModified(true),selectionDrawActivate(false),selectionIdMap(NULL),selectionCurrentId(NULL) {
  graph->addListener(this);
  graph->getRoot()->getProperty<GraphProperty>("viewMetaGraph")->addPropertyObserver(this);

  Iterator<node>* nodesIterator = graph->getNodes();

  while (nodesIterator->hasNext()) {
    node n=nodesIterator->next();

    if(graph->getNodeMetaInfo(n))
      metaNodes.insert(n);
  }

  delete nodesIterator;

  lodCalculator=NULL;
  fakeScene = new GlScene;
  fakeScene->addLayer(new GlLayer("fakeLayer"));
}

GlGraphComposite::~GlGraphComposite() {
  delete fakeScene;
}

void GlGraphComposite::visitGraph(GlSceneVisitor *visitor, bool visitHiddenEntities) {
  Graph *graph=inputData.getGraph();

  // Check if the current graph are in the hierarchy
  assert((rootGraph==graph) || (rootGraph->isDescendantGraph(graph)));

  if(visitor->isThreadSafe()) {
#ifdef HAVE_OMP
    #pragma omp parallel
#endif
    {
#ifdef HAVE_OMP
      #pragma omp sections nowait
#endif
      {
        visitNodes(graph,visitor,visitHiddenEntities);
      }
#ifdef HAVE_OMP
      #pragma omp sections nowait
#endif
      {
        visitEdges(graph,visitor,visitHiddenEntities);
      }
    }
  }
  else {
    visitNodes(graph,visitor,visitHiddenEntities);
    visitEdges(graph,visitor,visitHiddenEntities);
  }
}

void GlGraphComposite::visitNodes(Graph *graph,GlSceneVisitor *visitor,bool visitHiddenEntities) {
  if(parameters.isDisplayNodes() || parameters.isDisplayMetaNodes() || visitHiddenEntities) {
    visitor->reserveMemoryForNodes(graph->numberOfNodes());
    GlNode glNode(0);
    bool isMetaNode;

    Iterator<node>* nodesIterator = graph->getNodes();

    while (nodesIterator->hasNext()) {
      node n=nodesIterator->next();
      isMetaNode = inputData.getGraph()->isMetaNode(n);

      if((parameters.isDisplayNodes() && !isMetaNode) || (parameters.isDisplayMetaNodes() && isMetaNode) || visitHiddenEntities) {
        glNode.id=n.id;
        glNode.acceptVisitor(visitor);
      }
    }

    delete nodesIterator;
  }
}

void GlGraphComposite::visitEdges(Graph *graph,GlSceneVisitor *visitor,bool visitHiddenEntities) {
  if(parameters.isDisplayEdges() || visitHiddenEntities) {
    visitor->reserveMemoryForEdges(graph->numberOfEdges());

    GlEdge glEdge(0);
    Iterator<edge>* edgesIterator = graph->getEdges();

    while (edgesIterator->hasNext()) {
      glEdge.id=edgesIterator->next().id;
      glEdge.acceptVisitor(visitor);
    }

    delete edgesIterator;
  }
}

void GlGraphComposite::acceptVisitor(GlSceneVisitor *visitor) {
  GlBoundingBoxSceneVisitor bbVisitor(&inputData);
  visitGraph(&bbVisitor);
  boundingBox=bbVisitor.getBoundingBox();

  if(boundingBox.isValid())
    visitor->visit(this);
}
//===================================================================
void GlGraphComposite::draw(float,Camera* camera) {

  Graph *graph=inputData.getGraph();


  if(!lodCalculator){
    lodCalculator=camera->getScene()->getCalculator()->clone();
    lodCalculator->setAttachedLODCalculator(camera->getScene()->getCalculator());
    lodCalculator->setInputData(getInputData());
    lodCalculator->setScene(*fakeScene);
  }

  lodCalculator->clear();
  lodCalculator->setRenderingEntitiesFlag(RenderingAll);

  // Fake scene creation
  //  This scene is needed by lod calculator to compute lod
  Camera newCamera=*camera;
  fakeScene->setViewport(camera->getViewport()[0],camera->getViewport()[1],camera->getViewport()[2],camera->getViewport()[3]);
  fakeScene->getLayer("fakeLayer")->setCamera(newCamera);

  if(lodCalculator->needEntities()) {
    GlLODSceneVisitor visitor(lodCalculator, getInputData());
    visitor.visit(fakeScene->getLayer("fakeLayer"));
    visitGraph(&visitor);
  }

  lodCalculator->compute(fakeScene->getViewport(), fakeScene->getViewport());
  LayersLODVector &layersLODVector = lodCalculator->getResult();

  GlEdge::clearEdgeWidthLodSystem();

  if(!selectionDrawActivate) {
    inputData.getGlVertexArrayManager()->activate(true);
    inputData.getGlVertexArrayManager()->beginRendering();
  }
  else {
    inputData.getGlVertexArrayManager()->activate(false);
  }

  // VertexArrayManager update
  if(inputData.getGlVertexArrayManager()->haveToCompute()) {
    GlVertexArrayVisitor vertexArrayVisitor(&inputData);
    visitGraph(&vertexArrayVisitor,true);
    inputData.getGlVertexArrayManager()->setHaveToComputeAll(false);
  }

  BooleanProperty *filteringProperty = parameters.getDisplayFilteringProperty();
  bool displayNodes = parameters.isDisplayNodes();
  bool displayMetaNodes = parameters.isDisplayMetaNodes();
  bool displayEdges = parameters.isDisplayEdges();

  GlNode glNode(0);
  GlEdge glEdge(0);

  if(!parameters.isElementZOrdered()) {

    //draw nodes and metanodes
    for(vector<ComplexEntityLODUnit>::iterator it =layersLODVector[0].nodesLODVector.begin(); it!=layersLODVector[0].nodesLODVector.end(); ++it) {
      if((*it).lod<=0)
        continue;

      if(filteringProperty) {
        if(filteringProperty->getNodeValue(node((*it).id)))
          continue;
      }

      if((!graph->isMetaNode(node((*it).id)) && displayNodes) || (graph->isMetaNode(node((*it).id)) && displayMetaNodes)) {
        if(selectionDrawActivate) {
          if((selectionType & RenderingNodes)==0)
            continue;

          (*selectionIdMap)[*selectionCurrentId]=SelectedEntity(graph,(*it).id);
          glLoadName(*selectionCurrentId);
          (*selectionCurrentId)++;
        }

        glNode.id=(*it).id;
        glNode.draw((*it).lod,&inputData,camera);
      }
      else {
        continue;
      }

    }

    //draw edges
    for(vector<ComplexEntityLODUnit>::iterator it = layersLODVector[0].edgesLODVector.begin(); it!=layersLODVector[0].edgesLODVector.end(); ++it) {
      if((*it).lod<=0)
        continue;

      if(filteringProperty) {
        if(filteringProperty->getEdgeValue(edge((*it).id)))
          continue;
      }

      if(!displayEdges)
        continue;

      if(selectionDrawActivate) {
        if((selectionType & RenderingEdges)==0)
          continue;

        (*selectionIdMap)[*selectionCurrentId]=SelectedEntity(graph,(*it).id);
        glLoadName(*selectionCurrentId);
        (*selectionCurrentId)++;
      }

      glEdge.id=(*it).id;
      glEdge.draw((*it).lod,&inputData,camera);
    }

  }
  else {

    entityWithDistanceCompare::inputData=&inputData;
    multiset<EntityWithDistance,entityWithDistanceCompare> entitiesSet;
    Coord camPos=camera->getEyes();
    Coord camCenter=camera->getCenter();
    BoundingBox bb;
    double dist;

    if(!selectionDrawActivate || (selectionDrawActivate && (selectionType & RenderingNodes)!=0)) {
      // Colect complex entities
      for(vector<ComplexEntityLODUnit>::iterator it=layersLODVector[0].nodesLODVector.begin(); it!=layersLODVector[0].nodesLODVector.end(); ++it) {
        if((*it).lod<0)
          continue;

        if(filteringProperty) {
          if(filteringProperty->getNodeValue(node((*it).id)))
            continue;
        }

        bb=(*it).boundingBox;
        Coord middle((bb[1]+bb[0])/2.f);
        dist=(((double)middle[0])-((double)camPos[0]))*(((double)middle[0])-((double)camPos[0]));
        dist+=(((double)middle[1])-((double)camPos[1]))*(((double)middle[1])-((double)camPos[1]));
        dist+=(((double)middle[2])-((double)camPos[2]))*(((double)middle[2])-((double)camPos[2]));
        entitiesSet.insert(EntityWithDistance(dist,&(*it),true));
      }
    }

    if(!selectionDrawActivate || (selectionDrawActivate && (selectionType & RenderingEdges)!=0)) {
      for(vector<ComplexEntityLODUnit>::iterator it=layersLODVector[0].edgesLODVector.begin(); it!=layersLODVector[0].edgesLODVector.end(); ++it) {
        if((*it).lod<0)
          continue;

        if(filteringProperty) {
          if(filteringProperty->getEdgeValue(edge((*it).id)))
            continue;
        }

        bb = (*it).boundingBox;
        Coord middle((bb[0] + bb[1])/2.f);
        dist=(((double)middle[0])-((double)camPos[0]))*(((double)middle[0])-((double)camPos[0]));
        dist+=(((double)middle[1])-((double)camPos[1]))*(((double)middle[1])-((double)camPos[1]));
        dist+=(((double)middle[2])-((double)camPos[2]))*(((double)middle[2])-((double)camPos[2]));
        entitiesSet.insert(EntityWithDistance(dist,&(*it),false));
      }
    }

    // Draw
    for(multiset<EntityWithDistance,entityWithDistanceCompare>::iterator it=entitiesSet.begin(); it!=entitiesSet.end(); ++it) {
      // Complex entities
      ComplexEntityLODUnit *entity=(ComplexEntityLODUnit*)((*it).entity);

      if((*it).isNode && (selectionType & RenderingNodes)!=0) {
        if((!graph->isMetaNode(node(entity->id)) && displayNodes) || (graph->isMetaNode(node(entity->id)) && displayMetaNodes)) {
          if(selectionDrawActivate) {
            (*selectionIdMap)[*selectionCurrentId]=SelectedEntity(graph,entity->id);
            glLoadName(*selectionCurrentId);
            (*selectionCurrentId)++;
          }

          glNode.id=entity->id;
          glNode.draw(entity->lod,&inputData,camera);
        }
        else {
          continue;
        }
      }
      else if((selectionType & RenderingEdges)!=0) {
        if(!displayEdges)
          continue;

        if(selectionDrawActivate) {
          (*selectionIdMap)[*selectionCurrentId]=SelectedEntity(graph,entity->id);
          glLoadName(*selectionCurrentId);
          (*selectionCurrentId)++;
        }

        glEdge.id=entity->id;
        glEdge.draw(entity->lod,&inputData,camera);
      }
    }
  }

  if(!selectionDrawActivate) {
    inputData.getGlVertexArrayManager()->endRendering();
  }

  /*
    Label draw
  */
  OcclusionTest occlusionTest;
  bool labelDensityAtZero=true;

  if(parameters.getLabelsDensity()!=-100)
    labelDensityAtZero=false;

  if(!labelDensityAtZero) {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_LIGHTING);
    glDepthFunc(GL_ALWAYS );
    glDisable(GL_CULL_FACE);
    glDisable(GL_COLOR_MATERIAL);

    // Draw Labels for selected entities
    drawLabelsForComplexEntities(true,this,&occlusionTest,layersLODVector[0]);

    // Draw Labels for unselected entities
    drawLabelsForComplexEntities(false,this,&occlusionTest,layersLODVector[0]);

    glPopAttrib();
  }

  selectionDrawActivate=false;
}
//===================================================================
void GlGraphComposite::initSelectionRendering(RenderingEntitiesFlag type,map<unsigned int, SelectedEntity> &idMap,unsigned int &currentId) {
  selectionType=type;
  selectionIdMap=&idMap;
  selectionCurrentId=&currentId;
  selectionDrawActivate=true;
}
//===================================================================
const GlGraphRenderingParameters& GlGraphComposite::getRenderingParameters() {
  return parameters;
}
//===================================================================
void GlGraphComposite::setRenderingParameters(const GlGraphRenderingParameters &parameter) {
  if(parameters.isElementOrdered() != parameter.isElementOrdered()) {
    parameters = parameter;
    haveToSort=true;
  }
  else {
    parameters = parameter;
  }
}
//===================================================================
GlGraphRenderingParameters* GlGraphComposite::getRenderingParametersPointer() {
  return &parameters;
}
//===================================================================
GlGraphInputData* GlGraphComposite::getInputData() {
  return &inputData;
}
//====================================================
void GlGraphComposite::getXML(string &outString) {
  GlXMLTools::createProperty(outString, "type", "GlGraphComposite","GlEntity");
}
//====================================================
void GlGraphComposite::setWithXML(const string &, unsigned int&) {
}

void GlGraphComposite::treatEvent(const Event& evt) {
  const GraphEvent* graphEvent = dynamic_cast<const GraphEvent*>(&evt);

  if (graphEvent) {
    switch(graphEvent->getType()) {
    case GraphEvent::TLP_ADD_NODE:
      nodesModified=true;
      haveToSort=true;
      break;

    case GraphEvent::TLP_DEL_NODE:
      nodesModified=true;
      haveToSort=true;
      break;

    case GraphEvent::TLP_ADD_EDGE:
      haveToSort=true;
      break;

    case GraphEvent::TLP_DEL_EDGE:
      haveToSort=true;
      break;

    default:
      break;
    }
  }
  else if(evt.type() == Event::TLP_DELETE) {
    Graph* g = dynamic_cast<Graph*>(evt.sender());

    if(g && inputData.getGraph() == g) {
      inputData.graph = NULL;
    }
  }
  else {
    const PropertyEvent* propertyEvent = dynamic_cast<const PropertyEvent*>(&evt);

    if(propertyEvent && propertyEvent->getType() == PropertyEvent::TLP_AFTER_SET_NODE_VALUE) {
      nodesModified=true;
    }
  }
}

}
