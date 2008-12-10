#include "GlProgressBar.h"
#include "ParallelTools.h"


#include <tulip/GlPolygon.h>
#include <tulip/GlLabel.h>
#include <tulip/GlQuad.h>
#include <tulip/TlpTools.h>

#include <vector>

using namespace std;

static const string PROGRESS_BAR_ID = "progress bar quad";
static const string COMMENT_ID = "comment label";
static const string PERCENT_ID = "percent label";

namespace tlp {

GlProgressBar::GlProgressBar(const Coord &centerPosition, const unsigned int width, const unsigned int height, const Color &color) :
	width(width), height(height), color(color), percentLabelAndFrameColor(color) {


	if (percentLabelAndFrameColor.getH() >= 180) {
		percentLabelAndFrameColor.setH(percentLabelAndFrameColor.getH() - 180);
	} else {
		percentLabelAndFrameColor.setH(percentLabelAndFrameColor.getH() + 180);
	}

	Coord globalFrameVertice1 = Coord(centerPosition.getX() - (width / 2.), centerPosition.getY() + (height/2.));
	Coord globalFrameVertice2 = Coord(centerPosition.getX() + (width / 2.), centerPosition.getY() + (height/2.));
	Coord globalFrameVertice3 = Coord(centerPosition.getX() + (width / 2.), centerPosition.getY() - (height/2.));
	Coord globalFrameVertice4 = Coord(centerPosition.getX() - (width / 2.), centerPosition.getY() - (height/2.));

	vector<Coord> globalFrameCoords;
	globalFrameCoords.push_back(globalFrameVertice1);
	globalFrameCoords.push_back(globalFrameVertice2);
	globalFrameCoords.push_back(globalFrameVertice3);
	globalFrameCoords.push_back(globalFrameVertice4);

	vector<Color> globalFrameColors;
	globalFrameColors.push_back(color);
	globalFrameColors.push_back(color);
	globalFrameColors.push_back(color);
	globalFrameColors.push_back(color);

	addGlEntity(new GlPolygon(globalFrameCoords, globalFrameColors, globalFrameColors, false, true), "global frame");

	progressBarMaxWidth = (4./5.) * width;
	progressBarHeight = (4./5.) * (height / 2.);
	commentWidth = (4./5.) * width;
	commentHeight = (4./5.) * (height / 2.);

	progressBarTLCorner = Coord(centerPosition.getX() - ((2./5.) * width), centerPosition.getY() - ((1./10.) * (height / 2.)), 0);
	commentLabelCenter = Coord(centerPosition.getX(), centerPosition.getY() + (height / 4.), 0);

	Coord progressBarFrameVertice1 = progressBarTLCorner + Coord(-2,2,0);
	Coord progressBarFrameVertice2 = progressBarFrameVertice1 + Coord(progressBarMaxWidth + 2, 0, 0);
	Coord progressBarFrameVertice3 = progressBarFrameVertice2 + Coord(0, -(progressBarHeight + 2), 0);
	Coord progressBarFrameVertice4 = progressBarFrameVertice3 + Coord(-(progressBarMaxWidth + 2), 0,0);

	vector<Coord> progressBarFrameCoords;
	progressBarFrameCoords.push_back(progressBarFrameVertice1);
	progressBarFrameCoords.push_back(progressBarFrameVertice2);
	progressBarFrameCoords.push_back(progressBarFrameVertice3);
	progressBarFrameCoords.push_back(progressBarFrameVertice4);

	vector<Color> progressBarFrameColors;
	progressBarFrameColors.push_back(percentLabelAndFrameColor);
	progressBarFrameColors.push_back(percentLabelAndFrameColor);
	progressBarFrameColors.push_back(percentLabelAndFrameColor);
	progressBarFrameColors.push_back(percentLabelAndFrameColor);

	addGlEntity(new GlPolygon(progressBarFrameCoords, progressBarFrameColors, progressBarFrameColors, false, true), "progress bar frame");
}

GlProgressBar::~GlProgressBar() {
	reset(true);
}

void GlProgressBar::setProgress(const int current, const int max) {

	currentPercent = (unsigned int) (((double) ((double) current / (double) max)) * 100.);

	GlSimpleEntity *previousBar = findGlEntity(PROGRESS_BAR_ID);
	GlSimpleEntity *previousComment = findGlEntity(COMMENT_ID);
	GlSimpleEntity *previousPercent = findGlEntity(PERCENT_ID);

	if (previousBar != NULL) {
		deleteGlEntity(previousBar);
		delete previousBar;
	}

	if (previousComment != NULL) {
		deleteGlEntity(previousComment);
		delete previousComment;
	}

	if (previousPercent != NULL) {
		deleteGlEntity(previousPercent);
		delete previousPercent;
	}

	float progressBarWidth =  (currentPercent * progressBarMaxWidth) / 100.;

	Coord progressBarCoords[4];
	progressBarCoords[0] = progressBarTLCorner;
	progressBarCoords[1] = progressBarCoords[0] + Coord(progressBarWidth, 0, 0);
	progressBarCoords[2] = progressBarCoords[1] + Coord(0, -progressBarHeight, 0);
	progressBarCoords[3] = progressBarCoords[2] + Coord(-progressBarWidth, 0, 0);
	GlQuad *progressBarQuad = new GlQuad(progressBarCoords, color);
	progressBarQuad->setTextureName(TulipBitmapDir + SLIDER_TEXTURE_NAME);

	GlLabel *commentLabel = new GlLabel(TulipBitmapDir, commentLabelCenter, Coord(commentWidth, commentHeight, 0), color);
	commentLabel->setText(comment);

	GlLabel *percentLabel = new GlLabel(TulipBitmapDir, Coord(progressBarTLCorner.getX() + (progressBarMaxWidth / 2.), progressBarTLCorner.getY() - (progressBarHeight / 2.), 0),
									    Coord(((1./10.) * progressBarMaxWidth), ((8./10.) * progressBarHeight), 0), percentLabelAndFrameColor);
	percentLabel->setText(getStringFromNumber(currentPercent) + " %");

	addGlEntity(progressBarQuad, PROGRESS_BAR_ID);
	addGlEntity(commentLabel, COMMENT_ID);
	addGlEntity(percentLabel, PERCENT_ID);
}

}
