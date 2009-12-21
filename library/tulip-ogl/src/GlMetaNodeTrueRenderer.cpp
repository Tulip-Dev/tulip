#include "tulip/GlMetaNodeTrueRenderer.h"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <tulip/DrawingTools.h>
#include <tulip/BoundingBox.h>

#include "tulip/Camera.h"
#include "tulip/GlPointManager.h"
#include "tulip/GlGraphInputData.h"
#include "tulip/GlCPULODCalculator.h"
#include "tulip/Glyph.h"
#include "tulip/GlNode.h"
#include "tulip/GlEdge.h"
#include "tulip/GlMetaNode.h"

using namespace std;

namespace tlp {

  GlMetaNodeTrueRenderer::GlMetaNodeTrueRenderer(GlGraphInputData *inputData):inputData(inputData){

  }

  void GlMetaNodeTrueRenderer::render(node n,float lod,Camera* camera){
    GlPointManager::getInst().endRendering();
    GlPointManager::getInst().beginRendering();
    glPushMatrix();
    const Coord &nodeCoord = inputData->elementLayout->getNodeValue(n);
    const Size &nodeSize = inputData->elementSize->getNodeValue(n);
    glTranslatef(nodeCoord[0], nodeCoord[1], nodeCoord[2]);
    glRotatef(inputData->elementRotation->getNodeValue(n), 0., 0., 1.);
    // We remove nodeSize[N]/100. to be sure that includes nodes are in meta node
    glScalef(nodeSize[0], nodeSize[1], nodeSize[2]);

    Graph *metaGraph = inputData->getGraph()->getNodeMetaInfo(n);
    GlGraphRenderingParameters metaParameters = *inputData->parameters;
    GlGraphInputData metaData(metaGraph,&metaParameters);
    metaData.setMetaNodeRenderer(inputData->getMetaNodeRenderer());
    pair<Coord, Coord> bboxes = tlp::computeBoundingBox(metaData.getGraph(), metaData.elementLayout, metaData.elementSize, metaData.elementRotation);
    Coord maxC = bboxes.first;
    Coord minC = bboxes.second;
    BoundingBox includeBoundingBox;
    inputData->glyphs.get(inputData->elementShape->getNodeValue(n))->getIncludeBoundingBox(includeBoundingBox);
    Coord includeScale=includeBoundingBox.second-includeBoundingBox.first;
    Coord size=(maxC + minC)/-1.f;
    Coord translate=(maxC+minC)/-2.f - (maxC-minC) + includeBoundingBox.first*((maxC-minC)*2.f) +(maxC-minC)*includeScale ;
    double dept  = (maxC[2] - minC[2]) / includeScale[2];
    double width  = (maxC[0] - minC[0]) / includeScale[0];
    double height = (maxC[1] - minC[1]) / includeScale[1];
    if (width<0.0001) width=1;
    if (height<0.0001) height=1;
    if (dept<0.0001) dept=1;
    Coord scale(1/width,1/height,1/dept);

    Camera *activeCamera;

    vector<Coord> objectScale, objectTranslate, objectCoord;
    activeCamera=new Camera(*camera);
    activeCamera->addObjectTransformation(nodeCoord+translate, Coord(nodeSize*scale), nodeCoord);
    activeCamera->getObjectTransformation(objectTranslate, objectScale, objectCoord);


    GlCPULODCalculator calculator;

    calculator.beginNewCamera(activeCamera);

    Iterator<node> *itN=metaGraph->getNodes();
    GlNode glNode(0);
    while (itN->hasNext()) {
      glNode.id=itN->next().id;
      BoundingBox bb = glNode.getBoundingBox(&metaData);

      Coord size=(bb.second-bb.first);
      Coord middle=bb.first+size/2.f;

      for(int i=objectScale.size()-1; i>=0;--i) {
        middle+=objectTranslate[i];
        middle=objectCoord[i] - (objectCoord[i]-middle)*objectScale[i];
        size*=objectScale[i];
      }

      bb.first=middle-size/2.f;
      bb.second=middle+size/2.f;
      calculator.addNodeBoundingBox(glNode.id,bb);
    }
    delete itN;

    if (metaData.parameters->isDisplayEdges()) {
      Iterator<edge> *itE=metaGraph->getEdges();
      GlEdge glEdge(0);
      while (itE->hasNext()) {
        glEdge.id=itE->next().id;
        BoundingBox bb = glEdge.getBoundingBox(&metaData);
        Coord size=bb.second-bb.first;
        Coord middle=bb.first+(size)/2.f;

        for(int i=objectScale.size()-1; i>=0;--i) {
          middle+=objectTranslate[i];
          middle=objectCoord[i] - (objectCoord[i]-middle)*objectScale[i];
          size*=objectScale[i];
        }

        bb.first=middle-size/2.f;
        bb.second=middle+size/2.f;
        calculator.addEdgeBoundingBox(glEdge.id,bb);
      }
      delete itE;
    }

    calculator.compute(camera->getViewport(),camera->getViewport(),RenderingAll);

    ComplexLODResultVector* nodesResult=&((calculator.getResultForNodes())->front());
    ComplexLODResultVector* edgesResult=&((calculator.getResultForEdges())->front());

    glPushMatrix();
    glScalef(scale[0],scale[1],scale[2]);
    glTranslatef(translate[0],translate[1],translate[2]);

    GlMetaNode glMetaNode(0);
    GlEdge glEdge(0);
    for(ComplexLODResultVector::iterator it=nodesResult->begin();it!=nodesResult->end();++it){
      if(!metaData.getGraph()->isMetaNode(node((*it).first))){
        glNode.id=(*it).first;
        glNode.draw((*it).second,&metaData,activeCamera);
      }else{
        glMetaNode.id=(*it).first;
        glMetaNode.draw((*it).second,&metaData,activeCamera);
      }
    }
    for(ComplexLODResultVector::iterator it=edgesResult->begin();it!=edgesResult->end();++it){
      glEdge.id=(*it).first;
      glEdge.draw((*it).second,&metaData,activeCamera);
    }

    GlPointManager::getInst().endRendering();
    GlPointManager::getInst().beginRendering();

    glPopMatrix();
    glPopMatrix();

    delete activeCamera;

  }

}
