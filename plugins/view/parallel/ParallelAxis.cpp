#include "ParallelAxis.h"

#include <tulip/GlLabel.h>
#include <tulip/GlBox.h>
#include <tulip/GlRect.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/TlpTools.h>

using namespace std;

namespace tlp {

static const float MAGIG_FACTOR = (1. / (1.3));

void drawComposite(GlComposite *composite, float lod, Camera *camera) {
	map<string, GlSimpleEntity*> *glEntities = composite->getDisplays();
	map<string, GlSimpleEntity*>::iterator it2;
	for (it2 = glEntities->begin(); it2 != glEntities->end() ; ++it2) {
		GlSimpleEntity *entity = it2->second;
		GlComposite *compositeEntity = dynamic_cast<GlComposite *>(entity);
		if (compositeEntity != NULL) {
			drawComposite(compositeEntity, lod, camera);
		} else {
			entity->draw(lod, camera);
		}
	}
}

ParallelAxis::ParallelAxis(GlAxis *glAxis, const float axisAreaWidth) : glAxis(glAxis), slidersActivated(false), hidden(false) {
	glAxis->setStencil(1);
	glAxis->addCaption(GlAxis::LEFT_OR_BELOW, 30, true, axisAreaWidth, glAxis->getAxisLength() / 20.);
}

ParallelAxis::~ParallelAxis() {
	delete glAxis;
}

void ParallelAxis::setAxisHeight(const float height) {
	float resizeFactor = height / getAxisHeight();
	glAxis->setAxisLength(height);
	Coord baseCoord = getBaseCoord();
	bottomSliderCoord = baseCoord + Coord(0 ,(bottomSliderCoord.getY() - baseCoord.getY()) * resizeFactor);
	topSliderCoord = baseCoord + Coord(0 ,(topSliderCoord.getY() - baseCoord.getY()) * resizeFactor);
}

void ParallelAxis::translate(const Coord &c) {
	glAxis->translate(c);
	topSliderCoord += c;
	bottomSliderCoord += c;
}

void ParallelAxis::draw(float lod,Camera *camera) {
	drawComposite(glAxis, lod, camera);
}

void ParallelAxis::redraw() {
	glAxis->updateAxis();
}

void ParallelAxis::resetSlidersPosition() {
	Coord baseCoord = getBaseCoord();
	bottomSliderCoord = baseCoord;
	topSliderCoord = baseCoord + Coord(0, getAxisHeight());
}

}
