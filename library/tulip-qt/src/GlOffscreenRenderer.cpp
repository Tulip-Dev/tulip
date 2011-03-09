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

#include <sstream>

#include "tulip/GlOffscreenRenderer.h"
#include "tulip/GlMainWidget.h"
#include "tulip/GlVertexArrayManager.h"

using namespace std;

namespace tlp {

GlOffscreenRenderer *GlOffscreenRenderer::instance(NULL);

GlOffscreenRenderer *GlOffscreenRenderer::getInstance() {
	if (instance == NULL) {
		instance = new GlOffscreenRenderer();
	}
	return instance;
}

GlOffscreenRenderer::GlOffscreenRenderer()
: lastVPWidth(0), lastVPHeight(0), glFrameBuf(NULL), glFrameBuf2(NULL), mainLayer(new GlLayer("Main")),
  entitiesCpt(0), zoomFactor(DBL_MAX), cameraCenter(FLT_MAX, FLT_MAX, FLT_MAX) {
	GlLayer *backgroundLayer=new GlLayer("Background");
	backgroundLayer->setVisible(true);
	GlLayer *foregroundLayer=new GlLayer("Foreground");
	foregroundLayer->setVisible(true);
	backgroundLayer->set2DMode();
	foregroundLayer->set2DMode();
	scene.addLayer(backgroundLayer);
	scene.addLayer(mainLayer);
	scene.addLayer(foregroundLayer);
	antialiasedFbo = false;
#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0))
	antialiasedFbo = QGLFramebufferObject::hasOpenGLFramebufferBlit();
#endif
}

GlOffscreenRenderer::~GlOffscreenRenderer() {
	delete glFrameBuf;
	delete glFrameBuf2;
	clearScene();
	delete mainLayer;
}

void GlOffscreenRenderer::setViewPortSize(const unsigned int viewPortWidth, const unsigned int viewPortHeight) {
	if (glFrameBuf != NULL && (lastVPWidth != viewPortWidth || lastVPHeight != viewPortHeight)) {
		delete glFrameBuf;
		glFrameBuf = NULL;
		delete glFrameBuf2;
		glFrameBuf2 = NULL;
	}
	if (glFrameBuf == NULL) {
#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0))
		QGLFramebufferObjectFormat fboFmt;
		fboFmt.setAttachment(QGLFramebufferObject::CombinedDepthStencil);
		if (antialiasedFbo)
			fboFmt.setSamples(8);
		glFrameBuf = new QGLFramebufferObject(viewPortWidth, viewPortHeight, fboFmt);
	}
	if (antialiasedFbo && glFrameBuf2 == NULL) {
		glFrameBuf2 = new QGLFramebufferObject(viewPortWidth, viewPortHeight);
	}
#else
	glFrameBuf = new QGLFramebufferObject(viewPortWidth, viewPortHeight, QGLFramebufferObject::CombinedDepthStencil);
#endif
	scene.setViewport(0,0,viewPortWidth, viewPortHeight);
	lastVPWidth = viewPortWidth;
	lastVPHeight = viewPortHeight;
}

void GlOffscreenRenderer::setSceneBackgroundColor(const Color &color) {
	scene.setBackgroundColor(color);
}

unsigned int GlOffscreenRenderer::getViewportWidth() {
	return glFrameBuf->width();
}

unsigned int GlOffscreenRenderer::getViewportHeight() {
	return glFrameBuf->height();
}

void GlOffscreenRenderer::addGlEntityToScene(GlSimpleEntity *entity) {
	ostringstream oss;
	oss << "entity " << ++entitiesCpt;
	mainLayer->addGlEntity(entity, oss.str());
}

void GlOffscreenRenderer::addGraphToScene(Graph* graph){
	addGraphCompositeToScene(new GlGraphComposite(graph));
}

void GlOffscreenRenderer::addGraphCompositeToScene(GlGraphComposite *graphComposite) {
	//Delete old composite if it exist
	GlSimpleEntity *oldComposite = mainLayer->findGlEntity("graph");
	if(oldComposite!=NULL){
		mainLayer->deleteGlEntity(oldComposite);
	}
	GlVertexArrayManager *vertexArrayManager=graphComposite->getInputData()->getGlVertexArrayManager();
	vertexArrayManager->setHaveToComputeAll(true);
	mainLayer->addGlEntity(graphComposite, "graph");
	scene.addGlGraphCompositeInfo(mainLayer, graphComposite);
}

void GlOffscreenRenderer::clearScene() {
	mainLayer->getComposite()->reset(false);
	vector<pair<string, GlLayer *> > *layersList = scene.getLayersList();
	for (unsigned int i = 0 ; i < layersList->size() ; ++i) {
		if ((*layersList)[i].second != mainLayer) {
			(*layersList)[i].second->getComposite()->reset(true);
		}
	}
	entitiesCpt = 0;
	zoomFactor = DBL_MAX;
}

void GlOffscreenRenderer::renderScene(const bool centerScene) {

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	Camera &camera = scene.getCamera();
	glFrameBuf->bind();
	if (centerScene) {
		scene.centerScene();
	}
	if (cameraCenter != Coord(FLT_MAX, FLT_MAX, FLT_MAX)) {
		camera.setCenter(cameraCenter);
		camera.setEyes(Coord(0, 0, camera.getSceneRadius()));
		camera.setEyes(camera.getEyes() + camera.getCenter());
		camera.setUp(Coord(0, 1., 0));
	}
	if (zoomFactor != DBL_MAX) {
		camera.setZoomFactor(zoomFactor);
	}
	scene.draw();
	glFrameBuf->release();

#if (QT_VERSION >= QT_VERSION_CHECK(4, 6, 0))
	if (antialiasedFbo)
		QGLFramebufferObject::blitFramebuffer(glFrameBuf2, QRect(0,0,glFrameBuf2->width(), glFrameBuf2->height()), glFrameBuf, QRect(0,0,glFrameBuf->width(), glFrameBuf->height()));
#endif

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopAttrib();
}

QImage GlOffscreenRenderer::getImage() {
	if (!antialiasedFbo)
		return glFrameBuf->toImage();
	else
		return glFrameBuf2->toImage();
}

GLuint GlOffscreenRenderer::getGLTexture(const bool generateMipMaps) {

	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	if (generateMipMaps) {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	} else {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	unsigned char* buff = new unsigned char[getViewportWidth()*getViewportHeight()*4];
	if (!antialiasedFbo)
		glBindTexture(GL_TEXTURE_2D, glFrameBuf->texture());
	else
		glBindTexture(GL_TEXTURE_2D, glFrameBuf2->texture());
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buff);

	glBindTexture(GL_TEXTURE_2D, textureId);
	if (generateMipMaps) {
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, getViewportWidth(), getViewportHeight(), GL_RGBA, GL_UNSIGNED_BYTE, buff);
	} else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, getViewportWidth(), getViewportHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, buff);
	}

	delete [] buff;

	return textureId;

}

}
