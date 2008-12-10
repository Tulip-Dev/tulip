#include "ParallelAxis.h"

#include <tulip/GlLabel.h>
#include <tulip/GlBox.h>
#include <tulip/GlRect.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/TlpTools.h>

using namespace std;

namespace tlp {

static const float MAGIG_FACTOR = (1. / (1.3));

ParallelAxis::ParallelAxis(const Coord &baseCoord, const float height, const float axisAreaWidth, const string &name, const Color &axisColor) :
	 axisName(name), baseCoord(baseCoord), axisAreaWidth(axisAreaWidth), axisColor(axisColor), visible(true) {

	setAxisHeight(height);
	topSliderCoord = baseCoord + Coord(0, height);
	bottomSliderCoord = baseCoord;
	drawAxisLine();
	addCaption(name);
}

ParallelAxis::~ParallelAxis() {
	reset(true);
}

void ParallelAxis::setAxisHeight(const float height) {
	float resizeFactor = height / axisHeight;
	axisHeight = height;
	gradsWidth = DEFAULT_GRAD_WIDTH;
	gradsHeight = DEFAULT_GRAD_HEIGHT;

	bottomSliderCoord = baseCoord + Coord(0 ,(bottomSliderCoord.getY() - baseCoord.getY()) * resizeFactor);
	topSliderCoord = baseCoord + Coord(0 ,(topSliderCoord.getY() - baseCoord.getY()) * resizeFactor);
}

void ParallelAxis::drawAxisLine() {
	GlSimpleEntity * axis = new GlLine();
	((GlLine *)axis)->addPoint(baseCoord, axisColor);
	((GlLine *)axis)->addPoint(Coord(baseCoord.getX(), baseCoord.getY() + axisHeight), axisColor);
	axis->setStencil(1);
	addGlEntity(axis, axisName + " axis");
}

void
ParallelAxis::addLabelDrawing(const string &labelName, const float yCoord) {
	float xCoord = baseCoord.getX();

	GlSimpleEntity *line1 = new GlLine();
    ((GlLine *)line1)->addPoint(Coord(xCoord - (gradsWidth / 2), yCoord), axisColor);
    ((GlLine *)line1)->addPoint(Coord(xCoord + (gradsWidth / 2), yCoord), axisColor);

    line1->setStencil(1);
    addGlEntity(line1, "label line " + labelName);

    labelHeight = spaceBetweenAxisGrads * MAGIG_FACTOR;
    float labelWidth = labelName.length() * (labelHeight / 2);

    if (labelWidth > (axisAreaWidth / 4)) {
    	labelWidth = (axisAreaWidth / 4);
    }

    if (labelName.length() == 1) {
    	labelWidth *= 2;
    }

    GlLabel *axisGradLabel = new GlLabel(TulipBitmapDir, Coord(xCoord + gradsWidth, yCoord), Coord (labelWidth, labelHeight), axisColor);
    axisGradLabel->setText(labelName);
    float axisGradLabelWidth = axisGradLabel->getBoundingBox().second.getX() - axisGradLabel->getBoundingBox().first.getX();
    axisGradLabel->translate(Coord(axisGradLabelWidth / 2, 0, 0));
    axisGradLabel->setStencil(1);
    addGlEntity(axisGradLabel, "label text " + labelName);
}

void ParallelAxis::addCaption(const string &caption) {
	float captionWidth = axisAreaWidth * MAGIG_FACTOR;
	float captionHeight = axisAreaWidth / 6;
	float captionVOffset = (float) ((DEFAULT_CAPTION_VERTICAl_OFFSET * axisHeight) / (float) DEFAULT_AXIS_HEIGHT);

	GlLabel *axisCaption = new GlLabel(TulipBitmapDir, Coord(baseCoord.getX(), baseCoord.getY() - captionVOffset - (captionHeight / 2)), Coord (captionWidth, captionHeight), axisColor);
	axisCaption->setText(caption);
	addGlEntity(axisCaption, "caption " + caption);

	GlRect *axisCaptionInnerFrame = new GlRect(Coord(axisCaption->getBoundingBox().first.getX() - 1, axisCaption->getBoundingBox().second.getY() + 1),
											   Coord(axisCaption->getBoundingBox().second.getX() + 1, axisCaption->getBoundingBox().first.getY() - 1),
											   axisColor, axisColor, false, true);
	for (unsigned int i = 0 ; i < 4 ; ++i) {
		axisCaptionInnerFrame->ocolor(i) = axisColor;
	}
	addGlEntity(axisCaptionInnerFrame, "caption inner frame" + caption);

	GlRect *axisCaptionOuterFrame = new GlRect(Coord(axisCaption->getBoundingBox().first.getX() - 2, axisCaption->getBoundingBox().second.getY() + 2),
												   Coord(axisCaption->getBoundingBox().second.getX() + 2, axisCaption->getBoundingBox().first.getY() - 2),
												   axisColor, axisColor, false, true);
	for (unsigned int i = 0 ; i < 4 ; ++i) {
		axisCaptionOuterFrame->ocolor(i) = axisColor;
	}
	addGlEntity(axisCaptionOuterFrame, "caption outer frame" + caption);

	// just a way to make axis bounding box grow in Y direction, needed for correct work of axis sliders interactor
	GlLabel *emptyAxisCaption = new GlLabel(TulipBitmapDir, Coord(baseCoord.getX(), baseCoord.getY() + axisHeight + captionVOffset), Coord (captionWidth, captionHeight), axisColor);
	addGlEntity(emptyAxisCaption, "empty caption " + caption);
}

const Coord &
ParallelAxis::getBaseCoord() const {
	return baseCoord;
}

void
ParallelAxis::setBaseCoord(const Coord &newCoord){
	baseCoord = newCoord;
}

void ParallelAxis::translate(const Coord &c) {
	setBaseCoord(baseCoord + c);
	GlComposite::translate(c);
	topSliderCoord += c;
	bottomSliderCoord += c;
}

void ParallelAxis::computeBoundingBox() {
	GlBoundingBoxSceneVisitor glBBSV(NULL);
	acceptVisitor(&glBBSV);
	boundingBox = glBBSV.getBoundingBox();
}

void ParallelAxis::draw(float lod,Camera *camera) {
	list<GlSimpleEntity *>::iterator it;
	for (it = _sortedElements.begin() ; it != _sortedElements.end() ; ++it) {
		(*it)->draw(lod, camera);
	}
}

void ParallelAxis::resetSlidersPosition() {
	bottomSliderCoord = baseCoord;
	topSliderCoord = baseCoord + Coord(0, axisHeight);
}

}
