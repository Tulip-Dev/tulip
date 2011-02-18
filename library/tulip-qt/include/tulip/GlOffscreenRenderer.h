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

#ifndef GLOFFSCREENRENDERER_H_
#define GLOFFSCREENRENDERER_H_

#include <tulip/GlScene.h>
#include <QtOpenGL/QGLFramebufferObject>

namespace tlp {

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

public :
        /**
          * @brief Get the renderer instance.
        **/
	static GlOffscreenRenderer *getInstance();

	~GlOffscreenRenderer();

        /**
          * @brief Define the viewport size.
          **/
	void setViewPortSize(const unsigned int viewPortWidth, const unsigned int viewPortHeight);
	unsigned int getViewportWidth();
	unsigned int getViewportHeight();
	bool frameBufferOk() const {return glFrameBuf->isValid();}

	GlScene *getScene() {return &scene;}
	void setZoomFactor(double zoomFactor) {this->zoomFactor = zoomFactor;}
	void setCameraCenter(const Coord &cameraCenter) {this->cameraCenter = cameraCenter;}

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
        void addGraphToScene(Graph* graph);

        /**
          * @brief Delete all the elements of the scene and clear it.
          **/
	void clearScene();

        /**
          * @brief Render the scene in a buffer. You need to call this function before getting the result with getImage or getGlTexture.
          **/
        void renderScene(const bool centerScene = true);

        /**
          * @brief Generate a QImage from the scene. You need to call the renderScene function before this function.
          **/
	QImage getImage();
        /**
          * @brief Generate an open gl texture from the scene. You need to call the renderScene function before this function.
          **/
	GLuint getGLTexture(const bool generateMipMaps = false);

private :

	GlOffscreenRenderer();

	static GlOffscreenRenderer *instance;

	unsigned int lastVPWidth, lastVPHeight;
	QGLFramebufferObject *glFrameBuf;
	GlScene scene;
	GlLayer *mainLayer;
	unsigned int entitiesCpt;
	double zoomFactor;
	Coord cameraCenter;

};


};


#endif /* GLOFFSCREENRENDERER_H_ */
