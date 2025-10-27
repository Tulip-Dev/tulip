/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
///@cond DOXYGEN_HIDDEN

#ifndef GLOFFSCREENRENDERER_H_
#define GLOFFSCREENRENDERER_H_

#include <tulip/tulipconf.h>
#include <tulip/Coord.h>
#include <tulip/GlScene.h>

#include <tulip/OpenGlIncludes.h>

#include <QImage>

class QOpenGLContext;
class QOffscreenSurface;
class QOpenGLFramebufferObject;

namespace tlp {

class GlSimpleEntity;
class GlGraphComposite;
class GlMainWidget;

/**
 * @brief Render a scene in an image or in a texture.
 *
 * Here is an example to render a graph in a QImage to use it as preview.
 * @code
 * //Get the renderer
 * glOffscreenRenderer *glOffscreenRenderer = GlOffscreenRenderer::getInstance();
 * //Define the viewport size. Needed to initialize the offscreen rederer.
 * glOffscreenRenderer->setViewPortSize(200,200);
 * //Erase old elements
 * glOffscreenRenderer->clearScene();
 * //Change the background color of the scene to white
 * glOffscreenRenderer->setSceneBackgroundColor(Color(255,255,255,255));
 * //Add
 * //Center and render the scene.
 * glOffscreenRenderer->renderScene(true);
 * //Get the result
 * QImage preview = glOffscreenRenderer->getGLTexture(true);
 * @endcode
 **/
class TLP_QT_SCOPE GlOffscreenRenderer {

public:
  /**
   * @brief Get the renderer instance.
   **/
  inline static GlOffscreenRenderer *getInstance() {
    return instance;
  }

  ~GlOffscreenRenderer();

  /**
   * @brief Define the viewport size.
   **/
  void setViewPortSize(const unsigned int viewPortWidth, const unsigned int viewPortHeight);
  unsigned int getViewportWidth();
  unsigned int getViewportHeight();

  GlScene *getScene() {
    return &scene;
  }
  void setZoomFactor(double zoomFactor) {
    this->zoomFactor = zoomFactor;
  }
  void setCameraCenter(const Coord &cameraCenter) {
    this->cameraCenter = cameraCenter;
  }

  void setSceneBackgroundColor(const Color &color);
  /**
   * @brief Add an entity to the scene. The scene become the owner of the object.
   **/
  void addGlEntityToScene(GlSimpleEntity *entity);
  /**
   * @brief Add a graph composite to the scene. The scene become the owner of the object.
   **/
  void addGraphCompositeToScene(GlGraphComposite *graphComposite);

  /**
   * @brief Add a graph to the scene. Just create a new GraphComposite and call GlGraphComposite.
   **/
  void addGraphToScene(Graph *graph);

  /**
   * @brief Delete all the elements of the scene and clear it.
   **/
  void clearScene(bool deleteGlEntities = false);

  /**
   * @brief Render the scene in a buffer. You need to call this function before getting the result
   *with getImage or getGlTexture.
   **/
  void renderScene(const bool centerScene = true, const bool antialiased = false);

  void renderExternalScene(GlScene *scene, const bool antialiased = false);

  QImage renderGlMainWidget(GlMainWidget *glWidget, bool redrawNeeded);

  /**
   * @brief Generate a QImage from the scene. You need to call the renderScene function before this
   *function.
   **/
  QImage getImage(bool alpha = false);
  /**
   * @brief Generate an open gl texture from the scene. You need to call the renderScene function
   *before this function.
   **/
  GLuint getGLTexture(const bool generateMipMaps = false);

  QOpenGLContext *getOpenGLContext();
  void makeOpenGLContextCurrent();
  void doneOpenGLContextCurrent();
  bool isValid();

private:
  GlOffscreenRenderer();

  void initFrameBuffers(const bool antialiased);

  static GlOffscreenRenderer *instance;

  QOpenGLContext *glContext;
  QOffscreenSurface *offscreenSurface;

  unsigned int vPWidth, vPHeight;
  QOpenGLFramebufferObject *glFrameBuf, *glFrameBuf2;
  GlScene scene;
  GlLayer *mainLayer;
  unsigned int entitiesCpt;
  double zoomFactor;
  Coord cameraCenter;
  bool antialiasedFbo;
};
} // namespace tlp

#endif /* GLOFFSCREENRENDERER_H_ */
///@endcond
