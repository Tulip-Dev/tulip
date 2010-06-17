/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qevent.h>
#include <QtGui/qimage.h>
#include <QtGui/qtooltip.h>
#include <QtOpenGL/QGLPixelBuffer>
#include <QtCore/QTime>
#include <QtGui/QPainter>

#include "tulip/GlMainWidget.h"

#include <tulip/Graph.h>
#include <tulip/Iterator.h>
#include <tulip/BooleanProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/GlTools.h>
#include <tulip/StlIterator.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTextureManager.h>
#include <tulip/Gl2DRect.h>
#include <tulip/GlQuadTreeLODCalculator.h>

#include "tulip/QGlPixelBufferManager.h"
#include "tulip/Interactor.h"
#include "tulip/InteractorManager.h"
#include "tulip/QtMetaNodeRenderer.h"
#include <tulip/GlCompositeHierarchyManager.h>

using namespace std;

namespace tlp {

#ifdef _WIN32
#ifdef DLL_EXPORT
  QGLWidget* GlMainWidget::firstQGLWidget=NULL;
#endif
#else
  QGLWidget* GlMainWidget::firstQGLWidget=NULL;
#endif

  // indicates if we can deal with GL_AUX* buffers
  bool _glAuxBufferAvailable = false;
  // This has to be done just before the first time we draw
  // to ensure a right QGLContext already exist
  static void checkIfGlAuxBufferAvailable() {
    static bool done = false;
    if (!done) {
      GLint res;
      glGetIntegerv(GL_AUX_BUFFERS, &res);
      tlp::glTest(__PRETTY_FUNCTION__);
      _glAuxBufferAvailable = (res > 0 );
      done = true;
#ifndef NDEBUG
      if (_glAuxBufferAvailable)
  cerr << "Auxillary Buffer Available" << endl;
      else
  cerr << "Auxillary Buffer Not Available" << endl;
#endif
    }
  }
  //==================================================
  static void setRasterPosition(unsigned int x, unsigned int y) {
    //  cerr << __PRETTY_FUNCTION__ << endl;
    float val[4];
    unsigned char tmp[10];
    glGetFloatv(GL_CURRENT_RASTER_POSITION, (float*)&val);
    glBitmap(0,0,0,0,-val[0] + x, -val[1] + y, tmp);
    glGetFloatv(GL_CURRENT_RASTER_POSITION, (float*)&val);
    tlp::glTest(__PRETTY_FUNCTION__);
  }
  //==================================================
  static QGLFormat GlInit() {
    QGLFormat tmpFormat = QGLFormat::defaultFormat();
    tmpFormat.setDirectRendering(true);
    tmpFormat.setDoubleBuffer(true);
    tmpFormat.setAccum(false);
    tmpFormat.setStencil(true);
    tmpFormat.setOverlay(false);
    tmpFormat.setDepth(true);
    tmpFormat.setRgba(true);
    tmpFormat.setAlpha(true);
    tmpFormat.setOverlay(false);
    tmpFormat.setStereo(false);
    return tmpFormat;
  }

  QGLWidget* GlMainWidget::getFirstQGLWidget() {
    if(!GlMainWidget::firstQGLWidget) {
      GlMainWidget::firstQGLWidget=new QGLWidget(GlInit());
    }
    return GlMainWidget::firstQGLWidget;
  }

  //==================================================
  GlMainWidget::GlMainWidget(QWidget *parent,AbstractView *view):
    QGLWidget(GlInit(), parent, getFirstQGLWidget()),scene(new GlQuadTreeLODCalculator),view(view), _hasHulls(false){
    //setObjectName(name);
    //  cerr << __PRETTY_FUNCTION__ << endl;
    setFocusPolicy(Qt::StrongFocus);
    //_cGlSphere(const Coord &position, float radius,const Color& color,float rotX,float rotY,float rotZ)omposite = new GlADComposite();
    renderingStore=NULL;
    connect(this,SIGNAL(viewDrawn(GlMainWidget*,bool)),this,SLOT(viewDrawnSlot(GlMainWidget *,bool)));
  }
  //==================================================
  GlMainWidget::~GlMainWidget() {
  }
  //==================================================
  void GlMainWidget::setData(Graph *graph,DataSet dataSet) {
    vector<pair<string, GlLayer*> >* layerList=scene.getLayersList();
    for(vector<pair<string, GlLayer *> >::iterator it=layerList->begin();it!=layerList->end();++it){
      delete (*it).second;
    }

    scene.clearLayersList();

    string sceneInput="";
    if(dataSet.exist("scene")) {
      dataSet.get("scene",sceneInput);
    }
    if(sceneInput=="") {
      //Default scene
      GlLayer* layer=new GlLayer("Main");
      GlLayer *backgroundLayer=new GlLayer("Background");
      backgroundLayer->setVisible(false);
      GlLayer *foregroundLayer=new GlLayer("Foreground");
      foregroundLayer->setVisible(true);

      backgroundLayer->set2DMode();
      foregroundLayer->set2DMode();
      string dir=TulipBitmapDir;
      Gl2DRect *background=new Gl2DRect(0,1.,0,1.,dir + "tex_back.png",true);
      backgroundLayer->addGlEntity(background,"background");

      Gl2DRect *labri=new Gl2DRect(5.,5.,50.,50.,dir + "logolabri.jpg",true,false);
      labri->setVisible(false);
      foregroundLayer->addGlEntity(labri,"labrilogo");

      scene.addLayer(backgroundLayer);
      scene.addLayer(layer);
      scene.addLayer(foregroundLayer);
      GlGraphComposite* graphComposite=new GlGraphComposite(graph);
      scene.addGlGraphCompositeInfo(scene.getLayer("Main"),graphComposite);
      scene.getLayer("Main")->addGlEntity(graphComposite,"graph");
      scene.centerScene();
    }else{
      size_t pos=sceneInput.find("TulipBitmapDir/");
      while(pos!=string::npos){
        sceneInput.replace(pos,15,TulipBitmapDir);
        pos=sceneInput.find("TulipBitmapDir/");
      }
      pos=sceneInput.find("TulipLibDir/");
      while(pos!=string::npos){
        sceneInput.replace(pos,12,TulipLibDir);
        pos=sceneInput.find("TulipLibDir/");
      }
      scene.setWithXML(sceneInput,graph);
    }
    if(dataSet.exist("Display")){
      DataSet renderingParameters;
      dataSet.get("Display",renderingParameters);
      GlGraphRenderingParameters rp=scene.getGlGraphComposite()->getRenderingParameters();
      rp.setParameters(renderingParameters);
      scene.getGlGraphComposite()->setRenderingParameters(rp);
    }
  }
  //==================================================
  DataSet GlMainWidget::getData() {
    DataSet outDataSet;
    outDataSet.set<DataSet>("Display",scene.getGlGraphComposite()->getRenderingParameters().getParameters());
    string out;
    scene.getXML(out);
    size_t pos=out.find(TulipBitmapDir);
    while(pos!=string::npos){
      out.replace(pos,TulipBitmapDir.size(),"TulipBitmapDir/");
      pos=out.find(TulipBitmapDir);
    }
    outDataSet.set<string>("scene",out);
    return outDataSet;
  }
  //==================================================
  void GlMainWidget::setGraph(Graph *graph){
    if(!scene.getLayer("Main")){
      setData(graph,DataSet());
      return;
    }
    if(_hasHulls)
			manager->setGraph(graph);

    GlGraphComposite* oldGraphComposite=(GlGraphComposite *)(scene.getLayer("Main")->findGlEntity("graph"));

    if(!oldGraphComposite){
      setData(graph,DataSet());
      return;
    }

    GlGraphRenderingParameters param=oldGraphComposite->getRenderingParameters();
    GlMetaNodeRenderer *metaNodeRenderer=oldGraphComposite->getInputData()->getMetaNodeRenderer();
    oldGraphComposite->getInputData()->setMetaNodeRenderer(NULL);
    GlGraphComposite* graphComposite=new GlGraphComposite(graph);
    graphComposite->setRenderingParameters(param);

    metaNodeRenderer->setInputData(graphComposite->getInputData());

    graphComposite->getInputData()->setMetaNodeRenderer(metaNodeRenderer);
    scene.addGlGraphCompositeInfo(scene.getLayer("Main"),graphComposite);
    scene.getLayer("Main")->addGlEntity(graphComposite,"graph");
    delete oldGraphComposite;
  }
  //==================================================
  Graph *GlMainWidget::getGraph() {
    return scene.getGlGraphComposite()->getInputData()->getGraph();
  }
  //==================================================
  void GlMainWidget::paintEvent( QPaintEvent*) {
    QRegion rect = this->visibleRegion();
    //If the visible are changed we need to draw the entire scene
    //Because the saved snapshot only backup the visibile part of the
    //Graph
    if (rect.boundingRect() != _visibleArea.boundingRect()) {
      _visibleArea = rect;
      draw();
    }
    else {
      redraw();
    }
    _visibleArea = rect; //Save the new visible area.
  }
  //==================================================
  void GlMainWidget::closeEvent(QCloseEvent *e) {
    emit closing(this, e);
  }
  //==================================================
  void GlMainWidget::setupOpenGlContext() {
    //  cerr << __PRETTY_FUNCTION__ << " (" << (int)this << ")" << endl;
    assert(context()->isValid());
    makeCurrent();
  }

  //==================================================
  void GlMainWidget::paintGL() {
    //  cerr << __PRETTY_FUNCTION__ << endl;
  }
  //==================================================
  void GlMainWidget::glInit() {
    //  cerr << __PRETTY_FUNCTION__ << endl;
  }
  //==================================================
  void GlMainWidget::glDraw() {
    //  cerr << __PRETTY_FUNCTION__ << endl;
  }
  //==================================================
  void GlMainWidget::updateGL() {
    //  cerr << __PRETTY_FUNCTION__ << endl;
  }
  //==================================================
  void GlMainWidget::initializeGL() {
    //  cerr << __PRETTY_FUNCTION__ << endl;
  }
  //==================================================
  void GlMainWidget::redraw() {
    if (isVisible()) {
      checkIfGlAuxBufferAvailable();
      
      makeCurrent();
      
      glDisable(GL_TEXTURE_2D);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_STENCIL_TEST);
      glDisable(GL_BLEND);
      glDisable(GL_LIGHTING);


      glDrawBuffer(GL_BACK);


      if (!_glAuxBufferAvailable) {
        glDrawPixels(width(),height(),GL_RGBA,GL_UNSIGNED_BYTE,renderingStore);
      }else{
        glReadBuffer(GL_AUX0);
        setRasterPosition(0,0);
        //Restore the graph image
        glCopyPixels(0,0,width(), height(), GL_COLOR);
      }
      drawInteractors();
      drawForegroundEntities();

      swapBuffers();

    }
    emit viewRedrawn(this);
  }
  //==================================================
  void GlMainWidget::draw(bool graphChanged) {
    if (isVisible()) {

      makeCurrent();

      checkIfGlAuxBufferAvailable();
      computeInteractors();
      scene.prerenderMetaNodes();
      scene.draw();

      glDisable(GL_TEXTURE_2D);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_STENCIL_TEST);
      glDisable(GL_BLEND);
      glDisable(GL_LIGHTING);

      //save the drawing of the graph in order to prevent redrawing during
      //Interactor draw
      if (_glAuxBufferAvailable) {
        glReadBuffer(GL_BACK);
        glDrawBuffer(GL_AUX0);
        glClear(GL_COLOR_BUFFER_BIT);
        setRasterPosition(0,0);
        glCopyPixels(0,0,width(), height(), GL_COLOR);
        glFlush();
        glDrawBuffer(GL_BACK);
      }else{
        glReadBuffer(GL_BACK);
        if(!renderingStore)
          renderingStore=new char[4*height()*width()];
        glReadPixels(0,0,width(),height(),GL_RGBA,GL_UNSIGNED_BYTE,renderingStore);
        glFlush();
      }
      glTest(__PRETTY_FUNCTION__);

      glEnable(GL_DEPTH_TEST);
      glEnable(GL_BLEND);
      glEnable(GL_LIGHTING);
      drawForegroundEntities();
      drawInteractors();

      swapBuffers();
    }
    emit viewDrawn(this,graphChanged);
  }
  //==================================================
  void GlMainWidget::computeInteractors() {
    if(!view)
      return;

    Interactor *interactor=view->getActiveInteractor();
    if(!interactor)
      return;

    interactor->compute(this);
  }
  //==================================================
  void GlMainWidget::drawInteractors() {
    if(!view)
      return;

    Interactor *interactor=view->getActiveInteractor();
    if(!interactor)
      return;

    interactor->draw(this);
  }
  //==================================================
  void GlMainWidget::drawForegroundEntities() {
    for(vector<ForegroundEntity *>::iterator it=foregroundEntity.begin();it!=foregroundEntity.end();++it) {
      (*it)->draw(this);
    }
  }
  //==================================================
  //QGLWidget
  //==================================================
  QImage GlMainWidget::grabFrameBuffer(bool withAlpha) {
    QImage img = QGLWidget::grabFrameBuffer(withAlpha);
    return img;
  }
  //==================================================
  //QGLWidget slots
  //==================================================
  void GlMainWidget::resizeGL(int w, int h) {
#ifndef NDEBUG
    cerr << __PRETTY_FUNCTION__ << endl;
#endif
    if (w == 0 || h == 0) {
      cerr << "warning: GlMainWidget::resizeGL(" << w << ", " << h << ")" <<  endl;
      return ;
    }
    delete renderingStore;
    renderingStore=new char[4*h*w];
    scene.setViewport(0,0,w,h);
  }
  //==================================================
  void GlMainWidget::makeCurrent() {
    QGLWidget::makeCurrent();
    GlDisplayListManager::getInst().changeContext((unsigned long)GlMainWidget::firstQGLWidget);
    GlTextureManager::getInst().changeContext((unsigned long)GlMainWidget::firstQGLWidget);
    QRect rect=contentsRect();
    scene.setViewport(0,0,rect.width(),rect.height());
  }
  //==================================================
  bool GlMainWidget::selectGlEntities(const int x, const int y,
              const int width, const int height,
              std::vector<GlEntity *> &pickedEntities,
              GlLayer* layer) {
    makeCurrent();
    vector<unsigned long> entities;
    unsigned int number=scene.selectEntities((RenderingEntitiesFlag)(RenderingSimpleEntities | RenderingWithoutRemove),x, y,
        width, height,
        layer,
        entities);
    for(vector<unsigned long>::iterator it=entities.begin();it!=entities.end();++it){
      pickedEntities.push_back((GlEntity*)(*it));
    }
    return number;
  }
  //==================================================
  bool GlMainWidget::selectGlEntities(const int x, const int y,
              std::vector <GlEntity *> &pickedEntities,
              GlLayer* layer) {
   return selectGlEntities(x,y,2,2,pickedEntities,layer);
  }
  //==================================================
  void GlMainWidget::doSelect(const int x, const int y,
            const int width ,const int height,
            vector<node> &sNode, vector<edge> &sEdge,
            GlLayer* layer) {
#ifndef NDEBUG
    cerr << __PRETTY_FUNCTION__ << " x:" << x << ", y:" <<y <<", wi:"<<width<<", height:" << height << endl;
#endif
    makeCurrent();
    vector<unsigned long> selectedElements;
    scene.selectEntities((RenderingEntitiesFlag)(RenderingNodes | RenderingWithoutRemove), x, y, width, height, layer, selectedElements);
    for(vector<unsigned long>::iterator it=selectedElements.begin();it!=selectedElements.end();++it) {
      sNode.push_back(node((unsigned int)(*it)));
    }
    selectedElements.clear();
    scene.selectEntities((RenderingEntitiesFlag)(RenderingEdges | RenderingWithoutRemove), x, y, width, height, layer, selectedElements);
    for(vector<unsigned long>::iterator it=selectedElements.begin();it!=selectedElements.end();++it) {
      sEdge.push_back(edge((unsigned int)(*it)));
    }
  }
  //=====================================================
  bool GlMainWidget::doSelect(const int x, const int y,tlp::ElementType &type ,node &n,edge &e, GlLayer* layer) {
#ifndef NDEBUG
    cerr << __PRETTY_FUNCTION__ << endl;
#endif
    makeCurrent();
    vector<unsigned long> selectedElements;
    scene.selectEntities((RenderingEntitiesFlag)(RenderingNodes | RenderingWithoutRemove), x-1, y-1, 3, 3, layer, selectedElements);
    if(selectedElements.size()!=0) {
      type=NODE;
      n=node((unsigned int)(selectedElements[0]));
      return true;
    }
    scene.selectEntities((RenderingEntitiesFlag)(RenderingEdges | RenderingWithoutRemove), x-1, y-1, 3, 3, layer, selectedElements);
    if(selectedElements.size()!=0) {
      type=EDGE;
      e=edge((unsigned int)(selectedElements[0]));
      return true;
    }
    return false;
  }
  //=====================================================
  unsigned char * GlMainWidget::getImage() {
    makeCurrent();
    return scene.getImage();
    //return GlGraph::getImage();
  }
  //=====================================================
  bool GlMainWidget::outputEPS(int size, int, const char *filename) {
    makeCurrent();
    scene.outputEPS(size, filename);
    return true;
  }
  //=====================================================
  bool GlMainWidget::outputSVG(int size, const char* filename) {
    makeCurrent();
    scene.outputSVG(size, filename);
    return true;
  }
  //=====================================================
  void GlMainWidget::getTextureRealSize(int width, int height, int &textureRealWidth, int &textureRealHeight){
    textureRealWidth=1;
    textureRealHeight=1;

    while(textureRealWidth<=width)
      textureRealWidth*=2;
    while(textureRealHeight<=height)
      textureRealHeight*=2;

    if(textureRealWidth>4096){
      textureRealHeight=textureRealHeight/(textureRealWidth/8192);
      textureRealWidth=4096;
    }
    if(textureRealHeight>4096){
      textureRealWidth=textureRealWidth/(textureRealHeight/8192);
      textureRealHeight=4096;
    }

  }
  //=====================================================
  void GlMainWidget::getTextureShift(int width, int height, float &xTextureShift, float &yTextureShift){
    int textureRealWidth;
    int textureRealHeight;

  getTextureRealSize(width,height,textureRealWidth,textureRealHeight);

    scene.computeAjustSceneToSize(textureRealWidth, textureRealHeight,NULL,NULL,NULL, &xTextureShift, &yTextureShift);
  }
  //=====================================================
  QGLFramebufferObject *GlMainWidget::createTexture(const string &textureName, int width, int height){
    scene.setViewport(0,0,width,height);
    scene.ajustSceneToSize(width,height);

    scene.prerenderMetaNodes();

    QGLPixelBuffer *glFrameBuf=QGlPixelBufferManager::getInst().getPixelBuffer(width,height);

    glFrameBuf->makeCurrent();

    GLuint textureId=glFrameBuf->generateDynamicTexture();

    scene.draw();

    glFrameBuf->updateDynamicTexture(textureId);

    GlTextureManager::getInst().registerExternalTexture(textureName,textureId);

    return NULL;
  }
  //=====================================================
  void GlMainWidget::createPicture(const string &pictureName, int width, int height,bool center){
#ifndef WITHOUT_QT_PICTURE_OUTPUT   
    scene.setViewport(0,0,width,height);
    if(center)
      scene.ajustSceneToSize(width,height);

    scene.prerenderMetaNodes();

    QGLPixelBuffer *glFrameBuf=QGlPixelBufferManager::getInst().getPixelBuffer(width,height);

    glFrameBuf->makeCurrent();

    scene.draw();

    glFrameBuf->toImage().save(pictureName.c_str());
#else
    makeCurrent();
    scene.setViewport(0,0,width,height);
    if(center)
      scene.ajustSceneToSize(width,height);
    scene.prerenderMetaNodes();

    unsigned char *image = (unsigned char *)malloc(width*height*3*sizeof(unsigned char));
    scene.draw();
    glFlush();
    glFinish();
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,image);

    QPixmap pm(width,height);
    QPainter painter;
    painter.begin(&pm);
    for (int y=0; y<height; y++) {
      for (int x=0; x<width; x++) {
        painter.setPen(QColor(image[(height-y-1)*width*3+(x)*3],
			      image[(height-y-1)*width*3+(x)*3+1],
			      image[(height-y-1)*width*3+(x)*3+2]));
	painter.drawPoint(x,y);
      }
    }
    painter.end();
    free(image);
    pm.save(pictureName.c_str());
#endif
  }
  
  void GlMainWidget::useHulls(bool hasHulls) {
		_hasHulls = hasHulls;
		if(_hasHulls)
			manager = new GlCompositeHierarchyManager(scene.getGlGraphComposite()->getInputData()->getGraph(), 
																							scene.getLayer("Main"), 
																							"Hulls", 
																							this->getScene()->getGlGraphComposite()->getInputData()->elementLayout, 
																							this->getScene()->getGlGraphComposite()->getInputData()->elementSize,
																							this->getScene()->getGlGraphComposite()->getInputData()->elementRotation);
	}
	
	bool GlMainWidget::hasHulls() const {
		return _hasHulls;
	}
}
