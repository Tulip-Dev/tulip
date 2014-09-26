/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#include <tulip/ForEach.h>
#include <tulip/GraphProperty.h>
#include <tulip/NumericProperty.h>
#include <tulip/GlTools.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlLODSceneVisitor.h>
#include <tulip/GlScene.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlVertexArrayVisitor.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/OcclusionTest.h>
#include <tulip/GlEdge.h>
#include <tulip/GlNode.h>
#include <tulip/Camera.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlGlyphRenderer.h>
#include <tulip/OpenGlConfigManager.h>

using namespace std;

namespace tlp {

/** \brief Storage class for Z ordering
 * Storage class for Z ordering
 */
struct EntityWithDistance {

  EntityWithDistance(const double &dist,EntityLODUnit *entity)
    :distance(dist),entity(entity),isComplexEntity(false),isNode(false) {
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

    // opaque objects have to be drawn first
    if (e1.isComplexEntity && e2.isComplexEntity) {
      Color e1Color, e2Color;

      if (e1.isNode) {
        e1Color = inputData->getElementColor()->getNodeValue(node(static_cast<ComplexEntityLODUnit*>(e1.entity)->id));
      }
      else {
        e1Color = inputData->getElementColor()->getEdgeValue(edge(static_cast<ComplexEntityLODUnit*>(e1.entity)->id));
      }

      if (e2.isNode) {
        e2Color = inputData->getElementColor()->getNodeValue(node(static_cast<ComplexEntityLODUnit*>(e2.entity)->id));
      }
      else {
        e2Color = inputData->getElementColor()->getEdgeValue(edge(static_cast<ComplexEntityLODUnit*>(e2.entity)->id));
      }

      if (e1Color[3] == 255 && e2Color[3] == 255) {
        return e1.distance < e2.distance;
      }

      if (e1Color[3] == 255 && e2Color[3] < 255) {
        return true;
      }

      if (e2Color[3] == 255 && e1Color[3] < 255) {
        return false;
      }
    }

    // transparent objects ordering

    // if e2 bounding box is contained in e1 bounding box,
    // e2 must be drawn before e1
    if (e1.entity->boundingBox.contains(e2.entity->boundingBox)) {
      return false;
    }

    // if e1 bounding box is contained in e2 bounding box,
    // e1 must be drawn before e2
    if (e2.entity->boundingBox.contains(e1.entity->boundingBox)) {
      return true;
    }

    // if e1 is further from the viewer than e2,
    // e1 must be drawn before e2
    if(e1.distance>e2.distance)
      return true;

    // if e2 is further from the viewer than e1,
    // e2 must be drawn before e2
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
  NumericProperty *metric;
  bool operator() (pair<node,float> n1,pair<node,float> n2)  {
    return (metric->getNodeDoubleValue(n1.first) > metric->getNodeDoubleValue(n2.first));
  }
};
/** \brief Comparator to order entities with metric to display labels with metric ordering
 * Comparator to order entities with metric to display labels with metric ordering
 */
class GreatThanEdge {
public:
  NumericProperty *metric;
  bool operator() (pair<edge,float> e1,pair<edge,float> e2) {
    return (metric->getEdgeDoubleValue(e1.first) > metric->getEdgeDoubleValue(e2.first));
  }
};

//===================================================================
//
// GlGraphHighDetailsRenderer functions implementation
//
//===================================================================

GlGraphHighDetailsRenderer::GlGraphHighDetailsRenderer(const GlGraphInputData *inputData):GlGraphRenderer(inputData),lodCalculator(NULL),baseScene(NULL),fakeScene(new GlScene) {
  fakeScene->createLayer("fakeLayer");
}
//===================================================================
GlGraphHighDetailsRenderer::GlGraphHighDetailsRenderer(const GlGraphInputData *inputData,GlScene *scene):GlGraphRenderer(inputData),lodCalculator(NULL),baseScene(scene),fakeScene(new GlScene) {
  fakeScene->createLayer("fakeLayer");
}
//===================================================================
GlGraphHighDetailsRenderer::~GlGraphHighDetailsRenderer() {
  if(lodCalculator)
    lodCalculator->setAttachedLODCalculator(NULL);

  delete lodCalculator;

  delete fakeScene;
}
//===================================================================
void GlGraphHighDetailsRenderer::draw(float,Camera* camera) {

  if (!inputData->renderingParameters()->isAntialiased()) {
    OpenGlConfigManager::getInst().desactivateAntiAliasing();
  }

  Graph *graph=inputData->getGraph();

  // If we don't init lod calculator : clone the scene one
  if(!lodCalculator) {
    if(baseScene) {
      lodCalculator=baseScene->getCalculator()->clone();
      lodCalculator->setAttachedLODCalculator(baseScene->getCalculator());
    }
    else {
      lodCalculator=camera->getScene()->getCalculator()->clone();
      lodCalculator->setAttachedLODCalculator(camera->getScene()->getCalculator());
    }

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

  if (!selectionDrawActivate) {
    lodCalculator->compute(fakeScene->getViewport(), fakeScene->getViewport());
  }
  else {
    lodCalculator->compute(fakeScene->getViewport(), selectionViewport);
  }

  LayersLODVector &layersLODVector = lodCalculator->getResult();

  bool vertexArrayManagerActivated=inputData->getGlVertexArrayManager()->isActivated();

  if(vertexArrayManagerActivated) {
    // VertexArrayManager begin
    if(!selectionDrawActivate) {
      //inputData->getGlVertexArrayManager()->activate(true);
      inputData->getGlVertexArrayManager()->beginRendering();
      inputData->getGlGlyphRenderer()->startRendering();
    }
    else {
      inputData->getGlVertexArrayManager()->activate(false);
    }
  }

  // VertexArrayManager update
  if(inputData->getGlVertexArrayManager()->haveToCompute()) {
    GlVertexArrayVisitor vertexArrayVisitor(inputData);
    visitGraph(&vertexArrayVisitor,true);
    inputData->getGlVertexArrayManager()->setHaveToComputeAll(false);
  }

  BooleanProperty *filteringProperty = inputData->parameters->getDisplayFilteringProperty();
  NumericProperty *metric=inputData->parameters->getElementOrderingProperty();
  bool displayNodes = inputData->parameters->isDisplayNodes();
  bool displayMetaNodes = inputData->parameters->isDisplayMetaNodes();
  bool displayMetaNodesLabel = inputData->parameters->isViewMetaLabel();
  bool displayEdges = inputData->parameters->isDisplayEdges();

  GlNode glNode(0);
  GlEdge glEdge(0);

  bool renderOnlyOneNode=false;

  if(!selectionDrawActivate) {
    Iterator<node> *nonDefaultLayoutNodes = inputData->getElementLayout()->getNonDefaultValuatedNodes();
    Iterator<node> *nonDefaultSizeNodes = inputData->getElementSize()->getNonDefaultValuatedNodes();

    if(!nonDefaultLayoutNodes->hasNext() && !nonDefaultSizeNodes->hasNext())
      renderOnlyOneNode=true;

    delete nonDefaultLayoutNodes;
    delete nonDefaultSizeNodes;
  }

  if(!inputData->parameters->isElementZOrdered()) {

    vector<pair<node,float> > nodesMetricOrdered;
    vector<pair<edge,float> > edgesMetricOrdered;

    //draw nodes and metanodes
    for(vector<ComplexEntityLODUnit>::iterator it =layersLODVector[0].nodesLODVector.begin(); it!=layersLODVector[0].nodesLODVector.end(); ++it) {

      if(it->lod<=0)
        continue;

      if(filteringProperty) {
        if(filteringProperty->getNodeValue(node(it->id)))
          continue;
      }

      if(displayNodes || ((displayMetaNodes || displayMetaNodesLabel) && graph->isMetaNode(node(it->id)))) {
        if (!metric)  {
          if(selectionDrawActivate) {
            if((selectionType & RenderingNodes)==0)
              continue;

            (*selectionIdMap)[*selectionCurrentId]=SelectedEntity(graph,it->id,SelectedEntity::NODE_SELECTED);
            glLoadName(*selectionCurrentId);
            (*selectionCurrentId)++;
          }

          glNode.id=it->id;
          glNode.draw(it->lod,inputData,camera);
        }
        else {
          nodesMetricOrdered.push_back(make_pair(node(it->id), it->lod));
        }

        if(renderOnlyOneNode)
          break;
      }
      else {
        continue;
      }
    }

    if(metric) {
      // Draw nodes with metric ordering
      GreatThanNode ltn;
      ltn.metric=metric;
      sort(nodesMetricOrdered.begin(),nodesMetricOrdered.end(),ltn);

      for(vector<pair<node,float> >::iterator it=nodesMetricOrdered.begin(); it!=nodesMetricOrdered.end(); ++it) {
        if(selectionDrawActivate) {
          if((selectionType & RenderingNodes)==0)
            continue;

          (*selectionIdMap)[*selectionCurrentId]=SelectedEntity(graph,it->first.id,SelectedEntity::NODE_SELECTED);
          glLoadName(*selectionCurrentId);
          (*selectionCurrentId)++;
        }

        glNode.id=it->first.id;
        glNode.draw(it->second,inputData,camera);
      }
    }

    if(!renderOnlyOneNode) {
      //draw edges
      for(vector<ComplexEntityLODUnit>::iterator it = layersLODVector[0].edgesLODVector.begin(); it!=layersLODVector[0].edgesLODVector.end(); ++it) {
        if(it->lod<=0)
          continue;

        if(filteringProperty) {
          if(filteringProperty->getEdgeValue(edge(it->id)))
            continue;
        }

        if(!displayEdges)
          continue;

        if (!metric) {
          if(selectionDrawActivate) {
            if((selectionType & RenderingEdges)==0)
              continue;

            (*selectionIdMap)[*selectionCurrentId]=SelectedEntity(graph,it->id,SelectedEntity::EDGE_SELECTED);
            glLoadName(*selectionCurrentId);
            (*selectionCurrentId)++;
          }

          glEdge.id=it->id;
          glEdge.draw(it->lod,inputData,camera);
        }
        else {
          edgesMetricOrdered.push_back(make_pair(edge(it->id), it->lod));
        }
      }

      if (metric) {
        // Draw edges with metric ordering
        GreatThanEdge lte;
        lte.metric=metric;
        sort(edgesMetricOrdered.begin(),edgesMetricOrdered.end(),lte);

        for(vector<pair<edge,float> >::iterator it=edgesMetricOrdered.begin(); it!=edgesMetricOrdered.end(); ++it) {
          if(selectionDrawActivate) {
            if((selectionType & RenderingEdges)==0)
              continue;

            (*selectionIdMap)[*selectionCurrentId]=SelectedEntity(graph,it->first.id,SelectedEntity::EDGE_SELECTED);
            glLoadName(*selectionCurrentId);
            (*selectionCurrentId)++;
          }

          glEdge.id=it->first.id;
          glEdge.draw(it->second,inputData,camera);
        }
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

        if (it->lod<0)
          continue;

        if(filteringProperty) {
          if(filteringProperty->getNodeValue(node(it->id)))
            continue;
        }

        bb=it->boundingBox;
        Coord middle((bb[1]+bb[0])/2.f);
        dist=(((double)middle[0])-((double)camPos[0]))*(((double)middle[0])-((double)camPos[0]));
        dist+=(((double)middle[1])-((double)camPos[1]))*(((double)middle[1])-((double)camPos[1]));
        dist+=(((double)middle[2])-((double)camPos[2]))*(((double)middle[2])-((double)camPos[2]));
        entitiesSet.insert(EntityWithDistance(dist,&(*it),true));
      }
    }

    if(!selectionDrawActivate || (selectionDrawActivate && (selectionType & RenderingEdges)!=0)) {
      for(vector<ComplexEntityLODUnit>::iterator it=layersLODVector[0].edgesLODVector.begin(); it!=layersLODVector[0].edgesLODVector.end(); ++it) {
        if(it->lod<0)
          continue;

        if(filteringProperty) {
          if(filteringProperty->getEdgeValue(edge(it->id)))
            continue;
        }

        bb = it->boundingBox;
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
      ComplexEntityLODUnit *entity=(ComplexEntityLODUnit*)(it->entity);


      if(it->isNode) {

        if(displayNodes || ((displayMetaNodes || displayMetaNodesLabel) && graph->isMetaNode(node(entity->id)))) {

          // All opaque elements have been drawn, turn the depth buffer read-only
          // in order for a transparent object to not occlude another transparent object
          if (inputData->getElementColor()->getNodeValue(node(entity->id)).getA() < 255) {
            glDepthMask(GL_FALSE);
          }

          if(selectionDrawActivate) {
            if((selectionType & RenderingNodes)==0)
              continue;

            (*selectionIdMap)[*selectionCurrentId]=SelectedEntity(graph,entity->id,SelectedEntity::NODE_SELECTED);
            glLoadName(*selectionCurrentId);
            (*selectionCurrentId)++;
          }

          glNode.id=entity->id;
          glNode.draw(entity->lod,inputData,camera);

          if(renderOnlyOneNode)
            break;
        }

      }
      else {
        if(!displayEdges)
          continue;

        // All opaque elements have been drawn, turn the depth buffer read-only
        // in order for a transparent object to not occlude another transparent object
        if (inputData->getElementColor()->getEdgeValue(edge(entity->id)).getA() < 255) {
          glDepthMask(GL_FALSE);
        }

        if(selectionDrawActivate) {
          if((selectionType & RenderingEdges)==0)
            continue;

          (*selectionIdMap)[*selectionCurrentId]=SelectedEntity(graph,entity->id,SelectedEntity::EDGE_SELECTED);
          glLoadName(*selectionCurrentId);
          (*selectionCurrentId)++;
        }

        glEdge.id=entity->id;
        glEdge.draw(entity->lod,inputData,camera);
      }
    }

    glDepthMask(GL_TRUE);

  }

  if(!selectionDrawActivate) {
    if(vertexArrayManagerActivated) {
      inputData->getGlVertexArrayManager()->endRendering();
      inputData->getGlGlyphRenderer()->endRendering();
    }
  }
  else {
    selectionDrawActivate=false;
    inputData->getGlVertexArrayManager()->activate(true);
    OpenGlConfigManager::getInst().activateAntiAliasing();
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

  OpenGlConfigManager::getInst().activateAntiAliasing();

}
//===================================================================
void GlGraphHighDetailsRenderer::selectEntities(Camera *camera,RenderingEntitiesFlag type, int x, int y, int w, int h, vector<SelectedEntity> &selectedEntities) {
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

  initSelectionRendering(type, x, y, w, h, idToEntity,id);

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
void GlGraphHighDetailsRenderer::initSelectionRendering(RenderingEntitiesFlag type, int x, int y, int w, int h, map<unsigned int, SelectedEntity> &idMap,unsigned int &currentId) {
  selectionType=type;
  selectionIdMap=&idMap;
  selectionCurrentId=&currentId;
  selectionDrawActivate=true;
  selectionViewport[0] = x;
  selectionViewport[1] = y;
  selectionViewport[2] = w;
  selectionViewport[3] = h;
}
//===================================================================
void GlGraphHighDetailsRenderer::drawLabelsForComplexEntities(bool drawSelected,OcclusionTest *occlusionTest,LayerLODUnit &layerLODUnit) {
  Graph *graph=inputData->getGraph();
  BooleanProperty *selectionProperty=inputData->getElementSelected();
  bool viewOutScreenLabel=inputData->parameters->isViewOutScreenLabel();
  NumericProperty *metric=inputData->parameters->getElementOrderingProperty();
  BooleanProperty *filteringProperty = inputData->parameters->getDisplayFilteringProperty();

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

  // Draw Labels for Nodes
  if(viewNodeLabel && !nodeLabelEmpty) {
    node n;

    for(vector<ComplexEntityLODUnit>::iterator it=layerLODUnit.nodesLODVector.begin(); it!=layerLODUnit.nodesLODVector.end(); ++it) {
      if(it->lod<0 && !viewOutScreenLabel)
        continue;

      if (it->lod < 10 && inputData->renderingParameters()->isLabelScaled())
        continue;

      float lod=it->lod;

      if(viewOutScreenLabel && lod<0)
        lod=-lod;

      n.id=it->id;

      if (filteringProperty && filteringProperty->getNodeValue(n)) {
        continue;
      }

      if(selectionProperty->getNodeValue(n)==drawSelected) {
        if(!metric) {
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

    if(metric) {
      // Draw selected nodes label with metric ordering
      GreatThanNode ltn;
      ltn.metric=metric;
      sort(nodesMetricOrdered.begin(),nodesMetricOrdered.end(),ltn);

      for(vector<pair<node,float> >::iterator it=nodesMetricOrdered.begin(); it!=nodesMetricOrdered.end(); ++it) {
        glNode.id=it->first.id;
        glNode.drawLabel(occlusionTest,inputData,it->second,(Camera *)(layerLODUnit.camera));
      }
    }
  }

  // Draw Labels for Edges
  if(inputData->parameters->isViewEdgeLabel() && !edgeLabelEmpty) {
    edge e;

    for(vector<ComplexEntityLODUnit>::iterator it=layerLODUnit.edgesLODVector.begin(); it!=layerLODUnit.edgesLODVector.end(); ++it) {
      if(it->lod<0 && !viewOutScreenLabel)
        continue;

      if (it->lod < 5)
        continue;

      e.id=it->id;

      if (filteringProperty && filteringProperty->getEdgeValue(e)) {
        continue;
      }

      if(selectionProperty->getEdgeValue(e) == drawSelected) {
        if(!metric) {
          // Not metric ordered
          glEdge.id=e.id;
          glEdge.drawLabel(occlusionTest,inputData,it->lod,(Camera *)(layerLODUnit.camera));
        }
        else {
          // Metric ordered
          edgesMetricOrdered.push_back(pair<edge,float>(e,it->lod));
        }
      }
    }

    // If not Metric ordered : a this point selected edges are draw
    if(metric) {
      // Draw selected edges label with metric ordering

      GreatThanEdge lte;
      lte.metric=metric;
      sort(edgesMetricOrdered.begin(),edgesMetricOrdered.end(),lte);

      for(vector<pair<edge,float> >::iterator it=edgesMetricOrdered.begin(); it!=edgesMetricOrdered.end(); ++it) {
        glEdge.id=it->first.id;
        glEdge.drawLabel(occlusionTest,inputData,it->second,(Camera *)(layerLODUnit.camera));
      }
    }
  }
}

}
