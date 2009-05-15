#include "tulip/QtMetaNodeRenderer.h"

#include <QtOpenGL/QGLFramebufferObject>

#include <tulip/GlGraphInputData.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlTools.h>

using namespace std;

namespace tlp {

  QtMetaNodeRenderer::QtMetaNodeRenderer(QWidget *parent, GlMainWidget *widget,GlGraphInputData *inputData):parent(parent),parentGlMainWidget(widget),inputData(inputData) {
    glMainWidget=NULL;
  }

  QtMetaNodeRenderer::~QtMetaNodeRenderer(){
    if(glMainWidget)
      delete glMainWidget;

    for(list<string>::iterator it=textureName.begin();it!=textureName.end();++it){
      GlTextureManager::getInst().deleteTexture(*it);
      //delete (*it).second;
    }

    clearObservers();
  }

  void QtMetaNodeRenderer::clearObservers(){
    for(map<Graph *,node>::iterator it=graphToMetaNode.begin();it!=graphToMetaNode.end();++it){
      (*it).first->removeObserver(this);
      (*it).first->removeGraphObserver(this);
    }

    for(map<Graph *,list<Graph *> >::iterator it=parentGraphToGraph.begin();it!=parentGraphToGraph.end();++it){
      (*it).first->removeGraphObserver(this);
    }

    for(multimap<PropertyInterface *,Graph *>::iterator it=propertyToGraph.begin();it!=propertyToGraph.end();++it){
      (*it).first->removeObserver(this);
      (*it).first->removePropertyObserver(this);
    }
  }

  void QtMetaNodeRenderer::prerender(node n,float lod,Camera *camera){
    if(lod<8.)
      return;

    lod=lod/2.;

    Graph *metaGraph=inputData->getGraph()->getNodeMetaInfo(n);
    if(graphToMetaNode.find(metaGraph)==graphToMetaNode.end()){
      metaGraph->addObserver(this);
      metaGraph->addGraphObserver(this);
      graphToMetaNode[metaGraph]=n;

      Graph *parentGraph=metaGraph;
      while(parentGraph!=parentGraph->getSuperGraph()){
        parentGraph=parentGraph->getSuperGraph();
        if(parentGraphToGraph.find(parentGraph)==parentGraphToGraph.end()){
          parentGraphToGraph[parentGraph]=list<Graph *>();
          parentGraph->addGraphObserver(this);
        }
        (*parentGraphToGraph.find(parentGraph)).second.push_back(metaGraph);
      }

      Iterator<std::string> *it=metaGraph->getProperties();
      while(it->hasNext()){
        string propertyName=it->next();
        PropertyInterface* property=metaGraph->getProperty(propertyName);
        propertyToGraph.insert(pair<PropertyInterface *,Graph*>(property,metaGraph));
        property->addPropertyObserver(this);
        property->addObserver(this);
      }
    }

    stringstream str;
    str << "metaNode" << n.id ;

    const Size &nodeSize = inputData->elementSize->getNodeValue(n);

    float diagonal=sqrt(nodeSize[0]*nodeSize[0]+nodeSize[1]*nodeSize[1]);
    int newWidth=(int)(nodeSize[0]*(lod/diagonal));
    int newHeight=(int)(nodeSize[1]*(lod/diagonal));
	
    if(newWidth<128)
      newWidth=128;
    if(newHeight<128)
      newHeight=128;

    int textureWidth,textureHeight;
    GlMainWidget::getTextureRealSize(newWidth,newHeight, textureWidth, textureHeight);

    bool render=true;

    if(metaNodeTextureSize.count(n)>0){
      if(textureWidth==metaNodeTextureSize[n].first && textureHeight==metaNodeTextureSize[n].second){
        render=false;
      }
    }
    if(haveToRenderGraph.find(metaGraph)!=haveToRenderGraph.end()){
      if(haveToRenderGraph[metaGraph]){
        render=true;
        haveToRenderGraph[metaGraph]=false;
      }
    }

    if(render){
      if(!glMainWidget){
	glMainWidget=new GlMainWidget(NULL,NULL);
        glMainWidget->getScene()->setBackgroundColor(Color(255,255,255,0));
      }
      //clear QGLFramebufferObject
      list<string>::iterator it=find(textureName.begin(),textureName.end(),str.str());
      if(it!=textureName.end()){
        GlTextureManager::getInst().deleteTexture(*it);
        //delete (*it).second;
        textureName.erase(it);
      }

      GlGraphRenderingParameters param=parentGlMainWidget->getScene()->getGlGraphComposite()->getRenderingParameters();
      Vector<int,4> viewport;
      glGetIntegerv(GL_VIEWPORT, (GLint *) &viewport[0]);
      QGLContext *context=(QGLContext *)(QGLContext::currentContext());

      Matrix<float, 4> modelviewMatrix;
      glGetFloatv (GL_MODELVIEW_MATRIX, (GLfloat*)&modelviewMatrix);
      Matrix<float, 4> projectionMatrix;
      glGetFloatv (GL_PROJECTION_MATRIX, (GLfloat*)&projectionMatrix);
	  
      //glMainWidget->makeCurrent();
      Graph *graph=inputData->getGraph();

      Graph *metaGraph = graph->getNodeMetaInfo(n);

      glMainWidget->setData(metaGraph,DataSet());
      glMainWidget->getScene()->getGlGraphComposite()->getInputData()->setMetaNodeRenderer(new QtMetaNodeRenderer(NULL,glMainWidget,glMainWidget->getScene()->getGlGraphComposite()->getInputData()));
      glMainWidget->getScene()->getGlGraphComposite()->setRenderingParameters(param);

      glMainWidget->createTexture(str.str(),textureWidth,textureHeight);
      textureName.push_back(str.str());

      glMatrixMode(GL_PROJECTION);
      glLoadMatrixf((GLfloat*)&projectionMatrix);
      glMatrixMode(GL_MODELVIEW);
      glLoadMatrixf((GLfloat*)&modelviewMatrix);

      context->makeCurrent();
      glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

      metaNodeTextureSize[n].first=textureWidth;
      metaNodeTextureSize[n].second=textureHeight;
    }
  }

  void QtMetaNodeRenderer::render(node n,float lod, Camera *camera){
    if(lod<8.)
      return;

    lod=lod/2.;

    stringstream str;
    str << "metaNode" << n.id;

    const Coord &nodePos = inputData->elementLayout->getNodeValue(n);
    const Size &nodeSize = inputData->elementSize->getNodeValue(n);

    BoundingBox includeBoundingBox;
    inputData->glyphs.get(inputData->elementShape->getNodeValue(n))->getIncludeBoundingBox(includeBoundingBox);

    Coord newCenter=nodePos+((includeBoundingBox.second-includeBoundingBox.first)/2.+includeBoundingBox.first-0.5)*nodeSize;
    //newCenter[2]+=nodeSize[2]/10.;
    Coord newSize=(includeBoundingBox.second-includeBoundingBox.first)*nodeSize;

    float diagonal=sqrt(nodeSize[0]*nodeSize[0]+nodeSize[1]*nodeSize[1]);
    int newWidth=(int)(nodeSize[0]*(lod/diagonal));
    int newHeight=(int)(nodeSize[1]*(lod/diagonal));  
	if(newWidth<128)
      newWidth=128;
    if(newHeight<128)
      newHeight=128;

    int textureWidth,textureHeight;
    float xTextureDec;
    float yTextureDec;

    Graph *metaGraph=inputData->getGraph()->getNodeMetaInfo(n);
    glMainWidget->setData(metaGraph,DataSet());
    GlMainWidget::getTextureRealSize(newWidth, newHeight, textureWidth,textureHeight);
	glMainWidget->getTextureShift(newWidth, newHeight, xTextureDec, yTextureDec);

    setMaterial(Color(255,255,255,255));
    GlTextureManager::getInst().activateTexture(str.str());

    glDisable(GL_LIGHTING);
    glBegin(GL_QUADS);
    glTexCoord2f(xTextureDec, yTextureDec);
    glVertex3f(newCenter[0]-newSize[0]/2.,newCenter[1]-newSize[1]/2.,newCenter[2]);
    glTexCoord2f(1.0-xTextureDec, yTextureDec);
    glVertex3f(newCenter[0]+newSize[0]/2.,newCenter[1]-newSize[1]/2.,newCenter[2]);
    glTexCoord2f(1.0-xTextureDec, 1.0-yTextureDec);
    glVertex3f(newCenter[0]+newSize[0]/2.,newCenter[1]+newSize[1]/2.,newCenter[2]);
    glTexCoord2f(xTextureDec, 1.0-yTextureDec);
    glVertex3f(newCenter[0]-newSize[0]/2.,newCenter[1]+newSize[1]/2.,newCenter[2]);
    glEnd();
    glEnable(GL_LIGHTING);

    GlTextureManager::getInst().desactivateTexture();
  }

}
