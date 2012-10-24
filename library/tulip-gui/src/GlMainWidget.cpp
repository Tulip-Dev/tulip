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
#include <tulip/GLInteractor.h>
#include <tulip/GlGraphComposite.h>

#include "tulip/QGlPixelBufferManager.h"
#include "tulip/Interactor.h"
#include <tulip/GlCompositeHierarchyManager.h>
#include "tulip/GlVertexArrayManager.h"
#include "tulip/ExtendedMetaNodeRenderer.h"

using namespace std;

namespace tlp {

QGLWidget* GlMainWidget::firstQGLWidget=NULL;
bool GlMainWidget::inRendering=false;

//==================================================
static void setRasterPosition(unsigned int x, unsigned int y) {
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
    assert(GlMainWidget::firstQGLWidget->isValid());
  }

  return GlMainWidget::firstQGLWidget;
}

void GlMainWidget::clearFirstQGLWidget() {
  if(GlMainWidget::firstQGLWidget)
    delete GlMainWidget::firstQGLWidget;
}

//==================================================
GlMainWidget::GlMainWidget(QWidget *parent,View *view):
  QGLWidget(GlInit(), parent, getFirstQGLWidget()),scene(new GlQuadTreeLODCalculator),view(view), useFramebufferObject(false), glFrameBuf(NULL) {
  assert(this->isValid());
  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true);
  grabGesture(Qt::PinchGesture);
  grabGesture(Qt::PanGesture);
  grabGesture(Qt::SwipeGesture);
  renderingStore=NULL;
#if defined(__APPLE__) && defined(QT_MAC_USE_COCOA)
  // This code is here to bug fix black screen problem on MACOSX with Qt 4.7
  // When we do the first redraw we don't use frame backup but we draw the view
  // We have to test with next version of Qt to check if the problem exist
  // (It seems to no exist when Qt is built with Carbon)
  renderingNumber=0;
#if (QT_VERSION > QT_VERSION_CHECK(4, 7, 1))
#warning Qt fix must be tested with this version of Qt, see GlMainWidget.cpp l.106
#endif
#endif
}
//==================================================
GlMainWidget::~GlMainWidget() {
  delete glFrameBuf;
  delete [] renderingStore;
}
//==================================================
void GlMainWidget::paintEvent( QPaintEvent*) {
  QRegion rect = this->visibleRegion();

  //If the visible are changed we need to draw the entire scene
  //Because the saved snapshot only backup the visible part of the
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
  assert(context()->isValid());
  makeCurrent();
}
//==================================================
void GlMainWidget::createRenderingStore(int width, int height) {

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
void GlMainWidget::render(RenderingOptions options,bool checkVisibility) {
  if ((isVisible() || !checkVisibility) && !inRendering) {

    assert(contentsRect().width()!=0 && contentsRect().height()!=0);
    //Begin rendering process
    inRendering=true;
    makeCurrent();

    //Get the content width and height
    int width = contentsRect().width();
    int height = contentsRect().height();

    //If the rendering store is not valid need to regenerate new one force the RenderGraph flag.
    if(widthStored!=width || heightStored!=height) {
      options |= RenderScene;
    }

    computeInteractors();

    if(options.testFlag(RenderScene)) {
      createRenderingStore(width,height);

      widthStored=width;
      heightStored=height;

      //Render the graph in the back buffer.
      scene.draw();
    }
    else {
      scene.initGlParameters();
    }

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);


    if(options.testFlag(RenderScene)) {
      //Copy the back buffer (containing the graph render) in the rendering store to reuse it later.
      glReadBuffer(GL_BACK);
      glReadPixels(0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,renderingStore);
      glFlush();
    }
    else {
      //Copy the rendering store into the back buffer : restore the last graph render.
      glDrawBuffer(GL_BACK);
      setRasterPosition(0,0);
      glDrawPixels(width,height,GL_RGBA,GL_UNSIGNED_BYTE,renderingStore);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);

    //Draw interactors and foreground entities.
    drawInteractors();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_STENCIL_TEST);

    if(options.testFlag(SwapBuffers)) {
      swapBuffers();
    }

    inRendering=false;
  }
}
//==================================================
void GlMainWidget::redraw() {

#ifdef __APPLE__

  if (isVisible() && !inRendering) {
    // This code is here to bug fix black screen problem on MACOSX with Qt 4.7
    // When we do the first redraw we don't use frame backup but we draw the view
    // We have to test with next version of Qt to check if the problem exist
    if(renderingNumber<=4) {
      return draw(false);
    }
  }

#endif
  render(SwapBuffers);
  emit viewRedrawn(this);
}
//==================================================
void GlMainWidget::draw(bool graphChanged) {
  render(RenderingOptions(RenderScene|SwapBuffers));
#ifdef __APPLE__

  // This code is here to bug fix black screen problem on MACOSX with Qt 4.7
  // When we do the first redraw we don't use frame backup but we draw the view
  // We have to test with next version of Qt to check if the problem exist
  if (isVisible() && !inRendering) {
    if(renderingNumber<=4)
      renderingNumber++;
  }

#endif
  emit viewDrawn(this,graphChanged);
}
//==================================================
void GlMainWidget::computeInteractors() {
  if(!view)
    return;

  GLInteractorComposite *interactor=dynamic_cast<GLInteractorComposite*>(view->currentInteractor());

  if(interactor == NULL)
    return;

  interactor->compute(this);
}
//==================================================
void GlMainWidget::drawInteractors() {
  if(!view)
    return;

  GLInteractorComposite *interactor=dynamic_cast<GLInteractorComposite*>(view->currentInteractor());

  if(!interactor)
    return;

  interactor->draw(this);
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

  if (w == 0 || h == 0) {
    return ;
  }

  int width = contentsRect().width();
  int height = contentsRect().height();

  if(glFrameBuf) {
    delete glFrameBuf;
    glFrameBuf=NULL;
    delete[] renderingStore;
    renderingStore=NULL;
  }

  scene.setViewport(0,0,width,height);

  emit glResized(w,h);
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
bool GlMainWidget::pickGlEntities(const int x, const int y,
                                  const int width, const int height,
                                  std::vector<SelectedEntity> &pickedEntities,
                                  GlLayer* layer) {
  makeCurrent();
  return scene.selectEntities((RenderingEntitiesFlag)(RenderingSimpleEntities | RenderingWithoutRemove),x, y,
                              width, height,
                              layer,
                              pickedEntities);
}
//==================================================
bool GlMainWidget::pickGlEntities(const int x, const int y,
                                  std::vector <SelectedEntity> &pickedEntities,
                                  GlLayer* layer) {
  return pickGlEntities(x,y,2,2,pickedEntities,layer);
}
//==================================================
void GlMainWidget::pickNodesEdges(const int x, const int y,
                                  const int width ,const int height,
                                  std::vector<SelectedEntity> &selectedNodes, std::vector<SelectedEntity> &selectedEdges,
                                  GlLayer* layer) {
  makeCurrent();
  scene.selectEntities((RenderingEntitiesFlag)(RenderingNodes | RenderingWithoutRemove), x, y, width, height, layer, selectedNodes);
  scene.selectEntities((RenderingEntitiesFlag)(RenderingEdges | RenderingWithoutRemove), x, y, width, height, layer, selectedEdges);
}
//=====================================================
bool GlMainWidget::pickNodesEdges(const int x, const int y,SelectedEntity &selectedEntity, GlLayer* layer) {
  makeCurrent();
  vector<SelectedEntity> selectedEntities;

  if(scene.selectEntities((RenderingEntitiesFlag)(RenderingNodes | RenderingWithoutRemove), x-1, y-1, 3, 3, layer, selectedEntities)) {
    selectedEntity=selectedEntities[0];
    return true;
  }

  if(scene.selectEntities((RenderingEntitiesFlag)(RenderingEdges | RenderingWithoutRemove), x-1, y-1, 3, 3, layer, selectedEntities)) {
    selectedEntity=selectedEntities[0];
    return true;
  }

  return false;
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
void GlMainWidget::getTextureRealSize(int width, int height, int &textureRealWidth, int &textureRealHeight) {
  textureRealWidth=1;
  textureRealHeight=1;

  while(textureRealWidth<=width)
    textureRealWidth*=2;

  while(textureRealHeight<=height)
    textureRealHeight*=2;

  if(textureRealWidth>4096) {
    textureRealHeight=textureRealHeight/(textureRealWidth/8192);
    textureRealWidth=4096;
  }

  if(textureRealHeight>4096) {
    textureRealWidth=textureRealWidth/(textureRealHeight/8192);
    textureRealHeight=4096;
  }

}
//=====================================================
QGLFramebufferObject *GlMainWidget::createTexture(const std::string &textureName, int width, int height) {

  makeCurrent();
  scene.setViewport(0,0,width,height);
  scene.ajustSceneToSize(width,height);

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
void GlMainWidget::createPicture(const std::string &pictureName, int width, int height, bool center) {
  createPicture(width,height,center).save(pictureName.c_str());
}

//=====================================================
QImage GlMainWidget::createPicture(int width, int height,bool center) {

  int oldWidth=scene.getViewport()[2];
  int oldHeight=scene.getViewport()[3];
  vector<Camera> oldCameras;
  const vector<pair<string, GlLayer*> > &layersList=scene.getLayersList();

  if(center) {
    for(vector<pair<string, GlLayer*> >::const_iterator it=layersList.begin(); it!=layersList.end(); ++it) {
      if(!(*it).second->useSharedCamera())
        oldCameras.push_back((*it).second->getCamera());
    }
  }

  QImage resultImage;

  GlMainWidget::getFirstQGLWidget()->makeCurrent();
  scene.setViewport(0,0,width,height);

  if(center)
    scene.ajustSceneToSize(width,height);

  QGLPixelBuffer *glFrameBuf=QGlPixelBufferManager::getInst().getPixelBuffer(width,height);

  glFrameBuf->makeCurrent();

  computeInteractors();
  scene.draw();
  drawInteractors();

  resultImage=glFrameBuf->toImage();

  scene.setViewport(0,0,oldWidth,oldHeight);

  if(center) {
    int i=0;

    for(vector<pair<string, GlLayer*> >::const_iterator it=layersList.begin(); it!=layersList.end(); ++it) {
      if(!(*it).second->useSharedCamera()) {
        Camera &camera=(*it).second->getCamera();
        camera.setCenter(oldCameras[i].getCenter());
        camera.setEyes(oldCameras[i].getEyes());
        camera.setSceneRadius(oldCameras[i].getSceneRadius());
        camera.setUp(oldCameras[i].getUp());
        camera.setZoomFactor(oldCameras[i].getZoomFactor());
      }

      i++;
    }
  }

  //The QGlPixelBuffer returns the wrong image format QImage::Format_ARGB32_Premultiplied. We need to create an image from original data with the right format QImage::Format_ARGB32.
  //We need to clone the data as when the image var will be destroy at the end of the function it's data will be destroyed too and the newly created image object will have invalid data pointer.
  return QImage(resultImage.bits(),resultImage.width(),resultImage.height(),QImage::Format_ARGB32).convertToFormat(QImage::Format_RGB32);
}

void GlMainWidget::centerScene() {
  scene.centerScene();
  draw(false);
}

void GlMainWidget::emitGraphChanged() {
  emit graphChanged();
}

}
