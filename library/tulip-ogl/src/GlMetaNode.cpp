#include "tulip/GlMetaNode.h"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <tulip/Graph.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>

#include "tulip/Glyph.h"
#include "tulip/GlCPULODCalculator.h"
#include "tulip/GlGraphInputData.h"
#include "tulip/DrawingTools.h"
#include "tulip/GlEdge.h"
#include "tulip/GlComplexeEntity.h"
#include "tulip/GlGraphRenderingParameters.h"
#include "tulip/Camera.h"
#include "tulip/GlPointManager.h"

#include <iostream>

using namespace std;

namespace tlp {

  static const Color colorSelect2 = Color(255, 102, 255, 255);

  void GlMetaNode::draw(float lod,GlGraphInputData* data,Camera* camera) {
    node n=node(id);
    if(((data->elementColor->getNodeValue(n))[3]==255) && (data->parameters->getNodesStencil()==0xFFFF)){
      GlNode::draw(lod,data,camera);
      return;
    }
    GlPointManager::getInst().endRendering();
    GlPointManager::getInst().beginRendering();
    glPushMatrix();
    const Coord &nodeCoord = data->elementLayout->getNodeValue(node(id));
    const Size &nodeSize = data->elementSize->getNodeValue(node(id));
    glTranslatef(nodeCoord[0], nodeCoord[1], nodeCoord[2]);
    glRotatef(data->elementRotation->getNodeValue(node(id)), 0., 0., 1.);
    // We remove nodeSize[N]/100. to be sure that includes nodes are in meta node
    glScalef(nodeSize[0]-nodeSize[0]/100., nodeSize[1]-nodeSize[1]/100., nodeSize[2]-nodeSize[2]/100.);

    Graph *metaGraph = data->getGraph()->getNodeMetaInfo(node(id));
    GlGraphRenderingParameters metaParameters = *data->parameters;
    GlGraphInputData metaData(metaGraph,&metaParameters);
    pair<Coord, Coord> bboxes = tlp::computeBoundingBox(metaData.getGraph(), metaData.elementLayout, metaData.elementSize, metaData.elementRotation);
    Coord maxC = bboxes.first;
    Coord minC = bboxes.second;
    BoundingBox includeBoundingBox;
    data->glyphs.get(data->elementShape->getNodeValue(n))->getIncludeBoundingBox(includeBoundingBox);
    Coord includeScale=includeBoundingBox.second-includeBoundingBox.first;
    Coord size=(maxC + minC)/-1.;
    Coord translate=(maxC+minC)/-2 - (maxC-minC) + includeBoundingBox.first*((maxC-minC)*2) +(maxC-minC)*includeScale ;
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
    activeCamera->addObjectTransformation(nodeCoord+translate,nodeSize*scale,nodeCoord);
    activeCamera->getObjectTransformation(objectTranslate,objectScale,objectCoord);


    GlCPULODCalculator calculator;

    calculator.beginNewCamera(activeCamera);

    Iterator<node> *itN=metaGraph->getNodes();
    GlNode glNode(0);
    while (itN->hasNext()) {
      glNode.id=itN->next().id;
      BoundingBox bb = glNode.getBoundingBox(&metaData);

      Coord size=(bb.second-bb.first);
      Coord middle=bb.first+size/2;

      for(int i=objectScale.size()-1; i>=0;--i) {
        middle+=objectTranslate[i];
        middle=objectCoord[i] - (objectCoord[i]-middle)*objectScale[i];
        size*=objectScale[i];
      }

      bb.first=middle-size/2;
      bb.second=middle+size/2;
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
        Coord middle=bb.first+(size)/2;

        for(int i=objectScale.size()-1; i>=0;--i) {
          middle+=objectTranslate[i];
          middle=objectCoord[i] - (objectCoord[i]-middle)*objectScale[i];
          size*=objectScale[i];
        }

        bb.first=middle-size/2;
        bb.second=middle+size/2;
        calculator.addEdgeBoundingBox(glEdge.id,bb);
      }
      delete itE;
    }

    calculator.compute(camera->getViewport(),camera->getViewport());

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

    GlNode::draw(lod,data,camera);
  }

  void GlMetaNode::drawLabel(bool drawSelect,OcclusionTest* test,TextRenderer* renderer,GlGraphInputData* data){
    node n=node(id);

    GlNode::drawLabel(drawSelect,test,renderer,data);

    if((data->elementColor->getNodeValue(n))[3]==255){
      return;
    }

    Graph *metaGraph = data->getGraph()->getNodeMetaInfo(n);
    GlGraphRenderingParameters metaParameters = *data->parameters;
    GlGraphInputData metaData(metaGraph,&metaParameters);

    vector<GlNode> nodes;
    vector<GlMetaNode> metaNodes;
    vector<GlEdge> edges;

    Iterator<node> *itN=metaGraph->getNodes();
    unsigned int id;
    while (itN->hasNext()) {
      id=itN->next().id;
      if(metaData.getGraph()->isMetaNode(node(id)))
	metaNodes.push_back(GlMetaNode(id));
      else
	nodes.push_back(GlNode(id));
    }
    delete itN;

    if (metaData.parameters->isDisplayEdges()) {
      Iterator<edge> *itE=metaGraph->getEdges();
      while (itE->hasNext()) {
	edges.push_back(GlEdge(itE->next().id));
      }
      delete itE;
    }

    glPushMatrix();
    const Coord &nodeCoord = data->elementLayout->getNodeValue(n);
    const Size &nodeSize = data->elementSize->getNodeValue(n);
    glTranslatef(nodeCoord[0], nodeCoord[1], nodeCoord[2]);
    glRotatef(data->elementRotation->getNodeValue(n), 0., 0., 1.);
    glScalef(nodeSize[0], nodeSize[1], nodeSize[2]);

    pair<Coord, Coord> bboxes = tlp::computeBoundingBox(metaData.getGraph(), metaData.elementLayout, metaData.elementSize, metaData.elementRotation);

    Coord maxC = bboxes.first;
    Coord minC = bboxes.second;
    BoundingBox includeBoundingBox;
    data->glyphs.get(data->elementShape->getNodeValue(n))->getIncludeBoundingBox(includeBoundingBox);
    Coord includeScale=includeBoundingBox.second-includeBoundingBox.first;
    Coord size=(maxC + minC)/-1.;
    Coord translate=(maxC+minC)/-2 - (maxC-minC) + includeBoundingBox.first*((maxC-minC)*2) +(maxC-minC)*includeScale ;
    double dept  = (maxC[2] - minC[2]) / includeScale[2];
    double width  = (maxC[0] - minC[0]) / includeScale[0];
    double height = (maxC[1] - minC[1]) / includeScale[1];
    if (width<0.0001) width=1;
    if (height<0.0001) height=1;
    if (dept<0.0001) dept=1;

    glScalef(1.0/width, 1.0/height, 1.0/dept);
    glTranslatef(translate[0],translate[1],translate[2]);

    for(vector<GlNode>::iterator it=nodes.begin();it!=nodes.end();++it) {
      (*it).drawLabel(drawSelect,test,renderer,&metaData);
    }

    for(vector<GlMetaNode>::iterator it=metaNodes.begin();it!=metaNodes.end();++it) {
      (*it).drawLabel(drawSelect,test,renderer,&metaData);
    }

    for(vector<GlEdge>::iterator it=edges.begin();it!=edges.end();++it) {
      (*it).drawLabel(drawSelect,test,renderer,&metaData);
    }

    glPopMatrix();

  }
}
