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
#include "tulip/GlTools.h"
#include "tulip/GlTextureManager.h"

#include <iostream>

using namespace std;

namespace tlp {

  static const Color colorSelect2 = Color(255, 102, 255, 255);

  void GlMetaNode::draw(float lod,GlGraphInputData* data,Camera* camera) {
    node n=node(id);

    if(!(((data->elementColor->getNodeValue(n))[3]==255) && (data->parameters->getNodesStencil()==0xFFFF)))
      data->getMetaNodeRenderer()->render(n,lod,camera);

    if(lod>=20)
      GlNode::draw(lod,data,camera);
    else
      GlNode::draw(20,data,camera);
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
    Coord size=(maxC + minC)/-1.f;
    Coord translate=(maxC+minC)/-2.f - (maxC-minC) + includeBoundingBox.first*((maxC-minC)*2.f) +(maxC-minC)*includeScale ;
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
