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
#include "tulip/GlMetaNodeTrueRenderer.h"

#include "tulip/OpenGlConfigManager.h"

#include <tulip/DrawingTools.h>
#include <tulip/BoundingBox.h>

#include "tulip/Camera.h"
#include "tulip/GlGraphInputData.h"
#include "tulip/GlCPULODCalculator.h"
#include "tulip/Glyph.h"
#include "tulip/GlNode.h"
#include "tulip/GlEdge.h"
#include "tulip/GlMetaNode.h"
#include "tulip/GlVertexArrayManager.h"

using namespace std;

//====================================================
#ifdef _WIN32
#ifdef DLL_EXPORT
unsigned int tlp::GlMetaNodeTrueRenderer::depth=0;
#endif
#else
unsigned int tlp::GlMetaNodeTrueRenderer::depth=0;
#endif

namespace tlp {

  GlMetaNodeTrueRenderer::GlMetaNodeTrueRenderer(GlGraphInputData *inputData):inputData(inputData){

  }

  void GlMetaNodeTrueRenderer::render(node n,float lod,Camera* camera){
    if(GlMetaNodeTrueRenderer::depth>=3)
      return;
    if(lod<10)
      return;

    GlGraphInputData *inputDataBackup=inputData;
    inputData->getGlVertexArrayManager()->pauseRendering(true);

    glPushMatrix();
    const Coord &nodeCoord = inputData->elementLayout->getNodeValue(n);
    const Size &nodeSize = inputData->elementSize->getNodeValue(n);
    glTranslatef(nodeCoord[0], nodeCoord[1], nodeCoord[2]);
    glRotatef(inputData->elementRotation->getNodeValue(n), 0., 0., 1.);
    // We remove nodeSize[N]/100. to be sure that includes nodes are in meta node

    Graph *metaGraph = inputData->getGraph()->getNodeMetaInfo(n);
    GlGraphRenderingParameters metaParameters = *inputData->parameters;
    GlGraphInputData metaData(metaGraph,&metaParameters);
    metaData.setMetaNodeRenderer(inputData->getMetaNodeRenderer());
    BoundingBox bboxes = tlp::computeBoundingBox(metaData.getGraph(), metaData.elementLayout, metaData.elementSize, metaData.elementRotation);
    Coord maxC(bboxes[1]);
    Coord minC(bboxes[0]);
    BoundingBox includeBoundingBox;
    inputData->glyphs.get(inputData->elementShape->getNodeValue(n))->getIncludeBoundingBox(includeBoundingBox,n);
    Coord includeScale(includeBoundingBox[1] - includeBoundingBox[0]);
    Coord size = (maxC + minC)/-1.f;
    Coord translate( (maxC+minC)/-2.f - (maxC-minC) + (includeBoundingBox[0]+Coord(.5,.5,.5))*((maxC-minC)*2.f) +(maxC-minC)*includeScale) ;
    double dept;
    if(includeScale[2]==0)
      dept=0;
    else
      dept= (maxC[2] - minC[2]) / includeScale[2];

    double width  = (maxC[0] - minC[0]) / includeScale[0];
    double height = (maxC[1] - minC[1]) / includeScale[1];

    Coord includeSize(bboxes[1] - bboxes[0]);
    if(nodeSize[0]/includeSize[0]<nodeSize[1]/includeSize[1]){
      includeSize[1]*=nodeSize[0]/includeSize[0];
      includeSize[0]*=nodeSize[0]/includeSize[0];
    }else{
      includeSize[0]*=nodeSize[1]/includeSize[1];
      includeSize[1]*=nodeSize[1]/includeSize[1];
    }
    if(includeSize[2]!=0)
      includeSize[2]*=nodeSize[2]/includeSize[2];

    glScalef(includeSize[0], includeSize[1], includeSize[2]);

    if (width<0.0001) width=1;
    if (height<0.0001) height=1;
    if (dept<0.0001) dept=1;
    Coord scale(1/width,1/height,1/dept);

    Camera *activeCamera;

    vector<Coord> objectScale, objectTranslate, objectCoord;
    activeCamera=new Camera(*camera);
    activeCamera->addObjectTransformation(nodeCoord+translate, Coord(includeSize*scale), nodeCoord);
    activeCamera->getObjectTransformation(objectTranslate, objectScale, objectCoord);


    GlCPULODCalculator calculator;
    calculator.setRenderingEntitiesFlag(RenderingAll);

    calculator.beginNewCamera(activeCamera);

    Iterator<node> *itN=metaGraph->getNodes();
    GlNode glNode(0);
    while (itN->hasNext()) {
      glNode.id=itN->next().id;
      BoundingBox bb = glNode.getBoundingBox(&metaData);

      Coord size(bb[1] - bb[0]);
      Coord middle(bb[0] + size/2.f);

      for(int i=objectScale.size()-1; i>=0;--i) {
        middle+=objectTranslate[i];
        middle=objectCoord[i] - (objectCoord[i]-middle)*objectScale[i];
        size*=objectScale[i];
      }

      bb[0]  = middle-size/2.f;
      bb[1]  = middle+size/2.f;
      calculator.addNodeBoundingBox(glNode.id,bb);
    }
    delete itN;

    if (metaData.parameters->isDisplayEdges()) {
      Iterator<edge> *itE=metaGraph->getEdges();
      GlEdge glEdge(0);
      while (itE->hasNext()) {
        glEdge.id=itE->next().id;
        BoundingBox bb = glEdge.getBoundingBox(&metaData);
        Coord size(bb[1] - bb[0]);
        Coord middle(bb[0] + (size)/2.f);

        for(int i=objectScale.size()-1; i>=0;--i) {
          middle+=objectTranslate[i];
          middle=objectCoord[i] - (objectCoord[i]-middle)*objectScale[i];
          size*=objectScale[i];
        }

        bb[0] = middle-size/2.f;
        bb[1] = middle+size/2.f;
        calculator.addEdgeBoundingBox(glEdge.id,bb);
      }
      delete itE;
    }

    calculator.compute(camera->getViewport(),camera->getViewport());

    LayersLODVector *layersLODVector=calculator.getResult();
    LayerLODUnit *layerLODUnit=&(layersLODVector->front());

    glPushMatrix();
    glScalef(scale[0],scale[1],scale[2]);
    glTranslatef(translate[0],translate[1],translate[2]);

    GlMetaNodeTrueRenderer::depth++;

    metaData.getMetaNodeRenderer()->setInputData(&metaData);
    metaData.getGlVertexArrayManager()->pauseRendering(true);
    GlMetaNode glMetaNode(0);
    GlEdge glEdge(0);
    for(vector<ComplexEntityLODUnit>::iterator it=layerLODUnit->nodesLODVector.begin();it!=layerLODUnit->nodesLODVector.end();++it){
      if((*it).lod<0)
        continue;

      metaData.getGlVertexArrayManager()->pauseRendering(true);
      if(!metaData.getGraph()->isMetaNode(node((*it).id))){
        glNode.id=(*it).id;
        glNode.draw((*it).lod,&metaData,activeCamera);
      }else{
        glMetaNode.id=(*it).id;
        glMetaNode.draw((*it).lod,&metaData,activeCamera);
      }
    }
    metaData.getGlVertexArrayManager()->pauseRendering(true);
    for(vector<ComplexEntityLODUnit>::iterator it=layerLODUnit->edgesLODVector.begin();it!=layerLODUnit->edgesLODVector.end();++it){
      if((*it).lod<0)
        continue;

      glEdge.id=(*it).id;
      glEdge.draw((*it).lod,&metaData,activeCamera);
    }
    metaData.getMetaNodeRenderer()->setInputData(inputDataBackup);

    inputData->getGlVertexArrayManager()->pauseRendering(false);

    GlMetaNodeTrueRenderer::depth--;

    glPopMatrix();
    glPopMatrix();

    delete activeCamera;

  }

}
