/*
 */

#include <sstream>
#include "tulip/GlOffscreenRenderer.h"
#include "tulip/GlMainWidget.h"

#include <tulip/OpenGlConfigManager.h>
#include <tulip/GlVertexArrayManager.h>

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
: lastVPWidth(0), lastVPHeight(0), glFrameBuf(NULL), mainLayer(new GlLayer("Main")),
  entitiesCpt(0), zoomFactor(DBL_MAX), cameraCenter(FLT_MAX, FLT_MAX, FLT_MAX) {
	GlGraphComposite *glGraphComposite = new GlGraphComposite(tlp::newGraph());
	mainLayer->addGlEntity(glGraphComposite, "graph");
	GlLayer *backgroundLayer=new GlLayer("Background");
	backgroundLayer->setVisible(true);
	GlLayer *foregroundLayer=new GlLayer("Foreground");
	foregroundLayer->setVisible(true);
	backgroundLayer->set2DMode();
	foregroundLayer->set2DMode();
	scene.addLayer(backgroundLayer);
	scene.addLayer(mainLayer);
	scene.addLayer(foregroundLayer);
	scene.addGlGraphCompositeInfo(mainLayer, glGraphComposite);
}

GlOffscreenRenderer::~GlOffscreenRenderer() {
	if (glFrameBuf != NULL) {
		delete glFrameBuf;
	}
	clearScene();
	delete mainLayer;
}

void GlOffscreenRenderer::setViewPortSize(const unsigned int viewPortWidth, const unsigned int viewPortHeight) {
	if (glFrameBuf != NULL && (lastVPWidth != viewPortWidth || lastVPHeight != viewPortHeight)) {
		delete glFrameBuf;
		glFrameBuf = NULL;
	}
	if (glFrameBuf == NULL) {
		glFrameBuf = new QGLFramebufferObject(viewPortWidth, viewPortHeight, QGLFramebufferObject::CombinedDepthStencil);
	}
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
	scene.centerScene();
	if (centerScene) {
		scene.ajustSceneToSize(glFrameBuf->width(), glFrameBuf->height());
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

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopAttrib();
}

QImage GlOffscreenRenderer::getImage() {
	return glFrameBuf->toImage();
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
	glBindTexture(GL_TEXTURE_2D, glFrameBuf->texture());
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
