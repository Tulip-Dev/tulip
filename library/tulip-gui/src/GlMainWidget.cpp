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

#include <tulip/GlMainWidget.h>

#include <QGLPixelBuffer>
#include <QGLFramebufferObject>
#include <QGLFormat>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QWindow>
#endif
#include <tulip/TulipSettings.h>
#include <tulip/Graph.h>
#include <tulip/GlQuadTreeLODCalculator.h>
#include <tulip/GLInteractor.h>
#include <tulip/QGlPixelBufferManager.h>
#include <tulip/Interactor.h>
#include <tulip/View.h>
#include <tulip/Camera.h>
#include <tulip/OpenGlConfigManager.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlLayer.h>

using namespace std;

namespace tlp {

QGLWidget* GlMainWidget::firstQGLWidget=nullptr;
bool GlMainWidget::inRendering=false;

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

  static int maxSamples = -1;

  if (maxSamples < 0) {
    maxSamples = 0;
    GlMainWidget::getFirstQGLWidget()->makeCurrent();
    maxSamples = OpenGlConfigManager::instance().maxNumberOfSamples();
    GlMainWidget::getFirstQGLWidget()->doneCurrent();
  }

  tmpFormat.setSamples(maxSamples);

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
  QGLWidget(GlInit(), parent, getFirstQGLWidget()),scene(new GlQuadTreeLODCalculator),view(view),
  widthStored(0),heightStored(0), useFramebufferObject(false), glFrameBuf(nullptr), glFrameBuf2(nullptr),
  keepPointOfViewOnSubgraphChanging(false), advancedAntiAliasing(false) {
  assert(this->isValid());
  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true);
  grabGesture(Qt::PinchGesture);
  grabGesture(Qt::PanGesture);
  grabGesture(Qt::SwipeGesture);
}
//==================================================
GlMainWidget::~GlMainWidget() {
  delete glFrameBuf;
  delete glFrameBuf2;
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
void GlMainWidget::createFrameBuffers(int width, int height) {

  useFramebufferObject = advancedAntiAliasing && QGLFramebufferObject::hasOpenGLFramebufferBlit();

  if (useFramebufferObject && (!glFrameBuf || glFrameBuf->size().width()!=width || glFrameBuf->size().height()!=height)) {
    makeCurrent();
    deleteFrameBuffers();
    QGLFramebufferObjectFormat fboFormat;
    fboFormat.setAttachment(QGLFramebufferObject::CombinedDepthStencil);
    fboFormat.setSamples(OpenGlConfigManager::instance().maxNumberOfSamples());
    glFrameBuf=new QGLFramebufferObject(width,height, fboFormat);
    glFrameBuf2=new QGLFramebufferObject(width,height);
    useFramebufferObject=glFrameBuf->isValid();
    widthStored=width;
    heightStored=height;
  }

}
//==================================================
void GlMainWidget::deleteFrameBuffers() {
  delete glFrameBuf;
  glFrameBuf=nullptr;
  delete glFrameBuf2;
  glFrameBuf2=nullptr;
}

//==================================================
void GlMainWidget::render(RenderingOptions options,bool checkVisibility) {

  if ((isVisible() || !checkVisibility) && !inRendering) {

    assert(contentsRect().width()!=0 && contentsRect().height()!=0);
    //Begin rendering process
    inRendering=true;
    makeCurrent();

    //Get the content width and height
    int width  = contentsRect().width();
    int height = contentsRect().height();

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    width  *= this->windowHandle()->devicePixelRatio();
    height *= this->windowHandle()->devicePixelRatio();
#endif

    computeInteractors();

    createFrameBuffers(width,height);

    if (useFramebufferObject) {
      glFrameBuf->bind();
    }

    //Render the scene in the frame buffer.
    scene.draw();

    if (useFramebufferObject) {
      glFrameBuf->release();
      QGLFramebufferObject::blitFramebuffer(glFrameBuf2, QRect(0,0,width, height), glFrameBuf, QRect(0,0,width, height));
    }

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);

    if (useFramebufferObject) {
      QGLFramebufferObject::blitFramebuffer(0, QRect(0,0,width, height), glFrameBuf2, QRect(0,0,width, height));
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
  scene.setSceneNeedRedraw(false);
  render(SwapBuffers);
  emit viewRedrawn(this);
}
//==================================================
void GlMainWidget::draw(bool graphChanged) {
  render(RenderingOptions(RenderScene|SwapBuffers));
  emit viewDrawn(this,graphChanged);
}
//==================================================
void GlMainWidget::computeInteractors() {
  if(!view)
    return;

  GLInteractorComposite *interactor=dynamic_cast<GLInteractorComposite*>(view->currentInteractor());

  if(interactor == nullptr)
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

  deleteFrameBuffers();

  scene.setViewport(0,0,screenToViewport(width), screenToViewport(height));

  emit glResized(w,h);
}
//==================================================
void GlMainWidget::makeCurrent() {
  if (isVisible()) {
    QGLWidget::makeCurrent();
    int width = contentsRect().width();
    int height = contentsRect().height();
    scene.setViewport(0,0,screenToViewport(width),screenToViewport(height));
  }
}
//==================================================
bool GlMainWidget::pickGlEntities(const int x, const int y,
                                  const int width, const int height,
                                  std::vector<SelectedEntity> &pickedEntities,
                                  GlLayer* layer) {
  makeCurrent();
  return scene.selectEntities(RenderingGlEntities, screenToViewport(x), screenToViewport(contentsRect().height()-y),
                              screenToViewport(width), screenToViewport(height),
                              pickedEntities, layer);
}
//==================================================
bool GlMainWidget::pickGlEntities(const int x, const int y,
                                  std::vector <SelectedEntity> &pickedEntities,
                                  GlLayer* layer) {
  return pickGlEntities(x,y,2,2,pickedEntities,layer);
}
//==================================================
void GlMainWidget::pickNodesEdges(const int x, const int y,
                                  const int width , const int height,
                                  std::vector<SelectedEntity> &selectedNodes, std::vector<SelectedEntity> &selectedEdges,
                                  GlLayer* layer, bool pickNodes, bool pickEdges) {
  makeCurrent();

  if (pickNodes) {
    scene.selectEntities(RenderingNodes, screenToViewport(x), screenToViewport(y), screenToViewport(width), screenToViewport(height), selectedNodes, layer);
  }

  if (pickEdges) {
    scene.selectEntities(RenderingEdges, screenToViewport(x), screenToViewport(y), screenToViewport(width), screenToViewport(height), selectedEdges, layer);
  }
}
//=====================================================
bool GlMainWidget::pickNodesEdges(const int x, const int y, SelectedEntity &selectedEntity, GlLayer* layer, bool pickNodes, bool pickEdges) {
  makeCurrent();
  vector<SelectedEntity> selectedEntities;

  if(pickNodes && scene.selectEntities(RenderingNodes, screenToViewport(x-1), screenToViewport(y-1), screenToViewport(3), screenToViewport(3), selectedEntities, layer)) {
    selectedEntity=selectedEntities[0];
    return true;
  }

  if(pickEdges && scene.selectEntities(RenderingEdges, screenToViewport(x-1), screenToViewport(y-1), screenToViewport(3), screenToViewport(3), selectedEntities, layer)) {
    selectedEntity=selectedEntities[0];
    return true;
  }

  return false;
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
void GlMainWidget::createTexture(const std::string &textureName, int width, int height) {

  makeCurrent();
  scene.setViewport(0,0,width,height);
  scene.centerScene();

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

  GlTextureManager::instance()->addExternalTexture(textureName,textureId);
}

//=====================================================
void GlMainWidget::createPicture(const std::string &pictureName, int width, int height, bool center) {
  createPicture(width,height,center).save(pictureName.c_str());
}

//=====================================================
QImage GlMainWidget::createPicture(int width, int height,bool center) {

  QImage resultImage;

  GlMainWidget::getFirstQGLWidget()->makeCurrent();

  QGLFramebufferObject *frameBuf=nullptr;
  QGLFramebufferObject *frameBuf2=nullptr;

  QGLFramebufferObjectFormat fboFormat;
  fboFormat.setAttachment(QGLFramebufferObject::CombinedDepthStencil);
  fboFormat.setSamples(OpenGlConfigManager::instance().maxNumberOfSamples());
  frameBuf=new QGLFramebufferObject(width,height, fboFormat);
  frameBuf2=new QGLFramebufferObject(width,height);

  if (frameBuf->isValid() && frameBuf2->isValid()) {
    frameBuf->bind();

    int oldWidth=scene.getViewport()[2];
    int oldHeight=scene.getViewport()[3];
    vector<Camera> oldCameras;

    if(center) {
      for(GlLayer *layer : scene.getLayersList()) {
        if(!layer->useSharedCamera())
          oldCameras.push_back(*(layer->getCamera()));
      }
    }

    scene.setViewport(0,0,width,height);

    if(center)
      scene.centerScene();

    computeInteractors();
    scene.draw();
    drawInteractors();
    frameBuf->release();

    QGLFramebufferObject::blitFramebuffer(frameBuf2, QRect(0,0,width, height), frameBuf, QRect(0,0,width, height));

    resultImage=frameBuf2->toImage();

    scene.setViewport(0,0,oldWidth,oldHeight);

    if(center) {
      int i=0;

      for(GlLayer *layer : scene.getLayersList()) {
        if(!layer->useSharedCamera()) {
          Camera *camera=layer->getCamera();
          camera->setCenter(oldCameras[i].getCenter());
          camera->setEyes(oldCameras[i].getEyes());
          camera->setSceneRadius(oldCameras[i].getSceneRadius());
          camera->setUp(oldCameras[i].getUp());
          camera->setZoomFactor(oldCameras[i].getZoomFactor());
        }

        i++;
      }
    }

  }

  delete frameBuf;
  delete frameBuf2;

  //The QGLFramebufferObject returns the wrong image format QImage::Format_ARGB32_Premultiplied. We need to create an image from original data with the right format QImage::Format_ARGB32.
  //We need to clone the data as when the image var will be destroy at the end of the function it's data will be destroyed too and the newly created image object will have invalid data pointer.
  return QImage(resultImage.bits(),resultImage.width(),resultImage.height(),QImage::Format_ARGB32).convertToFormat(QImage::Format_RGB32);
}

void GlMainWidget::centerScene(bool graphChanged, float zf) {
  scene.centerScene();

  if (zf != 1)
    scene.getMainLayer()->getCamera()->setZoomFactor(zf);

  draw(graphChanged);
}

void GlMainWidget::emitGraphChanged() {
  emit graphChanged();
}


void GlMainWidget::setKeepScenePointOfViewOnSubgraphChanging(bool k) {
  keepPointOfViewOnSubgraphChanging = k;
}

bool GlMainWidget::keepScenePointOfViewOnSubgraphChanging() const {
  return keepPointOfViewOnSubgraphChanging;
}

}
