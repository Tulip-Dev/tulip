#include <tulip/GlTools.h>

#include "ParallelAxis.h"

using namespace std;

namespace tlp {

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

ParallelAxis::ParallelAxis(GlAxis *glAxis, const float axisAreaWidth, const float rotationAngle, const GlAxis::LabelPosition captionPosition) : glAxis(glAxis), axisAreaWidth(axisAreaWidth), slidersActivated(false), rotationAngle(rotationAngle), hidden(false) {
	glAxis->setStencil(1);
	glAxis->addCaption(captionPosition, 30, true, axisAreaWidth, glAxis->getAxisLength() / 18.);
	glAxis->updateAxis();
	BoundingBox axisBB = glAxis->getBoundingBox();
	emptyRect = new GlRect(Coord(axisBB.first.getX(), axisBB.second.getY() + glAxis->getAxisLength() / 10.), Coord(axisBB.second.getX(), axisBB.first.getY()- glAxis->getAxisLength() / 15.), Color(0,0,0,0), Color(0,0,0,0), true, false);
	enableTrickForSelection();
	glAxis->updateAxis();
	resetSlidersPosition();
}

ParallelAxis::~ParallelAxis() {
	delete glAxis;
}

void ParallelAxis::setAxisHeight(const float height) {
	float resizeFactor = height / getAxisHeight();
	glAxis->setAxisLength(height);
	Coord baseCoord = glAxis->getAxisBaseCoord();
	bottomSliderCoord = baseCoord + Coord(0 ,(bottomSliderCoord.getY() - baseCoord.getY()) * resizeFactor);
	topSliderCoord = baseCoord + Coord(0 ,(topSliderCoord.getY() - baseCoord.getY()) * resizeFactor);
}

void ParallelAxis::setRotationAngle(const float rotationAngle) {
	this->rotationAngle = rotationAngle;
}

void ParallelAxis::setCaptionPosition(const GlAxis::LabelPosition captionPosition) {
	glAxis->addCaption(captionPosition, 30, true, axisAreaWidth, glAxis->getAxisLength() / 18.);
	glAxis->updateAxis();
}

void ParallelAxis::translate(const Coord &c) {
	glAxis->translate(c);
	topSliderCoord += c;
	bottomSliderCoord += c;
}

void ParallelAxis::setBaseCoord(const Coord &baseCoord) {
	translate(baseCoord - glAxis->getAxisBaseCoord());
}

void ParallelAxis::draw(float lod,Camera *camera) {
	if (rotationAngle != 0) {
		glPushMatrix();
		glRotatef(rotationAngle, 0.0,0.0,1.0);
	}
	drawComposite(glAxis, lod, camera);
	if (rotationAngle != 0) {
		glPopMatrix();
	}
}

void ParallelAxis::redraw() {
	glAxis->updateAxis();
}

void ParallelAxis::resetSlidersPosition() {
	Coord baseCoord = glAxis->getAxisBaseCoord();
	bottomSliderCoord = baseCoord;
	topSliderCoord = baseCoord + Coord(0, getAxisHeight());
}

void ParallelAxis::enableTrickForSelection() {
	glAxis->addGlEntity(emptyRect, "empty rect");
}

void ParallelAxis::disableTrickForSelection() {
	glAxis->deleteGlEntity(emptyRect);
}

BoundingBox ParallelAxis::getBoundingBox() {
	BoundingBox glAxisBB = glAxis->getBoundingBox();
	if (rotationAngle != 0) {
		Coord bbBL = glAxisBB.first;
		Coord bbTR = glAxisBB.second;
		Coord bbBR = Coord(glAxisBB.second.getX(), glAxisBB.first.getY());
		Coord bbTL = Coord(glAxisBB.first.getX(), glAxisBB.second.getY());
		rotateVector(bbBL, rotationAngle, Z_ROT);
		rotateVector(bbTR, rotationAngle, Z_ROT);
		rotateVector(bbBR, rotationAngle, Z_ROT);
		rotateVector(bbTL, rotationAngle, Z_ROT);
		glAxisBB = BoundingBox();
		glAxisBB.check(bbBL);
		glAxisBB.check(bbTR);
		glAxisBB.check(bbBR);
		glAxisBB.check(bbTL);
	}
	return glAxisBB;
}

Vector<Coord, 4> ParallelAxis::getBoundingPolygonCoords() const {
	Vector<Coord, 4> ret;
	BoundingBox glAxisBB = glAxis->getBoundingBox();
	Coord bbBL = glAxisBB.first;
	Coord bbTR = glAxisBB.second;
	Coord bbBR = Coord(glAxisBB.second.getX(), glAxisBB.first.getY());
	Coord bbTL = Coord(glAxisBB.first.getX(), glAxisBB.second.getY());
	if (rotationAngle != 0) {
		rotateVector(bbBL, rotationAngle, Z_ROT);
		rotateVector(bbTR, rotationAngle, Z_ROT);
		rotateVector(bbBR, rotationAngle, Z_ROT);
		rotateVector(bbTL, rotationAngle, Z_ROT);
	}
	ret[0] = bbTL;
	ret[1] = bbTR;
	ret[2] = bbBR;
	ret[3] = bbBL;
	return ret;
}

Coord ParallelAxis::getBaseCoord() const {
	return glAxis->getAxisBaseCoord();
}

Coord ParallelAxis::getTopCoord() const {
	return glAxis->getAxisBaseCoord() + Coord(0, getAxisHeight(), 0);

}

}
