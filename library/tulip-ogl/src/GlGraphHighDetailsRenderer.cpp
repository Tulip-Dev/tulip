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
#include <tulip/GlGraphHighDetailsRenderer.h>

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
 * Storage class for Z ordering
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

/** \brief Comparator to order entities with Z
 * Comparator to order entities with Z
 */
struct entityWithDistanceCompare {
  static const GlGraphInputData *inputData;
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

const GlGraphInputData *entityWithDistanceCompare::inputData=NULL;

/** \brief Comparator to order entities with metric to display labels with metric ordering
 * Comparator to order entities with metric to display labels with metric ordering
 */
class GreatThanNode {
public:
  DoubleProperty *metric;
  bool operator() (pair<node,float> n1,pair<node,float> n2)  {
    return (metric->getNodeValue(n1.first) > metric->getNodeValue(n2.first));
  }
};
/** \brief Comparator to order entities with metric to display labels with metric ordering
 * Comparator to order entities with metric to display labels with metric ordering
 */
class GreatThanEdge {
public:
  DoubleProperty *metric;
  bool operator() (pair<edge,float> e1,pair<edge,float> e2) {
    return (metric->getEdgeValue(e1.first) > metric->getEdgeValue(e2.first));
  }
};

//===================================================================
//
// GlGraphHighDetailsRenderer functions implementation
//
//===================================================================

GlGraphHighDetailsRenderer::GlGraphHighDetailsRenderer(const GlGraphInputData *inputData):GlGraphRenderer(inputData) {
  lodCalculator=NULL;
  fakeScene = new GlScene;
  fakeScene->createLayer("fakeLayer");
}
//===================================================================
GlGraphHighDetailsRenderer::~GlGraphHighDetailsRenderer() {
  delete fakeScene;
  delete lodCalculator;
}
//===================================================================
void GlGraphHighDetailsRenderer::draw(float,Camera* camera) {

  Graph *graph=inputData->getGraph();

  // If we don't init lod calculator : clone the scene one
  if(!lodCalculator) {
    lodCalculator=camera->getScene()->getCalculator()->clone();
    lodCalculator->setAttachedLODCalculator(camera->getScene()->getCalculator());
    lodCalculator->setInputData(inputData);
    lodCalculator->setScene(*fakeScene);
  }

  lodCalculator->clear();
  lodCalculator->setRenderingEntitiesFlag(RenderingAll);

  // Fake scene creation
  //  This scene is needed by lod calculator to compute lod
  fakeScene->setViewport(camera->getViewport()[0],camera->getViewport()[1],camera->getViewport()[2],camera->getViewport()[3]);
  fakeScene->getLayer("fakeLayer")->setSharedCamera(camera);

  // LOD computation
  if(lodCalculator->needEntities()) {
    GlLODSceneVisitor visitor(lodCalculator, inputData);
    visitor.visit(fakeScene->getLayer("fakeLayer"));
    visitGraph(&visitor);
  }

  lodCalculator->compute(fakeScene->getViewport(), fakeScene->getViewport());
  LayersLODVector &layersLODVector = lodCalculator->getResult();

  GlEdge::clearEdgeWidthLodSystem();

  // VertexArrayManager begin
  if(!selectionDrawActivate) {
    inputData->getGlVertexArrayManager()->activate(true);
    inputData->getGlVertexArrayManager()->beginRendering();
  }
  else {
    inputData->getGlVertexArrayManager()->activate(false);
  }

  // VertexArrayManager update
  if(inputData->getGlVertexArrayManager()->haveToCompute()) {
    GlVertexArrayVisitor vertexArrayVisitor(inputData);
    visitGraph(&vertexArrayVisitor,true);
    inputData->getGlVertexArrayManager()->setHaveToComputeAll(false);
  }

  BooleanProperty *filteringProperty = inputData->parameters->getDisplayFilteringProperty();
  bool displayNodes = inputData->parameters->isDisplayNodes();
  bool displayMetaNodes = inputData->parameters->isDisplayMetaNodes();
  bool displayEdges = inputData->parameters->isDisplayEdges();

  GlNode glNode(0);
  GlEdge glEdge(0);

  bool renderOnlyOneNode=false;
  if(!selectionDrawActivate){
    if(!inputData->getElementLayout()->getNonDefaultValuatedNodes()->hasNext() &&
       !inputData->getElementSize()->getNonDefaultValuatedNodes()->hasNext())
      renderOnlyOneNode=true;
  }

  if(!inputData->parameters->isElementZOrdered()) {

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

          (*selectionIdMap)[*selectionCurrentId]=SelectedEntity(graph,(*it).id,SelectedEntity::NODE_SELECTED);
          glLoadName(*selectionCurrentId);
          (*selectionCurrentId)++;
        }

        glNode.id=(*it).id;
        glNode.draw((*it).lod,inputData,camera);

        if(renderOnlyOneNode)
          break;
      }
      else {
        continue;
      }

    }

    if(!renderOnlyOneNode){
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

          (*selectionIdMap)[*selectionCurrentId]=SelectedEntity(graph,(*it).id,SelectedEntity::EDGE_SELECTED);
          glLoadName(*selectionCurrentId);
          (*selectionCurrentId)++;
        }

        glEdge.id=(*it).id;
        glEdge.draw((*it).lod,inputData,camera);
      }
    }

  }
  else {

    entityWithDistanceCompare::inputData=inputData;
    multiset<EntityWithDistance,entityWithDistanceCompare> entitiesSet;
    Coord camPos=camera->getEyes();
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
            (*selectionIdMap)[*selectionCurrentId]=SelectedEntity(graph,entity->id,SelectedEntity::NODE_SELECTED);
            glLoadName(*selectionCurrentId);
            (*selectionCurrentId)++;
          }

          glNode.id=entity->id;
          glNode.draw(entity->lod,inputData,camera);

          if(renderOnlyOneNode)
            break;
        }
        else {
          continue;
        }
      }
      else if((selectionType & RenderingEdges)!=0 && !renderOnlyOneNode) {
        if(!displayEdges)
          continue;

        if(selectionDrawActivate) {
          (*selectionIdMap)[*selectionCurrentId]=SelectedEntity(graph,entity->id,SelectedEntity::EDGE_SELECTED);
          glLoadName(*selectionCurrentId);
          (*selectionCurrentId)++;
        }

        glEdge.id=entity->id;
        glEdge.draw(entity->lod,inputData,camera);
      }
    }
  }

  if(!selectionDrawActivate) {
    inputData->getGlVertexArrayManager()->endRendering();
  }
  else {
    selectionDrawActivate=false;
    return;
  }

  /*
    Label draw
  */
  OcclusionTest occlusionTest;
  bool labelDensityAtZero=true;

  if(inputData->parameters->getLabelsDensity()!=-100)
    labelDensityAtZero=false;

  if(!labelDensityAtZero) {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_LIGHTING);
    glDepthFunc(GL_ALWAYS );
    glDisable(GL_CULL_FACE);
    glDisable(GL_COLOR_MATERIAL);

    // Draw Labels for selected entities
    drawLabelsForComplexEntities(true,&occlusionTest,layersLODVector[0]);

    // Draw Labels for unselected entities
    drawLabelsForComplexEntities(false,&occlusionTest,layersLODVector[0]);

    glPopAttrib();
  }

  selectionDrawActivate=false;
}
//===================================================================
void GlGraphHighDetailsRenderer::selectEntities(Camera *camera,RenderingEntitiesFlag type, int, int, int, int, vector<SelectedEntity> &selectedEntities) {
  map<unsigned int, SelectedEntity> idToEntity;
  unsigned int id=1;

  unsigned int size=inputData->getGraph()->numberOfNodes() + inputData->getGraph()->numberOfEdges();

  //Allocate memory to store the result oh the selection
  GLuint (*selectBuf)[4] = new GLuint[size][4];
  glSelectBuffer(size*4 , (GLuint *)selectBuf);
  //Activate Open Gl Selection mode
  glRenderMode(GL_SELECT);
  glInitNames();
  glPushName(0);

  initSelectionRendering(type,idToEntity,id);

  draw(20,camera);

  glFlush();
  GLint hits = glRenderMode(GL_RENDER);

  while(hits>0) {
    selectedEntities.push_back(idToEntity[selectBuf[hits-1][3]]);
    hits--;
  }

  delete[] selectBuf;
}
//===================================================================
void GlGraphHighDetailsRenderer::initSelectionRendering(RenderingEntitiesFlag type,map<unsigned int, SelectedEntity> &idMap,unsigned int &currentId) {
  selectionType=type;
  selectionIdMap=&idMap;
  selectionCurrentId=&currentId;
  selectionDrawActivate=true;
}
//===================================================================
void GlGraphHighDetailsRenderer::drawLabelsForComplexEntities(bool drawSelected,OcclusionTest *occlusionTest,LayerLODUnit &layerLODUnit) {
  Graph *graph=inputData->getGraph();
  BooleanProperty *selectionProperty=inputData->getElementSelected();
  bool viewOutScreenLabel=inputData->parameters->isViewOutScreenLabel();
  DoubleProperty *metric=NULL;

  if(graph->existProperty("viewMetric"))
    metric=graph->getProperty<DoubleProperty>("viewMetric");

  vector<pair<node,float> > nodesMetricOrdered;
  vector<pair<edge,float> > edgesMetricOrdered;
  GlNode glNode(0);
  GlEdge glEdge(0);

  Iterator<node> *nonDefaultLabelNodes = inputData->getElementLabel()->getNonDefaultValuatedNodes();
  Iterator<edge> *nonDefaultLabelEdges = inputData->getElementLabel()->getNonDefaultValuatedEdges();

  bool nodeLabelEmpty=(!nonDefaultLabelNodes->hasNext())
                      && inputData->getElementLabel()->getNodeDefaultStringValue()=="";
  bool edgeLabelEmpty=(!nonDefaultLabelEdges->hasNext())
                      && inputData->getElementLabel()->getEdgeDefaultStringValue()=="";

  delete nonDefaultLabelNodes;
  delete nonDefaultLabelEdges;

  bool viewNodeLabel=inputData->parameters->isViewNodeLabel();
  bool viewMetaLabel=inputData->parameters->isViewMetaLabel();

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
        if(!inputData->parameters->isElementOrdered() || !metric) {
          // Not metric ordered
          if((!graph->isMetaNode(n) && viewNodeLabel) || graph->isMetaNode(n)) {
            glNode.id=n.id;
            glNode.drawLabel(occlusionTest,inputData,lod,(Camera *)(layerLODUnit.camera));
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

    if(inputData->parameters->isElementOrdered()) {
      // Draw selected nodes label with metric ordering
      if(inputData->parameters->getElementOrderingProperty()) {
        metric = inputData->parameters->getElementOrderingProperty();
      }

      GreatThanNode ltn;
      ltn.metric=metric;
      sort(nodesMetricOrdered.begin(),nodesMetricOrdered.end(),ltn);

      for(vector<pair<node,float> >::iterator it=nodesMetricOrdered.begin(); it!=nodesMetricOrdered.end(); ++it) {
        glNode.id=(*it).first.id;
        glNode.drawLabel(occlusionTest,inputData,(*it).second,(Camera *)(layerLODUnit.camera));
      }
    }
  }

  // Draw Labels for Edges
  if(inputData->parameters->isViewEdgeLabel() && !edgeLabelEmpty) {
    edge e;

    for(vector<ComplexEntityLODUnit>::iterator it=layerLODUnit.edgesLODVector.begin(); it!=layerLODUnit.edgesLODVector.end(); ++it) {
      if((*it).lod<0 && !viewOutScreenLabel)
        continue;

      e.id=(*it).id;

      if(selectionProperty->getEdgeValue(e) == drawSelected) {
        if(!inputData->parameters->isElementOrdered() || !metric) {
          // Not metric ordered
          glEdge.id=e.id;
          glEdge.drawLabel(occlusionTest,inputData,(*it).lod,(Camera *)(layerLODUnit.camera));
        }
        else {
          // Metric ordered
          edgesMetricOrdered.push_back(pair<edge,float>(e,(*it).lod));
        }
      }
    }

    // If not Metric ordered : a this point selected edges are draw
    if(inputData->parameters->isElementOrdered()) {
      // Draw selected edges label with metric ordering
      if(inputData->parameters->getElementOrderingProperty()) {
        metric = inputData->parameters->getElementOrderingProperty();
      }

      GreatThanEdge lte;
      lte.metric=metric;
      sort(edgesMetricOrdered.begin(),edgesMetricOrdered.end(),lte);

      for(vector<pair<edge,float> >::iterator it=edgesMetricOrdered.begin(); it!=edgesMetricOrdered.end(); ++it) {
        glEdge.id=(*it).first.id;
        glEdge.drawLabel(occlusionTest,inputData,(*it).second,(Camera *)(layerLODUnit.camera));
      }
    }
  }
}

}
