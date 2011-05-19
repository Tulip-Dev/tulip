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

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qimage.h>
#include <QtOpenGL/QGLPixelBuffer>
#include <QtOpenGL/QGLFramebufferObject>
#include <QtOpenGL/QGLFormat>

#ifdef ENABLE_RENDERING_TIME_DISPLAY
#include <omp.h>
#endif

#include "tulip/GlMainWidget.h"

#include <tulip/Graph.h>
#include <tulip/GlTools.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTextureManager.h>
#include <tulip/Gl2DRect.h>
#include <tulip/GlQuadTreeLODCalculator.h>

#include "tulip/QGlPixelBufferManager.h"
#include "tulip/Interactor.h"
#include <tulip/GlCompositeHierarchyManager.h>
#include "tulip/GlVertexArrayManager.h"

using namespace std;

namespace tlp {

QGLWidget* GlMainWidget::firstQGLWidget=NULL;
bool GlMainWidget::inRendering=false;

//==================================================
static void setRasterPosition(unsigned int x, unsigned int y) {
	//  std::cerr << __PRETTY_FUNCTION__ << std::endl;
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
	tmpFormat.setSampleBuffers(true);
	return tmpFormat;
}

QGLWidget* GlMainWidget::getFirstQGLWidget() {
	if(!GlMainWidget::firstQGLWidget) {
		GlMainWidget::firstQGLWidget=new QGLWidget(GlInit());
	}
	return GlMainWidget::firstQGLWidget;
}

//==================================================
GlMainWidget::GlMainWidget(QWidget *parent,View *view):
    								QGLWidget(GlInit(), parent, getFirstQGLWidget()),scene(new GlQuadTreeLODCalculator),view(view), _hasHulls(false), useFramebufferObject(false), glFrameBuf(NULL){
	setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);
	grabGesture(Qt::PinchGesture);
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::SwipeGesture);
	renderingStore=NULL;
#ifdef __APPLE__
	// This code is here to bug fix black screen problem on MACOSX with Qt 4.7
	// When we do the first redraw we don't use frame backup but we draw the view
	// We have to test with next version of Qt to check if the problem exist
	renderingNumber=0;
#if (QT_VERSION > QT_VERSION_CHECK(4, 7, 1))
#warning Qt fix must be tested with this version of Qt, see GlMainWidget.cpp l.106
#endif
#endif 
	connect(this,SIGNAL(viewDrawn(GlMainWidget*,bool)),this,SLOT(viewDrawnSlot(GlMainWidget *,bool)));
}
//==================================================
GlMainWidget::~GlMainWidget() {
	delete glFrameBuf;
	delete [] renderingStore;
}
//==================================================
void GlMainWidget::setData(Graph *graph,DataSet dataSet) {
	std::vector<std::pair<std::string, GlLayer*> >* layerList=scene.getLayersList();
	for(std::vector<std::pair<std::string, GlLayer *> >::iterator it=layerList->begin();it!=layerList->end();++it){
		delete (*it).second;
	}

	scene.clearLayersList();

	std::string sceneInput="";
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
		std::string dir=TulipBitmapDir;
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
		while(pos!=std::string::npos){
			sceneInput.replace(pos,15,TulipBitmapDir);
			pos=sceneInput.find("TulipBitmapDir/");
		}
		pos=sceneInput.find("TulipLibDir/");
		while(pos!=std::string::npos){
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
	if(dataSet.exist("Hulls")) {
		useHulls(true);
		DataSet hullsSet;
		dataSet.get<DataSet>("Hulls", hullsSet);
		manager->setVisible(true);
		manager->setData(hullsSet);
	}
}
//==================================================
DataSet GlMainWidget::getData() {
	DataSet outDataSet;
	outDataSet.set<DataSet>("Display",scene.getGlGraphComposite()->getRenderingParameters().getParameters());
	std::string out;
	scene.getXML(out);
	size_t pos=out.find(TulipBitmapDir);
	while(pos!=std::string::npos){
		out.replace(pos,TulipBitmapDir.size(),"TulipBitmapDir/");
		pos=out.find(TulipBitmapDir);
	}
	outDataSet.set<std::string>("scene",out);
	if(_hasHulls && manager->isVisible()) {
		outDataSet.set<DataSet>("Hulls", manager->getData());
	}
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
	oldGraphComposite->getInputData()->setMetaNodeRenderer(NULL,false);
	GlGraphComposite* graphComposite=new GlGraphComposite(graph);
	graphComposite->setRenderingParameters(param);

	metaNodeRenderer->setInputData(graphComposite->getInputData());

	graphComposite->getInputData()->setMetaNodeRenderer(metaNodeRenderer);
	if(oldGraphComposite->getInputData()->graph==graph){
		oldGraphComposite->getInputData()->deleteGlVertexArrayManagerInDestructor(false);
		delete graphComposite->getInputData()->getGlVertexArrayManager();
		graphComposite->getInputData()->setGlVertexArrayManager(oldGraphComposite->getInputData()->getGlVertexArrayManager());
		graphComposite->getInputData()->getGlVertexArrayManager()->setInputData(graphComposite->getInputData());
	}
	scene.addGlGraphCompositeInfo(scene.getLayer("Main"),graphComposite);
	scene.getLayer("Main")->addGlEntity(graphComposite,"graph");
	delete oldGraphComposite;
}
//==================================================
Graph *GlMainWidget::getGraph() {
	if(scene.getGlGraphComposite()!=NULL)
		return scene.getGlGraphComposite()->getInputData()->getGraph();

	return NULL;
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
	//  std::cerr << __PRETTY_FUNCTION__ << " (" << (int)this << ")" << std::endl;
	assert(context()->isValid());
	makeCurrent();
}

//==================================================
void GlMainWidget::paintGL() {
	//  std::cerr << __PRETTY_FUNCTION__ << std::endl;
}
//==================================================
void GlMainWidget::glInit() {
	//  std::cerr << __PRETTY_FUNCTION__ << std::endl;
}
//==================================================
void GlMainWidget::glDraw() {
	//  std::cerr << __PRETTY_FUNCTION__ << std::endl;
}
//==================================================
void GlMainWidget::updateGL() {
	//  std::cerr << __PRETTY_FUNCTION__ << std::endl;
}
//==================================================
void GlMainWidget::initializeGL() {
	//  std::cerr << __PRETTY_FUNCTION__ << std::endl;
}
//==================================================
void GlMainWidget::createRenderingStore(int width, int height){

	if (useFramebufferObject && (!glFrameBuf || glFrameBuf->size().width()!=width || glFrameBuf->size().height()!=height)) {
		makeCurrent();
		delete glFrameBuf;
		glFrameBuf=new QGLFramebufferObject(width,height);
		useFramebufferObject=glFrameBuf->isValid();
	}

	if (!useFramebufferObject && (!renderingStore || width != widthStored || height != heightStored)) {
		delete [] renderingStore;
		renderingStore=new char[width*height*4];
	}
}
//==================================================
void GlMainWidget::redraw() {
	if (isVisible() && !inRendering) {

#ifdef __APPLE__
	  // This code is here to bug fix black screen problem on MACOSX with Qt 4.7
	  // When we do the first redraw we don't use frame backup but we draw the view
	  // We have to test with next version of Qt to check if the problem exist
	  if(renderingNumber<=4){
	     return draw(false);
	  }
#endif 
		int width = contentsRect().width();
		int height = contentsRect().height();

		if(widthStored!=width || heightStored!=height){
			draw(false);
			return;
		}

		inRendering=true;

		makeCurrent();

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_LIGHTING);

		//#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0))
		//		if (useFramebufferObject) {
		//			QGLFramebufferObject::blitFramebuffer(0, QRect(0,0,width, height), glFrameBuf, QRect(0,0,width, height));
		//		}
		//#endif

		if (!frameBufferStored) {
			glDrawBuffer(GL_BACK);
			setRasterPosition(0,0);
			glDrawPixels(width,height,GL_RGBA,GL_UNSIGNED_BYTE,renderingStore);
		}

		drawInteractors();
		drawForegroundEntities();

		swapBuffers();

		inRendering=false;
	}
	emit viewRedrawn(this);
}
//==================================================
void GlMainWidget::draw(bool graphChanged) {

	if (isVisible() && !inRendering) {
		assert(contentsRect().width()!=0 && contentsRect().height()!=0);

		inRendering=true;

		makeCurrent();

		int width = contentsRect().width();
		int height = contentsRect().height();

		//#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0))
		//		if(QGlBufferManager::getInst().canUseFramebufferObject() && QGLFramebufferObject::hasOpenGLFramebufferBlit()){
		//			useFramebufferObject = true;
		//		}
		//#endif

		createRenderingStore(width,height);

		widthStored=width;
		heightStored=height;

		computeInteractors();

#ifdef ENABLE_RENDERING_TIME_DISPLAY
		double beginTime=omp_get_wtime();
#endif

		scene.prerenderMetaNodes();
		scene.draw();

#ifdef ENABLE_RENDERING_TIME_DISPLAY
		cout << ">>> rendering time : " << (int)((omp_get_wtime()-beginTime)*1000) << " ms" << endl << endl;
#endif

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_LIGHTING);

		frameBufferStored=false;

		//#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0))
		//		if(useFramebufferObject){
		//			QGLFramebufferObject::blitFramebuffer(glFrameBuf, QRect(0,0,width, height), 0, QRect(0,0,width, height));
		//			frameBufferStored=true;
		//		}
		//#endif

		if (!frameBufferStored) {
			glReadBuffer(GL_BACK);
			glReadPixels(0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,renderingStore);
			glFlush();
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glEnable(GL_LIGHTING);
		drawForegroundEntities();
		drawInteractors();

		swapBuffers();
		inRendering=false;

#ifdef __APPLE__
		// This code is here to bug fix black screen problem on MACOSX with Qt 4.7
		// When we do the first redraw we don't use frame backup but we draw the view
		// We have to test with next version of Qt to check if the problem exist
		if(renderingNumber<=4)
		  renderingNumber++;
#endif 


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
	for(std::vector<ForegroundEntity *>::iterator it=foregroundEntity.begin();it!=foregroundEntity.end();++it) {
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
	std::cerr << __PRETTY_FUNCTION__ << std::endl;
#endif
	if (w == 0 || h == 0) {
		std::cerr << "warning: GlMainWidget::resizeGL(" << w << ", " << h << ")" <<  std::endl;
		return ;
	}

	int width = contentsRect().width();
	int height = contentsRect().height();

	if(glFrameBuf){
		delete glFrameBuf;
		glFrameBuf=NULL;
		delete[] renderingStore;
		renderingStore=NULL;
	}

	scene.setViewport(0,0,width,height);
}
//==================================================
void GlMainWidget::makeCurrent() {
	if (isVisible()) {
		QGLWidget::makeCurrent();
		GlDisplayListManager::getInst().changeContext((unsigned long)GlMainWidget::firstQGLWidget);
		GlTextureManager::getInst().changeContext((unsigned long)GlMainWidget::firstQGLWidget);
		QRect rect=contentsRect();
		scene.setViewport(0,0,rect.width(),rect.height());
	}
}
//==================================================
bool GlMainWidget::selectGlEntities(const int x, const int y,
		const int width, const int height,
		std::vector<GlSimpleEntity *> &pickedEntities,
		GlLayer* layer) {
	makeCurrent();
	std::vector<unsigned long> entities;
	unsigned int number=scene.selectEntities((RenderingEntitiesFlag)(RenderingSimpleEntities | RenderingWithoutRemove),x, y,
			width, height,
			layer,
			entities);
	for(std::vector<unsigned long>::iterator it=entities.begin();it!=entities.end();++it){
		pickedEntities.push_back((GlSimpleEntity*)(*it));
	}
	return number;
}
//==================================================
bool GlMainWidget::selectGlEntities(const int x, const int y,
		std::vector <GlSimpleEntity *> &pickedEntities,
		GlLayer* layer) {
	return selectGlEntities(x,y,2,2,pickedEntities,layer);
}
//==================================================
void GlMainWidget::doSelect(const int x, const int y,
		const int width ,const int height,
		std::vector<node> &sNode, std::vector<edge> &sEdge,
		GlLayer* layer) {
#ifndef NDEBUG
	std::cerr << __PRETTY_FUNCTION__ << " x:" << x << ", y:" <<y <<", wi:"<<width<<", height:" << height << std::endl;
#endif
	makeCurrent();
	std::vector<unsigned long> selectedElements;
	scene.selectEntities((RenderingEntitiesFlag)(RenderingNodes | RenderingWithoutRemove), x, y, width, height, layer, selectedElements);
	for(std::vector<unsigned long>::iterator it=selectedElements.begin();it!=selectedElements.end();++it) {
		sNode.push_back(node((unsigned int)(*it)));
	}
	selectedElements.clear();
	scene.selectEntities((RenderingEntitiesFlag)(RenderingEdges | RenderingWithoutRemove), x, y, width, height, layer, selectedElements);
	for(std::vector<unsigned long>::iterator it=selectedElements.begin();it!=selectedElements.end();++it) {
		sEdge.push_back(edge((unsigned int)(*it)));
	}
}
//=====================================================
bool GlMainWidget::doSelect(const int x, const int y,tlp::ElementType &type ,node &n,edge &e, GlLayer* layer) {
#ifndef NDEBUG
	std::cerr << __PRETTY_FUNCTION__ << std::endl;
#endif
	makeCurrent();
	std::vector<unsigned long> selectedElements;
	scene.selectEntities((RenderingEntitiesFlag)(RenderingNodes | RenderingWithoutRemove), x-1, y-1, 3, 3, layer, selectedElements);
	if(!selectedElements.empty()) {
		type=NODE;
		n=node((unsigned int)(selectedElements[0]));
		return true;
	}
	scene.selectEntities((RenderingEntitiesFlag)(RenderingEdges | RenderingWithoutRemove), x-1, y-1, 3, 3, layer, selectedElements);
	if(!selectedElements.empty()) {
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
QGLFramebufferObject *GlMainWidget::createTexture(const std::string &textureName, int width, int height){

	makeCurrent();
	scene.setViewport(0,0,width,height);
	scene.ajustSceneToSize(width,height);

	scene.prerenderMetaNodes();

	QGLFramebufferObject *glFrameBuf= QGlPixelBufferManager::getInst().getFramebufferObject(width,height);
	assert(glFrameBuf->size()==QSize(width,height));

	glFrameBuf->bind();
	scene.draw();
	glFrameBuf->release();

	GLuint textureId=0;
	glGenTextures(1,&textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	unsigned char* buff = new unsigned char[width*height*4];
	glBindTexture(GL_TEXTURE_2D, glFrameBuf->texture());
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buff);

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buff);

	delete [] buff;

	glFrameBuf->release();

	GlTextureManager::getInst().registerExternalTexture(textureName,textureId);

	return NULL;
}

//=====================================================
void GlMainWidget::createPicture(const std::string &pictureName, int width, int height,bool center, int zoom, int xDec, int yDec){
	createPicture(width,height,center,zoom,xDec,yDec).save(pictureName.c_str());
}

//=====================================================
QImage GlMainWidget::createPicture(int width, int height,bool center, int zoom, int xDec, int yDec){
#ifndef WITHOUT_QT_PICTURE_OUTPUT   
	scene.setViewport(0,0,width,height);
	if(center)
		scene.ajustSceneToSize(width,height);

	scene.setViewportZoom(zoom,xDec,yDec);

	scene.prerenderMetaNodes();

	QGLPixelBuffer *glFrameBuf=QGlPixelBufferManager::getInst().getPixelBuffer(width,height);

	glFrameBuf->makeCurrent();

	scene.draw();

	return glFrameBuf->toImage();
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
	return pm.toImage();
#endif
}

void GlMainWidget::useHulls(bool hasHulls) {
	if(hasHulls == _hasHulls)
		return;

	_hasHulls = hasHulls;
	if(_hasHulls) {
		manager = new GlCompositeHierarchyManager(scene.getGlGraphComposite()->getInputData()->getGraph(),
				scene.getLayer("Main"),
				"Hulls",
				this->getScene()->getGlGraphComposite()->getInputData()->getElementLayout(),
				this->getScene()->getGlGraphComposite()->getInputData()->getElementSize(),
				this->getScene()->getGlGraphComposite()->getInputData()->getElementRotation());
		// Now we remove and add GlGraphComposite to be sure of the order (first Hulls and after GraphComposite)
		// This code doesn't affect the behavior of tulip but the tlp file is modified
		scene.getLayer("Main")->deleteGlEntity(this->getScene()->getGlGraphComposite());
		scene.getLayer("Main")->addGlEntity(this->getScene()->getGlGraphComposite(),"graph");
	}
}

bool GlMainWidget::hasHulls() const {
	return _hasHulls;
}
}
